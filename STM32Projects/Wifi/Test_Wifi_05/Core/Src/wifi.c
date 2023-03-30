#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"


uint8_t aRXBufferUser[RX_BUFFER_SIZE];
uint8_t aRXBufferA[RX_BUFFER_SIZE];
uint8_t aRXBufferB[RX_BUFFER_SIZE];
__IO uint32_t     uwNbReceivedChars;
uint8_t *pBufferReadyForUser;
uint8_t *pBufferReadyForReception;
uint8_t *SSID = "Reseau du KGB";

uint8_t at[] = "\r\nAT\r\n";
uint8_t apConnect[] = "\r\nAT+CWJAP=\"Reseau du KGB\",\"12345678\"\r\n";
uint8_t apConnectAlready[] = "\r\nAT+CWJAP?\r\n";
uint8_t cwMode[] = "\r\nAT+CWMODE=1\r\n";
uint8_t cipStart[] = "\r\nAT+CIPSTART=\"TCP\",\"tcpbin.com\",4242\r\n";
uint8_t cipClose[] = "\r\nAT+CIPCLOSE\r\n";
uint8_t SendDataSize[] = "\r\nAT+CIPSEND=4\r\n";
uint8_t SendData[] = "\r\nTest\r\n";

/**
  * @brief  Send Txt information message on UART Tx line (to PC Com port).
  * @param  huart UART handle.
  * @param  String String to be sent to user display
  * @param  Size   Size of string
  * @retval None
  */
void PrintInfo(UART_HandleTypeDef *huart, uint8_t *String, uint16_t Size)
{
  if (HAL_OK != HAL_UART_Transmit(huart, String, Size,1000))
  {
    Error_Handler();
  } else { printf("Command AT sent : %s\n",String); }
}

/**
  * @brief  This function prints user info on PC com port and initiates RX transfer
  * @retval None
  */
void StartReception(void)
{
  /* Initializes Buffer swap mechanism (used in User callback) :
     - 2 physical buffers aRXBufferA and aRXBufferB (RX_BUFFER_SIZE length)
  */
  pBufferReadyForReception = aRXBufferA;
  pBufferReadyForUser      = aRXBufferB;
  uwNbReceivedChars        = 0;

  /* Print user info on PC com port */
  //printf("Command AT : %s\n",apConnectAlready);
  PrintInfo(&hlpuart1, apConnectAlready, COUNTOF(apConnectAlready));

  /* Initializes Rx sequence using Reception To Idle event API.
     As DMA channel associated to UART Rx is configured as Circular,
     reception is endless.
     If reception has to be stopped, call to HAL_UART_AbortReceive() could be used.

     Use of HAL_UARTEx_ReceiveToIdle_DMA service, will generate calls to
     user defined HAL_UARTEx_RxEventCallback callback for each occurrence of
     following events :
     - DMA RX Half Transfer event (HT)
     - DMA RX Transfer Complete event (TC)
     - IDLE event on UART Rx line (indicating a pause is UART reception flow)
  */
  if (HAL_OK != HAL_UARTEx_ReceiveToIdle_DMA(&hlpuart1, aRXBufferUser, RX_BUFFER_SIZE))
  {
    Error_Handler();
    printf("StartReception Failed\n");
  } else {
	  printf("StartReception OK\n");

  }
  HAL_Delay(500);
  HAL_NVIC_DisableIRQ(LPUART1_IRQn);
  PrintInfo(&hlpuart1, cwMode, COUNTOF(cwMode));
  HAL_NVIC_EnableIRQ(LPUART1_IRQn);
  HAL_Delay(500);
}

/**
  * @brief  This function handles buffer containing received data on PC com port
  * @note   In this example, received data are sent back on UART Tx (loopback)
  *         Any other processing such as copying received data in a larger buffer to make it
  *         available for application, could be implemented here.
  * @note   This routine is executed in Interrupt context.
  * @param  huart UART handle.
  * @param  pData Pointer on received data buffer to be processed
  * @retval Size  Nb of received characters available in buffer
  */
void UserDataTreatment(UART_HandleTypeDef *huart, uint8_t* pData, uint16_t Size)
{
  /*
   * This function might be called in any of the following interrupt contexts :
   *  - DMA TC and HT events
   *  - UART IDLE line event
   *
   * pData and Size defines the buffer where received data have been copied, in order to be processed.
   * During this processing of already received data, reception is still ongoing.
   *
   */
  uint8_t* pBuff = pData;
  uint8_t  i;

  //debug callback
  printf("Callback : %s\n",pData);

  if(strstr(pData, "WIFI CONNECTE") != NULL)printf("Wifi Connect Successful !\n");
  if(strstr(pData, "+CWJAP:%s",SSID) != NULL)printf("Wifi already Connected !\n");
  //if(strstr(pData, "+CWJAP:\"") != NULL)printf("Wifi already Connected !\n");

  /* Implementation of loopback is on purpose implemented in direct register access,
     in order to be able to echo received characters as fast as they are received.
     Wait for TC flag to be raised at end of transmit is then removed, only TXE is checked */

  for (i = 0; i < Size; i++)
  {
    while (!(__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE))) {}
    huart->Instance->TDR = *pBuff;
    pBuff++;
  }
}

/**
  * @brief  User implementation of the Reception Event Callback
  *         (Rx event notification called after use of advanced reception service).
  * @param  huart UART handle
  * @param  Size  Number of data available in application reception buffer (indicates a position in
  *               reception buffer until which, data are available)
  * @retval None
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  static uint8_t old_pos = 0;
  uint8_t *ptemp;
  uint8_t i;

  /* Check if number of received data in recpetion buffer has changed */
  if (Size != old_pos)
  {
    /* Check if position of index in reception buffer has simply be increased
       of if end of buffer has been reached */
    if (Size > old_pos)
    {
      /* Current position is higher than previous one */
      uwNbReceivedChars = Size - old_pos;
      /* Copy received data in "User" buffer for evacuation */
      for (i = 0; i < uwNbReceivedChars; i++)
      {
        pBufferReadyForUser[i] = aRXBufferUser[old_pos + i];
      }
    }
    else
    {
      /* Current position is lower than previous one : end of buffer has been reached */
      /* First copy data from current position till end of buffer */
      uwNbReceivedChars = RX_BUFFER_SIZE - old_pos;
      /* Copy received data in "User" buffer for evacuation */
      for (i = 0; i < uwNbReceivedChars; i++)
      {
        pBufferReadyForUser[i] = aRXBufferUser[old_pos + i];
      }
      /* Check and continue with beginning of buffer */
      if (Size > 0)
      {
        for (i = 0; i < Size; i++)
        {
          pBufferReadyForUser[uwNbReceivedChars + i] = aRXBufferUser[i];
        }
        uwNbReceivedChars += Size;
      }
    }
    /* Process received data that has been extracted from Rx User buffer */
    UserDataTreatment(huart, pBufferReadyForUser, uwNbReceivedChars);

    /* Swap buffers for next bytes to be processed */
    ptemp = pBufferReadyForUser;
    pBufferReadyForUser = pBufferReadyForReception;
    pBufferReadyForReception = ptemp;
  }
  /* Update old_pos as new reference of position in User Rx buffer that
     indicates position to which data have been processed */
  old_pos = Size;
}

void Wifi_Send_Data(char data[]){
	//Start IP session

}

void Wifi_Init(void){
/*
	//Put the wifi in listening mode
	HAL_UART_Receive_IT(&huart1, (uint8_t*)rxBuffer, sizeof(rxBuffer)-1);
	//check that the ESP8266 is operational
	//HAL_UART_Transmit(&huart1, (uint8_t*)at, strlen(at) , 1000);
	//printf("%s", at);

	//connect the ESP8266 to 'Reseau du KGB' Wifi
	HAL_UART_Transmit(&huart1, (uint8_t*)apConnect, strlen(apConnect) , 1000);
	printf("%s", apConnect);
	//debug rxBuffer

	//Set the Wi-Fi mode to station
	HAL_UART_Transmit(&huart1, (uint8_t*)cwMode, strlen(cwMode) , 1000);
	printf("%s", cwMode);

	Wifi_Send_Data(cipStart);
*/
	//PrintInfo(&hlpuart1, apConnectAlready, COUNTOF(apConnectAlready));
	/*PrintInfo(&hlpuart1, cipStart, COUNTOF(cipStart));
	PrintInfo(&hlpuart1, SendDataSize, COUNTOF(SendDataSize));
	PrintInfo(&hlpuart1, SendData, COUNTOF(SendData));
	PrintInfo(&hlpuart1, cipClose, COUNTOF(cipClose));
*/
	StartReception();
}

void Wifi_Process(void){
	HAL_Delay(1000);
}


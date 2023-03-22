#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


char at[] = "\r\nAT\r\n";
char apConnect[] = "\r\nAT+CWJAP=\"Reseau du KGB\",\"12345678\"\r\n";
char cwMode[] = "\r\nAT+CWMODE=1\r\n";
char cipStart[] = "\r\nAT+CIPSTART=\"TCP\",\"https://tcpbin.com\",4242\r\n";
char cipClose[] = "\r\nAT+CIPCLOSE\r\n";
int rxBufferSize = 120;
char rxBuffer[40];
/*
char ExtendBuffer(void);

char ExtendBuffer(void){
	if (strlen(rxBuffer) == rxBufferSize) {
	    // le tableau est plein
		int newBufferSize = rxBufferSize * rxBufferSizeCount;
		char *newBuffer[newBufferSize];
		strcpy(rxBuffer, *newBuffer);
		memset(rxBuffer, 0, sizeof(rxBuffer));
		rxBufferSizeCount++;
		return *newBuffer;
	}
	else { return rxBuffer; }
}*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart1)
  {
	  //debug rxBuffer
	  printf("Wifi Response 1 : %s\n", rxBuffer);
	  if(strstr(rxBuffer, "WIFI CONNECTE") != NULL)printf("Wifi_Init Successful !\n");
	  else printf("Wifi_Init Failed !\n");
	  HAL_UART_Receive_IT(&huart1, (uint8_t*)rxBuffer, sizeof(rxBuffer)-1);
  }
}

void Wifi_Send_Data(char data[]){
	//Start IP session
	HAL_UART_Transmit(&huart1, (uint8_t*)cipStart, strlen(cipStart) , 1000);
	printf("%s", cipStart);
	//Start IP session
	HAL_UART_Transmit(&huart1, (uint8_t*)cipClose, strlen(cipClose) , 1000);
	printf("%s", cipClose);
}

void Wifi_Init(void){

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
}

void Wifi_Process(void){
	HAL_Delay(1000);
}


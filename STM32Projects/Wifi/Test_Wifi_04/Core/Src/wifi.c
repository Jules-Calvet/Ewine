#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


char at[] = "\r\nAT\r\n";
char version[] = "\r\nAT+GMR\r\n";
char apConnect[] = "\r\nAT+CWJAP=\"Reseau du KGB\",\"12345678\"\r\n";
char rxBuffer[100];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart1)
  {
	  printf("Wifi Response: %s\n", rxBuffer);
	  memset(rxBuffer, 0, sizeof(rxBuffer));
	  HAL_UART_Receive_IT(&huart1, (uint8_t*)rxBuffer, sizeof(rxBuffer)-1);
  }
}

void Wifi_Init(void){
	//Put the wifi in listening mode
	HAL_UART_Receive_IT(&huart1, (uint8_t*)rxBuffer, sizeof(rxBuffer)-1);
	//check that the ESP8266 is operational
	HAL_UART_Transmit(&huart1, (uint8_t*)at, strlen(at) , 1000); //Send
	printf("%s\n", at);

	HAL_Delay(1000);
	//connect the ESP8266 to 'Reseau du KGB' Wifi
	HAL_UART_Transmit(&huart1, (uint8_t*)apConnect, strlen(apConnect) , 1000); //Send
	printf("%s\n", apConnect);

	HAL_Delay(1000);
}

void Wifi_Process(void){


}


#include "main.h"
#include "esp8266.h"
#include "stdio.h"

void init_done();

void init_done()
{
	for(int i=0; i<3; i++)
	{
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		HAL_Delay(200);
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		HAL_Delay(200);
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		HAL_Delay(200);
	}
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, RESET);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, RESET);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, RESET);
}

void wifi_init(){
	if (esp8266_SetupIP() != _SUCCEED) {
	  printf("esp8266_SetupIP failed - RESTART and configure WIFI source\n");
  } else {
	  while(esp8266_SetupConnect_BrokerMQTT() != _SUCCEED_MQTT)
	  {
		  printf("esp8266_SetupConnect_BrokerMQTT failed - RESTART and configure MQTT\n");
	  }
	  init_done();
  }
}

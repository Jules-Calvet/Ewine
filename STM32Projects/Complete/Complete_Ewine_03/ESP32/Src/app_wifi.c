#include "main.h"
#include "esp8266.h"
#include "parson.h"
#include "stdio.h"

void init_done();
static char* formate_json_button(int value);
static char* formate_json_tof(int id, int value);
static int check_subscription();

typedef enum _button_interface
{
	NONE,
	B1,
	B2,
}button_interface;

uint16_t button_pushed = NONE;

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

static char* formate_json_button(int value)
{
	JSON_Value *val = json_value_init_object();
	if(val == NULL)
		return NULL;

	JSON_Object *obj = json_value_get_object(val);
	if(obj == NULL)
		return NULL;

	if(json_object_set_number(obj, "id", value) != JSONSuccess)
		return NULL;

	char* serialized_string = json_serialize_to_string(val);

	json_value_free(val);

	return serialized_string;
}

static char* formate_json_tof(int id, int value)
{
	JSON_Value *val = json_value_init_object();
	if(val == NULL)
		return NULL;

	JSON_Object *obj = json_value_get_object(val);
	if(obj == NULL)
		return NULL;

	if(id == 0){
		if(json_object_set_number(obj, "tof_left", value) != JSONSuccess)
			return NULL;
	}
	if(id == 1){
		if(json_object_set_number(obj, "tof_center", value) != JSONSuccess)
			return NULL;
	}
	if(id == 2){
		if(json_object_set_number(obj, "tof_right", value) != JSONSuccess)
			return NULL;
	}

	char* serialized_string = json_serialize_to_string(val);

	json_value_free(val);

	return serialized_string;
}

static int check_subscription()
{
	if (UART_ESP32_RecvEndFlag == 1) {
		char pattern[24];
		memset(pattern, 0, sizeof(pattern));
		//sprintf((char*) pattern, "+MQTTSUBRECV:0,\"isen%s/led\",", CARD_NUMBER);
		sprintf((char*) pattern, "+MQTTSUBRECV:0,\"%s/", MQTT_CLIENTID);
		uint16_t data_size = 0, off = sizeof(pattern)-1; // \0 not needed
		if (strstr((const char*) UART_ESP32_RxBUF, pattern) != NULL) {
			printf("APP: Match succeed\r\n");
			int topic_id = 0;
			// nul mais ok
			if(UART_ESP32_RxBUF[off] == 'l')
			{
				topic_id = 1;
				off+= 5;
			}
			else
			{
				topic_id = 2;
				off+= 9;
			}
			return 0;
		}
		else
		{
			printf("APP: Match failed\r\n");
			return -1;
		}
	}
	else
	{
		return 1;
	}
}

void wifi_init(){
	if (esp8266_SetupIP() != _SUCCEED) {
	  printf("esp8266_SetupIP failed - RESTART and configure WIFI source\n");
	  while(1)
	  {
		  HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		  HAL_Delay(200);
	  }
  }

  if(esp8266_SetupConnect_BrokerMQTT() != _SUCCEED_MQTT)
  {
	  printf("esp8266_SetupConnect_BrokerMQTT failed - RESTART and configure MQTT\n");
	  while(1)
	  {
		  HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		  HAL_Delay(200);
	  }
  }
  init_done();
}

void wifi_process(){
	// app part
	int i = 0;
	while(i<1)
	  {
		  if(get_mqtt_state() < _SUBSCRIBED_TEMP_MQTT)
		  {
			  if(esp8266_Subscribe_Temp_BrokerMQTT() != _SUBSCRIBED_TEMP_MQTT)
				printf("APP: esp8266_Subscribe_Temp_BrokerMQTT() failed\n");
		  }
		 if(get_mqtt_state() < _SUBSCRIBED_TOF_MQTT)
		  {
			  if(esp8266_Subscribe_ToF_BrokerMQTT() != _SUBSCRIBED_TOF_MQTT)
				printf("APP: esp8266_Subscribe_ToF_BrokerMQTT() failed\n");
		  }

		 if(LeftPos == 1){
			 char *data = formate_json_tof(0,1);
			  if(esp8266_Publish_ToF_BrokerMQTT((uint8_t*)data,strlen(data)) != _PUBLISH_TOF_MQTT)
				printf("APP: B1 esp8266_Publish_ToF_BrokerMQTT() failed\n");
			  json_free_serialized_string(data);
			 LeftPos = 2;
		 } else if (LeftPos == 0){
			 char *data = formate_json_tof(0,0);
			  if(esp8266_Publish_ToF_BrokerMQTT((uint8_t*)data,strlen(data)) != _PUBLISH_TOF_MQTT)
				printf("APP: B1 esp8266_Publish_ToF_BrokerMQTT() failed\n");
			  json_free_serialized_string(data);
		 }
		 if(CenterPos == 1){
			 char *data = formate_json_tof(1,1);
			  if(esp8266_Publish_ToF_BrokerMQTT((uint8_t*)data,strlen(data)) != _PUBLISH_TOF_MQTT)
				printf("APP: B1 esp8266_Publish_ToF_BrokerMQTT() failed\n");
			  json_free_serialized_string(data);
			 CenterPos = 2;
		 } else if (CenterPos == 0){
			 char *data = formate_json_tof(1,0);
			  if(esp8266_Publish_ToF_BrokerMQTT((uint8_t*)data,strlen(data)) != _PUBLISH_TOF_MQTT)
				printf("APP: B1 esp8266_Publish_ToF_BrokerMQTT() failed\n");
			  json_free_serialized_string(data);
		 }
		 if(RightPos == 1){
			 char *data = formate_json_tof(2,1);
			  if(esp8266_Publish_ToF_BrokerMQTT((uint8_t*)data,strlen(data)) != _PUBLISH_TOF_MQTT)
				printf("APP: B1 esp8266_Publish_ToF_BrokerMQTT() failed\n");
			  json_free_serialized_string(data);
			 RightPos = 2;
		 } else if (RightPos == 0){
			 char *data = formate_json_tof(2,0);
			  if(esp8266_Publish_ToF_BrokerMQTT((uint8_t*)data,strlen(data)) != _PUBLISH_TOF_MQTT)
				printf("APP: B1 esp8266_Publish_ToF_BrokerMQTT() failed\n");
			  json_free_serialized_string(data);
		 }

		  int sub_status = check_subscription();
		  if(sub_status == 0)
			  printf("APP: check_subscription() SUB MQTT RCV\n");
		  i++;
	  }

}

#include "app_IR.h"
#include "main.h"
#include "mlx90614.h"
#include "mlx90614Config.h"
#include "esp8266.h"
#include "parson.h"
#include <stdio.h>

static char* formate_json_temp(int value)
{
	JSON_Value *val = json_value_init_object();
	if(val == NULL)
		return NULL;

	JSON_Object *obj = json_value_get_object(val);
	if(obj == NULL)
		return NULL;
	if(json_object_set_number(obj, "value", value) != JSONSuccess)
		return NULL;

	char* serialized_string = json_serialize_to_string(val);

	json_value_free(val);

	return serialized_string;
}

void IR_Init()
{
	if(mlx90614_init() == true)printf("Initialization IR OK ! \n");
	  else printf("Initialization IR Failed ! \n");

	  mlx90614.configReg.DualIRSensor = 0;
	  mlx90614.configReg.SelectObjAmb = 1;
	  mlx90614.configReg.FIR = 4;
}

void IR_Process()
{
	float temp = 0.0f;
	  if ( mlx90614_getAmbient(&temp) == true ) printf("getAmbient OK : %0.2f *C\n", temp);
	  else printf("Failed to read ambient temperature\n");

	  HAL_Delay(200);
	//Send Temperature data
	 char *data = formate_json_temp(temp);
	  if(esp8266_Publish_Temp_BrokerMQTT((uint8_t*)data,strlen(data)) != _PUBLISH_TEMP_MQTT)
		printf("APP: B1 esp8266_Publish_Temp_BrokerMQTT() failed\n");
	  json_free_serialized_string(data);
}

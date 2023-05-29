#ifndef __ESP8266_H
#define __ESP8266_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "net_conf.h"
#include <string.h>
#include <stdlib.h>

#define UART_ESP32_MAX_SENDLEN  2048
#define UART_ESP32_MAX_RECVLEN  2048

extern uint8_t UART_ESP32_TxBUF[];
extern uint8_t UART_ESP32_RxBUF[];
extern volatile uint8_t UART_ESP32_RxLen;
extern volatile uint8_t UART_ESP32_RecvEndFlag;
extern char AP_SSID[];
extern char AP_PSWD[];

#define OK_ACK            (uint8_t*)"OK"
#define TRANS_QUIT_CMD    (uint8_t*)"+++"

#define WITH_NEWLINE     1
#define WITHOUT_NEWLINE  0

typedef enum __WIFI_StateTypDef {
	_FAILED = 0,        // failed
	_SUCCEED = 1,       // succeed
	_TIMEOUT = 2,       // wait timeout
	_MATCHERROR = 3,    // cmd match error
	_MATCHOK = 4,       // cmd match ok
	_ONLINE = 5,        // wifi connect
	_OFFLINE = 6,       // wifi disconnect
	_CONNECTED = 7,     // server connected
	_DISCONNECTED = 8,  // server disconnected
	_TRANS_ENBALE = 9,
	_TRANS_DISABLE = 10,
	_UNKNOWN_STATE = 0xee,
	_UNKNOWN_ERROR = 0xff
} WIFI_StateTypeDef;

typedef enum __MQTT_StateTypDef {
	_FAILED_MQTT = 0,        // failed
	_SUCCEED_MQTT = 1,       // succeed
	_TIMEOUT_MQTT = 2,       // wait timeout
	_UNINIT_MQTT = 3,
	_CONFIG_ERROR_MQTT = 4,
	_CONFIG_OK_MQTT = 5,
	_UNCONNECTED_MQTT = 6,
	_CONNECTED_MQTT = 7,
	_SUBSCRIBED_TEMP_MQTT = 8,
	_SUBSCRIBED_TOF_MQTT = 9,
	_PUBLISH_TEMP_MQTT = 10,
	_PUBLISH_TOF_MQTT = 11,
	_UNKNOWN_STATE_MQTT = 0xee,
	_UNKNOWN_ERROR_MQTT = 0xff
} MQTT_StateTypeDef;

#define ESP8266_MAX_TIMEOUT     (uint16_t)0xffff
#define ESP8266_MAX_RETRY_TIME  3

#define WIFI_RST_Enable()    HAL_GPIO_WritePin(ESP32_RST_GPIO_Port,ESP32_RST_Pin,RESET);
#define WIFI_RST_Disable() 	HAL_GPIO_WritePin(ESP32_RST_GPIO_Port, ESP32_RST_Pin, SET);

extern uint8_t *esp8266_ack;
extern WIFI_StateTypeDef wifi_state;

WIFI_StateTypeDef esp8266_CheckRespond(uint8_t *str);
WIFI_StateTypeDef esp8266_TransmitCmd(uint8_t *cmd, uint8_t *ack,
		uint32_t waittime, uint8_t newline);
WIFI_StateTypeDef esp8266_HardwareReset(uint32_t timeout);
WIFI_StateTypeDef esp8266_QuitTrans(void);
WIFI_StateTypeDef esp8266_ConnectAP(void);
WIFI_StateTypeDef esp8266_ConnectServer(void);
WIFI_StateTypeDef esp8266_SetUpTCPConnection(void);

// +BCH
WIFI_StateTypeDef esp8266_SetupIP();
MQTT_StateTypeDef esp8266_SetupConnect_BrokerMQTT();

uint8_t get_mqtt_state();

#ifdef __cplusplus
}
#endif

#endif

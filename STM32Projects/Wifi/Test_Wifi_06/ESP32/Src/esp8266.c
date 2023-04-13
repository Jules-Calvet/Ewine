#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "esp8266.h"

static WIFI_StateTypeDef wifi_state = _OFFLINE;
static WIFI_StateTypeDef trans_state = _UNKNOWN_STATE;
static uint8_t wifi_config_step = 0;
static uint8_t mqtt_config_step = 0;

static MQTT_StateTypeDef mqtt_state = _UNINIT_MQTT;

uint8_t UART_ESP32_TxBUF[UART_ESP32_MAX_SENDLEN];
uint8_t UART_ESP32_RxBUF[UART_ESP32_MAX_RECVLEN];
volatile uint8_t UART_ESP32_RxLen = 0;
volatile uint8_t UART_ESP32_RecvEndFlag = 0;

void uart_esp32_transmit(char *fmt, ...) {
	uint16_t i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char*) UART_ESP32_TxBUF, fmt, ap);
	va_end(ap);

	for (i = 0; i < UART_ESP32_MAX_SENDLEN; i++) {
		j = i + 1;
		if (UART_ESP32_TxBUF[i] == '\00') {
			for (; j < UART_ESP32_MAX_SENDLEN; j++) {
				UART_ESP32_TxBUF[j - 1] = UART_ESP32_TxBUF[j];
			}
		}
	}

	i = strlen((const char*) UART_ESP32_TxBUF);

	HAL_UART_Transmit(&hlpuart1, UART_ESP32_TxBUF, i, 100);

	memset(UART_ESP32_TxBUF, 0, UART_ESP32_MAX_SENDLEN);
	memset(UART_ESP32_RxBUF, 0, UART_ESP32_MAX_RECVLEN);
	UART_ESP32_RecvEndFlag = 0;
}

WIFI_StateTypeDef esp8266_CheckRespond(uint8_t *str) {
	printf("(DBG:) recv:\r\n%s\r\n", UART_ESP32_RxBUF);
	if (strstr((const char*) UART_ESP32_RxBUF, (const char*) str) != NULL) {
		printf("(DBG:) Match succeed\r\n");
		memset(UART_ESP32_RxBUF, 0, UART_ESP32_MAX_RECVLEN);
		return _MATCHOK;
	}

	memset(UART_ESP32_RxBUF, 0, UART_ESP32_MAX_RECVLEN);
	return _MATCHERROR;
}

WIFI_StateTypeDef esp8266_TransmitCmd(uint8_t *cmd, uint8_t *ack,
		uint32_t waitms, uint8_t newline) {
	int timeout = waitms;
	uint8_t check = 0;
	memset(UART_ESP32_RxBUF, 0, UART_ESP32_MAX_RECVLEN);
	printf("\r\n(DBG:) Try to send cmd: %s\r\n", cmd);
	if (newline == 0)
		uart_esp32_transmit("%s", cmd);      // transmit cmd to usart2
	else
		uart_esp32_transmit("%s\r\n", cmd); // transmit cmd to usart2

	//printf("(DBG:) Waiting reply\r\n");
	while (timeout--) {
		// //printf("%d ", timeout);
		// finish dma receive
		if (UART_ESP32_RecvEndFlag == 1) {
			check = esp8266_CheckRespond(ack);
			if (check == _MATCHOK) {
				//printf("(DBG:) Command closed loop completed\r\n");
			}

			UART_ESP32_RxLen = 0;
			UART_ESP32_RecvEndFlag = 0;
			HAL_UART_Receive_DMA(&hlpuart1, UART_ESP32_RxBUF, UART_ESP32_MAX_RECVLEN);

			break;
		}
		HAL_Delay(1);
	}

	if (check == _MATCHERROR) {
		printf("\r\n(DBG:) Cmd match failed\r\n");
		return check;
	}
	if (timeout <= 0) {
		printf("(DBG:) Finish waiting\r\n");
		printf("\r\n(DBG:) Timeout\r\n");
		return _TIMEOUT;
	}
	printf("(DBG:) Succeed\r\n");
	return _SUCCEED;
}

WIFI_StateTypeDef esp8266_HardwareReset(uint32_t waitms) {
	int timeout = waitms;
	WIFI_RST_Enable();
	HAL_Delay(500);
	WIFI_RST_Disable();
	while (timeout--) {
		if (UART_ESP32_RecvEndFlag == 1) {

			printf("(DBG:) Hardware Reset OK!\r\n");
			HAL_Delay(100);

			UART_ESP32_RxLen = 0;
			UART_ESP32_RecvEndFlag = 0;
			HAL_UART_Receive_DMA(&hlpuart1, UART_ESP32_RxBUF, UART_ESP32_MAX_RECVLEN);

			return _SUCCEED;
		}
		HAL_Delay(1);
	}
	if (timeout <= 0) {
		printf("(DBG:) Finish waiting\r\n");
		printf("\r\n(DBG:) Timeout\r\n");
		return _TIMEOUT;
	}

	return _UNKNOWN_ERROR;
}

WIFI_StateTypeDef esp8266_ConnectAP() {
	uint16_t cmd_len = strlen(AP_SSID) + strlen(AP_PSWD) + 30;
	uint8_t *cmd = (uint8_t*) malloc(cmd_len * sizeof(uint8_t));
	memset(cmd, 0, cmd_len);
	sprintf((char*) cmd, "AT+CWJAP=\"%s\",\"%s\"", AP_SSID, AP_PSWD);

	if (esp8266_TransmitCmd(cmd, (uint8_t*) "WIFI CONNECTED", ESP8266_MAX_TIMEOUT, WITH_NEWLINE) == _SUCCEED)
		wifi_state = _ONLINE;
	else
		wifi_state = _OFFLINE;

	return wifi_state;
}

WIFI_StateTypeDef esp8266_ConnectServer() {
	uint16_t cmd_len = strlen(IpServer) + strlen(ServerPort) + 30;
	uint8_t *cmd = (uint8_t*) malloc(cmd_len * sizeof(uint8_t));
	memset(cmd, 0, cmd_len);
	sprintf((char*) cmd, "AT+CIPSTART=\"TCP\",\"%s\",%s", IpServer, ServerPort);

	//if (esp8266_TransmitCmd(cmd, (uint8_t*) "CONNECT\r\n\r\nOK\r\n", ESP8266_MAX_TIMEOUT, WITH_NEWLINE) == _SUCCEED)
	if (esp8266_TransmitCmd(cmd, (uint8_t*) "\r\nOK\r\n", ESP8266_MAX_TIMEOUT, WITH_NEWLINE) == _SUCCEED)
		wifi_state = _CONNECTED;
	else
		wifi_state = _DISCONNECTED;

	return wifi_state;
}

WIFI_StateTypeDef esp8266_SetUpTCPConnection() {
	uint8_t retry_count = 0;

	/* Reset esp8266 */
	printf("(DBG:) Trying to reset esp8266\r\n");
	wifi_config_step++;
	while (esp8266_HardwareReset(500) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Reset failed\r\n");
			retry_count = 0;
			trans_state = _UNKNOWN_STATE;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(2000);		// wait 2 seconds
	retry_count = 0;		// reset retry count

	/* Disable transparent transmission */
	printf("(DBG:) Trying to close transparent transmission\r\n");
	wifi_config_step++;
	/*while (esp8266_TransmitCmd(TRANS_QUIT_CMD, TRANS_QUIT_CMD,
	ESP8266_MAX_TIMEOUT,
	WITHOUT_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1500);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Close transparent transmission failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}*/

	trans_state = _TRANS_DISABLE;
	//HAL_Delay(1500);
	retry_count = 0;		// reset retry count

	/* Close echo */
	printf("(DBG:) Trying to close echo\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "ATE0", OK_ACK, 500, WITH_NEWLINE)
			!= _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Close echo failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Set wifi mode 0:AP 1:STA 2:AP+STA */
	printf("(DBG:) Trying to set Wifi mode\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CWMODE=1", OK_ACK, 500,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Set Wifi mode failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Disable auto connect */
	printf("(DBG:) Trying to close auto connect\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CWAUTOCONN=0", OK_ACK, 500,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Close auto connect failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Connect to AP(Wifi) */
	printf("(DBG:) Trying to connect to AP\r\n");
	wifi_config_step++;
	while (esp8266_ConnectAP() != _ONLINE) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Connect to AP failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Try to get AP info */
	if (wifi_state == _ONLINE) {
		while (esp8266_TransmitCmd((uint8_t*) "AT+CWJAP?", OK_ACK,
		ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED) {
			retry_count++;
			HAL_Delay(1000);
			if (retry_count > ESP8266_MAX_RETRY_TIME / 2) {
				printf("(DBG:) Get AP msg failed\r\n");
				printf("(DBG:) Connect server process will not be terminated");
				retry_count = 0;
				wifi_config_step--;
				break;
			}
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Try to get IP info */
	if (wifi_state == _ONLINE) {
		while (esp8266_TransmitCmd((uint8_t*) "AT+CIPSTA?", OK_ACK,
		ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED) {
			retry_count++;
			HAL_Delay(1000);
			if (retry_count > ESP8266_MAX_RETRY_TIME / 2) {
				printf("(DBG:) Get IP info failed\r\n");
				printf("(DBG:) Connect server process will not be terminated");
				retry_count = 0;
				wifi_config_step--;
				break;
			}
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Set DHCP */
	printf("(DBG:) Trying to set DHCP mode\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CWDHCP=1,1", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Set DHCP model failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;

	/* Set single connection */
	printf("(DBG:) Trying to set single connection\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CIPMUX=0", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Set single connection model failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;

	/* Set transparent transmission */
	printf("(DBG:) Trying to set transparent transmission mode\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CIPMODE=1", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Set transparent transmission mode failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(1000);
	retry_count = 0;

	/* Connect to TCP server */
	printf("(DBG:) Trying to connect TCP server\r\n");
	wifi_config_step++;
	while (esp8266_ConnectServer() != _CONNECTED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Connect TCP server failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(1000);
	retry_count = 0;

	/* enable data send(transparent transmission) */
	printf("(DBG:) Trying to enable data send\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CIPSEND", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Set transparent transmission mode failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	trans_state = _TRANS_ENBALE;
	HAL_Delay(100);
	retry_count = 0;

	/* send test msg */
//	printf("Test msg is sending to TCP Server\r\n");
//	uart_esp32_transmit("This msg means TCP connection has been set up\r\n");
//	printf("Test msg has been send to TCP Server\r\n");
	return _SUCCEED;
}

// +BCH
WIFI_StateTypeDef esp8266_SetupIP()
{
	uint8_t retry_count = 0;

	/* Reset esp8266 */
	printf("(DBG:) Trying to reset esp8266\r\n");
	wifi_config_step++;
	while (esp8266_HardwareReset(500) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Reset failed\r\n");
			retry_count = 0;
			trans_state = _UNKNOWN_STATE;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(2000);		// wait 2 seconds
	retry_count = 0;		// reset retry count

	/* Close echo */
	printf("(DBG:) Trying to close echo\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "ATE0", OK_ACK, 500, WITH_NEWLINE)
			!= _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Close echo failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Set wifi mode 0:AP 1:STA 2:AP+STA */
	printf("(DBG:) Trying to set Wifi mode\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CWMODE=1", OK_ACK, 500,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Set Wifi mode failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Disable auto connect */
	printf("(DBG:) Trying to close auto connect\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CWAUTOCONN=0", OK_ACK, 500,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Close auto connect failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Connect to AP(Wifi) */
	printf("(DBG:) Trying to connect to AP\r\n");
	wifi_config_step++;
	while (esp8266_ConnectAP() != _ONLINE) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Connect to AP failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Try to get AP info */
	if (wifi_state == _ONLINE) {
		while (esp8266_TransmitCmd((uint8_t*) "AT+CWJAP?", OK_ACK,
		ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED) {
			retry_count++;
			HAL_Delay(1000);
			if (retry_count > ESP8266_MAX_RETRY_TIME / 2) {
				printf("(DBG:) Get AP msg failed\r\n");
				printf("(DBG:) Connect server process will not be terminated");
				retry_count = 0;
				wifi_config_step--;
				break;
			}
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Try to get IP info */
	if (wifi_state == _ONLINE) {
		while (esp8266_TransmitCmd((uint8_t*) "AT+CIPSTA?", OK_ACK,
		ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED) {
			retry_count++;
			HAL_Delay(1000);
			if (retry_count > ESP8266_MAX_RETRY_TIME / 2) {
				printf("(DBG:) Get IP info failed\r\n");
				printf("(DBG:) Connect server process will not be terminated");
				retry_count = 0;
				wifi_config_step--;
				break;
			}
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* Set DHCP */
	printf("(DBG:) Trying to set DHCP mode\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CWDHCP=1,1", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Set DHCP model failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;

	/* Set single connection */
	printf("(DBG:) Trying to set single connection\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CIPMUX=0", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Set single connection model failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;

	/* Set transparent transmission */
	printf("(DBG:) Trying to set transparent transmission mode\r\n");
	wifi_config_step++;
	while (esp8266_TransmitCmd((uint8_t*) "AT+CIPMODE=1", OK_ACK, 1000,
	WITH_NEWLINE) != _SUCCEED) {
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Set transparent transmission mode failed\r\n");
			retry_count = 0;
			wifi_config_step--;
			return _FAILED;
		}
	}

	HAL_Delay(100);
	retry_count = 0;
	return _SUCCEED;

}

static MQTT_StateTypeDef esp8266_ConfigAccessBroker()
{
	int retry_count = 0;
	uint16_t cmd_len = strlen(MQTT_CLIENTID) + strlen(MQTT_USERNAME) + strlen(MQTT_PASSWORD) + 40;
	uint8_t *cmd = (uint8_t*) malloc(cmd_len * sizeof(uint8_t));
	memset(cmd, 0, cmd_len);
	sprintf((char*) cmd, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"", MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

	while (esp8266_TransmitCmd(cmd, OK_ACK, ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED)
	{
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) esp8266_ConfigAccessBroker() failed\r\n");
			retry_count = 0;
			mqtt_state = _CONFIG_ERROR_MQTT;
		}
	}

	mqtt_state = _CONFIG_OK_MQTT;
	HAL_Delay(100);
	retry_count = 0;

	return mqtt_state;
}

static MQTT_StateTypeDef esp8266_ConfigConnectionBroker()
{
	if (esp8266_TransmitCmd((uint8_t*) "AT+MQTTCONNCFG=0,60,0,\"lwtt\",\"lwtm\",0,0", OK_ACK, ESP8266_MAX_TIMEOUT,
			WITH_NEWLINE) == _SUCCEED)
		mqtt_state = _CONFIG_OK_MQTT;
	else
		mqtt_state = _CONFIG_ERROR_MQTT;

	return mqtt_state;
}

static MQTT_StateTypeDef esp8266_ConnectBroker()
{
	int retry_count = 0;
	uint16_t cmd_len = strlen(IpServer) + strlen(ServerPort) + 25;
	uint8_t *cmd = (uint8_t*) malloc(cmd_len * sizeof(uint8_t));
	memset(cmd, 0, cmd_len);
	sprintf((char*) cmd, "AT+MQTTCONN=0,\"%s\",%s,0", IpServer, ServerPort);

	//if (esp8266_TransmitCmd(cmd, (uint8_t*)"\r\nready\r\n", ESP8266_MAX_TIMEOUT, WITH_NEWLINE) == _SUCCEED)
	while (esp8266_TransmitCmd(cmd, OK_ACK, ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED)
	{
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) esp8266_ConnectBroker() failed\r\n");
			retry_count = 0;
			mqtt_state = _UNCONNECTED_MQTT;
		}
	}

	mqtt_state = _CONNECTED_MQTT;

	return mqtt_state;
}

MQTT_StateTypeDef esp8266_SetupConnect_BrokerMQTT()
{
	uint8_t retry_count = 0;

	/* Set NTP esp8266 */
//	printf("(DBG:) Trying to configure NTP server esp8266\r\n");
//	mqtt_config_step++;
//	while (esp8266_TransmitCmd((uint8_t*)"AT+CIPSNTPCFG=1,8,\"time.google.com\"", OK_ACK, ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED) {
//		retry_count++;
//		HAL_Delay(500);
//		if (retry_count > ESP8266_MAX_RETRY_TIME) {
//			printf("(DBG:) Configure NTP server failed\r\n");
//			retry_count = 0;
//			mqtt_config_step--;
//			return _FAILED_MQTT;
//		}
//	}
//
//	HAL_Delay(100);
//	retry_count = 0;		// reset retry count
//
//
//	/* test NTP esp8266 */
//	printf("(DBG:) Trying to test NTP server esp8266\r\n");
//	mqtt_config_step++;
//	while (esp8266_TransmitCmd((uint8_t*)"AT+CIPSNTPTIME?", (uint8_t*)"+CIPSNTPTIME:", ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED) {
//		retry_count++;
//		HAL_Delay(500);
//		if (retry_count > ESP8266_MAX_RETRY_TIME) {
//			printf("(DBG:) test NTP failed\r\n");
//			retry_count = 0;
//			mqtt_config_step--;
//			return _FAILED_MQTT;
//		}
//	}
//
//	HAL_Delay(100);
//	retry_count = 0;		// reset retry count

	/* MQTT broker configuration esp8266 */
	printf("(DBG:) Trying to configure MQTT access broker esp8266\r\n");
	mqtt_config_step++;
	while (esp8266_ConfigAccessBroker() != _CONFIG_OK_MQTT) {
		retry_count++;
		HAL_Delay(500);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Configure MQTT access broker failed\r\n");
			retry_count = 0;
			mqtt_config_step--;
			return _FAILED_MQTT;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count

	/* MQTT broker configuration esp8266 */
	/*printf("(DBG:) Trying to configure MQTT connection broker esp8266\r\n");
	mqtt_config_step++;
	while (esp8266_ConfigConnectionBroker() != _CONFIG_OK_MQTT) {
		retry_count++;
		HAL_Delay(500);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) Configure MQTT connection broker failed\r\n");
			retry_count = 0;
			mqtt_config_step--;
			return _FAILED_MQTT;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count
	*/

	/* MQTT broker connection esp8266 */
	printf("(DBG:) Trying to connect to MQTT broker esp8266\r\n");
	mqtt_config_step++;
	while (esp8266_ConnectBroker() < _CONNECTED_MQTT) {
		retry_count++;
		HAL_Delay(500);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) connection MQTT broker failed\r\n");
			retry_count = 0;
			mqtt_config_step--;
			return _FAILED_MQTT;
		}
	}

	HAL_Delay(100);
	retry_count = 0;		// reset retry count
	return _SUCCEED_MQTT;
}

MQTT_StateTypeDef esp8266_Subscribe_Led_BrokerMQTT()
{
	int retry_count = 0;
	if(mqtt_state < _CONNECTED_MQTT)
		return _FAILED_MQTT;

	uint16_t cmd_len = 40;
	uint8_t *cmd = (uint8_t*) malloc(cmd_len * sizeof(uint8_t));
	memset(cmd, 0, cmd_len);
	sprintf((char*) cmd, "AT+MQTTSUB=0,\"%s/led\",1", MQTT_CLIENTID);

	while(esp8266_TransmitCmd(cmd, OK_ACK, ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED)
	{
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) esp8266_Subscribe_Led_BrokerMQTT() failed\r\n");
			retry_count = 0;
			mqtt_state = _FAILED_MQTT;
		}
	}

	HAL_Delay(100);
	retry_count = 0;
	mqtt_state = _SUBSCRIBED_LED_MQTT;
	return mqtt_state;
}

MQTT_StateTypeDef esp8266_Subscribe_Temp_BrokerMQTT()
{
	int retry_count = 0;
	if(mqtt_state < _CONNECTED_MQTT)
		return _FAILED_MQTT;

	uint16_t cmd_len = 40;
	uint8_t *cmd = (uint8_t*) malloc(cmd_len * sizeof(uint8_t));
	memset(cmd, 0, cmd_len);
	sprintf((char*) cmd, "AT+MQTTSUB=0,\"%s/getTemp\",1", MQTT_CLIENTID);

	while(esp8266_TransmitCmd(cmd, OK_ACK, ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED)
	{
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) esp8266_Subscribe_Temp_BrokerMQTT() failed\r\n");
			retry_count = 0;
			mqtt_state = _FAILED_MQTT;
		}
	}
	HAL_Delay(100);
	retry_count = 0;
	mqtt_state = _SUBSCRIBED_TEMP_MQTT;
	return mqtt_state;
}

MQTT_StateTypeDef esp8266_Publish_Button_BrokerMQTT(uint8_t *data, uint16_t size)
{
	int retry_count = 0;
	if(mqtt_state < _CONNECTED_MQTT || data == NULL)
		return _FAILED_MQTT;

	uint16_t cmd_len = size + 40;
	uint8_t *cmd = (uint8_t*) malloc(cmd_len * sizeof(uint8_t));
	memset(cmd, 0, cmd_len);
	//sprintf((char*) cmd, "AT+MQTTPUB=0,\"home/temp\",\"%s\",1,0", data);
	sprintf((char*) cmd, "AT+MQTTPUB=0,\"%s/button\",\"%s\",1,0", MQTT_CLIENTID, data);

	while(esp8266_TransmitCmd(cmd, OK_ACK, ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED)
	{
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) esp8266_Subscribe_Temp_BrokerMQTT() failed\r\n");
			retry_count = 0;
			mqtt_state = _FAILED_MQTT;
		}
	}
	HAL_Delay(100);
	retry_count = 0;
	mqtt_state = _PUBLISH_BUTTON_MQTT;
	return mqtt_state;
}

MQTT_StateTypeDef esp8266_Publish_Temp_BrokerMQTT(uint8_t *data, uint16_t size)
{
	int retry_count = 0;
	if(mqtt_state < _CONNECTED_MQTT || data == NULL)
		return _FAILED_MQTT;

	uint16_t cmd_len = size + 40;
	uint8_t *cmd = (uint8_t*) malloc(cmd_len * sizeof(uint8_t));
	memset(cmd, 0, cmd_len);
	sprintf((char*) cmd, "AT+MQTTPUB=0,\"%s/temp\",\"%s\",1,0", MQTT_CLIENTID, data);

	while (esp8266_TransmitCmd(cmd, OK_ACK, ESP8266_MAX_TIMEOUT, WITH_NEWLINE) != _SUCCEED)
	{
		retry_count++;
		HAL_Delay(1000);
		if (retry_count > ESP8266_MAX_RETRY_TIME) {
			printf("(DBG:) esp8266_Subscribe_Temp_BrokerMQTT() failed\r\n");
			retry_count = 0;
			mqtt_state = _FAILED_MQTT;
		}
	}
	HAL_Delay(100);
	retry_count = 0;
	mqtt_state = _PUBLISH_TEMP_MQTT;
	return mqtt_state;
}

uint8_t get_mqtt_state()
{
	return mqtt_state;
}

#include <stdbool.h>

void Wifi_Process(void);
bool Wifi_Init(void);
bool Wifi_Send_Data(char data[]);
bool PrintInfo(UART_HandleTypeDef *huart, uint8_t *String, uint16_t Size);
bool addAtCommand(UART_HandleTypeDef *huart, uint8_t *String, uint16_t Size);
void StartReception(void);
void UserDataTreatment(UART_HandleTypeDef *huart, uint8_t* pData, uint16_t Size);

void Wifi_Process(void);
void Wifi_Init(void);
void Wifi_Send_Data(char data[]);
void PrintInfo(UART_HandleTypeDef *huart, uint8_t *String, uint16_t Size);
void StartReception(void);
void UserDataTreatment(UART_HandleTypeDef *huart, uint8_t* pData, uint16_t Size);

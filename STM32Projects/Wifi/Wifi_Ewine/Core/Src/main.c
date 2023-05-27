/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "tim.h"
#include "usb.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "esp8266.h"
#include "parson.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
typedef enum _button_interface
{
	NONE,
	B1,
	B2,
}button_interface;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t button_pushed = NONE;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */
void init_done();
static HAL_StatusTypeDef acq_internal_temp(uint16_t *temperature);
static char* formate_json_button(int value);
static char* formate_json_temp(int value);
static int check_subscription();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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
			if(topic_id == 1)
			{
				uint8_t data[data_size];
				JSON_Value *val = json_parse_string(data);
				JSON_Object *obj = json_value_get_object(val);
				int motor_state = json_object_get_number (obj, "state");
				if(motor_state == 0){
					HAL_GPIO_WritePin(Motor_GPIO_Port, Motor_Pin, RESET);
					printf("Motor OFF\n");
				}
				if(motor_state == 1){
					HAL_GPIO_WritePin(Motor_GPIO_Port, Motor_Pin, SET);
					printf("Motor ON\n");
				}
				json_free_serialized_string(data);
			}
			if(topic_id == 2)
			{
				// PUB ADC temperature
				uint16_t temp = 0;
				//acq_internal_temp(&temp);
				char *data = formate_json_temp((int)temp);
				if(esp8266_Publish_Temp_BrokerMQTT((uint8_t*)data,strlen(data)) != _PUBLISH_TEMP_MQTT)
					printf("APP: TEMP esp8266_Publish_BrokerMQTT() failed\n");
				json_free_serialized_string(data);
			}
			else
				printf("topic_id unknown %d\n",topic_id);

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
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USB_PCD_Init();
  MX_LPUART1_UART_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */

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
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  init_done();
	 	  //click button to start
	 	  printf("Waiting to start...\n");
	 	  while(HAL_GPIO_ReadPin(B3_GPIO_Port, B3_Pin) != GPIO_PIN_RESET)
	 	  {
	 		  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
	 		  HAL_Delay(100);
	 	  }
	 	  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, RESET);

	 	  printf("App started...\n");

	 	  // app part
	 	  while(1)
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
	 		if(get_mqtt_state() < _SUBSCRIBED_MOTOR_MQTT)
			  {
				  if(esp8266_Subscribe_Motor_BrokerMQTT() != _SUBSCRIBED_MOTOR_MQTT)
					printf("APP: esp8266_Subscribe_Motor_BrokerMQTT() failed\n");
			  }

	 		  switch(button_pushed)
	 		  {
	 		  case B1:
	 		  {
	 			  char *data = formate_json_button(1);
	 			  if(esp8266_Publish_Button_BrokerMQTT((uint8_t*)data,strlen(data)) != _PUBLISH_BUTTON_MQTT)
	 				printf("APP: B1 esp8266_Publish_Button_BrokerMQTT() failed\n");
	 			  json_free_serialized_string(data);
	 		  }
	 			  break;
	 		  case B2:
	 		  {
	 			  char *data = formate_json_button(2);
	 			  if(esp8266_Publish_Button_BrokerMQTT((uint8_t*)data,strlen(data)) != _PUBLISH_BUTTON_MQTT)
	 				printf("APP: B2 esp8266_Publish_Button_BrokerMQTT() failed\n");
	 		  }
	 			  break;
	 		  }
	 		  button_pushed = NONE;

	 		  int sub_status = check_subscription();
	 		  if(sub_status == 0)
	 			  printf("APP: check_subscription() SUB MQTT RCV\n");
	 	  }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Macro to configure the PLL multiplication factor
  */
  __HAL_RCC_PLL_PLLM_CONFIG(RCC_PLLM_DIV1);

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_MSI);

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE0;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN Smps */

  /* USER CODE END Smps */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	printf("CALLBACK LPUART\n");
}

int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
	//__io_putchar(*ptr++);
	ITM_SendChar(*ptr++);
	}
	return len;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
	case B1_Pin:
		button_pushed = B1;
		break;
	case B2_Pin:
		button_pushed = B2;
		break;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

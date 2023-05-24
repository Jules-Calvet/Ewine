/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_app.c
  * @author  MCD Application Team
  * @brief   Custom Example Application (Server)
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
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* First_service */
  /* USER CODE BEGIN CUSTOM_APP_Context_t */
	uint64_t ssid[30];
	uint64_t password[30];
  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SSID ((uint64_t*)0x73736964)
/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

uint8_t UpdateCharData[247];
uint8_t NotifyCharData[247];

/* USER CODE BEGIN PV */
int nb = 0;
uint64_t error = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* First_service */

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */
	int i = 0;
	// copy data update buffer +BCH
	memcpy(UpdateCharData,pNotification->DataTransfered.pPayload,pNotification->DataTransfered.Length);
  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* First_service */
    case CUSTOM_STM_GSSID_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_GSSID_READ_EVT */
      /* USER CODE END CUSTOM_STM_GSSID_READ_EVT */
      break;

    case CUSTOM_STM_GSSID_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_GSSID_WRITE_NO_RESP_EVT */
    	error = 0;
    	while(UpdateCharData[i] != 0x00) {
    		Custom_App_Context.ssid[i] = UpdateCharData[i];
    		i++;
    		if(nb != 0 && i >= nb) {
    			break;
    		}
    	}
    	nb = i;
    	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    	for(int pos=0; pos < i; pos++) {
    		error += Custom_App_Context.ssid[i-1-pos]*pow(16, pos*2);
    	}
    	if(error == SSID) {
    		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
    	} else {
    		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
    	}

    	/* USER CODE END CUSTOM_STM_GSSID_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_GPASS_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_GPASS_READ_EVT */

      /* USER CODE END CUSTOM_STM_GPASS_READ_EVT */
      break;

    case CUSTOM_STM_GPASS_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_GPASS_WRITE_NO_RESP_EVT */

      /* USER CODE END CUSTOM_STM_GPASS_WRITE_NO_RESP_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_STM_App_Notification_default */

      /* USER CODE END CUSTOM_STM_App_Notification_default */
      break;
  }
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

  /* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_APP_Notification_1 */

  /* USER CODE END CUSTOM_APP_Notification_1 */

  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

    /* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
    case CUSTOM_CONN_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */

      /* USER CODE END CUSTOM_CONN_HANDLE_EVT */
      break;

    case CUSTOM_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */

      /* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_APP_Notification_default */

      /* USER CODE END CUSTOM_APP_Notification_default */
      break;
  }

  /* USER CODE BEGIN CUSTOM_APP_Notification_2 */

  /* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void)
{
  /* USER CODE BEGIN CUSTOM_APP_Init */

  /* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/* First_service */

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/

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
#include "app_wifi.h"
#include "esp8266.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* First_service */
  uint8_t               Ack_Notification_Status;
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
char ssid[30] = "";
char password[30] = "";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* First_service */
static void Custom_Ack_Update_Char(void);
static void Custom_Ack_Send_Notification(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */
	int i = 0;
	int j = 0;
	memset(UpdateCharData, 0, sizeof(uint8_t) * 247);
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

    	memset(Custom_App_Context.ssid, 0x00, sizeof(uint64_t)*30);
    	while(UpdateCharData[i] != 0x00) {
    		Custom_App_Context.ssid[i] = (uint64_t)UpdateCharData[i];
    		i++;
    	}
    	memset(ssid, NULL, sizeof(char)*30);
    	for(int pos=0;pos<i; pos++) {
    		ssid[pos] = (char)Custom_App_Context.ssid[pos];
    	}
      /* USER CODE END CUSTOM_STM_GSSID_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_GPASS_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_GPASS_READ_EVT */

      /* USER CODE END CUSTOM_STM_GPASS_READ_EVT */
      break;

    case CUSTOM_STM_GPASS_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_GPASS_WRITE_NO_RESP_EVT */
		memset(Custom_App_Context.password, 0x00, sizeof(uint64_t)*30);
		while(UpdateCharData[j] != 0x00) {
			Custom_App_Context.password[j] = (uint64_t)UpdateCharData[j];
			j++;
		}
		memset(password, NULL, sizeof(char)*30);
		for(int pos=0;pos<j; pos++) {
			password[pos] = (char)Custom_App_Context.password[pos];
		}
		wifi_init();
		Custom_Ack_Send_Notification();
      /* USER CODE END CUSTOM_STM_GPASS_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_ACK_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_ACK_NOTIFY_ENABLED_EVT */
    	Custom_App_Context.Ack_Notification_Status = 1;        /* My_Switch_Char notification status has been enabled */
    	Custom_Ack_Send_Notification();
      /* USER CODE END CUSTOM_STM_ACK_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_ACK_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_ACK_NOTIFY_DISABLED_EVT */
    	Custom_App_Context.Ack_Notification_Status = 0;
      /* USER CODE END CUSTOM_STM_ACK_NOTIFY_DISABLED_EVT */
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
	Custom_App_Context.Ack_Notification_Status = 0;
	UTIL_SEQ_RegTask(1<<CFG_SEND_WIFI_ACK, UTIL_SEQ_RFU, Custom_Ack_Send_Notification);
	Custom_Ack_Update_Char();
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
void Custom_Ack_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ack_UC_1*/

  /* USER CODE END Ack_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_ACK, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Ack_UC_Last*/

  /* USER CODE END Ack_UC_Last*/
  return;
}

void Custom_Ack_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ack_NS_1*/
  if (Custom_App_Context.Ack_Notification_Status == 1)
  {
	  updateflag = 1;

	  if(wifi_state == _ONLINE)
	  {
		  uint8_t value = 0x01;
		  memcpy(&NotifyCharData[0],&value,sizeof(uint8_t));
	  } else {
		  uint8_t value = 0x00;
		  memcpy(&NotifyCharData[0],&value,sizeof(uint8_t));
	  }
  }
  else
  {
	  APP_DBG_MSG("-- CUSTOM APPLICATION : CAN'T INFORM CLIENT -  NOTIFICATION DISABLED\n");
  }
  /* USER CODE END Ack_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_ACK, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Ack_NS_Last*/

  /* USER CODE END Ack_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/

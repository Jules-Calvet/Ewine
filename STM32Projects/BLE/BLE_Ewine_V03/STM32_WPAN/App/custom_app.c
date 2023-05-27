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
#define FLASH_USER_START_ADDR_SSID   ADDR_FLASH_PAGE_16   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR_SSID     (ADDR_FLASH_PAGE_24 + FLASH_PAGE_SIZE - 1)   /* End @ of user Flash area */
#define FLASH_USER_START_ADDR_PASS   ADDR_FLASH_PAGE_32   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR_PASS    (ADDR_FLASH_PAGE_48 + FLASH_PAGE_SIZE - 1)   /* End @ of user Flash area */
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
uint64_t dataSsid = 0;
uint64_t dataPass = 0;

uint32_t FirstPage = 0, NbOfPages = 0;
uint32_t Address = 0, PageError = 0;
__IO uint32_t MemoryProgramStatus = 0;
__IO uint32_t data32Ssid = 0;
__IO uint32_t data32Pass = 0;


static FLASH_EraseInitTypeDef EraseInitStruct;
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
    	dataSsid = (uint64_t)ssid;
    	/* Unlock the Flash to enable the flash control register access *************/
		  HAL_FLASH_Unlock();

		 /* Clear OPTVERR bit set on virgin samples */
		  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

		  /* Erase the user Flash area
			(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

		  /* Get the 1st page to erase */
		  FirstPage = GetPage(FLASH_USER_START_ADDR_SSID);

		  /* Get the number of pages to erase from 1st page */
		  NbOfPages = GetPage(FLASH_USER_END_ADDR_SSID) - FirstPage + 1;

		  /* Fill EraseInit structure*/
		  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
		  EraseInitStruct.Page        = FirstPage;
		  EraseInitStruct.NbPages     = NbOfPages;

		  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
			 you have to make sure that these data are rewritten before they are accessed during code
			 execution. If this cannot be done safely, it is recommended to flush the caches by setting the
			 DCRST and ICRST bits in the FLASH_CR register. */
		  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
		  {
			/*
			  Error occurred while  erase.
			  User can add here some code to deal with this error.
			  PageError will contain the faulty  and then to know the code error on this ,
			  user can call function 'HAL_FLASH_GetError()'
			*/
			/* Infinite loop */
			while (1)
			{
			  /* Turn on LED3 */
			  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
			}
		  }

		  /* Program the user Flash area word by word
			(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

		  Address = FLASH_USER_START_ADDR_SSID;


		  while (Address < FLASH_USER_END_ADDR_SSID)
		  {
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, dataSsid) == HAL_OK)
			{
			  Address = Address + 8;  /* increment to next double word*/
			}
		   else
			{
			  /* Error occurred while writing data in Flash memory.
				 User can add here some code to deal with this error */
			  while (1)
			  {
				/* Turn on LED3 */
				  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
			  }
			}
		  }

		  /* Lock the Flash to disable the flash control register access (recommended
			 to protect the FLASH memory against possible unwanted operation) *********/
		  HAL_FLASH_Lock();

	  /* Check if the programmed data is OK
				 MemoryProgramStatus = 0: data programmed correctly
				 MemoryProgramStatus != 0: number of words not programmed correctly ******/
			 Address = FLASH_USER_START_ADDR_SSID;
			 MemoryProgramStatus = 0x0;

			 while (Address < FLASH_USER_END_ADDR_SSID)
			 {
			   data32Ssid = *(__IO uint32_t *)Address;

			   if (data32Ssid != dataSsid)
			   {
				 MemoryProgramStatus++;
			   }
			   Address = Address + 8;
			 }

			 /*Check if there is an issue to program data*/
			 if (MemoryProgramStatus == 0)
			 {
			   /* No error detected. Switch on LED2*/
			  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
			 }
			 else
			 {
			   /* Error detected. Switch on LED1*/
			  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
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
		dataPass = (uint64_t)password;
		wifi_init();

		/* Unlock the Flash to enable the flash control register access *************/
		  HAL_FLASH_Unlock();

		 /* Clear OPTVERR bit set on virgin samples */
		  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

		  /* Erase the user Flash area
			(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

		  /* Get the 1st page to erase */
		  FirstPage = GetPage(FLASH_USER_START_ADDR_PASS);

		  /* Get the number of pages to erase from 1st page */
		  NbOfPages = GetPage(FLASH_USER_END_ADDR_PASS) - FirstPage + 1;

		  /* Fill EraseInit structure*/
		  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
		  EraseInitStruct.Page        = FirstPage;
		  EraseInitStruct.NbPages     = NbOfPages;

		  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
			 you have to make sure that these data are rewritten before they are accessed during code
			 execution. If this cannot be done safely, it is recommended to flush the caches by setting the
			 DCRST and ICRST bits in the FLASH_CR register. */
		  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
		  {
			/*
			  Error occurred while  erase.
			  User can add here some code to deal with this error.
			  PageError will contain the faulty  and then to know the code error on this ,
			  user can call function 'HAL_FLASH_GetError()'
			*/
			/* Infinite loop */
			while (1)
			{
			  /* Turn on LED3 */
			  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
			}
		  }

		  /* Program the user Flash area word by word
			(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

		  Address = FLASH_USER_START_ADDR_PASS;


		  while (Address < FLASH_USER_END_ADDR_PASS)
		  {
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, dataPass) == HAL_OK)
			{
			  Address = Address + 8;  /* increment to next double word*/
			}
		   else
			{
			  /* Error occurred while writing data in Flash memory.
				 User can add here some code to deal with this error */
			  while (1)
			  {
				/* Turn on LED3 */
				  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
			  }
			}
		  }

		  /* Lock the Flash to disable the flash control register access (recommended
			 to protect the FLASH memory against possible unwanted operation) *********/
		  HAL_FLASH_Lock();

	  /* Check if the programmed data is OK
				 MemoryProgramStatus = 0: data programmed correctly
				 MemoryProgramStatus != 0: number of words not programmed correctly ******/
			 Address = FLASH_USER_START_ADDR_PASS;
			 MemoryProgramStatus = 0x0;

			 while (Address < FLASH_USER_END_ADDR_PASS)
			 {
			   data32Pass = *(__IO uint32_t *)Address;

			   if (data32Pass != dataPass)
			   {
				 MemoryProgramStatus++;
			   }
			   Address = Address + 8;
			 }

			 /*Check if there is an issue to program data*/
			 if (MemoryProgramStatus == 0)
			 {
			   /* No error detected. Switch on LED2*/
			  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
			 }
			 else
			 {
			   /* Error detected. Switch on LED1*/
			  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
			 }
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

/**
  ******************************************************************************
  * @file          : app_tof.c
  * @author        : IMG SW Application Team
  * @brief         : This file provides code for the configuration
  *                  of the STMicroelectronics.X-CUBE-TOF1.3.1.0 instances.
  ******************************************************************************
  *
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_tof.h"
#include "main.h"
#include "stdio.h"
#include "53l1a2_ranging_sensor.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TIMING_BUDGET (30U) /* 16 ms < TimingBudget < 500 ms */
#define REPETITION (300U) /* refresh rate for polling mode (milliseconds, shall be consistent with TimingBudget value) */

/* Private variables ---------------------------------------------------------*/
static RANGING_SENSOR_ProfileConfig_t ProfileCenter;
static RANGING_SENSOR_ProfileConfig_t ProfileLeft;
//static RANGING_SENSOR_ProfileConfig_t ProfileRight;
static int32_t statusCenter = 0;
static int32_t statusLeft = 0;
//static int32_t statusRight = 0;

/* Private function prototypes -----------------------------------------------*/
static void VL_53L1A2_SimpleRanging_Init(void);
static void VL_53L1A2_SimpleRanging_Process(void);
static void print_result(RANGING_SENSOR_Result_t *Result);

void VL_TOF_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN TOF_Init_PreTreatment */

  /* USER CODE END TOF_Init_PreTreatment */

  /* Initialize the peripherals and the TOF components */

  VL_53L1A2_SimpleRanging_Init();

  /* USER CODE BEGIN TOF_Init_PostTreatment */

  /* USER CODE END TOF_Init_PostTreatment */
}

/*
 * LM background task
 */
void VL_TOF_Process(void)
{
  /* USER CODE BEGIN TOF_Process_PreTreatment */

  /* USER CODE END TOF_Process_PreTreatment */

  VL_53L1A2_SimpleRanging_Process();

  /* USER CODE BEGIN TOF_Process_PostTreatment */

  /* USER CODE END TOF_Process_PostTreatment */
}

static void VL_53L1A2_SimpleRanging_Init(void)
{
  // initialization of the center tof
  statusCenter = VL53L1A2_RANGING_SENSOR_Init(VL53L1A2_DEV_CENTER);
  statusLeft = VL53L1A2_RANGING_SENSOR_Init(VL53L1A2_DEV_LEFT);
  //statusRight = VL53L1A2_RANGING_SENSOR_Init(VL53L1A2_DEV_RIGHT);

  // DEBUG ERROR
  if (statusCenter != BSP_ERROR_NONE)
  {
    printf("VL53L1A2_RANGING_SENSOR_Init failed\n");
    while(1);
  }

  if (statusLeft != BSP_ERROR_NONE)
    {
      printf("VL53L1A2_RANGING_SENSOR_Init failed\n");
      while(1);
    }/*
  if (statusRight != BSP_ERROR_NONE)
    {
      printf("VL53L1A2_RANGING_SENSOR_Init failed\n");
      while(1);
    }*/

}

static void VL_53L1A2_SimpleRanging_Process(void)
{
  uint32_t Id_Center;
  uint32_t Id_Left;
  //uint32_t Id_Right;
  RANGING_SENSOR_Result_t Result_Center;
  RANGING_SENSOR_Result_t Result_Left;
  //RANGING_SENSOR_Result_t Result_Right;

  // verify id of sensor
  VL53L1A2_RANGING_SENSOR_ReadID(VL53L1A2_DEV_CENTER, &Id_Center);
  VL53L1A2_RANGING_SENSOR_ReadID(VL53L1A2_DEV_LEFT, &Id_Left);
  //VL53L1A2_RANGING_SENSOR_ReadID(VL53L1A2_DEV_RIGHT, &Id_Right);

  // sensor configuration
  ProfileCenter.RangingProfile = RS_MULTI_TARGET_SHORT_RANGE;
  ProfileCenter.TimingBudget = TIMING_BUDGET;
  ProfileCenter.Frequency = 0; // Induces intermeasurement period, set to ZERO for normal ranging
  ProfileCenter.EnableAmbient = 0; // Enable: 1, Disable: 0
  ProfileCenter.EnableSignal = 0; // Enable: 1, Disable: 0


  ProfileLeft.RangingProfile = RS_MULTI_TARGET_SHORT_RANGE;
  ProfileLeft.TimingBudget = TIMING_BUDGET;
  ProfileLeft.Frequency = 0; // Induces intermeasurement period, set to ZERO for normal ranging
  ProfileLeft.EnableAmbient = 0; // Enable: 1, Disable: 0
  ProfileLeft.EnableSignal = 0; // Enable: 1, Disable: 0
/*
  ProfileRight.RangingProfile = RS_MULTI_TARGET_SHORT_RANGE;
  ProfileRight.TimingBudget = TIMING_BUDGET;
  ProfileRight.Frequency = 0; // Induces intermeasurement period, set to ZERO for normal ranging
  ProfileRight.EnableAmbient = 0; // Enable: 1, Disable: 0
  ProfileRight.EnableSignal = 0; // Enable: 1, Disable: 0
*/
  // set the profile if different from default one
  VL53L1A2_RANGING_SENSOR_ConfigProfile(VL53L1A2_DEV_CENTER, &ProfileCenter);
  VL53L1A2_RANGING_SENSOR_ConfigProfile(VL53L1A2_DEV_LEFT, &ProfileLeft);
  //VL53L1A2_RANGING_SENSOR_ConfigProfile(VL53L1A2_DEV_RIGHT, &ProfileRight);

  statusCenter = VL53L1A2_RANGING_SENSOR_Start(VL53L1A2_DEV_CENTER, RS_MODE_BLOCKING_CONTINUOUS);
  statusLeft = VL53L1A2_RANGING_SENSOR_Start(VL53L1A2_DEV_LEFT, RS_MODE_BLOCKING_CONTINUOUS);

  if (statusCenter != BSP_ERROR_NONE)
  {
    printf("VL53L1A2_RANGING_SENSOR_Start_Center failed\n");
    while(1);
  }
  if (statusLeft != BSP_ERROR_NONE)
    {
      printf("VL53L1A2_RANGING_SENSOR_Start_Left failed\n");
      while(1);
    }/*
  if (statusRight != BSP_ERROR_NONE)
    {
      printf("VL53L1A2_RANGING_SENSOR_Start_Right failed\n");
      while(1);
    }*/
  printf("Initialization OK ! \n");
  while (1)
  {
	  printf("Start Loop ! \n");
    /* repetition mode */
	  statusCenter = VL53L1A2_RANGING_SENSOR_GetDistance(VL53L1A2_DEV_CENTER, &Result_Center);
	  if (statusCenter == BSP_ERROR_NONE)
	  {
		  printf("\n CENTER ");
		  print_result(&Result_Center);
	  }
	  //statusCenter = VL53L1A2_RANGING_SENSOR_Stop(VL53L1A2_DEV_CENTER);
	  statusCenter = VL53L1A2_RANGING_SENSOR_SetPowerMode(VL53L1A2_DEV_CENTER, RANGING_SENSOR_POWERMODE_ON)
	  HAL_Delay(REPETITION);


	  statusLeft = VL53L1A2_RANGING_SENSOR_GetDistance(VL53L1A2_DEV_LEFT, &Result_Left);
	  if (statusLeft == BSP_ERROR_NONE)
	  {
		  printf("\n LEFT ");
		  print_result(&Result_Left);
	  }
	  HAL_Delay(REPETITION);

/*
	  statusRight = VL53L1A2_RANGING_SENSOR_Start(VL53L1A2_DEV_RIGHT, RS_MODE_BLOCKING_CONTINUOUS);
		statusRight = VL53L1A2_RANGING_SENSOR_GetDistance(VL53L1A2_DEV_RIGHT, &Result_Right);
		printf("\n RIGHT ");
	  print_result(&Result_Right);
	  statusLeft = VL53L1A2_RANGING_SENSOR_Stop(VL53L1A2_DEV_LEFT);
	  HAL_Delay(REPETITION);
*/

  }
}

static void print_result(RANGING_SENSOR_Result_t *Result)
{
  uint8_t i, j;

  for (i = 0; i < RANGING_SENSOR_MAX_NB_ZONES; i++)
  {
    printf("\nTargets = %lu", (unsigned long)Result->ZoneResult[i].NumberOfTargets);

    for (j = 0; j < Result->ZoneResult[i].NumberOfTargets; j++)
    {
      printf("\n |---> ");
      printf("Status = %ld, Distance = %5ld mm ",
        (long)Result->ZoneResult[i].Status[j],
        (long)Result->ZoneResult[i].Distance[j]);
    }
  }
  printf ("\n");
}

#ifdef __cplusplus
}
#endif

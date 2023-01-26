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
#define REPETITION_TIMING (1500U) /* refresh rate for polling mode (milliseconds, shall be consistent with TimingBudget value) */

/* Private variables ---------------------------------------------------------*/
static int REPETITION_NUMBER = 11;
static RANGING_SENSOR_ProfileConfig_t ProfileCenter;
static RANGING_SENSOR_ProfileConfig_t ProfileRight;
static RANGING_SENSOR_ProfileConfig_t ProfileLeft;
static int32_t statusCenter = 0;
static int32_t statusRight = 0;
static int32_t statusLeft = 0;

/* Private function prototypes -----------------------------------------------*/
static void VL_53L1A2_SimpleRanging_Init_C(void);
static void VL_53L1A2_SimpleRanging_Init_R(void);
static void VL_53L1A2_SimpleRanging_Init_L(void);
static void VL_53L1A2_SimpleRanging_Process_C(void);
static void VL_53L1A2_SimpleRanging_Process_R(void);
static void VL_53L1A2_SimpleRanging_Process_L(void);
static void print_result(RANGING_SENSOR_Result_t *Result);

void VL_TOF_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN TOF_Init_PreTreatment */

	REPETITION_NUMBER = 10 ;

  /* USER CODE END TOF_Init_PreTreatment */

  /* Initialize the peripherals and the TOF components */

  VL_53L1A2_SimpleRanging_Init_C();
  //VL_53L1A2_SimpleRanging_Init_R();
  //VL_53L1A2_SimpleRanging_Init_L();

  /* USER CODE BEGIN TOF_Init_PostTreatment */

  /* USER CODE END TOF_Init_PostTreatment */
}

/*
 * LM background task
 */
void VL_TOF_Process(void)
{
  /* USER CODE BEGIN TOF_Process_PreTreatment */
  printf("\n\n ****** Start TOF Test ****** \n\n");
  /* USER CODE END TOF_Process_PreTreatment */
  printf("\n ---- start TOF Center ---- \n");
  VL_53L1A2_SimpleRanging_Process_C();
  printf("\n ---- end TOF Center ---- \n");
  HAL_Delay(2000);
  /*printf("\n ---- start TOF Right ---- \n");
  VL_53L1A2_SimpleRanging_Process_R();
  printf("\n ---- end TOF Right ---- \n");
  HAL_Delay(2000);
  printf("\n ---- start TOF Left ---- \n");
  VL_53L1A2_SimpleRanging_Process_L();
  printf("\n ---- end TOF Left ---- \n");
  HAL_Delay(2000);*/
  /* USER CODE BEGIN TOF_Process_PostTreatment */
  printf("\n\n ****** End TOF Test ****** \n\n");
  /* USER CODE END TOF_Process_PostTreatment */
}

static void VL_53L1A2_SimpleRanging_Init_C(void)
{
  // initialization of the center tof
  statusCenter = VL53L1A2_RANGING_SENSOR_Init(VL53L1A2_DEV_CENTER);

  // DEBUG ERROR
  if (statusCenter != BSP_ERROR_NONE)
  {
    printf("VL53L1A2_RANGING_SENSOR_Init failed\n");
    while(1);
  }
  else printf(" ****** Initialization TOF CENTER OK ****** \n");
}
static void VL_53L1A2_SimpleRanging_Init_R(void)
{
  // initialization of the center tof
  statusRight = VL53L1A2_RANGING_SENSOR_Init(VL53L1A2_DEV_RIGHT);

  // DEBUG ERROR
  if (statusRight != BSP_ERROR_NONE)
  {
    printf("VL53L1A2_RANGING_SENSOR_Init failed\n");
    while(1);
  }
  else printf(" ****** Initialization TOF RIGHT OK ****** \n");
}
static void VL_53L1A2_SimpleRanging_Init_L(void)
{
  // initialization of the center tof
  statusLeft = VL53L1A2_RANGING_SENSOR_Init(VL53L1A2_DEV_LEFT);

  // DEBUG ERROR
  if (statusLeft != BSP_ERROR_NONE)
  {
    printf("VL53L1A2_RANGING_SENSOR_Init failed\n");
    while(1);
  }
  else printf(" ****** Initialization TOF LEFT OK ****** \n");
}

static void VL_53L1A2_SimpleRanging_Process_C(void)
{
  uint32_t Id_Center;
  RANGING_SENSOR_Result_t Result_Center;

  // verify id of sensor
  VL53L1A2_RANGING_SENSOR_ReadID(VL53L1A2_DEV_CENTER, &Id_Center);

  // sensor configuration
  ProfileCenter.RangingProfile = RS_MULTI_TARGET_SHORT_RANGE;
  ProfileCenter.TimingBudget = TIMING_BUDGET;
  ProfileCenter.Frequency = 0; // Induces intermeasurement period, set to ZERO for normal ranging
  ProfileCenter.EnableAmbient = 0; // Enable: 1, Disable: 0
  ProfileCenter.EnableSignal = 0; // Enable: 1, Disable: 0

  // set the profile if different from default one
  VL53L1A2_RANGING_SENSOR_ConfigProfile(VL53L1A2_DEV_CENTER, &ProfileCenter);

  statusCenter = VL53L1A2_RANGING_SENSOR_Start(VL53L1A2_DEV_CENTER, RS_MODE_BLOCKING_CONTINUOUS);

  if (statusCenter != BSP_ERROR_NONE)
  {
    printf("VL53L1A2_RANGING_SENSOR_Start_Center failed\n");
    while(1);
  }
/*
  while (1)
  {*/
  for(int i = 1 ; i < REPETITION_NUMBER ; i++){
    /* repetition mode */
    statusCenter = VL53L1A2_RANGING_SENSOR_GetDistance(VL53L1A2_DEV_CENTER, &Result_Center);

    if (statusCenter == BSP_ERROR_NONE)
    {
      printf("\n CENTER %d\n",i);
      print_result(&Result_Center);
    }

    HAL_Delay(REPETITION_TIMING);
  }
  statusCenter = VL53L1A2_RANGING_SENSOR_Stop(VL53L1A2_DEV_CENTER);
}

static void VL_53L1A2_SimpleRanging_Process_R(void)
{
  uint32_t Id_Right;
  RANGING_SENSOR_Result_t Result_Right;

  // verify id of sensor
  VL53L1A2_RANGING_SENSOR_ReadID(VL53L1A2_DEV_RIGHT, &Id_Right);

  // sensor configuration
  ProfileRight.RangingProfile = RS_MULTI_TARGET_SHORT_RANGE;
  ProfileRight.TimingBudget = TIMING_BUDGET;
  ProfileRight.Frequency = 0; // Induces intermeasurement period, set to ZERO for normal ranging
  ProfileRight.EnableAmbient = 0; // Enable: 1, Disable: 0
  ProfileRight.EnableSignal = 0; // Enable: 1, Disable: 0

  // set the profile if different from default one
  VL53L1A2_RANGING_SENSOR_ConfigProfile(VL53L1A2_DEV_RIGHT, &ProfileRight);

  statusRight = VL53L1A2_RANGING_SENSOR_Start(VL53L1A2_DEV_RIGHT, RS_MODE_BLOCKING_CONTINUOUS);

  if (statusRight != BSP_ERROR_NONE)
  {
    printf("VL53L1A2_RANGING_SENSOR_Start_Right failed\n");
    while(1);
  }
/*
  while (1)
  {*/
  for(int i = 1 ; i < REPETITION_NUMBER ; i++){
    /* repetition mode */
    statusRight = VL53L1A2_RANGING_SENSOR_GetDistance(VL53L1A2_DEV_RIGHT, &Result_Right);

    if (statusRight == BSP_ERROR_NONE)
    {
      printf("\n RIGHT %d\n",i);
      print_result(&Result_Right);
    }

    HAL_Delay(REPETITION_TIMING);

  }
  statusRight = VL53L1A2_RANGING_SENSOR_Stop(VL53L1A2_DEV_RIGHT);
}
static void VL_53L1A2_SimpleRanging_Process_L(void)
{
  uint32_t Id_Left;
  RANGING_SENSOR_Result_t Result_Left;

  // verify id of sensor
  VL53L1A2_RANGING_SENSOR_ReadID(VL53L1A2_DEV_LEFT, &Id_Left);

  // sensor configuration
  ProfileLeft.RangingProfile = RS_MULTI_TARGET_SHORT_RANGE;
  ProfileLeft.TimingBudget = TIMING_BUDGET;
  ProfileLeft.Frequency = 0; // Induces intermeasurement period, set to ZERO for normal ranging
  ProfileLeft.EnableAmbient = 0; // Enable: 1, Disable: 0
  ProfileLeft.EnableSignal = 0; // Enable: 1, Disable: 0

  // set the profile if different from default one
  VL53L1A2_RANGING_SENSOR_ConfigProfile(VL53L1A2_DEV_LEFT, &ProfileLeft);

  statusLeft = VL53L1A2_RANGING_SENSOR_Start(VL53L1A2_DEV_LEFT, RS_MODE_BLOCKING_CONTINUOUS);

  if (statusLeft != BSP_ERROR_NONE)
  {
    printf("VL53L1A2_RANGING_SENSOR_Start_Left failed\n");
    while(1);
  }
/*
  while (1)
  {*/
  for(int i = 1 ; i < REPETITION_NUMBER ; i++){
    /* repetition mode */
    statusLeft = VL53L1A2_RANGING_SENSOR_GetDistance(VL53L1A2_DEV_LEFT, &Result_Left);

    if (statusLeft == BSP_ERROR_NONE)
    {
      printf("\n LEFT %d\n",i);
      print_result(&Result_Left);
    }

    HAL_Delay(REPETITION_TIMING);

  }
  statusLeft = VL53L1A2_RANGING_SENSOR_Stop(VL53L1A2_DEV_LEFT);
}

static void print_result(RANGING_SENSOR_Result_t *Result)
{
  uint8_t i, j;

  for (i = 0; i < RANGING_SENSOR_MAX_NB_ZONES; i++)
  {
    printf("\nTargets = %lu", (unsigned long)Result->ZoneResult[i].NumberOfTargets);

    for (j = 0; j < Result->ZoneResult[i].NumberOfTargets; j++)
    {
      printf(" |---> ");
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

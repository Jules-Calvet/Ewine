


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "mlx90614.h"
#include "app_mlx.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static float temp = 0.0f;

/* Private function prototypes -----------------------------------------------*/
static void MLX_90614_AmbiantTemperature_Process(void);

void MLX_IR_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN IR_Init_PreTreatment */

  /* USER CODE END IR_Init_PreTreatment */

  /* Initialize the peripherals and the IR components */

	if(mlx90614_init() == true)printf(" ****** Initialization MLX90614 OK ****** \n");
	else printf("Initialization Failed ! \n");

  /* USER CODE BEGIN IR_Init_PostTreatment */
	mlx90614.configReg.DualIRSensor = 0;
	mlx90614.configReg.SelectObjAmb = 1;
	mlx90614.configReg.FIR = 4;
  /* USER CODE END IR_Init_PostTreatment */
}

void MLX_IR_Process(void)
{
  /* USER CODE BEGIN IR_Process_PreTreatment */

  /* USER CODE END IR_Process_PreTreatment */

	printf("\n\n ****** Start MLX90614 Test ****** \n\n");
	MLX_90614_AmbiantTemperature_Process();
	printf("\n\n ****** End MLX90614 Test ****** \n\n");

  /* USER CODE BEGIN IR_Process_PostTreatment */

  /* USER CODE END IR_Process_PostTreatment */
}

static void MLX_90614_AmbiantTemperature_Process(void)
{
  for(int i = 1 ; i < 10 ; i++ ){
	  printf("loop %d start\n",i);

	  if ( mlx90614_getAmbient(&temp) == true ) printf("getAmbient OK : %0.2f *C\n", temp);
	  else printf("Failed to read ambient temperature\n");

	  printf("loop %d end\n",i);
	  HAL_Delay(1500);
  }
}

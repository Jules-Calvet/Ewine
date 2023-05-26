void IR_Init();
void IR_Process();

//app_ToF.c printf function to put back in after every modif in the .ioc
/*
static void print_result(uint32_t Instance, RANGING_SENSOR_Result_t *Result)
{
  uint8_t i;

  for (i = 0; i < RANGING_SENSOR_MAX_NB_ZONES; i++)
  {
	  //JN
	  if((long)Result->ZoneResult[i].Distance[0] > 10 && (long)Result->ZoneResult[i].Distance[0] < 100){
		if(Instance == 0){
			printf ("Bottle in left position\n");
			if(LeftPos != 2){
				LeftPos = 1;
			}
		}
		if(Instance == 1){
			printf ("Bottle in center position\n");
			if(CenterPos != 2){
				CenterPos = 1;
			}
		}
		if(Instance == 2){
			printf ("Bottle in right position\n");
			if(RightPos != 2){
				RightPos = 1;
			}
		}
	 } else {
		 if(Instance == 0){
			 LeftPos = 0;
		 }
		 if(Instance == 1){
			CenterPos = 0;
		 }
		 if(Instance == 2){
			 RightPos = 0;
		 }
	 }
  }
  printf ("\n");
}
*/

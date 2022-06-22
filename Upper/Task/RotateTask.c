#include "RotateTask.h"

extern Pid_Typedef Pid_Rotate_Motor_Speed,Pid_Rotate_Motor_Positioin;//pid结构体
extern int RotatePositionInit , RotateJourney;
extern int volatile Pos_Rotate_Motor;


int rotate_set;			//旋转电机位置设定

uint32_t rotate_high_water;

void motorRotate(void)
{
	
	Pid_Rotate_Motor_Speed.SetPoint = (g_Flag.rotate_s - g_Flag.rotate_w)  * 1000.0;
	
	
	
	
//	if(g_Flag.rotate_flag == 0)
//	{
////					Pid_Rotate_Motor_Positioin.SetPoint = Pos_Rotate_Motor;
//		rotate_set = rotate_set;
//	}else if(g_Flag.rotate_flag == 1)			//短按
//	{
//		rotate_set = Pos_Rotate_Motor + RotateJourney;
//		g_Flag.rotate_flag = 0;
//	}else if(g_Flag.rotate_flag == 2)
//	{
//		rotate_set = Pos_Rotate_Motor - RotateJourney;
//		g_Flag.rotate_flag = 0;
//	}else if(g_Flag.rotate_flag == 3)			//长按
//	{
//		rotate_set += 100;
//	}else if(g_Flag.rotate_flag == 4)
//	{
//		rotate_set -= 100;
//	}
//	
//		Pid_Rotate_Motor_Positioin.SetPoint = rotate_set;
}


void rotate_Task(void *pvParameters)
{
	
//	portTickType xLastWakeTime;
//	const portTickType xFrequency=1;
	
   while (1) {
		 
			motorRotate();
		 
			vTaskDelay(1);

//			xLastWakeTime=xTaskGetTickCount();
//			vTaskDelayUntil(&xLastWakeTime,xFrequency);
		 
//#if INCLUDE_uxTaskGetStackHighWaterMark
//        rotate_high_water = uxTaskGetStackHighWaterMark(NULL);
//#endif
    }
}


















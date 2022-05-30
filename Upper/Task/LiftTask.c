#include "LiftTask.h"

uint32_t Lift_high_water;

extern Pid_Typedef Pid_LiftTrack_Speed[2],Pid_LiftTrack_Positioin[2];//pid结构体
//extern float Lift_pos_set_pre[2];							//记录抬升电机位置环上次的设定值
extern int LiftPositionInit[2],LiftJourney_1,LiftJourney_2,lift_once_offset;
extern int volatile Pos_LiftTrack[2];


int lift_pos_set[2],Lift_pos_set_pre[2];		//设定值,上一值
float test_p = 0.84;
int test_ii = 1;
void motor_lift_task(void)
{
		if(g_Flag.lift_once_flag == 0) //如果上层抬升标志位为0
		{
			Lift_pos_set_pre[0] = LiftPositionInit[0] - lift_once_offset - LiftJourney_1;										//记录另一个状态
			Lift_pos_set_pre[1] = LiftPositionInit[1] + lift_once_offset + LiftJourney_1;
			
			lift_pos_set[0] = LiftPositionInit[0] - lift_once_offset;									//修改目标值
			lift_pos_set[1] = LiftPositionInit[1] + lift_once_offset;
			
							Pid_LiftTrack_Positioin[0].P = 0.05f;
							Pid_LiftTrack_Positioin[0].I = 0.001f;
							Pid_LiftTrack_Positioin[0].D = 0.00f;
							Pid_LiftTrack_Positioin[0].IMax = 10000.0f;
			
		}
		else if(g_Flag.lift_once_flag == 1)//如果上层抬升标志位为1
		{
			Lift_pos_set_pre[0] = LiftPositionInit[0] - lift_once_offset;																	//记录另一个状态
			Lift_pos_set_pre[1] = LiftPositionInit[1] - lift_once_offset;
			
			lift_pos_set[0] = LiftPositionInit[0] - lift_once_offset - LiftJourney_1;									//修改目标值
			lift_pos_set[1] = LiftPositionInit[1] + lift_once_offset + LiftJourney_1;
			
							Pid_LiftTrack_Positioin[0].P = 1.5f;
							Pid_LiftTrack_Positioin[0].I = 0.002f;
							Pid_LiftTrack_Positioin[0].D = 0.00f;
							Pid_LiftTrack_Positioin[0].IMax = 10000.0f;
		}
		else if(g_Flag.lift_once_flag == 2)//如果上层抬升标志位为2
		{
			Lift_pos_set_pre[0] = LiftPositionInit[0] - lift_once_offset;																	//记录另一个状态
			Lift_pos_set_pre[1] = LiftPositionInit[1] - lift_once_offset;
			
			lift_pos_set[0] = LiftPositionInit[0] - lift_once_offset - LiftJourney_2;									//修改目标值
			lift_pos_set[1] = LiftPositionInit[1] + lift_once_offset + LiftJourney_2;
			
							Pid_LiftTrack_Positioin[0].P = 0.1f;
							Pid_LiftTrack_Positioin[0].I = 0.001f;
							Pid_LiftTrack_Positioin[0].D = 0.000f;
							Pid_LiftTrack_Positioin[0].IMax = 10000.0f;
		}
		
		
		if(ABS(Pid_LiftTrack_Positioin[0].SetPoint - Pos_LiftTrack[0]) < 50)
		{
						Pid_LiftTrack_Positioin[0].P = 0.5f;
						Pid_LiftTrack_Positioin[0].I = 0.02f;
						Pid_LiftTrack_Positioin[0].D = 0.00f;
						Pid_LiftTrack_Positioin[0].IMax = 500.0f;
			
			g_Flag.lift_once_flag_pre = g_Flag.lift_once_flag;
		}
		
		
		Pid_LiftTrack_Positioin[0].SetPoint = lift_pos_set[0];
		
		
//		if(ABS(Pos_LiftTrack[0]-Lift_pos_set_pre[0]) < 0.9 * ABS(lift_pos_set[0]-Lift_pos_set_pre[0]) )
//				Pid_LiftTrack_Positioin[0].SetPoint = Lift_pos_set_pre[0] + test_p * (lift_pos_set[0]-Lift_pos_set_pre[0]);
//		else
//				Pid_LiftTrack_Positioin[0].SetPoint = lift_pos_set[0];
		
		
		
//	}
	
}


void Lift_task(void *pvParameters)
{
	
//	portTickType xLastWakeTime;
//	const portTickType xFrequency=1;
//	
  while (1) {
		motor_lift_task();
	   
		vTaskDelay(1);

//    xLastWakeTime=xTaskGetTickCount();
//		vTaskDelayUntil(&xLastWakeTime,xFrequency);
//		 
//		#if INCLUDE_uxTaskGetStackHighWaterMark
//						Lift_high_water = uxTaskGetStackHighWaterMark(NULL);
//		#endif
  }
}


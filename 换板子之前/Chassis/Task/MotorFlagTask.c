#include "MotorFlagTask.h"

uint32_t motor_flag_high_water;


extern pid_Typedef Pid_LiftTrack_Speed[2],Pid_LiftTrack_Positioin[2], pid_warehouse_speed, pid_warehouse_position;//pid�ṹ��
extern int LiftPositionInit[2],LiftJourney_1,LiftJourney_2,lift_offset, warehouse_position_init, warehouse_journey;//��ʼλ��,����λ�ã��г̾���




void motorFlagTask(void  *pvParameters)
{

//		portTickType xLastWakeTime;
//	const portTickType xFrequency=1;
		
	while(1){
		
		motorFlag();
		
		vTaskDelay(1);
//		xLastWakeTime=xTaskGetTickCount();
//		vTaskDelayUntil(&xLastWakeTime,xFrequency);
		 
		#if INCLUDE_uxTaskGetStackHighWaterMark
        motor_flag_high_water = uxTaskGetStackHighWaterMark(NULL);
		#endif
  }
}


void motorFlag(void)
{
	if(g_Flag.lift_down_twice_flag == 0)
	{ 
		Pid_LiftTrack_Positioin[0].SetPoint = LiftPositionInit[0] - lift_offset;
		Pid_LiftTrack_Positioin[1].SetPoint = LiftPositionInit[1] + lift_offset;
	}else if (g_Flag.lift_down_twice_flag == 1)
	{
		Pid_LiftTrack_Positioin[0].SetPoint = LiftPositionInit[0] - lift_offset - LiftJourney_1;
		Pid_LiftTrack_Positioin[1].SetPoint = LiftPositionInit[1] + lift_offset + LiftJourney_1;
	}else if (g_Flag.lift_down_twice_flag == 2)
	{
		Pid_LiftTrack_Positioin[0].SetPoint = LiftPositionInit[0] - lift_offset - LiftJourney_2;
		Pid_LiftTrack_Positioin[1].SetPoint = LiftPositionInit[1] + lift_offset + LiftJourney_2;
	}
	
	/////////////////////////////////////////////////////�ֿ����߼����ԣ��д��޸�///////////////////////////////////////
	if(g_Flag.warehouse_flag == 0)
	{
		pid_warehouse_position.SetPoint = warehouse_position_init;
	}else if(g_Flag.warehouse_flag == 1)
	{
		pid_warehouse_position.SetPoint = warehouse_position_init + warehouse_journey;
	}else if(g_Flag.warehouse_flag == 2)
	{
		pid_warehouse_position.SetPoint = warehouse_position_init - warehouse_journey;
	}
	
}





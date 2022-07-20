//#include "MotorFlagTask.h"

//uint32_t motor_flag_high_water;


//extern pid_Typedef Pid_LiftTrack_Speed[2],Pid_LiftTrack_Positioin[2], pid_warehouse_speed, pid_warehouse_position;//pid结构体
//extern int LiftPositionInit[2],LiftJourney_1,LiftJourney_2,LiftJourney_3,lift_offset, warehouse_journey;//初始位置,结束位置，行程距离
////int warehouse_set;		//仓库旋转电机设定值



//void motorFlagTask(void  *pvParameters)
//{

////		portTickType xLastWakeTime;
////	const portTickType xFrequency=1;
//		
//	while(1){
//		
//		motorFlag();
//		
//		vTaskDelay(1);
////		xLastWakeTime=xTaskGetTickCount();
////		vTaskDelayUntil(&xLastWakeTime,xFrequency);
//		 
////		#if INCLUDE_uxTaskGetStackHighWaterMark
////        motor_flag_high_water = uxTaskGetStackHighWaterMark(NULL);
////		#endif
//  }
//}

//void motorFlag(void)
//{
////	if(g_Flag.lift_down_twice_flag == 0)
////	{ 
////		Pid_LiftTrack_Positioin[0].SetPoint = LiftPositionInit[0] - lift_offset;
////		Pid_LiftTrack_Positioin[1].SetPoint = LiftPositionInit[1] + lift_offset;
////	}else if (g_Flag.lift_down_twice_flag == 1)
////	{
////		Pid_LiftTrack_Positioin[0].SetPoint = LiftPositionInit[0] - lift_offset - LiftJourney_1;
////		Pid_LiftTrack_Positioin[1].SetPoint = LiftPositionInit[1] + lift_offset + LiftJourney_1;
////	}else if (g_Flag.lift_down_twice_flag == 2)
////	{
////		Pid_LiftTrack_Positioin[0].SetPoint = LiftPositionInit[0] - lift_offset - LiftJourney_2;
////		Pid_LiftTrack_Positioin[1].SetPoint = LiftPositionInit[1] + lift_offset + LiftJourney_2;
////	}else if (g_Flag.lift_down_twice_flag == 3)
////	{
////		Pid_LiftTrack_Positioin[0].SetPoint = LiftPositionInit[0] - lift_offset - LiftJourney_3;
////		Pid_LiftTrack_Positioin[1].SetPoint = LiftPositionInit[1] + lift_offset + LiftJourney_3;
////	}
//	
////	//仓库电机
////	if(rc_ctrl.key.ctrl == 1 && rc_ctrl.key.shift == 0 )
////	{
////		pid_warehouse_speed.SetPoint = (rc_ctrl.key.a - rc_ctrl.key.d)  * 1000.0;
////	}else
////		pid_warehouse_speed.SetPoint = 0;
////	
//	
//	
////	if(g_Flag.warehouse_flag == 0)
////	{
////		warehouse_set = warehouse_set;
////	}else if(g_Flag.warehouse_flag == 1)
////	{
////		warehouse_set = warehouse_set + warehouse_journey;
////		g_Flag.warehouse_flag = 0;
////	}else if(g_Flag.warehouse_flag == 2)
////	{
////		warehouse_set = warehouse_set - warehouse_journey;
////		g_Flag.warehouse_flag = 0;
////	}
////	
////	pid_warehouse_position.SetPoint = warehouse_set;
//	
//}





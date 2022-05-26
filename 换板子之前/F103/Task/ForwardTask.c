#include "ForwardTask.h"

uint32_t Forward_high_water;
//int  forward_flag, forward_flag_old;

////extern Pid_Typedef Pid_Forward_Motor_Position;//pid结构体
//extern Pid_Typedef Pid_Forward_Motor_speed;//pid结构体
//extern int ForwardPositionInit , ForwardJourney;
//extern unsigned char data_receive[8]; //C板接收的数据
//extern float vf;						//前移电机速度

//extern Pid_Typedef Pid_Forward_Motor_speed[2],Pid_Forward_Motor_Position[2];//pid结构体
//extern int ForwardPositionInit[2] , ForwardJourney;

void motor_forward_task(void)
{
	//	if(data_receive[0] == '!' && data_receive[1] == 1 && data_receive[2] == 2)   //如果用遥控器控制并且运动模式为抬升模式
//	{
//		if(data_receive[3] == 0) //如果上层抬升标志位为0
//		{
//			Pid_Forward_Motor_Position[0].SetPoint = ForwardPositionInit[0];
//			Pid_Forward_Motor_Position[1].SetPoint = ForwardPositionInit[1];
//		}
//		
//		else if(data_receive[3] == 1)//如果上层抬升标志位为1
//		{
//			Pid_Forward_Motor_Position[0].SetPoint = ForwardPositionInit[0] + ForwardJourney;
//			Pid_Forward_Motor_Position[1].SetPoint = ForwardPositionInit[1] - ForwardJourney;
//		}
		
//	}
	
	
//	
//	if(data_receive[0] == '!' && data_receive[1] == 1 && data_receive[2] == 2)   //如果用遥控器控制并且运动模式为抬升模式
//	{
//		if(data_receive[4] == 0) //如果上层抬升标志位为0
//			forward_flag = 0;
//		else if(data_receive[4] == 1)//如果上层抬升标志位为1
//			forward_flag = 1;

//		
//		
//		if(forward_flag_old != forward_flag)
//		{
//			
//			if(forward_flag == 1) //如果上层前移标志位为1
//			{
//				Pid_Forward_Motor_speed.SetPoint = vf;
//				if(end == 1)
//				{
//					Pid_Forward_Motor_speed.SetPoint = 0;
//					forward_flag_old = forward_flag;
//				}
//			}
//			
//			else if(forward_flag == 0) //如果上层前移标志位为0
//			{
//				Pid_Forward_Motor_speed.SetPoint = - vf;
//				if(start == 1)
//				{
//					Pid_Forward_Motor_speed.SetPoint = 0;
//					forward_flag_old = forward_flag;
//				}
//			}
//			
//		}
//		else			Pid_Forward_Motor_speed.SetPoint = 0;
//		
//////		if(data_receive[4] == 0) //如果上层前移标志位为0
////		if(forward_flag == 0) //如果上层前移标志位为0
////		{
//////			Pid_Forward_Motor_Position.SetPoint = ForwardPositionInit;
////		}
////		
//////		else if(data_receive[4] == 1)//如果上层前移标志位为1
////		else if(forward_flag == 1) //如果上层前移标志位为0
////		{
//////			Pid_Forward_Motor_Position.SetPoint = ForwardPositionInit - ForwardJourney;
////		}
//	}
//	else			Pid_Forward_Motor_speed.SetPoint = 0;
	
}


void Forward_task(void *pvParameters)
{
	
	portTickType xLastWakeTime;
	const portTickType xFrequency=1;
	
   while (1) {
			motor_forward_task();
				 
				xLastWakeTime=xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime,xFrequency);
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        Forward_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}



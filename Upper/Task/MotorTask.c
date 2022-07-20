#include "MotorTask.h"

uint32_t Motor_high_water;

Pid_Typedef Pid_LiftTrack_Speed[2],Pid_LiftTrack_Positioin[2],Pid_Rotate_Motor_Speed,Pid_Rotate_Motor_Positioin;//pid结构体

int rotate_set;			//旋转电机位置设定
int lift_pos_set[2];		//四连杆设定值

int LiftTrack_Current[2], Rotate_Motor_Current;//电流
int LiftPositionInit[2],LiftJourney_1,LiftJourney_2,LiftJourney_3,lift_once_offset, RotatePositionInit, RotateJourney;//初始位置,结束位置，行程距离
int Lift_Init_Change_Journey = -75000;

extern rmc620_t Track_Motor[2], Rotate_Motor;//电机的反馈数据
extern volatile int Pos_LiftTrack[2], Pos_Rotate_Motor;


void Pid_Motor_Init(void)
{
		LiftJourney_2 = -70000;
		LiftJourney_1 = -60000;
		LiftJourney_3 = -70000;
	
		RotateJourney = 30000;
	
//		lift_once_offset = 10000;
		lift_once_offset = 0;

	
		//右侧电机参数(抬)
		Pid_LiftTrack_Speed[0].P = 13.0f;
		Pid_LiftTrack_Speed[0].I = 0.01f;
		Pid_LiftTrack_Speed[0].D = 0.0f;
		Pid_LiftTrack_Speed[0].IMax = 5000.0f;
		Pid_LiftTrack_Speed[0].SetPoint = 0.0f;
		Pid_LiftTrack_Positioin[0].P = 0.5f;
		Pid_LiftTrack_Positioin[0].I = 0.02f;
		Pid_LiftTrack_Positioin[0].D = 0.00f;
		Pid_LiftTrack_Positioin[0].IMax = 500.0f;

		//左侧电机参数（抬）
		Pid_LiftTrack_Speed[1].P = 13.0f;
		Pid_LiftTrack_Speed[1].I = 0.01f;
		Pid_LiftTrack_Speed[1].D = 0.0f;
		Pid_LiftTrack_Speed[1].IMax = 5000.0f;
		Pid_LiftTrack_Speed[1].SetPoint = 0.0f;
		Pid_LiftTrack_Positioin[1].P = 0.5f;
		Pid_LiftTrack_Positioin[1].I = 0.02f;
		Pid_LiftTrack_Positioin[1].D = 0.00f;
		Pid_LiftTrack_Positioin[1].IMax = 500.0f;
		
		
				//旋转电机参数
		Pid_Rotate_Motor_Speed.P = 13.0f;
		Pid_Rotate_Motor_Speed.I = 0.0f;
		Pid_Rotate_Motor_Speed.D = 0.0f;
		Pid_Rotate_Motor_Speed.IMax = 5000.0f;
		Pid_Rotate_Motor_Speed.SetPoint = 0.0f;
		Pid_Rotate_Motor_Positioin.P = 0.5f;
		Pid_Rotate_Motor_Positioin.I = 0.02f;
		Pid_Rotate_Motor_Positioin.D = 0.00f;
		Pid_Rotate_Motor_Positioin.IMax = 500.0f;

		
		ZeroCheck_cal();	
		LiftPositionInit[0] = Pos_LiftTrack[0];
		LiftPositionInit[1] = Pos_LiftTrack[1];
		rotate_set = Pos_Rotate_Motor;
		//让电机初始位置保持不动
		Pid_LiftTrack_Positioin[0].SetPoint = LiftPositionInit[0];
		Pid_LiftTrack_Positioin[1].SetPoint = LiftPositionInit[1];
		Pid_Rotate_Motor_Positioin.SetPoint = rotate_set;
}


void motor_cal_task(void)
{
	/********************************************电机过零检测***************************************************/
	ZeroCheck_cal();
	
	
	
	
	/******************************************四连杆的两个电机控制*********************************************/
	/*******修正四连杆状态*********/
	if(g_Flag.lift_once_init_flag == 1)
	{
		LiftPositionInit[0] = LiftPositionInit[0] + Lift_Init_Change_Journey;
		LiftPositionInit[1] = LiftPositionInit[1] - Lift_Init_Change_Journey;
		g_Flag.lift_once_init_flag = 0;
	}else if(g_Flag.lift_once_init_flag == 2)
	{
		LiftPositionInit[0] = LiftPositionInit[0] - Lift_Init_Change_Journey;
		LiftPositionInit[1] = LiftPositionInit[1] + Lift_Init_Change_Journey;
		g_Flag.lift_once_init_flag = 0;
	}
	/**********修改位置环*********/
	if(g_Flag.lift_once_flag == 0) //如果上层抬升标志位为0
	{
		lift_pos_set[0] = LiftPositionInit[0] - lift_once_offset;									//修改目标值
		lift_pos_set[1] = LiftPositionInit[1] + lift_once_offset;
		
						Pid_LiftTrack_Positioin[0].P = 0.05f;
						Pid_LiftTrack_Positioin[0].I = 0.001f;
						Pid_LiftTrack_Positioin[0].D = 0.00f;
						Pid_LiftTrack_Positioin[0].IMax = 10000.0f;
		
	}else if(g_Flag.lift_once_flag == 1)//如果上层抬升标志位为1
	{
		lift_pos_set[0] = LiftPositionInit[0] - lift_once_offset - LiftJourney_1;									//修改目标值
		lift_pos_set[1] = LiftPositionInit[1] + lift_once_offset + LiftJourney_1;
		
						Pid_LiftTrack_Positioin[0].P = 1.5f;
						Pid_LiftTrack_Positioin[0].I = 0.002f;
						Pid_LiftTrack_Positioin[0].D = 0.00f;
						Pid_LiftTrack_Positioin[0].IMax = 10000.0f;
	}else if(g_Flag.lift_once_flag == 2)//如果上层抬升标志位为2
	{
		lift_pos_set[0] = LiftPositionInit[0] - lift_once_offset - LiftJourney_2;									//修改目标值
		lift_pos_set[1] = LiftPositionInit[1] + lift_once_offset + LiftJourney_2;
		
						Pid_LiftTrack_Positioin[0].P = 0.1f;
						Pid_LiftTrack_Positioin[0].I = 0.001f;
						Pid_LiftTrack_Positioin[0].D = 0.000f;
						Pid_LiftTrack_Positioin[0].IMax = 10000.0f;
	}else if(g_Flag.lift_once_flag == 3)//如果上层抬升标志位为3
	{
		lift_pos_set[0] = LiftPositionInit[0] - lift_once_offset - LiftJourney_3;									//修改目标值
		lift_pos_set[1] = LiftPositionInit[1] + lift_once_offset + LiftJourney_3;
		
						Pid_LiftTrack_Positioin[0].P = 1.5f;
						Pid_LiftTrack_Positioin[0].I = 0.002f;
						Pid_LiftTrack_Positioin[0].D = 0.00f;
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
		
	/************PID双环运算**************/
	//位置环（左抬升）
	Pid_LiftTrack_Speed[0].SetPoint = LIMIT_MAX_MIN(PID_Calc(&Pid_LiftTrack_Positioin[0],Pos_LiftTrack[0]), 100000, -100000);
	//位置环（右抬升）
	Pid_LiftTrack_Speed[1].SetPoint = LIMIT_MAX_MIN(PID_Calc(&Pid_LiftTrack_Positioin[1],Pos_LiftTrack[1]), 10000, -10000);
	//速度环（左抬升）
	LiftTrack_Current[0] = LIMIT_MAX_MIN(PID_Calc(&Pid_LiftTrack_Speed[0],(float)Track_Motor[0].speed),15000,-15000);
	//速度环（右抬升）
	LiftTrack_Current[1] = LIMIT_MAX_MIN(PID_Calc(&Pid_LiftTrack_Speed[1],(float)Track_Motor[1].speed),13000,-13000);
	
	
	
	
	
	
	/***************************************************爪子旋转电机**************************************************/
	//位置环
	if(g_Flag.rotate_s - g_Flag.rotate_w != 0)
	{
		Pid_Rotate_Motor_Positioin.SetPoint = Pos_Rotate_Motor;
		Pid_Rotate_Motor_Speed.SetPoint = (g_Flag.rotate_s - g_Flag.rotate_w)  * 1000.0;
	}else
	{
		Pid_Rotate_Motor_Speed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&Pid_Rotate_Motor_Positioin,Pos_Rotate_Motor), 5000, -5000);
	}
	//速度环
	Rotate_Motor_Current = LIMIT_MAX_MIN(PID_Calc(&Pid_Rotate_Motor_Speed,(float)Rotate_Motor.speed),13000,-13000);
	
	
	
	
	
	/***************************************************电机电流输出**************************************************/

	if(g_Flag.control_target == POWER_OFF_MODE)
		motorCurrentSend(0,0,0,0);
	else
	{
		motorCurrentSend(LiftTrack_Current[0],0,-LiftTrack_Current[0],0);
		motorCurrentSend2(Rotate_Motor_Current,0,0,0);
	}
}

void Motor_task(void *pvParameters)
{
	
//	portTickType xLastWakeTime;
//	const portTickType xFrequency=1;
		Pid_Motor_Init();
  while (1) {
		
		motor_cal_task();
		
		vTaskDelay(1);
//    xLastWakeTime=xTaskGetTickCount();
//	vTaskDelayUntil(&xLastWakeTime,xFrequency);
		 
//#if INCLUDE_uxTaskGetStackHighWaterMark
//        Motor_high_water = uxTaskGetStackHighWaterMark(NULL);
//#endif
    }
}





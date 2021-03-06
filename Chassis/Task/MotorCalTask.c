#include "MotorCalTask.h"

uint32_t motor_cal_high_water;

pid_Typedef Pid_LiftTrack_Speed[2],Pid_LiftTrack_Positioin[2], pid_warehouse_speed, pid_warehouse_position;//pid结构体
int LiftTrack_Current[2], warehouse_current;//电流
int LiftPositionInit[2],LiftJourney_1,LiftJourney_2,LiftJourney_3,lift_offset,  warehouse_journey;//初始位置,结束位置，行程距离
int warehouse_set;		//仓库旋转电机设定值

extern rmc620_t lift_track_motor[2], warehouse_motor;//电机的反馈数据
extern int pos_lift_track[2], pos_warehouse;


void motorPidInit(void)
{
		LiftJourney_1 = 165000;
		LiftJourney_2	= 225000;
		LiftJourney_3 = 225000;
	
		lift_offset = 15000;
		warehouse_journey = 85000;
	
		//左侧电机参数(抬)
		Pid_LiftTrack_Positioin[0].P = 0.8f;
		Pid_LiftTrack_Positioin[0].I = 0.05f;
		Pid_LiftTrack_Positioin[0].D = 0.01f;
		Pid_LiftTrack_Positioin[0].IMax = 500.0f;

		Pid_LiftTrack_Speed[0].P = 20.0f;
		Pid_LiftTrack_Speed[0].I = 0.5f;
		Pid_LiftTrack_Speed[0].D = 0.0f;
		Pid_LiftTrack_Speed[0].IMax = 500.0f;
		Pid_LiftTrack_Speed[0].SetPoint = 0.0f;

		//右侧电机参数（抬）
		Pid_LiftTrack_Positioin[1].P = 0.8f;
		Pid_LiftTrack_Positioin[1].I = 0.05f;
		Pid_LiftTrack_Positioin[1].D = 0.01f;
		Pid_LiftTrack_Positioin[1].IMax = 500.0f;
		
		Pid_LiftTrack_Speed[1].P = 20.0f;
		Pid_LiftTrack_Speed[1].I = 0.5f;
		Pid_LiftTrack_Speed[1].D = 0.0f;
		Pid_LiftTrack_Speed[1].IMax = 500.0f;
		Pid_LiftTrack_Speed[1].SetPoint = 0.0f;

	/******************************仓库电机***************************************/
		pid_warehouse_position.P = 0.8f;
		pid_warehouse_position.I = 0.001f;
		pid_warehouse_position.D = 0.0f;
		pid_warehouse_position.IMax = 500.0f;
		
		pid_warehouse_speed.P = 5.0f;
		pid_warehouse_speed.I = 0.1f;
		pid_warehouse_speed.D = 0.0f;
		pid_warehouse_speed.IMax = 500.0f;
		pid_warehouse_speed.SetPoint = 0.0f;
		
		
		
		ZeroCheck_cal();	
		
		LiftPositionInit[0] = pos_lift_track[0];
		LiftPositionInit[1] = pos_lift_track[1];
		warehouse_set = pos_warehouse;
		
		//让电机初始位置保持不动
		Pid_LiftTrack_Positioin[0].SetPoint = LiftPositionInit[0];
		Pid_LiftTrack_Positioin[1].SetPoint = LiftPositionInit[1];
		
		pid_warehouse_position.SetPoint = warehouse_set;

}

void motorCal(void)
{
	/********************************************电机过零检测***************************************************/
	ZeroCheck_cal();
	
	
	
	
	/********************************************二级抬升的两个电机控制***************************************************/
	/**********修改位置环*********/
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
	}else if (g_Flag.lift_down_twice_flag == 3)
	{
		Pid_LiftTrack_Positioin[0].SetPoint = LiftPositionInit[0] - lift_offset - LiftJourney_3;
		Pid_LiftTrack_Positioin[1].SetPoint = LiftPositionInit[1] + lift_offset + LiftJourney_3;
	}

	/************PID双环运算**************/
	//位置环（左抬升）
	Pid_LiftTrack_Speed[0].SetPoint = PID_Calc(&Pid_LiftTrack_Positioin[0],pos_lift_track[0]);
	Pid_LiftTrack_Speed[0].SetPoint = LIMIT_MAX_MIN(Pid_LiftTrack_Speed[0].SetPoint, 5000, -5000);
	//位置环（右抬升）
	Pid_LiftTrack_Speed[1].SetPoint = PID_Calc(&Pid_LiftTrack_Positioin[1],pos_lift_track[1]);
	Pid_LiftTrack_Speed[1].SetPoint = LIMIT_MAX_MIN(Pid_LiftTrack_Speed[1].SetPoint, 5000, -5000);
	//速度环（左抬升）
	LiftTrack_Current[0] = LIMIT_MAX_MIN(PID_Calc(&Pid_LiftTrack_Speed[0],lift_track_motor[0].speed),12000,-12000);
	//速度环（右抬升）
	LiftTrack_Current[1] = LIMIT_MAX_MIN(PID_Calc(&Pid_LiftTrack_Speed[1],lift_track_motor[1].speed),12000,-12000);
	
	
	
	
	
	/***************************************************仓库旋转电机控制**************************************************/
	//位置环
	if(rc_ctrl.key.a - rc_ctrl.key.d != 0 && rc_ctrl.key.ctrl == 1 && rc_ctrl.key.shift == 0)
	{
		pid_warehouse_position.SetPoint = pos_warehouse;
		pid_warehouse_speed.SetPoint = (rc_ctrl.key.a - rc_ctrl.key.d)  * 600.0;
	}else
	{
		pid_warehouse_speed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&pid_warehouse_position,pos_warehouse), 5000, -5000);
	}
	//速度环
	warehouse_current = LIMIT_MAX_MIN(PID_Calc(&pid_warehouse_speed,(float)warehouse_motor.speed),13000,-13000);
	
	
	
	/***************************************************电机电流输出**************************************************/
	if(g_Flag.control_target == POWER_OFF_MODE)
	{
			motorCurrentSend(0,0,0,0);		
			warehouseCurrentSend(0,0,0,0);
	}else	{
			motorCurrentSend(LiftTrack_Current[0],-LiftTrack_Current[0],0,0);		
			warehouseCurrentSend(warehouse_current,0,0,0);
	}	
}

void motorCalTask(void *pvParameters)
{
	
//	portTickType xLastWakeTime;
//	const portTickType xFrequency=1;
	
  while (1) {
		
		motorCal();
		
		vTaskDelay(1);
		

//    xLastWakeTime=xTaskGetTickCount();
//	vTaskDelayUntil(&xLastWakeTime,xFrequency);
		 
//#if INCLUDE_uxTaskGetStackHighWaterMark
//        motor_cal_high_water = uxTaskGetStackHighWaterMark(NULL);
//#endif
    }
}





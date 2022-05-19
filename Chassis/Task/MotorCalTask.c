#include "MotorCalTask.h"

uint32_t motor_cal_high_water;

pid_Typedef Pid_LiftTrack_Speed[2],Pid_LiftTrack_Positioin[2], pid_warehouse_speed, pid_warehouse_position;//pid�ṹ��
int LiftTrack_Current[2], warehouse_current;//����
int LiftPositionInit[2],LiftJourney_1,LiftJourney_2,lift_offset, warehouse_position_init, warehouse_journey;//��ʼλ��,����λ�ã��г̾���


extern rmc620_t lift_track_motor[2], warehouse_motor;//����ķ�������
extern int pos_lift_track[2], pos_warehouse;


void motorPidInit(void)
{
		ZeroCheck_cal();	
		LiftPositionInit[0] = pos_lift_track[0];
		LiftPositionInit[1] = pos_lift_track[1];
		warehouse_position_init = pos_warehouse;
//		LiftJourney_1 = 80000;
//		LiftJourney_1 = 180000;
		LiftJourney_1 = 195000;
		LiftJourney_2	= 160000;
	
		lift_offset = 30000;
//		ForwardJourney = 5000;		//ǰ�ƾ������
//		ForwardJourney = 0;
//		lift_once_flag = 0;			//һ��̧����־λ����
		warehouse_journey = 85000;
	
		//���������(̧)
		Pid_LiftTrack_Positioin[0].P = 0.8f;
		Pid_LiftTrack_Positioin[0].I = 0.05f;
		Pid_LiftTrack_Positioin[0].D = 0.01f;
		Pid_LiftTrack_Positioin[0].IMax = 500.0f;

		Pid_LiftTrack_Speed[0].P = 20.0f;
		Pid_LiftTrack_Speed[0].I = 0.5f;
		Pid_LiftTrack_Speed[0].D = 0.0f;
		Pid_LiftTrack_Speed[0].IMax = 500.0f;
		Pid_LiftTrack_Speed[0].SetPoint = 0.0f;

		//�Ҳ���������̧��
		Pid_LiftTrack_Positioin[1].P = 0.8f;
		Pid_LiftTrack_Positioin[1].I = 0.05f;
		Pid_LiftTrack_Positioin[1].D = 0.01f;
		Pid_LiftTrack_Positioin[1].IMax = 500.0f;
		
		Pid_LiftTrack_Speed[1].P = 20.0f;
		Pid_LiftTrack_Speed[1].I = 0.5f;
		Pid_LiftTrack_Speed[1].D = 0.0f;
		Pid_LiftTrack_Speed[1].IMax = 500.0f;
		Pid_LiftTrack_Speed[1].SetPoint = 0.0f;

	/******************************�ֿ���***************************************/
		
		pid_warehouse_position.P = 0.02f;
		pid_warehouse_position.I = 0.001f;
		pid_warehouse_position.D = 0.0f;
		pid_warehouse_position.IMax = 500.0f;

		pid_warehouse_speed.P = 5.0f;
		pid_warehouse_speed.I = 0.1f;
		pid_warehouse_speed.D = 0.0f;
		pid_warehouse_speed.IMax = 500.0f;
		pid_warehouse_speed.SetPoint = 0.0f;
//		//�Ҳ���������̧��
//		Pid_Forward_Motor_speed[1].P = 20.0f;
//		Pid_Forward_Motor_speed[1].I = 0.0f;
//		Pid_Forward_Motor_speed[1].D = 0.0f;
//		Pid_Forward_Motor_speed[1].IMax = 500.0f;
//		Pid_Forward_Motor_speed[1].SetPoint = 0.0f;
//		Pid_Forward_Motor_Position[1].P = 0.8f;
//		Pid_Forward_Motor_Position[1].I = 0.05f;
//		Pid_Forward_Motor_Position[1].D = 0.01f;
//		Pid_Forward_Motor_Position[1].IMax = 500.0f;

	
	
	
////		Pid_Forward_Motor_speed.P = 20.0f;  //Ŀǰû��p��ֵ
//		Pid_Forward_Motor_speed.P = 6.5f;  //Ŀǰû��p��ֵ
//		Pid_Forward_Motor_speed.I = 0.0f;
//		Pid_Forward_Motor_speed.D = 0.0f;
//		Pid_Forward_Motor_speed.IMax = 500.0f;
//		Pid_Forward_Motor_speed.SetPoint = 0.0f;
////		Pid_Forward_Motor_Position.P = 6.5f;
//		Pid_Forward_Motor_Position.P = 0.0f;
//		Pid_Forward_Motor_Position.I = 0.0f;
//		Pid_Forward_Motor_Position.D = 0.0f;
//		Pid_Forward_Motor_Position.IMax = 500.0f;
		
		

	//�õ����ʼλ�ñ��ֲ���
	Pid_LiftTrack_Positioin[0].SetPoint = LiftPositionInit[0];
	Pid_LiftTrack_Positioin[1].SetPoint = LiftPositionInit[1];
	
	pid_warehouse_position.SetPoint = warehouse_position_init;
//	Pid_Forward_Motor_Position[0].SetPoint = ForwardPositionInit[0];
//	Pid_Forward_Motor_Position[1].SetPoint = ForwardPositionInit[1];

}

//int limit = 5000;		//����

void motorCal(void)
{
	
	
	//���������
	ZeroCheck_cal();
	
	/*����̧���������������*/
	//λ�û�����̧����
	Pid_LiftTrack_Speed[0].SetPoint = PID_Calc(&Pid_LiftTrack_Positioin[0],pos_lift_track[0]);
	Pid_LiftTrack_Speed[0].SetPoint = LIMIT_MAX_MIN(Pid_LiftTrack_Speed[0].SetPoint, 5000, -5000);
	//λ�û�����̧����
	Pid_LiftTrack_Speed[1].SetPoint = PID_Calc(&Pid_LiftTrack_Positioin[1],pos_lift_track[1]);
	Pid_LiftTrack_Speed[1].SetPoint = LIMIT_MAX_MIN(Pid_LiftTrack_Speed[1].SetPoint, 5000, -5000);
	//�ٶȻ�����̧����
	LiftTrack_Current[0] = LIMIT_MAX_MIN(PID_Calc(&Pid_LiftTrack_Speed[0],lift_track_motor[0].speed),12000,-12000);
	//�ٶȻ�����̧����
	LiftTrack_Current[1] = LIMIT_MAX_MIN(PID_Calc(&Pid_LiftTrack_Speed[1],lift_track_motor[1].speed),12000,-12000);
	
	
	
	/*�ֿ���ת�������*/
	//λ�û�
	pid_warehouse_speed.SetPoint = PID_Calc(&pid_warehouse_position,pos_warehouse);
	pid_warehouse_speed.SetPoint = LIMIT_MAX_MIN(pid_warehouse_speed.SetPoint, 5000, -5000);
	//�ٶȻ�
	warehouse_current = LIMIT_MAX_MIN(PID_Calc(&pid_warehouse_speed,(float)warehouse_motor.speed),13000,-13000);
	
	
	
	
//	
//	/*ǰ�Ƶ������������*/
//	//λ�û�����̧����
//	Pid_Forward_Motor_speed[0].SetPoint = PID_Calc(&Pid_Forward_Motor_Position[0],pos_warehouse[0]);
//	Pid_Forward_Motor_speed[0].SetPoint = LIMIT_MAX_MIN(Pid_Forward_Motor_speed[0].SetPoint, 5000, -5000);
//	//λ�û�����̧����
//	Pid_Forward_Motor_speed[1].SetPoint = PID_Calc(&Pid_Forward_Motor_Position[1],pos_warehouse[1]);
//	Pid_Forward_Motor_speed[1].SetPoint = LIMIT_MAX_MIN(Pid_Forward_Motor_speed[1].SetPoint, 5000, -5000);
//	//�ٶȻ�����̧����
//	Forward_Motor_Current[0] = LIMIT_MAX_MIN(PID_Calc(&Pid_Forward_Motor_speed[0],(float)forward_motor[0].speed),13000,-13000);
//	//�ٶȻ�����̧����
//	Forward_Motor_Current[1] = LIMIT_MAX_MIN(PID_Calc(&Pid_Forward_Motor_speed[1],(float)forward_motor[1].speed),13000,-13000);
//	
	
	
	

	
	//����������
//	motorCurrentSend(LiftTrack_Current[0],LiftTrack_Current[1],warehouse_current,0);														
	if(g_Flag.control_target == POWER_OFF_MODE)
	{
		motorCurrentSend(0,0,0,0);		
	}else	{
		motorCurrentSend(LiftTrack_Current[0],-LiftTrack_Current[0],warehouse_current,0);			
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
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        motor_cal_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}





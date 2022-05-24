#include "MotorTask.h"

uint32_t Motor_high_water;

//Pid_Typedef Pid_LiftTrack_Speed[2],Pid_LiftTrack_Positioin[2],Pid_Forward_Motor_speed[2],Pid_Forward_Motor_Position[2];//pid�ṹ��
//int LiftTrack_Current[2],Forward_Motor_Current[2];//����
//int LiftPositionInit[2],ForwardPositionInit[2],LiftJourney,ForwardJourney;//��ʼλ��,����λ�ã��г̾���
Pid_Typedef Pid_LiftTrack_Speed[2],Pid_LiftTrack_Positioin[2],Pid_Rotate_Motor_Speed,Pid_Rotate_Motor_Positioin;//pid�ṹ��
//float Lift_pos_set_pre[2];							//��¼̧�����λ�û��ϴε��趨ֵ

int LiftTrack_Current[2], Rotate_Motor_Current;//����
int LiftPositionInit[2],LiftJourney_1,LiftJourney_2,lift_once_offset, RotatePositionInit, RotateJourney;//��ʼλ��,����λ�ã��г̾���

//extern rmc620_t Track_Motor[2], forward_motor[2];//����ķ�������
//extern int Pos_LiftTrack[2], Pos_Forward_Motor[2];
extern rmc620_t Track_Motor[2], Rotate_Motor;//����ķ�������
extern volatile int Pos_LiftTrack[2], Pos_Rotate_Motor;

extern int rotate_set;		//��ת���λ���趨

//extern int  forward_flag;		//ǰ�Ʊ�־λ
//extern int  forward_flag_old;		//ǰ�Ʊ�־λ
void Pid_Motor_Init(void)
{

//		ForwardPositionInit[0] = Pos_Forward_Motor[0];
//		ForwardPositionInit[1] = Pos_Forward_Motor[1];
		LiftJourney_2 = -60000;
		LiftJourney_1 = -40000;
		RotateJourney = 0;
	
	
	
//		lift_once_offset = 10000;
		lift_once_offset = 0;

//		ForwardJourney = 5000;		//ǰ�ƾ������
//		ForwardJourney = 0;
//		forward_flag = 0;			//ǰ�Ʊ�־λ����
//		forward_flag_old = forward_flag;
//		vf	= -3500;		//ǰ�Ƶ���ٶ�(������)
	
	
		//�Ҳ�������(̧)
//		Pid_LiftTrack_Speed[0].P = 23.0f;
//		Pid_LiftTrack_Speed[0].P = 12.0f;
		Pid_LiftTrack_Speed[0].P = 13.0f;
		Pid_LiftTrack_Speed[0].I = 0.01f;
		Pid_LiftTrack_Speed[0].D = 0.0f;
		Pid_LiftTrack_Speed[0].IMax = 5000.0f;
		Pid_LiftTrack_Speed[0].SetPoint = 0.0f;
		Pid_LiftTrack_Positioin[0].P = 0.5f;
		Pid_LiftTrack_Positioin[0].I = 0.02f;
		Pid_LiftTrack_Positioin[0].D = 0.00f;
		Pid_LiftTrack_Positioin[0].IMax = 500.0f;

		//�����������̧��
//		Pid_LiftTrack_Speed[1].P = 23.0f;
//		Pid_LiftTrack_Speed[1].P = 12.0f;
		Pid_LiftTrack_Speed[1].P = 13.0f;
		Pid_LiftTrack_Speed[1].I = 0.01f;
		Pid_LiftTrack_Speed[1].D = 0.0f;
		Pid_LiftTrack_Speed[1].IMax = 5000.0f;
		Pid_LiftTrack_Speed[1].SetPoint = 0.0f;
		Pid_LiftTrack_Positioin[1].P = 0.5f;
		Pid_LiftTrack_Positioin[1].I = 0.02f;
		Pid_LiftTrack_Positioin[1].D = 0.00f;
		Pid_LiftTrack_Positioin[1].IMax = 500.0f;
		
		
				//��ת�������
//		Pid_Rotate_Motor_Speed.P = 23.0f;
//		Pid_Rotate_Motor_Speed.P = 12.0f;
		Pid_Rotate_Motor_Speed.P = 13.0f;
		Pid_Rotate_Motor_Speed.I = 0.01f;
		Pid_Rotate_Motor_Speed.D = 0.0f;
		Pid_Rotate_Motor_Speed.IMax = 5000.0f;
		Pid_Rotate_Motor_Speed.SetPoint = 0.0f;
		Pid_Rotate_Motor_Positioin.P = 0.5f;
		Pid_Rotate_Motor_Positioin.I = 0.02f;
		Pid_Rotate_Motor_Positioin.D = 0.00f;
		Pid_Rotate_Motor_Positioin.IMax = 500.0f;

	
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
		
	ZeroCheck_cal();	
	LiftPositionInit[0] = Pos_LiftTrack[0];
	LiftPositionInit[1] = Pos_LiftTrack[1];
	rotate_set = Pos_Rotate_Motor;
	//�õ����ʼλ�ñ��ֲ���
	Pid_LiftTrack_Positioin[0].SetPoint = LiftPositionInit[0];
	Pid_LiftTrack_Positioin[1].SetPoint = LiftPositionInit[1];
	Pid_Rotate_Motor_Positioin.SetPoint = rotate_set;
	
//	Pid_Forward_Motor_Position[0].SetPoint = ForwardPositionInit[0];
//	Pid_Forward_Motor_Position[1].SetPoint = ForwardPositionInit[1];

}

//int limit_max = 5000;
//int limit_min = 1000;
//float test_P[4] = {0.2,0.4,0.6,0.8};
//int limit = 1100;		//����
//int limit = 5000;		//����

//int test11 = 1;
void motor_cal_task(void)
{
	
	
	//���������
	ZeroCheck_cal();
	

//	float lift_displace = ABS(Pid_LiftTrack_Positioin[0].SetPoint-Lift_pos_set_pre[0]);
//	if( ABS(Pid_LiftTrack_Positioin[0].SetPoint-Pos_LiftTrack[0]) < test_P[0] * lift_displace ||
//		  ABS(Pid_LiftTrack_Positioin[0].SetPoint-Pos_LiftTrack[0]) > test_P[3] * lift_displace )
//	{
//		limit = limit_min;
//	}else if( ABS(Pid_LiftTrack_Positioin[0].SetPoint-Pos_LiftTrack[0]) < test_P[1] * lift_displace )
//	{
//		limit = limit_min + (limit_max - limit_min) * ((ABS(Pos_LiftTrack[0]-Lift_pos_set_pre[0]) - test_P[0]*lift_displace) / ((test_P[1]-test_P[0])*lift_displace));
//	}else if(	ABS(Pid_LiftTrack_Positioin[0].SetPoint-Pos_LiftTrack[0]) > test_P[2] * lift_displace )
//	{
//		limit = limit_min + (limit_max - limit_min) * (1 - ((ABS(Pos_LiftTrack[0]-Lift_pos_set_pre[0]) - test_P[2]*lift_displace) / ((test_P[3]-test_P[2])*lift_displace)));
//	}else 
//	{
//		limit = limit_max;
//	}
	
	/*̧���������������*/
	//λ�û�����̧����
	Pid_LiftTrack_Speed[0].SetPoint = LIMIT_MAX_MIN(PID_Calc(&Pid_LiftTrack_Positioin[0],Pos_LiftTrack[0]), 5000, -5000);
//	Pid_LiftTrack_Speed[0].SetPoint = 0;
	//λ�û�����̧����
	Pid_LiftTrack_Speed[1].SetPoint = LIMIT_MAX_MIN(PID_Calc(&Pid_LiftTrack_Positioin[1],Pos_LiftTrack[1]), 5000, -5000);
//	Pid_LiftTrack_Speed[1].SetPoint = 0;
	//�ٶȻ�����̧����
	LiftTrack_Current[0] = LIMIT_MAX_MIN(PID_Calc(&Pid_LiftTrack_Speed[0],(float)Track_Motor[0].speed),13000,-13000);
	//�ٶȻ�����̧����
	LiftTrack_Current[1] = LIMIT_MAX_MIN(PID_Calc(&Pid_LiftTrack_Speed[1],(float)Track_Motor[1].speed),13000,-13000);
	

	
	/*******צ����ת���********/
	//λ�û�
//	Pid_LiftTrack_Speed[1].SetPoint = PID_Calc(&Pid_LiftTrack_Positioin[1],Pos_LiftTrack[1]);
	Pid_Rotate_Motor_Speed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&Pid_Rotate_Motor_Positioin,Pos_Rotate_Motor), 5000, -5000);
	//�ٶȻ�
	Rotate_Motor_Current = LIMIT_MAX_MIN(PID_Calc(&Pid_Rotate_Motor_Speed,(float)Rotate_Motor.speed),13000,-13000);
	

//	//����������
//	if(g_Flag.control_target == SENIOR_UP_MODE || g_Flag.control_target == SENIOR_MODE2 || g_Flag.control_target == SENIOR_AUTO_MODE)
//	{
//		motorCurrentSend(LiftTrack_Current[0],-LiftTrack_Current[0],0,0);
//	}else{
//		motorCurrentSend(0,0,0,0);
//	}
	if(g_Flag.control_target == POWER_OFF_MODE)
		motorCurrentSend(0,0,0,0);
	else
//		motorCurrentSend(0,LiftTrack_Current[0],-LiftTrack_Current[0],Rotate_Motor_Current);
		motorCurrentSend(0,LiftTrack_Current[0],-LiftTrack_Current[0],0);

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
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        Motor_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}





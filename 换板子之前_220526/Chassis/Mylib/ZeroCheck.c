#include "ZeroCheck.h"
/*--------------------------�ڲ�����----------------------------*/
//ZeroCheck_Typedef zerocheck_lift_track[2], zerocheck_warehouse_motor;         
ZeroCheck_Typedef zerocheck_lift_track[2], zerocheck_warehouse_motor;         

//int pos_lift_track[2], pos_warehouse;
int pos_lift_track[2], pos_warehouse;
/*--------------------------�ⲿ���ñ���----------------------------*/
extern  rmc620_t lift_track_motor[2], warehouse_motor;
/*--------------------------��������----------------------------*/

/**
  * @brief  λ��ʽ���ٶ�ʽ������
	           Zero->ActualValue ��ʾ�������ǰֵ
						 Zero->LastValue ��ʾ�������һ��ֵ
						 Zero->CountCycle ��ʾ���������ʱԽ��ֵ������������
						 Zero->PreError ��ʾ�������ֵ
						 ʹ�ô˺���ǰҪ������Ӧ������ṹ��� Zero->CountCycle��Zero->LastValue
  * @param  ZeroCheck_Typedef *Zero  ������ṹ��
  *         float value  �������
            short Zerocheck_mode��ȡֵPosition��Speed
  * @retval ȡ����Zerocheck_mode���ֱ�����������λ��ֵ���ٶ�ֵ
  */
float ZeroCheck(ZeroCheck_Typedef *Zero,float value,short Zerocheck_mode)
{
	Zero->ActualValue=value;
	
	Zero->PreError=Zero->ActualValue-Zero->LastValue;
	Zero->LastValue=Zero->ActualValue;
	
	if(Zero->PreError>0.7f*Zero->CountCycle)
	{
		Zero->PreError=Zero->PreError-Zero->CountCycle;
		Zero->Circle--;
	}
	if(Zero->PreError<-0.7f*Zero->CountCycle)
	{
		Zero->PreError=Zero->PreError+Zero->CountCycle;
		Zero->Circle++;
	}
	
	if(Zerocheck_mode==Position)
		return Zero->ActualValue + Zero->Circle*Zero->CountCycle;
	else if(Zerocheck_mode==Speed)
	  return Zero->PreError;
	else 
		return 0;
}


/**
  * @brief  ������ִ�к���
  * @param  None
  * @retval None
  */
volatile int pos_before0,pos_before1 = 0 , pos_before2 = 0, pos_before3 = 0;


void ZeroCheck_cal(void)
{
	
	
	
	/************************************** ����̧����� *************************************/
//	pos_before0 = ZeroCheck(&zerocheck_lift_track[0],lift_track_motor[0].angle,Position);
//	pos_before1 = ZeroCheck(&zerocheck_lift_track[1],lift_track_motor[1].angle,Position);
//	pos_lift_track[0] = (int)(pos_before0/19.0f);
//	pos_lift_track[1] = (int)(pos_before1/19.0f);
	pos_lift_track[0] = ZeroCheck(&zerocheck_lift_track[0],lift_track_motor[0].angle,Position);
	pos_lift_track[1] = ZeroCheck(&zerocheck_lift_track[1],lift_track_motor[1].angle,Position);
	/*************************************************************************************/
	
	
	/************************************** �ֿ��� *************************************/
//	pos_before2 = ZeroCheck(&zerocheck_warehouse_motor,warehouse_motor.angle,Position);
//	pos_warehouse = (int)(pos_before2/19.0f);
	pos_warehouse = ZeroCheck(&zerocheck_warehouse_motor,warehouse_motor.angle,Position);
	/*************************************************************************************/
	
	
}


/**
  * @brief  ������ṹ�������ʼ��
  * @param  None
  * @retval None
  */
void ZeroCheck_Init(void)
{
	zerocheck_lift_track[0].Circle = 0;
	zerocheck_lift_track[0].CountCycle = 8192.0f;
	zerocheck_lift_track[0].LastValue = lift_track_motor[0].angle;

	zerocheck_lift_track[1].Circle = 0;
	zerocheck_lift_track[1].CountCycle = 8192.0f;
	zerocheck_lift_track[1].LastValue = lift_track_motor[1].angle;

	
	
	zerocheck_warehouse_motor.Circle = 0;
	zerocheck_warehouse_motor.CountCycle = 8192.0f;
	zerocheck_warehouse_motor.LastValue = warehouse_motor.angle;
	
	
}


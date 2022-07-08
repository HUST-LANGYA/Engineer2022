#include "ZeroCheck.h"
/*--------------------------内部变量----------------------------*/
//ZeroCheck_Typedef ZeroCheck_LiftTrack[2], ZeroCheck_Forward_Motor[2];         
ZeroCheck_Typedef ZeroCheck_LiftTrack[2], ZeroCheck_Rotate_Motor;         

//int Pos_LiftTrack[2], Pos_Forward_Motor[2];
int volatile Pos_LiftTrack[2], Pos_Rotate_Motor;
/*--------------------------外部引用变量----------------------------*/
extern  rmc620_t Track_Motor[2], Rotate_Motor;
//extern  rmc620_t Track_Motor[2], forward_motor[2];
/*--------------------------函数部分----------------------------*/

/**
  * @brief  位置式和速度式过零检测
	           Zero->ActualValue 表示检测量当前值
						 Zero->LastValue 表示检测量上一次值
						 使用此函数前要申明对应检测量结构体的 Zero->CountCycle与Zero->LastValue
  * @param  ZeroCheck_Typedef *Zero  过零检测结构体
  *         float value  待检测量
            short Zerocheck_mode：取值Position或Speed
  * @retval 取决于Zerocheck_mode，分别输出过零检测后位置值或速度值
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

//float ZeroCheck_forward(ZeroCheck_Typedef *Zero,float value,short Zerocheck_mode)
//{
//	Zero->ActualValue=value;
//	
//	Zero->PreError=Zero->ActualValue-Zero->LastValue;
//	Zero->LastValue=Zero->ActualValue;
//	
//	if(Zero->PreError>0.6f*Zero->CountCycle)
//	{
//		Zero->PreError=Zero->PreError-Zero->CountCycle;
//		Zero->Circle--;
//	}
//	if(Zero->PreError<-0.6f*Zero->CountCycle)
//	{
//		Zero->PreError=Zero->PreError+Zero->CountCycle;
//		Zero->Circle++;
//	}
//	
//	if(Zerocheck_mode==Position)
//		return Zero->ActualValue + Zero->Circle*Zero->CountCycle;
//	else if(Zerocheck_mode==Speed)
//	  return Zero->PreError;
//	else 
//		return 0;
//}


/**
  * @brief  过零检测执行函数
  * @param  None
  * @retval None
  */
volatile int pos_before0,pos_before1 = 0 , pos_before2 = 0, pos_before3 = 0;



void ZeroCheck_cal(void)
{
	
	
	
	/************************************** 抬升电机 *************************************/
	Pos_LiftTrack[0] = ZeroCheck(&ZeroCheck_LiftTrack[0],Track_Motor[0].angle,Position);
	Pos_LiftTrack[1] = ZeroCheck(&ZeroCheck_LiftTrack[1],Track_Motor[1].angle,Position);
//	Pos_LiftTrack[0] = (int)(pos_before0/19.0f);
//	Pos_LiftTrack[1] = (int)(pos_before1/19.0f);
	/*************************************************************************************/
	
	
	
	/************************************** 旋转电机 *************************************/
	Pos_Rotate_Motor = ZeroCheck(&ZeroCheck_Rotate_Motor,Rotate_Motor.angle,Position);
//	Pos_Forward_Motor[0] = (int)(pos_before2/19.0f);
//	Pos_Forward_Motor[1] = (int)(pos_before3/19.0f);
	/*************************************************************************************/
	
	
}


/**
  * @brief  过零检测结构体参数初始化
  * @param  None
  * @retval None
  */
void ZeroCheck_Init(void)
{
	ZeroCheck_LiftTrack[0].Circle = 0;
	ZeroCheck_LiftTrack[0].CountCycle = 8192;
	ZeroCheck_LiftTrack[0].LastValue = Track_Motor[0].angle;

	ZeroCheck_LiftTrack[1].Circle = 0;
	ZeroCheck_LiftTrack[1].CountCycle = 8192;
	ZeroCheck_LiftTrack[1].LastValue = Track_Motor[1].angle;

	
	ZeroCheck_Rotate_Motor.Circle = 0;
	ZeroCheck_Rotate_Motor.CountCycle = 8192;
	ZeroCheck_Rotate_Motor.LastValue = Track_Motor[1].angle;

	
	
}


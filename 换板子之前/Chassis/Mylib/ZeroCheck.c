#include "ZeroCheck.h"
/*--------------------------内部变量----------------------------*/
//ZeroCheck_Typedef zerocheck_lift_track[2], zerocheck_warehouse_motor;         
ZeroCheck_Typedef zerocheck_lift_track[2], zerocheck_warehouse_motor;         

//int pos_lift_track[2], pos_warehouse;
int pos_lift_track[2], pos_warehouse;
/*--------------------------外部引用变量----------------------------*/
extern  rmc620_t lift_track_motor[2], warehouse_motor;
/*--------------------------函数部分----------------------------*/

/**
  * @brief  位置式和速度式过零检测
	           Zero->ActualValue 表示检测量当前值
						 Zero->LastValue 表示检测量上一次值
						 Zero->CountCycle 表示检测量过零时越变值，即计数周期
						 Zero->PreError 表示检测量差值
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


/**
  * @brief  过零检测执行函数
  * @param  None
  * @retval None
  */
volatile int pos_before0,pos_before1 = 0 , pos_before2 = 0, pos_before3 = 0;


void ZeroCheck_cal(void)
{
	
	
	
	/************************************** 二级抬升电机 *************************************/
//	pos_before0 = ZeroCheck(&zerocheck_lift_track[0],lift_track_motor[0].angle,Position);
//	pos_before1 = ZeroCheck(&zerocheck_lift_track[1],lift_track_motor[1].angle,Position);
//	pos_lift_track[0] = (int)(pos_before0/19.0f);
//	pos_lift_track[1] = (int)(pos_before1/19.0f);
	pos_lift_track[0] = ZeroCheck(&zerocheck_lift_track[0],lift_track_motor[0].angle,Position);
	pos_lift_track[1] = ZeroCheck(&zerocheck_lift_track[1],lift_track_motor[1].angle,Position);
	/*************************************************************************************/
	
	
	/************************************** 仓库电机 *************************************/
//	pos_before2 = ZeroCheck(&zerocheck_warehouse_motor,warehouse_motor.angle,Position);
//	pos_warehouse = (int)(pos_before2/19.0f);
	pos_warehouse = ZeroCheck(&zerocheck_warehouse_motor,warehouse_motor.angle,Position);
	/*************************************************************************************/
	
	
}


/**
  * @brief  过零检测结构体参数初始化
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


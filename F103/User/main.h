#ifndef __MAIN_H
#define __MAIN_H

/*Library*/
#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "math.h"
#include "string.h"

/*Mylib*/
#include "delay.h"
#include "tim.h"
#include "can.h"
#include "Nvic.h"
#include "ZeroCheck.h"
#include "Usart3.h"
#include "iwdg.h"
#include "tim2.h"

/*Algorithm*/
#include "pid.h"
#include "ModbusCRC16.h"

/*TASK*/
#include "StartTask.h"
#include "StateTask.h"
#include "AutoexecTask.h"
#include "MotorTask.h"
#include "ClampAngleTask.h"
#include "LiftTask.h"
#include "RotateTask.h"
//#include "ForwardTask.h"

/*FreeRTOS*/
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"


/*******************宏定义*************************/
#define ABS(x) ((x)>0? (x):(-(x))) 
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))

//控制模式
#define RC_MODE     					0X01		//遥控模式
#define KEY_MODE    					0X02		//键鼠模式
#define POWER_OFF_MODE 				0X03		//掉电模式
//底盘模式
#define CHASSIS_MODE					0X04		//底盘运动模式
#define CHASSIS_MODE_STATIC		0X05		//底盘静步模式
#define CHASSIS_MODE3					0X06		//底盘模式3
//上层模式
#define SENIOR_UP_MODE						0X07		//上层上升模式
#define SENIOR_MODE2							0X08		//上层模式2
#define SENIOR_AUTO_MODE					0X09		//上层自动控制模式


//自动执行模式auto_mode
#define LARGE_ISLAND_MINE	0X01		//大资源岛取矿
#define MINE_MIDAIR				0X02		//空接矿石
#define GET_MINE_MIDAIR		0X03		//空接接取矿石
#define SMALL_ISLAND_MINE 0X04		//小资源岛取矿
#define EXCHANGE_MINE			0X05		//兑换矿石

/*******接口宏定义*******/
//前移气缸电磁阀
#define FORWARD_SOLENOID_GPIO_PORT		GPIOA
#define FORWARD_SOLENOID_GPIO_CLK			RCC_APB2Periph_GPIOA
#define FORWARD_SOLENOID_GPIO_PIN			GPIO_Pin_1
#define FORWARD_SOLENOID PAout(1)			//前移气缸电磁阀
//夹取气缸电磁阀
#define CLAMP_SOLENOID_GPIO_PORT			GPIOA
#define CLAMP_SOLENOID_GPIO_CLK				RCC_APB2Periph_GPIOA
#define CLAMP_SOLENOID_GPIO_PIN				GPIO_Pin_2
#define CLAMP_SOLENOID   PAout(2)			//夹取气缸电磁阀
//空接气缸电磁阀
#define MIDAIR_SOLENOID_GPIO_PORT			GPIOA
#define MIDAIR_SOLENOID_GPIO_CLK			RCC_APB2Periph_GPIOA
#define MIDAIR_SOLENOID_GPIO_PIN			GPIO_Pin_3
#define MIDAIR_SOLENOID  PAout(3)			//空接气缸电磁阀
//兑换气缸电磁阀
#define EXCHANGE_SOLENOID_GPIO_PORT			GPIOA
#define EXCHANGE_SOLENOID_GPIO_CLK			RCC_APB2Periph_GPIOA
#define EXCHANGE_SOLENOID_GPIO_PIN			GPIO_Pin_4
#define EXCHANGE_SOLENOID  PAout(4)			//空接气缸电磁阀
//光电门
#define PHOTOGATE_GPIO_PORT							GPIOC
#define PHOTOGATE_GPIO_CLK							RCC_APB2Periph_GPIOC
#define PHOTOGATE_GPIO_PIN							GPIO_Pin_13
#define PHOTOGATE  PCin(13)						//光电门
 //光电门地线开关
#define PHOTOGATE_GND_GPIO_PORT							GPIOA
#define PHOTOGATE_GND_GPIO_CLK							RCC_APB2Periph_GPIOA
#define PHOTOGATE_GND_GPIO_PIN							GPIO_Pin_0
#define PHOTOGATE_GND  PAout(0)						//光电门地线开关


//IO??????
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO?????
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n) 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n) 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n) 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n) 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n) 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n) 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n) 

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n) 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  




typedef struct
{
	s16 angle;					//角度返回值
	s16 last_angle;			//前一次角度返回值
	s16 cycle;					//圈数
	s32 real_posion;		//实际位置
	s16 speed;					//实时速度
	s16 flow;						//实时电流
	s16 temperature;		//实时温度
}rmc620_t;

typedef union {
	unsigned char data;
	__packed struct{
		u8 lift_once_flag:2;					//上层一级抬升标志位，0为下降，1为抬升
		u8 lift_down_twice_flag:2; 		//底盘控制二级抬升标志位，0为下降，1为抬高
		u8 forward_flag:1;						//上层前移标志位，0为收回，1为前移
		u8 clamp_flag:1;							//上层夹取标志位，0为松开，1为夹取
		u8 midair_solenoid_flag:1;		//空接电磁阀标志位
		u8 exchange_solenoid_flag:1;	//兑换电磁阀标志位
	}flag;				
}FlagWithMaster_t;													//和B板之间的标志位通讯

typedef enum{
	//默认位置
	CAMERA_DEFAULT 			= 0,
	
	//pitch轴
	CAMERA_PITCH_UP 		= 1,
	CAMERA_PITCH_DOWN		= 2,
	
	//yaw轴
	CAMERA_YAW_REAR		= 3
	
}FlagOfCameraSteering;

typedef struct
{
	u8 control_mode;										//控制模式，1为遥控器，0为键鼠
	u8 control_target;									//控制目标，0为掉电，1为底盘，234为上层三种模式
	
	u8 auto_mode;												//自动执行模式选择
	u8 auto_mode_pre;

	u8 argle_use_flag;									//是否使用角度陀螺仪数据
	u8 lift_once_flag;									//四连杆抬升标志
	u8 lift_once_flag_pre;
	u8 lift_twice_flag;									//二级抬升标志
	u8 rotate_flag;											//爪子旋转电机标志
	
	u8 forward_solenoid_flag;						//前移电磁阀标志
	u8 clamp_solenoid_flag;							//夹取电磁阀标志
	u8 midair_solenoid_flag;						//空接电磁阀标志
	u8 exchange_solenoid_flag;					//兑换电磁阀标志
	
	u8 photogate_flag;									//光电门标志
	
	FlagOfCameraSteering camera_pitch;	//图传pitch轴
	FlagOfCameraSteering camera_yaw;		//图传yaw轴
//	u8 can_connect;
//	u32 can_cnt;
//	
//	u8 rc_connect;
//	u32 rc_cnt;
//	
//	u8 gyro_connect;
//	u32 gyro_cnt;
//	u8 gyro_use_flag;         //是否启用陀螺仪
//	
//	u8 initial_flag;
//	u8 offline_flag;


//	u8 gyro_initial_flag;
}Flag_t;												//状态机模式标志

//typedef struct
//{

////	u8 forward_motor_flag;			//前移标志
//	
//}ExecFlag_t;										//状态机执行动作标志


/*******************全局引用数据*************************/
extern Flag_t g_Flag;
//extern ExecFlag_t g_ExecFlag;


#endif

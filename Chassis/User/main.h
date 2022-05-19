#ifndef __MAIN_H_
#define __MAIN_H_



#include <stdio.h>
/*Library*/
#include "stm32f10x.h"
#include "misc.h"
#include "string.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_i2c.h"

/*Mylib*/
#include "can1.h"
#include "can2.h"
#include "led.h"
	//#include "usart.h"
#include "delay.h"
#include "nvic.h"
#include "remote.h"
#include "tim.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "uart4.h"
#include "uart5.h"
#include "JudgeSend.h"
#include "ZeroCheck.h"
//#include "iwdg.h"

/*Algorithm*/
#include "pid.h"
#include "algorithmOfCRC.h"
#include "modbusCRC16.h"

/*TASK*/
#include "StartTask.h"
#include "StateTask.h"
#include "ChassisTask.h"
#include "Master2SlaveTask.h"
#include "OfflineCheckTask.h"
#include "MotorCalTask.h"
#include "MotorFlagTask.h"

/*FreeRTOS*/
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
#define SENIOR_UP_MODE				0X07		//上层上升模式
#define SENIOR_MODE2					0X08		//上层模式2
#define SENIOR_AUTO_MODE					0X09		//上层模式3

//拨杆位置
#define UP 		 	1
#define MIDDLE 	3
#define DOWN   	2

#define LIFT_UP           1
#define LIFT_DOWN         2

/*******接口宏定义*******/
//救援电磁阀
#define RESCUE_SOLENOID_GPIO_PORT			GPIOA
#define RESCUE_SOLENOID_GPIO_CLK			RCC_APB2Periph_GPIOA
#define RESCUE_SOLENOID_GPIO_PIN			GPIO_Pin_6
#define RESCUE_SOLENOID_ON 						PAout(6)=1										//救援电磁阀打开
#define RESCUE_SOLENOID_OFF 					PAout(6)=0										//救援电磁阀关闭

//复活电磁阀
#define RESURGE_SOLENOID_GPIO_PORT			GPIOA
#define RESURGE_SOLENOID_GPIO_CLK			RCC_APB2Periph_GPIOA
#define RESURGE_SOLENOID_GPIO_PIN			GPIO_Pin_7
#define RESURGE_SOLENOID_ON 					PAout(7)=1										//复活电磁阀打开
#define RESURGE_SOLENOID_OFF 					PAout(7)=0										//复活电磁阀关闭


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

#define YAW_INFROUT      ServoGimbal_yaw(5000)
#define YAW_BEHIND       ServoGimbal_yaw(1520); 
#define PITCH_DOWN       ServoGimbal_pitch(2700); 
#define PITCH_UP         ServoGimbal_pitch(5800); 
/*******************结构体定义*************************/
//typedef union
//{
//	float fdata;			//4?
//	unsigned long idata;
//}
//FloatLongType;

//typedef union
//{
//	float fdata;			//4?
//	unsigned char idata[4];
//}
//FloatCharType;

typedef struct
{
    s16 angle;					//角度返回值
	s16 last_angle;				//前一次角度返回值
	s16 cycle;					//圈数
	s32 real_posion;			//实际位置
    s16 speed;					//实时速度
	s16 flow;					//实时电流
	s16 temperature;			//实时温度
}rmc620_t;

typedef struct
{
	unsigned short angle;  //0~8191(0x1fff)
	short speed;       //     RPM
	short last_angle;
	s16 cycle;
	s16 real_posion;
	short Disconnect;
}rmc610_t;

typedef struct
{
	uint8_t RobotID;
	uint8_t RobotLevel;
	uint8_t bulletFreq;
	uint16_t remainHP;
	uint16_t shooterHeat17;
	uint16_t shooterHeat42;
	float bulletSpeed;
	float realChassisOutV;
	float realChassisOutA;
	float realChassispower;
	float remainEnergy;       //剩余能量
}tGameInfo;

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
}FlagWithSlave_t;													//和C板之间的标志位通讯



typedef struct
{
	
	
	u8 forward_flag;						//上层前移标志位，0为收回，1为前移
	u8 clamp_flag;							//上层夹取标志位，0为松开，1为夹取
	u8 midair_solenoid_flag;		//空接电磁阀标志位
	u8 exchange_solenoid_flag;	//兑换电磁阀标志位
	
	u8 lift_once_flag;					//上层一级抬升标志位，0为下降，1为抬升
	u8 lift_down_twice_flag; 		//底盘控制二级抬升标志位，0为下降，1为抬高
	
	u8 warehouse_flag;					//仓库旋转电机标志位，0为不动，1为右旋转，2为左旋转
	
	u8 control_mode;          	//控制模式
	u8 control_target;         	//控制目标
	
	
	u8 gyro_connect;
	u32 gyro_cnt;
	u8 gyro_use_flag;         		//是否启用陀螺仪
	u8 gyro_initial_flag;
	
	u8 rescue_solenoid_flag;					//救援电磁阀
	u8 resurge_solenoid_flag;					//复活电磁阀
	
	u8 soft_reset_flag;								//软件复位
	
	u8 can1_connect;
	u32 can1_cnt;
	
	u8 can2_connect;
	u32 can2_cnt;
	
	u8 rc_connect;
	u32 rc_cnt;
	
	u8 initial_flag;
	u8 offline_flag;
	

//	u8 lift_state;
//	u8 lift_finish;
//	u8 lift_flag;
}Flag_t;


/*******************全局引用数据*************************/
extern Flag_t g_Flag;
extern int vx_set, vy_set, vw_set;

#endif




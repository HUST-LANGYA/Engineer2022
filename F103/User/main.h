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


/*******************�궨��*************************/
#define ABS(x) ((x)>0? (x):(-(x))) 
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))

//����ģʽ
#define RC_MODE     					0X01		//ң��ģʽ
#define KEY_MODE    					0X02		//����ģʽ
#define POWER_OFF_MODE 				0X03		//����ģʽ
//����ģʽ
#define CHASSIS_MODE					0X04		//�����˶�ģʽ
#define CHASSIS_MODE_STATIC		0X05		//���̾���ģʽ
#define CHASSIS_MODE3					0X06		//����ģʽ3
//�ϲ�ģʽ
#define SENIOR_UP_MODE						0X07		//�ϲ�����ģʽ
#define SENIOR_MODE2							0X08		//�ϲ�ģʽ2
#define SENIOR_AUTO_MODE					0X09		//�ϲ��Զ�����ģʽ


//�Զ�ִ��ģʽauto_mode
#define LARGE_ISLAND_MINE	0X01		//����Դ��ȡ��
#define MINE_MIDAIR				0X02		//�սӿ�ʯ
#define GET_MINE_MIDAIR		0X03		//�սӽ�ȡ��ʯ
#define SMALL_ISLAND_MINE 0X04		//С��Դ��ȡ��
#define EXCHANGE_MINE			0X05		//�һ���ʯ

/*******�ӿں궨��*******/
//ǰ�����׵�ŷ�
#define FORWARD_SOLENOID_GPIO_PORT		GPIOA
#define FORWARD_SOLENOID_GPIO_CLK			RCC_APB2Periph_GPIOA
#define FORWARD_SOLENOID_GPIO_PIN			GPIO_Pin_1
#define FORWARD_SOLENOID PAout(1)			//ǰ�����׵�ŷ�
//��ȡ���׵�ŷ�
#define CLAMP_SOLENOID_GPIO_PORT			GPIOA
#define CLAMP_SOLENOID_GPIO_CLK				RCC_APB2Periph_GPIOA
#define CLAMP_SOLENOID_GPIO_PIN				GPIO_Pin_2
#define CLAMP_SOLENOID   PAout(2)			//��ȡ���׵�ŷ�
//�ս����׵�ŷ�
#define MIDAIR_SOLENOID_GPIO_PORT			GPIOA
#define MIDAIR_SOLENOID_GPIO_CLK			RCC_APB2Periph_GPIOA
#define MIDAIR_SOLENOID_GPIO_PIN			GPIO_Pin_3
#define MIDAIR_SOLENOID  PAout(3)			//�ս����׵�ŷ�
//�һ����׵�ŷ�
#define EXCHANGE_SOLENOID_GPIO_PORT			GPIOA
#define EXCHANGE_SOLENOID_GPIO_CLK			RCC_APB2Periph_GPIOA
#define EXCHANGE_SOLENOID_GPIO_PIN			GPIO_Pin_4
#define EXCHANGE_SOLENOID  PAout(4)			//�ս����׵�ŷ�
//�����
#define PHOTOGATE_GPIO_PORT							GPIOC
#define PHOTOGATE_GPIO_CLK							RCC_APB2Periph_GPIOC
#define PHOTOGATE_GPIO_PIN							GPIO_Pin_13
#define PHOTOGATE  PCin(13)						//�����
 //����ŵ��߿���
#define PHOTOGATE_GND_GPIO_PORT							GPIOA
#define PHOTOGATE_GND_GPIO_CLK							RCC_APB2Periph_GPIOA
#define PHOTOGATE_GND_GPIO_PIN							GPIO_Pin_0
#define PHOTOGATE_GND  PAout(0)						//����ŵ��߿���


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
	s16 angle;					//�Ƕȷ���ֵ
	s16 last_angle;			//ǰһ�νǶȷ���ֵ
	s16 cycle;					//Ȧ��
	s32 real_posion;		//ʵ��λ��
	s16 speed;					//ʵʱ�ٶ�
	s16 flow;						//ʵʱ����
	s16 temperature;		//ʵʱ�¶�
}rmc620_t;

typedef union {
	unsigned char data;
	__packed struct{
		u8 lift_once_flag:2;					//�ϲ�һ��̧����־λ��0Ϊ�½���1Ϊ̧��
		u8 lift_down_twice_flag:2; 		//���̿��ƶ���̧����־λ��0Ϊ�½���1Ϊ̧��
		u8 forward_flag:1;						//�ϲ�ǰ�Ʊ�־λ��0Ϊ�ջأ�1Ϊǰ��
		u8 clamp_flag:1;							//�ϲ��ȡ��־λ��0Ϊ�ɿ���1Ϊ��ȡ
		u8 midair_solenoid_flag:1;		//�սӵ�ŷ���־λ
		u8 exchange_solenoid_flag:1;	//�һ���ŷ���־λ
	}flag;				
}FlagWithMaster_t;													//��B��֮��ı�־λͨѶ

typedef enum{
	//Ĭ��λ��
	CAMERA_DEFAULT 			= 0,
	
	//pitch��
	CAMERA_PITCH_UP 		= 1,
	CAMERA_PITCH_DOWN		= 2,
	
	//yaw��
	CAMERA_YAW_REAR		= 3
	
}FlagOfCameraSteering;

typedef struct
{
	u8 control_mode;										//����ģʽ��1Ϊң������0Ϊ����
	u8 control_target;									//����Ŀ�꣬0Ϊ���磬1Ϊ���̣�234Ϊ�ϲ�����ģʽ
	
	u8 auto_mode;												//�Զ�ִ��ģʽѡ��
	u8 auto_mode_pre;

	u8 argle_use_flag;									//�Ƿ�ʹ�ýǶ�����������
	u8 lift_once_flag;									//������̧����־
	u8 lift_once_flag_pre;
	u8 lift_twice_flag;									//����̧����־
	u8 rotate_flag;											//צ����ת�����־
	
	u8 forward_solenoid_flag;						//ǰ�Ƶ�ŷ���־
	u8 clamp_solenoid_flag;							//��ȡ��ŷ���־
	u8 midair_solenoid_flag;						//�սӵ�ŷ���־
	u8 exchange_solenoid_flag;					//�һ���ŷ���־
	
	u8 photogate_flag;									//����ű�־
	
	FlagOfCameraSteering camera_pitch;	//ͼ��pitch��
	FlagOfCameraSteering camera_yaw;		//ͼ��yaw��
//	u8 can_connect;
//	u32 can_cnt;
//	
//	u8 rc_connect;
//	u32 rc_cnt;
//	
//	u8 gyro_connect;
//	u32 gyro_cnt;
//	u8 gyro_use_flag;         //�Ƿ�����������
//	
//	u8 initial_flag;
//	u8 offline_flag;


//	u8 gyro_initial_flag;
}Flag_t;												//״̬��ģʽ��־

//typedef struct
//{

////	u8 forward_motor_flag;			//ǰ�Ʊ�־
//	
//}ExecFlag_t;										//״̬��ִ�ж�����־


/*******************ȫ����������*************************/
extern Flag_t g_Flag;
//extern ExecFlag_t g_ExecFlag;


#endif

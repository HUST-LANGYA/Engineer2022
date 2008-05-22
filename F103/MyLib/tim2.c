/**
	*@brief 		tim2
							输出PWM波到两个图传舵机
	*@author		祝科伟
	*@date			2022-5-20
*/
#include "tim2.h"

//单位：us，变化范围：500--2500(0.5ms-2.5ms)，高电平时间，控制PWM占空比，周期是20ms
unsigned short camera_pitch = 1450;	
unsigned short camera_yaw 	= 1250;



/**
 * @brief  tim2初始化
 * @param  None
 * @retval None
 */
void TIM2_PWM_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,  ENABLE);  //时钟配置
	
	//TIM2部分重映射		PA15 CH1 		PB3 CH2
  GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);	
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	
													//禁用JTAG功能，把PB3/PA15作为普通口使用
	
		//PB3 CH2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		//PA15 CH1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	//初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = 20000-1; 
	TIM_TimeBaseStructure.TIM_Prescaler =72-1;  
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
	
	//初始化TIM2 CH1/CH2	PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //TIM2 OC1
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //TIM2 OC2
  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	TIM_Cmd(TIM2, ENABLE);
	
}
	
	
void CameraSteeringEngine_Set(void)       
{
	switch(g_Flag.camera_pitch)
	{
		case CAMERA_DEFAULT:
			
		break;
		
		case CAMERA_PITCH_UP:
			
		break;
		
		case CAMERA_PITCH_DOWN:
			
		break;
		
		default:
			
		break;
	}
	
	switch(g_Flag.camera_yaw)
	{
		case CAMERA_DEFAULT:
			
		break;
		
		case CAMERA_YAW_REAR:
			
		break;
		
		default:
			
		break;
	}
	
	
	TIM_SetCompare1(TIM2,camera_pitch);
	TIM_SetCompare2(TIM2,camera_yaw);
	
	
}





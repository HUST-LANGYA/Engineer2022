/**
	*@brief 		tim2
							���PWM��������ͼ�����
	*@author		ף��ΰ
	*@date			2022-5-20
*/
#include "tim2.h"

//��λ��us���仯��Χ��500--2500(0.5ms-2.5ms)���ߵ�ƽʱ�䣬����PWMռ�ձȣ�������20ms
unsigned short camera_pitch = 1450;	
unsigned short camera_yaw 	= 1250;



/**
 * @brief  tim3��ʼ��
 * @param  None
 * @retval None
 */
void TIM3_PWM_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,  ENABLE);  //ʱ������
	
	//TIM3��ȫ��ӳ��		PC6 CH1 		PC7 CH2
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);	
//  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	
//													//����JTAG���ܣ���PB3/PA15��Ϊ��ͨ��ʹ��
	
		//PC6 CH1 		PC7 CH2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
	//��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = 20000-1; 
	TIM_TimeBaseStructure.TIM_Prescaler =72-1;  
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	//��ʼ��TIM3 CH1/CH2	PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //TIM3 OC1
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //TIM3 OC2
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 
	TIM_Cmd(TIM3, ENABLE);
	
}
	
	
void CameraSteeringEngine_Set(void)       
{
	switch(g_Flag.camera_pitch)
	{
		case CAMERA_DEFAULT:
			camera_pitch = 1450;	
		break;
		
		case CAMERA_PITCH_UP:
			
		break;
		
		case CAMERA_PITCH_DOWN:
			camera_pitch = 1650;	
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
	
	
	TIM_SetCompare1(TIM3,camera_pitch);
	TIM_SetCompare2(TIM3,camera_yaw);
	
	
}





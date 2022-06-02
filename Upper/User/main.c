/**
	*@brief 		�������ļ�	
	*@author		ף��ΰ
	*@date			2021-12-09
*/
#include "main.h"

Flag_t g_Flag;
//ExecFlag_t g_ExecFlag;

void BSP_init(void);
void gpioConfig(void);
void flagInit(void);


/**
  	*@brief 		������
  	*@param			void
	*@return			0
*/
int main(void)
{
	BSP_init();
	flagInit();
	Data_Send_to_Master_Init();
	delay_ms(100);
	ZeroCheck_Init();
  //	Pid_Motor_Init();
	startTask();
	vTaskStartScheduler();
	while (1)
	{
			 ;
	}
}

/**
  	*@brief 		�����ʼ������
  	*@param			void
	*@return			void
*/
void BSP_init(void)
{
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
//	USART3_Configuration();
	TIM3_PWM_Init();
	TIM4_Init();
	TIM6_Configration();
	TIM7_Configration();
	CAN1_Config();
	can2Config();
	gpioConfig();
	UART4_Configuration();
//	IWDG_Config(IWDG_Prescaler_128 ,625);
}

/**
  	*@brief 		GPIO��ʼ��
								����IO�ڿ�������ĳ�ʼ�������ŷ�
  	*@param		  void
	*@return		  void
*/
void gpioConfig(void)
{
		GPIO_InitTypeDef gpio;

	//ǰ�����׵�ŷ�(�ӿڼ��궨��)
	RCC_APB2PeriphClockCmd(FORWARD_SOLENOID_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = FORWARD_SOLENOID_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FORWARD_SOLENOID_GPIO_PORT,&gpio);
	
	//��ȡ���׵�ŷ�(�ӿڼ��궨��)
	RCC_APB2PeriphClockCmd(CLAMP_SOLENOID_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = CLAMP_SOLENOID_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CLAMP_SOLENOID_GPIO_PORT,&gpio);
	
	//�ս����׵�ŷ�(�ӿڼ��궨��)
	RCC_APB2PeriphClockCmd(MIDAIR_SOLENOID_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = MIDAIR_SOLENOID_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MIDAIR_SOLENOID_GPIO_PORT,&gpio);
	
//	//�һ����׵�ŷ�(�ӿڼ��궨��)
//	RCC_APB2PeriphClockCmd(EXCHANGE_SOLENOID_GPIO_CLK, ENABLE);
//	gpio.GPIO_Pin = EXCHANGE_SOLENOID_GPIO_PIN;
//	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
//	gpio.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(EXCHANGE_SOLENOID_GPIO_PORT,&gpio);


	//�����(�ӿڼ��궨��)
	RCC_APB2PeriphClockCmd(PHOTOGATE_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = PHOTOGATE_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_IPD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PHOTOGATE_GPIO_PORT,&gpio);
	
	//����ŵ���(�ӿڼ��궨��)
	RCC_APB2PeriphClockCmd(PHOTOGATE_GND_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = PHOTOGATE_GND_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PHOTOGATE_GND_GPIO_PORT,&gpio);
	PHOTOGATE_GND = 1;
}

/**
  	*@brief 		��־λ��ʼ������
  	*@param			void
	*@return			void
*/
void flagInit(void)
{
	/***********************״̬��ģʽ��־λ*****************/
	//����ģʽ
	g_Flag.control_mode		= RC_MODE;									//Ĭ�ϼ������
	g_Flag.control_target	= POWER_OFF_MODE;						//Ĭ�ϵ���ģʽ	
	
	//ģʽ������ʼ��
	g_Flag.auto_mode = AUTO_MODE_OFF;
	g_Flag.auto_mode_pre = AUTO_MODE_OFF;
	g_Flag.auto_end_flag = 0;
	//�Ƕ��������Ƿ�ʹ��
	g_Flag.argle_use_flag = 0;
	
	/***********************״̬��ִ�б�־λ*****************/
	g_Flag.lift_once_flag = 0;					//����̧�������½�״̬
	g_Flag.lift_once_flag_pre = g_Flag.lift_once_flag;
	g_Flag.rotate_flag = 0;							//��ת�������
	
	g_Flag.forward_solenoid_flag = 0;		//ǰ�Ƶ�ŷ��ر�
	g_Flag.clamp_solenoid_flag = 0;			//��ȡ��ŷ��ر�
	g_Flag.midair_solenoid_flag = 0;		//�սӵ�ŷ��ر�
	g_Flag.exchange_solenoid_flag = 0;	//�һ���ŷ��ر�
	
	g_Flag.camera_pitch = CAMERA_DEFAULT;
	g_Flag.camera_yaw = CAMERA_DEFAULT;
}

///**********************************************************************************************************
//*�� �� ��: delay_ms
//*����˵��: ms��ʱ
//*��    ��: ��
//*�� �� ֵ: ��
//************************** ++********************************************************************************/
//void delay_ms(unsigned long t)
//{
//	int i;
//	for( i=0;i<t;i++)
//	{
//		int a=12000;
// 		while(a--);
//	}
//}

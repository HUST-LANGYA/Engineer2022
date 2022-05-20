/**
	*@brief 		�������ļ�	
	*@author		ף��ΰ
	*@date			2021-12-19
*/
#include "main.h"

Flag_t g_Flag;

void BSP_init(void);
void gflagInit(void);
void gpioConfig(void);

int main(void)
{
		BSP_init();
		delay_ms(1000);
		ZeroCheck_Init();																	//�������ʼ��
		motorPidInit();																		//�����������PID��ʼ��
		startTask();
		vTaskStartScheduler();
		while (1)
		{
				 ;
		}
}

/**
  	*@brief 		��ʼ������
  	*@param		  void
	*@return		  void
*/
void BSP_init(void)
{
/*-------------------------------------------�����ʼ��-----------------------------------------------------------*/

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );
	
	can1Config();
	can2Config();

	TIM4_Init();
	TIM6_Configration();
	TIM7_Configration();
	
//	IWDG_Config(IWDG_Prescaler_128 ,625);
/*-----------------------------------------���ܳ�ʼ��-------------------------------------------------------------*/
	LED_Config();
	gpioConfig();
	uasrt1Config();
	uasrt2Config();
	usart3Config();
//	UART4_Configuration();

/*------------------------------------------������ʼ��---------------------------------------------------------------*/
	rc_reset();                                    		//ң������ʼ������������λ���㣩
	gflagInit();																			//��־λ��ʼ��
	chassisVelPidInit(7.5f, 2.5f, 10.0f, 100.0f);			//����PID��ʼ��
	

	/*�����ٶȳ�ʼ�趨Ϊ0*/
	vx_set = 0;
	vy_set = 0;
	vw_set = 0;
	
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

	//��Ԯģ���ŷ�(�ӿڼ��궨��)
	RCC_APB2PeriphClockCmd(RESCUE_SOLENOID_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = RESCUE_SOLENOID_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RESCUE_SOLENOID_GPIO_PORT,&gpio);
	
	//����ģ���ŷ�(�ӿڼ��궨��)
	RCC_APB2PeriphClockCmd(RESURGE_SOLENOID_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = RESURGE_SOLENOID_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RESURGE_SOLENOID_GPIO_PORT,&gpio);
}


/**
  	*@brief 		ȫ�ֱ�־λ��ʼ��
  	*@param		  void
	*@return		  void
*/
void gflagInit(void)
{
	g_Flag.soft_reset_flag = 0;		//Ĭ�ϲ���λ
	
	g_Flag.control_mode		= RC_MODE;									//Ĭ�ϼ������
	g_Flag.control_target	= POWER_OFF_MODE;						//Ĭ�ϵ���ģʽ	
	
	g_Flag.lift_down_twice_flag 		= 0;							//Ĭ�϶���̧���ĳ�ʼ״̬Ϊ�½�
	g_Flag.lift_once_flag						= 0;							//Ĭ��һ��̧�������½�״̬
	g_Flag.forward_flag							= 0;							//Ĭ���ϲ�ǰ�ƴ����ջ�״̬
	g_Flag.clamp_flag								= 0;							//Ĭ���ϲ��ȡΪ�ɿ�״̬
	g_Flag.midair_solenoid_flag			= 0;							//Ĭ�Ͽսӵ�ŷ��ر�
	g_Flag.exchange_solenoid_flag		= 0;							//Ĭ�϶һ���ŷ��ر�
	
	g_Flag.warehouse_flag						= 0;							//Ĭ�ϲֿ�������
	
	g_Flag.gyro_use_flag = 0;                      		//�Ƿ����������ǣ�Ĭ��Ϊ��ʹ�ã�
	
	g_Flag.rescue_solenoid_flag = 0;									//��ʼʱ�رվ�Ԯģ��
	g_Flag.resurge_solenoid_flag = 0;									//��ʼʱ�رո���ģ��
}


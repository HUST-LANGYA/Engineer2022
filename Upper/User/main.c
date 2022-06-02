/**
	*@brief 		主函数文件	
	*@author		祝科伟
	*@date			2021-12-09
*/
#include "main.h"

Flag_t g_Flag;
//ExecFlag_t g_ExecFlag;

void BSP_init(void);
void gpioConfig(void);
void flagInit(void);


/**
  	*@brief 		主函数
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
  	*@brief 		外设初始化函数
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
  	*@brief 		GPIO初始化
								用于IO口控制外设的初始化，如电磁阀
  	*@param		  void
	*@return		  void
*/
void gpioConfig(void)
{
		GPIO_InitTypeDef gpio;

	//前移气缸电磁阀(接口见宏定义)
	RCC_APB2PeriphClockCmd(FORWARD_SOLENOID_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = FORWARD_SOLENOID_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FORWARD_SOLENOID_GPIO_PORT,&gpio);
	
	//夹取气缸电磁阀(接口见宏定义)
	RCC_APB2PeriphClockCmd(CLAMP_SOLENOID_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = CLAMP_SOLENOID_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CLAMP_SOLENOID_GPIO_PORT,&gpio);
	
	//空接气缸电磁阀(接口见宏定义)
	RCC_APB2PeriphClockCmd(MIDAIR_SOLENOID_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = MIDAIR_SOLENOID_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MIDAIR_SOLENOID_GPIO_PORT,&gpio);
	
//	//兑换气缸电磁阀(接口见宏定义)
//	RCC_APB2PeriphClockCmd(EXCHANGE_SOLENOID_GPIO_CLK, ENABLE);
//	gpio.GPIO_Pin = EXCHANGE_SOLENOID_GPIO_PIN;
//	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
//	gpio.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(EXCHANGE_SOLENOID_GPIO_PORT,&gpio);


	//光电门(接口见宏定义)
	RCC_APB2PeriphClockCmd(PHOTOGATE_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = PHOTOGATE_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_IPD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PHOTOGATE_GPIO_PORT,&gpio);
	
	//光电门地线(接口见宏定义)
	RCC_APB2PeriphClockCmd(PHOTOGATE_GND_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = PHOTOGATE_GND_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PHOTOGATE_GND_GPIO_PORT,&gpio);
	PHOTOGATE_GND = 1;
}

/**
  	*@brief 		标志位初始化函数
  	*@param			void
	*@return			void
*/
void flagInit(void)
{
	/***********************状态机模式标志位*****************/
	//控制模式
	g_Flag.control_mode		= RC_MODE;									//默认键鼠控制
	g_Flag.control_target	= POWER_OFF_MODE;						//默认掉电模式	
	
	//模式变量初始化
	g_Flag.auto_mode = AUTO_MODE_OFF;
	g_Flag.auto_mode_pre = AUTO_MODE_OFF;
	g_Flag.auto_end_flag = 0;
	//角度陀螺仪是否使用
	g_Flag.argle_use_flag = 0;
	
	/***********************状态机执行标志位*****************/
	g_Flag.lift_once_flag = 0;					//二级抬升处于下降状态
	g_Flag.lift_once_flag_pre = g_Flag.lift_once_flag;
	g_Flag.rotate_flag = 0;							//旋转电机不动
	
	g_Flag.forward_solenoid_flag = 0;		//前移电磁阀关闭
	g_Flag.clamp_solenoid_flag = 0;			//夹取电磁阀关闭
	g_Flag.midair_solenoid_flag = 0;		//空接电磁阀关闭
	g_Flag.exchange_solenoid_flag = 0;	//兑换电磁阀关闭
	
	g_Flag.camera_pitch = CAMERA_DEFAULT;
	g_Flag.camera_yaw = CAMERA_DEFAULT;
}

///**********************************************************************************************************
//*函 数 名: delay_ms
//*功能说明: ms延时
//*形    参: 无
//*返 回 值: 无
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

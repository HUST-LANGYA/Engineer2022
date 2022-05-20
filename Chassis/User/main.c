/**
	*@brief 		主函数文件	
	*@author		祝科伟
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
		ZeroCheck_Init();																	//过零检测初始化
		motorPidInit();																		//其他三个电机PID初始化
		startTask();
		vTaskStartScheduler();
		while (1)
		{
				 ;
		}
}

/**
  	*@brief 		初始化函数
  	*@param		  void
	*@return		  void
*/
void BSP_init(void)
{
/*-------------------------------------------外设初始化-----------------------------------------------------------*/

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );
	
	can1Config();
	can2Config();

	TIM4_Init();
	TIM6_Configration();
	TIM7_Configration();
	
//	IWDG_Config(IWDG_Prescaler_128 ,625);
/*-----------------------------------------功能初始化-------------------------------------------------------------*/
	LED_Config();
	gpioConfig();
	uasrt1Config();
	uasrt2Config();
	usart3Config();
//	UART4_Configuration();

/*------------------------------------------参数初始化---------------------------------------------------------------*/
	rc_reset();                                    		//遥控器初始化函数（所有位清零）
	gflagInit();																			//标志位初始化
	chassisVelPidInit(7.5f, 2.5f, 10.0f, 100.0f);			//底盘PID初始化
	

	/*底盘速度初始设定为0*/
	vx_set = 0;
	vy_set = 0;
	vw_set = 0;
	
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

	//救援模块电磁阀(接口见宏定义)
	RCC_APB2PeriphClockCmd(RESCUE_SOLENOID_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = RESCUE_SOLENOID_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RESCUE_SOLENOID_GPIO_PORT,&gpio);
	
	//复活模块电磁阀(接口见宏定义)
	RCC_APB2PeriphClockCmd(RESURGE_SOLENOID_GPIO_CLK, ENABLE);
	gpio.GPIO_Pin = RESURGE_SOLENOID_GPIO_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RESURGE_SOLENOID_GPIO_PORT,&gpio);
}


/**
  	*@brief 		全局标志位初始化
  	*@param		  void
	*@return		  void
*/
void gflagInit(void)
{
	g_Flag.soft_reset_flag = 0;		//默认不复位
	
	g_Flag.control_mode		= RC_MODE;									//默认键鼠控制
	g_Flag.control_target	= POWER_OFF_MODE;						//默认掉电模式	
	
	g_Flag.lift_down_twice_flag 		= 0;							//默认二级抬升的初始状态为下降
	g_Flag.lift_once_flag						= 0;							//默认一级抬升处于下降状态
	g_Flag.forward_flag							= 0;							//默认上层前移处于收回状态
	g_Flag.clamp_flag								= 0;							//默认上层夹取为松开状态
	g_Flag.midair_solenoid_flag			= 0;							//默认空接电磁阀关闭
	g_Flag.exchange_solenoid_flag		= 0;							//默认兑换电磁阀关闭
	
	g_Flag.warehouse_flag						= 0;							//默认仓库电机不动
	
	g_Flag.gyro_use_flag = 0;                      		//是否启用陀螺仪（默认为不使用）
	
	g_Flag.rescue_solenoid_flag = 0;									//初始时关闭救援模块
	g_Flag.resurge_solenoid_flag = 0;									//初始时关闭复活模块
}


/**
	*@brief 		状态转换任务
							用于状态机状态的切换
	*@author		祝科伟
	*@date			2021-12-09
*/
#include "StateTask.h"


uint32_t  Mode_Switch_high_water;


extern FlagWithMaster_t FlagOfMaster;		//接收到的B板标志位
//extern FLAG_AUTOEXEC_ENUM auto_large_enum;
//extern FLAG_AUTOEXEC_ENUM auto_large_enum_next;

//extern FLAG_AUTOEXEC_ENUM auto_midair_enum;
//extern FLAG_AUTOEXEC_ENUM auto_midair_enum_next;

//extern FLAG_AUTOEXEC_ENUM auto_small_enum;
//extern FLAG_AUTOEXEC_ENUM auto_small_enum_next;

extern DMA_InitTypeDef USART3_TX_dma;
/**
  	*@brief 		模式转换任务函数
  	*@param		  void
	*@return		  void
*/
void modeSwitchTask(void *pvParameters)
{
	
	portTickType xLastWakeTime;
	const portTickType xFrequency=1;
	
	modeInit();
	
	while(1){
		xLastWakeTime=xTaskGetTickCount();
		
//		clampAngleSend();
		modeSwitch();
		Slave2Master();
//		IWDG_Feed();
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
		
		#if INCLUDE_uxTaskGetStackHighWaterMark
        Mode_Switch_high_water = uxTaskGetStackHighWaterMark(NULL);
		#endif
  }

}

//void clampAngleSend(void)
//{
////	USART3->SR=0;
////	USART3->DR=0;
//	
//	DMA_Init(DMA1_Channel2,&USART3_TX_dma);
//	DMA_Cmd(DMA1_Channel2,ENABLE);
//}

/**
  	*@brief 		模式变量初始化
  	*@param		  void
	*@return		  void
*/
void modeInit(void)
{
	g_Flag.auto_mode = 0;
	g_Flag.auto_mode_pre = 0;

	
}

/**
  	*@brief 		模式转换函数
  	*@param		  void
	*@return		  void
*/
void modeSwitch(void)
{
	g_Flag.photogate_flag =	PHOTOGATE;			//获取光电门数据
	
	flagModeSwitch();												//模式检测
	
	if(g_Flag.control_target == SENIOR_AUTO_MODE)		//自动模式检测
			autoModeSwitch();
	
	
	
	switch(g_Flag.forward_solenoid_flag)
	{
		case 0:
			FORWARD_SOLENOID = 0;
			break;
		case 1:
			FORWARD_SOLENOID = 1;
			break;
		default:
			FORWARD_SOLENOID = 0;
			break;
	}
	
	switch(g_Flag.clamp_solenoid_flag)
	{
		case 0:
			CLAMP_SOLENOID = 0;
			break;
		case 1:
			CLAMP_SOLENOID = 1;
			break;
		default:
			CLAMP_SOLENOID = 0;
			break;
	}
	
	switch(g_Flag.midair_solenoid_flag)
	{
		case 0:
			MIDAIR_SOLENOID = 0;
			break;
		case 1:
			MIDAIR_SOLENOID = 1;
			break;
		default:
			MIDAIR_SOLENOID = 0;
			break;
	}
	
	switch(g_Flag.exchange_solenoid_flag)
	{
		case 0:
			EXCHANGE_SOLENOID = 0;
			break;
		case 1:
			EXCHANGE_SOLENOID = 1;
			break;
		default:
			EXCHANGE_SOLENOID = 0;
			break;
	}
	
}


/**
  	*@brief 		标志位状态转换
  	*@param		  void
	*@return		  void
*/
void flagModeSwitch(void)
{
	g_Flag.lift_once_flag_pre = g_Flag.lift_once_flag;
	
	
//	if(data_receive[7] == 0)				//是否使用自动模式
//	{
//		
//	}
	if(data_receive[0] == '!')				//确保数据正确
	{
			switch(data_receive[1])			//确认控制模式
			{
					case 1:
						g_Flag.control_mode = RC_MODE;
						break;
					
					case 2:
						g_Flag.control_mode = KEY_MODE;
						break;
					
					default:
						g_Flag.control_mode = RC_MODE;
						break;
			}
			
			switch(data_receive[2])			//确认控制模式
			{
					case 0:
						g_Flag.control_target = POWER_OFF_MODE;
						break;
					
					case 1:
						g_Flag.control_target = CHASSIS_MODE;
						break;
					
					case 2:
						g_Flag.control_target = SENIOR_UP_MODE;
						break;
					
					case 3:
						g_Flag.control_target = SENIOR_MODE2;
						break;
					
					case 4:
						g_Flag.control_target = SENIOR_AUTO_MODE;
						break;
					
					default:
						g_Flag.control_target = POWER_OFF_MODE;
						break;
			}
			
			if(g_Flag.control_target == SENIOR_AUTO_MODE)
			{
//				g_Flag.auto_mode = MINE_MIDAIR;
			}else
				g_Flag.auto_mode = 0;
			
			
			
			
			if(g_Flag.control_target == SENIOR_UP_MODE || g_Flag.control_target == SENIOR_MODE2)
			{
				
////					
////																													if(g_Flag.photogate_flag)									//调试！！！！！！！！！！！！！！！！！
////																													{
////																														g_Flag.clamp_solenoid_flag = 1;
////																													}
				g_Flag.clamp_solenoid_flag = FlagOfMaster.flag.clamp_flag;
				g_Flag.exchange_solenoid_flag = FlagOfMaster.flag.exchange_solenoid_flag;
				g_Flag.forward_solenoid_flag = FlagOfMaster.flag.forward_flag;
				g_Flag.lift_twice_flag = FlagOfMaster.flag.lift_down_twice_flag;
				g_Flag.lift_once_flag = FlagOfMaster.flag.lift_once_flag;
				g_Flag.midair_solenoid_flag = FlagOfMaster.flag.midair_solenoid_flag;
				
				
				
//					switch(data_receive[3])			//上层二级抬升
//					{
//							case 0:
//								g_Flag.lift_once_flag = 0;
//								break;
//							
//							case 1:
//								g_Flag.lift_once_flag = 1;
//								break;
//							
//							default:
//								g_Flag.lift_once_flag = 0;
//								break;
//					}
//					
//					switch(data_receive[4])			//前移控制标志位
//					{
//							case 0:
//								g_Flag.forward_solenoid_flag = 0;
//								break;
//							
//							case 1:
//								g_Flag.forward_solenoid_flag = 1;
//								break;
//							
//							default:
//								g_Flag.forward_solenoid_flag = 0;
//								break;
//					}
//					switch(data_receive[5])			//夹取控制标志位
//					{
//							case 0:
//								g_Flag.clamp_solenoid_flag = 0;
//								break;
//							
//							case 1:
//								g_Flag.clamp_solenoid_flag = 1;
//								break;
//							
//							default:
//								g_Flag.clamp_solenoid_flag = 0;
//								break;
//					}
//					
//					switch(data_receive[6])			//空接控制标志位
//					{
//							case 0:
//								g_Flag.midair_solenoid_flag = 0;
//								break;
//							
//							case 1:
//								g_Flag.midair_solenoid_flag = 1;
//								break;
//							
//							default:
//								g_Flag.midair_solenoid_flag = 0;
//								break;
//					}
//					
//					switch(data_receive[7])			//兑换控制标志位
//					{
//							case 0:
//								g_Flag.exchange_solenoid_flag = 0;
//								break;
//							
//							case 1:
//								g_Flag.exchange_solenoid_flag = 1;
//								break;
//							
//							default:
//								g_Flag.exchange_solenoid_flag = 0;
//								break;
//					}

			}
		
	}
	
	
	
}



/**
  	*@brief 		自动执行模式状态转换
  	*@param		  void
	*@return		  void
*/
void autoModeSwitch(void)
{
	
//	switch( data_receive[7] )
//	{
//		case 1:
//			g_Flag.auto_mode = LARGE_ISLAND_MINE;			//大资源岛取矿
//			break;
//		
//		case 2:
//			g_Flag.auto_mode = MINE_MIDAIR;						//空接矿石
//			break;
//		
//		case 3:
//			g_Flag.auto_mode = SMALL_ISLAND_MINE;			//大资源岛取矿
//			break;
	
//		default:
//			g_Flag.auto_mode = 0;
//			break;
//	}
	

}


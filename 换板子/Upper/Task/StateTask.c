/**
	*@brief 		״̬ת������
							����״̬��״̬���л�
	*@author		ף��ΰ
	*@date			2021-12-09
*/
#include "StateTask.h"


uint32_t  Mode_Switch_high_water;


extern FlagWithMaster_t FlagOfMaster;		//���յ���B���־λ
//extern FLAG_AUTOEXEC_ENUM auto_large_enum;
//extern FLAG_AUTOEXEC_ENUM auto_large_enum_next;

//extern FLAG_AUTOEXEC_ENUM auto_midair_enum;
//extern FLAG_AUTOEXEC_ENUM auto_midair_enum_next;

//extern FLAG_AUTOEXEC_ENUM auto_small_enum;
//extern FLAG_AUTOEXEC_ENUM auto_small_enum_next;

extern DMA_InitTypeDef USART3_TX_dma;
/**
  	*@brief 		ģʽת��������
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
  	*@brief 		ģʽ������ʼ��
  	*@param		  void
	*@return		  void
*/
void modeInit(void)
{
	g_Flag.auto_mode = 0;
	g_Flag.auto_mode_pre = 0;

	
}

/**
  	*@brief 		ģʽת������
  	*@param		  void
	*@return		  void
*/
void modeSwitch(void)
{
	g_Flag.photogate_flag =	PHOTOGATE;			//��ȡ���������
	
	flagModeSwitch();												//ģʽ���
	
	if(g_Flag.control_target == SENIOR_AUTO_MODE)		//�Զ�ģʽ���
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
  	*@brief 		��־λ״̬ת��
  	*@param		  void
	*@return		  void
*/
void flagModeSwitch(void)
{
	g_Flag.lift_once_flag_pre = g_Flag.lift_once_flag;
	
	
//	if(data_receive[7] == 0)				//�Ƿ�ʹ���Զ�ģʽ
//	{
//		
//	}
	if(data_receive[0] == '!')				//ȷ��������ȷ
	{
			switch(data_receive[1])			//ȷ�Ͽ���ģʽ
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
			
			switch(data_receive[2])			//ȷ�Ͽ���ģʽ
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
////																													if(g_Flag.photogate_flag)									//���ԣ���������������������������������
////																													{
////																														g_Flag.clamp_solenoid_flag = 1;
////																													}
				g_Flag.clamp_solenoid_flag = FlagOfMaster.flag.clamp_flag;
				g_Flag.exchange_solenoid_flag = FlagOfMaster.flag.exchange_solenoid_flag;
				g_Flag.forward_solenoid_flag = FlagOfMaster.flag.forward_flag;
				g_Flag.lift_twice_flag = FlagOfMaster.flag.lift_down_twice_flag;
				g_Flag.lift_once_flag = FlagOfMaster.flag.lift_once_flag;
				g_Flag.midair_solenoid_flag = FlagOfMaster.flag.midair_solenoid_flag;
				
				
				
//					switch(data_receive[3])			//�ϲ����̧��
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
//					switch(data_receive[4])			//ǰ�ƿ��Ʊ�־λ
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
//					switch(data_receive[5])			//��ȡ���Ʊ�־λ
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
//					switch(data_receive[6])			//�սӿ��Ʊ�־λ
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
//					switch(data_receive[7])			//�һ����Ʊ�־λ
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
  	*@brief 		�Զ�ִ��ģʽ״̬ת��
  	*@param		  void
	*@return		  void
*/
void autoModeSwitch(void)
{
	
//	switch( data_receive[7] )
//	{
//		case 1:
//			g_Flag.auto_mode = LARGE_ISLAND_MINE;			//����Դ��ȡ��
//			break;
//		
//		case 2:
//			g_Flag.auto_mode = MINE_MIDAIR;						//�սӿ�ʯ
//			break;
//		
//		case 3:
//			g_Flag.auto_mode = SMALL_ISLAND_MINE;			//����Դ��ȡ��
//			break;
	
//		default:
//			g_Flag.auto_mode = 0;
//			break;
//	}
	

}


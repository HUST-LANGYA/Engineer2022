/**
	*@brief 		�Զ�ִ������
							�����Զ�ִ��
	*@author		ף��ΰ
	*@date			2021-12-09
*/
#include "AutoexecTask.h"

uint32_t  Autoexec_high_water;


//����Դ��ȡ��״̬����
		//�������ֽϳ�������ʱ��'large'����
FLAG_AUTOEXEC_ENUM auto_large_enum 				= AUTOEXEC_DEFAULT;			
FLAG_AUTOEXEC_ENUM auto_large_enum_next 	= AUTOEXEC_DEFAULT;

//�սӿ�ʯ׼��״̬����
		//����ʱ��'midair'
FLAG_AUTOEXEC_ENUM auto_midair_enum				= AUTOEXEC_DEFAULT;
FLAG_AUTOEXEC_ENUM auto_midair_enum_next	= AUTOEXEC_DEFAULT;

//�սӿ�ʯ��ȡ״̬����
		//����ʱ��'midair'
FLAG_AUTOEXEC_ENUM auto_get_midair_enum				= AUTOEXEC_DEFAULT;
FLAG_AUTOEXEC_ENUM auto_get_midair_enum_next	= AUTOEXEC_DEFAULT;

//С��Դ��ȡ��״̬����
		//�������ֽϳ�������ʱ��'small'����
FLAG_AUTOEXEC_ENUM auto_small_enum 				= AUTOEXEC_DEFAULT;			
FLAG_AUTOEXEC_ENUM auto_small_enum_next 	= AUTOEXEC_DEFAULT;

//�Զ��һ�״̬����
		//�������ֽϳ�������ʱ��'exchange'����
FLAG_AUTOEXEC_ENUM auto_exchange_enum 			= AUTOEXEC_DEFAULT;			
FLAG_AUTOEXEC_ENUM auto_exchange_enum_next 	= AUTOEXEC_DEFAULT;


//����Ϊ��ʱ��Ҫ�õ��ı���
u8 auto_delay_flag = 0;						//��ʱ��־λ��1��ʾ������ʱ
u8 auto_delay_en_flag = 1;				//��ʱʹ�ܱ�־λ��ֻ�е���1ʱ������ʱ//ÿ����ʱ��ɸ�1
u32 auto_delay_length = 0;				//��ʱ��ʱ�䳤��
u32 auto_time_pre;								//��ʱ��ʼʱ��
u32 auto_time_now;								//��ǰʱ��


/**
  	*@brief 		�Զ�ִ��ģʽר��״̬����ʱ����
				(û�з�װ������ֱ����ֲ��ֻ��Ϊ�˷���ʹ��)
  	*@param		  void
	*@return		  void
*/
void autoModeDelay_ms(u32 t)
{
	if(auto_delay_en_flag)
	{
		auto_delay_en_flag 	= 0;
		auto_delay_length	 	= t;
		auto_time_pre				= xTaskGetTickCount();
		auto_delay_flag		 	= 1;
	}
}


/**
  	*@brief 		�Զ�ִ��������
  	*@param		  void
	*@return		  void
*/
void autoexecTask(void)
{
	while(1){
		
		autoexec();
		
		vTaskDelay(1);
		#if INCLUDE_uxTaskGetStackHighWaterMark
				Autoexec_high_water = uxTaskGetStackHighWaterMark(NULL);
		#endif
  }
}

/**
  	*@brief 		�Զ�ִ�к���
  	*@param		  void
	*@return		  void
*/
void autoexec(void)
{
	
/*************************�ж�״̬����ʱ�Ƿ�ﵽ**********************/
	if(auto_delay_flag)
	{
		auto_time_now				= xTaskGetTickCount();
		
		//��������ʱ����
		u32 t_length = 0;
		if(auto_time_now < auto_time_pre)
				t_length = auto_time_now - auto_time_pre + 4294967296;		//��ֹ�������Ȼ������
		else 	t_length = auto_time_now - auto_time_pre;
		
		//ʱ�䵽����ʱ����
		if(t_length >= auto_delay_length)
		{
			auto_small_enum = auto_small_enum_next;
			auto_exchange_enum = auto_exchange_enum_next;
			auto_large_enum = auto_large_enum_next;
			auto_midair_enum = auto_midair_enum_next;
			auto_get_midair_enum = auto_get_midair_enum_next;
			
			auto_delay_en_flag 	= 1;
			auto_delay_flag		 	= 0;
		}
	}
	
/*************************״̬�ı�ʱ��Ҫִ�е��޸�**********************/
	if(g_Flag.auto_mode_pre != g_Flag.auto_mode)
	{
		auto_large_enum 		 		= AUTOEXEC_DEFAULT;
		auto_large_enum_next 		= AUTOEXEC_DEFAULT;
		
		auto_midair_enum				= AUTOEXEC_DEFAULT;
		auto_midair_enum_next		= AUTOEXEC_DEFAULT;
		
		auto_get_midair_enum				= AUTOEXEC_DEFAULT;
		auto_get_midair_enum_next		= AUTOEXEC_DEFAULT;
		
		auto_small_enum 		 		= AUTOEXEC_DEFAULT;
		auto_small_enum_next 		= AUTOEXEC_DEFAULT;
		
		auto_exchange_enum 		 	= AUTOEXEC_DEFAULT;
		auto_exchange_enum_next	= AUTOEXEC_DEFAULT;
	}
	
	g_Flag.auto_mode_pre = g_Flag.auto_mode;

/********************************״̬��**********************************/
	switch( g_Flag.auto_mode )
	{
		case LARGE_ISLAND_MINE:					//����Դ��ȡ��
			autoLargeIslandMine();
			break;
		
		case SMALL_ISLAND_MINE:					//С��Դ��ȡ��
			autoSmallIslandMine();
			break;
		
		case MINE_MIDAIR:								//�սӿ�ʯ
			autoMineMidair();
			break;
		
		case GET_MINE_MIDAIR:						//�սӽ�ȡ��ʯ
			autoGetMineMidair();
			break;
		
		case EXCHANGE_MINE:							//�һ���ʯ
			autoExchange();
			break;
		
		default:
			break;
	}
	

}

/**
  	*@brief 		�Զ�ȡ����Դ����ʯ
  	*@param		  void
	*@return		  void
*/
void autoLargeIslandMine(void)
{
	
//		SL_LIFT_ONCE 	= 11,					//һ��̧��
//		SL_FORWARD 		= 12,					//ǰ��״̬
//		SL_CLAMP   		= 13,					//��ȡ��ʯ
//		SL_LIFT_TWICE	= 14,					//����̧��
//		SL_BACK				= 15,					//����״̬
//		SL_LAND_ONCE	= 16,					//�����˽���
//		SL_LAND_TWICE	= 17,					//����̧������
//		SL_LOOSE			= 18,					//�ͷſ�ʯ

/******************************************ȷ����һ��״̬******************************************/
	switch(auto_large_enum)
	{
		case AUTOEXEC_DEFAULT:
			auto_large_enum_next = SL_CLAMP_PRE;
			auto_large_enum = auto_large_enum_next;
//			//ִ�в���
//			if()//���������жϱ�״̬��������������һ״̬
//			{
//				auto_large_enum = auto_large_enum_next;
//			}
			break;
		
		case SL_CLAMP_PRE:
			auto_large_enum_next = SL_LIFT_ONCE;
			g_Flag.clamp_solenoid_flag = 0;
			autoModeDelay_ms(300);
			break;
		
		case SL_LIFT_ONCE:
			auto_large_enum_next = SL_FORWARD;
			g_Flag.lift_once_flag = 2;
			autoModeDelay_ms(800);
			break;
		
		case SL_FORWARD:
			auto_large_enum_next = SL_CLAMP;
			g_Flag.forward_solenoid_flag = 1;
			autoModeDelay_ms(300);
			break;
		
		case SL_CLAMP:
			auto_large_enum_next = SL_LIFT_TWICE;
			g_Flag.clamp_solenoid_flag = 1;
			autoModeDelay_ms(300);
			break;

		case SL_LIFT_TWICE:
			auto_large_enum_next = SL_BACK;
			g_Flag.lift_twice_flag = 1;
			autoModeDelay_ms(800);
			break;
		
		case SL_BACK:
			auto_large_enum_next = SL_LAND_ONCE;
			g_Flag.forward_solenoid_flag = 0;
			autoModeDelay_ms(300);
			break;
		
		case SL_LAND_ONCE:
			auto_large_enum_next = SL_LAND_TWICE;
			g_Flag.lift_once_flag = 0;
			autoModeDelay_ms(800);
			break;
		
		case SL_LAND_TWICE:
			auto_large_enum_next = SL_LOOSE;
			g_Flag.lift_twice_flag = 0;
			autoModeDelay_ms(800);
			break;
		
		case SL_LOOSE:
			auto_large_enum_next = ATUOEXEC_END;
			g_Flag.clamp_solenoid_flag = 0;
			autoModeDelay_ms(300);
			break;
		
		case ATUOEXEC_END:
			auto_large_enum_next = AUTOEXEC_DEFAULT;
			g_Flag.auto_mode = 0;
			autoModeDelay_ms(100);
			break;
			
		default:
			auto_large_enum = AUTOEXEC_DEFAULT;
			auto_large_enum_next = AUTOEXEC_DEFAULT;
			break;
	}
	
	
/*****************************ִ����ض������������ж���������ǰ״̬��ֵΪ��һ״̬*****************************/
	
}

/**
  	*@brief 		�Զ�ȡС��Դ����ʯ
  	*@param		  void
	*@return		  void
*/
void autoSmallIslandMine(void)
{
/******************************************ȷ����һ��״̬******************************************/
	switch(auto_small_enum)
	{
		case AUTOEXEC_DEFAULT:
			auto_small_enum_next = SS_CLAMP_PRE;
			auto_small_enum = auto_small_enum_next;
//			//ִ�в���
//			if()//���������жϱ�״̬��������������һ״̬
//			{
//				auto_small_enum = auto_small_enum_next;
//			}
			break;
		
		case SS_CLAMP_PRE:
			auto_large_enum_next = SS_LIFT_TWICE;
			g_Flag.clamp_solenoid_flag = 0;
			autoModeDelay_ms(300);
			break;
		
		case SS_LIFT_TWICE:
			auto_small_enum_next = SS_LIFT_ONCE;
			g_Flag.lift_twice_flag = 1;
			autoModeDelay_ms(800);
			break;

		
		case SS_LIFT_ONCE:
			auto_small_enum_next = SS_FORWARD;
			g_Flag.lift_once_flag = 1;
			autoModeDelay_ms(800);
//			vTaskDelay(2000);
//			auto_small_enum = auto_small_enum_next;
			break;
		
		case SS_FORWARD:
			auto_small_enum_next = SS_CLAMP;
			g_Flag.forward_solenoid_flag = 1;
			autoModeDelay_ms(300);
//			vTaskDelay(2000);
//			auto_small_enum = auto_small_enum_next;
			break;
		
		case SS_CLAMP:
			auto_small_enum_next = SS_GET;
			g_Flag.clamp_solenoid_flag = 1;
			autoModeDelay_ms(300);
			break;
		
		case SS_GET:
			auto_small_enum_next = SS_LAND_ONCE;
			g_Flag.midair_solenoid_flag = 1;
			autoModeDelay_ms(300);
			break;
		
		case SS_LAND_ONCE:
			auto_small_enum_next = SS_BACK;
			g_Flag.lift_once_flag = 0;
			autoModeDelay_ms(800);
			break;
		
		case SS_BACK:
			auto_small_enum_next = SS_MID_OFF;
			g_Flag.forward_solenoid_flag = 0;
			autoModeDelay_ms(300);
			break;
		
		case SS_MID_OFF:
			auto_small_enum_next = SS_LAND_TWICE;
			g_Flag.midair_solenoid_flag = 0;
			autoModeDelay_ms(300);
			break;
		
		case SS_LAND_TWICE:
			auto_small_enum_next = SS_LOOSE;
			g_Flag.lift_twice_flag = 0;
			autoModeDelay_ms(800);
			break;
			
		case SS_LOOSE:
			auto_small_enum_next = ATUOEXEC_END;
			g_Flag.clamp_solenoid_flag = 0;
			autoModeDelay_ms(300);
			break;
		
		case ATUOEXEC_END:
			auto_large_enum_next = AUTOEXEC_DEFAULT;
			g_Flag.auto_mode = 0;
			autoModeDelay_ms(100);
			break;
			
		default:
			auto_small_enum = AUTOEXEC_DEFAULT;
			auto_small_enum_next = AUTOEXEC_DEFAULT;
			break;
	}
	
	
/*****************************ִ����ض������������ж���������ǰ״̬��ֵΪ��һ״̬*****************************/
	
}

/**
  	*@brief 		�Զ��һ���ʯ
  	*@param		  void
	*@return		  void
*/
void autoExchange(void)
{
/******************************************ȷ����һ��״̬******************************************/
	switch(auto_exchange_enum)
	{
		case AUTOEXEC_DEFAULT:
			auto_exchange_enum_next = SE_CLAMP;
			auto_exchange_enum = auto_exchange_enum_next;
//			//ִ�в���
//			if()//���������жϱ�״̬��������������һ״̬
//			{
//				auto_exchange_enum = auto_exchange_enum_next;
//			}
			break;
		
		case SE_CLAMP:
			auto_exchange_enum_next = SE_LIFT_TWICE;
			g_Flag.clamp_solenoid_flag = 1;
			autoModeDelay_ms(300);
			break;

		
		case SE_LIFT_TWICE:
			auto_exchange_enum_next = SE_LIFT_ONCE;
			g_Flag.lift_twice_flag = 2;
			autoModeDelay_ms(800);
//			vTaskDelay(2000);
//			auto_exchange_enum = auto_exchange_enum_next;
			break;
		
		case SE_LIFT_ONCE:
			auto_exchange_enum_next = SE_FORWARD;
			g_Flag.lift_once_flag = 1;
			autoModeDelay_ms(800);
//			vTaskDelay(2000);
//			auto_exchange_enum = auto_exchange_enum_next;
			break;
		
		case SE_FORWARD:
			auto_exchange_enum_next = SE_LOOSE;
			g_Flag.forward_solenoid_flag = 1;
			autoModeDelay_ms(300);
			break;
		
		case SE_LOOSE:
			auto_exchange_enum_next = SE_EXCHANGE;
			g_Flag.clamp_solenoid_flag = 0;
			autoModeDelay_ms(300);
			break;
		
		
		case SE_EXCHANGE:
			auto_exchange_enum_next = SE_BACK;
			g_Flag.exchange_solenoid_flag = 1;
			autoModeDelay_ms(300);
			break;
		
		case SE_BACK:
			auto_exchange_enum_next = SE_LAND_ONCE;		
			g_Flag.exchange_solenoid_flag = 0;
			g_Flag.forward_solenoid_flag = 0;
			autoModeDelay_ms(300);
			break;
		
		case SE_LAND_ONCE:
			auto_exchange_enum_next = SE_LAND_TWICE;
			g_Flag.lift_once_flag = 0;
			autoModeDelay_ms(800);
			break;
		
		case SE_LAND_TWICE:
			auto_exchange_enum_next = ATUOEXEC_END;
			g_Flag.lift_twice_flag = 0;
			autoModeDelay_ms(800);
			break;
		
		case ATUOEXEC_END:
			auto_large_enum_next = AUTOEXEC_DEFAULT;
			g_Flag.auto_mode = 0;
			autoModeDelay_ms(100);
			break;
			
		default:
			auto_exchange_enum = AUTOEXEC_DEFAULT;
			auto_exchange_enum_next = AUTOEXEC_DEFAULT;
			break;
	}
	
	
/*****************************ִ����ض������������ж���������ǰ״̬��ֵΪ��һ״̬*****************************/
	
}



/**
  	*@brief 		�Զ�׼���ÿսӶ���
  	*@param		  void
	*@return		  void
*/
void autoMineMidair(void)
{
/******************************************ȷ����һ��״̬******************************************/
	//����K210ʶ����ʱû�У����״̬��������ʱδ����SM_ALIGN�Զ�������һ��
	switch(auto_midair_enum)
	{
		case AUTOEXEC_DEFAULT:
			auto_midair_enum_next = SM_CLAMP_PRE;
			auto_midair_enum = auto_midair_enum_next;
//			//ִ�в���
//			if()//���������жϱ�״̬��������������һ״̬
//			{
//				auto_midair_enum = auto_midair_enum_next;
//			}
			break;
		
		case SM_CLAMP_PRE:
			auto_midair_enum_next = SM_LIFT_TWICE;
			g_Flag.clamp_solenoid_flag = 0;
			autoModeDelay_ms(100);
			break;
		
		case SM_LIFT_TWICE:
			auto_midair_enum_next = SM_LIFT_ONCE;
			g_Flag.lift_twice_flag = 1;
			autoModeDelay_ms(800);
			break;
		
		case SM_LIFT_ONCE:
			auto_midair_enum_next = SM_FORWARD;
			g_Flag.lift_once_flag = 2;
			autoModeDelay_ms(800);
			break;

		case SM_FORWARD:
			auto_midair_enum_next = SM_MID_ON;
			g_Flag.forward_solenoid_flag = 1;
			autoModeDelay_ms(300);
			break;
		
		case SM_MID_ON:
			auto_midair_enum_next = ATUOEXEC_END;
			g_Flag.midair_solenoid_flag = 1;
			autoModeDelay_ms(300);
			break;
		
		case ATUOEXEC_END:
			auto_large_enum_next = AUTOEXEC_DEFAULT;
			g_Flag.auto_mode = GET_MINE_MIDAIR;
//			autoModeDelay_ms(100);
			break;
		
		default:
			auto_midair_enum = AUTOEXEC_DEFAULT;
			auto_midair_enum_next = AUTOEXEC_DEFAULT;
			break;
	}
	
}


/**
  	*@brief 		�սӽ�ȡ��ʯ
  	*@param		  void
	*@return		  void
*/
void autoGetMineMidair(void)
{
/******************************************ȷ����һ��״̬******************************************/
	//����K210ʶ����ʱû�У����״̬��������ʱδ����SM_ALIGN�Զ�������һ��
	switch(auto_get_midair_enum)
	{
		case AUTOEXEC_DEFAULT:
			auto_get_midair_enum_next = SM_MID_WAIT;
			auto_get_midair_enum = auto_get_midair_enum_next;
//			//ִ�в���
//			if()//���������жϱ�״̬��������������һ״̬
//			{
//				auto_get_midair_enum = auto_get_midair_enum_next;
//			}
			break;
		
		case SM_MID_WAIT:
			auto_get_midair_enum_next = SM_CLAMP;
			if(g_Flag.photogate_flag == 1)
							autoModeDelay_ms(25);
			break;
		
		case SM_CLAMP:
			auto_get_midair_enum_next = SM_BACK;
			g_Flag.clamp_solenoid_flag = 1;
			autoModeDelay_ms(300);
			break;
		
		case SM_BACK:
			auto_get_midair_enum_next = SM_LAND_ONCE;
			g_Flag.forward_solenoid_flag = 0;
			autoModeDelay_ms(300);
			break;

		case SM_LAND_ONCE:
			auto_get_midair_enum_next = SM_MID_OFF;
			g_Flag.lift_once_flag = 0;
			autoModeDelay_ms(800);
			break;
		
		case SM_MID_OFF:
			auto_get_midair_enum_next = SM_LAND_TWICE;
			g_Flag.midair_solenoid_flag = 0;
			autoModeDelay_ms(300);
			break;

		case SM_LAND_TWICE:
			auto_get_midair_enum_next = SM_LOOSE;
			g_Flag.lift_twice_flag = 0;
			autoModeDelay_ms(800);
			break;
		
		case SM_LOOSE:
			auto_get_midair_enum_next = ATUOEXEC_END;
			g_Flag.clamp_solenoid_flag = 0;
			autoModeDelay_ms(300);
			break;
		
		case ATUOEXEC_END:
			auto_get_midair_enum_next = AUTOEXEC_DEFAULT;
			g_Flag.auto_mode = 0;
			autoModeDelay_ms(100);
			break;
		
		default:
			auto_get_midair_enum = AUTOEXEC_DEFAULT;
			auto_get_midair_enum_next = AUTOEXEC_DEFAULT;
			break;
	}
	
}



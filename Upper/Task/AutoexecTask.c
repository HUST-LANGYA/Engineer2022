/**
	*@brief 		自动执行任务
							用于自动执行
	*@author		祝科伟
	*@date			2021-12-09
*/
#include "AutoexecTask.h"

uint32_t  Autoexec_high_water;

FLAG_AUTOEXEC_ENUM auto_state_enum				= AUTOEXEC_DEFAULT;
FLAG_AUTOEXEC_ENUM auto_state_enum_next		= AUTOEXEC_DEFAULT;

//以下为延时需要用到的变量
u8 auto_delay_flag = 0;						//延时标志位，1表示正在延时
u8 auto_delay_en_flag = 1;				//延时使能标志位，只有等于1时才能延时//每次延时完成赋1
u32 auto_delay_length = 0;				//延时的时间长短
u32 auto_time_pre;								//延时初始时间
u32 auto_time_now;								//当前时间


/**
  	*@brief 		自动执行模式专属状态机延时函数
				(没有封装，不能直接移植，只是为了方便使用)
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
  	*@brief 		自动执行任务函数
  	*@param		  void
	*@return		  void
*/
void autoexecTask(void)
{
	while(1){
		
		autoexec();
		
		vTaskDelay(1);
//		#if INCLUDE_uxTaskGetStackHighWaterMark
//				Autoexec_high_water = uxTaskGetStackHighWaterMark(NULL);
//		#endif
  }
}

/**
  	*@brief 		自动执行函数
  	*@param		  void
	*@return		  void
*/
void autoexec(void)
{
	
/*************************判断状态机延时是否达到**********************/
	if(auto_delay_flag)
	{
		auto_time_now				= xTaskGetTickCount();
		
		//计算已延时长度
		u32 t_length = 0;
		if(auto_time_now < auto_time_pre)
				t_length = auto_time_now - auto_time_pre + 4294967296;		//防止溢出，虽然不可能
		else 	t_length = auto_time_now - auto_time_pre;
		
		//时间到了延时结束
		if(t_length >= auto_delay_length)
		{
			auto_state_enum = auto_state_enum_next;

			auto_delay_en_flag 	= 1;
			auto_delay_flag		 	= 0;
		}
	}
	
/*************************状态改变时需要执行的修改**********************/
	if(g_Flag.auto_mode_pre != g_Flag.auto_mode)
	{
		auto_state_enum 		 			= AUTOEXEC_DEFAULT;
		auto_state_enum_next 			= AUTOEXEC_DEFAULT;
	}
	
	g_Flag.auto_mode_pre = g_Flag.auto_mode;

/********************************状态机**********************************/
	switch( g_Flag.auto_mode )
	{
		case LARGE_ISLAND_MINE:					//大资源岛取矿
			autoLargeIslandMine();
			break;
		
		case SMALL_ISLAND_MINE:					//小资源岛取矿
			autoSmallIslandMine();
			break;
		
		case MINE_MIDAIR:								//空接矿石
			autoMineMidair();
			break;

//		case GET_MINE_MIDAIR:						//空接接取矿石
//			autoGetMineMidair();
//			break;
		
		case EXCHANGE_MINE:							//兑换矿石
			autoExchange();
			break;
		
		case EXCHANGE_MINE_TWICE:				//二次兑换矿石
			autoExchangeTwice();
			break;
		
		case RESET_SOFTWARE:						//软件自动复位
			autoResetSoftware();
			break;
		
		case LASER_ALIGNING_MID_PRE:						//软件自动复位
			autoResetSoftware();
			break;
		
		default:
			break;
	}
	

}

/**
  	*@brief 		自动取大资源岛矿石
  	*@param		  void
	*@return		  void
*/
void autoLargeIslandMine(void)
{
/******************************************确定下一个状态******************************************/
	switch(auto_state_enum)
	{
		case AUTOEXEC_DEFAULT:
			auto_state_enum_next = SL_CLAMP_PRE;
			auto_state_enum = auto_state_enum_next;
//			//执行操作
//			if()//根据条件判断本状态结束，并进入下一状态
//			{
//				auto_state_enum = auto_state_enum_next;
//			}
			break;
		
		case SL_CLAMP_PRE:
			auto_state_enum_next = SL_LIFT_ONCE;
			if(g_Flag.clamp_solenoid_flag != 0)
			{
				g_Flag.clamp_solenoid_flag = 0;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SL_LIFT_ONCE:
			auto_state_enum_next = SL_FORWARD;
			if(g_Flag.lift_once_flag != 2)
			{
				g_Flag.lift_once_flag = 2;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SL_FORWARD:
			auto_state_enum_next = SL_CLAMP;
			if(g_Flag.forward_solenoid_flag != 1)
			{
				g_Flag.forward_solenoid_flag = 1;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SL_CLAMP:
			auto_state_enum_next = SL_LIFT_TWICE;
			if(g_Flag.clamp_solenoid_flag != 1)
			{
				g_Flag.clamp_solenoid_flag = 1;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;

		case SL_LIFT_TWICE:
			auto_state_enum_next = SL_BACK;
			if(g_Flag.lift_twice_flag != 3)
			{
				g_Flag.lift_twice_flag = 3;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SL_BACK:
			auto_state_enum_next = SL_LAND_ONCE;
			if(g_Flag.forward_solenoid_flag != 0)
			{
				g_Flag.forward_solenoid_flag = 0;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SL_LAND_ONCE:
			auto_state_enum_next = ATUOEXEC_END;
			if(g_Flag.lift_once_flag != 0)
			{
				g_Flag.lift_once_flag = 0;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;
		
//		case SL_LAND_TWICE:
//			auto_state_enum_next = SL_LOOSE;
//			if(g_Flag.lift_twice_flag != 0)
//			{
//				g_Flag.lift_twice_flag = 0;
//				autoModeDelay_ms(800);
//			}else
//				autoModeDelay_ms(50);
//			break;
		
//		case SL_LOOSE:
//			auto_state_enum_next = ATUOEXEC_END;
////			if(g_Flag.clamp_solenoid_flag != 0)
////			{
////				g_Flag.clamp_solenoid_flag = 0;
////				autoModeDelay_ms(300);
////			}else
////				autoModeDelay_ms(50);
//			g_Flag.clamp_solenoid_flag = 0;
//			autoModeDelay_ms(300);
//			break;
		
		case ATUOEXEC_END:
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			g_Flag.auto_end_flag = 1;
			autoModeDelay_ms(100);
			break;
			
		default:
			auto_state_enum = AUTOEXEC_DEFAULT;
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			break;
	}
	
	
/*****************************执行相关动作，并根据判断条件将当前状态赋值为下一状态*****************************/
	
}

/**
  	*@brief 		自动取小资源岛矿石
  	*@param		  void
	*@return		  void
*/
void autoSmallIslandMine(void)
{
/******************************************确定下一个状态******************************************/
	switch(auto_state_enum)
	{
		case AUTOEXEC_DEFAULT:
			auto_state_enum_next = SS_CLAMP_PRE;
			auto_state_enum = auto_state_enum_next;
//			//执行操作
//			if()//根据条件判断本状态结束，并进入下一状态
//			{
//				auto_state_enum = auto_state_enum_next;
//			}
			break;
		
		case SS_CLAMP_PRE:
			auto_state_enum_next = SS_LIFT_TWICE;
			if(g_Flag.clamp_solenoid_flag != 0)
			{
				g_Flag.clamp_solenoid_flag = 0;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SS_LIFT_TWICE:
			auto_state_enum_next = SS_LIFT_ONCE;
			if(g_Flag.lift_twice_flag != 1)
			{
				g_Flag.lift_twice_flag = 1;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;

		
		case SS_LIFT_ONCE:
			auto_state_enum_next = SS_FORWARD;
			if(g_Flag.lift_once_flag != 1)
			{
					g_Flag.lift_once_flag = 1;
					autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SS_FORWARD:
			auto_state_enum_next = SS_CLAMP;
			if(g_Flag.forward_solenoid_flag != 1)
			{
				g_Flag.forward_solenoid_flag = 1;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SS_CLAMP:
			auto_state_enum_next = SS_GET;
			if(g_Flag.clamp_solenoid_flag != 1)
			{
				g_Flag.clamp_solenoid_flag = 1;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SS_GET:
			auto_state_enum_next = SS_LAND_ONCE;
			if(g_Flag.midair_solenoid_flag != 1)
			{
				g_Flag.midair_solenoid_flag = 1;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SS_LAND_ONCE:
			auto_state_enum_next = SS_BACK;
			if(g_Flag.lift_once_flag != 0)
			{
				g_Flag.lift_once_flag = 0;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SS_BACK:
			auto_state_enum_next = SS_MID_OFF;
			if(g_Flag.forward_solenoid_flag != 0)
			{
				g_Flag.forward_solenoid_flag = 0;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SS_MID_OFF:
			auto_state_enum_next = ATUOEXEC_END;
			if(g_Flag.midair_solenoid_flag != 0)
			{
				g_Flag.midair_solenoid_flag = 0;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
//		case SS_LAND_TWICE:
//			auto_state_enum_next = SS_LOOSE;
//			if(g_Flag.lift_twice_flag != 0)
//			{
//				g_Flag.lift_twice_flag = 0;
//				autoModeDelay_ms(800);
//			}else
//				autoModeDelay_ms(50);
//			break;
//			
//		case SS_LOOSE:
//			auto_state_enum_next = ATUOEXEC_END;
//			if(g_Flag.clamp_solenoid_flag != 0)
//			{
//				g_Flag.clamp_solenoid_flag = 0;
//				autoModeDelay_ms(300);
//			}else
//				autoModeDelay_ms(50);
//			break;
		
		case ATUOEXEC_END:
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			g_Flag.auto_end_flag = 1;
			autoModeDelay_ms(100);
			break;
			
		default:
			auto_state_enum = AUTOEXEC_DEFAULT;
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			break;
	}
	
	
/*****************************执行相关动作，并根据判断条件将当前状态赋值为下一状态*****************************/
	
}

/**
  	*@brief 		自动兑换矿石
  	*@param		  void
	*@return		  void
*/
void autoExchange(void)
{
/******************************************确定下一个状态******************************************/
	switch(auto_state_enum)
	{
		case AUTOEXEC_DEFAULT:
			auto_state_enum_next = SE_CLAMP;
			auto_state_enum = auto_state_enum_next;
//			//执行操作
//			if()//根据条件判断本状态结束，并进入下一状态
//			{
//				auto_state_enum = auto_state_enum_next;
//			}
			break;
		
		case SE_CLAMP:
			auto_state_enum_next = SE_LIFT_ONCE;
			if(g_Flag.clamp_solenoid_flag != 1)
			{
				g_Flag.clamp_solenoid_flag = 1;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SE_LIFT_ONCE:
			auto_state_enum_next = SE_LIFT_TWICE;
			if(g_Flag.lift_once_flag != 1)
			{
				g_Flag.lift_once_flag = 1;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;

		
		case SE_LIFT_TWICE:
			auto_state_enum_next = SE_FORWARD;
			if(g_Flag.lift_twice_flag != 2)
			{
				g_Flag.lift_twice_flag = 2;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SE_FORWARD:
			auto_state_enum_next = SE_EXCHANGE_ON;
			if(g_Flag.forward_solenoid_flag != 1)
			{
				g_Flag.forward_solenoid_flag = 1;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
			
			//测试
		case SE_EXCHANGE_ON:
			auto_state_enum_next = SE_LOOSE;
			if(g_Flag.exchange_solenoid_flag != 1)
			{
				g_Flag.exchange_solenoid_flag = 1;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
			
		case SE_LOOSE:
			auto_state_enum_next = SE_EXCHANGE_OFF;
			if(g_Flag.clamp_solenoid_flag != 0)
			{
				g_Flag.clamp_solenoid_flag = 0;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
			
		case SE_EXCHANGE_OFF:
			auto_state_enum_next = ATUOEXEC_END;
			if(g_Flag.exchange_solenoid_flag != 0)
			{
				g_Flag.exchange_solenoid_flag = 0;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
			
		case ATUOEXEC_END:
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			g_Flag.auto_end_flag = 1;
			autoModeDelay_ms(100);
			break;
		
			//测试
//			
//		case ATUOEXEC_END:
//			auto_state_enum_next = AUTOEXEC_DEFAULT;
//			g_Flag.auto_end_flag = 1;
//			autoModeDelay_ms(100);
//			break;
			
		default:
			auto_state_enum = AUTOEXEC_DEFAULT;
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			break;
	}
	
	
/*****************************执行相关动作，并根据判断条件将当前状态赋值为下一状态*****************************/
	
}

/**
  	*@brief 		二次兑换矿石
  	*@param		  void
	*@return		  void
*/
void autoExchangeTwice(void)
{
/******************************************确定下一个状态******************************************/
	switch(auto_state_enum)
	{
		case AUTOEXEC_DEFAULT:
			auto_state_enum_next = SE_LOOSE;
			auto_state_enum = auto_state_enum_next;
//			//执行操作
//			if()//根据条件判断本状态结束，并进入下一状态
//			{
//				auto_state_enum = auto_state_enum_next;
//			}
			break;
		
		case SE_LOOSE:
			auto_state_enum_next = SE_FORWARD_BACK_2;
			if(g_Flag.clamp_solenoid_flag != 0)
			{
					g_Flag.clamp_solenoid_flag = 0;
					autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SE_FORWARD_BACK_2:
			auto_state_enum_next = SE_LIFT_ONCE_2;
			if(g_Flag.forward_solenoid_flag != 0)
			{
					g_Flag.forward_solenoid_flag = 0;
					autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
			
			
		case SE_LIFT_ONCE_2:
			auto_state_enum_next = SE_CLAMP_2;
			if(g_Flag.lift_once_flag != 3)
			{
				g_Flag.lift_once_flag = 3;
				autoModeDelay_ms(400);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SE_CLAMP_2:
			auto_state_enum_next = SE_FORWARD_2;
			if(g_Flag.clamp_solenoid_flag != 1)
			{
					g_Flag.clamp_solenoid_flag = 1;
					autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SE_FORWARD_2:
			auto_state_enum_next = ATUOEXEC_END;
			if(g_Flag.forward_solenoid_flag != 1)
			{
				g_Flag.forward_solenoid_flag = 1;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
//		
//		case SE_BACK:
//			auto_state_enum_next = SE_LAND_ONCE;		
//			if(g_Flag.exchange_solenoid_flag != 0 || g_Flag.forward_solenoid_flag != 0)
//			{
//				g_Flag.exchange_solenoid_flag = 0;
//				g_Flag.forward_solenoid_flag = 0;
//				autoModeDelay_ms(300);
//			}else
//				autoModeDelay_ms(50);
//			break;
//		
//		case SE_LAND_ONCE:
//			auto_state_enum_next = SE_LAND_TWICE;
//			if(g_Flag.lift_once_flag != 0)
//			{
//				g_Flag.lift_once_flag = 0;
//				autoModeDelay_ms(800);
//			}else
//				autoModeDelay_ms(50);
//			break;
//		
//		case SE_LAND_TWICE:
//			auto_state_enum_next = SE_LOOSE_2;
//			if(g_Flag.lift_twice_flag != 0)
//			{
//				g_Flag.lift_twice_flag = 0;
//				autoModeDelay_ms(800);
//			}else
//				autoModeDelay_ms(50);
//			break;
//		
//		case SE_LOOSE_2:
//			auto_state_enum_next = ATUOEXEC_END;
//			if(g_Flag.clamp_solenoid_flag != 0)
//			{
//				g_Flag.clamp_solenoid_flag = 0;
//				autoModeDelay_ms(300);
//			}else
//				autoModeDelay_ms(50);
//			break;
		
		case ATUOEXEC_END:
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			g_Flag.auto_end_flag = 1;
			autoModeDelay_ms(100);
			break;
			
		default:
			auto_state_enum = AUTOEXEC_DEFAULT;
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			break;
	}
	
	
/*****************************执行相关动作，并根据判断条件将当前状态赋值为下一状态*****************************/
	
}



/**
  	*@brief 		自动准备好空接动作
  	*@param		  void
	*@return		  void
*/
void autoMineMidair(void)
{
/******************************************确定下一个状态******************************************/
	//由于K210识别暂时没有，因此状态机里面暂时未加上SM_ALIGN自动对齐这一步
	switch(auto_state_enum)
	{
		case AUTOEXEC_DEFAULT:
			auto_state_enum_next = SM_CLAMP_PRE;
			auto_state_enum = auto_state_enum_next;
//			//执行操作
//			if()//根据条件判断本状态结束，并进入下一状态
//			{
//				auto_state_enum = auto_state_enum_next;
//			}
			break;
		
		case SM_CLAMP_PRE:
			auto_state_enum_next = SM_LIFT_ONCE;
			if(g_Flag.clamp_solenoid_flag != 0)
			{
				g_Flag.clamp_solenoid_flag = 0;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SM_LIFT_ONCE:
			auto_state_enum_next = SM_FORWARD;
			if(g_Flag.lift_once_flag != 2)
			{
				g_Flag.lift_once_flag = 2;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;

		case SM_FORWARD:
			auto_state_enum_next = SM_LIFT_TWICE;
			if(g_Flag.forward_solenoid_flag != 1)
			{
				g_Flag.forward_solenoid_flag = 1;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SM_LIFT_TWICE:
			auto_state_enum_next = SM_MID_ON;
			if(g_Flag.lift_twice_flag != 3)
			{
				g_Flag.lift_twice_flag = 3;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SM_MID_ON:
			auto_state_enum_next = SM_MID_WAIT;
			if(g_Flag.midair_solenoid_flag != 1)
			{
				g_Flag.midair_solenoid_flag = 1;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
			
		
		case SM_MID_WAIT:
			auto_state_enum_next = SM_CLAMP;
			if(g_Flag.photogate_flag == 1)
							autoModeDelay_ms(125);
			break;
		
		case SM_CLAMP:
			auto_state_enum_next = SM_BACK;
			if(g_Flag.clamp_solenoid_flag != 1)
			{
				g_Flag.clamp_solenoid_flag = 1;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SM_BACK:
			auto_state_enum_next = SM_LAND_ONCE;
			if(g_Flag.forward_solenoid_flag != 0)
			{
				g_Flag.forward_solenoid_flag = 0;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;

		case SM_LAND_ONCE:
			auto_state_enum_next = SM_MID_OFF;
			if(g_Flag.lift_once_flag != 0)
			{
				g_Flag.lift_once_flag = 0;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SM_MID_OFF:
			auto_state_enum_next = ATUOEXEC_END;
			if(g_Flag.midair_solenoid_flag != 0)
			{
				g_Flag.midair_solenoid_flag = 0;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case ATUOEXEC_END:
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			g_Flag.auto_end_flag = 1;				
			break;
		
		default:
			auto_state_enum = AUTOEXEC_DEFAULT;
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			break;
	}
}


///**
//  	*@brief 		空接接取矿石
//  	*@param		  void
//	*@return		  void
//*/
//void autoGetMineMidair(void)
//{
///******************************************确定下一个状态******************************************/
//	//由于K210识别暂时没有，因此状态机里面暂时未加上SM_ALIGN自动对齐这一步
//	switch(auto_state_enum)
//	{
//		case AUTOEXEC_DEFAULT:
//			auto_state_enum_next = SM_MID_WAIT;
//			auto_state_enum = auto_state_enum_next;
////			//执行操作
////			if()//根据条件判断本状态结束，并进入下一状态
////			{
////				auto_state_enum = auto_state_enum_next;
////			}
//			break;
//		
//		case SM_MID_WAIT:
//			auto_state_enum_next = SM_CLAMP;
//			if(g_Flag.photogate_flag == 1)
//							autoModeDelay_ms(125);
//			break;
//		
//		case SM_CLAMP:
//			auto_state_enum_next = SM_BACK;
//			if(g_Flag.clamp_solenoid_flag != 1)
//			{
//				g_Flag.clamp_solenoid_flag = 1;
//				autoModeDelay_ms(300);
//			}else
//				autoModeDelay_ms(50);
//			break;
//		
//		case SM_BACK:
//			auto_state_enum_next = SM_LAND_ONCE;
//			if(g_Flag.forward_solenoid_flag != 0)
//			{
//				g_Flag.forward_solenoid_flag = 0;
//				autoModeDelay_ms(300);
//			}else
//				autoModeDelay_ms(50);
//			break;

//		case SM_LAND_ONCE:
//			auto_state_enum_next = SM_MID_OFF;
//			if(g_Flag.lift_once_flag != 0)
//			{
//				g_Flag.lift_once_flag = 0;
//				autoModeDelay_ms(800);
//			}else
//				autoModeDelay_ms(50);
//			break;
//		
//		case SM_MID_OFF:
//			auto_state_enum_next = ATUOEXEC_END;
//			if(g_Flag.midair_solenoid_flag != 0)
//			{
//				g_Flag.midair_solenoid_flag = 0;
//				autoModeDelay_ms(300);
//			}else
//				autoModeDelay_ms(50);
//			break;

////		case SM_LAND_TWICE:
////			auto_state_enum_next = SM_LOOSE;
////			if(g_Flag.lift_twice_flag != 0)
////			{
////				g_Flag.lift_twice_flag = 0;
////				autoModeDelay_ms(800);
////			}else
////				autoModeDelay_ms(50);
////			break;
////		
////		case SM_LOOSE:
////			auto_state_enum_next = ATUOEXEC_END;
////			if(g_Flag.clamp_solenoid_flag != 0)
////			{
////				g_Flag.clamp_solenoid_flag = 0;
////				autoModeDelay_ms(300);
////			}else
////				autoModeDelay_ms(50);
////			break;
//		
//		case ATUOEXEC_END:
//			auto_state_enum_next = AUTOEXEC_DEFAULT;
//			g_Flag.auto_end_flag = 1;
//			autoModeDelay_ms(100);
//			break;
//		
//		default:
//			auto_state_enum = AUTOEXEC_DEFAULT;
//			auto_state_enum_next = AUTOEXEC_DEFAULT;
//			break;
//	}
//	
//}


/**
  	*@brief 		软件自动复位
  	*@param		  void
	*@return		  void
*/
void autoResetSoftware(void)
{
		switch(auto_state_enum)
	{
		case AUTOEXEC_DEFAULT:
			auto_state_enum_next = SR_FORWARD;
			auto_state_enum = auto_state_enum_next;
//			//执行操作
//			if()//根据条件判断本状态结束，并进入下一状态
//			{
//				auto_state_enum = auto_state_enum_next;
//			}
			break;
		
		case SR_FORWARD:
			auto_state_enum_next = SR_LIFT_ONCE;
			if(g_Flag.forward_solenoid_flag != 0)
			{
				g_Flag.forward_solenoid_flag = 0;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SR_LIFT_ONCE:
			auto_state_enum_next = SR_MID;
			if(g_Flag.lift_once_flag != 0)
			{
				g_Flag.lift_once_flag = 0;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;

		
		case SR_MID:
			auto_state_enum_next = SR_LIFT_TWICE;
			if(g_Flag.midair_solenoid_flag != 0)
			{
				g_Flag.midair_solenoid_flag = 0;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case SR_LIFT_TWICE:
			auto_state_enum_next = ATUOEXEC_END;
			if(g_Flag.lift_twice_flag != 0)
			{
				g_Flag.lift_twice_flag = 0;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;
		
		case ATUOEXEC_END:
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			g_Flag.auto_end_flag = 1;
			autoModeDelay_ms(100);
			break;
			
		default:
			auto_state_enum = AUTOEXEC_DEFAULT;
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			break;
	}
	
}

/**
  	*@brief 		激光对位空接动作准备
  	*@param		  void
	*@return		  void
*/
void autoLaserMidPre(void)
{
		switch(auto_state_enum)
	{
		case AUTOEXEC_DEFAULT:
			auto_state_enum_next = LP_LOOSE;
			auto_state_enum = auto_state_enum_next;
//			//执行操作
//			if()//根据条件判断本状态结束，并进入下一状态
//			{
//				auto_state_enum = auto_state_enum_next;
//			}
			break;
		
		case LP_LOOSE:
			auto_state_enum_next = LP_LIFT_ONCE;
			if(g_Flag.clamp_solenoid_flag != 0)
			{
				g_Flag.clamp_solenoid_flag = 0;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case LP_LIFT_ONCE:
			auto_state_enum_next = LP_FORWARD;
			if(g_Flag.lift_once_flag != 2)
			{
				g_Flag.lift_once_flag = 2;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;

		
		case LP_FORWARD:
			auto_state_enum_next = LP_CLAMP;
			if(g_Flag.forward_solenoid_flag != 1)
			{
				g_Flag.forward_solenoid_flag = 1;
				autoModeDelay_ms(300);
			}else
				autoModeDelay_ms(50);
			break;
		
		case LP_CLAMP:
			auto_state_enum_next = ATUOEXEC_END;
			if(g_Flag.clamp_solenoid_flag != 1)
			{
				g_Flag.clamp_solenoid_flag = 1;
				autoModeDelay_ms(800);
			}else
				autoModeDelay_ms(50);
			break;
		
		case ATUOEXEC_END:
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			g_Flag.auto_end_flag = 1;
			autoModeDelay_ms(100);
			break;
			
		default:
			auto_state_enum = AUTOEXEC_DEFAULT;
			auto_state_enum_next = AUTOEXEC_DEFAULT;
			break;
	}
}


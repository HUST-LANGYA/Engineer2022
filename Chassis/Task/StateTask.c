/**
	*@brief 		模式判断任务文件	
	*@author		祝科伟
	*@date			2021-12-19
*/
/************************************************************************************
模式判断
判断逻辑：主要分为两部分，第一部分是控制模式，分为遥控模式，键盘模式，掉电模式，

第二部分为控制目标，针对遥控模式时遥控的部分，比如底盘移动，底盘升降，上层升降，
上层伸缩，等等，遥控器状态有限，随时修改

s2（左）：在下{s1（右）在下：掉电模式
             s1（右）在中：键鼠模式
             s1（右）在上：遥控模式}
s2（左）：在中{s1（右）在下：底盘模式3
               s1（右）在中：底盘静步模式
               s1（右）在上：底盘普通模式}						 
s2（左）：在上{s1（右）在下：上层模式1
               s1（右）在中：上层模式2
               s1（右）在上：上层自动控制}
***************************************************************************************/
#include "StateTask.h"

uint32_t  Mode_Switch_high_water;


extern FlagWithSlave_t FlagOfSlave;		//接收到的C板标志位

extern short ctrl_rising_flag,shift_rising_flag,v_rising_flag,c_rising_flag,e_rising_flag,x_rising_flag,Press_Key_x_Flag,v_rising_flag;
extern short f_rising_flag,mouse_Press_r_rising_flag,r_rising_flag,z_rising_flag,Press_Key_z_Flag,g_rising_flag,q_rising_flag;
extern unsigned char Data_Receive_from_F103[8];//Data_Receive存的是接收到的从C板传来的数据


/**
  * @brief  模式转换任务，状态机
  * @param  None
  * @retval None
*/
void modeSwitchTask(void *pvParameters)
{
	
	portTickType xLastWakeTime;
	const portTickType xFrequency=1;
	
	while(1){
		xLastWakeTime=xTaskGetTickCount();
		
		switchMode();
		
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
		
		#if INCLUDE_uxTaskGetStackHighWaterMark
        Mode_Switch_high_water = uxTaskGetStackHighWaterMark(NULL);
		#endif
  }
}


/**
  * @brief  模式转换函数
  * @param  None
	* @retval 
*/
int test_q = 0;
int test11 = 1;

void switchMode(void)
{
/**************** 键鼠模式下按键状态判断 *****************/
	key_rising_check();
	key_refresh();
/*********************** 状态切换 ************************/
	controlStateGet();
	motionTargetGet();																	//获取运动模式
	
	/************************************ 遥控器模式修改标志位 **********************************************/
	/*倘若是上层抬升模式，对抬升的标志位进行判断*/
	if(g_Flag.control_mode == RC_MODE)
	{
		switch(g_Flag.control_target)
		{
			case CHASSIS_MODE3:
				if(rc_ctrl.rc.ch2 < 500) 							//左左移动拨杆
				{
					g_Flag.rescue_solenoid_flag = 1; //救援电磁阀
				}else if(rc_ctrl.rc.ch2 > 1500) 					//左右移动拨杆
				{
					g_Flag.rescue_solenoid_flag = 0; //救援电磁阀关闭
				}
				
				if(rc_ctrl.rc.ch0 < 500) 							//右左移动拨杆
				{
					g_Flag.resurge_solenoid_flag = 1; //复活电磁阀
				}else if(rc_ctrl.rc.ch0 > 1500) 					//右右移动拨杆
				{
					g_Flag.resurge_solenoid_flag = 0; //复活电磁阀关闭
				}
				break;
			
			case SENIOR_UP_MODE:
				if(rc_ctrl.rc.ch3 > 1500) 							//左上移动拨杆
				{
					g_Flag.lift_down_twice_flag = 1; //二级抬升
				}else if(rc_ctrl.rc.ch3 < 500) 					//左下移动拨杆
				{
					g_Flag.lift_down_twice_flag = 0; //二级下降
				}
				
				if(rc_ctrl.rc.ch2 < 500) 							//左左移动拨杆
				{
					g_Flag.midair_solenoid_flag = 1; //空接电磁阀
				}else if(rc_ctrl.rc.ch2 > 1500) 					//左右移动拨杆
				{
					g_Flag.midair_solenoid_flag = 0; //空接电磁阀关闭
				}
				
				if(rc_ctrl.rc.ch1 > 1500) 							//右上移动拨杆
				{
					g_Flag.lift_once_flag 	= 1; 	//一级抬升
//					g_Flag.forward_flag			= 1;	//前移
				}else if(rc_ctrl.rc.ch1 < 500) 					//右下移动拨杆
				{
					g_Flag.lift_once_flag = 0; 	//一级下降
//					g_Flag.forward_flag			= 0;	//前移收回
				}


				if(rc_ctrl.rc.ch0 < 500) 							//右左移动拨杆
				{
					g_Flag.clamp_flag = 1; //夹取
				}else if(rc_ctrl.rc.ch0 > 1500) 					//右右移动拨杆
				{
					g_Flag.clamp_flag = 0; //松开
				}
				break;
			
			case SENIOR_MODE2:
				if(rc_ctrl.rc.ch0 < 500) 							//右左移动拨杆
				{
					g_Flag.exchange_solenoid_flag = 1; //兑换电磁阀
				}else if(rc_ctrl.rc.ch0 > 1500) 					//右右移动拨杆
				{
					g_Flag.exchange_solenoid_flag = 0; //兑换电磁阀关闭
				}
				
				if(rc_ctrl.rc.ch1 > 1500) 							//右上移动拨杆
				{
					g_Flag.forward_flag			= 1;	//前移
				}else if(rc_ctrl.rc.ch1 < 500) 					//右下移动拨杆
				{
					g_Flag.forward_flag			= 0;	//前移收回
				}
				break;
				
			
			case SENIOR_AUTO_MODE:
				if(Data_Receive_from_F103[0]=='!')
				{
					g_Flag.clamp_flag = FlagOfSlave.flag.clamp_flag;
					g_Flag.exchange_solenoid_flag = FlagOfSlave.flag.exchange_solenoid_flag;
					g_Flag.forward_flag = FlagOfSlave.flag.forward_flag;
					g_Flag.lift_down_twice_flag = FlagOfSlave.flag.lift_down_twice_flag;
					g_Flag.lift_once_flag = FlagOfSlave.flag.lift_once_flag;
					g_Flag.midair_solenoid_flag = FlagOfSlave.flag.midair_solenoid_flag;
				}
				break;
			
			default:
				break;
		}
	}
	
	/************************************ 键鼠模式修改标志位 **********************************************/
	if(g_Flag.control_mode == KEY_MODE)
	{
		/*********************** q键上升沿控制一级抬升打开/关闭 **************************/
		if(q_rising_flag == 1)
		{
//			test_q ++;
			g_Flag.lift_once_flag = 1 - g_Flag.lift_once_flag;
		}
		
		/*********************** e键上升沿控制夹取电磁阀打开/关闭 **************************/
		if(e_rising_flag == 1)
		{
			g_Flag.clamp_flag = 1 - g_Flag.clamp_flag;
		}
		
		/*********************** r键上升沿控制二级抬升打开/关闭 **************************/
		if(r_rising_flag == 1)
		{
			g_Flag.lift_down_twice_flag = 1 - g_Flag.lift_down_twice_flag;
		}
		
	}

	
	if(g_Flag.rescue_solenoid_flag == 1) 			//是否开启救援模块
						RESCUE_SOLENOID_ON;
	else			RESCUE_SOLENOID_OFF;
	
	if(g_Flag.resurge_solenoid_flag == 1) 			//是否开启复活模块
						RESURGE_SOLENOID_ON;
	else			RESURGE_SOLENOID_OFF;
	
}

/**
  * @brief  控制模式判断函数
						注意这套代码里面的s1和s2跟说明书上面的是反的
  * @param  None
	* @retval s1: 1:键鼠模式
			  2:断电模式
			  3:遥控模式
*/
void controlStateGet(void)															//获取控制模式
{
	
	if (rc_ctrl.rc.s2 == DOWN)															//获取控制模式
	{
		switch(rc_ctrl.rc.s1)
		{
			case UP:
				g_Flag.control_mode = RC_MODE;				//遥控模式
				break;
			case MIDDLE:
				g_Flag.control_mode = KEY_MODE;				//键鼠模式
				break;
			case DOWN:
				g_Flag.control_target = POWER_OFF_MODE;	//掉电模式
				break;
			default:
				break;
		}
	}
//	if (rc_ctrl.rc.s2 == DOWN)															//获取控制模式
//	{
//		switch(rc_ctrl.rc.s1)
//		{
//			case UP:
//				g_Flag.control_mode = RC_MODE;				//遥控模式
//				break;
//			case MIDDLE:
//				g_Flag.control_mode = KEY_MODE;				//键鼠模式
//				break;
//			case DOWN:
//				g_Flag.control_target = POWER_OFF_MODE;	//掉电模式
//				break;
//			default:
//				break;
//		}
//	}
}


/**
  * @brief  控制目标判断函数
  * @param  None
  * @retval None
  */
int test_solenoid = 0, test_solenoid_old = 0;						//测试!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void motionTargetGet(void)															//获取运动模式		
{
	if(g_Flag.control_mode == RC_MODE)				//遥控器模式
	{
		if (rc_ctrl.rc.s2 == MIDDLE)			//底盘模式
		{
			switch(rc_ctrl.rc.s1)
			{
				case UP:						//正常底盘运动
					g_Flag.control_target = CHASSIS_MODE;	
					break;
				case MIDDLE:				//底盘静步模式
					g_Flag.control_target = CHASSIS_MODE_STATIC;
					break;
				case DOWN:					//底盘模式3
					g_Flag.control_target = CHASSIS_MODE3;
					break;
				default:
					break;
			}			
		}
		else if (rc_ctrl.rc.s2 == UP)			//上层模式
		{
			switch(rc_ctrl.rc.s1)
			{
				case UP:						//上层抬升模式
					g_Flag.control_target = SENIOR_UP_MODE;
					break;
				case MIDDLE:				//上层模式2
					g_Flag.control_target = SENIOR_MODE2;
					break;
				case DOWN:					//上层模式3
					g_Flag.control_target = SENIOR_AUTO_MODE;
					break;
				default:
					break;
			}
		}
	}else if (g_Flag.control_mode == KEY_MODE)	//键鼠模式
	{
		//掉电模式
		if(rc_ctrl.key.ctrl == 1 && rc_ctrl.key.b == 1)
		{
			g_Flag.control_target = POWER_OFF_MODE;
		}
		
		//底盘模式
		if(rc_ctrl.key.ctrl == 1 && rc_ctrl.key.z == 1)
		{
			g_Flag.control_target = CHASSIS_MODE;
		}
		if(rc_ctrl.key.ctrl == 1 && rc_ctrl.key.x == 1)
		{
			g_Flag.control_target = CHASSIS_MODE_STATIC;
		}
		if(rc_ctrl.key.ctrl == 1 && rc_ctrl.key.c == 1)
		{
			g_Flag.control_target = CHASSIS_MODE3;
		}
		
		//上层模式
		if(rc_ctrl.key.ctrl == 1 && rc_ctrl.key.q == 1)
		{
			g_Flag.control_target = SENIOR_UP_MODE;
		}
		if(rc_ctrl.key.ctrl == 1 && rc_ctrl.key.e == 1)
		{
			g_Flag.control_target = SENIOR_MODE2;
		}
		if(rc_ctrl.key.ctrl == 1 && rc_ctrl.key.r == 1)
		{
			g_Flag.control_target = SENIOR_AUTO_MODE;
		}
		
	}
	

}


//key_control  key;
///**
//  * @brief  键鼠模式标志位初始化
//  * @param  None
//  * @retval None
//  */
//void  key_control_Init(void)
//{
//	key.flag_save_mode = 0;
//	key.flag_landing_mode = 0;
//	key.flag_manual_landing_mode = 0;
//	key.flag_get_buttel_mode = 0;
//	key.flag_manual_get_buttel_mode = 0;
//	key.flag_key_mode = 0;
//	key.flag_lift_up_mode = 0;
//  key.flag_initialize_mode = 0;
//	key.flag_get_buttel_mode1 = 0;
//	key.flag_choose_mode = 0;
//}

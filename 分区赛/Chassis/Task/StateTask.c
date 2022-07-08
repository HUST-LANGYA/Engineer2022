/**
	*@brief 		ģʽ�ж������ļ�	
	*@author		ף��ΰ
	*@date			2021-12-19
*/
/************************************************************************************
ģʽ�ж�
�ж��߼�����Ҫ��Ϊ�����֣���һ�����ǿ���ģʽ����Ϊң��ģʽ������ģʽ������ģʽ��

�ڶ�����Ϊ����Ŀ�꣬���ң��ģʽʱң�صĲ��֣���������ƶ��������������ϲ�������
�ϲ��������ȵȣ�ң����״̬���ޣ���ʱ�޸�

s2���󣩣�����{s1���ң����£�����ģʽ
             s1���ң����У�����ģʽ
             s1���ң����ϣ�ң��ģʽ}
s2���󣩣�����{s1���ң����£�����ģʽ3
               s1���ң����У����̾���ģʽ
               s1���ң����ϣ�������ͨģʽ}						 
s2���󣩣�����{s1���ң����£��ϲ�ģʽ1
               s1���ң����У��ϲ�ģʽ2
               s1���ң����ϣ��ϲ��Զ�����}
***************************************************************************************/
#include "StateTask.h"

uint32_t  Mode_Switch_high_water;


extern FlagWithSlave_t FlagOfSlave;		//���յ���C���־λ


extern short q_rising_flag,w_rising_flag,e_rising_flag,r_rising_flag,
		a_rising_flag,s_rising_flag,d_rising_flag,f_rising_flag,g_rising_flag,
		z_rising_flag,x_rising_flag,c_rising_flag,v_rising_flag,b_rising_flag,
		shift_rising_flag,ctrl_rising_flag,mouse_Press_l_rising_flag,mouse_Press_r_rising_flag;
extern unsigned char Data_Receive_from_F103[8];//Data_Receive����ǽ��յ��Ĵ�C�崫��������

extern uint8_t graphic_init_flag;  //ͼ�ν����ʼ����־λ


/**
  * @brief  ģʽת������״̬��
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
		IWDG_Feed();
		
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
		
//		#if INCLUDE_uxTaskGetStackHighWaterMark
//        Mode_Switch_high_water = uxTaskGetStackHighWaterMark(NULL);
//		#endif
  }
}


/**
  * @brief  ģʽת������
  * @param  None
	* @retval 
*/
void switchMode(void)
{
/**************** ����ģʽ�°���״̬�ж� *****************/
	key_rising_check();
	key_refresh();
/*********************** ״̬�л� ************************/
	controlStateGet();
	
	/************************************ �޸ı�־λ **********************************************/
	if(g_Flag.control_mode == KEY_MODE)
			keyModeFlagChange();		//����ģʽ�޸ı�־λ 
	else if(g_Flag.control_mode == RC_MODE)
	{
			motionTargetGet();			//��ȡ�˶�ģʽ
			rcModeFlagChange();			//ң����ģʽ�޸ı�־λ 
	}else
			g_Flag.control_mode = KEY_MODE;		//Ĭ�ϼ���ģʽ

	if(rc_ctrl.key.ctrl == 0 && rc_ctrl.key.shift == 1)
		g_Flag.gyro_use_flag = 0;
	else
		g_Flag.gyro_use_flag = g_Flag.gyro_use_flag_pre;
	
	if(g_Flag.control_target == POWER_OFF_MODE)			//������ط��ӿ��Ź�!!!!!!!!!!!!!!!!
	{
		//�����ж������޸�ι����־λ
		
	}
	
	
	/*************************������״̬���**************************/
	g_Flag.laser_ranging_flag_rising 	= g_Flag.laser_ranging_flag - g_Flag.laser_ranging_flag_pre;
	g_Flag.laser_ranging_flag_pre 		= g_Flag.laser_ranging_flag;
	if(g_Flag.laser_mid != LASER_MID_INIT)
	{
		if(g_Flag.laser_mid < LASER_MID_INIT)
		{
			if(g_Flag.laser_ranging_flag_rising == 1)
				g_Flag.laser_mid ++;
		}else if(g_Flag.laser_mid > LASER_MID_INIT)
		{
			if(g_Flag.laser_ranging_flag_rising == -1)
				g_Flag.laser_mid --;
		}
	}
	
	/***************************��ŷ�*******************************/
	if(g_Flag.rescue_solenoid_flag == 1) 			//�Ƿ�����Ԯģ��
						RESCUE_SOLENOID_ON;
	else			RESCUE_SOLENOID_OFF;
	
	if(g_Flag.resurge_solenoid_flag == 1) 			//�Ƿ�������ģ��
						RESURGE_SOLENOID_ON;
	else			RESURGE_SOLENOID_OFF;
	
	if(g_Flag.fork_solenoid_flag == 1) 			//�Ƿ����泵ģ��
						FORK_SOLENOID_ON;
	else			FORK_SOLENOID_OFF;
	
}



/**
  * @brief  ����ģʽ�±�־λ�ı亯��
  * @param  None
	* @retval 
*/
void keyModeFlagChange(void)
{
	if(g_Flag.control_mode == KEY_MODE)
	{
		if(g_Flag.control_target == NORMAL_MODE)
		{
			if(rc_ctrl.key.ctrl == 0 && rc_ctrl.key.shift == 0)
			{
					
					
					/*********************** r�������ؿ���һ���Զ���λ **************************/
					if(r_rising_flag == 1)
					{
						g_Flag.auto_mode = RESET_SOFTWARE;
					}
					
					/*********************** g�������ؿ��Ƽ��Ӵ�/�ر� **************************/
					if(g_rising_flag == 1)
					{
						g_Flag.clamp_flag = 1 - g_Flag.clamp_flag;
					}
					
					/*********************** z�������ؿ��ƾ�Ԯ��/�ر� **************************/
					if(z_rising_flag == 1)
					{
						g_Flag.rescue_solenoid_flag = 1 - g_Flag.rescue_solenoid_flag;
					}
					
					/*********************** x�������ؿ��Ƹ����/�ر� **************************/
					if(x_rising_flag == 1)
					{
						g_Flag.resurge_solenoid_flag = 1 - g_Flag.resurge_solenoid_flag;
					}
					
					/*********************** c�������ؿ��Ʋ泵��/�ر� **************************/
					if(c_rising_flag == 1)
					{
						g_Flag.fork_solenoid_flag = 1 - g_Flag.fork_solenoid_flag;
					}
					
					/*********************** v��������ǿ���˳��Զ�ģʽ  **************************/
					if(v_rising_flag == 1)
					{
						g_Flag.auto_mode = AUTO_MODE_OFF;
						g_Flag.laser_mid = LASER_MID_INIT;
					}
					
					/*********************** f�������ؿ����Զ��һ� **************************/
					if(f_rising_flag == 1)
					{
						g_Flag.auto_mode = EXCHANGE_MINE;
					}
					/*********************** b�����������³�ʼ��ȫ��ͼ�� **************************/
					if(b_rising_flag == 1)
					{
						if(graphic_init_flag == 1)
							graphic_init_flag = 0;
						else
							graphic_init_flag = 1;
					}
			}else if(rc_ctrl.key.ctrl == 1 && rc_ctrl.key.shift == 0)
			{
				
					/*********************** ctrl + q�������ؿ��ƴ���Դ���Զ�ȡ�� **************************/
					if(q_rising_flag == 1)
					{
						g_Flag.auto_mode = LARGE_ISLAND_MINE;
					}
					
					/*********************** ctrl + e�������ؿ���С��Դ���Զ�ȡ�� **************************/
					if(e_rising_flag == 1)
					{
						g_Flag.auto_mode = SMALL_ISLAND_MINE;
					}
					
					
					/*********************** ctrl + r�������ؿ����Զ��һ����β��� **************************/
					if(r_rising_flag == 1)
					{
						g_Flag.auto_mode = EXCHANGE_MINE_TWICE;
					}
					
					/*********************** ctrl + f�������ؿ����Զ��սӿ�ʯ **************************/
					if(f_rising_flag == 1)
					{
						g_Flag.auto_mode = MINE_MIDAIR;
					}
					
					/*********************** ctrl + g�� **************************/
					if(g_rising_flag == 1)
					{
						
					}
					
					/*********************** ctrl + z�����Ʋֿ�����ת **************************/
					if(z_rising_flag == 1)
					{
						g_Flag.warehouse_flag = 1 - g_Flag.warehouse_flag;
					}
					
					/*********************** ctrl + x������צ�ӵ����ת **************************/
					if(x_rising_flag == 1)
					{
						g_Flag.rotate_flag = 1 - g_Flag.rotate_flag;
					}
					
					/*********************** ctrl + c�� **************************/
					if(c_rising_flag == 1)
					{
						
					}
				
					/*********************** ctrl + v������ͼ��pitch���ͷ  **************************/
					if(v_rising_flag == 1)
					{
						if(g_Flag.camera_pitch == CAMERA_DEFAULT)
							g_Flag.camera_pitch = CAMERA_PITCH_DOWN;
						else
							g_Flag.camera_pitch = CAMERA_DEFAULT;
					}
					
					/*********************** ctrl + b�����������ǿ��� **************************/
					if(b_rising_flag == 1)
					{
						g_Flag.gyro_use_flag_pre = 1 - g_Flag.gyro_use_flag_pre;
					}
			}else if(rc_ctrl.key.ctrl == 0 && rc_ctrl.key.shift == 1)
			{
					/*********************** shift + b��  **************************/
									//�����ٶ�������ǰƫ����
				
					/*********************** shift + v��  **************************/
					if(v_rising_flag == 1)
					{
						
					}
					
					/*********************** shift + b��  **************************/
					if(b_rising_flag == 1)
					{
						
					}
			}else if(rc_ctrl.key.ctrl == 1 && rc_ctrl.key.shift == 1)
			{
					/*********************** ctrl + shift + q�����ƴ���Դ��������̧�� **************************/
					if(q_rising_flag == 1)
					{
						if(g_Flag.lift_once_flag == 0)
								g_Flag.lift_once_flag = 2;
						else g_Flag.lift_once_flag = 0;
					}
					
					/*********************** ctrl + shift + e������ǰ�Ƶ�ŷ����� **************************/
					if(e_rising_flag == 1)
					{
						g_Flag.forward_flag = 1 - g_Flag.forward_flag;
					}
					
					/*********************** ctrl + shift + r�����ƴ���Դ������̧�� **************************/
					if(r_rising_flag == 1)
					{
						if(g_Flag.lift_down_twice_flag != 3)
								g_Flag.lift_down_twice_flag = 3;
						else g_Flag.lift_down_twice_flag = 0;
					}
					
					/*********************** ctrl + shift + f�� **************************/
					if(f_rising_flag == 1)
					{
						
					}
					
					/*********************** ctrl + shift + g�� **************************/
					if(g_rising_flag == 1)
					{
						
					}
					
					/*********************** ctrl + shift + z������С��Դ��������̧�� **************************/
					if(z_rising_flag == 1)
					{
						if(g_Flag.lift_once_flag == 0)
								g_Flag.lift_once_flag = 1;
						else g_Flag.lift_once_flag = 0;
					}
					
					/*********************** ctrl + shift + x�����ƶһ���ʯ����̧�� **************************/
					if(x_rising_flag == 1)
					{
						if(g_Flag.lift_down_twice_flag != 2)
								g_Flag.lift_down_twice_flag = 2;
						else g_Flag.lift_down_twice_flag = 0;
					}
					
					/*********************** ctrl + shift + c�����ƿս����׿��� **************************/
					if(c_rising_flag == 1)
					{
						g_Flag.midair_solenoid_flag = 1 - g_Flag.midair_solenoid_flag;
					}
				
					/*********************** ctrl + shift + v������С��Դ��ȡ�����̧��  **************************/
					if(v_rising_flag == 1)
					{
						if(g_Flag.lift_down_twice_flag != 1)
								g_Flag.lift_down_twice_flag = 1;
						else g_Flag.lift_down_twice_flag = 0;
					}
				
//					/*********************** ctrl + shift + z�����Ƽ����λ���� **************************/
//					if(z_rising_flag == 1)
//					{
//						g_Flag.laser_mid --;
//					}
//					
//					/*********************** ctrl + shift + x�����Ƽ����λ���� **************************/
//					if(x_rising_flag == 1)
//					{
//						g_Flag.laser_mid ++;
//					}
				
					/*********************** ctrl + shift + b��  **************************/
					if(b_rising_flag == 1)
					{
						
					}
			}
				
			
			if(rc_ctrl.key.ctrl == 1 && rc_ctrl.key.shift == 0)
			{
				g_Flag.rotate_w = rc_ctrl.key.w;
				g_Flag.rotate_s = rc_ctrl.key.s;
			}else
			{
				g_Flag.rotate_w = 0;
				g_Flag.rotate_s = 0;
			}
			
			
			
			
			/*********************** �Զ�ģʽ�µ������ϲ���� **************************/
			if(g_Flag.auto_mode != AUTO_MODE_OFF)
			{
					g_Flag.clamp_flag = FlagOfSlave.flag.clamp_flag;
					g_Flag.exchange_solenoid_flag = FlagOfSlave.flag.exchange_solenoid_flag;
					g_Flag.forward_flag = FlagOfSlave.flag.forward_flag;
					g_Flag.lift_down_twice_flag = FlagOfSlave.flag.lift_down_twice_flag;
					g_Flag.lift_once_flag = FlagOfSlave.flag.lift_once_flag;
					g_Flag.midair_solenoid_flag = FlagOfSlave.flag.midair_solenoid_flag;
			}
			
		}else 
		{
			
		}
	}
}


/**
  * @brief  ң����ģʽ�±�־λ�ı亯��
  * @param  None
	* @retval 
*/
void rcModeFlagChange(void)
{
	if(g_Flag.control_mode == RC_MODE)
	{
		switch(g_Flag.control_target)
		{
			case CHASSIS_MODE3:
				if(rc_ctrl.rc.ch2 < 500) 							//�����ƶ�����
				{
					g_Flag.rescue_solenoid_flag = 1; //��Ԯ��ŷ�
				}else if(rc_ctrl.rc.ch2 > 1500) 					//�����ƶ�����
				{
					g_Flag.rescue_solenoid_flag = 0; //��Ԯ��ŷ��ر�
				}
				
				if(rc_ctrl.rc.ch0 < 500) 							//�����ƶ�����
				{
					g_Flag.resurge_solenoid_flag = 1; //�����ŷ�
				}else if(rc_ctrl.rc.ch0 > 1500) 					//�����ƶ�����
				{
					g_Flag.resurge_solenoid_flag = 0; //�����ŷ��ر�
				}
				break;
			
			case SENIOR_UP_MODE:
				if(rc_ctrl.rc.ch3 > 1500) 							//�����ƶ�����
				{
					g_Flag.lift_down_twice_flag = 1; //����̧��
				}else if(rc_ctrl.rc.ch3 < 500) 					//�����ƶ�����
				{
					g_Flag.lift_down_twice_flag = 0; //�����½�
				}
				
				if(rc_ctrl.rc.ch2 < 500) 							//�����ƶ�����
				{
					g_Flag.midair_solenoid_flag = 1; //�սӵ�ŷ�
				}else if(rc_ctrl.rc.ch2 > 1500) 					//�����ƶ�����
				{
					g_Flag.midair_solenoid_flag = 0; //�սӵ�ŷ��ر�
				}
				
				if(rc_ctrl.rc.ch1 > 1500) 							//�����ƶ�����
				{
					g_Flag.lift_once_flag 	= 1; 	//һ��̧��
		//					g_Flag.forward_flag			= 1;	//ǰ��
				}else if(rc_ctrl.rc.ch1 < 500) 					//�����ƶ�����
				{
					g_Flag.lift_once_flag = 0; 	//һ���½�
		//					g_Flag.forward_flag			= 0;	//ǰ���ջ�
				}


				if(rc_ctrl.rc.ch0 < 500) 							//�����ƶ�����
				{
					g_Flag.clamp_flag = 1; //��ȡ
				}else if(rc_ctrl.rc.ch0 > 1500) 					//�����ƶ�����
				{
					g_Flag.clamp_flag = 0; //�ɿ�
				}
				break;
			
			case SENIOR_MODE2:
				if(rc_ctrl.rc.ch0 < 500) 							//�����ƶ�����
				{
					g_Flag.exchange_solenoid_flag = 1; //�һ���ŷ�
				}else if(rc_ctrl.rc.ch0 > 1500) 					//�����ƶ�����
				{
					g_Flag.exchange_solenoid_flag = 0; //�һ���ŷ��ر�
				}
				
				if(rc_ctrl.rc.ch1 > 1500) 							//�����ƶ�����
				{
					g_Flag.forward_flag			= 1;	//ǰ��
				}else if(rc_ctrl.rc.ch1 < 500) 					//�����ƶ�����
				{
					g_Flag.forward_flag			= 0;	//ǰ���ջ�
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
}


/**
  * @brief  ����ģʽ�жϺ���
						ע�����״��������s1��s2��˵����������Ƿ���
  * @param  None
	* @retval s1: 1:����ģʽ
			  2:�ϵ�ģʽ
			  3:ң��ģʽ
*/
void controlStateGet(void)															//��ȡ����ģʽ
{
	
	if (rc_ctrl.rc.s2 == UP)															//��ȡ����ģʽ
	{
		switch(rc_ctrl.rc.s1)
		{
			case UP:
				g_Flag.control_mode = KEY_MODE;							//����ģʽ
				g_Flag.control_target = NORMAL_MODE;			//����ģʽ
				break;
			case MIDDLE:
				if(g_Flag.control_target == RC_MODE)
						g_Flag.control_target = CHASSIS_MODE;		//ң���������˶�ģʽ
				else
						g_Flag.control_mode = KEY_MODE;
				break;
			case DOWN:
				g_Flag.control_mode = RC_MODE;					//ң��ģʽ
				break;
			default:
				g_Flag.control_mode = KEY_MODE;	
				break;
		}
	}
	
	if(rc_ctrl.rc.s2 == DOWN && rc_ctrl.rc.s1 == DOWN)
	{
		g_Flag.control_target = POWER_OFF_MODE;
		g_Flag.control_mode = KEY_MODE;	
	}
	if(rc_ctrl.rc.s2 == MIDDLE && rc_ctrl.rc.s1 == MIDDLE)
	{
		g_Flag.control_target = CHECK_MODE;
//		g_Flag.control_mode 	= KEY_MODE;	
	}
	
	
	
//	if (rc_ctrl.rc.s2 == DOWN)															//��ȡ����ģʽ
//	{
//		switch(rc_ctrl.rc.s1)
//		{
//			case UP:
//				g_Flag.control_mode = RC_MODE;				//ң��ģʽ
//				break;
//			case MIDDLE:
//				g_Flag.control_mode = KEY_MODE;				//����ģʽ
//				break;
//			case DOWN:
//				g_Flag.control_target = POWER_OFF_MODE;	//����ģʽ
//				break;
//			default:
//				break;
//		}
//	}
}


/**
  * @brief  ����Ŀ���жϺ���
  * @param  None
  * @retval None
  */
int test_solenoid = 0, test_solenoid_old = 0;						//����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void motionTargetGet(void)															//��ȡ�˶�ģʽ		
{
	if(g_Flag.control_mode == RC_MODE)				//ң����ģʽ
	{
		if (rc_ctrl.rc.s2 == MIDDLE)			//����ģʽ
		{
			switch(rc_ctrl.rc.s1)
			{
				case UP:						//���������˶�
					g_Flag.control_target = CHASSIS_MODE;	
					break;
				case MIDDLE:				//���̾���ģʽ
					g_Flag.control_target = CHASSIS_MODE_STATIC;
					break;
				case DOWN:					//����ģʽ3
					g_Flag.control_target = CHASSIS_MODE3;
					break;
				default:
					break;
			}
		}
		else if (rc_ctrl.rc.s2 == UP)			//�ϲ�ģʽ
		{
			switch(rc_ctrl.rc.s1)
			{
				case UP:						//�ϲ�̧��ģʽ
					g_Flag.control_target = SENIOR_UP_MODE;
					break;
				case MIDDLE:				//�ϲ�ģʽ2
					g_Flag.control_target = SENIOR_MODE2;
					break;
				case DOWN:					//�ϲ�ģʽ3
//					g_Flag.control_target = SENIOR_AUTO_MODE;
					break;
				default:
					break;
			}
		}
	}
	
}


//key_control  key;
///**
//  * @brief  ����ģʽ��־λ��ʼ��
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

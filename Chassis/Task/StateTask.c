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

extern short ctrl_rising_flag,shift_rising_flag,v_rising_flag,c_rising_flag,e_rising_flag,x_rising_flag,Press_Key_x_Flag,v_rising_flag;
extern short f_rising_flag,mouse_Press_r_rising_flag,r_rising_flag,z_rising_flag,Press_Key_z_Flag,g_rising_flag,q_rising_flag;
extern unsigned char Data_Receive_from_F103[8];//Data_Receive����ǽ��յ��Ĵ�C�崫��������


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
		
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
		
		#if INCLUDE_uxTaskGetStackHighWaterMark
        Mode_Switch_high_water = uxTaskGetStackHighWaterMark(NULL);
		#endif
  }
}


/**
  * @brief  ģʽת������
  * @param  None
	* @retval 
*/
int test_q = 0;
int test11 = 1;

void switchMode(void)
{
/**************** ����ģʽ�°���״̬�ж� *****************/
	key_rising_check();
	key_refresh();
/*********************** ״̬�л� ************************/
	controlStateGet();
	motionTargetGet();																	//��ȡ�˶�ģʽ
	
	/************************************ ң����ģʽ�޸ı�־λ **********************************************/
	/*�������ϲ�̧��ģʽ����̧���ı�־λ�����ж�*/
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
	
	/************************************ ����ģʽ�޸ı�־λ **********************************************/
	if(g_Flag.control_mode == KEY_MODE)
	{
		/*********************** q�������ؿ���һ��̧����/�ر� **************************/
		if(q_rising_flag == 1)
		{
//			test_q ++;
			g_Flag.lift_once_flag = 1 - g_Flag.lift_once_flag;
		}
		
		/*********************** e�������ؿ��Ƽ�ȡ��ŷ���/�ر� **************************/
		if(e_rising_flag == 1)
		{
			g_Flag.clamp_flag = 1 - g_Flag.clamp_flag;
		}
		
		/*********************** r�������ؿ��ƶ���̧����/�ر� **************************/
		if(r_rising_flag == 1)
		{
			g_Flag.lift_down_twice_flag = 1 - g_Flag.lift_down_twice_flag;
		}
		
	}

	
	if(g_Flag.rescue_solenoid_flag == 1) 			//�Ƿ�����Ԯģ��
						RESCUE_SOLENOID_ON;
	else			RESCUE_SOLENOID_OFF;
	
	if(g_Flag.resurge_solenoid_flag == 1) 			//�Ƿ�������ģ��
						RESURGE_SOLENOID_ON;
	else			RESURGE_SOLENOID_OFF;
	
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
	
	if (rc_ctrl.rc.s2 == DOWN)															//��ȡ����ģʽ
	{
		switch(rc_ctrl.rc.s1)
		{
			case UP:
				g_Flag.control_mode = RC_MODE;				//ң��ģʽ
				break;
			case MIDDLE:
				g_Flag.control_mode = KEY_MODE;				//����ģʽ
				break;
			case DOWN:
				g_Flag.control_target = POWER_OFF_MODE;	//����ģʽ
				break;
			default:
				break;
		}
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
					g_Flag.control_target = SENIOR_AUTO_MODE;
					break;
				default:
					break;
			}
		}
	}else if (g_Flag.control_mode == KEY_MODE)	//����ģʽ
	{
		//����ģʽ
		if(rc_ctrl.key.ctrl == 1 && rc_ctrl.key.b == 1)
		{
			g_Flag.control_target = POWER_OFF_MODE;
		}
		
		//����ģʽ
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
		
		//�ϲ�ģʽ
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

/**
	*@brief 		�����˶������ļ�
	*@author		ף��ΰ
	*@date			2021-12-19
*/
#include "ChassisTask.h"

uint32_t Chassis_high_water;

int vx, vy, vw;											//Ԥ���ٶ�
int vx_set, vy_set, vw_set;					//��ǰ���õ��ٶ�
pid_Typedef chassis_vel_pid[4];			//�����ٶ�PID

int w_offset_cnt;
int w_all_offset;	

pid_Typedef chassis_pos_follow_pid = {
	.P = 0.25f,
	.I = 0.00065f,
	.D = 0.3f,
	.IMax = 100.0f,
	.SetPoint = 0.0f
};
pid_Typedef chassis_vel_follow_pid = {
	.P = 1750.0f,
	.I = 0.0f,
	.D = 150.0f,
	.IMax = 0.0f,
	.SetPoint = 0.0f
};

extern rmc620_t chassis_motor[4];     

/**
  	*@brief 		�����˶�������
  	*@param		  void
	*@return		  void
*/
void chassisTask(void *pvParameters)
{
//	portTickType xLastWakeTime;
//	const portTickType xFrequency=1;
	
	while(1){
//		xLastWakeTime=xTaskGetTickCount();
//		
		chassisExec();
		
		vTaskDelay(1);
//		
//		vTaskDelayUntil(&xLastWakeTime,xFrequency);
//		
		#if INCLUDE_uxTaskGetStackHighWaterMark
        Chassis_high_water = uxTaskGetStackHighWaterMark(NULL);
		#endif
  }
}

/**
  * @brief  �����ٶȻ�������ʼ��
  * @param  PID������
  * @retval None
  */
void chassisVelPidInit(float p, float i, float d, float i_max)
{
	int j = 0;
	
	for (j = 0; j < 4; j ++)
	{
		chassis_vel_pid[j].P = p;
		chassis_vel_pid[j].I = i;
		chassis_vel_pid[j].D = d;
        chassis_vel_pid[j].IMax = i_max;
		chassis_vel_pid[j].SetPoint = 0.0f;
	}
}


/**
  * @brief  ����ִ�д���
  * @param  void
  * @retval void
*/
void chassisExec(void)
{  
	if(g_Flag.control_target == POWER_OFF_MODE )
	{
		chassisPowerOff();
	}
	else 
	{
//		if(g_Flag.control_target == CHASSIS_MODE || g_Flag.control_target == CHASSIS_MODE_STATIC)
//		{
			if (g_Flag.control_mode == RC_MODE)         //����ң��ģʽ
			{
				chassisRcModeCal();
			}
			//�������
			else if(g_Flag.control_mode == KEY_MODE)
//			else
			{
				chassisKeyModeCal();
			}
			
			if (g_Flag.gyro_use_flag)																										//���ʹ������������
			{
					chassis_vel_follow_pid.SetPoint = LIMIT_MAX_MIN(PID_Calc(&chassis_pos_follow_pid, getYawAngle()), 5.7f, -5.7f);
					vw = PID_Calc(&chassis_vel_follow_pid, getGz());
			}
//		}else{
//			vx = 0;
//			vy = 0;
//			vw = 0;
//		}
		
		if(g_Flag.control_target == CHASSIS_MODE_STATIC)												//���̾���ģʽ���������٣�����ɲ����
		{
			chassisModeStatic();
		}else																																//��������ģʽ
		{
			chassisModeNormal();
		}
		
		chassisCal(vx_set, vy_set, vw_set);
	}
}

/**
  * @brief  ���̶ϵ�
  * @param  None
  * @retval None
  */
void chassisPowerOff(void)
{
	chassisCurrentSend(0, 0, 0, 0);
}

/**
  * @brief  ң����ģʽ����ʱ���̼������
  * @param  void
  * @retval void
*/
void chassisRcModeCal(void)
{
			if (rc_ctrl.rc.ch0 > 1044 || rc_ctrl.rc.ch0 < 1004)
				vx = (1024 - rc_ctrl.rc.ch0) * 10.0f;
			else 
				vx = 0;
			
			if (rc_ctrl.rc.ch1 > 1044 || rc_ctrl.rc.ch1 < 1004)
				vy = (1024 - rc_ctrl.rc.ch1) * 10.0f;
			else 
				vy = 0;
			
			if (rc_ctrl.rc.ch2 > 1044 || rc_ctrl.rc.ch2 < 1004)
			{	
				if (g_Flag.gyro_use_flag)	//ʹ������������
					chassis_pos_follow_pid.SetPoint += (1024 - rc_ctrl.rc.ch2) / 4000.0f;
				else											//��ʹ������������
					vw = (1024 - rc_ctrl.rc.ch2) * 10.0f;
			}
			else 
				vw = 0;
}

/**
  * @brief  ����ģʽ����ʱ���̼������
  * @param  void
  * @retval void
*/
void chassisKeyModeCal(void)
{
	
	if (g_Flag.gyro_use_flag)	//ʹ������������
	{
		vx = (rc_ctrl.key.a - rc_ctrl.key.d) * (1.0F - rc_ctrl.key.shift * 0.56F+ rc_ctrl.mouse.press_l * 2.0f ) * 1500.0F;
		vy = (-rc_ctrl.key.w + rc_ctrl.key.s) * (1.0F - rc_ctrl.key.shift * 0.56F+ rc_ctrl.mouse.press_l * 2.0f ) * 2000.0F;
		chassis_pos_follow_pid.SetPoint -= rc_ctrl.mouse.x / 300.0f * (1.0F - rc_ctrl.key.shift * 0.56F);    //����
		//chassis_pos_follow_pid.SetPoint += (rc_ctrl.mouse.press_l - rc_ctrl.mouse.press_r)*0.02 * (1.0F - rc_ctrl.key.shift * 0.7F);    //����
	}
	else											//��ʹ������������
	{			
		vx = (rc_ctrl.key.a - rc_ctrl.key.d) * (1.0F - rc_ctrl.key.shift * 0.56F ) * 2500.0F;
		vy = (-rc_ctrl.key.w + rc_ctrl.key.s) * (1.0F - rc_ctrl.key.shift * 0.56F ) * 2500.0F;
		//vw = -rc_ctrl.mouse.x * 100.0f * (1.0F - rc_ctrl.key.shift * 0.7F);
		//chassis_pos_follow_pid.SetPoint = getYawAngle();
		vw = (rc_ctrl.mouse.press_l - rc_ctrl.mouse.press_r)*1500.0f*(1.0f - rc_ctrl.key.shift * 0.56F);
	}
}

/**
  * @brief  ��������ģʽ�µ��ٶ��趨����
  * @param  void
  * @retval void
*/
void chassisModeNormal(void)
{
//	vw_set = vw;
//	
//	if (ABS(vx - vx_set) > 100.0f && ((vx < -1000) || (vx > 1000)))   //����ʱ���٣�ֹͣʱѸ��ɲ��
//		vx_set = vx + 0.02f * (vx - vx_set);
//	else
//		vx_set = vx;

//	if (ABS(vy - vy_set) > 100.0f && ((vy < -1000) || (vy > 1000)))   //����ʱ���٣�ֹͣʱѸ��ɲ��
//		vy_set = vy + 0.02f * (vy - vy_set);
//	else
//		vy_set = vy;
	
	vw_set = chassisModeNormalCalExec(vw, vw_set);
	vx_set = chassisModeNormalCalExec(vx, vx_set);
	vy_set = chassisModeNormalCalExec(vy, vy_set);
}

/**
  * @brief  ��������ģʽ�ٶ��趨������㺯�� 
						��vx,vy,vw����ͬ��������˷�װ���򻯴���
	* @param  v:Ԥ���ٶ�,v_set:Ŀǰ�ٶ�
	* @retval v_set:����Ŀǰ�ٶ��Ը�ֵ
*/	
int chassisModeNormalCalExec(int v, int v_set)
{
	if(v > 0)
	{
		if(v > v_set)	
			v_set = v - 0.9f * (v - v_set);
		else
			v_set = v - 0.9f * (v - v_set);
	}else if(v < 0)
	{
		if(v < v_set)	
			v_set = v - 0.9f * (v - v_set);
		else
			v_set = v - 0.9f * (v - v_set);
	}else 
	{
		v_set = v - 0.999f * (v - v_set);
	}
	return v_set;
	
}

/**
  * @brief  ���̾���ģʽ�µ��ٶ��趨����
  * @param  void
  * @retval void
*/
void chassisModeStatic(void)
{
	vw_set = chassisModeStaticCalExec(vw, vw_set);
	vx_set = chassisModeStaticCalExec(vx, vx_set);
	vy_set = chassisModeStaticCalExec(vy, vy_set);
}

/**
  * @brief  ���̾���ģʽ�ٶ��趨������㺯�� 
						��vx,vy,vw����ͬ��������˷�װ���򻯴���
	* @param  v:Ԥ���ٶ�,v_set:Ŀǰ�ٶ�
	* @retval v_set:����Ŀǰ�ٶ��Ը�ֵ
*/	
int chassisModeStaticCalExec(int v, int v_set)
{
	if(v > 0)
	{
		if(v > v_set)	
			v_set = v - 0.999f * (v - v_set);
		else
			v_set = v - 0.9f * (v - v_set);
	}else if(v < 0)
	{
		if(v < v_set)	
			v_set = v - 0.999f * (v - v_set);
		else
			v_set = v - 0.9f * (v - v_set);
	}else 
	{
		v_set = v - 0.9f * (v - v_set);
	}
	return v_set;
}








/**
  * @brief  �����ٶȷֽ⼰PID����
  * @param  vx�������ٶ�
			vy��ǰ���ٶ�
			vw����ת�ٶ�
  * @retval None
  */
//int chassis_current[4];							//Only for debug
void chassisCal(int vx, int vy, int vw)
{
	int chassis_current[4];	
	int i = 0;
	
	chassis_vel_pid[0].SetPoint = LIMIT_MAX_MIN(-(vx + vy + vw), 10000, -10000);
	chassis_vel_pid[1].SetPoint = LIMIT_MAX_MIN((vx - vy - vw), 10000, -10000);
	chassis_vel_pid[2].SetPoint = LIMIT_MAX_MIN(-(vx - vy + vw), 10000, -10000);
	chassis_vel_pid[3].SetPoint = LIMIT_MAX_MIN((vx + vy - vw), 10000, -10000);
	
	for (i = 0; i < 4; i ++)
		chassis_current[i] = PID_Calc(&chassis_vel_pid[i], chassis_motor[i].speed);
	chassisCurrentSend(chassis_current[0], chassis_current[1], chassis_current[2], chassis_current[3]);
}

/**
  * @brief  ������תƫ��
  * @param  speed����ת�ٶ�
  * @retval None
  */						
void w_offset(float speed)
{
	if (w_offset_cnt < w_all_offset)
	{
		chassis_pos_follow_pid.SetPoint += speed;
		w_offset_cnt ++;
	}
	else if (w_offset_cnt > w_all_offset)
	{
		chassis_pos_follow_pid.SetPoint -= speed;
		w_offset_cnt --;
	}
	chassis_vel_follow_pid.SetPoint = LIMIT_MAX_MIN(PID_Calc(&chassis_pos_follow_pid, getYawAngle()), 5.7f, -5.7f);
		vw = PID_Calc(&chassis_vel_follow_pid, getGz());
	chassisCal(0, 0, vw);
}

void w_turn(float speed)
{
	chassis_pos_follow_pid.SetPoint -= speed;
	chassis_vel_follow_pid.SetPoint = LIMIT_MAX_MIN(PID_Calc(&chassis_pos_follow_pid, getYawAngle()), 5.7f, -5.7f);
		vw = PID_Calc(&chassis_vel_follow_pid, getGz());
	//chassisCal(0, 0, vw);
}




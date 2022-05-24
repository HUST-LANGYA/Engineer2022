/**
	*@brief 		底盘运动任务文件
	*@author		祝科伟
	*@date			2021-12-19
*/
#include "ChassisTask.h"

uint32_t Chassis_high_water;

int vx, vy, vw;											//预期速度
int vx_set, vy_set, vw_set;					//当前设置的速度
pid_Typedef chassis_vel_pid[4];			//底盘速度PID

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
  	*@brief 		底盘运动任务函数
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
  * @brief  底盘速度环参数初始化
  * @param  PID各参数
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
  * @brief  底盘执行代码
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
			if (g_Flag.control_mode == RC_MODE)         //处于遥控模式
			{
				chassisRcModeCal();
			}
			//键鼠控制
			else if(g_Flag.control_mode == KEY_MODE)
//			else
			{
				chassisKeyModeCal();
			}
			
			if (g_Flag.gyro_use_flag)																										//如果使用陀螺仪数据
			{
					chassis_vel_follow_pid.SetPoint = LIMIT_MAX_MIN(PID_Calc(&chassis_pos_follow_pid, getYawAngle()), 5.7f, -5.7f);
					vw = PID_Calc(&chassis_vel_follow_pid, getGz());
			}
//		}else{
//			vx = 0;
//			vy = 0;
//			vw = 0;
//		}
		
		if(g_Flag.control_target == CHASSIS_MODE_STATIC)												//底盘静步模式（缓慢加速，快速刹车）
		{
			chassisModeStatic();
		}else																																//正常底盘模式
		{
			chassisModeNormal();
		}
		
		chassisCal(vx_set, vy_set, vw_set);
	}
}

/**
  * @brief  底盘断电
  * @param  None
  * @retval None
  */
void chassisPowerOff(void)
{
	chassisCurrentSend(0, 0, 0, 0);
}

/**
  * @brief  遥控器模式控制时底盘计算代码
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
				if (g_Flag.gyro_use_flag)	//使用陀螺仪数据
					chassis_pos_follow_pid.SetPoint += (1024 - rc_ctrl.rc.ch2) / 4000.0f;
				else											//不使用陀螺仪数据
					vw = (1024 - rc_ctrl.rc.ch2) * 10.0f;
			}
			else 
				vw = 0;
}

/**
  * @brief  键鼠模式控制时底盘计算代码
  * @param  void
  * @retval void
*/
void chassisKeyModeCal(void)
{
	
	if (g_Flag.gyro_use_flag)	//使用陀螺仪数据
	{
		vx = (rc_ctrl.key.a - rc_ctrl.key.d) * (1.0F - rc_ctrl.key.shift * 0.56F+ rc_ctrl.mouse.press_l * 2.0f ) * 1500.0F;
		vy = (-rc_ctrl.key.w + rc_ctrl.key.s) * (1.0F - rc_ctrl.key.shift * 0.56F+ rc_ctrl.mouse.press_l * 2.0f ) * 2000.0F;
		chassis_pos_follow_pid.SetPoint -= rc_ctrl.mouse.x / 300.0f * (1.0F - rc_ctrl.key.shift * 0.56F);    //键鼠
		//chassis_pos_follow_pid.SetPoint += (rc_ctrl.mouse.press_l - rc_ctrl.mouse.press_r)*0.02 * (1.0F - rc_ctrl.key.shift * 0.7F);    //键鼠
	}
	else											//不使用陀螺仪数据
	{			
		vx = (rc_ctrl.key.a - rc_ctrl.key.d) * (1.0F - rc_ctrl.key.shift * 0.56F ) * 2500.0F;
		vy = (-rc_ctrl.key.w + rc_ctrl.key.s) * (1.0F - rc_ctrl.key.shift * 0.56F ) * 2500.0F;
		//vw = -rc_ctrl.mouse.x * 100.0f * (1.0F - rc_ctrl.key.shift * 0.7F);
		//chassis_pos_follow_pid.SetPoint = getYawAngle();
		vw = (rc_ctrl.mouse.press_l - rc_ctrl.mouse.press_r)*1500.0f*(1.0f - rc_ctrl.key.shift * 0.56F);
	}
}

/**
  * @brief  正常底盘模式下的速度设定计算
  * @param  void
  * @retval void
*/
void chassisModeNormal(void)
{
//	vw_set = vw;
//	
//	if (ABS(vx - vx_set) > 100.0f && ((vx < -1000) || (vx > 1000)))   //启动时提速，停止时迅速刹车
//		vx_set = vx + 0.02f * (vx - vx_set);
//	else
//		vx_set = vx;

//	if (ABS(vy - vy_set) > 100.0f && ((vy < -1000) || (vy > 1000)))   //启动时提速，停止时迅速刹车
//		vy_set = vy + 0.02f * (vy - vy_set);
//	else
//		vy_set = vy;
	
	vw_set = chassisModeNormalCalExec(vw, vw_set);
	vx_set = chassisModeNormalCalExec(vx, vx_set);
	vy_set = chassisModeNormalCalExec(vy, vy_set);
}

/**
  * @brief  正常底盘模式速度设定具体计算函数 
						对vx,vy,vw的相同计算进行了封装，简化代码
	* @param  v:预期速度,v_set:目前速度
	* @retval v_set:返回目前速度以赋值
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
  * @brief  底盘静步模式下的速度设定计算
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
  * @brief  底盘静步模式速度设定具体计算函数 
						对vx,vy,vw的相同计算进行了封装，简化代码
	* @param  v:预期速度,v_set:目前速度
	* @retval v_set:返回目前速度以赋值
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
  * @brief  底盘速度分解及PID计算
  * @param  vx：左右速度
			vy：前后速度
			vw：旋转速度
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
  * @brief  附加旋转偏量
  * @param  speed：旋转速度
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




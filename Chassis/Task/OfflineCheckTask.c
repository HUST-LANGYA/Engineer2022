/**
	*@brief 		掉线检测文件
							!!!!!!!!!只是创建了文件，没有修改不能使用，还没有开启任务!!!!!!!!!!!
	*@author		祝科伟
	*@date			2021-12-19
*/
#include "OfflineCheckTask.h"

uint32_t  Offline_Check_high_water;

int offline_state;

void  offlineCheckTask(void *pvParameters)
{
	
	while(1){
		
//		offline_state = offlineDetect();
		
//		if(g_Flag.soft_reset_flag == 0)
//			IWDG_Feed();
		vTaskDelay(1);
//		#if INCLUDE_uxTaskGetStackHighWaterMark
//        Offline_Check_high_water = uxTaskGetStackHighWaterMark(NULL);
//		#endif
    }
}

/**
  * @brief  线路断线检测
  * @param  None
  * @retval 返回断线id
*/
u8 offlineDetect(void)
{
	g_Flag.offline_flag  = 0;
/**************CAN1断线检测*******************/
	if (g_Flag.can1_connect == 0)
		g_Flag.can1_cnt ++;
	
	if (g_Flag.can1_cnt > 100)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13 );
		g_Flag.offline_flag |= 0x01;
	}
/**************CAN2断线检测*******************/    
    if (g_Flag.can2_connect == 0)
        g_Flag.can2_cnt ++;
    
    if (g_Flag.can2_cnt > 100)
		{
			  GPIO_ResetBits(GPIOC,GPIO_Pin_13 );
			  g_Flag.offline_flag |= 0x02;
		}
		else 
	     GPIO_SetBits(GPIOC,GPIO_Pin_13 );
/**************遥控器断线检测*******************/
//    if (g_Flag.rc_connect == 0)
//        g_Flag.rc_cnt ++;
//    
//    if (g_Flag.rc_cnt > 200)
//	{
//        UART5_Configuration();        //重启遥控器串口
//		return 3;
//	}

/**************陀螺仪断线检测*******************/
	if (g_Flag.gyro_connect == 0)
		g_Flag.gyro_cnt ++;
	
	if (g_Flag.gyro_cnt > 50)
	{
		g_Flag.gyro_use_flag = 0;    //掉线
		g_Flag.offline_flag |= 0x04;
	}
//	else
//		g_Flag.gyro_use_flag = 1;    //数据正常
/**************数据正常*******************/
	return g_Flag.offline_flag;
}





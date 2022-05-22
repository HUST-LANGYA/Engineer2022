/**
	*@brief 		B板发送数据到C板任务
							使用CAN1
	*@author		祝科伟
	*@date			2021-12-19
*/
#include "Master2SlaveTask.h"

uint32_t master_2_slave_high_water;

FlagWithSlave_t Flag2Slave;					//发送到C板的标志位

extern unsigned char Data2C_tx[8]; 	//发送到c板的数据


void master2SlaveTask(void *pvParameters)
{
	
	while(1){
		master2Slave();
		
		vTaskDelay(1);
		
		#if INCLUDE_uxTaskGetStackHighWaterMark
        master_2_slave_high_water = uxTaskGetStackHighWaterMark(NULL);
		#endif
  }
}


void master2Slave(void)
{
	Flag2Slave.flag.clamp_flag = g_Flag.clamp_flag;
	Flag2Slave.flag.exchange_solenoid_flag = g_Flag.exchange_solenoid_flag;
	Flag2Slave.flag.forward_flag = g_Flag.forward_flag;
	Flag2Slave.flag.lift_down_twice_flag = g_Flag.lift_down_twice_flag;
	Flag2Slave.flag.lift_once_flag = g_Flag.lift_once_flag;
	Flag2Slave.flag.midair_solenoid_flag = g_Flag.midair_solenoid_flag;
	
	
//	Data2C_tx[1] = g_Flag.control_mode;
//	Data2C_tx[2] = g_Flag.control_target;
	Data2C_tx[3] = Flag2Slave.data;
	
    //控制模式
	switch(g_Flag.control_mode)
	{
		case RC_MODE:
			Data2C_tx[1] = 0x01;
		break;
		
		case KEY_MODE:
			Data2C_tx[1] = 0X02;
		break;
		
		default:
			Data2C_tx[1] = 0X02;
		break;
//		case POWER_OFF_MODE:
//			Data2C_tx[1] = 0X03;
//		break;
	}		
	
	//运动模式
	switch(g_Flag.control_target)
	{
		case POWER_OFF_MODE:
			Data2C_tx[2] = 0x00;
		break;
		
		case CHASSIS_MODE:
			Data2C_tx[2] = 0x01;
		break;
		
		case CHASSIS_MODE_STATIC:
			Data2C_tx[2] = 0X01;
		break;
		
		case CHASSIS_MODE3:
			Data2C_tx[2] = 0X01;
		break;
		
		case SENIOR_UP_MODE:
			Data2C_tx[2] = 0x02;
		break;
		
		case SENIOR_MODE2:
			Data2C_tx[2] = 0X03;
		break;
		
		case SENIOR_AUTO_MODE:
			Data2C_tx[2] = 0X04;
		break;
		
		default:
			Data2C_tx[2] = 0x01;
		break;
	}
	
	
	can2Master2Slave();
}



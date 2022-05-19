/**
	*@brief 		B�巢�����ݵ�C������
							ʹ��CAN1
	*@author		ף��ΰ
	*@date			2021-12-19
*/
#include "Master2SlaveTask.h"

uint32_t master_2_slave_high_water;

FlagWithSlave_t Flag2Slave;					//���͵�C��ı�־λ

extern unsigned char Data2C_tx[8]; 	//���͵�c�������


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
	
    //����ģʽ
	switch(g_Flag.control_mode)
	{
		case RC_MODE:
			Data2C_tx[1] = 0x01;
		break;
		
		case KEY_MODE:
			Data2C_tx[1] = 0X02;
		break;
		
//		case POWER_OFF_MODE:
//			Data2C_tx[1] = 0X03;
//		break;
	}		
	
	//�˶�ģʽ
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
	}
	

//	//�ϲ�һ��̧����־λ
//	switch(g_Flag.lift_once_flag)
//	{
//		case 0:
//			Data2C_tx[3] = 0;
//		break;
//		
//		case 1:
////			Data2C_tx[3] = Data2C_tx[3] & ((g_Flag.lift_once_flag & 0x01) << 0);
//			Data2C_tx[3] = 1;
//		break;
//		
//		default:
//			Data2C_tx[3] = 0;
//			break;

//	}
//	
//	//ǰ�ƿ��Ʊ�־λ
//	switch(g_Flag.forward_flag)
//	{
//		case 0:
//			Data2C_tx[4] = 0;
//		break;
//		
//		case 1:
//			Data2C_tx[4] = 1;
//		break;
//		
//		default:
//			Data2C_tx[4] = 0;
//			break;
//	}
//	
//	//��ȡ���Ʊ�־λ
//	switch(g_Flag.clamp_flag)
//	{
//		case 0:
//			Data2C_tx[5] = 0;
//		break;
//		
//		case 1:
//			Data2C_tx[5] = 1;
//		break;
//		
//		default:
//			Data2C_tx[5] = 0;
//			break;
//	}
//	
//	//�սӿ��Ʊ�־λ
//	switch(g_Flag.midair_solenoid_flag)
//	{
//		case 0:
//			Data2C_tx[6] = 0;
//		break;
//		
//		case 1:
//			Data2C_tx[6] = 1;
//		break;
//		
//		default:
//			Data2C_tx[6] = 0;
//			break;
//	}
//	
//	//�һ����Ʊ�־λ
//	switch(g_Flag.exchange_solenoid_flag)
//	{
//		case 0:
//			Data2C_tx[7] = 0;
//		break;
//		
//		case 1:
//			Data2C_tx[7] = 1;
//		break;
//		
//		default:
//			Data2C_tx[7] = 0;
//			break;
//	}
	
	can2Master2Slave();
}



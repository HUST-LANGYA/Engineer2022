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
	
	
	Data2C_tx[1] = g_Flag.control_mode;			//����ģʽ
	Data2C_tx[2] = g_Flag.control_target;		//�˶�ģʽ
	Data2C_tx[3] = Flag2Slave.data;					//��־λ
	Data2C_tx[4] = g_Flag.auto_mode;				//�Զ�ģʽ
	Data2C_tx[5] = g_Flag.camera_pitch;			//ͼ����̧ͷ

	can2Master2Slave();
	
}



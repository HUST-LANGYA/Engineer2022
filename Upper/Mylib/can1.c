#include "main.h"

int can = 0;

unsigned char Data_Send_to_F105[8];//׼�����͸�B�������
unsigned char data_receive[8] = {0,0,0,0,0,0,0,0},data_receive_pre[8] = {0,0,0,0,0,0,0,0}; //C����յ�����

float yaw_angle;
float gz;
float pitch_angle;
float g_pitch;
FlagWithMaster_t Flag2Master;			//���͸�B���־λ
FlagWithMaster_t FlagOfMaster;		//���յ���B���־λ
//unsigned char Laser_Left,Laser_Right,Laser_Mid;


/**
  * @brief  ����CAN1
  * @param  None
  * @retval None
  */
void CAN1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/* ��GPIOʱ�ӡ�AFIOʱ�ӣ�CANʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);


	/* CAN1 RX PB8 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* CAN1 TX PB9 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);  // CAN1 remap

	/* CAN1 Enabling interrupt */	
	NVIC_Config(CAN1_RX0_IRQn, 0, 0);			
	NVIC_Config(CAN1_RX1_IRQn, 0, 1);	
	NVIC_Config(CAN1_TX_IRQn, 1, 0);
	
	/* CAN  BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);   

	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=ENABLE;
	CAN_InitStructure.CAN_AWUM=ENABLE;
	CAN_InitStructure.CAN_NART=ENABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=ENABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;   
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;  
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;	
	CAN_InitStructure.CAN_Prescaler=4;

	CAN_Init(CAN1,&CAN_InitStructure);	// CAN1											


	CAN_FilterInitStructure.CAN_FilterNumber=0;	 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // ��ʶ������λģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 32λ������
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x401 <<5;			// ��������ʶ��
	CAN_FilterInitStructure.CAN_FilterIdLow=0x000 <<5;				
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x000 <<5;		// ���������α�ʶ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x000 <<5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0ָ�������
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber=1;	 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // ��ʶ������λģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 32λ������
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x100 <<5;			// ��������ʶ��
	CAN_FilterInitStructure.CAN_FilterIdLow=0x101 <<5;				
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x000 <<5;		// ���������α�ʶ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x000 <<5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO1;	 // FIFO0ָ�������
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	

    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
		CAN_ITConfig(CAN1,CAN_IT_FMP1,ENABLE);
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);
}


/**
  * @brief  CAN1�����ж�(���̧�����ID��Ϊ5���Ҳ�̧�����ID��Ϊ6) FIFO0
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg rx_message;
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
	{
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
		switch(rx_message.StdId)
		{
			//����B������
			case 0x401:
				data_receive[0] = rx_message.Data[0];//'!'
				data_receive[1] = rx_message.Data[1];//����ģʽ
				data_receive[2] = rx_message.Data[2];//�˶�ģʽ
				data_receive[3] = rx_message.Data[3];//��־λ
				data_receive[4] = rx_message.Data[4];//
				data_receive[5] = rx_message.Data[5];//
				data_receive[6] = rx_message.Data[6];
				data_receive[7] = rx_message.Data[7];
			
			
				FlagOfMaster.data = data_receive[3];
				break;
			
		
			default:
				break;
		}
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	}
}


/**
  * @brief  CAN1�����ж�(���մ�B�巢�������ݺ�צ������ת�������) FIFO1
  * @param  None
  * @retval None
  */
void CAN1_RX1_IRQHandler(void)
{
	CanRxMsg rx_message;
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP1)!= RESET) 
	{
		CAN_Receive(CAN1, CAN_FIFO1, &rx_message);
		switch(rx_message.StdId)
		{
			case 0x100:
				memcpy(&pitch_angle,&rx_message.Data,4);
				memcpy(&g_pitch,&(rx_message.Data[4]),4);
//				if(g_Flag.gyro_initial_flag == 0)
//				{
//					g_Flag.gyro_initial_flag = 1;
//					chassis_pos_follow_pid.SetPoint = yaw_angle;
//				}
				break;
			
			case 0x101:
				memcpy(&yaw_angle,&rx_message.Data,4);
				memcpy(&gz,&(rx_message.Data[4]),4);
//				if(g_Flag.gyro_initial_flag == 0)
//				{
//					g_Flag.gyro_initial_flag = 1;
//					chassis_pos_follow_pid.SetPoint = yaw_angle;
//				}
				break;
			
			default:
				break;
		}
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP1);
	}
}


/**
  * @brief  CAN1�����ж�
  * @param  None
  * @retval None
  */
void CAN1_TX_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
	}
}

/**
  * @brief  CAN1��������,�ڴ������޷� -16384 ~ 0 ~ 16384
  * @param  a��0x205��������
            b��0x206��������
            c��0x207��������
            d��0x208��������
  * @retval None
  */
void Slave2Master(void)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x301;
		
		Flag2Master.flag.clamp_flag = g_Flag.clamp_solenoid_flag;
		Flag2Master.flag.exchange_solenoid_flag = g_Flag.exchange_solenoid_flag;
		Flag2Master.flag.forward_flag = g_Flag.forward_solenoid_flag;
		Flag2Master.flag.lift_down_twice_flag = g_Flag.lift_twice_flag;
		Flag2Master.flag.lift_once_flag = g_Flag.lift_once_flag;
		Flag2Master.flag.midair_solenoid_flag = g_Flag.midair_solenoid_flag;
		
		Data_Send_to_F105[0] = '!';
		Data_Send_to_F105[1] = Flag2Master.data;
	
		Data_Send_to_F105[2] = g_Flag.auto_end_flag;
		g_Flag.auto_end_flag = 0;
	
	
    tx_message.Data[0] = Data_Send_to_F105[0];
    tx_message.Data[1] = Data_Send_to_F105[1];		//
    tx_message.Data[2] = Data_Send_to_F105[2];
    tx_message.Data[3] = Data_Send_to_F105[3];
    tx_message.Data[4] = Data_Send_to_F105[4];
    tx_message.Data[5] = Data_Send_to_F105[5];
    tx_message.Data[6] = Data_Send_to_F105[6];
    tx_message.Data[7] = Data_Send_to_F105[7];

	
    CAN_Transmit(CAN1, &tx_message);
}

/**
  * @brief  C�嵽B�巢�����ݳ�ʼ��
  * @param  None
  * @retval None
  */
void Data_Send_to_Master_Init(void)
{
	Data_Send_to_F105[0] = '!';
	Data_Send_to_F105[1] = 0;				//���͸�B��ı�־λ
	Data_Send_to_F105[2] = 0;
	Data_Send_to_F105[3] = 0;		
	Data_Send_to_F105[4] = 0;
	Data_Send_to_F105[5] = 0;		
	Data_Send_to_F105[6] = 0;
	Data_Send_to_F105[7] = 0;		
	

}

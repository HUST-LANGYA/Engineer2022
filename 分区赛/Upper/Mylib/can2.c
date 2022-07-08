/**
	*@brief 		CAN2����
							���ڽ���C������
	*@author		
	*@date			
*/
/************************************************************************************
CAN2��C��ͨѶ�Լ����������ݽ��գ�ID�����£�

 B�����ݽ���c�壺0x301,  B����C�巢�����ݣ�0x401
 ���������ݽ��գ�0x101
***************************************************************************************/

#include "can2.h"
/*NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4)*/



rmc620_t Track_Motor[2], Rotate_Motor;

/**
  * @brief  ����CAN2
  * @param  None
  * @retval None
  */
void can2Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/* ��GPIOʱ�ӡ�AFIOʱ�ӣ�CANʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);


	/* CAN2 RX PB12 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* CAN2 TX PB13 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

//	GPIO_PinRemapConfig(GPIO_Remap_CAN2,ENABLE);  // CAN2 remap

	/* CAN2 Enabling interrupt */	
	NVIC_Config(CAN2_RX0_IRQn, 2, 0);
	NVIC_Config(CAN2_RX1_IRQn, 2, 1);
	NVIC_Config(CAN2_TX_IRQn, 2, 2);
	
	/* CAN  BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler */
	CAN_DeInit(CAN2);
	CAN_StructInit(&CAN_InitStructure);   

	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=ENABLE;
	CAN_InitStructure.CAN_AWUM=ENABLE;
	CAN_InitStructure.CAN_NART=ENABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=ENABLE;
//	CAN_InitStructure.CAN_ABOM=DISABLE;
//	CAN_InitStructure.CAN_AWUM=DISABLE;
//	CAN_InitStructure.CAN_NART=ENABLE;
//	CAN_InitStructure.CAN_RFLM=DISABLE;
//	CAN_InitStructure.CAN_TXFP=DISABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;   
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;  
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;	
	CAN_InitStructure.CAN_Prescaler=4;

	CAN_Init(CAN2,&CAN_InitStructure);	// CAN2										


	CAN_FilterInitStructure.CAN_FilterNumber=14;	 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // ��ʶ������λģʽ
//	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32λ������
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 32λ������
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x201 << 5;			// ��������ʶ��
	CAN_FilterInitStructure.CAN_FilterIdLow=0x202 << 5;				
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x203 << 5;		// ���������α�ʶ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x205 << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0ָ�������
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
//	CAN_FilterInitStructure.CAN_FilterNumber=14;	 
//	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // ��ʶ������λģʽ
//	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 32λ������
//	CAN_FilterInitStructure.CAN_FilterIdHigh=0x201 << 5;			// ��������ʶ��
//	CAN_FilterInitStructure.CAN_FilterIdLow=0x202 << 5;				
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x203 << 5;		// ���������α�ʶ��
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x204 << 5;
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0ָ�������
//	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
//	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber=15;	// 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // ��ʶ������λģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 32λ������
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x100 << 5;			// ��������ʶ��
	CAN_FilterInitStructure.CAN_FilterIdLow=0x101 <<5;				
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x000 <<5;		
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x000 <<5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO1;	 // FIFO1ָ�������
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);
	CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);
}

/**
  * @brief  CAN2�����ж�, FIFO0
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg rx_message;
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET) 
	{
		CAN_Receive(CAN2, CAN_FIFO0, &rx_message);
		
		switch(rx_message.StdId)
		{
			
			//��̧�����
			case 0x201:
				Track_Motor[0].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
				Track_Motor[0].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
				Track_Motor[0].flow = (rx_message.Data[4] << 8) | rx_message.Data[5];

				break;
			//��̧�����
			case 0x203:
				Track_Motor[1].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
				Track_Motor[1].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
				Track_Motor[1].flow = (rx_message.Data[4] << 8) | rx_message.Data[5];

				break;
			
			//צ����ת�������
			case 0x205:
				Rotate_Motor.angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
				Rotate_Motor.speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
				break;
				
			default:
				break;
		}

		
		CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);
	}
}	

/**
  * @brief  CAN2�����ж�, FIFO1
  * @param  None
  * @retval None
  */
void CAN2_RX1_IRQHandler(void)
{
	CanRxMsg rx_message;
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP1)!= RESET) 
	{
		CAN_Receive(CAN2, CAN_FIFO1, &rx_message);
		
		switch(rx_message.StdId)
		{
//			case 0x100:
//				memcpy(&yaw_angle,&rx_message.Data,4);
//				memcpy(&gz,&(rx_message.Data[4]),4);
//				if(g_Flag.gyro_initial_flag == 0)
//				{
//					g_Flag.gyro_initial_flag = 1;
//					chassis_pos_follow_pid.SetPoint = yaw_angle;
//				}
//				break;
//			case 0x101:
//				memcpy(&yaw_angle,&rx_message.Data,4);
//				memcpy(&gz,&(rx_message.Data[4]),4);
//				if(g_Flag.gyro_initial_flag == 0)
//				{
//					g_Flag.gyro_initial_flag = 1;
//					chassis_pos_follow_pid.SetPoint = yaw_angle;
//				}
//				break;
				
			default:
				break;
		}

		
		CAN_ClearITPendingBit(CAN2,CAN_IT_FMP1);
	}
}	







/**
  * @brief  CAN2�����ж�
  * @param  None
  * @retval None
  */
void CAN2_TX_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
	}
}


/**
  * @brief  CAN1��������1,�ڴ������޷� -16384 ~ 0 ~ 16384
  * @param  a��0x201��������
            b��0x202��������
            c��0x203��������
            d��0x204��������
  * @retval None
  */
CanTxMsg tx_message_t;
void motorCurrentSend(int a, int b, int c, int d)
{
    tx_message_t.IDE = CAN_ID_STD;    
    tx_message_t.RTR = CAN_RTR_DATA; 
    tx_message_t.DLC = 0x08;    
    tx_message_t.StdId = 0x200;
	
    a = LIMIT_MAX_MIN(a, 15000, -15000);
    b = LIMIT_MAX_MIN(b, 15000, -15000);
    c = LIMIT_MAX_MIN(c, 15000, -15000);
    d = LIMIT_MAX_MIN(d, 15000, -15000);
	
    tx_message_t.Data[0] = (unsigned char)((a >> 8) & 0xff);
    tx_message_t.Data[1] = (unsigned char)(a & 0xff);  
    tx_message_t.Data[2] = (unsigned char)((b >> 8) & 0xff);
    tx_message_t.Data[3] = (unsigned char)(b & 0xff);
    tx_message_t.Data[4] = (unsigned char)((c >> 8) & 0xff);
    tx_message_t.Data[5] = (unsigned char)(c & 0xff);
    tx_message_t.Data[6] = (unsigned char)((d >> 8) & 0xff); 
    tx_message_t.Data[7] = (unsigned char)(d & 0xff);

    CAN_Transmit(CAN2, &tx_message_t);
}


/**
  * @brief  CAN1��������2,�ڴ������޷� -16384 ~ 0 ~ 16384
  * @param  a��0x205��������
            b��0x206��������
            c��0x207��������
            d��0x208��������
  * @retval None
  */
void motorCurrentSend2(int a, int b, int c, int d)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x1FF;
	
    a = LIMIT_MAX_MIN(a, 15000, -15000);
    b = LIMIT_MAX_MIN(b, 15000, -15000);
    c = LIMIT_MAX_MIN(c, 15000, -15000);
    d = LIMIT_MAX_MIN(d, 15000, -15000);
	
    tx_message.Data[0] = (unsigned char)((a >> 8) & 0xff);
    tx_message.Data[1] = (unsigned char)(a & 0xff);  
    tx_message.Data[2] = (unsigned char)((b >> 8) & 0xff);
    tx_message.Data[3] = (unsigned char)(b & 0xff);
    tx_message.Data[4] = (unsigned char)((c >> 8) & 0xff);
    tx_message.Data[5] = (unsigned char)(c & 0xff);
    tx_message.Data[6] = (unsigned char)((d >> 8) & 0xff); 
    tx_message.Data[7] = (unsigned char)(d & 0xff);

    CAN_Transmit(CAN2, &tx_message);
}


///**
//  * @brief  CAN1�������� ��B����C�壨Master -> Slave��
//  * @param  a��
//            b��
//            c��
//            d��
//  * @retval None
//  */
//void can2Master2Slave(void)
//{
//    CanTxMsg tx_message;
//    tx_message.IDE = CAN_ID_STD;    
//    tx_message.RTR = CAN_RTR_DATA; 
//    tx_message.DLC = 0x08;    
//    tx_message.StdId = 0x406;
//	
//		Data2C_tx[0] = '!';

//    tx_message.Data[0] = Data2C_tx[0];
//    tx_message.Data[1] = Data2C_tx[1];
//    tx_message.Data[2] = Data2C_tx[2];
//    tx_message.Data[3] = Data2C_tx[3];
//    tx_message.Data[4] = Data2C_tx[4];
//    tx_message.Data[5] = Data2C_tx[5];
//    tx_message.Data[6] = Data2C_tx[6];
//    tx_message.Data[7] = Data2C_tx[7];

//    CAN_Transmit(CAN2, &tx_message);
//}

///**
//  * @brief  B�嵽C�巢�����ݳ�ʼ��
//  * @param  None
//  * @retval None
//  */
//void Data_Send_to_Slave_Init(void)
//{
//	Data2C_tx[0] = '!';
//	Data2C_tx[1] = 0;//����ģʽ	�궨��
//	Data2C_tx[2] = 0;//�˶�ģʽ	�궨��
//	Data2C_tx[3] = 0;//���͸�C��ı�־λ
//	Data2C_tx[4] = 0;//�Զ����Ʊ�־λ
//	Data2C_tx[5] = 0;//
//	Data2C_tx[6] = 0;
//	Data2C_tx[7] = 0;
//}



///**
//  * @brief  ��ȡYAW��Ƕ�
//  * @param  None
//  * @retval None
//  */
//float getYawAngle(void)
//{
//	return yaw_angle;
//}

///**
//  * @brief  ��ȡYAW����ٶ�
//  * @param  None
//  * @retval None
//  */
//float getGz(void)
//{
//	return gz;
//}



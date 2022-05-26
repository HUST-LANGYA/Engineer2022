/**
	*@brief 		CAN2配置
							用于接受C板数据
	*@author		
	*@date			
*/
/************************************************************************************
CAN2与C板通讯以及陀螺仪数据接收，ID号如下：

 B板数据接收c板：0x301,  B板向C板发送数据：0x401
 陀螺仪数据接收：0x101
***************************************************************************************/

#include "can2.h"
/*NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4)*/


//rmc620_t forklift_motor[2];

int lift_switch_flag;


float yaw_angle;
float gz;
unsigned char Data_Receive_from_F103[8];//Data_Receive存的是接收到的从C板传来的数据
unsigned char Data2C_tx[8]; //发送到c板的数据
FlagWithSlave_t FlagOfSlave;		//接收到的C板标志位
//unsigned char Laser_Left,Laser_Right,Laser_Mid;

extern pid_Typedef chassis_pos_follow_pid;


//rmc620_t chassis_motor[4];

/**
  * @brief  配置CAN2
  * @param  None
  * @retval None
  */
void can2Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/* 打开GPIO时钟、AFIO时钟，CAN时钟 */
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
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // 标识符屏蔽位模式
//	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x301 << 5;			// 过滤器标识符
	CAN_FilterInitStructure.CAN_FilterIdLow=0x000 << 5;				
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x000 << 5;		// 过滤器屏蔽标识符
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x000 << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0指向过滤器
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
//	CAN_FilterInitStructure.CAN_FilterNumber=14;	 
//	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // 标识符屏蔽位模式
//	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 32位过滤器
//	CAN_FilterInitStructure.CAN_FilterIdHigh=0x201 << 5;			// 过滤器标识符
//	CAN_FilterInitStructure.CAN_FilterIdLow=0x202 << 5;				
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x203 << 5;		// 过滤器屏蔽标识符
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x204 << 5;
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0指向过滤器
//	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
//	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber=15;	// 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // 标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x101 << 5;			// 过滤器标识符
	CAN_FilterInitStructure.CAN_FilterIdLow=0x000 <<5;				
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x000 <<5;		
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x000 <<5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO1;	 // FIFO1指向过滤器
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);
	CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);
}

/**
  * @brief  CAN2接收中断, FIFO0
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
			
			case 0x301:
				
				Data_Receive_from_F103[0] = rx_message.Data[0];//'!'
				Data_Receive_from_F103[1] = rx_message.Data[1];//
				Data_Receive_from_F103[2] = rx_message.Data[2];//
				Data_Receive_from_F103[3] = rx_message.Data[3];//
				Data_Receive_from_F103[4] = rx_message.Data[4];//
				Data_Receive_from_F103[5] = rx_message.Data[5];//
				Data_Receive_from_F103[6] = rx_message.Data[6];
				Data_Receive_from_F103[7] = rx_message.Data[7];
				
				FlagOfSlave.data = Data_Receive_from_F103[1];
				break;
					
				
			default:
				break;
		}

		
		CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);
	}
}	

/**
  * @brief  CAN2接收中断, FIFO1
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
			case 0x101:
				memcpy(&yaw_angle,&rx_message.Data,4);
				memcpy(&gz,&(rx_message.Data[4]),4);
				if(g_Flag.gyro_initial_flag == 0)
				{
					g_Flag.gyro_initial_flag = 1;
					chassis_pos_follow_pid.SetPoint = yaw_angle;
				}
				break;
				
			default:
				break;
		}

		
		CAN_ClearITPendingBit(CAN2,CAN_IT_FMP1);
	}
}	


///**
//  * @brief  CAN2接收中断，FIFO1
//  * @param  None
//  * @retval None
//  */
//CanRxMsg rx_message_1;	
//void CAN2_RX1_IRQHandler(void)
//{
//	if(CAN_GetITStatus(CAN2, CAN_IT_FMP1) != RESET) 
//	{
//    CAN_Receive(CAN2, CAN_FIFO1, &rx_message_1);	

//				
//		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
//	}
//}




/**
  * @brief  CAN2发送中断
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
  * @brief  CAN1发送中断
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
  * @brief  CAN1发送数据 从B板向C板（Master -> Slave）
  * @param  a：
            b：
            c：
            d：
  * @retval None
  */
void can2Master2Slave(void)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x406;
	
		Data2C_tx[0] = '!';

    tx_message.Data[0] = Data2C_tx[0];
    tx_message.Data[1] = Data2C_tx[1];
    tx_message.Data[2] = Data2C_tx[2];
    tx_message.Data[3] = Data2C_tx[3];
    tx_message.Data[4] = Data2C_tx[4];
    tx_message.Data[5] = Data2C_tx[5];
    tx_message.Data[6] = Data2C_tx[6];
    tx_message.Data[7] = Data2C_tx[7];

    CAN_Transmit(CAN2, &tx_message);
}

/**
  * @brief  B板到C板发送数据初始化
  * @param  None
  * @retval None
  */
void Data_Send_to_Slave_Init(void)
{
	Data2C_tx[0] = '!';
	Data2C_tx[1] = 0;//控制模式	宏定义
	Data2C_tx[2] = 0;//运动模式	宏定义
	Data2C_tx[3] = 0;//发送给C板的标志位
	Data2C_tx[4] = 0;//自动控制标志位
	Data2C_tx[5] = 0;//
	Data2C_tx[6] = 0;
	Data2C_tx[7] = 0;
}



/**
  * @brief  获取YAW轴角度
  * @param  None
  * @retval None
  */
float getYawAngle(void)
{
	return yaw_angle;
}

/**
  * @brief  获取YAW轴角速度
  * @param  None
  * @retval None
  */
float getGz(void)
{
	return gz;
}



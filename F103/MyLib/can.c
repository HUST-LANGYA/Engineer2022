#include "main.h"

int can = 0;
//rmc620_t Track_Motor[2] , forward_motor[2];
rmc620_t Track_Motor[2], Rotate_Motor;

unsigned char Data_Send_to_F105[8];//准备发送给B板的数据
unsigned char data_receive[8]; //C板接收的数据

FlagWithMaster_t Flag2Master;			//发送给B板标志位
FlagWithMaster_t FlagOfMaster;		//接收到的B板标志位
//unsigned char Laser_Left,Laser_Right,Laser_Mid;


/**
  * @brief  配置CAN1
  * @param  None
  * @retval None
  */
void CAN1_Config(void)
{
	GPIO_InitTypeDef 				GPIO_InitStructure;
	CAN_InitTypeDef        	CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;

	/* 打开GPIO时钟、AFIO时钟，CAN时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/* CAN1 RX PA11 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* CAN1 TX PA12 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* CAN1 Enabling interrupt */	
	NVIC_Config(USB_LP_CAN1_RX0_IRQn, 0, 0);		
	NVIC_Config(CAN1_RX1_IRQn, 0, 1);		
	NVIC_Config(USB_HP_CAN1_TX_IRQn, 1, 0);
	
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
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // 标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x201 <<5;			// 过滤器标识符
	CAN_FilterInitStructure.CAN_FilterIdLow=0x202<<5;				
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x203<<5;		// 过滤器屏蔽标识符
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x204<<5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0指向过滤器
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);


	CAN_FilterInitStructure.CAN_FilterNumber=1;	 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // 标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x406<<5;			// 过滤器标识符
	CAN_FilterInitStructure.CAN_FilterIdLow=0x000<<5;				
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x000<<5;		// 过滤器屏蔽标识符
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x000<<5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO1;	 // FIFO0指向过滤器
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);


	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN1,CAN_IT_FMP1,ENABLE);
	CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);
}


CanRxMsg rx_message;


/**
  * @brief  CAN1接收中断(左侧抬升电机ID号为5，右侧抬升电机ID号为6) FIFO0
  * @param  None
  * @retval None
  */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
//	CanRxMsg rx_message;
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
	{
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
		switch(rx_message.StdId)
		{
			//右抬升电机
			case 0x201:
				Track_Motor[0].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
				Track_Motor[0].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
				Track_Motor[0].flow = (rx_message.Data[4] << 8) | rx_message.Data[5];

				break;
			//左抬升电机
			case 0x203:
				Track_Motor[1].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
				Track_Motor[1].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
				Track_Motor[1].flow = (rx_message.Data[4] << 8) | rx_message.Data[5];

				break;
			
			//爪子旋转电机数据
			case 0x204:
				Rotate_Motor.angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
				Rotate_Motor.speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
				break;
			
//			//右前移电机
//			case 0x201:
//				forward_motor[0].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
//				forward_motor[0].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
//				break;
//			//左前移电机·
//			case 0x202:
//				forward_motor[1].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
//				forward_motor[1].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
//				break;
		
			default:
				break;
		}
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	}
}


/**
  * @brief  CAN1接收中断(接收从B板发来的数据和爪子上旋转电机数据) FIFO1
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
			//接受B板数据
			case 0x406:
				data_receive[0] = rx_message.Data[0];//'!'
				data_receive[1] = rx_message.Data[1];//控制模式
				data_receive[2] = rx_message.Data[2];//运动模式
				data_receive[3] = rx_message.Data[3];//标志位
				data_receive[4] = rx_message.Data[4];//
				data_receive[5] = rx_message.Data[5];//
				data_receive[6] = rx_message.Data[6];
				data_receive[7] = rx_message.Data[7];
			
			
				FlagOfMaster.data = data_receive[3];
				break;
			
			default:
				break;
		}
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP1);
	}
}

/**
  * @brief  CAN1发送中断
  * @param  None
  * @retval None
  */
void USB_HP_CAN1_TX_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
	}
}

/**
  * @brief  CAN1发送数据1,内带电流限幅 -16384 ~ 0 ~ 16384
  * @param  a：0x201电流给定
            b：0x202电流给定
            c：0x203电流给定
            d：0x204电流给定
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

    CAN_Transmit(CAN1, &tx_message_t);
}


/**
  * @brief  CAN1发送数据2,内带电流限幅 -16384 ~ 0 ~ 16384
  * @param  a：0x205电流给定
            b：0x206电流给定
            c：0x207电流给定
            d：0x208电流给定
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

    CAN_Transmit(CAN1, &tx_message);
}


/**
  * @brief  CAN1发送数据,内带电流限幅 -16384 ~ 0 ~ 16384
  * @param  a：0x205电流给定
            b：0x206电流给定
            c：0x207电流给定
            d：0x208电流给定
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
  * @brief  C板到B板发送数据初始化
  * @param  None
  * @retval None
  */
void Data_Send_to_Master_Init(void)
{
	Data_Send_to_F105[0] = '!';
	Data_Send_to_F105[1] = 0;				//发送给B板的标志位
	Data_Send_to_F105[2] = 0;
	Data_Send_to_F105[3] = 0;		
	Data_Send_to_F105[4] = 0;
	Data_Send_to_F105[5] = 0;		
	Data_Send_to_F105[6] = 0;
	Data_Send_to_F105[7] = 0;		
	

}

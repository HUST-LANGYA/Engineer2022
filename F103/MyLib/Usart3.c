/**
	*@brief 		usart3
							与角度陀螺仪通讯接受数据
	*@author		祝科伟
	*@date			2021-12-11
*/


#include "Usart3.h"
//角度陀螺仪数据
unsigned char ClampAngleReceiveBuffer[15];
unsigned char ClampAngleSend[15];
DMA_InitTypeDef 	USART3_TX_dma;
float ClampAngle_X;

int data_length=0;
uint8_t angle_receive[15];

/**
 * @brief  usart3初始化
 * @param  None
 * @retval None
 */
void USART3_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//RX,PB11
	gpio.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);

	//TX,PB10
	gpio.GPIO_Pin = GPIO_Pin_10;  
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);

	USART_DeInit(USART3);
//	usart.USART_BaudRate = 9600;
	usart.USART_BaudRate = 9600;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No ;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_Init(USART3,&usart);
	
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);   //开启串口闲时中断  	
	nvic.NVIC_IRQChannel = USART3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&nvic);
	
	
	USART_Cmd(USART3,ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE); 
	
	//RX
	nvic.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	{
		DMA_InitTypeDef  dma;
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
		DMA_DeInit(DMA1_Channel3);

		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
		dma.DMA_MemoryBaseAddr = (uint32_t)ClampAngleReceiveBuffer;
		dma.DMA_DIR = DMA_DIR_PeripheralSRC;
		dma.DMA_BufferSize = ClampAngleReceiveSize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_M2M = DMA_M2M_Disable;
		
		DMA_Init(DMA1_Channel3, &dma);
		DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Channel3, ENABLE);
	}
  
	
	//TX
		USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	
    nvic.NVIC_IRQChannel = DMA1_Channel2_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 1;
		nvic.NVIC_IRQChannelSubPriority = 1;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
		{
//		  DMA_InitTypeDef 	dma;
//			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//			DMA_DeInit(DMA1_Channel2);
//			dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
//			dma.DMA_MemoryBaseAddr = (uint32_t)ClampAngleSend;
//			dma.DMA_DIR = DMA_DIR_PeripheralDST;
//			dma.DMA_BufferSize = ClampAngleSendSize;
//			dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//			dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
//			dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//			dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//			dma.DMA_Mode = DMA_Mode_Normal;
//			dma.DMA_Priority = DMA_Priority_VeryHigh;
//			dma.DMA_M2M = DMA_M2M_Disable;

//			DMA_Init(DMA1_Channel2,&dma);
//			DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);
//			DMA_Cmd(DMA1_Channel2,DISABLE);
			
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
			DMA_DeInit(DMA1_Channel2);
			USART3_TX_dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
			USART3_TX_dma.DMA_MemoryBaseAddr = (uint32_t)ClampAngleSend;
			USART3_TX_dma.DMA_DIR = DMA_DIR_PeripheralDST;
			USART3_TX_dma.DMA_BufferSize = ClampAngleSendSize;
			USART3_TX_dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			USART3_TX_dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
			USART3_TX_dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			USART3_TX_dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			USART3_TX_dma.DMA_Mode = DMA_Mode_Normal;
			USART3_TX_dma.DMA_Priority = DMA_Priority_VeryHigh;
			USART3_TX_dma.DMA_M2M = DMA_M2M_Disable;

			DMA_Init(DMA1_Channel2,&USART3_TX_dma);
			DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);
			DMA_Cmd(DMA1_Channel2,DISABLE);
	 }		
		
}

/**
 * @brief  usart3中断
 * @param  None
 * @retval None
 */
void USART3_IRQHandler(void)
{
	int i = 0;
	if(USART_GetITStatus(USART3,USART_IT_IDLE)!=RESET)
	{
		(void)USART3->SR;
		(void)USART3->DR;	   //清除USART3的空闲中断标志位
		DMA_Cmd(DMA1_Channel3,DISABLE);
		data_length = ClampAngleReceiveSize - DMA_GetCurrDataCounter(DMA1_Channel5);
		for( i = 0; i < data_length; i++ )
		{
			angle_receive[i] = ClampAngleReceiveBuffer[i];
		}
		
		if(Verify_modbusCRC16_Check_Sum(angle_receive,data_length))
		{
			ClampAngle_X = ((float)((angle_receive[3]<<8)|angle_receive[4]))/32768*180;
		}
		
		DMA_SetCurrDataCounter(DMA1_Channel3,ClampAngleReceiveSize);
		DMA_Cmd(DMA1_Channel3,ENABLE);
	}
}


/**
 * @brief  usart3_RX的DMA中断
 * @param  None
 * @retval None
 */
void DMA1_Channel3_IRQHandler(void)
{	
	if(DMA_GetFlagStatus(DMA1_FLAG_TC3) == SET)
	{
		DMA_ClearFlag(DMA1_FLAG_TC3);
	}
}

/**
 * @brief  usart3_TX的DMA中断
 * @param  None
 * @retval None
 */
void DMA1_Channel2_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC2)!=RESET)
	{
		DMA_ClearFlag(DMA1_FLAG_TC2);
		DMA_ClearITPendingBit(DMA1_IT_TC2);
		DMA_Cmd(DMA1_Channel2,DISABLE);
	}	
}


void changeBaudRate(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//RX,PB11
	gpio.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);

	//TX,PB10
	gpio.GPIO_Pin = GPIO_Pin_10;  
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);

	USART_DeInit(USART3);
	usart.USART_BaudRate = 921600;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No ;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_Init(USART3,&usart);
	
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);   //开启串口闲时中断  	
	nvic.NVIC_IRQChannel = USART3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&nvic);
	
	
	USART_Cmd(USART3,ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE); 
	
	//RX
	nvic.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	{
		DMA_InitTypeDef  dma;
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
		DMA_DeInit(DMA1_Channel3);

		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
		dma.DMA_MemoryBaseAddr = (uint32_t)ClampAngleReceiveBuffer;
		dma.DMA_DIR = DMA_DIR_PeripheralSRC;
		dma.DMA_BufferSize = ClampAngleReceiveSize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_M2M = DMA_M2M_Disable;
		
		DMA_Init(DMA1_Channel3, &dma);
		DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Channel3, ENABLE);
	}
  
	
	//TX
		USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	
    nvic.NVIC_IRQChannel = DMA1_Channel2_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 1;
		nvic.NVIC_IRQChannelSubPriority = 1;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
		{
//		  DMA_InitTypeDef 	dma;
//			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//			DMA_DeInit(DMA1_Channel2);
//			dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
//			dma.DMA_MemoryBaseAddr = (uint32_t)ClampAngleSend;
//			dma.DMA_DIR = DMA_DIR_PeripheralDST;
//			dma.DMA_BufferSize = ClampAngleSendSize;
//			dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//			dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
//			dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//			dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//			dma.DMA_Mode = DMA_Mode_Normal;
//			dma.DMA_Priority = DMA_Priority_VeryHigh;
//			dma.DMA_M2M = DMA_M2M_Disable;

//			DMA_Init(DMA1_Channel2,&dma);
//			DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);
//			DMA_Cmd(DMA1_Channel2,DISABLE);
			
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
			DMA_DeInit(DMA1_Channel2);
			USART3_TX_dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
			USART3_TX_dma.DMA_MemoryBaseAddr = (uint32_t)ClampAngleSend;
			USART3_TX_dma.DMA_DIR = DMA_DIR_PeripheralDST;
			USART3_TX_dma.DMA_BufferSize = ClampAngleSendSize;
			USART3_TX_dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			USART3_TX_dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
			USART3_TX_dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			USART3_TX_dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			USART3_TX_dma.DMA_Mode = DMA_Mode_Normal;
			USART3_TX_dma.DMA_Priority = DMA_Priority_VeryHigh;
			USART3_TX_dma.DMA_M2M = DMA_M2M_Disable;

			DMA_Init(DMA1_Channel2,&USART3_TX_dma);
			DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);
			DMA_Cmd(DMA1_Channel2,DISABLE);
	 }		
}


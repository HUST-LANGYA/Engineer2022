#include "uart4.h"
//角度陀螺仪数据(已经改为使用C板控制，该串口处于闲置状态)
//unsigned char JudgeReceiveBuffer[JudgeBufBiggestSize];
//unsigned char JudgeSend[SEND_MAX_SIZE];
//unsigned char SaveBuffer[2*JudgeBufBiggestSize];
unsigned char LaserRangingReceiveBuffer[2*LaserRangingReceiveSize];
unsigned char LaserRangingSend[2*LaserRangingSendSize];
int LaserRanging;
//int LaserRanging_test;


int data_length=0;
uint8_t angle_receive[2*LaserRangingReceiveSize];

/**
 * @brief  uart4初始化
 * @param  None
 * @retval None
 */
void UART4_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	//RX
	gpio.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);

	//TX
	gpio.GPIO_Pin = GPIO_Pin_10;  
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);

	USART_DeInit(UART4);
	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No ;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_Init(UART4,&usart);
	
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);   //开启串口闲时中断  	
	nvic.NVIC_IRQChannel = UART4_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&nvic);
	
	
	USART_Cmd(UART4,ENABLE);
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE); 
	
	//RX
	nvic.NVIC_IRQChannel = DMA2_Channel3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	{
		DMA_InitTypeDef  dma;
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
		
		DMA_DeInit(DMA2_Channel3);

		dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
		dma.DMA_MemoryBaseAddr = (uint32_t)LaserRangingReceiveBuffer;
		dma.DMA_DIR = DMA_DIR_PeripheralSRC;
		dma.DMA_BufferSize = LaserRangingReceiveSize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_M2M = DMA_M2M_Disable;
		
		DMA_Init(DMA2_Channel3, &dma);
		DMA_ITConfig(DMA2_Channel3,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA2_Channel3, ENABLE);
	}
  
	
	//TX
		USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
	
    nvic.NVIC_IRQChannel = DMA2_Channel5_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 1;
		nvic.NVIC_IRQChannelSubPriority = 1;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
		{
		  DMA_InitTypeDef 	dma;
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
			DMA_DeInit(DMA2_Channel5);
			dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
			dma.DMA_MemoryBaseAddr = (uint32_t)LaserRangingSend;
			dma.DMA_DIR = DMA_DIR_PeripheralDST;
			dma.DMA_BufferSize = LaserRangingSendSize;
			dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
			dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			dma.DMA_Mode = DMA_Mode_Normal;
			dma.DMA_Priority = DMA_Priority_VeryHigh;
			dma.DMA_M2M = DMA_M2M_Disable;

			DMA_Init(DMA2_Channel5,&dma);
			DMA_ITConfig(DMA2_Channel5,DMA_IT_TC,ENABLE);
			DMA_Cmd(DMA2_Channel5,ENABLE);
	 }		
		
}
int test_11 = 1;
void UART4_IRQHandler(void)
{
	int i = 0;
	if(USART_GetITStatus(UART4,USART_IT_IDLE)!=RESET)
	{

		(void)UART4->SR;
		(void)UART4->DR;	   //清除UART4的空闲中断标志位
		DMA_Cmd(DMA2_Channel3,DISABLE);
		data_length = LaserRangingReceiveSize - DMA_GetCurrDataCounter(DMA2_Channel3);
		for( i = 0; i < data_length; i++ )
		{
			angle_receive[i] = LaserRangingReceiveBuffer[i];
		}
//		if(Verify_CRC16_Check_Sum(angle_receive,data_length))
		if(angle_receive[5]==0X00 && angle_receive[0]==0X55 && angle_receive[1]==0X0A)
		{
				LaserRanging = (angle_receive[8]<<8)|angle_receive[9];		//mm
		}
//		LaserRanging_test = (angle_receive[8]<<8)|angle_receive[9];		//mm
//		if(Verify_modbusCRC16_Check_Sum(angle_receive,data_length))
//		{
//			LaserRanging = ((float)((angle_receive[3]<<8)|angle_receive[4]))/32768*180;
//		}
		
		DMA_SetCurrDataCounter(DMA2_Channel3,LaserRangingReceiveSize);
		DMA_Cmd(DMA2_Channel3,ENABLE);
	}
}



void DMA2_Channel3_IRQHandler(void)
{	
	if(DMA_GetFlagStatus(DMA2_FLAG_TC3) == SET)
	{
		DMA_ClearFlag(DMA2_FLAG_TC3);			
		test_11 = - test_11;

	}
}

void DMA2_Channel5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_IT_TC5)!=RESET)
	{
		DMA_ClearFlag(DMA2_FLAG_TC5);
		DMA_ClearITPendingBit(DMA2_IT_TC5);
		DMA_Cmd(DMA2_Channel5,DISABLE);
	}	
}


//int fputc(int ch, FILE *f)
//{
//		
//		
//		/* ??????????? */
//		while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);	
// 
//        /* ?????? */
//		USART_SendData(UART4, (uint8_t) ch);	
//	
//		return (ch);
//}
// 


//void LaserRangingBuffReceive(unsigned char ReceiveBuffer[])
//{
//	if(Verify_CRC16_Check_Sum(ReceiveBuffer,LaserRangingReceiveSize))
//	{
//		LaserRanging = ((float)((ReceiveBuffer[3]<<8)|ReceiveBuffer[4]))/32768*180;
//	}
//}

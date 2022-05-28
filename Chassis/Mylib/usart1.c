#include "usart1.h"

//裁判系统数据
unsigned char JudgeReceiveBuffer[JudgeBufBiggestSize];
unsigned char JudgeSend[SEND_MAX_SIZE];
unsigned char SaveBuffer[2*JudgeBufBiggestSize];
tGameInfo JudgeReceive;

/**
 * @brief  usart1初始化
 * @param  None
 * @retval None
 */
void uasrt1Config(void)
{
	//RX PA10		TX PA9
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1, ENABLE);
	
	//RX PA10 浮空输入
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&gpio);

	//TX PA9 	推挽输出
	gpio.GPIO_Pin = GPIO_Pin_9;  
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);

	USART_DeInit(USART1);
	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No ;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_Init(USART1,&usart);
	
	USART_Cmd(USART1,ENABLE);
	
	
	//DMA RX
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); 
	
	nvic.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 8;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	{
		DMA_InitTypeDef  dma;
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
		DMA_DeInit(DMA1_Channel5);

		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
		dma.DMA_MemoryBaseAddr = (uint32_t)JudgeReceiveBuffer;
		dma.DMA_DIR = DMA_DIR_PeripheralSRC;
		dma.DMA_BufferSize = JudgeBufBiggestSize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_M2M = DMA_M2M_Disable;
		
		DMA_Init(DMA1_Channel5, &dma);
		DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
  
	
	//DMA TX
		USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	
    nvic.NVIC_IRQChannel = DMA1_Channel4_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 7;
		nvic.NVIC_IRQChannelSubPriority = 1;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
	
		{
		  DMA_InitTypeDef 	dma;
			DMA_DeInit(DMA1_Channel4);
			dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
			dma.DMA_MemoryBaseAddr = (uint32_t)JudgeSend;
			dma.DMA_DIR = DMA_DIR_PeripheralDST;
			dma.DMA_BufferSize = 30;
			dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
			dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			dma.DMA_Mode = DMA_Mode_Circular;
			dma.DMA_Priority = DMA_Priority_VeryHigh;
			dma.DMA_M2M = DMA_M2M_Disable;

			DMA_Init(DMA1_Channel4,&dma);
			DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
			DMA_Cmd(DMA1_Channel4,DISABLE);
	 }		
}


void DMA1_Channel5_IRQHandler(void)
{	
	if(DMA_GetFlagStatus(DMA1_FLAG_TC5) == SET)
	{
		DMA_ClearFlag(DMA1_FLAG_TC5);
		JudgeBuffReceive(JudgeReceiveBuffer,0); 
	}
}

void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC4)!=RESET)
	{
		DMA_ClearFlag(DMA1_IT_TC4);
		DMA_ClearITPendingBit(DMA1_IT_TC4);
		DMA_Cmd(DMA1_Channel4,DISABLE);
	}	
}

void JudgeBuffReceive(unsigned char ReceiveBuffer[],uint16_t DataLen)
{
	uint16_t cmd_id;
	short PackPoint;
	memcpy(&SaveBuffer[JudgeBufBiggestSize],&ReceiveBuffer[0],JudgeBufBiggestSize);
	for(PackPoint=0;PackPoint<JudgeBufBiggestSize;PackPoint++)
	{
		if(SaveBuffer[PackPoint]==0xA5) 
		{
			if((Verify_CRC8_Check_Sum(&SaveBuffer[PackPoint],5)==1))
			{
				cmd_id=(SaveBuffer[PackPoint+6])&0xff;
				cmd_id=(cmd_id<<8)|SaveBuffer[PackPoint+5];  
				DataLen=SaveBuffer[PackPoint+2]&0xff;
				DataLen=(DataLen<<8)|SaveBuffer[PackPoint+1];
				if((cmd_id==0x0201)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9))) 
				{
					JudgeReceive.RobotID=SaveBuffer[PackPoint+7];
					JudgeReceive.RobotLevel=SaveBuffer[PackPoint+8];
					JudgeReceive.remainHP=(SaveBuffer[PackPoint+10]<<8)|SaveBuffer[PackPoint+9]; 
				}
//				if((cmd_id==0x0207)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9)))
//				{
//					if(SaveBuffer[PackPoint+7+0]==1)
//					{
//						JudgeReceive.bulletFreq= SaveBuffer[PackPoint+7+1];
//						memcpy(&JudgeReceive.bulletSpeed,&SaveBuffer[PackPoint+7+2],4);
//					}
//				}
//				if((cmd_id==0x0202)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9)))
//				{
//					memcpy(&JudgeReceive.realChassisOutV,&SaveBuffer[PackPoint+7+0],2);
//					JudgeReceive.realChassisOutV = JudgeReceive.realChassisOutV /1000.0f;
//					memcpy(&JudgeReceive.realChassisOutA,&SaveBuffer[PackPoint+7+2],2);
//					memcpy(&JudgeReceive.realChassispower,&SaveBuffer[PackPoint+7+4],4);
//					memcpy(&JudgeReceive.remainEnergy,&SaveBuffer[PackPoint+7+8],2);
//					memcpy(&JudgeReceive.shooterHeat17,&SaveBuffer[PackPoint+7+10],2);
//				}
			}
		}
	}
	memcpy(&SaveBuffer[0],&SaveBuffer[JudgeBufBiggestSize],JudgeBufBiggestSize);
}






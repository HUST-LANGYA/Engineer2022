#include "main.h"
#include <math.h>

#define RX_USART2_BUFFER 30
int DATA_LENGTH=0;
int DATA_LENGTH_TX = 0;
int x1 , y1 , w1 , h1;
uint8_t usart_rx[100];
uint8_t data_receive[100];



/*串口2用来接收k210发来的数据，解算二维码位置*/
void uasrt2Config(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);

	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	USART_DeInit(USART2);
	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No ;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_Init(USART2, &usart);
	USART_Cmd(USART2, ENABLE);
	
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE); 
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); 
	
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);   //开启串口闲时中断  	
	nvic.NVIC_IRQChannel = USART2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&nvic);
	
	nvic.NVIC_IRQChannel = DMA1_Channel6_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);



	//接收dma  
	{
		DMA_InitTypeDef  dma;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
		DMA_DeInit(DMA1_Channel6);
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
		dma.DMA_MemoryBaseAddr = (uint32_t)usart_rx; 
		dma.DMA_DIR = DMA_DIR_PeripheralSRC;
		dma.DMA_BufferSize = RX_USART2_BUFFER;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_M2M = DMA_M2M_Disable;
		
		DMA_Init(DMA1_Channel6, &dma);
		DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Channel6, ENABLE);		
	}
	

	
}

void USART2_IRQHandler(void)
{
	int i = 0 , j = 0 ,k = 0 ,cnt = 0;
	int temp_value;
	char temp_space[100];

	if(USART_GetITStatus(USART2,USART_IT_IDLE)!=RESET)  
	{ 
		(void)USART2->SR;
		(void)USART2->DR;	   //清除USART1的空闲中断标志位
		DMA_Cmd(DMA1_Channel6,DISABLE);
		DATA_LENGTH = RX_USART2_BUFFER - DMA_GetCurrDataCounter(DMA1_Channel6);
		
		/*解码函数*/
		for( i = 0; i < DATA_LENGTH; i++ )
		{
			data_receive[i] = usart_rx[i];
		}
		
		if( data_receive[0] == '!')
		{
			cnt = 0 , i = 1;;
			while(i < DATA_LENGTH)
			{
				j = 0;
				/*以每个'？'符号为断点，解码出四个传过来的坐标*/
				while(data_receive[i] != '?')
				{
					temp_space[j++] = data_receive[i++];				
				}
				
				/*将字符串转化为整数*/
				temp_value = 0;
				if(temp_space[0] != '-')
				{
					for(k = 0 ; k < j ; k++)
					{
						temp_value += (temp_space[k] - '0')*pow(10.0,j-k-1);
					}
				}
				else
				{
					for(k = 1 ; k < j ; k++)
					{
						temp_value += (temp_space[k] - '0')*pow(10.0,j-k-1);
					}
					temp_value *= -1;
				}
				
				/*根据cnt的大小，来判断此时是四个坐标中的哪个坐标*/
				switch(cnt)
				{
					case 0:
						x1 = temp_value;
						break;
					
					case 1:
						y1 = temp_value;
						break;
					
					case 2:
						w1 = temp_value;
					
					case 3:
						h1 = temp_value;
				}
				cnt++;
				i++;
			}
			
			
		}
		
	
		DMA_SetCurrDataCounter(DMA1_Channel6,RX_USART2_BUFFER);
		DMA_Cmd(DMA1_Channel6,ENABLE);
	}
}





/**
	*@brief 		�Ƕ�������ͨѶ����
	*@author		ף��ΰ
	*@date			2021-12-11
*/

//#include "LaserRangingTask.h"
#include "ClampAngleTask.h"

/**********************************************************************************************************
*�� �� ��: Clamp_Angle_task
*����˵��: ��������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
extern unsigned char ClampAngleSend[ClampAngleSendSize];

uint32_t time_test = 50;
uint32_t Clamp_high_water;
void clampAngleSendTask(void *pvParameters)
{
//	
//	//�޸Ĳ�����Ϊ115200
//	ClampAngleSend[0] = 0X50;
//	ClampAngleSend[1] = 0X06;
//	ClampAngleSend[2] = 0X00;
//	ClampAngleSend[3] = 0X04;
//	ClampAngleSend[4] = 0X00;
//	ClampAngleSend[5] = 0X06;
//	ClampAngleSend[6] = 0X45;
//	ClampAngleSend[7] = 0X88;
//	clampAngleSend();	
//	vTaskDelay(100);
	
//	//����
//	ClampAngleSend[0] = 0X50;
//	ClampAngleSend[1] = 0X06;
//	ClampAngleSend[2] = 0X00;
//	ClampAngleSend[3] = 0X69;
//	ClampAngleSend[4] = 0XB5;
//	ClampAngleSend[5] = 0X88;
//	ClampAngleSend[6] = 0X22;
//	ClampAngleSend[7] = 0XA1;
//	clampAngleSend();	
//	vTaskDelay(100);
//	
//	//�޸Ĳ�����Ϊ921600
//	ClampAngleSend[0] = 0X50;
//	ClampAngleSend[1] = 0X06;
//	ClampAngleSend[2] = 0X00;
//	ClampAngleSend[3] = 0X04;
//	ClampAngleSend[4] = 0X00;
//	ClampAngleSend[5] = 0X09;
//	ClampAngleSend[6] = 0X05;
//	ClampAngleSend[7] = 0X8C;
//	clampAngleSend();	
//	vTaskDelay(100);
//	
//	//�޸Ĵ���Ϊ256HZ
//	ClampAngleSend[0] = 0X50;
//	ClampAngleSend[1] = 0X06;
//	ClampAngleSend[2] = 0X00;
//	ClampAngleSend[3] = 0X1F;
//	ClampAngleSend[4] = 0X00;
//	ClampAngleSend[5] = 0X00;
//	ClampAngleSend[6] = 0XB5;
//	ClampAngleSend[7] = 0XB8;
//	clampAngleSend();	
//	vTaskDelay(100);
//	
//	//����
//	ClampAngleSend[0] = 0X50;
//	ClampAngleSend[1] = 0X06;
//	ClampAngleSend[2] = 0X00;
//	ClampAngleSend[3] = 0X00;
//	ClampAngleSend[4] = 0X00;
//	ClampAngleSend[5] = 0X00;
//	ClampAngleSend[6] = 0X84;
//	ClampAngleSend[7] = 0X4B;
//	clampAngleSend();	
//	vTaskDelay(100);
//	
//	//�޸Ĵ��ڲ�����
//		changeBaudRate();
//		vTaskDelay(100);
//		
//		//��ȡ�Ƕ�
//	ClampAngleSend[0] = 0X50;
//	ClampAngleSend[1] = 0X03;
//	ClampAngleSend[2] = 0X00;
//	ClampAngleSend[3] = 0X3D;
//	ClampAngleSend[4] = 0X00;
//	ClampAngleSend[5] = 0X03;
//	ClampAngleSend[6] = 0X99;
//	ClampAngleSend[7] = 0X86;


		//��ȡ��������
	ClampAngleSend[0] = 0X51;
	ClampAngleSend[1] = 0X0A;
	ClampAngleSend[2] = 0X00;
	ClampAngleSend[3] = 0X01;
	ClampAngleSend[4] = 0X00;
	ClampAngleSend[5] = 0X05;
	ClampAngleSend[6] = 0X02;
	ClampAngleSend[7] = 0X00;
	ClampAngleSend[8] = 0X63;
	
	while(1){

		clampAngleSend();		
		
		vTaskDelay(time_test);
//		vTaskDelay(1);
//		#if INCLUDE_uxTaskGetStackHighWaterMark
//        Clamp_high_water = uxTaskGetStackHighWaterMark(NULL);
//		#endif
  }
}

//void clampAngleSend(void)
//{
////	USART3->SR=0;
////	USART3->DR=0;
//	
//	DMA_Init(DMA1_Channel2,&USART3_TX_dma);
//	DMA_Cmd(DMA1_Channel2,ENABLE);
//}

void clampAngleSend(void)
{
	(void)UART4->SR;
	(void)UART4->DR;
	DMA_InitTypeDef 	dma;
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
			DMA_DeInit(DMA1_Channel2);
			dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
			dma.DMA_MemoryBaseAddr = (uint32_t)ClampAngleSend;
			dma.DMA_DIR = DMA_DIR_PeripheralDST;
			dma.DMA_BufferSize = ClampAngleSendSize;
			dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
			dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			dma.DMA_Mode = DMA_Mode_Normal;
			dma.DMA_Priority = DMA_Priority_VeryHigh;
			dma.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel5,&dma);
	DMA_Cmd(DMA2_Channel5,ENABLE);
}


/**
	*@brief 		�Ƕ�������ͨѶ����
	*@author		ף��ΰ
	*@date			2021-12-11
*/

#include "LaserRangingTask.h"
//#include "ClampAngleTask.h"

/**********************************************************************************************************
*�� �� ��: Laser_Ranging_task
*����˵��: ��������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
extern unsigned char LaserRangingSend[LaserRangingSendSize];

uint32_t time_test = 50;
uint32_t Laser_high_water;
void laserRangingSendTask(void *pvParameters)
{
//	
//	//�޸Ĳ�����Ϊ115200
//	LaserRangingSend[0] = 0X50;
//	LaserRangingSend[1] = 0X06;
//	LaserRangingSend[2] = 0X00;
//	LaserRangingSend[3] = 0X04;
//	LaserRangingSend[4] = 0X00;
//	LaserRangingSend[5] = 0X06;
//	LaserRangingSend[6] = 0X45;
//	LaserRangingSend[7] = 0X88;
//	laserRangingSend();	
//	vTaskDelay(100);
	
//	//����
//	LaserRangingSend[0] = 0X50;
//	LaserRangingSend[1] = 0X06;
//	LaserRangingSend[2] = 0X00;
//	LaserRangingSend[3] = 0X69;
//	LaserRangingSend[4] = 0XB5;
//	LaserRangingSend[5] = 0X88;
//	LaserRangingSend[6] = 0X22;
//	LaserRangingSend[7] = 0XA1;
//	laserRangingSend();	
//	vTaskDelay(100);
//	
//	//�޸Ĳ�����Ϊ921600
//	LaserRangingSend[0] = 0X50;
//	LaserRangingSend[1] = 0X06;
//	LaserRangingSend[2] = 0X00;
//	LaserRangingSend[3] = 0X04;
//	LaserRangingSend[4] = 0X00;
//	LaserRangingSend[5] = 0X09;
//	LaserRangingSend[6] = 0X05;
//	LaserRangingSend[7] = 0X8C;
//	laserRangingSend();	
//	vTaskDelay(100);
//	
//	//�޸Ĵ���Ϊ256HZ
//	LaserRangingSend[0] = 0X50;
//	LaserRangingSend[1] = 0X06;
//	LaserRangingSend[2] = 0X00;
//	LaserRangingSend[3] = 0X1F;
//	LaserRangingSend[4] = 0X00;
//	LaserRangingSend[5] = 0X00;
//	LaserRangingSend[6] = 0XB5;
//	LaserRangingSend[7] = 0XB8;
//	laserRangingSend();	
//	vTaskDelay(100);
//	
//	//����
//	LaserRangingSend[0] = 0X50;
//	LaserRangingSend[1] = 0X06;
//	LaserRangingSend[2] = 0X00;
//	LaserRangingSend[3] = 0X00;
//	LaserRangingSend[4] = 0X00;
//	LaserRangingSend[5] = 0X00;
//	LaserRangingSend[6] = 0X84;
//	LaserRangingSend[7] = 0X4B;
//	laserRangingSend();	
//	vTaskDelay(100);
//	
//	//�޸Ĵ��ڲ�����
//		changeBaudRate();
//		vTaskDelay(100);
//		
//		//��ȡ�Ƕ�
//	LaserRangingSend[0] = 0X50;
//	LaserRangingSend[1] = 0X03;
//	LaserRangingSend[2] = 0X00;
//	LaserRangingSend[3] = 0X3D;
//	LaserRangingSend[4] = 0X00;
//	LaserRangingSend[5] = 0X03;
//	LaserRangingSend[6] = 0X99;
//	LaserRangingSend[7] = 0X86;


		//��ȡ��������
	LaserRangingSend[0] = 0X51;
	LaserRangingSend[1] = 0X0A;
	LaserRangingSend[2] = 0X00;
	LaserRangingSend[3] = 0X01;
	LaserRangingSend[4] = 0X00;
	LaserRangingSend[5] = 0X05;
	LaserRangingSend[6] = 0X02;
	LaserRangingSend[7] = 0X00;
	LaserRangingSend[8] = 0X63;
	
	while(1){

		laserRangingSend();		
		
		vTaskDelay(time_test);
//		vTaskDelay(1);
//		#if INCLUDE_uxTaskGetStackHighWaterMark
//        Laser_high_water = uxTaskGetStackHighWaterMark(NULL);
//		#endif
  }
}

//void laserRangingSend(void)
//{
////	USART3->SR=0;
////	USART3->DR=0;
//	
//	DMA_Init(DMA1_Channel2,&USART3_TX_dma);
//	DMA_Cmd(DMA1_Channel2,ENABLE);
//}

void laserRangingSend(void)
{
	(void)UART4->SR;
	(void)UART4->DR;
	DMA2_Channel5->CNDTR = LaserRangingSendSize;
	DMA_Cmd(DMA2_Channel5,ENABLE);
}


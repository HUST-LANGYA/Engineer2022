/**
	*@brief 		角度陀螺仪通讯任务
	*@author		祝科伟
	*@date			2021-12-11
*/

//#include "LaserRangingTask.h"
#include "ClampAngleTask.h"

/**********************************************************************************************************
*函 数 名: Clamp_Angle_task
*功能说明: 底盘任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
extern unsigned char ClampAngleSend[ClampAngleSendSize];

uint32_t time_test = 50;
uint32_t Clamp_high_water;
void clampAngleSendTask(void *pvParameters)
{
//	
//	//修改波特率为115200
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
	
//	//解锁
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
//	//修改波特率为921600
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
//	//修改带宽为256HZ
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
//	//保存
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
//	//修改串口波特率
//		changeBaudRate();
//		vTaskDelay(100);
//		
//		//获取角度
//	ClampAngleSend[0] = 0X50;
//	ClampAngleSend[1] = 0X03;
//	ClampAngleSend[2] = 0X00;
//	ClampAngleSend[3] = 0X3D;
//	ClampAngleSend[4] = 0X00;
//	ClampAngleSend[5] = 0X03;
//	ClampAngleSend[6] = 0X99;
//	ClampAngleSend[7] = 0X86;


		//获取距离数据
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
	DMA2_Channel5->CNDTR = ClampAngleSendSize;
	DMA_Cmd(DMA2_Channel5,ENABLE);

}


#ifndef _UART4_H
#define _UART4_H
#include "main.h"

#define LaserRangingReceiveSize 				15		//接收角度陀螺仪字长
#define LaserRangingSendSize		15   	//发送角度陀螺仪字长

void UART4_Configuration(void);
void UART4_IRQHandler(void);
void DMA2_Channel3_IRQHandler(void);
void DMA2_Channel5_IRQHandler(void);
//void LaserRangingBuffReceive(unsigned char ReceiveBuffer[]);
//int fputc(int ch, FILE *f);
#endif

#ifndef _UART4_H
#define _UART4_H
#include "main.h"

#define LaserRangingReceiveSize 				15		//���սǶ��������ֳ�
#define LaserRangingSendSize		15   	//���ͽǶ��������ֳ�

void UART4_Configuration(void);
void UART4_IRQHandler(void);
void DMA2_Channel3_IRQHandler(void);
void DMA2_Channel5_IRQHandler(void);
//void LaserRangingBuffReceive(unsigned char ReceiveBuffer[]);
//int fputc(int ch, FILE *f);
#endif

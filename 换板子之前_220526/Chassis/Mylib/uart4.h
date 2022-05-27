#ifndef _UART4_H
#define _UART4_H
#include "main.h"

#define ClampAngleReceiveSize 				11		//���սǶ��������ֳ�
#define ClampAngleSendSize		8   	//���ͽǶ��������ֳ�

void UART4_Configuration(void);
void DMA2_Channel3_IRQHandler(void);
void DMA2_Channel5_IRQHandler(void);
void ClampAngleBuffReceive(unsigned char ReceiveBuffer[]);
int fputc(int ch, FILE *f);
#endif

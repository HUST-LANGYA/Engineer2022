#ifndef __USART3_H
#define __USART3_H
#include "main.h"

#define ClampAngleReceiveSize 				11		//���սǶ��������ֳ�
#define ClampAngleSendSize		8   	//���ͽǶ��������ֳ�

void USART3_Configuration(void);
void USART3_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);

void changeBaudRate(void);
#endif

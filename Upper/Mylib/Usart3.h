#ifndef __USART3_H
#define __USART3_H
#include "main.h"

#define ClampAngleReceiveSize 				12		//接收角度陀螺仪字长
#define ClampAngleSendSize		9   	//发送角度陀螺仪字长

void USART3_Configuration(void);
void USART3_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);

void changeBaudRate(void);
#endif

#ifndef _USART1_H
#define _USART1_H
#include "main.h"

#define JudgeBufBiggestSize 45	//����������󳤶�
#define SEND_MAX_SIZE    128    //�ϴ��������ĳ���

void uasrt1Config(void);
void JudgeBuffReceive(unsigned char ReceiveBuffer[],uint16_t DataLen);

#endif

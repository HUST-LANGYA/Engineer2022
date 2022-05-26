#ifndef _USART1_H
#define _USART1_H
#include "main.h"

#define JudgeBufBiggestSize 45	//接收数据最大长度
#define SEND_MAX_SIZE    128    //上传数据最大的长度

void uasrt1Config(void);
void JudgeBuffReceive(unsigned char ReceiveBuffer[],uint16_t DataLen);

#endif

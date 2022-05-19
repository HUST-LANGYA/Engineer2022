#ifndef _CAN1_H_
#define _CAN1_H_
#include "main.h"

void can1Config(void);



void chassisCurrentSend(int a, int b, int c, int d);
void motorCurrentSend(int a, int b, int c, int d);

//void track_current_send(int left, int right);
#endif



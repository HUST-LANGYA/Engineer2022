#ifndef _CAN2_H_
#define _CAN2_H_
#include "main.h"

void can2Config(void);

void can2Master2Slave(void);
void Data_Send_to_Slave_Init(void);
//void ForkLift_current_send(int left, int right);
void warehouseCurrentSend(int a, int b, int c, int d);

float getYawAngle(void);
float getGz(void);


extern unsigned char Data_Receive_from_F103[8];//Data_Receive存的是接收到的从C板传来的数据

#endif



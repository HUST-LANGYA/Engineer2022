#ifndef __CAN_H
#define __CAN_H

void CAN1_Config(void);
void motorCurrentSend(int a, int b, int c, int d);
void Slave2Master(void);
void Data_Send_to_Master_Init(void);


extern unsigned char data_receive[8]; //C板接收的数据

#endif

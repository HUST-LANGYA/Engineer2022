#ifndef __MOTOR_TASK_H
#define __MOTOR_TASK_H

#include "main.h"

#define FORWARD_START_CHECK PBin(3)
#define FORWARD_END_CHECK   PBin(4)

void Pid_Motor_Init(void);
void Motor_task(void *pvParameters);
void motor_cal_task(void);
#endif

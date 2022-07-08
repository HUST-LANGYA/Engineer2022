#ifndef __MOTOR_CAL_TASK_H
#define __MOTOR_CAL_TASK_H

#include "main.h"


void motorPidInit(void);
void motorCalTask(void *pvParameters);
void motorCal(void);
#endif


#ifndef __STATE_TASK_H
#define __STATE_TASK_H

#include "main.h"

void modeInit(void);
void modeSwitch(void);
void modeSwitchTask(void *pvParameters);
void flagModeSwitch(void);
void autoModeSwitch(void);

#endif

#ifndef _STATE_H_
#define _STATE_H_

#include "main.h"
extern int test11;

//typedef struct 
//{
//	int flag_save_mode;
//	int flag_key_mode;
//	int flag_landing_mode;
//	int flag_manual_landing_mode;
//	int flag_get_buttel_mode;
//	int flag_get_buttel_mode1;
//	int flag_get_buttel_mode2;
//	int flag_manual_get_buttel_mode;
//	int flag_lift_up_mode;
//	int flag_initialize_mode;
//	int flag_choose_mode;
//	
//} key_control;



void modeSwitchTask(void *pvParameters);
void switchMode(void);
void controlStateGet(void);
void motionTargetGet(void);


void  key_control_Init(void);
//void ReturnState(void *pvParameters);
#endif

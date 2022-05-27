#ifndef __TIM2_H
#define __TIM2_H
#include "main.h"

//Í¼´«µÄ×´Ì¬
#define CAMERA_DEFAULT 0
#define CAMERA_PITCH_UP 1
#define CAMERA_PITCH_DOWN 2
#define CAMERA_YAW_REAR 3


void TIM3_PWM_Init(void);
void CameraSteeringEngine_Set(void);      

#endif

#ifndef __PID_H__
#define __PID_H__
#include "main.h"

typedef struct
{
		float ActualVaule;
	
    float SetPoint;			
    
    float P;			
    float I;			
    float D;				
    
    float LastError;		
    float PreError;			
    float SumError;			
    float dError;

    float IMax;				
    
    float POut;			
    float IOut;				
    float DOut;				
}pid_Typedef;

float PID_Calc(pid_Typedef * P, float ActualValue);
void PID_Par_Reset(pid_Typedef *P);

#endif

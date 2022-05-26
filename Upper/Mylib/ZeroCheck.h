#ifndef __ZEROCHECK_H
#define __ZEROCHECK_H

#include "main.h"

#define Position 1
#define Speed    2

typedef struct 
{
	float Circle;           //ת��Ȧ��
	float CountCycle;       //ת��һȦ���ܼ�������
	float LastValue;        //����������һ�ε�ֵ	
	float ActualValue;      //����������ǰֵ
	float PreError;         //������жϲ�ֵ
}ZeroCheck_Typedef;

float ZeroCheck(ZeroCheck_Typedef *Zero,float value,short Zerocheck_mode);
//float ZeroCheck_forward(ZeroCheck_Typedef *Zero,float value,short Zerocheck_mode);
void ZeroCheck_cal(void);
void ZeroCheck_Init(void);


float ResetZeroCheck(void);
#endif


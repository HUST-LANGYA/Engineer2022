#ifndef _CHASSIS_TASK_H_
#define _CHASSIS_TASK_H_
#include "main.h"

void chassisTask(void *pvParameters);														//底盘运动任务函数
void chassisVelPidInit(float p, float i, float d, float i_max);	//底盘速度环参数初始化
void chassisExec(void);																					//底盘执行代码
void chassisPowerOff(void);																			//底盘断电
void chassisRcModeCal(void);																		//遥控器模式控制时底盘计算代码
void chassisKeyModeCal(void);																		//键鼠模式控制时底盘计算代码
void chassisModeNormal(void);																		//正常底盘模式下的速度设定计算
void chassisModeStatic(void);																		//底盘静步模式下的速度设定计算
int  chassisModeStaticCalExec(int v, int v_set);								//底盘静步模式速度设定具体计算函数 (对vx,vy,vw的相同计算进行了封装，简化代码)
void chassisCal(int vx, int vy, int vw);												//底盘速度分解及PID计算

//老代码，不知道什么用
void w_offset(float speed);
void w_turn(float speed);
#endif

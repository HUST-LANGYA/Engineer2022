#ifndef _CHASSIS_TASK_H_
#define _CHASSIS_TASK_H_
#include "main.h"

void chassisTask(void *pvParameters);														//�����˶�������
void chassisVelPidInit(float p, float i, float d, float i_max);	//�����ٶȻ�������ʼ��
void chassisExec(void);																					//����ִ�д���
void chassisPowerOff(void);																			//���̶ϵ�
void chassisRcModeCal(void);																		//ң����ģʽ����ʱ���̼������
void chassisKeyModeCal(void);																		//����ģʽ����ʱ���̼������
void chassisModeNormal(void);																		//��������ģʽ�µ��ٶ��趨����
void chassisModeStatic(void);																		//���̾���ģʽ�µ��ٶ��趨����
int  chassisModeStaticCalExec(int v, int v_set);								//���̾���ģʽ�ٶ��趨������㺯�� (��vx,vy,vw����ͬ��������˷�װ���򻯴���)
void chassisCal(int vx, int vy, int vw);												//�����ٶȷֽ⼰PID����

//�ϴ��룬��֪��ʲô��
void w_offset(float speed);
void w_turn(float speed);
#endif

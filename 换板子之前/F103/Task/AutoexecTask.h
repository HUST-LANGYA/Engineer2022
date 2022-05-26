#ifndef __AUTOEXEC_TASK_H
#define __AUTOEXEC_TASK_H

#include "main.h"


typedef enum{

/***************************Ĭ��ֵ��û��ִ�к���ʱ��Ϊ��ֵ*************************/
	AUTOEXEC_DEFAULT 	= 0,
	ATUOEXEC_END			= 1,
	
/***************************ȡ����Դ����ʯ�õ���״̬**************************/
	//Ϊ���������ظ����ڱ�����ͷ����'SL'��'state large'��д
	SL_CLAMP_PRE  = 10,					//��ȡ��ŷ���
	SL_LIFT_ONCE 	= 11,					//һ��̧��
	SL_FORWARD 		= 12,					//ǰ��״̬
	SL_CLAMP   		= 13,					//��ȡ��ʯ
	SL_LIFT_TWICE	= 14,					//����̧��
	SL_BACK				= 15,					//����״̬
	SL_LAND_ONCE	= 16,					//�����˽���
	SL_LAND_TWICE	= 17,					//����̧������
	SL_LOOSE			= 18,					//�ͷſ�ʯ


/***************************׼���սӿ�ʯ�õ���״̬**************************/
	//'SM'��'state midair'��д
		//�ս���̬�Զ�����
	SM_CLAMP_PRE  = 61,					//��ȡ��ŷ���
	SM_LIFT_TWICE	= 62,					//����̧��
	SM_LIFT_ONCE 	= 63,					//һ��̧��
	SM_FORWARD 		= 64,					//ǰ��״̬
	SM_MID_ON			= 65,					//�ս����״�
		//�սӽ�ȡ��ʯ
	SM_MID_WAIT		= 66,					//�ȴ���ʯ����
	SM_CLAMP   		= 67,					//��ȡ��ʯ
	SM_BACK				= 68,					//����״̬
	SM_LAND_ONCE	= 69,					//�����˽���
	SM_MID_OFF		= 70,					//�ս����׹ر�
	SM_LAND_TWICE	= 71,					//����̧������
	SM_LOOSE			= 72,					//�ͷſ�ʯ
	
	
/***************************ȡС��Դ����ʯ�õ���״̬**************************/
	//'SS'��'state small'��д
	SS_CLAMP_PRE  		= 30,					//��ȡ��ŷ���
	SS_LIFT_TWICE			= 31,					//����̧��
	SS_LIFT_ONCE			= 32,					//������̧��
	SS_FORWARD 				= 33,					//��ŷ�ǰ��
	SS_CLAMP   				= 34,					//��ȡ��ʯ
	SS_GET						= 35,					//ȡ����ʯ(�ÿսӶ�����)
	SS_LAND_ONCE			= 36,					//�����˽���
	SS_BACK						= 37,					//����״̬(ǰ�Ƶ�ŷ��ر�)
	SS_MID_OFF				= 38,					//�ս��ջ�(���¿սӵ�ŷ�)
	SS_LAND_TWICE			= 39,					//����̧������
	SS_LOOSE					= 40,					//�ͷſ�ʯ
	
	
/***************************�Զ��һ��õ���״̬**************************/
	//'SE'��'state exchange'��д
	SE_CLAMP   				= 41,					//��ȡ��ʯ
	SE_LIFT_TWICE			= 42,					//����̧��
	SE_LIFT_ONCE			= 43,					//������̧��
	SE_FORWARD 				= 44,					//��ŷ�ǰ��
	SE_EXCHANGE				= 45,					//�һ���ŷ���
	SE_LOOSE					= 46,					//�ͷſ�ʯ
	SE_BACK						= 47,					//����״̬(ǰ�Ƶ�ŷ��ر�)
	SE_LAND_ONCE			= 48,					//�����˽���
	SE_LAND_TWICE			= 49,					//����̧������
	
}FLAG_AUTOEXEC_ENUM;


//�Զ�ִ��ģʽר����ʱ������û����ȫ��װ������ֱ����ֲ
void autoModeDelay_ms(u32 t);


//������
void autoexecTask(void);
void autoexec(void);

//��ģʽ����
void autoLargeIslandMine(void);
void autoSmallIslandMine(void);
void autoMineMidair(void);
void autoExchange(void);
void autoGetMineMidair(void);



#endif

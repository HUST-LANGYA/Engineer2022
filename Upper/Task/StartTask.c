/**
	*@brief 		��ʼ����
							�������ø�����
	*@author		ף��ΰ
	*@date			2021-12-09
*/


#include "StartTask.h"

uint32_t CPU_high_water;
uint8_t CPU_RunInfo1[200]; //������������ʱ����Ϣ �ֱ��ǣ������� ����״̬ ���ȼ� ʣ��ջ �������
uint8_t CPU_RunInfo2[200]; //������������ʱ����Ϣ �ֱ��ǣ������� ���м���  ʹ����
/*�������ȼ���ֵԽ�ͣ����ȼ�Խ��*/

#define START_TASK_PRIO 1  //�������ȼ�
#define START_STK_SIZE 512 //�����ջ
static TaskHandle_t StartTask_Handler; //������

#define CPU_TASK_PRIO 2  //�������ȼ�
#define CPU_STK_SIZE 128 //�����ջ
static TaskHandle_t CPUTask_Handler; //������

#define MODE_SWITCH_TASK_PRIO 20  //�������ȼ�
#define MODE_SWITCH_STK_SIZE 512 //�����ջ
static TaskHandle_t modeSwitchTask_Handler; //������

#define AUTOEXEC_TASK_PRIO 19  //�������ȼ�
#define AUTOEXEC_STK_SIZE 512 //�����ջ
static TaskHandle_t autoexecTask_Handler; //������

#define CLAMP_ANGLE_TASK_PRIO 10  //�������ȼ�
#define CLAMP_ANGLE_STK_SIZE 256 //�����ջ
static TaskHandle_t ClampAngleTask_Handler; //������

#define MOTOR_TASK_PRIO 17  //�������ȼ�
#define MOTOR_STK_SIZE 256 //�����ջ
static TaskHandle_t MOTOR_TASK_Handler; //������

#define LIFT_TASK_PRIO 13  //�������ȼ�
#define LIFT_STK_SIZE 256 //�����ջ
static TaskHandle_t LIFT_TASK_Handler; //������

#define ROTATE_TASK_PRIO 12  //�������ȼ�
#define ROTATE_STK_SIZE 128 //�����ջ
static TaskHandle_t ROTATE_TASK_Handler; //������


//#define Forward_TASK_PRIO 4  //�������ȼ�
//#define Forward_STK_SIZE 128 //�����ջ
//static TaskHandle_t ForwardTask_Handler; //������


/**
  	*@brief 		��������
  	*@param		  void
	*@return		  void
*/
void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();
	
	xTaskCreate((TaskFunction_t)CPU_task,          //������
                (const char *)"CPU_task",          //��������
                (uint16_t)CPU_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)CPU_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&CPUTask_Handler); //������
	
	xTaskCreate((TaskFunction_t)modeSwitchTask,          //������
                (const char *)"modeSwitchTask",          //��������
                (uint16_t)MODE_SWITCH_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)MODE_SWITCH_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&modeSwitchTask_Handler); //������
							
	xTaskCreate((TaskFunction_t)autoexecTask,          //������
                (const char *)"autoexecTask",          //��������
                (uint16_t)AUTOEXEC_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)AUTOEXEC_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&autoexecTask_Handler); //������

	xTaskCreate((TaskFunction_t)clampAngleSendTask,          //������
                (const char *)"clampAngleSendTask",          //��������
                (uint16_t)CLAMP_ANGLE_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)CLAMP_ANGLE_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&ClampAngleTask_Handler); //������				

	xTaskCreate((TaskFunction_t)Motor_task,          //������
                (const char *)"Motor_task",          //��������
                (uint16_t)MOTOR_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)MOTOR_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&MOTOR_TASK_Handler); //������			
				
	xTaskCreate((TaskFunction_t)Lift_task,          //������
                (const char *)"Lift_task",          //��������
                (uint16_t)LIFT_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)LIFT_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&LIFT_TASK_Handler); //������
				
	xTaskCreate((TaskFunction_t)rotate_Task,          //������
                (const char *)"rotate_Task",          //��������
                (uint16_t)ROTATE_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)ROTATE_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&ROTATE_TASK_Handler); //������			
								
								
								
//	xTaskCreate((TaskFunction_t)Forward_task,          //������
//                (const char *)"Forward_task",          //��������
//                (uint16_t)Forward_STK_SIZE,            //�����ջ��С
//                (void *)NULL,                        //���ݸ��������Ĳ���
//                (UBaseType_t)Forward_TASK_PRIO,        //�������ȼ�
//                (TaskHandle_t *)&ForwardTask_Handler); //������


	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
  taskEXIT_CRITICAL();            //�˳��ٽ���
}


/**
  	*@brief 		���CPUʹ�����
  	*@param		  void
	*@return		  void
*/
void CPU_task(void *pvParameters)
{
	

	
   while (1) {
//    memset(CPU_RunInfo1,0,200); //��Ϣ����������
// 
//    vTaskList((char *)&CPU_RunInfo1); //��ȡ��������ʱ����Ϣ
//    
//    memset(CPU_RunInfo2,0,200); //��Ϣ����������
// 
//    vTaskGetRunTimeStats((char *)&CPU_RunInfo2);
		 
		vTaskDelay(1000); /* ��ʱ 1000 �� tick */
//#if INCLUDE_uxTaskGetStackHighWaterMark
//        CPU_high_water = uxTaskGetStackHighWaterMark(NULL);
//#endif
    }
}

/**
  	*@brief 		��ʼ���������ڴ����������ú���
  	*@param		  void
	*@return		  void
*/
void startTask(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //������
                (const char *)"start_task",          //��������
                (uint16_t)START_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)START_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&StartTask_Handler); //������
}


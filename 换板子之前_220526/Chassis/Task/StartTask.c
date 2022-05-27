#include "StartTask.h"

uint32_t CPU_high_water;
uint8_t CPU_RunInfo1[400]; //������������ʱ����Ϣ �ֱ��ǣ������� ����״̬ ���ȼ� ʣ��ջ �������
uint8_t CPU_RunInfo2[400]; //������������ʱ����Ϣ �ֱ��ǣ������� ���м���  ʹ����


/*�������ȼ���ֵԽ�ͣ����ȼ�Խ��*/

#define START_TASK_PRIO 1  //�������ȼ�
#define START_STK_SIZE 128 //�����ջ
static TaskHandle_t StartTask_Handler; //������

#define CPU_TASK_PRIO 2  //�������ȼ�
#define CPU_STK_SIZE 128 //�����ջ
static TaskHandle_t CPUTask_Handler; //������

#define MODE_SWITCH_TASK_PRIO 20  //�������ȼ�
#define MODE_SWITCH_STK_SIZE 256 //�����ջ
static TaskHandle_t ModeSwitchTask_Handler; //������

//#define OFFLINE_CHECK_TASK_PRIO 3  //�������ȼ�
//#define OFFLINE_CHECK_STK_SIZE 256 //�����ջ
//static TaskHandle_t OfflineCheckTask_Handler; //������

#define CHASSIS_TASK_PRIO 18  //�������ȼ�
#define CHASSIS_STK_SIZE 512 //�����ջ
static TaskHandle_t ChassisTask_Handler; //������

#define MASTER2SLAVE_TASK_PRIO 10  //�������ȼ�
#define MASTER2SLAVE_STK_SIZE 256 //�����ջ
static TaskHandle_t Master2SlaveTask_Handler; //������

#define MOTOR_CAL_TASK_PRIO 18  //�������ȼ�
#define MOTOR_CAL_STK_SIZE 256 //�����ջ
static TaskHandle_t MotorCalTask_Handler; //������

#define MOTOR_FLAG_TASK_PRIO 17  //�������ȼ�
#define MOTOR_FLAG_STK_SIZE 128 //�����ջ
static TaskHandle_t MotorFlagTask_Handler; //������

//#define ReturnState_TASK_PRIO 17  //�������ȼ�
//#define ReturnState_STK_SIZE 512 //�����ջ
//static TaskHandle_t ReturnStateTask_Handler; //������

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
                (TaskHandle_t *)&ModeSwitchTask_Handler); //������

								
xTaskCreate((TaskFunction_t)chassisTask,          //������
                (const char *)"chassisTask",          //��������
                (uint16_t)CHASSIS_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)CHASSIS_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&ChassisTask_Handler); //������

xTaskCreate((TaskFunction_t)master2SlaveTask,          //������
                (const char *)"master2SlaveTask",          //��������
                (uint16_t)MASTER2SLAVE_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)MASTER2SLAVE_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&Master2SlaveTask_Handler); //������				

xTaskCreate((TaskFunction_t)motorCalTask,          //������
                (const char *)"motorCalTask",          //��������
                (uint16_t)MOTOR_CAL_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)MOTOR_CAL_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&MotorCalTask_Handler); //������			
												
xTaskCreate((TaskFunction_t)motorFlagTask,          //������
                (const char *)"motorFlagTask",          //��������
                (uint16_t)MOTOR_FLAG_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)MOTOR_FLAG_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&MotorFlagTask_Handler); //������			
				
//���߼�⺯�������⣬�д��޸ģ�����������������
//xTaskCreate((TaskFunction_t)offlineCheckTask,          //������
//                (const char *)"offlineCheckTask",          //��������
//                (uint16_t)OFFLINE_CHECK_STK_SIZE,            //�����ջ��С
//                (void *)NULL,                        //���ݸ��������Ĳ���
//                (UBaseType_t)OFFLINE_CHECK_TASK_PRIO,        //�������ȼ�
//                (TaskHandle_t *)&OfflineCheckTask_Handler); //������								

//xTaskCreate((TaskFunction_t) ReturnState,          //������
//                (const char *)"ReturnState",          //��������
//                (uint16_t)ReturnState_STK_SIZE,            //�����ջ��С
//                (void *)NULL,                        //���ݸ��������Ĳ���
//                (UBaseType_t)ReturnState_TASK_PRIO,        //�������ȼ�
//                (TaskHandle_t *)&ReturnStateTask_Handler); //������

	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
  taskEXIT_CRITICAL();            //�˳��ٽ���
}





void CPU_task(void *pvParameters)
{
	
	
   while (1) {
    memset(CPU_RunInfo1,0,400); //��Ϣ����������
 
    vTaskList((char *)&CPU_RunInfo1); //��ȡ��������ʱ����Ϣ
    
    memset(CPU_RunInfo2,0,400); //��Ϣ����������
 
    vTaskGetRunTimeStats((char *)&CPU_RunInfo2);
    vTaskDelay(1000); /* ��ʱ 1000 �� tick */
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        CPU_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
void startTask(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //������
                (const char *)"start_task",          //��������
                (uint16_t)START_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)START_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&StartTask_Handler); //������
}


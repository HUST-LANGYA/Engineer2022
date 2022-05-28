/**
	*@brief 		开始任务
							用于配置各任务
	*@author		祝科伟
	*@date			2021-12-09
*/


#include "StartTask.h"

uint32_t CPU_high_water;
uint8_t CPU_RunInfo1[200]; //保存任务运行时间信息 分别是：任务名 任务状态 优先级 剩余栈 任务序号
uint8_t CPU_RunInfo2[200]; //保存任务运行时间信息 分别是：任务名 运行计数  使用率
/*任务优先级数值越低，优先级越低*/

#define START_TASK_PRIO 1  //任务优先级
#define START_STK_SIZE 512 //任务堆栈
static TaskHandle_t StartTask_Handler; //任务句柄

#define CPU_TASK_PRIO 2  //任务优先级
#define CPU_STK_SIZE 128 //任务堆栈
static TaskHandle_t CPUTask_Handler; //任务句柄

#define MODE_SWITCH_TASK_PRIO 20  //任务优先级
#define MODE_SWITCH_STK_SIZE 512 //任务堆栈
static TaskHandle_t modeSwitchTask_Handler; //任务句柄

#define AUTOEXEC_TASK_PRIO 19  //任务优先级
#define AUTOEXEC_STK_SIZE 512 //任务堆栈
static TaskHandle_t autoexecTask_Handler; //任务句柄

#define CLAMP_ANGLE_TASK_PRIO 10  //任务优先级
#define CLAMP_ANGLE_STK_SIZE 256 //任务堆栈
static TaskHandle_t ClampAngleTask_Handler; //任务句柄

#define MOTOR_TASK_PRIO 17  //任务优先级
#define MOTOR_STK_SIZE 256 //任务堆栈
static TaskHandle_t MOTOR_TASK_Handler; //任务句柄

#define LIFT_TASK_PRIO 13  //任务优先级
#define LIFT_STK_SIZE 256 //任务堆栈
static TaskHandle_t LIFT_TASK_Handler; //任务句柄

#define ROTATE_TASK_PRIO 12  //任务优先级
#define ROTATE_STK_SIZE 128 //任务堆栈
static TaskHandle_t ROTATE_TASK_Handler; //任务句柄


//#define Forward_TASK_PRIO 4  //任务优先级
//#define Forward_STK_SIZE 128 //任务堆栈
//static TaskHandle_t ForwardTask_Handler; //任务句柄


/**
  	*@brief 		任务配置
  	*@param		  void
	*@return		  void
*/
void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();
	
	xTaskCreate((TaskFunction_t)CPU_task,          //任务函数
                (const char *)"CPU_task",          //任务名称
                (uint16_t)CPU_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)CPU_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&CPUTask_Handler); //任务句柄
	
	xTaskCreate((TaskFunction_t)modeSwitchTask,          //任务函数
                (const char *)"modeSwitchTask",          //任务名称
                (uint16_t)MODE_SWITCH_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)MODE_SWITCH_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&modeSwitchTask_Handler); //任务句柄
							
	xTaskCreate((TaskFunction_t)autoexecTask,          //任务函数
                (const char *)"autoexecTask",          //任务名称
                (uint16_t)AUTOEXEC_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)AUTOEXEC_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&autoexecTask_Handler); //任务句柄

	xTaskCreate((TaskFunction_t)clampAngleSendTask,          //任务函数
                (const char *)"clampAngleSendTask",          //任务名称
                (uint16_t)CLAMP_ANGLE_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)CLAMP_ANGLE_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&ClampAngleTask_Handler); //任务句柄				

	xTaskCreate((TaskFunction_t)Motor_task,          //任务函数
                (const char *)"Motor_task",          //任务名称
                (uint16_t)MOTOR_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)MOTOR_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&MOTOR_TASK_Handler); //任务句柄			
				
	xTaskCreate((TaskFunction_t)Lift_task,          //任务函数
                (const char *)"Lift_task",          //任务名称
                (uint16_t)LIFT_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)LIFT_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&LIFT_TASK_Handler); //任务句柄
				
	xTaskCreate((TaskFunction_t)rotate_Task,          //任务函数
                (const char *)"rotate_Task",          //任务名称
                (uint16_t)ROTATE_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)ROTATE_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&ROTATE_TASK_Handler); //任务句柄			
								
								
								
//	xTaskCreate((TaskFunction_t)Forward_task,          //任务函数
//                (const char *)"Forward_task",          //任务名称
//                (uint16_t)Forward_STK_SIZE,            //任务堆栈大小
//                (void *)NULL,                        //传递给任务函数的参数
//                (UBaseType_t)Forward_TASK_PRIO,        //任务优先级
//                (TaskHandle_t *)&ForwardTask_Handler); //任务句柄


	vTaskDelete(StartTask_Handler); //删除开始任务
  taskEXIT_CRITICAL();            //退出临界区
}


/**
  	*@brief 		监测CPU使用情况
  	*@param		  void
	*@return		  void
*/
void CPU_task(void *pvParameters)
{
	

	
   while (1) {
//    memset(CPU_RunInfo1,0,200); //信息缓冲区清零
// 
//    vTaskList((char *)&CPU_RunInfo1); //获取任务运行时间信息
//    
//    memset(CPU_RunInfo2,0,200); //信息缓冲区清零
// 
//    vTaskGetRunTimeStats((char *)&CPU_RunInfo2);
		 
		vTaskDelay(1000); /* 延时 1000 个 tick */
//#if INCLUDE_uxTaskGetStackHighWaterMark
//        CPU_high_water = uxTaskGetStackHighWaterMark(NULL);
//#endif
    }
}

/**
  	*@brief 		开始函数，用于创建任务配置函数
  	*@param		  void
	*@return		  void
*/
void startTask(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //任务函数
                (const char *)"start_task",          //任务名称
                (uint16_t)START_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&StartTask_Handler); //任务句柄
}


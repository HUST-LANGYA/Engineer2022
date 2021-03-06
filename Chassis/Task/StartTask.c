#include "StartTask.h"

uint32_t CPU_high_water;
uint8_t CPU_RunInfo1[400]; //保存任务运行时间信息 分别是：任务名 任务状态 优先级 剩余栈 任务序号
uint8_t CPU_RunInfo2[400]; //保存任务运行时间信息 分别是：任务名 运行计数  使用率


/*任务优先级数值越低，优先级越低*/

#define START_TASK_PRIO 1  //任务优先级
#define START_STK_SIZE 128 //任务堆栈
static TaskHandle_t StartTask_Handler; //任务句柄

#define CPU_TASK_PRIO 2  //任务优先级
#define CPU_STK_SIZE 128 //任务堆栈
static TaskHandle_t CPUTask_Handler; //任务句柄

#define MODE_SWITCH_TASK_PRIO 20  //任务优先级
#define MODE_SWITCH_STK_SIZE 256 //任务堆栈
static TaskHandle_t ModeSwitchTask_Handler; //任务句柄

//#define OFFLINE_CHECK_TASK_PRIO 3  //任务优先级
//#define OFFLINE_CHECK_STK_SIZE 256 //任务堆栈
//static TaskHandle_t OfflineCheckTask_Handler; //任务句柄

#define CHASSIS_TASK_PRIO 19  //任务优先级
#define CHASSIS_STK_SIZE 512 //任务堆栈
static TaskHandle_t ChassisTask_Handler; //任务句柄

#define MASTER2SLAVE_TASK_PRIO 10  //任务优先级
#define MASTER2SLAVE_STK_SIZE 256 //任务堆栈
static TaskHandle_t Master2SlaveTask_Handler; //任务句柄

#define MOTOR_CAL_TASK_PRIO 18  //任务优先级
#define MOTOR_CAL_STK_SIZE 256 //任务堆栈
static TaskHandle_t MotorCalTask_Handler; //任务句柄


#define LASER_RANGING_TASK_PRIO 10  //任务优先级
#define LASER_RANGING_STK_SIZE 256 //任务堆栈
static TaskHandle_t LaserRangingTask_Handler; //任务句柄

#define GRAPHIC_TASK_PRIO 4  //任务优先级
#define GRAPHIC_STK_SIZE 128 //任务堆栈
static TaskHandle_t Graphic_Handler; //任务句柄


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
                (TaskHandle_t *)&ModeSwitchTask_Handler); //任务句柄

								
xTaskCreate((TaskFunction_t)chassisTask,          //任务函数
                (const char *)"chassisTask",          //任务名称
                (uint16_t)CHASSIS_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)CHASSIS_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&ChassisTask_Handler); //任务句柄

xTaskCreate((TaskFunction_t)master2SlaveTask,          //任务函数
                (const char *)"master2SlaveTask",          //任务名称
                (uint16_t)MASTER2SLAVE_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)MASTER2SLAVE_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&Master2SlaveTask_Handler); //任务句柄				

xTaskCreate((TaskFunction_t)motorCalTask,          //任务函数
                (const char *)"motorCalTask",          //任务名称
                (uint16_t)MOTOR_CAL_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)MOTOR_CAL_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&MotorCalTask_Handler); //任务句柄			

	xTaskCreate((TaskFunction_t)laserRangingSendTask,          //任务函数
                (const char *)"laserRangingSendTask",          //任务名称
                (uint16_t)LASER_RANGING_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)LASER_RANGING_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&LaserRangingTask_Handler); //任务句柄					

								
	xTaskCreate((TaskFunction_t)GraphicSendtask,          //任务函数
								(const char *)"Graphic_task",          //任务名称
								(uint16_t)GRAPHIC_STK_SIZE,            //任务堆栈大小
								(void *)NULL,                        //传递给任务函数的参数
								(UBaseType_t)GRAPHIC_TASK_PRIO,        //任务优先级
									(TaskHandle_t *)&Graphic_Handler); //任务句柄
								
//断线检测函数有问题，有待修改！！！！！！！！！
//xTaskCreate((TaskFunction_t)offlineCheckTask,          //任务函数
//                (const char *)"offlineCheckTask",          //任务名称
//                (uint16_t)OFFLINE_CHECK_STK_SIZE,            //任务堆栈大小
//                (void *)NULL,                        //传递给任务函数的参数
//                (UBaseType_t)OFFLINE_CHECK_TASK_PRIO,        //任务优先级
//                (TaskHandle_t *)&OfflineCheckTask_Handler); //任务句柄								

	vTaskDelete(StartTask_Handler); //删除开始任务
  taskEXIT_CRITICAL();            //退出临界区
}





void CPU_task(void *pvParameters)
{
	
	
   while (1) {
    memset(CPU_RunInfo1,0,400); //信息缓冲区清零
 
    vTaskList((char *)&CPU_RunInfo1); //获取任务运行时间信息
    
    memset(CPU_RunInfo2,0,400); //信息缓冲区清零
 
    vTaskGetRunTimeStats((char *)&CPU_RunInfo2);
    vTaskDelay(1000); /* 延时 1000 个 tick */
		 
//#if INCLUDE_uxTaskGetStackHighWaterMark
//        CPU_high_water = uxTaskGetStackHighWaterMark(NULL);
//#endif
    }
}
void startTask(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //任务函数
                (const char *)"start_task",          //任务名称
                (uint16_t)START_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&StartTask_Handler); //任务句柄
}


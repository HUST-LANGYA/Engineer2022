#ifndef __AUTOEXEC_TASK_H
#define __AUTOEXEC_TASK_H

#include "main.h"


typedef enum{

/***************************默认值，没有执行函数时置为此值*************************/
	AUTOEXEC_DEFAULT 	= 0,
	ATUOEXEC_END			= 1,
	
/***************************取大资源岛矿石用到的状态**************************/
	//为避免命名重复，在变量开头加上'SL'，'state large'缩写
	SL_CLAMP_PRE  = 10,					//夹取电磁阀打开
	SL_LIFT_ONCE 	= 11,					//一级抬升
	SL_FORWARD 		= 12,					//前移状态
	SL_CLAMP   		= 13,					//夹取矿石
	SL_LIFT_TWICE	= 14,					//二级抬升
	SL_BACK				= 15,					//缩回状态
	SL_LAND_ONCE	= 16,					//四连杆降落
	SL_LAND_TWICE	= 17,					//二级抬升降落
	SL_LOOSE			= 18,					//释放矿石


/***************************准备空接矿石用到的状态**************************/
	//'SM'，'state midair'缩写
		//空接姿态自动调整
	SM_CLAMP_PRE  = 61,					//夹取电磁阀打开
	SM_LIFT_TWICE	= 62,					//二级抬升
	SM_LIFT_ONCE 	= 63,					//一级抬升
	SM_FORWARD 		= 64,					//前移状态
	SM_MID_ON			= 65,					//空接气缸打开
		//空接接取矿石
	SM_MID_WAIT		= 66,					//等待矿石落下
	SM_CLAMP   		= 67,					//夹取矿石
	SM_BACK				= 68,					//缩回状态
	SM_LAND_ONCE	= 69,					//四连杆降落
	SM_MID_OFF		= 70,					//空接气缸关闭
	SM_LAND_TWICE	= 71,					//二级抬升降落
	SM_LOOSE			= 72,					//释放矿石
	
	
/***************************取小资源岛矿石用到的状态**************************/
	//'SS'，'state small'缩写
	SS_CLAMP_PRE  		= 30,					//夹取电磁阀打开
	SS_LIFT_TWICE			= 31,					//二级抬升
	SS_LIFT_ONCE			= 32,					//四连杆抬升
	SS_FORWARD 				= 33,					//电磁阀前移
	SS_CLAMP   				= 34,					//夹取矿石
	SS_GET						= 35,					//取出矿石(用空接顶出来)
	SS_LAND_ONCE			= 36,					//四连杆降落
	SS_BACK						= 37,					//缩回状态(前移电磁阀关闭)
	SS_MID_OFF				= 38,					//空接收回(放下空接电磁阀)
	SS_LAND_TWICE			= 39,					//二级抬升降落
	SS_LOOSE					= 40,					//释放矿石
	
	
/***************************自动兑换用到的状态**************************/
	//'SE'，'state exchange'缩写
	SE_CLAMP   				= 41,					//夹取矿石
	SE_LIFT_TWICE			= 42,					//二级抬升
	SE_LIFT_ONCE			= 43,					//四连杆抬升
	SE_FORWARD 				= 44,					//电磁阀前移
	SE_LOOSE					= 45,					//释放矿石
	
	SE_FORWARD_BACK_2	= 46,					//前移收回
	SE_CLAMP_2				= 47,
	SE_FORWARD_2			= 48,
	
	SE_BACK						= 49,					//缩回状态(前移电磁阀关闭)
	SE_LAND_ONCE			= 50,					//四连杆降落
	SE_LAND_TWICE			= 51,					//二级抬升降落_2			
	SE_LOOSE_2				= 52,
	
	
/***************************软件复位用到的状态**************************/
	//'SR'，'state reset'缩写
	SR_FORWARD 				= 81,					//复位前移
	SR_LIFT_ONCE			= 82,					//复位四连杆
	SR_MID						= 83,					//复位空接气缸
	SR_LIFT_TWICE			= 84,					//复位二级抬升
	
	
/************************激光对位空接动作准备状态***********************/
	//'LP'，'laser pre' 缩写
	LP_LOOSE					= 91,					//松开爪子
	LP_LIFT_ONCE			= 92,					//四连杆抬升
	LP_FORWARD				= 93,					//前移伸出
	LP_CLAMP					= 94,					//夹子关闭


	
}FLAG_AUTOEXEC_ENUM;


//自动执行模式专属延时函数，没有完全封装，不能直接移植
void autoModeDelay_ms(u32 t);


//任务函数
void autoexecTask(void);
void autoexec(void);

//各模式函数
void autoLargeIslandMine(void);
void autoSmallIslandMine(void);
void autoMineMidair(void);
void autoExchange(void);
void autoGetMineMidair(void);
void autoResetSoftware(void);
void autoLaserMidPre(void);


#endif

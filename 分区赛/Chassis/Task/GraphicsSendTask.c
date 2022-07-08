/**********************************************************************************************************
 * @文件     Graphics_Send.c
 * @说明     裁判系统图形发送
 * @版本  	 V1.0
 * @作者     陈志鹏
 * @日期     2021.4
**********************************************************************************************************/
#include "GraphicsSendTask.h"
//#define CAP_GRAPHIC_NUM 9			//超级电容电量图形显示细分格数
//#define PACK_NUM 5

ext_student_interactive_header_data_t custom_grapic_draw;			//自定义图像绘制
ext_student_interactive_char_header_data_t custom_char_draw;  //自定义字符绘制


extern unsigned char JudgeSend[SEND_MAX_SIZE];
extern tGameInfo JudgeReceive;

uint8_t graphic_init_flag = 1;  //需分两次发送

/*********************************图形结构体及其初始化***********************************/

/*救援状态灯偏移量*/
graphic_data_struct_t gds_rescue_L = {
	.graphic_name[0] = 0,
	.graphic_name[1] = 1,
	.graphic_name[2] = 1,//图形名，用于图形索引，可自行定义
	.operate_tpye 	 = Op_Add,//图形操作，0：空操作；1：增加；2：修改；3：删除；
	.graphic_tpye    = UI_Circle,//图形类型，0为直线，其他的查看用户手册
	.layer           = 1,//图层数
	.color           = Green,//颜色
	.start_angle     = 0,
	.end_angle       = 0,
	.width           = 20,
	.start_x         = SCREEN_LENGTH/2-300-50,
	.end_x           = 0,
	.start_y 					= SCREEN_WIDTH/2-330,
	.end_y           = 0,
	.radius          = 10
};

graphic_data_struct_t gds_rescue_R = {
	.graphic_name[0] = 0,
	.graphic_name[1] = 1,
	.graphic_name[2] = 2,//图形名，用于图形索引，可自行定义
	.operate_tpye 	 = Op_Add,//图形操作，0：空操作；1：增加；2：修改；3：删除；
	.graphic_tpye    = UI_Circle,//图形类型，0为直线，其他的查看用户手册
	.layer           = 1,//图层数
	.color           = Green,//颜色
	.start_angle     = 0,
	.end_angle       = 0,
	.width           = 20,
	.start_x         = SCREEN_LENGTH/2-300+50,
	.end_x           = 0,
	.start_y 					= SCREEN_WIDTH/2-330,
	.end_y           = 0,
	.radius          = 10
};

/*复活状态灯偏移量*/
graphic_data_struct_t gds_revive = {
	.graphic_name[0] = 0,
	.graphic_name[1] = 1,
	.graphic_name[2] = 3,//图形名，用于图形索引，可自行定义
	.operate_tpye 	 = Op_Add,//图形操作，0：空操作；1：增加；2：修改；3：删除；
	.graphic_tpye    = UI_Circle,//图形类型，0为直线，其他的查看用户手册
	.layer           = 0,//图层数
	.color           = Green,//颜色
	.start_angle     = 0,
	.end_angle       = 0,
	.width           = 20,
	.start_x 					= SCREEN_LENGTH/2-300,
	.end_x   					= 0,
	.start_y 					= SCREEN_WIDTH/2-330,
	.end_y   					= 0,
	.radius          = 10
};

/*叉车状态灯偏移量*/
graphic_data_struct_t gds_fork_L = {
	.graphic_name[0] = 0,
	.graphic_name[1] = 1,
	.graphic_name[2] = 4,//图形名，用于图形索引，可自行定义
	.operate_tpye 	 = Op_Add,//图形操作，0：空操作；1：增加；2：修改；3：删除；
	.graphic_tpye    = UI_Circle,//图形类型，0为直线，其他的查看用户手册
	.layer           = 0,//图层数
	.color           = Green,//颜色
	.start_angle     = 0,
	.end_angle       = 0,
	.width           = 20,
	.start_x 					= SCREEN_LENGTH/2-300-100,
	.end_x   					= 0,
	.start_y 					= SCREEN_WIDTH/2-330,
	.end_y   					= 0,
	.radius          = 10
};

graphic_data_struct_t gds_fork_R = {
	.graphic_name[0] = 0,
	.graphic_name[1] = 1,
	.graphic_name[2] = 5,//图形名，用于图形索引，可自行定义
	.operate_tpye 	 = Op_Add,//图形操作，0：空操作；1：增加；2：修改；3：删除；
	.graphic_tpye    = UI_Circle,//图形类型，0为直线，其他的查看用户手册
	.layer           = 0,//图层数
	.color           = Green,//颜色
	.start_angle     = 0,
	.end_angle       = 0,
	.width           = 20,
	.start_x = SCREEN_LENGTH/2-300+100,
	.end_x   = 0,
	.start_y 					= SCREEN_WIDTH/2-330,
	.end_y   = 0,
	.radius          = 10
};

/*对矿线偏移量*/
graphic_data_struct_t gds_fetch_L = {
	.graphic_name[0] = 0,
	.graphic_name[1] = 0,
	.graphic_name[2] = 1,//图形名，用于图形索引，可自行定义
	.operate_tpye 	 = Op_Add,//图形操作，0：空操作；1：增加；2：修改；3：删除；
	.graphic_tpye    = UI_Line,//图形类型，0为直线，其他的查看用户手册
	.layer           = 0,//图层数
	.color           = Orange,//颜色
	.start_angle     = 0,
	.end_angle       = 0,
	.width           = 2,
	.start_x 			   = SCREEN_LENGTH/2-100,
	.end_x   			   = SCREEN_LENGTH/2-100,
	.start_y 			   = 0,
	.end_y   			   = SCREEN_WIDTH,
	.radius          = 0
};

graphic_data_struct_t gds_fetch_R = {
	.graphic_name[0] = 0,
	.graphic_name[1] = 0,
	.graphic_name[2] = 2,//图形名，用于图形索引，可自行定义
	.operate_tpye 	 = Op_Add,//图形操作，0：空操作；1：增加；2：修改；3：删除；
	.graphic_tpye    = UI_Line,//图形类型，0为直线，其他的查看用户手册
	.layer           = 0,//图层数
	.color           = Orange,//颜色
	.start_angle     = 0,
	.end_angle       = 0,
	.width           = 2,
	.start_x 			   = SCREEN_LENGTH/2+100,
	.end_x   			   = SCREEN_LENGTH/2+100,
	.start_y 			   = 0,
	.end_y   			   = SCREEN_WIDTH,
	.radius          = 0
};

/***********************字符*************************/
/*底盘三个字符*/
//救援
ext_client_custom_character_t char_rescue = {
	.grapic_data_struct = {
		.graphic_name[0] = 0,
		.graphic_name[1] = 0,
		.graphic_name[2] = 3,//图形名，用于图形索引，可自行定义
		.operate_tpye 	 = Op_Add,//图形操作，0：空操作；1：增加；2：修改；3：删除；
		.graphic_tpye    = UI_Character,//图形类型，0为直线，其他的查看用户手册
		.layer           = 0,//图层数
		.color           = Yellow,//颜色
		.start_angle     = 25,
		.end_angle       = 0,
		.width           = 5,
		.start_x 			   = SCREEN_LENGTH/2-100,
		.end_x   			   = 0,
		.start_y 			   = SCREEN_WIDTH/2-300,
		.end_y   			   = 0,
		.radius          = 0
	},
	.data = "rescue:"
};
//复活
ext_client_custom_character_t char_reveive = {
	.grapic_data_struct = {
		.graphic_name[0] = 0,
		.graphic_name[1] = 0,
		.graphic_name[2] = 4,//图形名，用于图形索引，可自行定义
		.operate_tpye 	 = Op_Add,//图形操作，0：空操作；1：增加；2：修改；3：删除；
		.graphic_tpye    = UI_Character,//图形类型，0为直线，其他的查看用户手册
		.layer           = 0,//图层数
		.color           = Yellow,//颜色
		.start_angle     = 25,
		.end_angle       = 0,
		.width           = 5,
		.start_x 			   = SCREEN_LENGTH/2,
		.end_x   			   = 0,
		.start_y 			   = SCREEN_WIDTH/2-300,
		.end_y   			   = 0,
		.radius          = 0
	},
	.data = "reveive:"
};
//叉车
ext_client_custom_character_t char_fork = {
	.grapic_data_struct = {
		.graphic_name[0] = 0,
		.graphic_name[1] = 0,
		.graphic_name[2] = 5,//图形名，用于图形索引，可自行定义
		.operate_tpye 	 = Op_Add,//图形操作，0：空操作；1：增加；2：修改；3：删除；
		.graphic_tpye    = UI_Character,//图形类型，0为直线，其他的查看用户手册
		.layer           = 0,//图层数
		.color           = Yellow,//颜色
		.start_angle     = 0,
		.end_angle       = 0,
		.width           = 5,
		.start_x 			   = SCREEN_LENGTH/2-500,
		.end_x   			   = 0,
		.start_y 			   = SCREEN_WIDTH/2-300,
		.end_y   			   = 0,
		.radius          = 0
	},
	.data = "fork:"
};
/*爪子状态字符*/
ext_client_custom_character_t char_clamp = {
	.grapic_data_struct = {
		.graphic_name[0] = 0,
		.graphic_name[1] = 0,
		.graphic_name[2] = 6,//图形名，用于图形索引，可自行定义
		.operate_tpye 	 = Op_Add,//图形操作，0：空操作；1：增加；2：修改；3：删除；
		.graphic_tpye    = UI_Character,//图形类型，0为直线，其他的查看用户手册
		.layer           = 0,//图层数
		.color           = Yellow,//颜色
		.start_angle     = 0,
		.end_angle       = 0,
		.width           = 5,
		.start_x 			   = SCREEN_LENGTH/2-500,
		.end_x   			   = 0,
		.start_y 			   = SCREEN_WIDTH/2+300,
		.end_y   			   = 0,
		.radius          = 0
	},
	.data = "clamp:"
};



//graphic_data_struct_t gds_fetch_bottom = {
//	.graphic_name[0] = 0,
//	.graphic_name[1] = 0,
//	.graphic_name[2] = 3,//图形名，用于图形索引，可自行定义
//	.operate_tpye 	 = Op_None,//图形操作，0：空操作；1：增加；2：修改；3：删除；
//	.graphic_tpye    = 0,//图形类型，0为直线，其他的查看用户手册
//	.layer           = 0,//图层数
//	.color           = Yellow,//颜色
//	.start_angle     = 0,
//	.end_angle       = 0,
//	.width           = 5,
//	.start_x 			   = SCREEN_LENGTH/2-200,
//	.end_x   			   = SCREEN_LENGTH/2+200,
//	.start_y 			   = SCREEN_WIDTH/2-200,
//	.end_y   			   = SCREEN_WIDTH/2-200,
//	.radius          = 0
//};

//graphic_data_struct_t gds_redeem_L = {
//	.graphic_name[0] = 0,
//	.graphic_name[1] = 0,
//	.graphic_name[2] = 4,//图形名，用于图形索引，可自行定义
//	.operate_tpye 	 = Op_None,//图形操作，0：空操作；1：增加；2：修改；3：删除；
//	.graphic_tpye    = 0,//图形类型，0为直线，其他的查看用户手册
//	.layer           = 1,//图层数
//	.color           = Green,//颜色
//	.start_angle     = 0,
//	.end_angle       = 0,
//	.width           = 5,
//	.start_x 			   = SCREEN_LENGTH/2-200,
//	.end_x   			   = SCREEN_LENGTH/2-50,
//	.start_y 			   = SCREEN_WIDTH/2,
//	.end_y   			   = SCREEN_WIDTH/2,
//	.radius          = 0
//};

//graphic_data_struct_t gds_redeem_R = {
//	.graphic_name[0] = 0,
//	.graphic_name[1] = 0,
//	.graphic_name[2] = 5,//图形名，用于图形索引，可自行定义
//	.operate_tpye 	 = Op_None,//图形操作，0：空操作；1：增加；2：修改；3：删除；
//	.graphic_tpye    = 0,//图形类型，0为直线，其他的查看用户手册
//	.layer           = 1,//图层数
//	.color           = Green,//颜色
//	.start_angle     = 0,
//	.end_angle       = 0,
//	.width           = 5,
//	.start_x 			   = SCREEN_LENGTH/2-200,
//	.end_x   			   = SCREEN_LENGTH/2-50,
//	.start_y 			   = SCREEN_WIDTH/2+300,
//	.end_y   			   = SCREEN_WIDTH/2,
//	.radius          = 0
//};

//graphic_data_struct_t gds_redeem_bottom = {
//	.graphic_name[0] = 0,
//	.graphic_name[1] = 0,
//	.graphic_name[2] = 6,//图形名，用于图形索引，可自行定义
//	.operate_tpye 	 = Op_None,//图形操作，0：空操作；1：增加；2：修改；3：删除；
//	.graphic_tpye    = 0,//图形类型，0为直线，其他的查看用户手册
//	.layer           = 1,//图层数
//	.color           = Green,//颜色
//	.start_angle     = 0,
//	.end_angle       = 0,
//	.width           = 5,
//	.start_x 			   = SCREEN_LENGTH/2-200,
//	.end_x   			   = SCREEN_LENGTH/2+200,
//	.start_y 			   = SCREEN_WIDTH/2-400,
//	.end_y   			   = SCREEN_WIDTH/2-400,
//	.radius          = 0
//};


/**********************************************************************************************************
*函 数 名: Graphic_Change_Check
*功能说明: 检测有没有图形的变动，或者进行图层的初始化
*形    参: 无
*返 回 值: 0: 无变动，不发送；1：有变动，需发送
**********************************************************************************************************/
//Flag_t last_g_Flag;
//uint8_t graphic_change_indicator = 0;
//int Graphic_Change_Check(void)
//{
//	
//	/*都没有变化*/
//	if(g_Flag == last_g_Flag)
//	{
//		
//	}
//	switch(graphic_change_indicator)
//	{	
//		case 0 : return Op_Add;    break; 
//		case 1 : return Op_Change; break; 
//		case 2 : return Op_Change; break; 
//		default: return Op_None;   break;	
//	}
//	
//	last_g_Flag = g_Flag;
//}

/**********************************************************************************************************
*函 数 名: JudgementCustomizeGraphics
*功能说明: 裁判系统自定义图形发送函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
//装载图形
uint8_t load_time_flag = 0;

void JudgementCustomizeGraphics(void)
{
/*********************************确定ID***********************************/
		custom_grapic_draw.sender_ID=JudgeReceive.RobotID;//发送者ID，机器人对应ID
//		custom_grapic_draw.sender_ID=2;//发送者ID，机器人对应ID
//				custom_grapic_draw.receiver_ID = 0x0102;	
		if(JudgeReceive.RobotID == 102) //蓝方工程
				custom_grapic_draw.receiver_ID = 0x0166;  //蓝方工程操作手客户端
		if(JudgeReceive.RobotID == 2)		//红方工程
				custom_grapic_draw.receiver_ID = 0x0102;	//红方工程操作手客户端
		
		
		custom_char_draw.sender_ID=JudgeReceive.RobotID;//发送者ID，机器人对应ID
//		custom_char_draw.sender_ID=2;//发送者ID，机器人对应ID
//				custom_char_draw.receiver_ID = 0x0102;	
		if(JudgeReceive.RobotID == 102) //蓝方工程
				custom_char_draw.receiver_ID = 0x0166;  //蓝方工程操作手客户端
		if(JudgeReceive.RobotID == 2)		//红方工程
				custom_char_draw.receiver_ID = 0x0102;	//红方工程操作手客户端
		
/*********************************确定各图形变化量***********************************/
		

/*********************************装载自定义图像数据***********************************/
		
		//救援复活线接反了
	if(g_Flag.rescue_solenoid_flag == 0){
		gds_revive.color = Green;
	}else{
		gds_revive.color = Orange;
	}
	
	if(g_Flag.resurge_solenoid_flag == 0){
		gds_rescue_L.color = Green;
		gds_rescue_R.color = Green;
	}else{
		gds_rescue_L.color = Orange;
		gds_rescue_R.color = Orange;
	}
	
	if(g_Flag.fork_solenoid_flag == 0){
		gds_fork_L.color = Green;
		gds_fork_R.color = Green;
	}else{
		gds_fork_L.color = Orange;
		gds_fork_R.color = Orange;
	}
	
	
	
	if(graphic_init_flag != 0)
	{
		gds_rescue_L.operate_tpye = Op_Add;
		gds_rescue_R.operate_tpye = Op_Add;
		gds_revive.operate_tpye 	 = Op_Add;
		gds_fork_L.operate_tpye 	 = Op_Add;
		gds_fork_R.operate_tpye 	 = Op_Add;
		gds_fetch_L.operate_tpye 	 = Op_Add;
		gds_fetch_R.operate_tpye 	 = Op_Add;
	}else 
	{
		gds_rescue_L.operate_tpye = Op_Change;
		gds_rescue_R.operate_tpye = Op_Change;
		gds_revive.operate_tpye 	 = Op_Change;
		gds_fork_L.operate_tpye 	 = Op_Change;
		gds_fork_R.operate_tpye 	 = Op_Change;
		gds_fetch_L.operate_tpye 	 = Op_None;
		gds_fetch_R.operate_tpye 	 = Op_None;
	}
	
	if(load_time_flag == 0){
		referee_data_load_7_Graphic(gds_rescue_L, gds_rescue_R,gds_revive, gds_fork_L, gds_fork_R, gds_fetch_L, gds_fetch_R);
		referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_grapic_draw,sizeof(custom_grapic_draw));
	}
	else if(load_time_flag == 1){
		custom_char_draw.char_custom = char_rescue;
		memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
		strcpy(custom_char_draw.char_custom.data,char_rescue.data);
		custom_char_draw.char_custom.grapic_data_struct.end_angle = strlen(custom_char_draw.char_custom.data);
		referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_char_draw,sizeof(custom_char_draw));
	}
	else if(load_time_flag == 2){
		custom_char_draw.char_custom = char_reveive;
		memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
		strcpy(custom_char_draw.char_custom.data,char_reveive.data);
		referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_char_draw,sizeof(custom_char_draw));
	}
	else if(load_time_flag == 3){
		custom_char_draw.char_custom = char_fork;
		referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_char_draw,sizeof(custom_char_draw));
	}
	else if(load_time_flag == 4){
		custom_char_draw.char_custom = char_clamp;
		referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_char_draw,sizeof(custom_char_draw));
	}
	
	
		
	load_time_flag = load_time_flag + 1;
	if(load_time_flag == LOAD_TIME_FLAG_MAX)
	{
		load_time_flag = 0;
		graphic_init_flag = 0;
	}
}

/**********************************************************************************************************
*函 数 名: referee_data_load_Graphic
*功能说明: 图形数据包装载图形
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void referee_data_load_5_Graphic(graphic_data_struct_t g0, graphic_data_struct_t g1, graphic_data_struct_t g2, graphic_data_struct_t g3, graphic_data_struct_t g4)
{
	custom_grapic_draw.data_cmd_id=0x0103;//绘制五个图形（内容ID，查询裁判系统手册）
	
	custom_grapic_draw.graphic_custom.grapic_data_struct[0] = g0;
	custom_grapic_draw.graphic_custom.grapic_data_struct[1] = g1;
	custom_grapic_draw.graphic_custom.grapic_data_struct[2] = g2;
	custom_grapic_draw.graphic_custom.grapic_data_struct[3] = g3;
	custom_grapic_draw.graphic_custom.grapic_data_struct[4] = g4;
}

/**********************************************************************************************************
*函 数 名: referee_data_load_7_Graphic
*功能说明: 图形数据包装载图形
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void referee_data_load_7_Graphic(graphic_data_struct_t g0, graphic_data_struct_t g1, graphic_data_struct_t g2, graphic_data_struct_t g3, graphic_data_struct_t g4, graphic_data_struct_t g5, graphic_data_struct_t g6)
{
	custom_grapic_draw.data_cmd_id=0x0104;//绘制七个图形（内容ID，查询裁判系统手册）
	
	custom_grapic_draw.graphic_custom.grapic_data_struct[0] = g0;
	custom_grapic_draw.graphic_custom.grapic_data_struct[1] = g1;
	custom_grapic_draw.graphic_custom.grapic_data_struct[2] = g2;
	custom_grapic_draw.graphic_custom.grapic_data_struct[3] = g3;
	custom_grapic_draw.graphic_custom.grapic_data_struct[4] = g4;
	custom_grapic_draw.graphic_custom.grapic_data_struct[5] = g5;
	custom_grapic_draw.graphic_custom.grapic_data_struct[6] = g6;
}

/**********************************************************************************************************
*函 数 名: referee_data_load_NumberUI
*功能说明: 图形数据包装载数字（浮点/字符/整形）
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
//void referee_data_load_NumberUI(void)
//{
//	static unsigned int pitch = 0;
//	pitch = F405.Gimbal_100 * 10;
//	/*******************************Pitch浮点数*********************************/
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[0] = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[1] = 3;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[2] = 1;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].operate_tpye=Op_Add;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_tpye=5;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].layer=3;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].color=Pink;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_angle=20;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_angle=3;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].width=4;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_x=0.7 * SCREEN_LENGTH;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_y=0.7 * SCREEN_WIDTH;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].radius = pitch & 0x03ff;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_x  = (pitch >> 10) & 0x07ff;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_y  = (pitch >> 21) & 0x07ff;
//}

/**********************************************************************************************************
*函 数 名: staight_line_load_Graphic
*功能说明: 图形数据包装载直线
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
//void staight_line_load_Graphic(graphic_straight_line_t u_straight_line, int Op_type)
//{
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[0] = 1;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[1] = 1;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 1;//图形名
//	//上面三个字节代表的是图形名，用于图形索引，可自行定义
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye = Op_type;//图形操作，0：空操作；1：增加；2：修改；3：删除；
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye = 0;//图形类型，0为直线，其他的查看用户手册
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer        = 0;//图层数
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].color        = u_straight_line.color;//颜色
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_angle  = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_angle    = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].width        = 5;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_x      = u_straight_line.start_x;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_y      = u_straight_line.start_y;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_x        = u_straight_line.end_x;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_y        = u_straight_line.end_y;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].radius       = 0;


//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[0] = 1;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[1] = 1;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[2] = 2;//图形名
//	//上面三个字节代表的是图形名，用于图形索引，可自行定义s
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].operate_tpye=Op_type;//图形操作，0：空操作；1：增加；2：修改；3：删除；
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_tpye=0;//图形类型，0为直线，其他的查看用户手册
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].layer=0;//图层数
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].color=Green;//颜色
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_angle=0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_angle=0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].width=1;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_x=SCREEN_LENGTH/2;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_y=SCREEN_WIDTH/2;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_x=SCREEN_LENGTH/2;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_y=SCREEN_WIDTH/2-300;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].radius=0;
//}

/**********************************************************************************************************
*函 数 名: fetch_load_Graphic
*功能说明: 对矿线
*形    参: 操作类型
*返 回 值: 无
**********************************************************************************************************/
/******************************对矿线**********************************/
//void fetch_load_Graphic(int Op_type)
//{
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[0] = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[1] = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 1;//图形名
//	//上面三个字节代表的是图形名，用于图形索引，可自行定义
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye = Op_type;//图形操作，0：空操作；1：增加；2：修改；3：删除；
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye = 0;//图形类型，0为直线，其他的查看用户手册
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer        = fetch_L.layer;//图层数
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].color        = fetch_L.color;//颜色
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_angle  = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_angle    = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].width        = 5;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_x      = fetch_L.start_x;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_y      = fetch_L.start_y;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_x        = fetch_L.end_x;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_y        = fetch_L.end_y;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].radius       = 0;


//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[0] = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[1] = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 2;//图形名
//	//上面三个字节代表的是图形名，用于图形索引，可自行定义
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye = Op_type;//图形操作，0：空操作；1：增加；2：修改；3：删除；
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye = 0;//图形类型，0为直线，其他的查看用户手册
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer        = fetch_R.layer;//图层数
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].color        = fetch_R.color;//颜色
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_angle  = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_angle    = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].width        = 5;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_x      = fetch_R.start_x;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_y      = fetch_R.start_y;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_x        = fetch_R.end_x;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_y        = fetch_R.end_y;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].radius       = 0;
//}


/**********************************************************************************************************
*函 数 名: referee_data_pack_handle
*功能说明: 裁判系统图形数据打包发送
*形    参: uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len
*返 回 值: 无
**********************************************************************************************************/
uint8_t seq = 0;	
extern volatile char dma_tx_busy;
void referee_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len)
{
	uint16_t frame_length = frameheader_len + cmd_len + len + crc_len;   //数据帧长度	

	memset(JudgeSend,0,frame_length);  //存储数据的数组清零
	
	/*****帧头打包*****/
	JudgeSend[0] = sof;//数据帧起始字节
	memcpy(&JudgeSend[1],(uint8_t*)&len, sizeof(len));//数据帧中data的长度
	JudgeSend[3] = seq;//包序号
	Append_CRC8_Check_Sum(JudgeSend,frameheader_len);  //帧头校验CRC8

	/*****命令码打包*****/
	memcpy(&JudgeSend[frameheader_len],(uint8_t*)&cmd_id, cmd_len);
	
	/*****数据打包*****/
	memcpy(&JudgeSend[frameheader_len+cmd_len], p_data, len);
	Append_CRC16_Check_Sum(JudgeSend,frame_length);  //一帧数据校验CRC16

	if (seq == 0xff) seq=0;
  else seq++;
	
	/*****数据上传*****/
//	(void)USART1->SR;
//	(void)USART1->DR;
//	DMA1_Channel4->CNDTR = frame_length;
//	DMA_Cmd(DMA1_Channel4,ENABLE);
	
	/*****普通串口发送*****/
//	USART_ClearFlag(USART1,USART_FLAG_TC);
//	for(i=0;i<frame_length;i++)
//	{
//	  USART_SendData(USART1,JudgeSend[i]);
//	  while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET); //等待之前的字符发送完成
//	}
	
	/*****DMA串口发送*****/
	while(dma_tx_busy);
//	(void)USART1->SR;
//	(void)USART1->DR;
	DMA1_Channel4->CNDTR=frame_length;
	dma_tx_busy=1;
	DMA_Cmd(DMA1_Channel4,ENABLE);
}
/**********************************************************************************************************
*函 数 名: GraphicSendtask
*功能说明: 图形发送任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
extern ext_student_interactive_char_header_data_t custom_char_draw;  //自定义字符绘制
int char_change_state,graphic_change_state;

void GraphicSendtask(void *pvParameters)
{
   while (1) {
    
			/*浮点数，图形*/
//			graphic_change_state = Graphic_Change_Check();
//		 
//			if(graphic_change_state!= Op_None){
//				JudgementCustomizeGraphics(graphic_change_state);
//				referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_grapic_draw,sizeof(custom_grapic_draw));
//			}
			
//			if(graphic_init_flag != 0){		//初始化未完成
				JudgementCustomizeGraphics();
//				graphic_init_flag = 0;
//			}else
//			{
//				
//			}

			vTaskDelay(50); 	 
//#if INCLUDE_uxTaskGetStackHighWaterMark
//		
//#endif
	}
}

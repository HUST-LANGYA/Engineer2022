/**********************************************************************************************************
 * @�ļ�     Graphics_Send.c
 * @˵��     ����ϵͳͼ�η���
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2021.4
**********************************************************************************************************/
#include "GraphicsSendTask.h"
//#define CAP_GRAPHIC_NUM 9			//�������ݵ���ͼ����ʾϸ�ָ���
//#define PACK_NUM 5

ext_student_interactive_header_data_t custom_grapic_draw;			//�Զ���ͼ�����
ext_student_interactive_char_header_data_t custom_char_draw;  //�Զ����ַ�����


extern unsigned char JudgeSend[SEND_MAX_SIZE];
extern tGameInfo JudgeReceive;

uint8_t graphic_init_flag = 1;  //������η���

/*********************************ͼ�νṹ�弰���ʼ��***********************************/

/*��Ԯ״̬��ƫ����*/
graphic_data_struct_t gds_rescue_L = {
	.graphic_name[0] = 0,
	.graphic_name[1] = 1,
	.graphic_name[2] = 1,//ͼ����������ͼ�������������ж���
	.operate_tpye 	 = Op_Add,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
	.graphic_tpye    = UI_Circle,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
	.layer           = 1,//ͼ����
	.color           = Green,//��ɫ
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
	.graphic_name[2] = 2,//ͼ����������ͼ�������������ж���
	.operate_tpye 	 = Op_Add,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
	.graphic_tpye    = UI_Circle,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
	.layer           = 1,//ͼ����
	.color           = Green,//��ɫ
	.start_angle     = 0,
	.end_angle       = 0,
	.width           = 20,
	.start_x         = SCREEN_LENGTH/2-300+50,
	.end_x           = 0,
	.start_y 					= SCREEN_WIDTH/2-330,
	.end_y           = 0,
	.radius          = 10
};

/*����״̬��ƫ����*/
graphic_data_struct_t gds_revive = {
	.graphic_name[0] = 0,
	.graphic_name[1] = 1,
	.graphic_name[2] = 3,//ͼ����������ͼ�������������ж���
	.operate_tpye 	 = Op_Add,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
	.graphic_tpye    = UI_Circle,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
	.layer           = 0,//ͼ����
	.color           = Green,//��ɫ
	.start_angle     = 0,
	.end_angle       = 0,
	.width           = 20,
	.start_x 					= SCREEN_LENGTH/2-300,
	.end_x   					= 0,
	.start_y 					= SCREEN_WIDTH/2-330,
	.end_y   					= 0,
	.radius          = 10
};

/*�泵״̬��ƫ����*/
graphic_data_struct_t gds_fork_L = {
	.graphic_name[0] = 0,
	.graphic_name[1] = 1,
	.graphic_name[2] = 4,//ͼ����������ͼ�������������ж���
	.operate_tpye 	 = Op_Add,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
	.graphic_tpye    = UI_Circle,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
	.layer           = 0,//ͼ����
	.color           = Green,//��ɫ
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
	.graphic_name[2] = 5,//ͼ����������ͼ�������������ж���
	.operate_tpye 	 = Op_Add,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
	.graphic_tpye    = UI_Circle,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
	.layer           = 0,//ͼ����
	.color           = Green,//��ɫ
	.start_angle     = 0,
	.end_angle       = 0,
	.width           = 20,
	.start_x = SCREEN_LENGTH/2-300+100,
	.end_x   = 0,
	.start_y 					= SCREEN_WIDTH/2-330,
	.end_y   = 0,
	.radius          = 10
};

/*�Կ���ƫ����*/
graphic_data_struct_t gds_fetch_L = {
	.graphic_name[0] = 0,
	.graphic_name[1] = 0,
	.graphic_name[2] = 1,//ͼ����������ͼ�������������ж���
	.operate_tpye 	 = Op_Add,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
	.graphic_tpye    = UI_Line,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
	.layer           = 0,//ͼ����
	.color           = Orange,//��ɫ
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
	.graphic_name[2] = 2,//ͼ����������ͼ�������������ж���
	.operate_tpye 	 = Op_Add,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
	.graphic_tpye    = UI_Line,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
	.layer           = 0,//ͼ����
	.color           = Orange,//��ɫ
	.start_angle     = 0,
	.end_angle       = 0,
	.width           = 2,
	.start_x 			   = SCREEN_LENGTH/2+100,
	.end_x   			   = SCREEN_LENGTH/2+100,
	.start_y 			   = 0,
	.end_y   			   = SCREEN_WIDTH,
	.radius          = 0
};

/***********************�ַ�*************************/
/*���������ַ�*/
//��Ԯ
ext_client_custom_character_t char_rescue = {
	.grapic_data_struct = {
		.graphic_name[0] = 0,
		.graphic_name[1] = 0,
		.graphic_name[2] = 3,//ͼ����������ͼ�������������ж���
		.operate_tpye 	 = Op_Add,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
		.graphic_tpye    = UI_Character,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
		.layer           = 0,//ͼ����
		.color           = Yellow,//��ɫ
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
//����
ext_client_custom_character_t char_reveive = {
	.grapic_data_struct = {
		.graphic_name[0] = 0,
		.graphic_name[1] = 0,
		.graphic_name[2] = 4,//ͼ����������ͼ�������������ж���
		.operate_tpye 	 = Op_Add,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
		.graphic_tpye    = UI_Character,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
		.layer           = 0,//ͼ����
		.color           = Yellow,//��ɫ
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
//�泵
ext_client_custom_character_t char_fork = {
	.grapic_data_struct = {
		.graphic_name[0] = 0,
		.graphic_name[1] = 0,
		.graphic_name[2] = 5,//ͼ����������ͼ�������������ж���
		.operate_tpye 	 = Op_Add,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
		.graphic_tpye    = UI_Character,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
		.layer           = 0,//ͼ����
		.color           = Yellow,//��ɫ
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
/*צ��״̬�ַ�*/
ext_client_custom_character_t char_clamp = {
	.grapic_data_struct = {
		.graphic_name[0] = 0,
		.graphic_name[1] = 0,
		.graphic_name[2] = 6,//ͼ����������ͼ�������������ж���
		.operate_tpye 	 = Op_Add,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
		.graphic_tpye    = UI_Character,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
		.layer           = 0,//ͼ����
		.color           = Yellow,//��ɫ
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
//	.graphic_name[2] = 3,//ͼ����������ͼ�������������ж���
//	.operate_tpye 	 = Op_None,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//	.graphic_tpye    = 0,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//	.layer           = 0,//ͼ����
//	.color           = Yellow,//��ɫ
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
//	.graphic_name[2] = 4,//ͼ����������ͼ�������������ж���
//	.operate_tpye 	 = Op_None,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//	.graphic_tpye    = 0,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//	.layer           = 1,//ͼ����
//	.color           = Green,//��ɫ
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
//	.graphic_name[2] = 5,//ͼ����������ͼ�������������ж���
//	.operate_tpye 	 = Op_None,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//	.graphic_tpye    = 0,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//	.layer           = 1,//ͼ����
//	.color           = Green,//��ɫ
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
//	.graphic_name[2] = 6,//ͼ����������ͼ�������������ж���
//	.operate_tpye 	 = Op_None,//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//	.graphic_tpye    = 0,//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//	.layer           = 1,//ͼ����
//	.color           = Green,//��ɫ
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
*�� �� ��: Graphic_Change_Check
*����˵��: �����û��ͼ�εı䶯�����߽���ͼ��ĳ�ʼ��
*��    ��: ��
*�� �� ֵ: 0: �ޱ䶯�������ͣ�1���б䶯���跢��
**********************************************************************************************************/
//Flag_t last_g_Flag;
//uint8_t graphic_change_indicator = 0;
//int Graphic_Change_Check(void)
//{
//	
//	/*��û�б仯*/
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
*�� �� ��: JudgementCustomizeGraphics
*����˵��: ����ϵͳ�Զ���ͼ�η��ͺ���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
//װ��ͼ��
uint8_t load_time_flag = 0;

void JudgementCustomizeGraphics(void)
{
/*********************************ȷ��ID***********************************/
		custom_grapic_draw.sender_ID=JudgeReceive.RobotID;//������ID�������˶�ӦID
//		custom_grapic_draw.sender_ID=2;//������ID�������˶�ӦID
//				custom_grapic_draw.receiver_ID = 0x0102;	
		if(JudgeReceive.RobotID == 102) //��������
				custom_grapic_draw.receiver_ID = 0x0166;  //�������̲����ֿͻ���
		if(JudgeReceive.RobotID == 2)		//�췽����
				custom_grapic_draw.receiver_ID = 0x0102;	//�췽���̲����ֿͻ���
		
		
		custom_char_draw.sender_ID=JudgeReceive.RobotID;//������ID�������˶�ӦID
//		custom_char_draw.sender_ID=2;//������ID�������˶�ӦID
//				custom_char_draw.receiver_ID = 0x0102;	
		if(JudgeReceive.RobotID == 102) //��������
				custom_char_draw.receiver_ID = 0x0166;  //�������̲����ֿͻ���
		if(JudgeReceive.RobotID == 2)		//�췽����
				custom_char_draw.receiver_ID = 0x0102;	//�췽���̲����ֿͻ���
		
/*********************************ȷ����ͼ�α仯��***********************************/
		

/*********************************װ���Զ���ͼ������***********************************/
		
		//��Ԯ�����߽ӷ���
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
*�� �� ��: referee_data_load_Graphic
*����˵��: ͼ�����ݰ�װ��ͼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void referee_data_load_5_Graphic(graphic_data_struct_t g0, graphic_data_struct_t g1, graphic_data_struct_t g2, graphic_data_struct_t g3, graphic_data_struct_t g4)
{
	custom_grapic_draw.data_cmd_id=0x0103;//�������ͼ�Σ�����ID����ѯ����ϵͳ�ֲᣩ
	
	custom_grapic_draw.graphic_custom.grapic_data_struct[0] = g0;
	custom_grapic_draw.graphic_custom.grapic_data_struct[1] = g1;
	custom_grapic_draw.graphic_custom.grapic_data_struct[2] = g2;
	custom_grapic_draw.graphic_custom.grapic_data_struct[3] = g3;
	custom_grapic_draw.graphic_custom.grapic_data_struct[4] = g4;
}

/**********************************************************************************************************
*�� �� ��: referee_data_load_7_Graphic
*����˵��: ͼ�����ݰ�װ��ͼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void referee_data_load_7_Graphic(graphic_data_struct_t g0, graphic_data_struct_t g1, graphic_data_struct_t g2, graphic_data_struct_t g3, graphic_data_struct_t g4, graphic_data_struct_t g5, graphic_data_struct_t g6)
{
	custom_grapic_draw.data_cmd_id=0x0104;//�����߸�ͼ�Σ�����ID����ѯ����ϵͳ�ֲᣩ
	
	custom_grapic_draw.graphic_custom.grapic_data_struct[0] = g0;
	custom_grapic_draw.graphic_custom.grapic_data_struct[1] = g1;
	custom_grapic_draw.graphic_custom.grapic_data_struct[2] = g2;
	custom_grapic_draw.graphic_custom.grapic_data_struct[3] = g3;
	custom_grapic_draw.graphic_custom.grapic_data_struct[4] = g4;
	custom_grapic_draw.graphic_custom.grapic_data_struct[5] = g5;
	custom_grapic_draw.graphic_custom.grapic_data_struct[6] = g6;
}

/**********************************************************************************************************
*�� �� ��: referee_data_load_NumberUI
*����˵��: ͼ�����ݰ�װ�����֣�����/�ַ�/���Σ�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
//void referee_data_load_NumberUI(void)
//{
//	static unsigned int pitch = 0;
//	pitch = F405.Gimbal_100 * 10;
//	/*******************************Pitch������*********************************/
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
*�� �� ��: staight_line_load_Graphic
*����˵��: ͼ�����ݰ�װ��ֱ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
//void staight_line_load_Graphic(graphic_straight_line_t u_straight_line, int Op_type)
//{
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[0] = 1;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[1] = 1;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 1;//ͼ����
//	//���������ֽڴ������ͼ����������ͼ�������������ж���
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye = Op_type;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye = 0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer        = 0;//ͼ����
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].color        = u_straight_line.color;//��ɫ
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
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[2] = 2;//ͼ����
//	//���������ֽڴ������ͼ����������ͼ�������������ж���s
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].operate_tpye=Op_type;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].layer=0;//ͼ����
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].color=Green;//��ɫ
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
*�� �� ��: fetch_load_Graphic
*����˵��: �Կ���
*��    ��: ��������
*�� �� ֵ: ��
**********************************************************************************************************/
/******************************�Կ���**********************************/
//void fetch_load_Graphic(int Op_type)
//{
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[0] = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[1] = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 1;//ͼ����
//	//���������ֽڴ������ͼ����������ͼ�������������ж���
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye = Op_type;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye = 0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer        = fetch_L.layer;//ͼ����
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].color        = fetch_L.color;//��ɫ
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
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 2;//ͼ����
//	//���������ֽڴ������ͼ����������ͼ�������������ж���
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye = Op_type;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye = 0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer        = fetch_R.layer;//ͼ����
//	custom_grapic_draw.graphic_custom.grapic_data_struct[0].color        = fetch_R.color;//��ɫ
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
*�� �� ��: referee_data_pack_handle
*����˵��: ����ϵͳͼ�����ݴ������
*��    ��: uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len
*�� �� ֵ: ��
**********************************************************************************************************/
uint8_t seq = 0;	
extern volatile char dma_tx_busy;
void referee_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len)
{
	uint16_t frame_length = frameheader_len + cmd_len + len + crc_len;   //����֡����	

	memset(JudgeSend,0,frame_length);  //�洢���ݵ���������
	
	/*****֡ͷ���*****/
	JudgeSend[0] = sof;//����֡��ʼ�ֽ�
	memcpy(&JudgeSend[1],(uint8_t*)&len, sizeof(len));//����֡��data�ĳ���
	JudgeSend[3] = seq;//�����
	Append_CRC8_Check_Sum(JudgeSend,frameheader_len);  //֡ͷУ��CRC8

	/*****��������*****/
	memcpy(&JudgeSend[frameheader_len],(uint8_t*)&cmd_id, cmd_len);
	
	/*****���ݴ��*****/
	memcpy(&JudgeSend[frameheader_len+cmd_len], p_data, len);
	Append_CRC16_Check_Sum(JudgeSend,frame_length);  //һ֡����У��CRC16

	if (seq == 0xff) seq=0;
  else seq++;
	
	/*****�����ϴ�*****/
//	(void)USART1->SR;
//	(void)USART1->DR;
//	DMA1_Channel4->CNDTR = frame_length;
//	DMA_Cmd(DMA1_Channel4,ENABLE);
	
	/*****��ͨ���ڷ���*****/
//	USART_ClearFlag(USART1,USART_FLAG_TC);
//	for(i=0;i<frame_length;i++)
//	{
//	  USART_SendData(USART1,JudgeSend[i]);
//	  while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET); //�ȴ�֮ǰ���ַ��������
//	}
	
	/*****DMA���ڷ���*****/
	while(dma_tx_busy);
//	(void)USART1->SR;
//	(void)USART1->DR;
	DMA1_Channel4->CNDTR=frame_length;
	dma_tx_busy=1;
	DMA_Cmd(DMA1_Channel4,ENABLE);
}
/**********************************************************************************************************
*�� �� ��: GraphicSendtask
*����˵��: ͼ�η�������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
extern ext_student_interactive_char_header_data_t custom_char_draw;  //�Զ����ַ�����
int char_change_state,graphic_change_state;

void GraphicSendtask(void *pvParameters)
{
   while (1) {
    
			/*��������ͼ��*/
//			graphic_change_state = Graphic_Change_Check();
//		 
//			if(graphic_change_state!= Op_None){
//				JudgementCustomizeGraphics(graphic_change_state);
//				referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_grapic_draw,sizeof(custom_grapic_draw));
//			}
			
//			if(graphic_init_flag != 0){		//��ʼ��δ���
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

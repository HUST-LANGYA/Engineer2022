#ifndef __GRAPHICS_SEND_TASK_H
#define __GRAPHICS_SEND_TASK_H

#include "stdint.h"
#include "main.h"

#define LOAD_TIME_FLAG_MAX 5	

#define SEND_MAX_SIZE    128    //�ϴ��������ĳ���
#define frameheader_len  5       //֡ͷ����
#define cmd_len          2       //�����볤��
#define crc_len          2       //CRC16У��
//��Ļ�ֱ���1920*1080
#define SCREEN_WIDTH 1080
#define SCREEN_LENGTH 1920			//��Ļ�ֱ���
/*��������*/
#define Op_None 0
#define Op_Add 1
#define Op_Change 2
#define Op_Delete 3
#define Op_Init		1			//��ʼ����Ҳ��������ͼ��
/*��ɫ*/
#define Red_Blue 0
#define Yellow   1
#define Green    2
#define Orange   3
#define Purple	 4
#define Pink     5
#define Cyan		 6
#define Black    7
#define White    8
/*ͼ������*/
#define UI_Line 				0		//ֱ��
#define UI_Rectangle		1		//����
#define UI_Circle    	2		//��Բ
#define UI_Ellipse   	3		//��Բ
#define UI_Arc	 				4		//Բ��
#define UI_Floating   	5		//������
#define UI_Integer		 	6		//������
#define UI_Character  	7		//�ַ�

//ֱ������
typedef struct
{
	short start_x;
	short end_x;
	short start_y;
	short end_y ;
	uint8_t layer;
	uint8_t color;
}graphic_straight_line_t;


//ͼ������
typedef __packed struct
{
	uint8_t graphic_name[3];
	uint32_t operate_tpye:3;
	uint32_t graphic_tpye:3;
	uint32_t layer:4;
	uint32_t color:4;
	uint32_t start_angle:9;
	uint32_t end_angle:9;
	uint32_t width:10;
	uint32_t start_x:11;
	uint32_t start_y:11;
	uint32_t radius:10;
	uint32_t end_x:11;
	uint32_t end_y:11;
}graphic_data_struct_t;	

/*����ϵͳ������Ϣ��*���������ϵͳ����Э��*/
typedef __packed struct
{
	uint8_t data[113];
}robot_interactive_data_t;//��������

typedef __packed struct
{
	uint8_t operate_tpye;		//0�ղ���  1ɾ������ͼ��  2ɾ������ͼ��
	uint8_t layer;					//ͼ���  0~9
}ext_client_custom_graphic_delete_t;//�ͻ���ɾ��ͼ��

typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct;
}ext_client_custom_graphic_single_t;//�ͻ��˻���һ��ͼ��

typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct[2];
}ext_client_custom_graphic_double_t;//�ͻ��˻�������ͼ��

typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct[5];
}ext_client_custom_graphic_five_t;//�ͻ��˻������ͼ��

typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct[7];
}ext_client_custom_graphic_seven_t;//�ͻ��˻����߸�ͼ��

typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct;
	char data[30];
}ext_client_custom_character_t;//�ͻ��˻����ַ�

//����������Ϣ
typedef __packed struct
{
	uint16_t data_cmd_id;	//���ݶ�����ID
	uint16_t sender_ID;	//������ID
	uint16_t receiver_ID;	//������ID
	ext_client_custom_graphic_seven_t graphic_custom;//�Զ���ͼ������: �ͻ��˻����߸�ͼ��
}ext_student_interactive_header_data_t;	

typedef __packed struct
{
	uint16_t data_cmd_id;	//���ݶ�����ID
	uint16_t sender_ID;	//������ID
	uint16_t receiver_ID;	//������ID
	ext_client_custom_character_t char_custom;//�Զ����ַ�������
}ext_student_interactive_char_header_data_t;



//void JudgementDataSend(void);
//void JudgementCustomizeGraphics(int Op_type);
void JudgementCustomizeGraphics(void);
void referee_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len);
void referee_data_load_Graphic(int Op_type);

void referee_data_load_5_Graphic(graphic_data_struct_t g0, graphic_data_struct_t g1, graphic_data_struct_t g2, graphic_data_struct_t g3, graphic_data_struct_t g4);
void referee_data_load_7_Graphic(graphic_data_struct_t g0, graphic_data_struct_t g1, graphic_data_struct_t g2, graphic_data_struct_t g3, graphic_data_struct_t g4, 
	graphic_data_struct_t g5, graphic_data_struct_t g6);

void fetch_load_Graphic(int Op_type);

//void referee_data_load_shootUI(uint8_t operate_type,uint8_t robot_level);
//void referee_data_load_NumberUI(void);

void GraphicSendtask(void *pvParameters);

#endif

#include "remote.h"

rc_ctrl_t rc_ctrl, rc_ctrl_last;
short q_rising_flag,w_rising_flag,e_rising_flag,r_rising_flag,
		a_rising_flag,s_rising_flag,d_rising_flag,f_rising_flag,g_rising_flag,
		z_rising_flag,x_rising_flag,c_rising_flag,v_rising_flag,b_rising_flag,
		shift_rising_flag,ctrl_rising_flag,mouse_Press_l_rising_flag,mouse_Press_r_rising_flag;
short Press_Key_x_Flag,Press_Key_z_Flag;

/**
  * @brief  discoding message
  * @param  
  * @retval None 	
  */ 
void remote_receive(volatile unsigned char rx_buffer[])
{	
	rc_ctrl.rc.ch0 = (rx_buffer[0]| (rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0
	rc_ctrl.rc.ch1 = ((rx_buffer[1] >> 3) | (rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
	rc_ctrl.rc.ch2 = ((rx_buffer[2] >> 6) | (rx_buffer[3] << 2) | (rx_buffer[4] << 10)) & 0x07ff;//!< Channel 2
	rc_ctrl.rc.ch3 = ((rx_buffer[4] >> 1) | (rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
	rc_ctrl.rc.s1 = ((rx_buffer[5] >> 4)& 0x0003); //!< Switch left
	rc_ctrl.rc.s2 = ((rx_buffer[5] >> 6)& 0x0003);

	rc_ctrl.mouse.x = rx_buffer[6] | (rx_buffer[7] << 8); //!< Mouse X axis
	rc_ctrl.mouse.y = rx_buffer[8] | (rx_buffer[9] << 8); //!< Mouse Y axis
	rc_ctrl.mouse.z = rx_buffer[10] | (rx_buffer[11] << 8); //!< Mouse Z axis
	rc_ctrl.mouse.press_l = rx_buffer[12]; //!< Mouse Left Is Press ?
	rc_ctrl.mouse.press_r = rx_buffer[13]; //!< Mouse Right Is Press ?

	rc_ctrl.key.w = rx_buffer[14]&0x01; // KeyBoard value
	rc_ctrl.key.s = (rx_buffer[14]>>1)&0x01;
	rc_ctrl.key.a = (rx_buffer[14]>>2)&0x01;
	rc_ctrl.key.d = (rx_buffer[14]>>3)&0x01;
	rc_ctrl.key.shift = (rx_buffer[14]>>4)&0x01;
	rc_ctrl.key.ctrl = (rx_buffer[14]>>5)&0x01;
	rc_ctrl.key.q = (rx_buffer[14]>>6)&0x01;
	rc_ctrl.key.e = (rx_buffer[14]>>7)&0x01;	
	rc_ctrl.key.r = (rx_buffer[15])&0x01;
	rc_ctrl.key.f = (rx_buffer[15]>>1)&0x01;
	rc_ctrl.key.g = (rx_buffer[15]>>2)&0x01; 
	rc_ctrl.key.z = (rx_buffer[15]>>3)&0x01;
	rc_ctrl.key.x = (rx_buffer[15]>>4)&0x01;
	rc_ctrl.key.c = (rx_buffer[15]>>5)&0x01;
	rc_ctrl.key.v = (rx_buffer[15]>>6)&0x01;
	rc_ctrl.key.b = (rx_buffer[15]>>7)&0x01;

	
	g_Flag.rc_cnt = 0;
	g_Flag.rc_connect = 1;
}

/*
 * @brief initial remote control number.
 */
/**
  * @brief  ң������������
  * @param  None
  * @retval None
  */
void rc_reset(void)
{
    rc_ctrl.rc.ch0 = 1024;
    rc_ctrl.rc.ch1 = 1024;
    rc_ctrl.rc.ch2 = 1024;
    rc_ctrl.rc.ch3 = 1024;
    rc_ctrl.mouse.x = 0;
    rc_ctrl.mouse.y = 0;
    rc_ctrl.mouse.z = 0;
    rc_ctrl.mouse.press_l = 0;                                                
    rc_ctrl.mouse.press_r = 0;

    rc_ctrl.key.w = 0;
    rc_ctrl.key.s = 0;                            
    rc_ctrl.key.a = 0;
    rc_ctrl.key.d = 0;
    rc_ctrl.key.q = 0;
    rc_ctrl.key.e = 0;
    rc_ctrl.key.r = 0;
    rc_ctrl.key.f = 0;
    rc_ctrl.key.shift = 0;
    rc_ctrl.key.ctrl = 0;

    rc_ctrl.rc.s1 = 2;
    rc_ctrl.rc.s2 = 2;
}   

/**
  * @brief  �ı�״̬�����ڼ�Ⲧ�ˣ�����״̬����ʼ״̬
  * @param  None
  * @retval None
  */
void key_refresh(void)
{
	rc_ctrl_last.rc.s1 = rc_ctrl.rc.s1;
	rc_ctrl_last.rc.s2 = rc_ctrl.rc.s2;
	rc_ctrl_last.key.q = rc_ctrl.key.q;
	rc_ctrl_last.key.w = rc_ctrl.key.w;
	rc_ctrl_last.key.e = rc_ctrl.key.e;
	rc_ctrl_last.key.r = rc_ctrl.key.r;
	rc_ctrl_last.key.a = rc_ctrl.key.a;
	rc_ctrl_last.key.s = rc_ctrl.key.s;
	rc_ctrl_last.key.d = rc_ctrl.key.d;
	rc_ctrl_last.key.f = rc_ctrl.key.f;
	rc_ctrl_last.key.g = rc_ctrl.key.g;
	rc_ctrl_last.key.z = rc_ctrl.key.z;
	rc_ctrl_last.key.x = rc_ctrl.key.x;
	rc_ctrl_last.key.c = rc_ctrl.key.c;
	rc_ctrl_last.key.v = rc_ctrl.key.v;
	rc_ctrl_last.key.b = rc_ctrl.key.b;
	rc_ctrl_last.mouse.press_l = rc_ctrl.mouse.press_l;
	rc_ctrl_last.mouse.press_r = rc_ctrl.mouse.press_r;
}

void key_rising_check(void)
{
	q_rising_flag = rc_ctrl.key.q - rc_ctrl_last.key.q;
	w_rising_flag = rc_ctrl.key.w - rc_ctrl_last.key.w;
	e_rising_flag = rc_ctrl.key.e - rc_ctrl_last.key.e;
	r_rising_flag = rc_ctrl.key.r - rc_ctrl_last.key.r;
	
	a_rising_flag = rc_ctrl.key.a - rc_ctrl_last.key.a;
	s_rising_flag = rc_ctrl.key.s - rc_ctrl_last.key.s;
	d_rising_flag = rc_ctrl.key.d - rc_ctrl_last.key.d;
	f_rising_flag = rc_ctrl.key.f - rc_ctrl_last.key.f;
	g_rising_flag = rc_ctrl.key.g - rc_ctrl_last.key.g;
	
	z_rising_flag = rc_ctrl.key.z - rc_ctrl_last.key.z;
	x_rising_flag = rc_ctrl.key.x - rc_ctrl_last.key.x;
	c_rising_flag = rc_ctrl.key.c - rc_ctrl_last.key.c;
	v_rising_flag = rc_ctrl.key.v - rc_ctrl_last.key.v;
	b_rising_flag = rc_ctrl.key.b - rc_ctrl_last.key.b;
	
	ctrl_rising_flag = rc_ctrl.key.ctrl - rc_ctrl_last.key.ctrl;
	shift_rising_flag = rc_ctrl.key.shift - rc_ctrl_last.key.shift;
	
	mouse_Press_l_rising_flag = rc_ctrl.mouse.press_l - rc_ctrl_last.mouse.press_l;
	mouse_Press_r_rising_flag = rc_ctrl.mouse.press_r - rc_ctrl_last.mouse.press_r;
}


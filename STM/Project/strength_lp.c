#include "strength_lp.h"
#include "SYSTICK.H"
#include "Types.h"
#include "exti.h"
#include "model.h"
#include "user_config.h"
#include <stdlib.h>
#include "show_work.h"
#include "led_beep.h"
#include "key.h"
#include "work_btn.h"

uint8 	g_strength_pl_status = 0; 																			//0�ر� 1�� 2����
uint8		g_strength_pl_beep = 0;																					//�Ƿ�����λ��ʾ��

/////////////////////////////////////////////////////////////////////////////////////////

#define STREANGTH_PL_BUFF_LEN					50

uint8   g_strength_pl_exercise_lock = 0;																							//�Ƿ�����Ƭ��
uint8  *g_strength_pl_btn1 = (g_send_buff + BUFF_LEN - STREANGTH_PL_BUFF_LEN);				//����1��¼
uint8  *g_strength_pl_btn2 = (g_send_buff + (BUFF_LEN - STREANGTH_PL_BUFF_LEN * 2));	//����2��¼
uint8   g_strength_pl_btn1_pos = 0;
uint8   g_strength_pl_btn2_pos = 0;
	
//��������״̬
int eg_strength_lp_net_set_status(unsigned int status, unsigned int space_time, unsigned int one_time, unsigned char beep)
{
	//����״̬
	g_strength_pl_status = status;
	g_strength_pl_beep = beep;
	
	//���ݸ�λ
	g_strength_pl_exercise_lock = 0;
	g_strength_pl_btn1_pos = 0;
	g_strength_pl_btn2_pos = 0;
	memset(g_strength_pl_btn1, 0, STREANGTH_PL_BUFF_LEN);
	memset(g_strength_pl_btn2, 0, STREANGTH_PL_BUFF_LEN);
	
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ϴ�����ֵ
void eg_strength_lp_push(void)
{
	//��ʾ��
	if(g_strength_pl_beep)
		GROUP_PRO_SUCCEED;
	
	g_send_len = ModelMakeStrengthReq((char *)g_send_buff, 0, 0, 0, 
		(char *)g_strength_pl_btn1, (char *)g_strength_pl_btn2);
	UDP_PUSH;
}


void eg_strength_lp_btn(unsigned int interval, char btn_key)
{
	if(g_strength_pl_status == 0)
		return;
	
	//�����˾Ͳ�Ҫ�ٴ�����
	if(g_strength_pl_exercise_lock)
		return;
	
	//��¼���1 
	if(btn_key == 1 && g_strength_pl_btn1_pos < STREANGTH_PL_BUFF_LEN)
		g_strength_pl_btn1[g_strength_pl_btn1_pos++] = interval > 0xFF ? 0xFF : interval;
	
	//��¼���2
	if(btn_key == 2 && g_strength_pl_btn2_pos < STREANGTH_PL_BUFF_LEN)
		g_strength_pl_btn2[g_strength_pl_btn2_pos++] = interval > 0xFF ? 0xFF : interval;
}


uint8 btn4_s = 0;

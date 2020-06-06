#include "work_rfid.h"
#include "SYSTICK.H"
#include "usart.h"
#include "Types.h"
#include "user_config.h"
#include "led_beep.h"
#include "model.h"
#include "work_btn.h"
#include "exti.h"
#include "strength.h"
#include "strength_lp.h"
#include "key.h"
#include <stdlib.h>
#include "adc.h"


//�ۻ�����
unsigned char 	g_btn_adc_count = 0;
//����ʱ��
unsigned int		g_btn1_trigger_time = 0;


//�������
unsigned short 	g_btn1_limit_time = 100;
//��������ʱ��
unsigned int 	  g_tickt_free = 0;
//���������ٶ�
unsigned short  g_btn1_speed = 0;


void eg_check_btn1(void)
{
	//��Ĭ���豸����Ҫ������ȷ��������
	if(g_user_config.type == 1)
		return;
	
	if(g_adc1_value >= 3500)
	{
		//����
		if(g_tickt_free > g_btn1_limit_time)
		{
			g_btn1_speed = g_time_now_ms - g_btn1_trigger_time;
			g_btn1_trigger_time = g_time_now_ms;
			g_btn_adc_count++;
		}
		g_tickt_free = 0;
	}
	else
	{
		g_tickt_free++;
	}
}

//int eg_counter_work()
//{
//	//û�����ݾͲ�Ҫ����
//	if(g_btn_adc_count == 0)
//		return 0;
//	
//	//���Ʒ���Ƶ��
//	if((unsigned int)(g_time_now - g_btn1_last_push_time) < 100)
//		return 0;
//	g_btn1_last_push_time = g_time_now;
//		
//	//����������Ϣ
//	g_send_len = ModelMakeBtnReq((char *)g_send_buff, 1, g_btn_adc_count, g_btn1_speed);
//	UDP_PUSH;
//	
//	g_btn_adc_count = 0;
//	
//	return 1;
//}


//�������ʱ��
unsigned int g_btn1_last_push_time = 0;
int eg_btn_loop(void)
{
//	if(eg_strength_loop() != 0)					//����������е��Ҫ��������		
//		return 1;
//	if(eg_strength_lp_loop() != 0)			//���������е
//		return 1;
//	return eg_counter_work();						//������
	
	/////////////////////////////////////////////////////////////////
	
	//û�����ݾͲ�Ҫ����
	if(g_btn_adc_count == 0)
		return 0;
	
	//���Ʒ���Ƶ��
	if((unsigned int)(g_time_now - g_btn1_last_push_time) < 100)
		return 0;
	g_btn1_last_push_time = g_time_now;
	
	//����-��������
	g_send_len = ModelMakeBtnReq((char *)g_send_buff, 1, g_btn_adc_count, g_btn1_speed);
	UDP_PUSH;
	g_btn_adc_count = 0;
	
	return 1;
}

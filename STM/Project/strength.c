#include "strength.h"
#include "usart.h"
#include "user_config.h"
#include "show_work.h"
#include "gui.h"
#include "Lcd_Driver.h"
#include "SYSTICK.h"
#include "exti.h"
#include "work_btn.h"

//��¼״̬
unsigned char 	g_strength_status = 0;

//��������-����������
unsigned char		g_strength_buff[18];
unsigned char		g_strength_buff_len = 0;
unsigned int		g_strength_values[3];
unsigned int		g_strength_value_pos = 0;

//��¼����index
unsigned int		g_strength_index = 0;

//��ʾ����
unsigned short	g_strength_count = 0;

//��¼��СֵУ׼��
unsigned int		g_strength_min = 0xFFFFFFFF;

//��¼��ʾ��index
unsigned int 	  g_streng_show_index = 0;

//��¼֮ǰ��ʾ����Ϣ������ˢ���õ�
char g_strength_old_show[4] = {0};

#define EG_STRENGTH_VALUE					"\xA5\x02\xA7"
#define EG_STRENGTH_VALUE_ALL			"\xA5\x03\xA6"

#define EG_STRENGTH_SHOW_LEFT			20

void eg_strength_close(void)
{
		sendDatatoRS232(EG_STRENGTH_VALUE, 3);
		g_strength_status = 0;
}


int eg_strength_sync_status(unsigned char staus, unsigned int index, unsigned char *out_buff, unsigned short *out_len)
{	
	if(index != g_strength_index)
	{
		g_strength_count = 0;
		g_strength_index = index;
	}
	
	//����
	if(staus == 1 && g_strength_status == 0)
	{
		CHECK_SHOW_LIVE(2);
		
		memset(g_strength_old_show, 0, sizeof(g_strength_old_show));
		g_strength_count = 0;
		g_strength_min = 0xFFFFFFFF;
		
		//����
		sendDatatoRS232(EG_STRENGTH_VALUE_ALL, 3);
	}
	
	//�ر�
	if(staus == 0 && g_strength_status != 0)
	{
		eg_strength_close();
		
		//�ر���ʾ
		CHECK_SHOW_LIVE(2);
		SHOW_LIVE_CLEARE;
	}
	
	//�������״̬
	if(staus == 2 && g_strength_status == 1)
	{
	}
	
	//�˳�����״̬
	if(staus == 1 && g_strength_status == 2)
	{
	}
	
	g_strength_status = staus;
	
	//���ü�����������300����
	g_btn1_limit_time = 300;
	
	////////////////////////////////////
	*out_len = ModelMakeStrengthStatusAck((char *)out_buff, 1, 0, g_streng_show_index, g_strength_count/2, g_strength_status);
	
	return 1;
}


int eg_strength_add_data(unsigned char data)
{
	if(g_user_config.type != 3 || g_w5500_status == 0)
		return 0;
	
	if(g_strength_buff_len < 9) {
		g_strength_buff[g_strength_buff_len] = data;
		g_strength_buff_len++;
	}
	return 1;
}

int eg_strength_check_view(void)
{
	static char send_time = 0;
	if(g_show_live == 2)
		return 0;
	
	if(g_time_now - send_time < 200)
		return 1;
	send_time = g_time_now;

	g_send_len = ModelMakeHeader((char *)g_send_buff, 167, 0);
	UDP_PUSH;
	return 1;
}

void eg_strength_push(void)
{
	static uint32 strength_last_push_time = 0;
	char show[4] = {0};
	
	if(g_btn_adc_count > 0) 
	{
		//����ʱ����1.2��
		g_btn_adc_count = 0;
		if(g_time_now - strength_last_push_time < 120)
			return;
		
		//����ɼ��������㲻���Ͳ�Ҫ����
		if(g_strength_value_pos < 3)
			return;
		strength_last_push_time = g_time_now;
		g_strength_count++;
		
		if(g_strength_count % 2 == 0)
		{			
			//��������
			g_send_len = ModelMakeStrengthReq((char *)g_send_buff, 
			g_strength_values[0], g_strength_values[1], g_strength_values[2], 0, 0);
			UDP_PUSH;
			
			//���´���
			if(g_strength_status == 1)
			{
				g_show_time = 0xFF;
				snprintf(show, 4, "%03d", g_strength_count/2);
				show_num_pic(show, EG_STRENGTH_SHOW_LEFT, 80, 0, 0xFFFF);
				g_streng_show_index = g_draw_pic_number;
			}
		}
	}
}

int eg_strength_loop(void)
{
	unsigned char key = 0, i = 0;
	unsigned char *p = 0;
	char show[4] = {0};
	
	//ֹͣ״̬�Ͳ�Ҫִ��
	if(g_strength_status == 0)
		return 0;
	
	//����Ƿ�������״̬
	show_check_standby();					
	
	//���ͼ�����ر�������е
	if(eg_strength_check_view())
		return 0;
	
	//��������
	eg_strength_push();
	
	///////////////////////////////////////////////////////////////////////////////////////////
	
	if(g_strength_buff_len < 9)
		return 1;
	
	//��֤�����Ƿ���Ч
	for (i = 0;i < 8; i++)
		key = key ^ g_strength_buff[i];
	
	//��ȡ����
	if (key == g_strength_buff[8]) {
		
		//����ֵ
		if(g_strength_value_pos >= 3)
		{
			g_strength_values[0] = g_strength_values[1];
			g_strength_values[1] = g_strength_values[2];
			g_strength_value_pos = 2;
		}
		p = (unsigned char *)(g_strength_values + g_strength_value_pos);
		g_strength_value_pos++;
		
		//��ȡ��β�����ֵ
		for (i= 0; i < 4; i++)
				p[i] = g_strength_buff[6-i];
		
		//��ȥ��СУ׼ֵ
		if(g_strength_min > g_strength_values[g_strength_value_pos - 1]) {
			g_strength_min = g_strength_values[g_strength_value_pos - 1];
		}
		g_strength_values[g_strength_value_pos - 1] -= g_strength_min;
		
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//��ʾ��ǰ����
		
		//���˵�û�仯������
		snprintf(show, 4, "%03d", (g_strength_values[g_strength_value_pos - 1] + 500)/1000);
		for(i = 0; i < 3; i++) {
				if(show[i] != g_strength_old_show[i])
					break;
		}
		
		if(i < 3) {
			
			//�������������ʾ������ˢ��
			if(g_streng_show_index != g_draw_pic_number)
			{
				i = 0;
				g_streng_show_index |= 0xF0000000;
				g_streng_show_index++;
			}
			
			//��ʾ���ֱ仯
			if(g_strength_status == 1)
			{
				g_show_time = 0xFF;
				show_num_pic(show + i, EG_STRENGTH_SHOW_LEFT + 21 * i, 40, 0, 0xFFFF);
				memcpy(g_strength_old_show, show, sizeof(show));
				g_streng_show_index = g_draw_pic_number;
			}
		}
	}
	
	g_strength_buff_len = 0;
	return 1;
}

#include "work_ttl.h"
#include "usart.h"
#include "user_config.h"
#include "string.h"
#include "udp_tool.h"
#include "model.h"
#include "Finger.h"
#include "led_beep.h"
#include "Relay.h"
#include "systick.h"
#include "work_rfid.h"
#include "finger_parse.h"



//void USART1_IRQHandler(void)
//{
//  if(USART_GetITStatus(USART_TTL, USART_IT_RXNE) != RESET)
//  { 	
//		USART1->DR=USART_ReceiveData(USART_TTL);//�����ܵ�������ֱ�ӷ��ش�ӡ
//  } 
//}

//USART2->DR = byte; printf��ӡ��ȥ

void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART_TTL, USART_IT_RXNE) != RESET)
  { 	
		uint8 byte = USART_ReceiveData(USART_TTL);
		
		//USART2->DR = byte;

		//������������
		if(g_data_type <= DATA_TYPE_TTL)
		{
			//��ֹ����Խ��
			if(g_rev_len >= BUFF_LEN)
					return;
			
			//�����ͼ�����
			if(g_data_type != DATA_TYPE_TTL)
				g_rev_len = 0;
			
			g_rev_buff[g_rev_len] = byte;
			g_rev_len++;
			g_data_type = DATA_TYPE_TTL;
			g_data_time = g_time_now;
		}
  } 
}

//������ר��ͨ��
int eg_ttl_loop(void)
{
	if(g_data_type != DATA_TYPE_TTL)
		return 0;
	
	/////////////////////////////////////////////////////////////////////����ָ��ģ��
	if(eg_finger_parse() == 1)
	{
		eg_clear_data();
		return 1;
	}
	
	////////////////////////////////////////////////////////////////////////////ˢ��
	if(eg_rfid_7941D3_ttl_read((unsigned char *)g_rev_buff, g_rev_len) == 1)
	{
		eg_clear_data();
		return 1;
	}
	
	///////////////////////////////////////////////////////////TTL�յ�������͸����ȥ
	if(g_user_config.type != 1 && g_time_now - g_data_time > 20)
	{
		g_send_len = ModelMakePushTTLExtReq((char *)g_send_buff, (char *)g_rev_buff, g_rev_len);
		UDP_PUSH;
		
		eg_clear_data();
		return 1;
	}
	
	return 0;
}

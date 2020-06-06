#include "finger_parse.h"
#include "usart.h"
#include "user_config.h"
#include "string.h"
#include "udp_tool.h"
#include "model.h"
#include "Finger.h"
#include "led_beep.h"
#include "Relay.h"
#include "systick.h"

uint32	g_rs232_package_num = 1;
char		g_finger_parge_ext_first = 0;
uint32  g_last_wake_up_finger_time = 0;

//ֻ���Ž��豸����װָ��
void eg_finger_init(void)
{
	
	//�����Ž��豸������ָ��ģ��
	if(g_user_config.type != 1) 
		return;
	
	////////////////��ʱά����ָ��ģ��-��Ҫ����
	if((g_finger_is == 0 && g_time_now - g_last_wake_up_finger_time >= 500) || 
		(g_time_now - g_last_wake_up_finger_time >= 60000))
	{
		g_last_wake_up_finger_time = g_time_now;
		g_send_len = FingerMakeBuff((unsigned char *)g_send_buff, FINGER_SYS_STATUS, 0, 0, 0);
		sendDatatoTTL((char *)g_send_buff, g_send_len);
		g_send_len = 0;
	}
}

int eg_finger_parse(void)
{
	FINGER_HEADER_S *finger_header = 0;
	char					 	*content_buff = 0;
	int 		 				 content_len = 0;
	
	
	//�����Ž��豸������ָ��ģ��
	if(g_user_config.type != 1) 
		return 0;
	

	//������ϸ���
	if((finger_header = FingerCheckBuff((unsigned char *)g_rev_buff, g_rev_len, 
		&content_buff, &content_len, g_rs232_package_num, g_finger_parge_ext_first)) != 0)
	{
		//û�н�������Ҫ��������
		if(content_len == -1)
			return 0;
		
		g_send_len = 0;
		
		if(finger_header->command != 0)
		{
			//�����1
			g_rs232_package_num ++ ;
			//��װָ��ģ��
			g_finger_is = 1;
			//��չЭ���������
			g_finger_parge_ext_first = 0;
			
			//������ָ��
			if(finger_header->command == FINGER_IDENTIFY_FREE)
			{
				if(finger_header->flag == 0x61)
				{
					GROUP_PRO_FINGER;
					g_send_len = ModelMakeFingerReq((char *)g_send_buff, (short)finger_header->param);
					UDP_PUSH;
				}
				else
				{
					GROUP_PRO_FAILED;
				}
			}
			
			//����û�
			else if(finger_header->command == 0x17)
			{
				g_send_len = ModelMakeCodeAck((char *)g_send_buff, 102, finger_header->flag);
			}
			
			//ɾ���û�
			else if(finger_header->command == 0xF3)
			{
				g_send_len = ModelMakeCodeAck((char *)g_send_buff, 104, finger_header->flag);
			}
			
			//ָ�ƵǼǽ��
			else if(finger_header->command == 0x15)
			{
				g_finger_ing = 0;
				
				if(finger_header->flag == 0x61)
				{
					GROUP_PRO_SUCCEED;
				}
				else
				{
					GROUP_PRO_FAILED;
				}
				
				g_send_len = ModelMakeFingerScanReq((char *)g_send_buff, finger_header->param & 0xFF, finger_header->flag == 0x61 ? 1:0);
				UDP_PUSH;
			}
			
			//��ȡָ��ģ��
			else if(finger_header->command == 0x89)
			{
				if(finger_header->flag != 0x61)
					g_send_len = ModelMakeFingerTmpAck((char *)g_send_buff, (char *)g_rev_buff + 26, 0);
				else
					g_send_len = ModelMakeFingerTmpAck((char *)g_send_buff, (char *)g_rev_buff + 26, (finger_header->param & 0xFFFF) + 4);
			}
			
			//�豸��λ
			else if(finger_header->command == 0xD0)
			{
				g_send_len = ModelMakeCodeAck((char *)g_send_buff, 112, finger_header->flag);
			}
			
			//ɾ�����е��û�
			else if(finger_header->command == 0xF5)
			{
				g_send_len = ModelMakeCodeAck((char *)g_send_buff, 114, finger_header->flag);
			}
			
			//ɾ�����ݿ�
			else if(finger_header->command == 0xF8)
			{
				g_send_len = ModelMakeCodeAck((char *)g_send_buff, 114, finger_header->flag);
			}
			
			//��ȡ���е��û�
			else if(finger_header->command == 0xA0)
			{
				if(content_len > 17)
				{
					g_send_len = ModelMakeHeader((char *)g_send_buff, 110, content_len);
					memcpy(g_send_buff + g_send_len, content_buff, content_len);
					g_send_len += content_len;
					UDP_SYNC_ACK;
					delay_ms(100);
				}
				
				g_rev_len = 0;
				g_data_type = 0;
				if(content_len > 17 && 
					(finger_header->flag == 0x61 || finger_header->flag == 0) && 
					(*(short *)(content_buff+2)) - 1 > *(short *)(content_buff+4))
				{
					FingerCheckMore(0xA0, *(short *)(content_buff+2), *(short *)(content_buff+4));
					delay_ms(10);
				}
				else
				{
					FingerCheckMore(0xA0, 0, 0);
					g_send_len = ModelMakeCodeAck((char *)g_send_buff, 110, 0);
				}
			}
			
			//��ȡָ��ģ��״̬
			else if(finger_header->command == 0x14)
			{
				if((g_time_now - g_last_wake_up_finger_time) > 10)
				{
					g_send_len = ModelMakeCodeAck((char *)g_send_buff, 116, finger_header->param);
				}
			}
		}
		
		UDP_SYNC_ACK;
		return 1;
	}
	
	return 0;
}

int eg_finger_net_comment(unsigned char type, unsigned char *content_buff, int content_len,
	unsigned char *out_buff, unsigned short *out_len, unsigned char *ip, unsigned short port)
{
	unsigned int tmp_num_32 = 0;
	
	//���ָ��
	if (type == 101)
	{
		if(content_len > 200)
		{
			tmp_num_32 = *((uint16_t *)content_buff);
			
			if(tmp_num_32 > 0 && 
				tmp_num_32 < 2000 && 
				FingerCheckTmp((unsigned char *)(content_buff + 2), content_len-2) == 1){
				
					g_send_len = FingerMakeBuff((unsigned char *)g_send_buff, 0x17, (char *)&tmp_num_32, content_len-2, 0x71);
					memcpy(out_buff + g_send_len, content_buff + 2, content_len-2);
					g_send_len += content_len-2;
					
					TTL_SYNC_SEND(ip, port);
					return 1;
			}
		}
		
		*out_len = ModelMakeCodeAck((char *)out_buff, 102, 0);
	}
	
	//ɾ���û�
	else if (type == 103)
	{
		if(content_len >= 2)
		{
			tmp_num_32 = *((uint16_t *)content_buff);
			g_send_len = FingerMakeBuff((unsigned char *)g_send_buff, 0xF3, (char *)&tmp_num_32, 0, 0);
			TTL_SYNC_SEND(ip, port);
		}
	}
	
	//����¼��ָ��
	else if (type == 105)
	{
		if(g_finger_ing <= 0)
		{
			if(content_len >= 2)
			{
				tmp_num_32 = *((uint16_t *)content_buff);
		
				//ɾ������¼��ģʽ
				g_send_len = FingerMakeBuff((unsigned char *)g_send_buff, 0x15, (char *)&tmp_num_32, 0, 0x71);
				TTL_SYNC_SEND(ip, port);
			
				g_finger_ing = 150;
				GROUP_PRO_FINGER;
			}
			*out_len = ModelMakeCodeAck((char *)out_buff, 106, content_len >= 2  ? 1:0);
		}
		else
		{
			*out_len = ModelMakeCodeAck((char *)out_buff, 106, 5);
		}
	}
	
	//��ȡģ��
	else if (type == 107)
	{
		if(content_len >= 2)
		{
			tmp_num_32 = *((uint16_t *)content_buff);
			g_send_len = FingerMakeBuff((unsigned char *)g_send_buff, 0x89, (char *)&tmp_num_32, 0, 1);
			TTL_SYNC_SEND(ip, port);
		}
	}
	
	//��ȡָ�������е��û�
	else if (type == 109)
	{
		tmp_num_32 = 4000;
		g_send_len = FingerMakeBuff((unsigned char *)g_send_buff, 0xA0, (char *)&tmp_num_32, 0, 0);
		g_finger_parge_ext_first = 1;
		TTL_SYNC_SEND(ip, port);
	}
	
	//ָ�Ƹ�λ
	else if (type == 111)
	{
		g_send_len = FingerMakeBuff((unsigned char *)g_send_buff, 0xD0, 0, 0, 0);
		TTL_SYNC_SEND(ip, port);
	}
	
	//ɾ�����е��û�
	else if (type == 113)
	{
		g_send_len = FingerMakeBuff((unsigned char *)g_send_buff, 0xF8, 0, 0, 0); //0xF5
		TTL_SYNC_SEND(ip, port);
	}

	//��ȡָ��ģ��״̬
	else if (type == 115)
	{
		g_send_len = FingerMakeBuff((unsigned char *)g_send_buff, 0x14, 0, 0, 0);
		TTL_SYNC_SEND(ip, port);
	}
	else
	{
		return 0;
	}
	
	return 1;
}

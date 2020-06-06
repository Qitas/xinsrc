/**
******************************************************************************
* @file   		udp_demo.c
* @author  		WIZnet Software Team 
******************************************************************************
**/
#include <stdio.h>
#include <string.h>
#include "w5500_conf.h"
#include "w5500.h"
#include "socket.h"
#include "utility.h"
#include "udp_tool.h"
#include "SYSTICK.h"
#include "user_config.h"

/**
*@brief		loopback_udp
*@param		s���˿�ѡ��  port���˿ں�
*@return	��
*/


uint16 eg_upd_recvfrom(uint8 *buff, uint16 max_len, uint8 *r_ip, uint16 *r_port)
{
	
	if(max_len <= 0 || max_len > BUFF_LEN)
		return 0;
	
	switch(getSn_SR(SOCK_UDPS))                                                /*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:                                                        /*socket���ڹر�״̬*/
			socket(SOCK_UDPS, Sn_MR_UDP ,SERVER_PORT, 0);                              /*��ʼ��socket*/
		  break;
		
		case SOCK_UDP:  /*socket��ʼ�����*/
                                                         
			if(getSn_IR(SOCK_UDPS) & Sn_IR_RECV)
			{
				setSn_IR(SOCK_UDPS, Sn_IR_RECV);                                     /*������ж�*/
			}
			
			if(getSn_RX_RSR(SOCK_UDPS) > 0)                                    /*���յ�����*/
			{
				return recvfrom(SOCK_UDPS, buff, max_len, r_ip, r_port);               /*W5500���ռ����������������*/
			}
			break;
	}
	
	return 0;
}

uint16 eg_upd_long_recvfrom(uint8 *buff, uint16 max_len, uint8 *r_ip, uint16 *r_port)
{
	if(max_len <= 0 || max_len > BUFF_LEN)
		return 0;
	
	switch(getSn_SR(SOCK_UDP_LONG))                                                /*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:                                                        /*socket���ڹر�״̬*/
			socket(SOCK_UDP_LONG, Sn_MR_UDP ,SERVER_LONG_PORT, 0);                              /*��ʼ��socket*/
		  break;
		
		case SOCK_UDP:  /*socket��ʼ�����*/
                                                         
			if(getSn_IR(SOCK_UDP_LONG) & Sn_IR_RECV)
			{
				setSn_IR(SOCK_UDP_LONG, Sn_IR_RECV);                                     /*������ж�*/
			}
			
			if(getSn_RX_RSR(SOCK_UDP_LONG) > 0)                                    /*���յ�����*/
			{	
				return recvfrom(SOCK_UDP_LONG, buff, max_len, r_ip, r_port);               /*W5500���ռ����������������*/
			}
			break;
	}
	
	return 0;
}


void eg_upd_sendto(uint8 *buff, uint16 len, uint8 *r_ip, uint16 r_port)
{
	switch(getSn_SR(SOCK_UDPS))                                                /*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:                                                        /*socket���ڹر�״̬*/
			socket(SOCK_UDPS, Sn_MR_UDP ,SERVER_PORT, 0);                              /*��ʼ��socket*/
		  break;
		
		case SOCK_UDP:  /*socket��ʼ�����*/
			sendto(SOCK_UDPS, buff, len, r_ip, r_port);
			break;
	}
}


//uint8 g_buff[2048];

//void loopback_udp(SOCKET s, uint16 port)
//{
//  uint16 		len=0;                                                         /*����һ��2KB�Ļ���*/	
//	uint16 		tmp_port = 0;
//	uint8  		tmp_ip[4]={0,0,0,0};
//	
//	switch(getSn_SR(s))                                                /*��ȡsocket��״̬*/
//	{
//		case SOCK_CLOSED:                                                        /*socket���ڹر�״̬*/
//			socket(s,Sn_MR_UDP,port,0);                              /*��ʼ��socket*/
//		  break;
//		
//		case SOCK_UDP:                                                           /*socket��ʼ�����*/
//			delay_ms(10);
//			if(getSn_IR(s) & Sn_IR_RECV)
//			{
//				setSn_IR(s, Sn_IR_RECV);                                     /*������ж�*/
//			}
//			if((len=getSn_RX_RSR(s))>0)                                    /*���յ�����*/
//			{
//				tmp_port = 0;
//				recvfrom(s, g_buff, len, tmp_ip, &tmp_port);               /*W5500���ռ����������������*/
//				g_buff[len-1]=0x00;                                                    /*����ַ���������*/
//				printf("%s\r\n", g_buff);                                               /*��ӡ���ջ���*/ 
//				sendto(s,g_buff,len, tmp_ip, tmp_port);                /*W5500�ѽ��յ������ݷ��͸�Remote*/
//			}
//			break;
//	}
//}


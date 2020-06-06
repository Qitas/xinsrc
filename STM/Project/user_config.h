#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#include "Types.h"
#include "udp_tool.h"
#include "stm32f10x_it.h"

//ȫ������ flash����
#pragma pack(1)
typedef struct UserConfig
{
	char device_id[50];			//id
	char device_se[50];			//��Ȩ��
	char type;							//�豸���ͣ�0��Ĭ�ϣ�1����բ�Ž��� 2���¹� ��
	
	//232͸��������
	char pass_232_start;
	char pass_232_end;
	int  pass_232_len;
	
	//������ 
	unsigned int  baud_rate_ttl_1;					//tt1������
	unsigned int  baud_rate_232_2;					//232������
	
	//WIFI����
	char wifi_name[50];
	char wifi_password[50];

	//�Ƿ������˺���Ϣ(2���ݰ汾�ģ�0ɽ���汾��)
	char hd_version;
}USER_CONFIG_S;

extern  USER_CONFIG_S		g_user_config;
#define FLASH_ADR   		0x0801F000  		/* �洢����Flashҳ�׵�ַ 128K */


//���Ӱ汾 - ���ݰ汾��
#define HD_VERSION_GZ		2
#define HD_VERSION_JN		1


//#define ALIYUN_NAME 			"HW-RELAY-000003"
//#define ALIYUN_SECRET 		"iF5XSRrs2nVq6aq0"
////�Ƿ�װRFIDģ��
//#define IS_EXIST_RFID			1
////�Ƿ�װWIFIģ��
//#define IS_EXIST_WIFI			0

//���ô���汾
#define CODE_VERSION			"HW 1.2.2"

//�Ƿ����� ����DHCP�õ�
//#define EG_DEBUG

//������Ϣ����˿�
#define PRINT_USART				USART_TTL

//�Ƿ�Ĭ�Ͽ������������Ϣ
#define PRINT_SWITCH			1

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ĭ�ϼ����˿�
#define SERVER_PORT				5301
//Ĭ�ϼ��������ݶ˿�
#define SERVER_LONG_PORT	5401
//��������С
#define BUFF_LEN 					4280

//�Ƿ������Դ�ӡ����(1������0�ر�)
extern uint8							g_show_printf;

//����������Ϣ
#define WRITE_USER_CONFIG_LEN			10
extern 	char		WRITE_USER_CONFIG_START[5];
extern  char		WRITE_USER_CONFIG_END[5];
//////////////////////////////////////////////////////////////

//�Ƿ�װRFID������
extern uint8		g_have_rfid;

//������IP�Ͷ˿�
extern uint8		g_server_ip[4];					
extern uint16		g_server_port;
extern uint8		g_sync_work_ip[4];				//�첽�����IP�Ͷ˿�				
extern uint16		g_sync_work_port;
extern uint16		g_server_push_port;  //ר�Ŵ洢���ͷ��������ն˿�

//��������������
extern uint8		g_main_udp_push;
extern uint8	 *g_main_udp_push_buff;

//��ʱ��
extern unsigned int g_time_now;
extern uint32				g_time_now_ms;

//������������
#define MAIN_UDP_PUSH			g_main_udp_push = 1;
#define UDP_PUSH					if(g_send_len != 0){eg_upd_sendto(g_send_buff, g_send_len, g_server_ip, g_server_push_port);g_send_len = 0;}
#define UDP_SYNC_ACK			if(g_send_len != 0){eg_upd_sendto(g_send_buff, g_send_len, g_sync_work_ip, g_sync_work_port);g_send_len = 0;}

//�����첽TTL����
#define TTL_SYNC_SEND(ip, port)			if(g_send_len != 0){\
	memcpy(g_sync_work_ip, ip, 4);\
	g_sync_work_port = port;\
	sendDatatoTTL((char *)g_send_buff, g_send_len);\
	g_send_len = 0;}

//����ִ��Ƶ��(time 0.01�뵥λ)
#define WORK_HZ_LIMIT(time) 	{static uint32 last_check_time = 0;\
	if(g_time_now - last_check_time < time)\
	{return 0;}\
	else\
	{last_check_time = g_time_now;}}
#define WORK_HZ_LIMIT_VOID(time) 	{static uint32 last_check_time = 0;\
	if(g_time_now - last_check_time < time)\
	{return;}\
	else\
	{last_check_time = g_time_now;}}

//��������
extern uint8		g_data_type;
extern uint32		g_data_time;
#define 				DATA_TYPE_NONE  	0
#define 				DATA_TYPE_TTL  		1
#define 				DATA_TYPE_232  		2
#define 				DATA_TYPE_NET  		3
//�ܳ���UDP��
#define 				DATA_TYPE_NET_LONG  		4	

//оƬID
extern uint32				g_chip_uuid[3];

//�Ƿ�װָ��ģ��
extern uint8				g_finger_is;

//��¼����ʱ��
extern uint32				g_heart_client;
extern uint32				g_heart_server;

//���Ź���ʱ��
extern uint32			  g_dog_client_time;

//�̵�����ʱ��
extern uint32				g_replay_time;

//w5500оƬ�������
extern uint8				g_w5500_status;

//�Ƿ�������IOT
extern uint8				g_iot_status;


//////////////////////////////////////////////////////////////

//��������
extern uint8  		g_rev_buff[BUFF_LEN+1];
extern uint16  		g_rev_len;
extern uint8  		g_send_buff[BUFF_LEN+1];
extern uint16  		g_send_len;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


//��ʼ������
void user_config_init(void);

//����������Ϣ
int user_set_config(char *buff, int len);

//�������ʱ������
void eg_clear_timeout_data(void);

//���ݱ�־��λ
void eg_clear_data(void);

//��ʱ������
void time_work(void);

//��ظ�������
int update_status(void);

//���Ź�
void open_iwdg(uint8 pr, uint16 rlr);

#endif

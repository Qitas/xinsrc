#include "user_config.h"
#include "stm32f10x.h"
#include "model.h"
#include "w5500.h"

//����������ϢЭ��
char						WRITE_USER_CONFIG_START[5]=  {0xfa, 0xc0, 0x71, 0xb9, 0x01};
char						WRITE_USER_CONFIG_END[5]  =  {0x2a, 0x81, 0x2d, 0xb8, 0x6d};

//ȫ������ flash
USER_CONFIG_S		g_user_config;

//�Ƿ�װRFID������
uint8		g_have_rfid = 0;

//�Ƿ���printf��ӡ
uint8						g_show_printf = PRINT_SWITCH;

//������IP�Ͷ˿�
uint8						g_server_ip[4];					
uint16					g_server_port = 0;
uint16					g_server_push_port = 0;
uint8						g_sync_work_ip[4];				//�첽�����IP�Ͷ˿�				
uint16					g_sync_work_port = 0;

//��������������
uint8						g_main_udp_push = 0;
uint8					 *g_main_udp_push_buff = g_send_buff + BUFF_LEN - 200;

//��������
uint8						g_data_type = 0;
uint32					g_data_time = 0;

//оƬID
uint32					g_chip_uuid[3];

//�Ƿ�װָ��ģ��
uint8						g_finger_is = 0;

//�Ƿ���������IOT
uint8						g_iot_status = 0;

//��¼����ʱ��
uint32					g_heart_client = 0;
uint32					g_heart_server = 0;

//���Ź���ʱ�� - ��ʼ�������g_time_nowһ��
unsigned int 			g_time_now = 0xFFFF;
uint32					g_time_now_ms = 0xFFFF;
uint32					g_dog_client_time = 0xFFFF;

//�̵�����ʱ��
uint32					g_replay_time = 0;

//��������
uint8  					g_rev_buff[BUFF_LEN+1];
uint16  				g_rev_len = 0;
uint8  					g_send_buff[BUFF_LEN+1];
uint16  				g_send_len = 0;


//w5500оƬ�������
uint8						g_w5500_status = 1;


void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET ) 
	{
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
		
		g_time_now++;
		time_work();
	}
}


void user_config_init(void)
{
	//��ȡоƬID
	g_chip_uuid[2] = *(__IO u32*)(0X1FFFF7E8);  
	g_chip_uuid[1] = *(__IO u32 *)(0X1FFFF7EC); 
	g_chip_uuid[0] = *(__IO u32 *)(0X1FFFF7F0); 
	ModelInit((char*)g_chip_uuid);
	
	//��ȡflash�����ļ�
	if(*(vu32*)(FLASH_ADR) == 0xffffffff)
		memset(&g_user_config, 0, sizeof(FLASH_ADR));
	else
		memcpy(&g_user_config, (char *)FLASH_ADR, sizeof(g_user_config));
}

int user_set_config(char *buff, int len)
{
	int i = 0;

	FLASH_Unlock();
	FLASH_ErasePage(FLASH_ADR);
	for(i = 0; i < (len + 1) / 2; i++) {
		FLASH_ProgramHalfWord(FLASH_ADR + i * 2, *(u16 *)(buff + i * 2));
	}
	FLASH_Lock();

	user_config_init();		
	
	return memcmp((char *)&g_user_config, buff, len) == 0;
}

void eg_clear_timeout_data(void)
{
	if((g_data_type != DATA_TYPE_NONE || g_send_len > 0) && g_time_now - g_data_time > 50)
	{
		eg_clear_data();
	}
}

void eg_clear_data(void)
{
		g_send_len = 0;
		g_data_type = DATA_TYPE_NONE;
		g_rev_len = 0;
}

void open_iwdg(uint8 pr, uint16 rlr)
{
	if(g_user_config.device_id[0] == 0 || (unsigned char)g_user_config.device_id[0] == 0xFF) 
		return;
	
  IWDG->KR = 0x5555;        //ʹ�ܶ�IWDG->PR��IWDG->RLR��д
  IWDG->PR = pr;            //LSI/32=40Khz/(4*2^pre) //4��8��16��32��64��128��256
  IWDG->RLR = rlr;          //�Ӽ��ؼĴ��� IWDG->RLR��Max=0xFFF����Ϊ���Ź����������Ǵ����ֵ��ʼ���¼�����
  IWDG->KR = 0xAAAA;        //reload
  IWDG->KR = 0xCCCC;        //ʹ�ܿ��Ź�
}

void IWDG_Feed(void)
{
    IWDG->KR=0XAAAA;                                  
}

int update_status(void)
{
	WORK_HZ_LIMIT(10);
	g_w5500_status = getNetLink();	
	//���Ź�ι�� - ����20��û�յ��������������豸
	if(g_time_now - g_dog_client_time < 2000 || g_show_printf == 1)	IWDG_Feed();
	return 0;
}

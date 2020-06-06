#ifndef SHOW_WORK_H
#define SHOW_WORK_H

#include "model.h"
#include "Types.h"

//�ֿ��������
#define SHOW_FONT_FLASH_FALSH_ADDRESS			0x8010000
#define SHOW_FONT_FLASH_FALSH_KEY_START		101
#define SHOW_FONT_FLASH_FALSH_KEY_END			120
#define SHOW_FONT_FLASH_FALSH_SIZE				2048

//��������е׼���Ĺ���ͼ��
#define SHOW_PIC_ADDR_CI									SHOW_FONT_FLASH_FALSH_ADDRESS + SHOW_FONT_FLASH_FALSH_SIZE * 14
#define SHOW_PIC_ADDR_KG									SHOW_FONT_FLASH_FALSH_ADDRESS + SHOW_FONT_FLASH_FALSH_SIZE * 15


#define   SHOW_INTO_LINE				show_scheme(1);	 	//δ��������
#define   SHOW_ERROR_CON				show_scheme(2);	 	//δ�����Ͽ�����
#define   SHOW_ERROR_SERVER			show_scheme(3);		//δ���ӷ�����
#define   SHOW_NO_CONFIG				show_scheme(4);	 	//û��������Ϣ

//��ʾ3��
#define   SHOW_RFID_ERROR				show_scheme(5);	 	//���Ŵ���
#define   SHOW_FINGER_ERROR			show_scheme(6);	 	//ָ�ƴ���
#define   SHOW_MSG_ERROR				show_scheme(7);	 	//����
#define   SHOW_SUCCEED					show_scheme(8);		//��֤�ɹ�
#define   SHOW_NET_ERROR				show_scheme(9);		//�������

#define   SHOW_LOGO							show_scheme(100);	//Ĭ����ʾLOGO

//��ʾ�㼶
extern unsigned char		g_show_live;
//�ڲ���ʾ����
extern char	g_show_scheme_id;
//��ʾʱ��
extern unsigned char		g_show_time;

//�жϴ��ڲ㼶
#define		CHECK_SHOW_LIVE(l)				if(l < g_show_live)\
	return 0;\
else\
	g_show_live = l;

//������
#define		SHOW_LIVE_CLEARE					{g_show_live = 0;g_show_scheme_id = 0;}


int show_net_work(MODEL_HEADER_S *header, char *in_buff, int len, unsigned int *show_number);

int show_check_pic_format(char *buff, int len);

int show_scheme(char scheme);

void show_check_time(void);

//�����Ƿ�������״̬
void show_check_standby(void);

//��ʾͼ�꿿��
void show_left_pic(int x, int y, char *buff, uint16 fc, uint16 bc);

//��ʾ����
int show_num_pic(char *text, int x, int y, uint16 bc, uint16 fc);

////д�ֿ�
//void show_write_falsh(char *buff, int len);

////д�����ֿ�
//int show_num_write_falsh(char *buff, int len);


#endif

#ifndef FINGER_PARSE_H
#define FINGER_PARSE_H

//��ʼ��ָ��ģ��
void eg_finger_init(void);

//����ָ��ģ�鷵�ص�����
int eg_finger_parse(void);

//����ָ��
int eg_finger_net_comment(unsigned char type, unsigned char *content_buff, 
	int content_len, unsigned char *out_buff, unsigned short *out_len, unsigned char *ip, unsigned short port);

#endif

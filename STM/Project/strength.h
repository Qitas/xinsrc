#ifndef STRENGTH_H
#define STRENGTH_H 

//ͬ��״̬
int eg_strength_sync_status(unsigned char staus, unsigned int index, unsigned char *out_buff, unsigned short *out_len);

//�������ݱ仯
int eg_strength_loop(void);

////////////////////////////////////////////////////////////////

//����������е������
int eg_strength_add_data(unsigned char data);


#endif

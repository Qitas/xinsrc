#ifndef STRENGTH_LP_H
#define STRENGTH_LP_H 

//����ͬ��״̬
int eg_strength_lp_net_set_status(unsigned int status, unsigned int space_time, unsigned int one_time, unsigned char beep);

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//�������ݱ仯
int eg_strength_lp_loop(void);

//����������
void eg_strength_lp_btn(unsigned int interval, char btn_key);


#endif

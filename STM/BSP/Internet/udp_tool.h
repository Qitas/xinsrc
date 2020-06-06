#ifndef __UDP_TOOL_H
#define __UDP_TOOL_H

#include "stm32f10x.h"
#include "Types.h"

uint16 eg_upd_recvfrom(uint8 *buff, uint16 max_len, uint8 *r_ip, uint16 *r_port);
void eg_upd_sendto(uint8 *buff, uint16 len, uint8 *r_ip, uint16 r_port);

//���ն��UDP�������õģ���ֹͨ�ö˿ڸ�������
uint16 eg_upd_long_recvfrom(uint8 *buff, uint16 max_len, uint8 *r_ip, uint16 *r_port);

#endif 

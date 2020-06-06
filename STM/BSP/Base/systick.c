/******************** (C) COPYRIGHT  QITAS ********************************
 * �ļ���  ��systick.c
 * ����    ����શ�ʱ��ʵ��ms��ʱ��us��ʱ   
 * ����    ��QITAS
 * ���Է�ʽ��J-Link-OB
****************************************************************************/	

#include "SYSTICK.h"

extern u16 nTime;

 /**
  * @file   SYSTICK_Init
  * @brief  ��ʼ��SYSTICK��1us�ж�1��
  * @param  ��
  * @retval ��
  */
void SYSTICK_Init(void)
{	
/*SystemCoreClock/ 1000000��1us�ж�1�Σ�SystemCoreClock/ 1000��1ms�ж�һ��*/	
  while (SysTick_Config(SystemCoreClock/1000));
}

 /**
  * @file   delay_us
  * @brief  ΢����ʱ
  * @param  ��ʱʱ��
  * @retval ��
  */
//void delay_us(u16 nus)
//{
//  nTime=nus;
//  while(nTime);
//}

 /**
  * @file   delay_ms
  * @brief  ������ʱ
  * @param  ��ʱʱ��
  * @retval ��
  */
void delay_ms(u16 nms)
{
	nTime=nms;
	while(nTime);
	
//  while(nms--)
//  delay_us(1000);
}


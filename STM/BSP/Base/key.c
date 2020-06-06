/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��key.c
 * ����    ��������������
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2015-10-20
 * Ӳ������  :S1->PB14;S2->PB15;S3->PB5
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/

// ͷ�ļ�
#include "key.h"

 /**
  * @file   KEY_GPIO_Config
  * @brief  ������������
  * @param  ��
  * @retval ��
  */
void KEY_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(KEY1_BUTTON_GPIO_RCC,ENABLE);//����GPIOC������ʱ��
	RCC_APB2PeriphClockCmd(KEY2_BUTTON_GPIO_RCC,ENABLE);//����GPIOC������ʱ��
	RCC_APB2PeriphClockCmd(KEY3_BUTTON_GPIO_RCC,ENABLE);//����GPIOC������ʱ��
	RCC_APB2PeriphClockCmd(KEY4_BUTTON_GPIO_RCC,ENABLE);//����GPIOA������ʱ��
	
	GPIO_InitStructure.GPIO_Pin = KEY1_BUTTON_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING; 			 
	GPIO_Init(KEY1_BUTTON_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = KEY2_BUTTON_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING; 			 
	GPIO_Init(KEY2_BUTTON_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = KEY3_BUTTON_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING; 			 
	GPIO_Init(KEY3_BUTTON_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = KEY4_BUTTON_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING; 			 
	GPIO_Init(KEY4_BUTTON_GPIO_PORT, &GPIO_InitStructure); 
}


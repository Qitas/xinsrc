/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��exti.c
 * ����    ��������������
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2017-05-12
 * Ӳ������: ��
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/
//ͷ�ļ�  
#include "exti.h"
#include "led.h"
#include "Relay.h"
#include "key.h"
#include "user_config.h"
#include <stm32f10x.h>
#include <stdlib.h>
#include "strength_lp.h"

//3�Ű�ť
//u8 	g_btn3_count = 0;

//��������
void Delay(u32 n);

 /**
  * @file   Exti_Config
  * @brief  ���ú����⣬��ʼ���ж�����
  * @param  ��
  * @retval ��
  */
void Exti_Config(void)
{	
	/*�鿴�ⲿ�ж�/�¼���·ӳ��
  PA0��PB0��PC0��PD0��PE0��PF0��PG0 ��������>ӳ���� EXTI0
  PA1��PB1��PC1��PD1��PE1��PF1��PG1 ��������>ӳ���� EXTI1
  PA2��PB2��PC2��PD2��PE2��PF2��PG2 ��������>ӳ���� EXTI2
  PA3��PB3��PC3��PD3��PE3��PF3��PG3 ��������>ӳ���� EXTI3
  .......�������ƣ�ÿ���ⲿ�ж϶���һ���¼���·ӳ��*/
  //����һ��EXTI_InitTypeDef ���͵Ľṹ��
  EXTI_InitTypeDef EXTI_InitStructure;
  //ʹ��AFIO���ܵ�ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	
		//////////////////////////////////////////////////////////////////////////
	
	if(g_user_config.type != 0)
	{
		EXTI_ClearITPendingBit(KEY1_BUTTON_EXTI_LINE);

		//����PC14,��Ϊ�ж���·
		GPIO_EXTILineConfig(KEY1_BUTTON_EXTI_PORT_SOURCE, KEY1_BUTTON_EXTI_PIN_SOURCE);
		//ѡ���ж���·,
		EXTI_InitStructure.EXTI_Line = KEY1_BUTTON_EXTI_LINE; 
		//����Ϊ�ж����󴥷�ģʽ
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		//����Ϊ�½��ش���
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
		//����Ϊʹ���ж���
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		//���ÿ⺯������ʼ���ж�
		EXTI_Init(&EXTI_InitStructure);
	}
	
	//////////////////////////////////////////////////////////////////////////
	
	EXTI_ClearITPendingBit(KEY2_BUTTON_EXTI_LINE);

  //����PC14,��Ϊ�ж���·
  GPIO_EXTILineConfig(KEY2_BUTTON_EXTI_PORT_SOURCE, KEY2_BUTTON_EXTI_PIN_SOURCE);
  //ѡ���ж���·,
  EXTI_InitStructure.EXTI_Line = KEY2_BUTTON_EXTI_LINE; 
  //����Ϊ�ж����󴥷�ģʽ
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  //����Ϊ�½��ش���
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  //����Ϊʹ���ж���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //���ÿ⺯������ʼ���ж�
  EXTI_Init(&EXTI_InitStructure);
	
		
	//////////////////////////////////////////////////////////////////////////
	EXTI_ClearITPendingBit(KEY3_BUTTON_EXTI_LINE);
	//����PC15,��Ϊ�ж���·
  GPIO_EXTILineConfig(KEY3_BUTTON_EXTI_PORT_SOURCE, KEY3_BUTTON_EXTI_PIN_SOURCE);
  //ѡ���ж���·,
  EXTI_InitStructure.EXTI_Line = KEY3_BUTTON_EXTI_LINE; 
  //����Ϊ�ж����󴥷�ģʽ
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  //����Ϊ�½��ش���
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  //����Ϊʹ���ж���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //���ÿ⺯������ʼ���ж�
  EXTI_Init(&EXTI_InitStructure);
	
	
	Nvic_Config();
}
/**
  * @file   nvic_config
  * @brief  ���ÿ⺯������ʼ���ж�����
  * @param  ��
  * @retval ��
  */
void Nvic_Config()
{
 
  NVIC_InitTypeDef NVIC_InitStructure; //����һ��NVIC_InitTypeDef ���͵Ľṹ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //ѡ���жϷ���0	
	
  NVIC_InitStructure.NVIC_IRQChannel = KEY_BUTTON_EXTI_IRQn;//����ѡ�е��ж�����,ָ���ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; //������ռ���ȼ�:��ռʽ�ж����ȼ�����Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //������Ӧ���ȼ�:��Ӧʽ�ж����ȼ�����Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ж�����
  NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC
}

/**
  * @file   EXTI15_10_IRQHandler
  * @brief  �жϴ�����
  * @param  ��
  * @retval ��
  */

static unsigned int key1_limit = 0;
static unsigned int key2_limit = 0;
static unsigned int key3_limit = 0;
void EXTI15_10_IRQHandler(void)
{   
	
	if (EXTI_GetITStatus(KEY1_BUTTON_EXTI_LINE) != RESET)
  { 
    /*���EXTI��·����λ*/
    EXTI_ClearITPendingBit(KEY1_BUTTON_EXTI_LINE);
		
		eg_strength_lp_btn(g_time_now_ms - key1_limit, 1);
		key1_limit = g_time_now_ms;
  }
	
	else if (EXTI_GetITStatus(KEY2_BUTTON_EXTI_LINE) != RESET)
  { 
    /*���EXTI��·����λ*/
    EXTI_ClearITPendingBit(KEY2_BUTTON_EXTI_LINE);
		
		eg_strength_lp_btn(g_time_now_ms - key2_limit, 2);
		key2_limit = g_time_now_ms;
  }
	
	else if (EXTI_GetITStatus(KEY3_BUTTON_EXTI_LINE) != RESET)      //����
  { 
		/*���EXTI��·����λ*/
    EXTI_ClearITPendingBit(KEY3_BUTTON_EXTI_LINE);
		
		if((unsigned int)(g_time_now_ms - key3_limit) > 300) 
		{
			//g_btn3_count++;
			key3_limit = g_time_now_ms;
		}
  }
}

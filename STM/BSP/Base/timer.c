/******************** (C) COPYRIGHT  QITAS ********************************
 * �ļ���  ��timer.c
 * ����    ����ʼ��ͨ�ö�ʱ��TIM3��ʵ��TIM3��ʱ����    
 * ����    ��QITAS
 * ���Է�ʽ��J-Link-OB
****************************************************************************/	

#include "timer.h"

//��������
uint16_t TIM_Count;	//��ʱ������
void TimerNvic_Config(void);

 /**
  * @file   TimerNvic_Config
  * @brief  ϵͳ�ж�����
  * @param  ��
  * @retval ��
  */
void TimerNvic_Config(void)
{ 	
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 		//�����ж���Ϊ2
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 		//�ж��¼�ΪTIM3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	//��ռ���ȼ�0   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //��Ӧ���ȼ�1 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //�����ж� 
	NVIC_Init(&NVIC_InitStructure);                         //����ָ��������ʼ���жϼĴ���
}

 /**
  * @file   TIM3_Config
  * @brief  ���ú����⣬��ʼ����ʱ��2������
  * @param  ��
  * @retval ��
  */
void TIM3_Config(void)
{
	//��ʱ����ʱʱ��T���㹫ʽ��T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
	TIM_TimeBaseInitTypeDef	 TIM_BaseInitStructure;			//����һ����ʱ���ṹ�����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   	//ʹ�ܶ�ʱ��2
	TIM_DeInit( TIM3);//��IM2��ʱ����ʼ��λ��λֵ
	TIM_InternalClockConfig(TIM3);//���� TIM3 �ڲ�ʱ��
	TIM_BaseInitStructure.TIM_Period = 1000;//�����Զ����ؼĴ���ֵΪ���ֵ	0~65535֮��  1000000/1000=1000us=1ms													
	//TIM_Period��TIM1_ARR��=1000�����������ϼ�����1000����������¼�������ֵ���� Ҳ���� 1MS���������¼�һ��
	TIM_BaseInitStructure.TIM_Prescaler = 71;//�Զ���Ԥ��Ƶϵ�����ṩ����ʱ����ʱ��	0~65535֮��
													//����Ԥ��Ƶ����Ƶϵ��71����CK_INT=72M, TIM1_CLK=72/72=1MHz 
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷָ�Ϊ0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
													//TIM���ϼ���ģʽ ��0��ʼ���ϼ�����������1000����������¼�
	TIM_TimeBaseInit(TIM3, &TIM_BaseInitStructure); //����ָ��������ʼ��TIMʱ������Ĵ���
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���жϣ�����һ�����жϺ����������ж� 
	TIM_ARRPreloadConfig(TIM3, DISABLE);//��ֹARR Ԥװ�ػ�����
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//ʹ��TIM3�ж�Դ 
	TIM_Cmd(TIM3, ENABLE);//TIM3�ܿ��أ�����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);		/*�ȹرյȴ�ʹ��*/
  	TimerNvic_Config();
}



void TimerNvic_TIM4_Config(void)
{ 	
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 		//�����ж���Ϊ2
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 		//�ж��¼�ΪTIM4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	//��ռ���ȼ�0   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //��Ӧ���ȼ�1 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //�����ж� 
	NVIC_Init(&NVIC_InitStructure);                         //����ָ��������ʼ���жϼĴ���
}

 /**
  * @file   TIM4_Config
  * @brief  ���ú����⣬��ʼ����ʱ��2������
  * @param  ��
  * @retval ��
  */
void TIM4_Start(void)
{
	//��ʱ����ʱʱ��T���㹫ʽ��T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
	TIM_TimeBaseInitTypeDef	 TIM_BaseInitStructure;			//����һ����ʱ���ṹ�����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   	//ʹ�ܶ�ʱ��4
	TIM_DeInit( TIM4);//��IM2��ʱ����ʼ��λ��λֵ
	TIM_InternalClockConfig(TIM4);//���� TIM4 �ڲ�ʱ��
	TIM_BaseInitStructure.TIM_Period = 5000-1;//�����Զ����ؼĴ���ֵΪ���ֵ	0~65535֮��  1000000/1000=1000us=1ms													
	//TIM_Period��TIM1_ARR��=1000�����������ϼ�����1000����������¼�������ֵ���� Ҳ���� 1MS���������¼�һ��
	TIM_BaseInitStructure.TIM_Prescaler = 144 - 1;//�Զ���Ԥ��Ƶϵ�����ṩ����ʱ����ʱ��	0~65535֮��
													//����Ԥ��Ƶ����Ƶϵ��71����CK_INT=72M, TIM1_CLK=72/72=1MHz 
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷָ�Ϊ0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
													//TIM���ϼ���ģʽ ��0��ʼ���ϼ�����������1000����������¼�
	TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure); //����ָ��������ʼ��TIMʱ������Ĵ���
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//���жϣ�����һ�����жϺ����������ж� 
	TIM_ARRPreloadConfig(TIM4, DISABLE);//��ֹARR Ԥװ�ػ�����
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//ʹ��TIM4�ж�Դ 
	TIM_Cmd(TIM4, ENABLE);//TIM4�ܿ��أ�����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
  TimerNvic_TIM4_Config();
}


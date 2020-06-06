/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��usart.c
 * ����    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ�������ݴ�
 *           ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�     
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2015-11-11
 * Ӳ������: TX->PA9;RX->PA10
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	

//ͷ�ļ�
#include "usart.h"
#include "user_config.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


 /**
  * @file   USART1_Config
  * @brief  USART1 GPIO ����,����ģʽ���á�9600-8-N-1
  * @param  ��
  * @retval ��
  */
void USART1_Config_ttl(uint32_t BaudRate)
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;  //���崮�ڳ�ʼ���ṹ��
	
    RCC_APB2PeriphClockCmd( USART_RCC_TTL | USART_GPIO_RCC,ENABLE);

    /*USART1_TX ->PA9*/			
    GPIO_InitStructure.GPIO_Pin = USART_TX_TTL;	       //ѡ�д���Ĭ������ܽ�         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�9��ģʽ  
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��		   
    /*USART1_RX ->PA10*/
    GPIO_InitStructure.GPIO_Pin = USART_RX_TTL;	        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);
    /*����ͨѶ��������*/
    USART_InitStructure.USART_BaudRate = BaudRate; //������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
    USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������

    USART_Init(USART_TTL, &USART_InitStructure);
		USART_ClearFlag(USART_TTL,USART_FLAG_TC);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART_TTL, ENABLE);
		
		NVIC1_Configuration_ttl();
}

void NVIC1_Configuration_ttl(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void USART2_Config_232(uint32_t BaudRate)
{	
  GPIO_InitTypeDef GPIO_InitStructure;	
  USART_InitTypeDef USART_InitStructure;  //���崮�ڳ�ʼ���ṹ��
  RCC_APB2PeriphClockCmd(USART_GPIO_RCC,ENABLE);	
  RCC_APB1PeriphClockCmd(USART_RCC_232,ENABLE);
 
  /*USART2_TX ->PA2*/			
  GPIO_InitStructure.GPIO_Pin = USART_TX_232;	       //ѡ�д���Ĭ������ܽ�         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�2��ģʽ  
  GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��		   
  /*USART2_RX ->PA3*/
  GPIO_InitStructure.GPIO_Pin = USART_RX_232;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);
  /*����ͨѶ��������*/
  USART_InitStructure.USART_BaudRate = BaudRate; //������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������
   /*�����ϸ���ֵ�Ľṹ�����⺯��USART_Init���г�ʼ��*/
  USART_Init(USART_232, &USART_InitStructure); 
	USART_ITConfig(USART_232,USART_IT_RXNE,ENABLE);//ʹ�ܴ����жϽ���
  USART_Cmd(USART_232, ENABLE);//����USART2
		
	NVIC2_Configuration_232();
}

void NVIC2_Configuration_232(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void sendDatatoTTL(char *buff, int reqLen)
{
	int i;
	USART_GetFlagStatus(USART_TTL, USART_FLAG_TC);
	for(i = 0; i< reqLen; i++)
	{
		USART_SendData(USART_TTL, (uint8_t) buff[i]);
		while (USART_GetFlagStatus(USART_TTL, USART_FLAG_TC) == RESET);
	}
}

void sendDatatoRS232(char *buff, int reqLen)
{
	int i;
	
	///////////////////////////////���˷��͵�Э�飬����Ӧ�÷ŵ�232��
	if(g_user_config.pass_232_len != 0xFFFFFFFF  && g_user_config.pass_232_len != 0 && g_user_config.pass_232_len < 0xFFFF)
	{
		if(reqLen != g_user_config.pass_232_len || 
			buff[0] != g_user_config.pass_232_start || 
			buff[reqLen-1] != g_user_config.pass_232_end)
			return;
	}
	///////////////////////////
	
	USART_GetFlagStatus(USART_232, USART_FLAG_TC);
	for(i = 0; i< reqLen; i++)
	{
		USART_SendData(USART_232, (uint8_t) buff[i]);
		while (USART_GetFlagStatus(USART_232, USART_FLAG_TC) == RESET);
	}
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	if(g_show_printf)
	{
		/* Place your implementation of fputc here */
		/* e.g. write a character to the USART */
		USART_SendData(PRINT_USART, (uint8_t) ch);
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(PRINT_USART, USART_FLAG_TC) == RESET)
		{}
	}
  return ch;
}

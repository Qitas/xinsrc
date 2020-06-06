/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��Relay.c
 * ����    ���̵���GPIO��������   
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2015-10-20
 * Ӳ������: Relay_GPIO-->PB3
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	

//ͷ�ļ�
#include "Relay.h"
#include "user_config.h"

 /**
  * @file   LED_GPIO_Config
  * @brief  LED����������
  * @param  ��
  * @retval ��
  */
void Relay_GPIO_Config(void)
{	
    //����һ��GPIO_InitTypeDef ���͵Ľṹ��
		GPIO_InitTypeDef  GPIO_InitStructure;	
		/*PB3Ϊjtag�ӿڣ���û�йر�jtag����֮ǰ���ڳ������޷����ó���ͨIO��ģʽ����Ҫ�����������
		����Ҫ����AFIOʱ�ӣ�Ȼ����AFIO�������У��ͷ��������*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO  | RCC_APB2Periph_GPIOB , ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		RCC_APB2PeriphClockCmd(Relay_GPIO_RCC | Relay_GPIO_RCC_1,ENABLE);//ʹ��GPIO������ʱ��

	
		/*Relay 0*/
		GPIO_InitStructure.GPIO_Pin = Relay_GPIO_PIN;//ѡ��Ҫ�õ�GPIO����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��������ģʽΪ�������ģʽ						 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ         
		GPIO_Init(Relay_GPIO_PORT, &GPIO_InitStructure);//���ÿ⺯������ʼ��GPIO
		REALY0_OFF;
		
		/*Relay 1*/
		GPIO_InitStructure.GPIO_Pin = Relay_GPIO_PIN_1;//ѡ��Ҫ�õ�GPIO����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��������ģʽΪ�������ģʽ						 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ         
		GPIO_Init(Relay_GPIO_PORT_1, &GPIO_InitStructure);//���ÿ⺯������ʼ��GPIO
		REALY1_OFF;
}

void Relay_Switch(char id, char on)
{
	//����ǹ������߰�ȡ��
	if(g_user_config.hd_version == HD_VERSION_GZ)
		on = !on;
	
	if(id == 1)
	{
		if(on == 1)
			GPIO_SetBits(Relay_GPIO_PORT, Relay_GPIO_PIN);
		else
			GPIO_ResetBits(Relay_GPIO_PORT, Relay_GPIO_PIN);
	}
	else if(id == 2)
	{
		if(on == 1)
			GPIO_SetBits(Relay_GPIO_PORT_1, Relay_GPIO_PIN_1);
		else
			GPIO_ResetBits(Relay_GPIO_PORT_1, Relay_GPIO_PIN_1);
	}
}

char Relay_Status(char id)
{
	char ret = 0;
	
	if(id == 1)
	{
		ret = GPIO_ReadOutputDataBit(Relay_GPIO_PORT, Relay_GPIO_PIN);
	}
	else if(id == 2)
	{
		ret = GPIO_ReadOutputDataBit(Relay_GPIO_PORT_1, Relay_GPIO_PIN_1);
	}
	
	//���ݰ淵�ص�ȡ��
	if(g_user_config.hd_version == HD_VERSION_GZ)
		ret = !ret;
	
	return ret;
}



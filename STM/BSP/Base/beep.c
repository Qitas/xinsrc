#include "beep.h"
#include "user_config.h"

void eg_beep_GPIO_config(void)
{
	//����һ��GPIO_InitTypeDef ���͵Ľṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;	
	/*PB3Ϊjtag�ӿڣ���û�йر�jtag����֮ǰ���ڳ������޷����ó���ͨIO��ģʽ����Ҫ�����������
	����Ҫ����AFIOʱ�ӣ�Ȼ����AFIO�������У��ͷ��������*/
	RCC_APB2PeriphClockCmd(BEEP_GPIO_RCC , ENABLE); //ʹ��GPIO������ʱ��

	//////////////////////////////////
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//�ܹؼ�Ҫ�ص�JTAG���� ӦΪPA15��ռ���� 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//////////////////////////////////

	/*������ 0*/
	GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;//ѡ��Ҫ�õ�GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��������ģʽΪ�������ģʽ						 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ         
	GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);//���ÿ⺯������ʼ��GPIO
	
	//Ĭ�ϻ�� Ҫ�ص�
	BEEP_OFF;
}

//static char g_beep_status = 0;

//void eg_beep_switch(char on)
//{
//	if(g_user_config.hd_version == HD_VERSION_GZ)
//	{
//		if(on)
//			GPIO_SetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);
//		else
//			GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);
//	}
//	else
//	{
//		if(on)
//		{
//			if(g_beep_status == 0){TIM_Cmd(TIM2, ENABLE);g_beep_status = 1;}
//		}
//		else
//		{
//			if(g_beep_status == 1){TIM_Cmd(TIM2, DISABLE);g_beep_status = 0;}
//		}	
//	}
//}

//char eg_beep_status(void)
//{
//	if(g_user_config.hd_version == HD_VERSION_GZ)
//	{
//		return GPIO_ReadOutputDataBit(BEEP_GPIO_PORT, BEEP_GPIO_PIN) == 0;
//	}
//	else
//	{
//		return g_beep_status;
//	}
//}

char eg_beep_status(void)
{
	return GPIO_ReadOutputDataBit(BEEP_GPIO_PORT, BEEP_GPIO_PIN) == 0;
}

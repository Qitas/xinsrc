#include "adc.h"

u32 g_adc1_value=0;
void dma_config()
{
    DMA_InitTypeDef t_dma;
    //����DMA1ʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    //DMA�豸����ַΪ((u32)0x40012400+0x4c)��Ҳ����д��(u32)(&(ADC1->DR))
    t_dma.DMA_PeripheralBaseAddr=(u32)(&(ADC1->DR));
    //DMA�ڴ����ַΪ&g_value
    t_dma.DMA_MemoryBaseAddr=(u32)&g_adc1_value;
    //DMA���䷽��Ϊ�豸���ڴ�
    t_dma.DMA_DIR=DMA_DIR_PeripheralSRC;
    //DMA��������СΪ1
    t_dma.DMA_BufferSize=1;
    //DMA�豸��ַ���������ڴ��ַ������
    t_dma.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
    t_dma.DMA_MemoryInc=DMA_MemoryInc_Disable;
    //DMA�豸���ݵ�λΪ���֡��ڴ����ݵ�λΪ���֣���ÿ�δ���16λ
    t_dma.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
    t_dma.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
    //DMAģʽΪѭ����������һ�־ͽ�����һ��
    t_dma.DMA_Mode=DMA_Mode_Circular;
    //DMA���ȼ�Ϊ��
    t_dma.DMA_Priority=DMA_Priority_Medium;
    //DMA��ֹ�ڴ浽�ڴ�
    t_dma.DMA_M2M=DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1,&t_dma);
    //����DMA1��ͨ��1
    DMA_Cmd(DMA1_Channel1,ENABLE);
    //����DMA����ADC��ֵ
    ADC_DMACmd(ADC1,ENABLE);
}

void adc_init_gpio()
{
	GPIO_InitTypeDef GPIO_InitStructure;                                                  
	//�ṹ��_���Ż���-����   
	RCC_APB2PeriphClockCmd(ADC_GPIO_RCC | RCC_APB2Periph_ADC1, ENABLE);           
	//ʱ�ӿ���_GPIOA,ADC1
	GPIO_InitStructure.GPIO_Pin = ADC_GPIO_PIN;                                             
	//�ṹ��_����-PA1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                     
	//�ṹ��_����-����Ƶ��_50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                                         
	//�ṹ��_����-����ģʽ_ģ������
	GPIO_Init(ADC_GPIO_PORT, &GPIO_InitStructure);                                                
	//�ṹ��_����_��������
}

void adc_init(void)
{
	//��ʼ��ADC
	ADC_InitTypeDef t_adc;
	//����ADC1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	//ADCʹ�ö���ģʽ
	t_adc.ADC_Mode=ADC_Mode_Independent;
	//����ɨ��ģʽ
	t_adc.ADC_ScanConvMode=DISABLE;
	//��������ת������ת����һ�κ����ת��
	t_adc.ADC_ContinuousConvMode=ENABLE;
	//��ʹ���ⲿ����
	t_adc.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	//�����Ҷ���
	t_adc.ADC_DataAlign=ADC_DataAlign_Right;
	//Ҫת����ͨ����Ϊ1
	t_adc.ADC_NbrOfChannel=1;
	//��ʼ��ADC1
	ADC_Init(ADC1,&t_adc);
	//����ADC��ʱ��ΪPCLK2��8��Ƶ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	//����ADC1��ͨ��0��ת������Ϊ71.5����������
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_71Cycles5);
	//ʹ��ADC1
	ADC_Cmd(ADC1,ENABLE);
	//ʹ���������
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);   
	
	//��ʼ��GPIO
	adc_init_gpio();
	
	//DMA��ȡ
	dma_config();
}


uint16_t adc_read_data()
{                                                                                                                                                                                                                                                                                                                                                 
    u16 t_value;
    //�ȴ�ADC1ת�����
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
    //��ȡADC1��ֵ
    t_value=ADC_GetConversionValue(ADC1);
    //���ת����ɱ�־
    ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
    //t_value�о���ADC��ֵ  
		return t_value;
}

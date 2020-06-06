/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��Lcd_Driver.c
 * ����    ������LCDҺ�����ĳ�ʼ����������  
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * Ӳ������:					
 * ���Է�ʽ��J-Link-OB
********************************************************************************/

#include "Lcd_Driver.h"
#include "SYSTICK.h"

void Delayms( uint16_t time_ms );
 /**
  * @file   Delayms
  * @brief  ������ʱ������ȷ
  * @param  time_ms 
  * @retval 
  */
//void Delayms( uint16_t time_ms )
//{
//  uint16_t i,j;
//  for( i=0;i<time_ms;i++ )
//  {
//    for( j=0;j<10309;j++ );//1ms
//  }
//}
//.\obj\YD_OLED.axf: Error: L6200E: Symbol gImage_qq multiply defined (by qdtft_demo.o and main.o).
 /**
  * @file   LCD_GPIO_Init
  * @brief  Һ��IO��ʼ������
  * @param  -
  * @retval -
  */
void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;    
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_RS| LCD_CS| LCD_RST | LCD_SCL | LCD_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_CTRL, &GPIO_InitStructure);  
	
	

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB ,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_LED;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

 /**
  * @file   SPI_WriteData
  * @brief  ��SPI���ߴ���һ��8λ����
  * @param  Data��
  * @retval -
  */
void  SPI_WriteData(u8 Data)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)	
	  LCD_SDA_SET; //�������
		else LCD_SDA_CLR;
	 
		LCD_SCL_CLR;       
		LCD_SCL_SET;
		Data<<=1; 
	}
}

 /**
  * @file   Lcd_WriteIndex
  * @brief  ��Һ����дһ��8λָ��
  * @param  Index
  * @retval -
  */
void Lcd_WriteIndex(u8 Index)
{
   //SPI д����ʱ��ʼ
   LCD_CS_CLR;
   LCD_RS_CLR;
	 SPI_WriteData(Index);
   LCD_CS_SET;
}

 /**
  * @file   Lcd_WriteData
  * @brief  ��Һ����дһ��8λ����
  * @param  Data
  * @retval -
  */
void Lcd_WriteData(u8 Data)
{
   LCD_CS_CLR;
   LCD_RS_SET;
   SPI_WriteData(Data);
   LCD_CS_SET; 
}
 /**
  * @file   LCD_WriteData_16Bit
  * @brief  ��Һ����дһ��16λ����
  * @param  Data
  * @retval -
  */
void LCD_WriteData_16Bit(u16 Data)
{
   LCD_CS_CLR;
   LCD_RS_SET;
	 SPI_WriteData(Data>>8); 	//д���8λ����
	 SPI_WriteData(Data); 			//д���8λ����
   LCD_CS_SET; 
}
 /**
  * @file   Lcd_WriteReg
  * @brief  ��Һ����дһ��16λ����
  * @param  Index��ָ��  Data������
  * @retval -
  */
void Lcd_WriteReg(u8 Index,u8 Data)
{
	Lcd_WriteIndex(Index);
  Lcd_WriteData(Data);
}
 /**
  * @file   Lcd_Reset
  * @brief  Һ������λ
  * @param  -
  * @retval -
  */
void Lcd_Reset(void)
{
	LCD_RST_CLR;
	delay_ms(100);
	LCD_RST_SET;
	delay_ms(50);
}

//LCD Init For 1.44Inch LCD Panel with ST7735R.
 /**
  * @file   Lcd_Init
  * @brief  Һ����������ʼ��
  * @param  -
  * @retval -
  */
void Lcd_Init(void)
{	
	LCD_GPIO_Init();
	Lcd_Reset(); //Reset before LCD Init.

	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Lcd_WriteIndex(0x11);//Sleep exit 
	delay_ms (120);
		
	//ST7735R Frame Rate
	Lcd_WriteIndex(0xB1); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB2); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB3); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	
	Lcd_WriteIndex(0xB4); //Column inversion 
	Lcd_WriteData(0x07); 
	
	//ST7735R Power Sequence
	Lcd_WriteIndex(0xC0); 
	Lcd_WriteData(0xA2); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x84); 
	Lcd_WriteIndex(0xC1); 
	Lcd_WriteData(0xC5); 

	Lcd_WriteIndex(0xC2); 
	Lcd_WriteData(0x0A); 
	Lcd_WriteData(0x00); 

	Lcd_WriteIndex(0xC3); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0x2A); 
	Lcd_WriteIndex(0xC4); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0xEE); 
	
	Lcd_WriteIndex(0xC5); //VCOM 
	Lcd_WriteData(0x0E); 
	
	Lcd_WriteIndex(0x36); //MX, MY, RGB mode 
	Lcd_WriteData(0xC8); 
	
	//ST7735R Gamma Sequence
	Lcd_WriteIndex(0xe0); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1a); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x18); 
	Lcd_WriteData(0x2f); 
	Lcd_WriteData(0x28); 
	Lcd_WriteData(0x20); 
	Lcd_WriteData(0x22); 
	Lcd_WriteData(0x1f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x23); 
	Lcd_WriteData(0x37); 
	Lcd_WriteData(0x00); 	
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x10); 

	Lcd_WriteIndex(0xe1); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x17); 
	Lcd_WriteData(0x33); 
	Lcd_WriteData(0x2c); 
	Lcd_WriteData(0x29); 
	Lcd_WriteData(0x2e); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x39); 
	Lcd_WriteData(0x3f); 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0x10);  
	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x7f);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x9f);
	
	Lcd_WriteIndex(0xF0); //Enable test command  
	Lcd_WriteData(0x01); 
	Lcd_WriteIndex(0xF6); //Disable ram power save mode 
	Lcd_WriteData(0x00); 
	
	Lcd_WriteIndex(0x3A); //65k mode 
	Lcd_WriteData(0x05); 
	
	Lcd_WriteIndex(0x29);//Display on	 
}

 /**
  * @file   Lcd_SetRegion
  * @brief  ����lcd��ʾ�����ڴ�����д�������Զ�����
  * @param  xy�����յ�
  * @retval -
  */
void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end)
{		
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start+3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end+3);
	
	Lcd_WriteIndex(0x2c);

}

 /**
  * @file   Lcd_SetXY
  * @brief  ����lcd��ʾ��ʼ��
  * @param  xy����
  * @retval -
  */
void Lcd_SetXY(u16 x,u16 y)
{
  	Lcd_SetRegion(x,y,x,y);
}
 /**
  * @file   LCD_DrawPoint
  * @brief  ��һ����
  * @param  xy���꣬data������
  * @retval -
  */
void Gui_DrawPoint(u16 x,u16 y,u16 Data)
{
	Lcd_SetRegion(x,y,x+1,y+1);
	LCD_WriteData_16Bit(Data);
}    
 /**
  * @file   Lcd_ReadPoint
  * @brief  ��һ����
  * @param  color  ����ɫֵ  
  * @retval -
  */
unsigned int Lcd_ReadPoint(u16 x,u16 y)
{
  unsigned int Data;
  Lcd_SetXY(x,y);

  //Lcd_ReadData();//���������ֽ�
  //Data=Lcd_ReadData();
  Lcd_WriteData(Data);
  return Data;
}
 /**
  * @file   Lcd_Clear
  * @brief  ȫ����������
  * @param  color  ����ɫֵ  
  * @retval -
  */
void Lcd_Clear(u16 Color)               
{	
   unsigned int i,m;
   Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   Lcd_WriteIndex(0x2C);
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {	
	  	LCD_WriteData_16Bit(Color);
    }   
}







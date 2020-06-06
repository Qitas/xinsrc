/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��GUI.c
 * ����    ��GUIͼ������
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * Ӳ������:					
 * ���Է�ʽ��J-Link-OB
********************************************************************************/
#include "GUI.h"
#include "Lcd_Driver.h"
#include "SYSTICK.h"
#include "font.h"
#include "user_config.h"


uint32 g_draw_pic_number = 0xF;

//ˢ��ʱ��
uint32		g_draw_pic_time = 0;

 /**
  * @file   LCD_BGR2RGB
  * @brief  ��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��ͨ���ú���ת��
  * @param  c GBR��ʽ����ɫֵ
  * @retval RGB��ʽ����ɫֵ
  */
u16 LCD_BGR2RGB(u16 c)
{
  u16  r,g,b,rgb;   
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);		 
  return(rgb);
}

 /**
  * @file  Gui_Circle
  * @brief ����һ��Բ���ʿ�Ϊ1������
  * @param 
	*			     X,Y  ��Բ�ĵ�����
	*			     R ��Բ�İ뾶
  *          fc:
  * @retval ��
  */
void Gui_Circle(u16 X,u16 Y,u16 R,u16 fc) 
{//Bresenham�㷨 
    unsigned short  a,b; 
    int c; 
    a=0; 
    b=R; 
    c=3-2*R; 
    while (a<b) 
    { 
        Gui_DrawPoint(X+a,Y+b,fc);     //        7 
        Gui_DrawPoint(X-a,Y+b,fc);     //        6 
        Gui_DrawPoint(X+a,Y-b,fc);     //        2 
        Gui_DrawPoint(X-a,Y-b,fc);     //        3 
        Gui_DrawPoint(X+b,Y+a,fc);     //        8 
        Gui_DrawPoint(X-b,Y+a,fc);     //        5 
        Gui_DrawPoint(X+b,Y-a,fc);     //        1 
        Gui_DrawPoint(X-b,Y-a,fc);     //        4 
        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
    { 
			Gui_DrawPoint(X+a,Y+b,fc); 
			Gui_DrawPoint(X+a,Y+b,fc); 
			Gui_DrawPoint(X+a,Y-b,fc); 
			Gui_DrawPoint(X-a,Y-b,fc); 
			Gui_DrawPoint(X+b,Y+a,fc); 
			Gui_DrawPoint(X-b,Y+a,fc); 
			Gui_DrawPoint(X+b,Y-a,fc); 
			Gui_DrawPoint(X-b,Y-a,fc); 
    } 
	
} 
 /**
  * @file  Gui_DrawLine
  * @brief ���� Bresenham �㷨����2��仭һ��ֱ�ߡ�
  * @param 
	*					 x0, y0 ����ʼ������
	*					 x1, y1 ����ֹ��Y����
	*					_ucColor     ����ɫ
  * @retval ��
  */
void Gui_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color)   
{
		int dx,         // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2, 
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping	


	Lcd_SetXY(x0,y0);
	dx = x1-x0;//����x����
	dy = y1-y0;//����y����

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x�������y���룬��ôÿ��x����ֻ��һ���㣬ÿ��y���������ɸ���
	{//���ߵĵ�������x���룬��x���������
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)//Ҫ���ĵ������ᳬ��x����
		{
			//����
			Gui_DrawPoint(x0,y0,Color);
			
			// test if error has overflowed
			if (error >= 0) //�Ƿ���Ҫ����y����ֵ
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//����y����ֵ
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//x����ֵÿ�λ���󶼵���1
		} // end for
	} // end if |slope| <= 1
	else//y�����x�ᣬ��ÿ��y����ֻ��һ���㣬x�����ɸ���
	{//��y��Ϊ��������
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			Gui_DrawPoint(x0,y0,Color);

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}

 /**
  * @file   Gui_box
  * @brief  
  * @param  
  * @retval 
  */

void Gui_box(u16 x, u16 y, u16 w, u16 h,u16 bc)
{
	Gui_DrawLine(x,y,x+w,y,0xEF7D);
	Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
	Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
	Gui_DrawLine(x,y,x,y+h,0xEF7D);
    Gui_DrawLine(x+1,y+1,x+1+w-2,y+1+h-2,bc);
}
 /**
  * @file   Gui_box2
  * @brief  
  * @param  
  * @retval 
  */
void Gui_box2(u16 x,u16 y,u16 w,u16 h, u8 mode)
{
	if (mode==0)	{
		Gui_DrawLine(x,y,x+w,y,0xEF7D);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
		Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
		Gui_DrawLine(x,y,x,y+h,0xEF7D);
		}
	if (mode==1)	{
		Gui_DrawLine(x,y,x+w,y,0x2965);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
		Gui_DrawLine(x,y+h,x+w,y+h,0xEF7D);
		Gui_DrawLine(x,y,x,y+h,0x2965);
	}
	if (mode==2)	{
		Gui_DrawLine(x,y,x+w,y,0xffff);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xffff);
		Gui_DrawLine(x,y+h,x+w,y+h,0xffff);
		Gui_DrawLine(x,y,x,y+h,0xffff);
	}
}

/**
  * @file   DisplayButtonDown
  * @brief  ����Ļ��ʾһ͹��İ�ť��
  * @param  u16 x1,y1,x2,y2 ��ť�����ϽǺ����½�����
  * @retval 
  */
void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2)
{
	Gui_DrawLine(x1,  y1,  x2,y1, GRAY2);  //H
	Gui_DrawLine(x1+1,y1+1,x2,y1+1, GRAY1);  //H
	Gui_DrawLine(x1,  y1,  x1,y2, GRAY2);  //V
	Gui_DrawLine(x1+1,y1+1,x1+1,y2, GRAY1);  //V
	Gui_DrawLine(x1,  y2,  x2,y2, WHITE);  //H
	Gui_DrawLine(x2,  y1,  x2,y2, WHITE);  //V
}

/**
  * @file   DisplayButtonUp
  * @brief  ����Ļ��ʾһ͹��İ�ť��
  * @param  u16 x1,y1,x2,y2 ��ť�����ϽǺ����½�����
  * @retval 
  */
void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
{
	Gui_DrawLine(x1,  y1,  x2,y1, WHITE); //H
	Gui_DrawLine(x1,  y1,  x1,y2, WHITE); //V
	
	Gui_DrawLine(x1+1,y2-1,x2,y2-1, GRAY1);  //H
	Gui_DrawLine(x1,  y2,  x2,y2, GRAY2);  //H
	Gui_DrawLine(x2-1,y1+1,x2-1,y2, GRAY1);  //V
    Gui_DrawLine(x2  ,y1  ,x2,y2, GRAY2); //V
}

void Gui_DrawFont_PIC(u16 x, u16 y, u16 width, u16 height, u16 fc, u16 bc, u8 *index_s, u8 *buff_s, u16 buff_len)
{
	int i, j;
	u16 color = 0;
	int all_len = width * height;
	int x_len = all_len / 8 + 1;
	int pos = 0;
	u8 *buff_s_pos = buff_s;
	
	if(x >= 128 || y >= 128 || x + width > 128 || y + height > 128)
		return;
	
	//��ʾ����ۼ�
	g_draw_pic_number++;
	
	//����ˢ��ʱ��
	g_draw_pic_time = g_time_now;
	
	//������ʾ����
	Lcd_SetRegion(x, y, x + width - 1, y + height - 1);		//��������
	
	for(i =0; i < x_len; i++)
	{
		for(j=0; j < 8; j++)
		{
			//��ֹԽ��
			if(pos >= all_len)
				break;
			pos++;
			
			color = bc;
			if(index_s[i] & (1 << (j % 8)))
			{
				if(buff_s_pos - buff_s < buff_len)
				{
					color = ((fc & BLUE) * *buff_s_pos / 255) |  (((fc & GREEN) >> 5) * *buff_s_pos / 255) << 5 | (((fc & RED) >> 11) * *buff_s_pos / 255) << 11;
					buff_s_pos++;
				}
				else
				{
					color = fc;
				}
			}

			LCD_WriteData_16Bit(color);
		}
	}
}

void Gui_Clear_Rect(u16 x, u16 y, u16 width, u16 height, u16 fc)
{
	int i;
	int all_len = width * height;
	
	Lcd_SetRegion(x, y, x + width - 1, y + height - 1);		//��������
	for(i =0; i < all_len; i++)
		LCD_WriteData_16Bit(fc);
}

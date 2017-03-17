/*
*********************************************************************************************************
*
*	ģ������ : TFTҺ����ʾ������ģ��
*	�ļ����� : bsp_ili9341_lcd.h
*	��    �� : V1.0
*	˵    �� : ����оƬ�ķ��ʵ�ַΪ:  0x60000000
*				
*	�޸ļ�¼ :
*		�汾��    ����          ����                 ˵��
*		v1.0    2011-10-31   WildFire Team  ST�̼���汾 V3.5.0�汾��
*       v1.1    2011-11-07   WildFire Team  ����������ʾ��ͨ��ֱ�ӽ�X,Y������Ϊ�ݼ�������GUI��ʾ��
*
*
*	Copyright (C), 2012-2013, WildFire Team
*
*********************************************************************************************************
*/

#include "stm32f10x.h"


/* ����LCD�������ķ��ʵ�ַ */
#define ILI9341_REG		*(__IO uint16_t *)(0x60000000)
#define ILI9341_RAM		*(__IO uint16_t *)(0x60020000)


void LCD9341_SetCursor(uint16_t _usX1 , uint16_t _usY1);
void LCD9341_SetPoint(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usColor);
void LCD_WR_Data(unsigned int val);


void LCD9341_Clear(uint16_t _usColor);
void LCD9341_SetDispWin(uint16_t _usX1, uint16_t _usY1, uint16_t _usX2, uint16_t _usY2);
void LCD9341_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor);
void LCD9341_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor);
void LCD9341_FillRect(uint16_t _usX1 , uint16_t _usY1 ,  uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);

uint16_t LCD9341_GetPoint(uint16_t _usX1 , uint16_t _usY1);
void LCD9341_SetBackLight(uint8_t _bright);
void LCD9341_DrawJPG(uint16_t _usX1 , uint16_t _usY1 ,  uint16_t _usX2 , uint16_t _usY2 , uint16_t *_ptr);
void LCD9341_SetDispHV(uint16_t _usX1, uint16_t _usY1, uint16_t _usX2, uint16_t _usY2, uint8_t _ucHV);

void bsp_InitLCD(void);



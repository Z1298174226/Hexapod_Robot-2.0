/*
*********************************************************************************************************
*
*	ģ������ : TFTҺ����ʾ������ģ��
*	�ļ����� : bsp_ili9341_lcd.c
*	��    �� : V1.1
*	˵    �� : ����оƬ�ķ��ʵ�ַΪ:  0x60000000
*				
*	�޸ļ�¼ :
*		�汾��    ����          ����                 ˵��
*		v1.0    2011-10-31   WildFire Team  ST�̼���汾 V3.5.0�汾��
*       v1.1    2011-11-07   WildFire Team  ����������ʾ��ͨ��ֱ�ӽ�X,Y������Ϊ�ݼ�������GUI��ʾ��
*
*	Copyright (C), 2012-2013, WildFire Team
*
*********************************************************************************************************
*/
#include "bsp_ili9341_lcd.h"


/* ILI9341�Ĵ�����ַ */
#define SWRESET 	0x01 //Software Reset
#define RDDIDIF 	0x04 //Read Display Identification Information
#define RDDST   	0x09 //Read Display Status
#define RDDPM   	0x0A //Read Display Power Mode
#define RDDMADCTL	0x0B //Read Display MADCTL
#define RDDCOLMOD	0x0C //Read Display Pixel Format
#define RDDIM		0x0D //Read Display Image Mode
#define RDDSM		0x0E //Read Display Signal Mode
#define RDDSDR		0x0F //Read Display Self-Diagnostic Result
#define SPLIN		0x10 //Enter Sleep Mode
#define SLPOUT		0x11 //Sleep Out
#define PTLON		0x12 //Partial Mode On
#define NORON		0x13 //Normal Display Mode On
#define DINVOFF		0x20 //Display Inversion OFF
#define DINVON		0x21 //Display Inversion ON
#define GAMSET      0x26 //Gamma Set
#define DISPOFF		0x28 //Display OFF
#define DISPON		0x29 //Display ON
#define CASET		0x2A //Column Address Set
#define PASET		0x2B //Page Address Set
#define RAMWR		0x2C //Memory Write
#define RGBSET		0x2D //Color Set
#define RAMRD       0x2E //Memory Read
#define PLTAR		0x30 //Partial Area
#define VSCRDEF		0x33 //Vertical Scrolling Definition
#define TEOFF		0x34 //Tearing Effect Line OFF
#define TEON		0x35 //Tearing Effect Line ON
#define MADCTL		0x36 //Memory Access Control
#define VSCRSADD	0x37 //Vertical Scrolling Start Address
#define IDMOFF		0x38 //Idle Mode OFF
#define IDMON		0x39 //Idle Mode ON
#define PIXSET		0x3A //Pixel Format Set
#define RAMWRC      0x3C //Write Memory Continue
#define RAMRDC		0x3E //Read  Memory Continue
#define STTS		0x44 //Set Tear Scanline 
#define GTSL		0x45 //Get Scan line
#define WRDISBV		0x51 //Write Display Brightness
#define RDDISBV		0x52 //Read Display Brightness Value
#define WRCTRLD		0x53 //Write Control Display
#define RDCTRLD		0x54 //Read Control Display
#define WRCABC		0x55 //Write Content Adaptive Brightness Control
#define RDCABC		0x56 //Read Content Adaptive Brightness Control
#define WRCABCMIN	0x5E //Write CABC Minimum Brightness
#define RDCABCMIN	0x5F //Read CABC Minimum Brightnes
#define RDID1		0xDA //Read ID1
#define RDID2		0xDB //Read ID2
#define RDID3		0xDC //Read ID3
#define IFMODE		0xB0 //Interface Mode Control
#define FRMCTR1		0xB1 //Frame Rate Control (In Normal Mode / Full colors
#define FRMCTR2		0xB2 //Frame Rate Control (In Idle Mode / 8l colors)
#define FRMCTR3		0xB3 //Frame Rate Control (In Partial Mode / Full colors)
#define INVTR		0xB4 //Display Inversion Control
#define PRCTR		0xB5 //Blanking Porch
#define DISCTRL		0xB6 //Display Function Control
#define ETMOD		0xB7 //Entry Mode Set
#define BKCTL1		0xB8 //Backlight Control 1 
#define BKCTL2		0xB9 //Backlight Control 2 
#define BKCTL3		0xBA //Backlight Control 3 
#define BKCTL4		0xBB //Backlight Control 4 
#define BKCTL5		0xBC //Backlight Control 5
#define BKCTL7		0xBE //Backlight Control 7
#define BKCTL8		0xBF //Backlight Control 8
#define PWCTRL1		0xC0 //Power Control 1
#define PWCTRL2		0xC1 //Power Control 2
#define VMCTRL1		0xC5 //VCOM Control 1
#define VMCTRL2		0xC7 //VCOM Control 2
#define NVMWR		0xD0 //NV Memory Write
#define NVMPKEY		0xD1 //NV Memory Protection Key
#define RDNVM		0xD2 //NV Memory Status Read
#define RDID4		0xD3 //Read ID4
#define PGAMCTRL	0xE0 //Positive Gamma Control
#define NGAMCTRL	0xE1 //Negative Gamma Correction
#define DGAMCTRL1	0xE2 //Digital Gamma Control 1
#define DGAMCTRL2	0xE3 //Digital Gamma Control 2
#define IFCTL		0xF6 //16bits Data Format Selection
#define PWCTLA		0xCB //Power control A 
#define PWCTLB		0xCF //Power control B 
#define DTIMCTLA	0xE8 //Driver timing control A 
#define DTIMCTLB	0xEA //Driver timing control B 
#define PWONSCTL	0xED //Power on sequence control 
#define EN3G		0xF2 //Enable_3G 
#define PRCTL		0xF7 //Pump ratio control 


/* ���� */
#define LCD_ILI9341_CMD(index)       LCD_WR_REG(index)
#define LCD_ILI9341_Parameter(val)   LCD_WR_Data(val)

/* ����LCD�������õ�����ʱ */
#define DEBUG_DELAY()   Delay(2000)


/*
*********************************************************************************************************
*	�� �� ��: LCD_WR_REG
*	����˵��: д�Ĵ�����ַ����
*	��    ��: index �Ĵ�����ַ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_WR_REG(unsigned int index)
{
	ILI9341_REG = index;

}

/*
*********************************************************************************************************
*	�� �� ��: LCD_WR_CMD
*	����˵��: д�Ĵ������ݺ���
*	��    ��: index �Ĵ�����ַ
*			  val   ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	ILI9341_REG = index;	
	ILI9341_RAM = val;
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_WR_CMD
*	����˵��: ��GRAM����-
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t LCD_RD_data(void)	
{	
	uint16_t R=0, G=0, B=0 ;

	R = ILI9341_RAM; 	/*FIRST READ OUT DUMMY DATA*/
	R = ILI9341_RAM;  	/*READ OUT RED DATA  */
	B = ILI9341_RAM;  	/*READ OUT BLACK DATA*/
	G = ILI9341_RAM;  	/*READ OUT GREEN DATA*/
	
    return (((R>>11)<<11) | ((G>>10)<<5) | (B>>11)) ;
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_WR_Data
*	����˵��: д16λ���ݺ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_WR_Data(unsigned int val)
{   
	ILI9341_RAM = val; 	
}

/*
*********************************************************************************************************
*	�� �� ��: Delay
*	����˵��: ��ʱ
*	��    ��: nCount ��ʱ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_FSMCConfig
*	����˵��: FSMC��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_FSMCConfig(void)
{	
	
	 FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;	
    FSMC_NORSRAMTimingInitTypeDef  p; 
    
    
    p.FSMC_AddressSetupTime = 0x01;	 //��ַ����ʱ��	//Ϊewim�޸ģ�ԭ����������ֵΪ2  ��С������ٶ�
    p.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ��
    p.FSMC_DataSetupTime = 0x02;		 //���ݽ���ʱ��	//Ϊewim�޸ģ�ԭ����������ֵΪ5  ��С������ٶ�
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision = 0x00;
    p.FSMC_DataLatency = 0x00;
    p.FSMC_AccessMode = FSMC_AccessMode_B;	 // һ��ʹ��ģʽB������LCD
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    //FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
		FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;  
    
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* ʹ�� FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
		

	
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_CtrlLinesConfig
*	����˵��: TFT��GPIO��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_CtrlLinesConfig(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
    
    /* ʹ��FSMCʱ��*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* ʹ��FSMC��Ӧ��Ӧ�ܽ�ʱ��*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD 
                            | RCC_APB2Periph_GPIOE 
	                          | RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /* ����LCD��λ���ƹܽ�*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 
    GPIO_Init(GPIOE, &GPIO_InitStructure);  		   
    
    /* ����FSMC���Ӧ��������,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10*/	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 /* |GPIO_Pin_9*/ | GPIO_Pin_10 
                                  /*GPIO_Pin_11*/ | GPIO_Pin_12 |/* GPIO_Pin_13*/ /* GPIO_Pin_14*/  
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
		/* ����FSMC���Ӧ�Ŀ�����
		 * PD4-FSMC_NOE   :LCD-RD
		 * PD5-FSMC_NWE   :LCD-WR
		 * PD7-FSMC_NE1   :LCD-CS
		 * PD11-FSMC_A16  :LCD-DC
		*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
    
    /* ����LCD������ƹܽ�*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_1);




}

/*
*********************************************************************************************************
*	�� �� ��: LCD_Reset
*	����˵��: ��λ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_Reset(void)
{			
	/*��λ LCD*/	 
		GPIO_ResetBits(GPIOE, GPIO_Pin_1);	 //�͵�ƽ��λ
    Delay(0xAFFf<<2); 					   
    GPIO_SetBits(GPIOE, GPIO_Pin_1);		 	 
    Delay(0xAFFf<<2); 	

}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLCD
*	����˵��: ili9341��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*   	36H �Ĵ���(MV����X,Y�л�)
*       MY      Row Address Order 
		MX		Column Address Order
		MV      Row / Column Exchange 
		7 	6		  
        MY  MX  
        0   0     X������Y����    
 		0   1     X������Y�ݼ�    	
        1   0     X�ݼ���Y����  
        1   1     X�ݼ���Y�ݼ� 

		������Ұ��İ���Ϊ��˵������������ֻ���������ʽ������ȡX,Y���ǵݼ���3��λ����ʵ��
		8���������ﲻ����X��Y�Ľ�������Ұ��LCD���ϵ����塰Ұ������ʱ����Ϊ���棬���½���
		��ʼ�㣬���Ͻ��ǽ����㣩
		�����̲��÷�ʽ��  36H = 0xCx

		    36H = 0x0x                    36H = 0x4x	  		                    
		  -------------------          -------------------
		 |(239,319)          |        |(239,319)          |
		 |                   |        |
		 |  ^           ^    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |  <------  |    |        |   | <------   |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |                   |        |	  v			  v	  |
		 |                   |        |                   |
		 |              (0,0)|        |              (0,0)|
		 |-------------------|        |-------------------|
		 |                   |        |                   |
		  -------------------          -------------------
			36H = 0x8x					   36H = 0xCx 
		  -------------------          -------------------
		 |(239,319)          |        |(239,319)          |
		 |                   |        |
		 |  ^           ^    |        |                   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |  <------  |    |        |   | ------>   |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |                   |        |	  v			  v	  |
		 |                   |        |                   |
		 |              (0,0)|        |              (0,0)|
		 |-------------------|        |-------------------|
		 |                   |        |                   |
		  -------------------          -------------------
*********************************************************************************************************
*/
void bsp_InitLCD(void)
{
	/* ����LCD���ƿ���GPIO */
	LCD_CtrlLinesConfig();

	/* ����FSMC�ӿڣ��������� */
	LCD_FSMCConfig();

	/* ��λ */
	LCD_Reset();
		
	/*  Power control B (CFh)  */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xCF);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x81);
	LCD_ILI9341_Parameter(0x30);
	
	/*  Power on sequence control (EDh) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xED);
	LCD_ILI9341_Parameter(0x64);
	LCD_ILI9341_Parameter(0x03);
	LCD_ILI9341_Parameter(0x12);
	LCD_ILI9341_Parameter(0x81);
	
	/*  Driver timing control A (E8h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xE8);
	LCD_ILI9341_Parameter(0x85);
	LCD_ILI9341_Parameter(0x10);
	LCD_ILI9341_Parameter(0x78);
	
	/*  Power control A (CBh) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xCB);
	LCD_ILI9341_Parameter(0x39);
	LCD_ILI9341_Parameter(0x2C);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x34);
	LCD_ILI9341_Parameter(0x02);
	
	/* Pump ratio control (F7h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xF7);
	LCD_ILI9341_Parameter(0x20);
	
	/* Driver timing control B */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xEA);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	
	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xB1);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x1B);
	
	/*  Display Function Control (B6h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xB6);
	LCD_ILI9341_Parameter(0x0A);
	LCD_ILI9341_Parameter(0xA2);
	
	/* Power Control 1 (C0h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xC0);
	LCD_ILI9341_Parameter(0x35);
	
	/* Power Control 2 (C1h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xC1);
	LCD_ILI9341_Parameter(0x11);
	
	/* VCOM Control 1(C5h) */
	LCD_ILI9341_CMD(0xC5);
	LCD_ILI9341_Parameter(0x45);
	LCD_ILI9341_Parameter(0x45);
	
	/*  VCOM Control 2(C7h)  */
	LCD_ILI9341_CMD(0xC7);
	LCD_ILI9341_Parameter(0xA2);
	
	/* Enable 3G (F2h) */
	LCD_ILI9341_CMD(0xF2);
	LCD_ILI9341_Parameter(0x00);
	
	/* Gamma Set (26h) */
	LCD_ILI9341_CMD(0x26);
	LCD_ILI9341_Parameter(0x01);

	/* Positive Gamma Correction */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xE0); 
	LCD_ILI9341_Parameter(0x0F);
	LCD_ILI9341_Parameter(0x26);
	LCD_ILI9341_Parameter(0x24);
	LCD_ILI9341_Parameter(0x0B);
	LCD_ILI9341_Parameter(0x0E);
	LCD_ILI9341_Parameter(0x09);
	LCD_ILI9341_Parameter(0x54);
	LCD_ILI9341_Parameter(0xA8);
	LCD_ILI9341_Parameter(0x46);
	LCD_ILI9341_Parameter(0x0C);
	LCD_ILI9341_Parameter(0x17);
	LCD_ILI9341_Parameter(0x09);
	LCD_ILI9341_Parameter(0x0F);
	LCD_ILI9341_Parameter(0x07);
	LCD_ILI9341_Parameter(0x00);

	/* Negative Gamma Correction (E1h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0XE1); 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x19);
	LCD_ILI9341_Parameter(0x1B);
	LCD_ILI9341_Parameter(0x04);
	LCD_ILI9341_Parameter(0x10);
	LCD_ILI9341_Parameter(0x07);
	LCD_ILI9341_Parameter(0x2A);
	LCD_ILI9341_Parameter(0x47);
	LCD_ILI9341_Parameter(0x39);
	LCD_ILI9341_Parameter(0x03);
	LCD_ILI9341_Parameter(0x06);
	LCD_ILI9341_Parameter(0x06);
	LCD_ILI9341_Parameter(0x30);
	LCD_ILI9341_Parameter(0x38);
	LCD_ILI9341_Parameter(0x0F);
	

	/*���ó���������ʾ��ʽ*/ 
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0x36); 
	LCD_ILI9341_Parameter(0xc8);    
	DEBUG_DELAY();

	/* ����X�����ʼ�ͽ������� */
	LCD_ILI9341_CMD(0X2A); 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0xEF);
	
	/* ����Y�����ʼ�ͽ������� */	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0X2B); 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x01);
	LCD_ILI9341_Parameter(0x3F);

	/*  Pixel Format Set (3Ah)  */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0x3a); 
	LCD_ILI9341_Parameter(0x55);

	/* Sleep Out (11h)  */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0x11); 

	/* Display ON (29h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0x29); 						
}

/*
*********************************************************************************************************
*	�� �� ��: Set_direction
*	����˵��: ������Ļ����
*	��    ��: _usX1      ������
*             _usY1      ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD9341_SetCursor(uint16_t _usX1 , uint16_t _usY1)	
{				
		
	/*����*/
	LCD_ILI9341_CMD(0X2A); 				 /* ����X���� */
	LCD_ILI9341_Parameter(_usX1>>8);	 /* �ȸ�8λ��Ȼ���8λ */
	LCD_ILI9341_Parameter(_usX1&0xff);	 /* ������ʼ��ͽ�����*/
	LCD_ILI9341_Parameter(_usX1>>8);
	LCD_ILI9341_Parameter(_usX1&0xff);

    LCD_ILI9341_CMD(0X2B); 			     /* ����Y����*/
	LCD_ILI9341_Parameter(_usY1>>8);
	LCD_ILI9341_Parameter(_usY1&0xff);
	LCD_ILI9341_Parameter(_usY1>>8);
	LCD_ILI9341_Parameter(_usY1&0xff);  
		     
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_Clear
*	����˵��: ����Ļ����ָ������ɫ��������������� 0xffff
*	��    ��: dat      ���ֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD9341_Clear(uint16_t _usColor)	
{
    uint32_t n;

	/***************����************/
	LCD_ILI9341_CMD(0X2A); 			  /* ����X���� */
	LCD_ILI9341_Parameter(0);		  /* �ȸ�8λ��Ȼ���8λ */
	LCD_ILI9341_Parameter(0);		  /* ������ʼ��ͽ�����*/
	LCD_ILI9341_Parameter(239>>8);
	LCD_ILI9341_Parameter(239&0xff);
	
	LCD_ILI9341_CMD(0X2B); 			 /* ����Y����*/
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter(319>>8);
	LCD_ILI9341_Parameter(319&0xff);
		
	LCD_ILI9341_CMD(0x2c);
	for(n=0; n<320*240; n++)
	{
		LCD_WR_Data(_usColor);
	} 
}

/*
*********************************************************************************************************
*	�� �� ��: LCD9341_GetPoint
*	����˵��: ��ȡָ���������ɫֵ
*	��    ��: _usX1      ������
*             _usY1      ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t LCD9341_GetPoint(uint16_t _usX1 , uint16_t _usY1)
{ 
    uint16_t temp;

	LCD9341_SetCursor(_usX1,_usY1);
    LCD_ILI9341_CMD(0x2e);         /* ������ */
	temp=LCD_RD_data();
    return (temp);
}

/*
*********************************************************************************************************
*	�� �� ��: LCD9341_SetPoint
*	����˵��: ��ָ�����껭��
*	��    ��: _usX1      ������
*             _usY1      ������
*             _usColor   �����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD9341_SetPoint(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usColor)	
{   	
	
	LCD9341_SetCursor(_usX1,_usY1);
	LCD_ILI9341_CMD(0x2c);	         /* д���� */
	LCD_WR_Data(_usColor);

}

/*
*********************************************************************************************************
*	�� �� ��: LCD_SetDispWin
*	����˵��: ���ô�������
*	��    ��: _usX1    �������
           	  _usY1	   �������
              _usX2    ���������� 
              _usY2    ����������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD9341_SetDispHV(uint16_t _usX1, uint16_t _usY1, uint16_t _usX2, uint16_t _usY2, uint8_t _ucHV)
{                    
	/* Ĭ�ϵ�ɨ�跽ʽ */
	if(_ucHV == 1)
    {
		LCD_ILI9341_CMD(0x36); 
	    LCD_ILI9341_Parameter(0xc8);    
	    DEBUG_DELAY();

		LCD_ILI9341_CMD(0X2A); 				  /* ����X���� */
		LCD_ILI9341_Parameter(_usX1>>8);	  /* ���ø�8λ�͵�8λ*/
		LCD_ILI9341_Parameter(_usX1&0xff);	  /* ������ʼ��ͽ�����*/
		LCD_ILI9341_Parameter(_usX2>>8);	
		LCD_ILI9341_Parameter(_usX2&0xff);
		
		LCD_ILI9341_CMD(0X2B); 				  /* ����Y����*/
		LCD_ILI9341_Parameter(_usY1>>8);   
		LCD_ILI9341_Parameter(_usY1&0xff);
		LCD_ILI9341_Parameter(_usY2>>8);   
		LCD_ILI9341_Parameter(_usY2&0xff);
	
		LCD_ILI9341_CMD(0x2c);		
	}
	else
	{
		LCD_ILI9341_CMD(0x36); 
	    LCD_ILI9341_Parameter(0x08);    
	    DEBUG_DELAY();

		LCD_ILI9341_CMD(0X2A); 				  /* ����X���� */
		LCD_ILI9341_Parameter(_usX1>>8);	  /* ���ø�8λ�͵�8λ*/
		LCD_ILI9341_Parameter(_usX1&0xff);	  /* ������ʼ��ͽ�����*/
		LCD_ILI9341_Parameter(_usX2>>8);	
		LCD_ILI9341_Parameter(_usX2&0xff);
		
		LCD_ILI9341_CMD(0X2B); 				  /* ����Y����*/
		LCD_ILI9341_Parameter(_usY1>>8);   
		LCD_ILI9341_Parameter(_usY1&0xff);
		LCD_ILI9341_Parameter(_usY2>>8);   
		LCD_ILI9341_Parameter(_usY2&0xff);
	
		LCD_ILI9341_CMD(0x2c);
	}	
	     
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_SetDispWin
*	����˵��: ���ô�������
*	��    ��: _usX1    �������
           	  _usY1	   �������
              _usX2    ���������� 
              _usY2    ����������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD9341_SetDispWin(uint16_t _usX1, uint16_t _usY1, uint16_t _usX2, uint16_t _usY2)
{                    

	LCD_ILI9341_CMD(0X2A); 				  /* ����X���� */
	LCD_ILI9341_Parameter(_usX1>>8);	  /* ���ø�8λ�͵�8λ*/
	LCD_ILI9341_Parameter(_usX1&0xff);	  /* ������ʼ��ͽ�����*/
	LCD_ILI9341_Parameter(_usX2>>8);	
	LCD_ILI9341_Parameter(_usX2&0xff);
	
	LCD_ILI9341_CMD(0X2B); 				  /* ����Y����*/
	LCD_ILI9341_Parameter(_usY1>>8);   
	LCD_ILI9341_Parameter(_usY1&0xff);
	LCD_ILI9341_Parameter(_usY2>>8);   
	LCD_ILI9341_Parameter(_usY2&0xff);

	LCD_ILI9341_CMD(0x2c);     
}

/*
*********************************************************************************************************
*	�� �� ��: LCD9341_DrawHLine
*	����˵��: ��ˮƽ�� ��UCGUI�Ľӿں���
*	��    ��: X,Y���������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD9341_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor)
{
	uint16_t i;

#if 1
	
	LCD9341_SetDispWin(_usX1, _usY1, _usX2,_usY1);

	for (i = 0; i <_usX2-_usX1+1; i++)
	{
		ILI9341_RAM = _usColor;
	}

#else

	for (i = _usX1; i <=_usX2; i++)
	{	
		LCD9341_SetPoint(i, _usY1, _usColor);	
	}

#endif

}

/*
*********************************************************************************************************
*	�� �� ��: LCD9341_DrawVLine
*	����˵��: ����ֱƽ�� ��UCGUI�Ľӿں���
*	��    ��: X,Y���������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD9341_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor)
{
	uint16_t i;
#if 1

	LCD9341_SetDispWin(_usX1, _usY1,_usX1,_usY2);

	for (i = 0; i <_usY2-_usY1+1; i++)
	{
		ILI9341_RAM = _usColor;
	}
#else

	for (i = _usY1; i <=_usY2; i++)
	{	
		LCD9341_SetPoint(_usX1, i, _usColor);	
	}

#endif
}

/*
*********************************************************************************************************
*	�� �� ��: LCD9341_DrawVLine
*	����˵��: ��������� ��UCGUI�Ľӿں���
*	��    ��: X,Y���������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD9341_FillRect(uint16_t _usX1 , uint16_t _usY1 ,  uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor)
{                    
   uint32_t n, temp;
   
   LCD9341_SetDispWin(_usX1, _usY1,_usX2,_usY2); 
    
   temp = (u32)(_usX2-_usX1+1)*(_usY2 -_usY1+1);
       
   for(n=0; n<temp; n++)
   {
		ILI9341_RAM =_usColor;
   }
	 	  
}

/*
*********************************************************************************************************
*	�� �� ��: LCD9341_DrawVLine
*	����˵��: ��������� ��UCGUI�Ľӿں���
*	��    ��: X,Y���������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD9341_DrawJPG(uint16_t _usX1 , uint16_t _usY1 ,  uint16_t _usX2 , uint16_t _usY2 , uint16_t *_ptr)
{                    
   uint32_t n, temp;
   
   LCD9341_SetDispWin(_usX1, _usY1,_usX2,_usY2); 
    
   temp = (u32)(_usX2-_usX1+1)*(_usY2 -_usY1+1);
       
   for(n=0; n<temp; n++)
   {
		ILI9341_RAM = *_ptr++;
   }
	 	  
}

/**
  ******************************************************************************
  * @file    GUIDEMO_Fatfs.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试文件系统
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "stm32f10x.h"
#include "bsp_usart1.h"	
#include "ff.h"
#include "GUI.h"

extern FIL file;													/* file objects */
//FATFS fs;													/* Work area (file system object) for logical drives */
extern FRESULT result; 
extern UINT rw;            					/* File R/W count */
BYTE buffer[4096]={0};       		  /* file copy buffer */
BYTE textFileBuffer[] = "Welcome to use Wildfire ISO-V2 stm32 Development Board,\
this is a demo about enWin && Flash FatFS.\n\
today is a good day";

void Fatfs_MainTask(void)
{
	  /* 用于显示文件内容的窗口大小 */ 
		GUI_RECT file_content={5,120,235,250};
	
		/* 给屏幕刷上背景颜色 */
		GUI_ClearRect(0,0,240,320);	
	  
		/* 设置字体颜色 */
		GUI_SetFont(&GUI_Font8x16);

		GUI_DispStringAt ("f_mount...",5,20);
	
		/* Register work area for each volume (Always succeeds regardless of disk status) */
//		f_mount(&fs,"0:",1);	
//		f_mount(&fs,"1:",1);
		/* function disk_initialize() has been called in f_open */
		
	    GUI_DispStringAt ("f_open :newfile.txt ...",5,40);
	
		/* Create new file on the drive 0 */
		result = f_open(&file, "0:newfile.txt", FA_CREATE_ALWAYS | FA_WRITE );
		 
		if ( result == FR_OK )
		{
			result = f_write(&file, textFileBuffer, sizeof(textFileBuffer), &rw);
			f_close(&file);      
		}

		GUI_DispStringAt ("f_read :newfile.txt ...",5,60);
		
		result = f_open(&file, "0:newfile.txt", FA_OPEN_EXISTING | FA_READ); 	 
		result = f_read(&file, buffer, sizeof(buffer), &rw); 

		printf("\r\n %s ", buffer);
		
		GUI_DispStringAt ("file content:",5,100);
		
		/* 设置前景颜色（字体颜色）*/
		GUI_SetColor(GUI_RED);
		
		/* 设置字体 */
		GUI_SetFont(GUI_FONT_COMIC18B_ASCII);
		
		/* 显示文本到屏幕上 */
		GUI_DispStringInRectWrap((const char *)buffer,&file_content,GUI_TA_LEFT,GUI_WRAPMODE_WORD) ;
		
		/* Close open files */
		f_close(&file);	                                      
		
		//while(1);
}

	  


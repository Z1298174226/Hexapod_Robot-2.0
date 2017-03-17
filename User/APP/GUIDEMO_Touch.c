/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   GUIDEMO_Touch.c
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
	

#include "GUI.h"

/*********************************************************************
*
*       �ڴ���У����ʱ��ʹ���������ȡADC��ֵ
*/
void Touch_MainTask(void) {
 
  GUI_CURSOR_Show();
  GUI_CURSOR_Select(&GUI_CursorCrossL);
  GUI_SetBkColor(GUI_WHITE);
  GUI_SetColor(GUI_BLACK);
  GUI_Clear();
  GUI_DispStringAt("Measurement of A/D converter values",10,10);
  while (1) {
    GUI_PID_STATE TouchState;
    int xPhys, yPhys;
    GUI_TOUCH_GetState(&TouchState);  /* Get the touch position in pixel */
    xPhys = GUI_TOUCH_GetxPhys();     /* Get the A/D mesurement result in x */
    yPhys = GUI_TOUCH_GetyPhys();     /* Get the A/D mesurement result in y */
    /* Display the measurement result */
    GUI_SetColor(GUI_BLUE);
    GUI_DispStringAt("Analog input:\n", 10, 20);
    GUI_GotoX(GUI_GetDispPosX() + 10);
	GUI_GotoY(GUI_GetDispPosY() + 5);
    GUI_DispString("x:");
    GUI_DispDec(xPhys, 4);
    GUI_DispString(", y:");
    GUI_DispDec(yPhys, 4);
    /* Display the according position */
    GUI_SetColor(GUI_RED);
//    GUI_GotoY(GUI_GetDispPosY() + 5);
//	GUI_GotoX(GUI_GetDispPosX() + 10);
    GUI_DispStringAt("Position:\n",10,50);
	GUI_GotoX(GUI_GetDispPosX() + 10);
    GUI_GotoY(GUI_GetDispPosY() + 5);	  
    GUI_DispString("x:");
    GUI_DispDec(TouchState.x,4);
    GUI_DispString(", y:");
    GUI_DispDec(TouchState.y,4);
    /* Wait a while */
    GUI_Delay(100);
  };
}


/**********************************end of file***********************************************/




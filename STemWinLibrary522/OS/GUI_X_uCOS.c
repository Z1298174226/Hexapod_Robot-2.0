/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.20 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
---Author-Explanation
* 
* 1.00.00 020519 JJL    First release of uC/GUI to uC/OS-III interface
* 
*
* Known problems or limitations with current version
*
*    None.
*
*
* Open issues
*
*    None
*********************************************************************************************************
*/

#include "OS.h"
#include "os_cfg_app.h"
#include "GUI_Private.H"
#include "stdio.H"

/*
*********************************************************************************************************
*                                         GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_SEM  DispSem;
static  OS_SEM  EventSem;

static  OS_SEM  KeySem;
static  int     KeyPressed;
static  char    KeyIsInited;
	


/*
*********************************************************************************************************
*                                        TIMING FUNCTIONS
*
* Notes: Some timing dependent routines of uC/GUI require a GetTime and delay funtion. 
*        Default time unit (tick), normally is 1 ms.
*********************************************************************************************************
*/

GUI_TIMER_TIME  GUI_X_GetTime (void) 
{
		OS_ERR err;
    return ((GUI_TIMER_TIME)OSTimeGet(&err));
}


void  GUI_X_Delay (int period) 
{
    OS_TICK  ticks;
		OS_ERR err;

    ticks = (period * 1000) / OS_CFG_TICK_RATE_HZ;
    OSTimeDly((OS_TICK)ticks,OS_OPT_TIME_DLY,&err);
}


/*
*********************************************************************************************************
*                                          GUI_X_ExecIdle()
*********************************************************************************************************
*/
void GUI_X_ExecIdle (void) 
{
    GUI_X_Delay(1);
}


/*
*********************************************************************************************************
*                                    MULTITASKING INTERFACE FUNCTIONS
*
* Note(1): 1) The following routines are required only if uC/GUI is used in a true multi task environment, 
*             which means you have more than one thread using the uC/GUI API.  In this case the #define 
*             GUI_OS 1   needs to be in GUIConf.h
*********************************************************************************************************
*/

/* 创建通常由 GUI_X_Lock() 和 GUI_X_Unlock() 使用的资源信号量或互斥体。 */
void  GUI_X_InitOS (void)
{ 
		OS_ERR err;
	
		
		/* 用于资源共享 cnt = 1 */
		OSSemCreate((OS_SEM    *)&DispSem,
                (CPU_CHAR  *)"DispSem",
                (OS_SEM_CTR )1,
                (OS_ERR    *)&err);
	
	
    /* 用于事件触发 cnt = 0*/
		OSSemCreate((OS_SEM    *)&EventSem,
                (CPU_CHAR  *)"EventSem",
                (OS_SEM_CTR )0,
                (OS_ERR    *)&err);
}

/* 锁定GUI 等待信号量*/
void  GUI_X_Lock (void)
{ 
   OS_ERR  err;   
    
   OSSemPend((OS_SEM *)&DispSem,
						 (OS_TICK )0,
						 (OS_OPT  )OS_OPT_PEND_BLOCKING,
						 (CPU_TS  )0,
						 (OS_ERR *)&err);
}

/* 解锁 GUI 释放信号量 */
void  GUI_X_Unlock (void)
{ 
	OS_ERR     err;
	
	OSSemPost((OS_SEM *)&DispSem,
						(OS_OPT  )OS_OPT_POST_1,
						(OS_ERR *)&err);
}

/* 返回当前任务的唯一 ID */
U32  GUI_X_GetTaskId (void) 
{ 
  //由于存在同优先级的任务，这里不知道是不是可以
	//uCOS-III中已经没有任务的ID
	return ((U32)(OSTCBCurPtr->Prio));
}

/*
*********************************************************************************************************
*                                        GUI_X_WaitEvent()
*                                        GUI_X_SignalEvent()
*********************************************************************************************************
*/

/* 发送事件信号 */
void GUI_X_WaitEvent (void) 
{
  OS_ERR  err;
	   
	OSSemPend((OS_SEM *)&EventSem,
						(OS_TICK )0,
						(OS_OPT  )OS_OPT_PEND_BLOCKING,
						(CPU_TS  )0,
						(OS_ERR *)&err);
}

/* 在给定时段内等待事件 */
void GUI_X_SignalEvent (void) 
{
 	OS_ERR     err;

	OSSemPost((OS_SEM *)&EventSem,
						(OS_OPT  )OS_OPT_POST_1,
						(OS_ERR *)&err);
}

/*
*********************************************************************************************************
*                                      KEYBOARD INTERFACE FUNCTIONS
*
* Purpose: The keyboard routines are required only by some widgets.
*          If widgets are not used, they may be eliminated.
*
* Note(s): If uC/OS-II is used, characters typed into the log window will be placed	in the keyboard buffer. 
*          This is a neat feature which allows you to operate your target system without having to use or 
*          even to have a keyboard connected to it. (useful for demos !)
*********************************************************************************************************
*/

static  void  CheckInit (void) 
{
    if (KeyIsInited == DEF_FALSE) {
        KeyIsInited = DEF_TRUE;
        GUI_X_Init();
    }
}


void GUI_X_Init (void) 
{
  OS_ERR err;
	
	OSSemCreate((OS_SEM    *)&KeySem,
							(CPU_CHAR  *)"KeySem",
							(OS_SEM_CTR )0,
							(OS_ERR    *)&err);
}


int  GUI_X_GetKey (void) 
{
    int r;


    r          = KeyPressed;
    CheckInit();
    KeyPressed = 0;
    return (r);
}


int  GUI_X_WaitKey (void) 
{
    int    r;
    OS_ERR  err;


    CheckInit();
    if (KeyPressed == 0) {
     		OSSemPend((OS_SEM *)&KeySem,
									(OS_TICK )0,
									(OS_OPT  )OS_OPT_PEND_BLOCKING,
									(CPU_TS  )0,
									(OS_ERR *)&err);
    }
    r          = KeyPressed;
    KeyPressed = 0;
    return (r);
}


void  GUI_X_StoreKey (int k) 
{
	OS_ERR     err;
	
	KeyPressed = k;
	
	OSSemPost((OS_SEM *)&KeySem,
						(OS_OPT  )OS_OPT_POST_1,
						(OS_ERR *)&err);
}

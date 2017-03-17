/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                           (c) Copyright 2009-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                         Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : JJL
*                 EHS
*                 DC
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_CFG_SERIAL_EN                          DEF_DISABLED

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/
   


#define 	APP_TASK_START_PRIO                     	2u		//uCOS启动任务优先级
#define  	APP_CFG_TASK_SHOT_PRIO                      15u		//液晶截图如图优先级
#define  	APP_CFG_TASK_KEYBOARD_PRIO                  14u		//中英文输入法键盘优先级
#define  	APP_CFG_TASK_GUI_PRIO                   	4u		//emWin界面任务优先级
#define  	APP_CFG_TASK_Control_PRIO                   3u		//主控函数优先级
#define  	APP_CFG_TASK_Kalman_PRIO                    6u		//倾角显示函数优先级
#define  	APP_CFG_TASK_Height_PRIO                    7u		//高度控制函数优先级
/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE                    	128u		//uCOS启动任务堆栈大小
#define  APP_CFG_TASK_SHOT_STK_SIZE                     256u		//液晶截图如图任务堆栈大小
#define  APP_CFG_TASK_KEYBOARD_STK_SIZE                 64u		//中英文输入法键盘任务堆栈大小
#define  APP_CFG_TASK_GUI_STK_SIZE                   	2*1024u		//emWin界面任务堆栈大小
#define  APP_CFG_TASK_Control_STK_SIZE                  256u		//主控函数任务堆栈大小
#define  APP_CFG_TASK_Kalman_STK_SIZE                   256u		//主控函数任务堆栈大小
#define  APP_CFG_TASK_Height_STK_SIZE                   128u		//高度控制任务堆栈大小
/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/
#if 0
#define  TRACE_LEVEL_OFF                            0
#define  TRACE_LEVEL_INFO                           1
#define  TRACE_LEVEL_DEBUG                          2
#endif

#define  APP_TRACE_LEVEL                            TRACE_LEVEL_INFO
#define  APP_TRACE                                  printf

#define  APP_TRACE_INFO(x)            ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DEBUG(x)           ((APP_TRACE_LEVEL >= TRACE_LEVEL_DEBUG) ? (void)(APP_TRACE x) : (void)0)


#endif

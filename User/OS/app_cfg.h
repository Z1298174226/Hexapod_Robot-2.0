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
   


#define 	APP_TASK_START_PRIO                     	2u		//uCOS�����������ȼ�
#define  	APP_CFG_TASK_SHOT_PRIO                      15u		//Һ����ͼ��ͼ���ȼ�
#define  	APP_CFG_TASK_KEYBOARD_PRIO                  14u		//��Ӣ�����뷨�������ȼ�
#define  	APP_CFG_TASK_GUI_PRIO                   	4u		//emWin�����������ȼ�
#define  	APP_CFG_TASK_Control_PRIO                   3u		//���غ������ȼ�
#define  	APP_CFG_TASK_Kalman_PRIO                    6u		//�����ʾ�������ȼ�
#define  	APP_CFG_TASK_Height_PRIO                    7u		//�߶ȿ��ƺ������ȼ�
/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE                    	128u		//uCOS���������ջ��С
#define  APP_CFG_TASK_SHOT_STK_SIZE                     256u		//Һ����ͼ��ͼ�����ջ��С
#define  APP_CFG_TASK_KEYBOARD_STK_SIZE                 64u		//��Ӣ�����뷨���������ջ��С
#define  APP_CFG_TASK_GUI_STK_SIZE                   	2*1024u		//emWin���������ջ��С
#define  APP_CFG_TASK_Control_STK_SIZE                  256u		//���غ��������ջ��С
#define  APP_CFG_TASK_Kalman_STK_SIZE                   256u		//���غ��������ջ��С
#define  APP_CFG_TASK_Height_STK_SIZE                   128u		//�߶ȿ��������ջ��С
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

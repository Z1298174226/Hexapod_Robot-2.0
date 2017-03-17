/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                         Evaluation Board
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : EHS
*                 DC
*********************************************************************************************************
*/

#ifndef  INCLUDES_PRESENT
#define  INCLUDES_PRESENT


/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>


/*
*********************************************************************************************************
*                                              APP / BSP
*********************************************************************************************************
*/

#include  <bsp.h>
#include  "bsp_led.h"
#include "bsp_touch.h"
#include "bsp_usart1.h"
#include "key.h"
#include "ff.h"
#include "TIM6.h"
#include "UltrasonicWave.h"
#include "pwm.h"
#include "kalman.h"
#include "bsp_i2c.h"
#include "mpu6050.h"
#include "control.h"
#include "bsp_SysTick.h"
#include "bsp_exti.h"

/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <os.h>
#include  <app_cfg.h>
#include  "app.h"


 

/*
*********************************************************************************************************
*                                                 ST
*********************************************************************************************************
*/
#include  <stm32f10x.h>
/*
*********************************************************************************************************
*                                                 emWin
*********************************************************************************************************
*/
#include "GUI.h"

/*
*********************************************************************************************************
*                                            INCLUDES END
*********************************************************************************************************
*/

#define __DEBUG 	//¿ªÆô´®¿Úµ÷ÊÔ

#ifdef __DEBUG

#define DEBUG(format,...) printf("FILE: "__FILE__", LINE: %d: "format"\n", __LINE__, ##__VA_ARGS__)

#else

#define DEBUG(format,...)

#endif

#endif


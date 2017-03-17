#ifndef __PWM_H
#define __PWM_H


#include "stm32f10x.h"


void TIM_Init(void);
void TIM1_Init(void);
void TIM2_Init(void);
void TIM3_Init(void);
void TIM4_Init(void);
void TIM5_Init(void);
void Close_All_TIM(void);
void Open_All_TIM(void);
#endif

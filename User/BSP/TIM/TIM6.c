/******************************************************************************
  * 文件路径  ：   Hardward/TIM6.c
  * 作者      ：   倾听枫声   QQ：1298174226
  * 固件库版本：   V3.5.0
  * 日期      ：   2015-8-2
  * 文件描述  ：   TIM6.c
  ******************************************************************************/
/*******************超声波模块控制**********************/
#include "TIM6.h"

/*
 * 函数名：TIM2_NVIC_Configuration
 * 描述  ：TIM2中断优先级配置
 * 输入  ：无
 * 输出  ：无	
 */
void TIM6_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);
    TIM_DeInit(TIM6);
    TIM_TimeBaseStructure.TIM_Period=65535;		 								/* 自动重装载寄存器周期的值(计数值) */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= (360 - 1);				                /* 时钟预分频数 72M/360 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		                /* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;                   /* 向上计数模式 */
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM6, ENABLE);																		/* 开启时钟 */
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*先关闭等待使用*/    
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

void TIM7_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定义一个定时器结构体变量

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//使能定时器2
  
  TIM_TimeBaseStructure.TIM_Period = (10000 - 1);	//计数100次，因为从0开始，所以减1
  TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);	//时钟72分频，因为0不分频，所以减1
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	// 使用的采样频率之间的分频比例
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);	//初始化定时器2

  TIM_ClearITPendingBit(TIM7, TIM_IT_Update);		//清除定时器2中断标志位
  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);		//打开定时器2中断

  TIM_Cmd(TIM7, ENABLE);  //计数器使能，开始计数
}

void TIM7_NVIC_Configuration(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

/********************注册定时器2中断处理函数***********************/

  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


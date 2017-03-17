
#include "delay.h"
 /******************************************************************************
  * 文件路径  ：   Hardward/delay.c
  * 作者      ：   倾听枫声   QQ：1298174226
  * 固件库版本：   V3.5.0
  * 日期      ：   2015-8-2
  * 文件描述  ：   延时函数
  ******************************************************************************/
/*******************延时函数**********************/
// static __IO uint32_t TimingDelay;
/***************************************************************************
*函数名：SysTick_Init()
*参数：void			
*返回值:void
*功能：系统节拍时钟初始化函数
*****************************************************************************
void SysTick_Init(void)
{
	 /****************************************
	 *SystemFrequency/1000      1ms中断一次  *
	 *SystemFrequency/100000    10us中断一次 *
	 *SystemFrequency/1000000   1us中断一次  *
	 *****************************************/
 //   while(SysTick_Config(SystemCoreClock/1000)!=0);    //设置系统节拍时钟为1us中断一次
	
	/*附加笔记：
	SysTick_Config(uint32_t ticks)函数为core_cm4.h中定义的设置节拍时钟的函数，当设置成功时，返回0，失败返回1
	*/
//}
/***************************************************************************
*函数名：TimeTick_Decrement()
*参数：void
*返回值:void
*功能：系统节拍时钟中断计数，给系统节拍中断函数调用
*****************************************************************************
void TimeTick_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
/***************************************************************************
*函数名：delay_nus()
*参数：u32 n  时钟延时的微秒数
*返回值:void
*功能：使用系统节拍时钟精准延时n个微秒函数
*****************************************************************************/
/*void delay_nus(u16 n)
{
	TimingDelay=n;
	while(TimingDelay!=0);
}
 /***************************************************************************
*函数名：delay_1ms()
*参数：void
*返回值:void
*功能：使用系统节拍时钟精准延时1个毫秒函数
*****************************************************************************/
/*void delay_1ms(void)
{
    TimingDelay=1000;
	while(TimingDelay!=0);
}
/***************************************************************************
*函数名：delay_nms()
*参数：u32 n  时钟延时的毫秒数
*返回值:void
*功能：系统节拍时钟初始化函数
*****************************************************************************/
/****************************************************************************
void delay_ms(u16 n)
{
  while(n--)delay_1ms();
  
}
void delay_nms(u16 n)
{
  while(n--)delay_1ms();
  
}
*******************************************************************************/



static __IO u32 TimingDelay;
 
/**
  * @brief  启动系统滴答定时器 SysTick
  * @param  无
  * @retval 无
  */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
	if (SysTick_Config(SystemCoreClock / 100000))	// ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1);
	}
		// 关闭滴答定时器  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief   us延时程序,10us为一个单位
  * @param  
  *		@arg nTime: Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
  * @retval  无
  */
void Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	// 使能滴答定时器  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);
}

/**
  * @brief  获取节拍程序
  * @param  无
  * @retval 无
  * @attention  在 SysTick 中断函数 SysTick_Handler()调用
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
/*********************************************END OF FILE**********************/

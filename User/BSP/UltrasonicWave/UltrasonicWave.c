/******************************************************************************
  * 文件路径  ：   Hardward/UltrasonicWave.c
  * 作者      ：   倾听枫声   QQ：1298174226
  * 固件库版本：   V3.5.0
  * 日期      ：   2015-8-2
  * 文件描述  ：   UltrasonicWave.c
  ******************************************************************************/
/*******************超声波模块控制**********************/
#include "UltrasonicWave.h"
#include "bsp_usart1.h"
#include "TIM6.h"
#include "bsp_SysTick.h"

#define	TRIG_PORT      GPIOB		//TRIG       
#define	ECHO_PORT      GPIOB		//ECHO 
#define	TRIG_PIN       GPIO_Pin_12   //TRIG       
#define	ECHO_PIN       GPIO_Pin_13	//ECHO   
#define	TRIG1_PIN      GPIO_Pin_14   //TRIG1       
#define	ECHO1_PIN      GPIO_Pin_15	//ECHO1  
#define	TRIG2_PIN      GPIO_Pin_8   //TRIG1       
#define	ECHO2_PIN      GPIO_Pin_9	//ECHO1

unsigned short int UltrasonicWave_Distance;      //计算出的距离 
unsigned short int UltrasonicWave_Distance1;      //计算出的距离  
unsigned short int UltrasonicWave_Distance2;      //计算出的距离      

/*
 * 函数名：DelayTime_us
 * 描述  ：1us延时函数
 * 输入  ：Time   	延时的时间 US
 * 输出  ：无	
 */


/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
  	       
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = TRIG_PIN|TRIG1_PIN|TRIG2_PIN;					 //PC8接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	                 //初始化外设GPIO 

  GPIO_InitStructure.GPIO_Pin = ECHO_PIN|ECHO1_PIN|ECHO2_PIN;				     //PC9接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为输入
  GPIO_Init(ECHO_PORT,&GPIO_InitStructure);						 //初始化GPIOA
}

/*
 * 函数名：UltrasonicWave_CalculateTime
 * 描述  ：计算距离
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave1_CalculateTime(void)
{
   UltrasonicWave_Distance=TIM_GetCounter(TIM6)*5*34/2000;
   
}
void UltrasonicWave2_CalculateTime(void)
{
   UltrasonicWave_Distance1=TIM_GetCounter(TIM6)*5*34/2000;
   
}
void UltrasonicWave3_CalculateTime(void)
{
   UltrasonicWave_Distance2=TIM_GetCounter(TIM6)*5*34/2000;
   
}

/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave1_StartMeasure(void)
{
  GPIO_SetBits(TRIG_PORT,TRIG_PIN); 		  //送>10US的高电平
  //Delay_us(2);		                      //延时20US
  delay_diy(1500);
  GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
  while(!GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	             //等待高电平
  TIM_Cmd(TIM6, ENABLE);                                             //开启时钟
  while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	                 //等待低电平
  TIM_Cmd(TIM6, DISABLE);			                                 //定时器6失能
  UltrasonicWave1_CalculateTime();									 //计算距离
  TIM_SetCounter(TIM6,0);
//  printf("\r\ndistance:%d%d cm\r\n",UltrasonicWave_Distance/256,UltrasonicWave_Distance%256);	            	
	
  	  
}
void UltrasonicWave2_StartMeasure(void)
{
  GPIO_SetBits(TRIG_PORT,TRIG1_PIN); 		  //送>10US的高电平
  //Delay_us(2);		                      //延时20US
  delay_diy(2500);
  GPIO_ResetBits(TRIG_PORT,TRIG1_PIN);
  while(!GPIO_ReadInputDataBit(ECHO_PORT,ECHO1_PIN));	             //等待高电平
  TIM_Cmd(TIM6, ENABLE);                                             //开启时钟
  while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO1_PIN));	                 //等待低电平
  TIM_Cmd(TIM6, DISABLE);			                                 //定时器6失能
  UltrasonicWave2_CalculateTime();									 //计算距离
  TIM_SetCounter(TIM6,0);
  //printf("\r\ndistance1:%d%d cm\r\n",UltrasonicWave_Distance1/256,UltrasonicWave_Distance1%256);	
	
  	  
}
void UltrasonicWave3_StartMeasure(void)
{
  GPIO_SetBits(TRIG_PORT,TRIG2_PIN); 		  //送>10US的高电平
  //Delay_us(2);		                      //延时20US
  delay_diy(2500);
  GPIO_ResetBits(TRIG_PORT,TRIG2_PIN);
  while(!GPIO_ReadInputDataBit(ECHO_PORT,ECHO2_PIN));	             //等待高电平
  TIM_Cmd(TIM6, ENABLE);                                             //开启时钟
  while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO2_PIN));	                 //等待低电平
  TIM_Cmd(TIM6, DISABLE);			                                 //定时器6失能
  UltrasonicWave3_CalculateTime();									 //计算距离
  TIM_SetCounter(TIM6,0);
 // printf("\r\ndistance3:%d%d cm\r\n",UltrasonicWave_Distance1/256,UltrasonicWave_Distance1%256);	
	
  	  
}
void UltrasonicWave_StartMeasure(void)
{
   UltrasonicWave1_StartMeasure();
   UltrasonicWave2_StartMeasure();
   UltrasonicWave3_StartMeasure();

}

void delay_diy (int cntime)
{
   while(cntime!=0)
   {
    cntime--;
   }
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

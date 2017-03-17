#include "usart.h"
 /******************************************************************************
  * 文件路径  ：   User/main.c
  * 作者      ：   倾听枫声   QQ：1298174226
  * 固件库版本：   V3.5.0
  * 日期      ：   2015-8-2
  * 文件描述  ：   主函数
  ******************************************************************************/
/*******************main函数**********************/
/*******************************************************************************************************************************
 * 函数名：USART1_Config()
 * 参数：无
 *返回值：无
 * 功能：配置USART1，并初始化
********************************************************************************************************************************/																		
void USART1_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		
		/* config USART1 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
		
		/* USART1 GPIO config */
		/* Configure USART1 Tx (PA.09) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);    
		/* Configure USART1 Rx (PA.10) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
		/* USART1 mode config */
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure); 
		
			/* 使能串口1接收中断 */
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		
		NVIC_Configuration();
		
		USART_Cmd(USART1, ENABLE);
}


/// 配置USART1中断
 void Usart1_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



/********************************************************************************************************************************
*函数名：fputc()
* 参数：int ch，FILE *f
* 返回值：int
* 功能：重新定义stdio.h中的fputc()函数，使printf()输出到USART1
*********************************************************************************************************************************/
int fputc(int ch,FILE *f)
{
  USART_SendData(USART1,(u8)ch);	                                                   //将ch从USART1发送出去
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)				               //等待发送完毕
  {
  }											  
  return ch;																	   //返回 ch
}																		           









/******************************************************************************
  * �ļ�·��  ��   Hardward/TIM6.c
  * ����      ��   ��������   QQ��1298174226
  * �̼���汾��   V3.5.0
  * ����      ��   2015-8-2
  * �ļ�����  ��   TIM6.c
  ******************************************************************************/
/*******************������ģ�����**********************/
#include "TIM6.h"

/*
 * ��������TIM2_NVIC_Configuration
 * ����  ��TIM2�ж����ȼ�����
 * ����  ����
 * ���  ����	
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


/*TIM_Period--1000   TIM_Prescaler--71 -->�ж�����Ϊ1ms*/
void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);
    TIM_DeInit(TIM6);
    TIM_TimeBaseStructure.TIM_Period=65535;		 								/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= (360 - 1);				                /* ʱ��Ԥ��Ƶ�� 72M/360 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		                /* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;                   /* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);							    		/* �������жϱ�־ */
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM6, ENABLE);																		/* ����ʱ�� */
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*�ȹرյȴ�ʹ��*/    
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

void TIM7_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//����һ����ʱ���ṹ�����

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//ʹ�ܶ�ʱ��2
  
  TIM_TimeBaseStructure.TIM_Period = (10000 - 1);	//����100�Σ���Ϊ��0��ʼ�����Լ�1
  TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);	//ʱ��72��Ƶ����Ϊ0����Ƶ�����Լ�1
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	// ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);	//��ʼ����ʱ��2

  TIM_ClearITPendingBit(TIM7, TIM_IT_Update);		//�����ʱ��2�жϱ�־λ
  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);		//�򿪶�ʱ��2�ж�

  TIM_Cmd(TIM7, ENABLE);  //������ʹ�ܣ���ʼ����
}

void TIM7_NVIC_Configuration(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

/********************ע�ᶨʱ��2�жϴ�����***********************/

  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


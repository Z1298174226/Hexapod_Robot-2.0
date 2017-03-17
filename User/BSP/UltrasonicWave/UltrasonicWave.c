/******************************************************************************
  * �ļ�·��  ��   Hardward/UltrasonicWave.c
  * ����      ��   ��������   QQ��1298174226
  * �̼���汾��   V3.5.0
  * ����      ��   2015-8-2
  * �ļ�����  ��   UltrasonicWave.c
  ******************************************************************************/
/*******************������ģ�����**********************/
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

unsigned short int UltrasonicWave_Distance;      //������ľ��� 
unsigned short int UltrasonicWave_Distance1;      //������ľ���  
unsigned short int UltrasonicWave_Distance2;      //������ľ���      

/*
 * ��������DelayTime_us
 * ����  ��1us��ʱ����
 * ����  ��Time   	��ʱ��ʱ�� US
 * ���  ����	
 */


/*
 * ��������UltrasonicWave_Configuration
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
  	       
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = TRIG_PIN|TRIG1_PIN|TRIG2_PIN;					 //PC8��TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	                 //��ʼ������GPIO 

  GPIO_InitStructure.GPIO_Pin = ECHO_PIN|ECHO1_PIN|ECHO2_PIN;				     //PC9��ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //��Ϊ����
  GPIO_Init(ECHO_PORT,&GPIO_InitStructure);						 //��ʼ��GPIOA
}

/*
 * ��������UltrasonicWave_CalculateTime
 * ����  ���������
 * ����  ����
 * ���  ����	
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
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave1_StartMeasure(void)
{
  GPIO_SetBits(TRIG_PORT,TRIG_PIN); 		  //��>10US�ĸߵ�ƽ
  //Delay_us(2);		                      //��ʱ20US
  delay_diy(1500);
  GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
  while(!GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	             //�ȴ��ߵ�ƽ
  TIM_Cmd(TIM6, ENABLE);                                             //����ʱ��
  while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	                 //�ȴ��͵�ƽ
  TIM_Cmd(TIM6, DISABLE);			                                 //��ʱ��6ʧ��
  UltrasonicWave1_CalculateTime();									 //�������
  TIM_SetCounter(TIM6,0);
//  printf("\r\ndistance:%d%d cm\r\n",UltrasonicWave_Distance/256,UltrasonicWave_Distance%256);	            	
	
  	  
}
void UltrasonicWave2_StartMeasure(void)
{
  GPIO_SetBits(TRIG_PORT,TRIG1_PIN); 		  //��>10US�ĸߵ�ƽ
  //Delay_us(2);		                      //��ʱ20US
  delay_diy(2500);
  GPIO_ResetBits(TRIG_PORT,TRIG1_PIN);
  while(!GPIO_ReadInputDataBit(ECHO_PORT,ECHO1_PIN));	             //�ȴ��ߵ�ƽ
  TIM_Cmd(TIM6, ENABLE);                                             //����ʱ��
  while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO1_PIN));	                 //�ȴ��͵�ƽ
  TIM_Cmd(TIM6, DISABLE);			                                 //��ʱ��6ʧ��
  UltrasonicWave2_CalculateTime();									 //�������
  TIM_SetCounter(TIM6,0);
  //printf("\r\ndistance1:%d%d cm\r\n",UltrasonicWave_Distance1/256,UltrasonicWave_Distance1%256);	
	
  	  
}
void UltrasonicWave3_StartMeasure(void)
{
  GPIO_SetBits(TRIG_PORT,TRIG2_PIN); 		  //��>10US�ĸߵ�ƽ
  //Delay_us(2);		                      //��ʱ20US
  delay_diy(2500);
  GPIO_ResetBits(TRIG_PORT,TRIG2_PIN);
  while(!GPIO_ReadInputDataBit(ECHO_PORT,ECHO2_PIN));	             //�ȴ��ߵ�ƽ
  TIM_Cmd(TIM6, ENABLE);                                             //����ʱ��
  while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO2_PIN));	                 //�ȴ��͵�ƽ
  TIM_Cmd(TIM6, DISABLE);			                                 //��ʱ��6ʧ��
  UltrasonicWave3_CalculateTime();									 //�������
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

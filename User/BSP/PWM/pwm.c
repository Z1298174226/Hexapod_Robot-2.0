 /******************************************************************************
  * 文件路径  ：   User/main.c
  * 作者      ：   倾听枫声   QQ：1298174226
  * 固件库版本：   V3.5.0
  * 日期      ：   2015-8-2
  * 文件描述  ：   主函数
  ******************************************************************************/
/*******************main函数**********************/
 #include "pwm.h"
 
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 TIM_OCInitTypeDef  TIM_OCInitStructure;
 TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
 uint16_t CCR1_Val;
 uint16_t CCR2_Val;
 uint16_t CCR3_Val;
 uint16_t CCR4_Val;
 uint16_t PrescalerValue=999;

 void TIM1_Config(void);
 void TIM2_Config(void);
 void TIM3_Config(void);
 void TIM4_Config(void);
 void TIM5_Config(void);
 
 void TIM_Init(void)
 {
 	TIM1_Init();
	TIM2_Init();
	TIM3_Init();
	TIM4_Init();
	TIM5_Init();
	
 }
 
 void Close_All_TIM(void)
 {
 	TIM_Cmd(TIM1, DISABLE);
 	TIM_Cmd(TIM2, DISABLE);
 	TIM_Cmd(TIM3, DISABLE);
 	TIM_Cmd(TIM4, DISABLE);
 	TIM_Cmd(TIM5, DISABLE);
 }
 void Open_All_TIM(void)
 {
 	TIM_Cmd(TIM1, ENABLE);
 	TIM_Cmd(TIM2, ENABLE);
 	TIM_Cmd(TIM3, ENABLE);
 	TIM_Cmd(TIM4, ENABLE);
 	TIM_Cmd(TIM5, ENABLE);
 }
 
 void TIM3_Init(void)							 
 {

   /* TIM Configuration */
  TIM3_Config();
   
   /* -----------------------------------------------------------------------
     TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles.
    
     In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
     since APB1 prescaler is different from 1.   
       TIM3CLK = 2 * PCLK1  
       PCLK1 = HCLK / 4 
       => TIM3CLK = HCLK / 2 = SystemCoreClock /2
           
     To get TIM3 counter clock at 21 MHz, the prescaler is computed as follows:
        Prescaler = (TIM3CLK / TIM3 counter clock) - 1
        Prescaler = ((SystemCoreClock /2) /21 MHz) - 1
                                               
     To get TIM3 output clock at 30 KHz, the period (ARR)) is computed as follows:
        ARR = (TIM3 counter clock / TIM3 output clock) - 1
            = 665
                   
     TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
     TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
     TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
     TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
 
     Note: 
      SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
      Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
      function to update SystemCoreClock variable value. Otherwise, any configuration
      based on this variable will be incorrect.    
   ----------------------------------------------------------------------- */   

   /* Compute the prescaler value */
  // PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 500) - 1;
 
   /* Time base configuration */
   TIM_TimeBaseStructure.TIM_Period = 1440-1;
   TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 
   /* PWM1 Mode configuration: Channel1 */
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
 
   TIM_OC1Init(TIM3, &TIM_OCInitStructure);
 
   TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel2 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
 
   TIM_OC2Init(TIM3, &TIM_OCInitStructure);
 
   TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel3 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
 
   TIM_OC3Init(TIM3, &TIM_OCInitStructure);
 
   TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel4 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
 
   TIM_OC4Init(TIM3, &TIM_OCInitStructure);
 
   TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
 
   TIM_ARRPreloadConfig(TIM3, ENABLE);
 
   /* TIM3 enable counter */
   TIM_Cmd(TIM3, ENABLE);
 
 }
 
 void TIM3_Config(void)
 {
   GPIO_InitTypeDef GPIO_InitStructure;
 
   /* TIM3 clock enable */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 
   /* GPIOC clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

   GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
   
   /* GPIOC Configuration: TIM3 CH1 (PC6), TIM3 CH2 (PC7), TIM3 CH3 (PC8) and TIM3 CH4 (PC9) */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure); 
 
   /* Connect TIM3 pins to AF2 */  
//   GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
//   GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3); 
//   GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
//   GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3); 
 }
 
 void TIM4_Init(void)
 {
  TIM4_Config();
   /* Compute the prescaler value */
 //  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 500) - 1;
 
   /* Time base configuration */
   TIM_TimeBaseStructure.TIM_Period = 1440-1;
   TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
   TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
   /* PWM1 Mode configuration: Channel1 */
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
 
   TIM_OC1Init(TIM4, &TIM_OCInitStructure);
 
   TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel2 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
 
   TIM_OC2Init(TIM4, &TIM_OCInitStructure);
 
   TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel3 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
 
   TIM_OC3Init(TIM4, &TIM_OCInitStructure);
 
   TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel4 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
 
   TIM_OC4Init(TIM4, &TIM_OCInitStructure);
 
   TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
   TIM_ARRPreloadConfig(TIM4, ENABLE);
 
   /* TIM3 enable counter */
   TIM_Cmd(TIM4, ENABLE);
 
 }
 
 void TIM4_Config(void)
 {
 
 //  GPIO_InitTypeDef GPIO_InitStructure;
 
   /* TIM4 clock enable */
  // RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 
   /* GPIOB clock enable */
 //  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
  
 //  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  
   
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
 //  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
 //  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 //  GPIO_Init(GPIOB, &GPIO_InitStructure); 
/******************************************************************************** 

    GPIO_InitTypeDef GPIO_InitStructure;

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

**************************************************************************************/
   GPIO_InitTypeDef GPIO_InitStructure;

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD  | RCC_APB2Periph_AFIO, ENABLE);  
	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOC,GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9);
   /* Connect TIM3 pins to AF2 */  
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4); 
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4); 
 }
 
void TIM5_Init(void)
 {
  TIM5_Config();
   /* Compute the prescaler value */
 //  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 500) - 1;
 
   /* Time base configuration */
   TIM_TimeBaseStructure.TIM_Period = 1440-1;
   TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
   TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
 
   /* PWM1 Mode configuration: Channel1 */
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
 
   TIM_OC1Init(TIM5, &TIM_OCInitStructure);
 
   TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel2 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
 
   TIM_OC2Init(TIM5, &TIM_OCInitStructure);
 
   TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel3 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
 
   TIM_OC3Init(TIM5, &TIM_OCInitStructure);
 
   TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel4 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
 
   TIM_OC4Init(TIM5, &TIM_OCInitStructure);
 
   TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
 
   TIM_ARRPreloadConfig(TIM5, ENABLE);
 
   /* TIM3 enable counter */
   TIM_Cmd(TIM5, ENABLE);
 
 }
 
 void TIM5_Config(void)
 {
   GPIO_InitTypeDef GPIO_InitStructure;
 
   /* TIM4 clock enable */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
 
   /* GPIOB clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);


  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  // GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure); 
 
   /* Connect TIM3 pins to AF2 */  
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4); 
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4); 
 }
  
 
 void TIM1_Init(void)
 {
  TIM1_Config();
   /* Compute the prescaler value */
//   PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 500) - 1;
 
   /* Time base configuration */
   TIM_TimeBaseStructure.TIM_Period = 1440-1;
   TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
   //TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //死区控制用。
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
   TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 
   /* PWM1 Mode configuration: Channel1 */
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//输出同相，TIM_OCNPolarity_High时输出反相
   TIM_OCInitStructure.TIM_OCIdleState	= TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState	= TIM_OCNIdleState_Reset;
 
   TIM_OC1Init(TIM1, &TIM_OCInitStructure);
 
   TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel2 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
 
   TIM_OC2Init(TIM1, &TIM_OCInitStructure);
 
   TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel3 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
 
   TIM_OC3Init(TIM1, &TIM_OCInitStructure);
 
   TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel4 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
 
   TIM_OC4Init(TIM1, &TIM_OCInitStructure);
 
   TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

   TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
   TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
   TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
 
   TIM_ARRPreloadConfig(TIM1, ENABLE);
 
   /* TIM1 enable counter */
   TIM_Cmd(TIM1, ENABLE);
   TIM_CtrlPWMOutputs(TIM1, ENABLE);							  //pwm输出使能，一定要记得打开
 }
 
 void TIM1_Config(void)
 {
   GPIO_InitTypeDef GPIO_InitStructure;
 
   /* TIM1 clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
   
   /* GPIOE clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
   
   GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);
  
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14 |GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   
   GPIO_Init(GPIOE, &GPIO_InitStructure); 
 
   /* Connect TIM1 pins to AF2 */  
//   GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
  // GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1); 
 //  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
   //GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1); 
 }
 void TIM2_Init(void)
 {
  TIM2_Config();
   /* Compute the prescaler value */
   
 
   /* Time base configuration */
   TIM_TimeBaseStructure.TIM_Period = 1440-1;
   TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
 
   /* PWM1 Mode configuration: Channel1 */
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
 
   TIM_OC1Init(TIM2, &TIM_OCInitStructure);
 
   TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
 				       
   /* PWM1 Mode configuration: Channel2 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
 
   TIM_OC2Init(TIM2, &TIM_OCInitStructure);
 
   TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel3 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
 
   TIM_OC3Init(TIM2, &TIM_OCInitStructure);
 
   TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
 
   /* PWM1 Mode configuration: Channel4 */
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
 
   TIM_OC4Init(TIM2, &TIM_OCInitStructure);
 
   TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
 
   TIM_ARRPreloadConfig(TIM2, ENABLE);
 
   /* TIM2 enable counter */
   TIM_Cmd(TIM2, ENABLE);
 
 }
 
 void TIM2_Config(void)
 {
   GPIO_InitTypeDef GPIO_InitStructure;
 
   /* TIM2 clock enable */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   /* GPIOB clock enable */

   GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
  
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure); 
   
 
   /* Connect TIM2 pins to AF2 */  
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_TIM2); 
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource2, GPIO_AF_TIM5);
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM5); 
 }



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
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : EHS
*                 DC
*********************************************************************************************************
*/
/******************************************************************************
  * 文件路径  ：   User/main.c
  * 作者      ：   倾听枫声   QQ：1298174226
  * 固件库版本：   V3.5.0
  * 西安电子科技大学                          
  * 日期      ：   2015-10-2
  * 文件描述  ：   main.c
  ******************************************************************************/
/*******************main.c**********************/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
extern u16 ACTION_SPEED;      //range:0 - 10
extern u16 SHRINK_SPEED;      //((u16)((0.6)*ACTION_SPEED))
extern u16 ACTION_DELAY;      //ms	    //20
extern u16 TURN_DELAY;        //4
extern u16 STRAIGHT_DELAY;    //ms	   //3
extern u16 STAMP_DELAY;       //stamp delay
extern u16 ALTITUDE_DELAY;
extern u16 STAND_HIGHT;
extern u16 SHRINK_DEGREE;     //((u16)((0.6)*STAND_HIGHT))
extern u16 Init_Height;
extern volatile u16 Raise;
extern volatile u16 Raise1;
int Height[2]={0,0};
extern int pwm_init[26];
extern int pwm_init1[26];
extern float Angle; 


/************************/
extern unsigned short int UltrasonicWave_Distance;      //计算出的距离 
extern unsigned short int UltrasonicWave_Distance1;      //计算出的距离  
extern unsigned short int UltrasonicWave_Distance2;      //计算出的距离  
/* 
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/
 u8  rec_flag;					   
 u8  buffercontorl[3];
 u8  number;
 
 short Accel[3];
 short Gyro[3];
 short Temp;
 u16 lean_flag=1;
/****定义全局数组******/
//static u8 Datadistance;

/*******************任务控制块声明***************************/
static  OS_TCB   AppTaskStartTCB;
static  OS_TCB   AppTaskGUITCB;
static  OS_TCB   AppTaskSHOTTCB;
static  OS_TCB   AppTaskControlTCB;
static  OS_TCB   AppTaskKalmanTCB;
static  OS_TCB   AppTaskHeightTCB;
OS_TCB   AppTaskKEYBOARDTCB;
/******************信号量声明********************************/
static  OS_SEM     	SEM_KEY2;
static  OS_SEM      User_Sem1;
static  OS_SEM      User_Kalman;
static  OS_SEM      User_Wave;
static  OS_SEM      User_avoidance;
static  OS_SEM      User_Height;

/******************消息队列声明********************************/
static  OS_Q        User_Q1;	 //超声波

__IO uint8_t key_flag=1;//1：允许按键截图，0：禁用按键截图
static KEY Key1,Key2;

FIL file;									/* file objects */
FRESULT result; 
UINT rw;            						/* File R/W count */
/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/
/******************任务堆栈数组声明********************************/
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];
__align(8) static  CPU_STK  AppTaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE];
static  CPU_STK  AppTaskSHOTStk[APP_CFG_TASK_SHOT_STK_SIZE];
static  CPU_STK  AppTaskKEYBOARDStk[APP_CFG_TASK_KEYBOARD_STK_SIZE];
static  CPU_STK  AppTaskControlStk[APP_CFG_TASK_Control_STK_SIZE];
static  CPU_STK  AppTaskKalmanStk[APP_CFG_TASK_Control_STK_SIZE];
static  CPU_STK  AppTaskHeightStk[APP_CFG_TASK_Height_STK_SIZE];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
/******************函数声明********************************/
static  void  AppTaskCreate (void);
static  void  AppObjCreate  (void);
static  void  AppTaskStart  (void *p_arg);
static  void  User_EventCreate(void);

static  void   AppTaskGUI			(void *p_arg);
static  void   AppTaskSHOT			(void *p_arg);
static  void   AppTaskKEYBOARD		(void *p_arg);
static  void   AppTaskControl         (void *p_arg);
static  void   AppTaskKalman          (void *p_arg);
static  void   AppTaskHeight          (void *p_arg);

/****************动作执行函数声明**************************/
/************************************************************/
extern void Go_straight(u8 Com_Num,u8 * Input_num);
extern void  Go_back (u8 Com_Num,u8 * Input_num);
extern void Turn(u8 Com_Num,u8 * Input_num);
extern void Stamp(u8 Com_Num,u8 * Input_num);
static USART_SENTDATA(uint8_t type,uint8_t command,uint8_t data);
/************************************************************/
/******************* 定时器1回调函数 ***************************/
/******************* 电阻屏触摸查询 ***************************/
/******************* 截屏按键查询   ***************************/
static void _cbOfTmr1(OS_TMR *p_tmr, void *p_arg)
{
//	OS_ERR err;	
	(void)p_arg;
	
//	GUI_TOUCH_Exec();			//每10ms调用一次，触发调用触摸驱动
	
	Key_RefreshState(&Key2);//刷新按键状态
	if(Key_AccessTimes(&Key2,KEY_ACCESS_READ)!=0 && key_flag)//按键被按下过
	{
		//LED2_TOGGLE;
		//OSSemPost ((OS_SEM  *)&SEM_KEY2,							   //释放任务信号量
			//	   (OS_OPT   )OS_OPT_POST_ALL,
				//   (OS_ERR  *)&err);
		Key_AccessTimes(&Key2,KEY_ACCESS_WRITE_CLEAR);
	}
}
/******************* 定时器2回调务函数 ***************************/
/******************* 卡尔曼滤波***************************/
/******************* 卡尔曼滤波***************************/

static void _cbOfTmr2(OS_TMR *p_tmr, void *p_arg)
{

	u8 Angle_int;
	(void)p_arg;
 //    printf("%f",Angle);
 printf("\r\nThe angle of X %f",Angle);
//	 Angle_int=(u8)Angle;
//	 USART_SENTDATA(0x09,0x01,Angle_int);
	 if(Angle_int>30)
	 {lean_flag=0;}


}
/******************* 定时器3回调务函数 ***************************/
/******************* 超声波测距***************************/
/******************* 超声波测距***************************/
static u8 Distance;
//static u8 cross_flag=0;
static u8 ULtrasonicWave_Cal;
static u8 ULtrasonicWave_Cal1; 
static u8 ULtrasonicWave_Cal2; 	  	  
static void _cbOfTmr3(OS_TMR *p_tmr, void *p_arg)
{
	OS_ERR err;	
	int i=0;
	int _height;
//	(void)p_arg;
	UltrasonicWave_StartMeasure();
	ULtrasonicWave_Cal=(UltrasonicWave_Distance/256)*256+(UltrasonicWave_Distance%256);
	ULtrasonicWave_Cal1=(UltrasonicWave_Distance1/256)*256+(UltrasonicWave_Distance1%256);
	ULtrasonicWave_Cal2=(UltrasonicWave_Distance2/256)*256+(UltrasonicWave_Distance2%256);
    USART_SENTDATA(0x04,0x01,ULtrasonicWave_Cal);
	USART_SENTDATA(0x04,0x02,ULtrasonicWave_Cal1);
	USART_SENTDATA(0x04,0x03,ULtrasonicWave_Cal2);
	

	if(ULtrasonicWave_Cal2>38){
    if(ULtrasonicWave_Cal>40&&ULtrasonicWave_Cal1>40)
	  {
	  	 
		Distance=1;
		LED3_OFF; LED2_OFF; LED1_OFF;
		
	   }
	  
    
	else if(ULtrasonicWave_Cal<15||ULtrasonicWave_Cal1<15) 
    {
	 
		Distance=3; 
	    LED3_ON; LED2_ON; LED1_ON; }
	else  
   	    {Distance=2;
		LED3_OFF; LED2_ON; LED1_ON;
		 }
		 }
		 else if(ULtrasonicWave_Cal2<38)
		 {
		   if(ULtrasonicWave_Cal>40&&ULtrasonicWave_Cal1>40)
	  {
	  	 
		Distance=4;
		OSTimeDlyHMSM(0, 0, 2, 0,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
					  if(ULtrasonicWave_Cal>40&&ULtrasonicWave_Cal1>40&& ULtrasonicWave_Cal2<38)
					  {
	    Raise=28;
		for(;i<6;i++)
		{
		_height=2*i;
		Height_Adjust(_height);
		}
		Distance=1;
		LED3_OFF; LED2_OFF; LED1_OFF;
		}
		
	   }
	    
	else if(ULtrasonicWave_Cal<15||ULtrasonicWave_Cal1<15) 
    {
	 	if(Raise!=16)
		{
		Raise=16;
		for(i=6;i>0;i--)
		{
		_height=2*i;
			Height_Adjust(_height);
		}
		}
		Distance=3; 
	    LED3_ON; LED2_ON; LED1_ON; }
	  
	  	else  
   	    {Distance=2;
		LED3_OFF; LED2_ON; LED1_ON;
		 }
		 }
    //	Datadistance=Distance;
       //   OSQPost ((OS_Q  *)&User_Q1,	
	    //           (void   *)Datadistance,						   //释放任务信号量
		//		   (OS_MSG_SIZE)sizeof(Datadistance),
		//		   (OS_OPT   )OS_OPT_POST_LIFO,
		//		   (OS_ERR  *)&err);

 }
/*
*********************************************************************************************************
*	函 数 名: _WriteByte2File()
*	功能说明: 
*	形    参：  	
*	返 回 值: 
*********************************************************************************************************
*/
static void _WriteByte2File(uint8_t Data, void * p) 
{
	result = f_write(p, &Data, 1, &rw);
}

static USART_SENTDATA(uint8_t type,uint8_t command,uint8_t data)
{
USART_SendData(USART1,0xFF);
USART_SendData(USART1,type);
USART_SendData(USART1,command);
USART_SendData(USART1,data);
USART_SendData(USART1,0xFF);
}
/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)	  //TCM.DLL//DCM.DLL//-pCM3
{
    OS_ERR  err;

    OSInit(&err);                                               /* Init uC/OS-III.                                      */

    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 2u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    OS_ERR      err;
	//定时器变量
    OS_TMR             MyTmr;
	OS_TMR             MyTmr2;
	OS_TMR             MyTmr3;
   (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();

//    Mem_Init();                                                 /* Initialize Memory Management Module                  */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

//    CPU_IntDisMeasMaxCurReset();
    
//    APP_TRACE_INFO(("Creating Application Tasks...\n\r"));
    AppTaskCreate();                                            /* Create Application Tasks                             */
    
//   APP_TRACE_INFO(("Creating Application Events...\n\r"));
  //创建任务消息量  
	AppObjCreate(); 
															        /* Create Application Objects                           */
	User_EventCreate();                                         /* Create Application Sem                       */
   
    //创建一个按键，按键的具体细节如无兴趣可不管
	KeyCreate(&Key1,GetPinStateOfKey1);
	KeyCreate(&Key2,GetPinStateOfKey2);
   
   //创建定时器
  OSTmrCreate ((OS_TMR              *)&MyTmr,
               (CPU_CHAR            *)"MyTimer",          
               (OS_TICK              )2,                //第一次延时设置为2，结合定时器的频率是100Hz，正好20ms
               (OS_TICK              )2,                //重复延时的时候2个TmrTick，结合定时器的频率是100Hz，正好20ms
               (OS_OPT               )OS_OPT_TMR_PERIODIC,//模式设置为重复模式
               (OS_TMR_CALLBACK_PTR  )_cbOfTmr1,          //回调函数
               (void                *)0,                  //参数设置为0
               (OS_ERR              *)err);
   //创建定时器2
  OSTmrCreate ((OS_TMR              *)&MyTmr2,
               (CPU_CHAR            *)"MyTimer2",          
               (OS_TICK              )100,                //第一次延时设置为2，结合定时器的频率是100Hz，正好10ms
               (OS_TICK              )100,                //重复延时的时候2个TmrTick，结合定时器的频率是100Hz，正好10ms
               (OS_OPT               )OS_OPT_TMR_PERIODIC,//模式设置为重复模式
               (OS_TMR_CALLBACK_PTR  )_cbOfTmr2,          //回调函数
               (void                *)0,                  //参数设置为0
               (OS_ERR              *)err);
   //创建定时器3
  OSTmrCreate ((OS_TMR              *)&MyTmr3,
               (CPU_CHAR            *)"MyTimer3",          
               (OS_TICK              )50,               //第一次延时设置为2，结合定时器的频率是100Hz，正好100ms
               (OS_TICK              )50,               //重复延时的时候2个TmrTick，结合定时器的频率是100Hz，正好100ms
               (OS_OPT               )OS_OPT_TMR_PERIODIC,//模式设置为重复模式
               (OS_TMR_CALLBACK_PTR  )_cbOfTmr3,          //回调函数
               (void                *)0,                  //参数设置为0
               (OS_ERR              *)err);
	//启动定时器
	OSTmrStart((OS_TMR *)&MyTmr,(OS_ERR *)err);					 //OS_TICKS_RER_SEG
	OSTmrStart((OS_TMR *)&MyTmr2,(OS_ERR *)err);
	OSTmrStart((OS_TMR *)&MyTmr3,(OS_ERR *)err);			  
	/*Delete task*/
	OSTaskDel(&AppTaskStartTCB,&err);	
//    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
//		OSTimeDlyHMSM(0, 0, 5, 0,
//                      OS_OPT_TIME_HMSM_STRICT,
//                      &err);
//    }

}


/*
*********************************************************************************************************
*	函 数 名: AppTaskGUI
*	功能说明: 	emWin界面任务		  
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级：OS_CFG_PRIO_MAX - 6u
*********************************************************************************************************
*/

u8   condition;
u8   flag_sem=1;
u8   barrier_height=0;
void avoidance(void);
void avoidance(void)
{
//	OS_ERR      err;
//	CPU_TS   ts;
//	OS_MSG_SIZE size;
while (1) 
	{  
	   
	 //  condition=(u8)OSQPend((OS_Q  *)&User_Q1,	
	     //                    (OS_TICK )0,						   
		//		             (OS_OPT   )OS_OPT_PEND_BLOCKING,
		//					 (OS_MSG_SIZE*)&size,
		//					 (CPU_TS*)&ts,
		//		             (OS_ERR  *)&err);
	//	printf("\r\n condition:%d \r\n",condition); 
	Go_straight(0x01,&Distance);
	Turn(0x02,&Distance);
	Go_back(0x03,&Distance);
	Stamp(0x04,&Distance);
	/************************************************************
		switch(Distance)
		{
			
			case 1:  
				
			         Go_straight_auto();   
		             break;

		 
			case 2:  
			        
			         Turn_auto();    
		             break;
			
			case 3:	 
			         
					 Go_back_auto();  
			         break;

			
			default: break;
		}
		********************************************/		   
		    if(flag_sem==0)
	        {break;}
	}
 }
static  void   AppTaskGUI(void *p_arg)
{
    
	OS_ERR      err;
	CPU_TS   ts;
	(void)p_arg;		/* 避免编译器告警 */
   	while(1)
	{
    OSSemPend(&User_avoidance,0,OS_OPT_PEND_BLOCKING,&ts,&err);
	avoidance();
 	}
     //	MainAPP();
     //	MPU6050ReadID();
	

	
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskControl
*	功能说明: 	主控函数		  
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级：OS_CFG_PRIO_MAX - 7u
*********************************************************************************************************
*/
short flag_saElect=0;
static  void   AppTaskControl(void *p_arg)
{
	OS_ERR      err;
	CPU_TS   ts;
   (void)p_arg;		/* 避免编译器告警 */
  
    while(1)
	{
	  OSSemPend(&User_Sem1,0,OS_OPT_PEND_BLOCKING,&ts,&err);
	  OSTimeDlyHMSM(0, 0, 0, 10,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
	  
    

	/**********核心程序**************************/
	   if(buffercontorl[0]==0x00)																 
	{
		switch(buffercontorl[1])
		{
            case 0x00:  PWM_Init();        break;

			case 0x01:  Go_straight(0x01,&buffercontorl[1]); 	  break;
			           
			case 0x02:  Go_back(0x02,&buffercontorl[1]);      break;
		               	     
			case 0x03:  Turn_Left(0x03,&buffercontorl[1]);       break; 
			 		    
			case 0x04:  Turn(0x04,&buffercontorl[1]);         break;
					
			case 0x05:  LED3_ON;  LED2_ON;     LED1_ON;   break;

			case 0x06:  LED3_OFF;  LED2_OFF;    LED1_OFF; 		   break;

			case 0x07:  if(flag_saElect==1)
			           
					    {Stand_Up(ACTION_DELAY);  flag_saElect=0;    break;}
			            else                          break;

			case 0x08:  Sit_Down(ACTION_DELAY);  flag_saElect=1;    break;
		
			default: break;
		}	
	}
	else if(buffercontorl[0]==0x01)
	{
		switch(buffercontorl[1])
		{
		case 0x01:
		 if(buffercontorl[2]>35&&buffercontorl[2]<181)
			 TIM1->CCR1 = buffercontorl[2];	
		break; 
		case 0x02:
		 if(buffercontorl[2]>35&&buffercontorl[2]<181)
			 TIM1->CCR2 = buffercontorl[2];	
	    break; 
        case 0x03:
		 if(buffercontorl[2]>35&&buffercontorl[2]<181)
			 TIM1->CCR3 = buffercontorl[2];	
	    break; 
		case 0x04:
		 if(buffercontorl[2]>35&&buffercontorl[2]<181)
			 TIM1->CCR4 = buffercontorl[2];	
	    break;
		case 0x05:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM2->CCR3 = buffercontorl[2];	
	    break;
		case 0x06:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM2->CCR4 = buffercontorl[2];	
	    break;
		case 0x07:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM3->CCR1 = buffercontorl[2];	
	    break;
		case 0x08:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM3->CCR2 = buffercontorl[2];	
     	break;
		case 0x09:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM3->CCR3 = buffercontorl[2];	
	    break;
		case 0x0A:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM3->CCR4 = buffercontorl[2];	
    	break;	
		case 0x0B:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM4->CCR1 = buffercontorl[2];	
    	break;
		case 0x0C:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM4->CCR2 = buffercontorl[2];	
	    break;
		case 0x0D:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM4->CCR3 = buffercontorl[2];	
	    break;
		case 0x0E:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM4->CCR4 = buffercontorl[2];	
	    break;
	    case 0x0F:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM5->CCR1 = buffercontorl[2];	
     	break;
		case 0x10:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM5->CCR2 = buffercontorl[2];	
     	break;
		case 0x11:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM5->CCR3 = buffercontorl[2];	
	    break;
		case 0x12:
		 if(buffercontorl[2]>36&&buffercontorl[2]<180)
			 TIM5->CCR4 = buffercontorl[2];	
	    break;
		default:	break;
	  }
	 }
	  else if(buffercontorl[0]==0x02)
	  {
	  switch(buffercontorl[1])
		{
		case 0x01:
		 if(buffercontorl[2]>5&&buffercontorl[2]<31)
			 TURN_DELAY = buffercontorl[2];	
			 STAMP_DELAY= buffercontorl[2];
			 ALTITUDE_DELAY= buffercontorl[2];
		break; 
		case 0x02:
		 if(buffercontorl[2]>5&&buffercontorl[2]<31)
			 STRAIGHT_DELAY = buffercontorl[2];
			 ACTION_DELAY=buffercontorl[2];	
	    break; 
	    case 0x03:
		 if(buffercontorl[2]>0&&buffercontorl[2]<40)
	      Height[1]=buffercontorl[2];
		  Height_Adjust(Height[1]);
		break; 
		case 0x04:
		 if(buffercontorl[2]>=16&&buffercontorl[2]<30)
			 Raise = buffercontorl[2];
			 	
	    break; 
		default:      break;
		}
	  }
	   else if(buffercontorl[0]==0x09)
	    {switch(buffercontorl[1])
		{
		case 0x00: flag_sem=1;
			       OSSemPost ((OS_SEM  *)&User_avoidance,							   //释放任务信号量
				             (OS_OPT   )OS_OPT_POST_ALL,
				             (OS_ERR  *)&err);
	
		break; 
		case 0x01:	flag_sem=0;
		break;
		
		case 0x02:	Molest(0x02,&buffercontorl[1]);
	
		default: break;
		}
		}

	}
	
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskKalman
*	功能说明: 	倾角显示		  
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级：OS_CFG_PRIO_MAX - 5u
*********************************************************************************************************
*/
static  void   AppTaskKalman(void *p_arg)
{
	OS_ERR      err;
	CPU_TS      ts;
    (void)p_arg;		/* 避免编译器告警 */
  	while (1) 
	{
	   OSSemPend((OS_SEM   *)&User_Kalman,
				(OS_TICK   )0,                     //如果这个参数设置为0就一直等待下去
				(OS_OPT    )OS_OPT_PEND_BLOCKING,  //如果没有信号量可用就等待
				(CPU_TS   *)&ts,             //这个参数是指向存放信号量被提交、强制解除等待状态、或者信号量被删除的时间戳        
				(OS_ERR   *)&err);
  	    Angle_Calculate();
        //	MPU6050ReadAcc(Accel);			
		//	printf("\r\n加速度： %8d%8d%8d    ",Accel[0],Accel[1],Accel[2]);
		//	MPU6050ReadGyro(Gyro);
		//	printf("陀螺仪： %8d%8d%8d    ",Gyro[0],Gyro[1],Gyro[2]);
      	//	MPU6050_ReturnTemp(&Temp);
		//	printf("温度： %d",Temp);
				
	}
}
/*
*********************************************************************************************************
*	函 数 名: AppTaskHeight
*	功能说明: 	倾角显示		  
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级：OS_CFG_PRIO_MAX - 5u
*********************************************************************************************************
*/
static  void   AppTaskHeight(void *p_arg)
{
	OS_ERR      err;
	CPU_TS      ts;
    (void)p_arg;		/* 避免编译器告警 */
  	while (1) 
	{
	   OSSemPend((OS_SEM   *)&User_Height,
				(OS_TICK   )0,                     //如果这个参数设置为0就一直等待下去
				(OS_OPT    )OS_OPT_PEND_BLOCKING,  //如果没有信号量可用就等待
				(CPU_TS   *)&ts,             //这个参数是指向存放信号量被提交、强制解除等待状态、或者信号量被删除的时间戳        
				(OS_ERR   *)&err);
	
	}
}
/*
*********************************************************************************************************
*	函 数 名: AppTaskSHOT
*	功能说明: 按键截屏			  
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级：3
*********************************************************************************************************
*/
static  void   AppTaskSHOT(void *p_arg)
{

	OS_ERR      err;
	static uint8_t	Pic_Name = 0;
	CPU_TS Ts;
	char buf[20];
	(void)p_arg;
	
	while(1)
	{	
		//阻塞任务直到按键被按下！
		OSSemPend((OS_SEM   *)&SEM_KEY2,
						 (OS_TICK   )0,                     //如果这个参数设置为0就一直等待下去
						 (OS_OPT    )OS_OPT_PEND_BLOCKING,  //如果没有信号量可用就等待
						 (CPU_TS   *)&Ts,             //这个参数是指向存放信号量被提交、强制解除等待状态、或者信号量被删除的时间戳        
						 (OS_ERR   *)&err);
		sprintf(buf,"0:/picture%d.bmp",Pic_Name);
		printf("\nstart shot picture\n\r");
		result=f_unlink(buf);
		/* 创建截图 */
		result = f_open(&file,buf, FA_WRITE|FA_CREATE_ALWAYS);
		OSSchedLock(&err);
		/* 向SD卡绘制BMP图片 */
		GUI_BMP_Serialize(_WriteByte2File,&file);
		OSSchedUnlock(&err);
		/* 创建完成后关闭file */
		result = f_close(&file);
		printf("shot picture success\n\r");
		Pic_Name++;		
		OSTimeDlyHMSM(0, 0, 0, 10,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
	}
	
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskKEYBOARD
*	功能说明: 中英输入法键盘			  
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级：4
*********************************************************************************************************
*/
static  void   AppTaskKEYBOARD(void *p_arg)
{
	OS_ERR   err;
//	uint16_t *msg;
//	OS_MSG_SIZE	msg_size;
//	CPU_TS			ts;
	(void)p_arg;		/* 避免编译器告警 */	
	while (1) 
	{
	//	LED2_TOGGLE;
	
		OSTimeDlyHMSM(0, 0, 1, 0,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
//		msg = OSTaskQPend(0,
//						OS_OPT_PEND_BLOCKING,
//						&msg_size,
//						&ts,
//						&err);
//		if(err == OS_ERR_NONE)
//		{
//			printf("pend size=%d,hedit[0]=%d,hedit[1]=%d\n",msg_size,msg[0],msg[1]);
//			//keypad(msg);
//		}
    }	
}

/*
*********************************************************************************************************
*                                      CREATE APPLICATION TASKS
*									  1
* Description:  This function creates the application tasks.
*
* Arguments  :  none
*
* Returns    :  none
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
	OS_ERR      err;
		/************* GUI 任务 *********************/
	OSTaskCreate((OS_TCB       *)&AppTaskGUITCB,             
                 (CPU_CHAR     *)"App Task GUI",
                 (OS_TASK_PTR   )AppTaskGUI, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_GUI_PRIO,
                 (CPU_STK      *)&AppTaskGUIStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);			
				 
	/************** SHOT 任务 *********************/
	OSTaskCreate((OS_TCB       *)&AppTaskSHOTTCB,             
                 (CPU_CHAR     *)"App Task SHOT",
                 (OS_TASK_PTR   )AppTaskSHOT, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_SHOT_PRIO,
                 (CPU_STK      *)&AppTaskSHOTStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_SHOT_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_SHOT_STK_SIZE,
                 (OS_MSG_QTY    )2,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);	
				 
	/**************GUI刷新任务*********************/	
	OSTaskCreate((OS_TCB       *)&AppTaskKEYBOARDTCB,             
                 (CPU_CHAR     *)"App Task KEYBOARD",
                 (OS_TASK_PTR   )AppTaskKEYBOARD, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_KEYBOARD_PRIO,
                 (CPU_STK      *)&AppTaskKEYBOARDStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_KEYBOARD_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_KEYBOARD_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);	
	
	/**************主控函数*********************/	
	OSTaskCreate((OS_TCB       *)&AppTaskControlTCB,             
                 (CPU_CHAR     *)"App Task Control",
                 (OS_TASK_PTR   )AppTaskControl, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_Control_PRIO,
                 (CPU_STK      *)&AppTaskControlStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_Control_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_Control_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);	
	/**************倾角显示函数*********************/	
	OSTaskCreate((OS_TCB       *)&AppTaskKalmanTCB,             
                 (CPU_CHAR     *)"App Task Kalman",
                 (OS_TASK_PTR   )AppTaskKalman, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_Kalman_PRIO,
                 (CPU_STK      *)&AppTaskKalmanStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_Kalman_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_Kalman_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);	
    /**************高度控制函数*********************/	
	OSTaskCreate((OS_TCB       *)&AppTaskHeightTCB,             
                 (CPU_CHAR     *)"App Task Height",
                 (OS_TASK_PTR   )AppTaskHeight, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_Height_PRIO,
                 (CPU_STK      *)&AppTaskHeightStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_Height_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_Height_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);			
					
}


/*
*********************************************************************************************************
*                                      CREATE APPLICATION EVENTS
*
* Description:  This function creates the application kernel objects.
*
* Arguments  :  none
*
* Returns    :  none
*********************************************************************************************************
*/

static  void  AppObjCreate (void)
{
	OS_ERR      err;
	//创建一个信号量，按键按下的时候post一个信号量给其他任务
	OSSemCreate ((OS_SEM      *)&SEM_KEY2,     //指向信号量变量的指针
			   (CPU_CHAR    *)"SEM_KEY2",    //信号量的名字
			   (OS_SEM_CTR   )0,             //信号量这里是指示事件发生，所以赋值为0，表示事件还没有发生
			   (OS_ERR      *)&err);         //错误类型
    OSSemCreate ((OS_SEM      *)&User_Wave,     //指向信号量变量的指针
			   (CPU_CHAR    *)"User Wave",    //信号量的名字
			   (OS_SEM_CTR   )0,             //信号量这里是指示事件发生，所以赋值为0，表示事件还没有发生
			   (OS_ERR      *)&err);         //错误类型
}

static void  User_EventCreate(void)
{
   OS_ERR  err;
   OSSemCreate(&User_Sem1,"User Sem1",(OS_SEM_CTR)0,&err);					
   OSSemCreate(&User_Kalman,"User Kalman",(OS_SEM_CTR)0,&err);
   OSSemCreate(&User_avoidance,"User avoidance",(OS_SEM_CTR)0,&err);
   OSSemCreate(&User_Height,"User Height",(OS_SEM_CTR)0,&err);
   OSQCreate((OS_Q     *)&User_Q1,		 //创建一个消息队列Q1，用于存放超声波模块所探测距离
             (CPU_CHAR*)"User Q1",
			 (OS_MSG_QTY)10,
			 (OS_ERR   *)&err);
}


/*********************************************END OF FILE**********************/
/************配置USART1中断处理程序***************************/
 void USART1_IRQHandler(void)                	
{
     OS_ERR  err;
     
	 OSIntEnter();  //进入中断
//	 OS_ENTER_CRITICAL();                        //关总中断
//     OSIntNesting++;							 //中断嵌套标志
//    OS_EXIT_CRITICAL();						 //恢复全局中断标志
	 
if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==1)	
  {
		 
    USART_ClearFlag(USART1,USART_FLAG_RXNE);
    if(rec_flag==0)
    {
      if(USART_ReceiveData(USART1)==0xff)	
      {
        rec_flag=1;	
        number=0;
      }			
    }
    else
    {
      if(USART_ReceiveData(USART1)==0xff)  
      {
        rec_flag=0;	
        if(number==3)
        OSSemPost(&User_Sem1,OS_OPT_POST_ALL,&err);
        number=0;
      }
      else
      {
        buffercontorl[number]=USART_ReceiveData(USART1);   
        number++;
      }
    }		
  }
  /*
  if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==1)
  {
    USART_ClearFlag(USART1,USART_FLAG_RXNE);

        buffer[i]=USART_ReceiveData(USART1);
        i++;
        if(i>2)i=0;
    	
  }*/
  else
  {
    USART_ClearFlag(USART1,USART_FLAG_LBD);	   
  }
     
	 OSIntExit();        //触发任务切换软中断	
  
}
/***************************************************/
void TIM7_IRQHandler(void)
{
    
	OS_ERR  err;
  	OSIntEnter();  //进入中断
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
  {
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
		OSSemPost ((OS_SEM  *)&User_Kalman,							   //释放任务信号量
			      (OS_OPT   )OS_OPT_POST_ALL,
			      (OS_ERR  *)&err);
	}
	OSIntExit();        //触发任务切换软中断	
}
/***********************************************************
void EXTI0_IRQHandler(void)
{
    
  //	OSIntEnter();  //进入中断
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //确保是否产生了EXTI Line中断
	{
		cross_flag=1;
		EXTI_ClearITPendingBit(EXTI_Line0);     //清除中断标志位
	} 
	OSIntExit();        //触发任务切换软中断	 
}
*************************************************************/

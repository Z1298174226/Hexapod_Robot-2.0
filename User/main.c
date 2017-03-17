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
  * �ļ�·��  ��   User/main.c
  * ����      ��   ��������   QQ��1298174226
  * �̼���汾��   V3.5.0
  * �������ӿƼ���ѧ                          
  * ����      ��   2015-10-2
  * �ļ�����  ��   main.c
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
extern unsigned short int UltrasonicWave_Distance;      //������ľ��� 
extern unsigned short int UltrasonicWave_Distance1;      //������ľ���  
extern unsigned short int UltrasonicWave_Distance2;      //������ľ���  
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
/****����ȫ������******/
//static u8 Datadistance;

/*******************������ƿ�����***************************/
static  OS_TCB   AppTaskStartTCB;
static  OS_TCB   AppTaskGUITCB;
static  OS_TCB   AppTaskSHOTTCB;
static  OS_TCB   AppTaskControlTCB;
static  OS_TCB   AppTaskKalmanTCB;
static  OS_TCB   AppTaskHeightTCB;
OS_TCB   AppTaskKEYBOARDTCB;
/******************�ź�������********************************/
static  OS_SEM     	SEM_KEY2;
static  OS_SEM      User_Sem1;
static  OS_SEM      User_Kalman;
static  OS_SEM      User_Wave;
static  OS_SEM      User_avoidance;
static  OS_SEM      User_Height;

/******************��Ϣ��������********************************/
static  OS_Q        User_Q1;	 //������

__IO uint8_t key_flag=1;//1����������ͼ��0�����ð�����ͼ
static KEY Key1,Key2;

FIL file;									/* file objects */
FRESULT result; 
UINT rw;            						/* File R/W count */
/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/
/******************�����ջ��������********************************/
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
/******************��������********************************/
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

/****************����ִ�к�������**************************/
/************************************************************/
extern void Go_straight(u8 Com_Num,u8 * Input_num);
extern void  Go_back (u8 Com_Num,u8 * Input_num);
extern void Turn(u8 Com_Num,u8 * Input_num);
extern void Stamp(u8 Com_Num,u8 * Input_num);
static USART_SENTDATA(uint8_t type,uint8_t command,uint8_t data);
/************************************************************/
/******************* ��ʱ��1�ص����� ***************************/
/******************* ������������ѯ ***************************/
/******************* ����������ѯ   ***************************/
static void _cbOfTmr1(OS_TMR *p_tmr, void *p_arg)
{
//	OS_ERR err;	
	(void)p_arg;
	
//	GUI_TOUCH_Exec();			//ÿ10ms����һ�Σ��������ô�������
	
	Key_RefreshState(&Key2);//ˢ�°���״̬
	if(Key_AccessTimes(&Key2,KEY_ACCESS_READ)!=0 && key_flag)//���������¹�
	{
		//LED2_TOGGLE;
		//OSSemPost ((OS_SEM  *)&SEM_KEY2,							   //�ͷ������ź���
			//	   (OS_OPT   )OS_OPT_POST_ALL,
				//   (OS_ERR  *)&err);
		Key_AccessTimes(&Key2,KEY_ACCESS_WRITE_CLEAR);
	}
}
/******************* ��ʱ��2�ص����� ***************************/
/******************* �������˲�***************************/
/******************* �������˲�***************************/

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
/******************* ��ʱ��3�ص����� ***************************/
/******************* ���������***************************/
/******************* ���������***************************/
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
	    //           (void   *)Datadistance,						   //�ͷ������ź���
		//		   (OS_MSG_SIZE)sizeof(Datadistance),
		//		   (OS_OPT   )OS_OPT_POST_LIFO,
		//		   (OS_ERR  *)&err);

 }
/*
*********************************************************************************************************
*	�� �� ��: _WriteByte2File()
*	����˵��: 
*	��    �Σ�  	
*	�� �� ֵ: 
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
	//��ʱ������
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
  //����������Ϣ��  
	AppObjCreate(); 
															        /* Create Application Objects                           */
	User_EventCreate();                                         /* Create Application Sem                       */
   
    //����һ�������������ľ���ϸ��������Ȥ�ɲ���
	KeyCreate(&Key1,GetPinStateOfKey1);
	KeyCreate(&Key2,GetPinStateOfKey2);
   
   //������ʱ��
  OSTmrCreate ((OS_TMR              *)&MyTmr,
               (CPU_CHAR            *)"MyTimer",          
               (OS_TICK              )2,                //��һ����ʱ����Ϊ2����϶�ʱ����Ƶ����100Hz������20ms
               (OS_TICK              )2,                //�ظ���ʱ��ʱ��2��TmrTick����϶�ʱ����Ƶ����100Hz������20ms
               (OS_OPT               )OS_OPT_TMR_PERIODIC,//ģʽ����Ϊ�ظ�ģʽ
               (OS_TMR_CALLBACK_PTR  )_cbOfTmr1,          //�ص�����
               (void                *)0,                  //��������Ϊ0
               (OS_ERR              *)err);
   //������ʱ��2
  OSTmrCreate ((OS_TMR              *)&MyTmr2,
               (CPU_CHAR            *)"MyTimer2",          
               (OS_TICK              )100,                //��һ����ʱ����Ϊ2����϶�ʱ����Ƶ����100Hz������10ms
               (OS_TICK              )100,                //�ظ���ʱ��ʱ��2��TmrTick����϶�ʱ����Ƶ����100Hz������10ms
               (OS_OPT               )OS_OPT_TMR_PERIODIC,//ģʽ����Ϊ�ظ�ģʽ
               (OS_TMR_CALLBACK_PTR  )_cbOfTmr2,          //�ص�����
               (void                *)0,                  //��������Ϊ0
               (OS_ERR              *)err);
   //������ʱ��3
  OSTmrCreate ((OS_TMR              *)&MyTmr3,
               (CPU_CHAR            *)"MyTimer3",          
               (OS_TICK              )50,               //��һ����ʱ����Ϊ2����϶�ʱ����Ƶ����100Hz������100ms
               (OS_TICK              )50,               //�ظ���ʱ��ʱ��2��TmrTick����϶�ʱ����Ƶ����100Hz������100ms
               (OS_OPT               )OS_OPT_TMR_PERIODIC,//ģʽ����Ϊ�ظ�ģʽ
               (OS_TMR_CALLBACK_PTR  )_cbOfTmr3,          //�ص�����
               (void                *)0,                  //��������Ϊ0
               (OS_ERR              *)err);
	//������ʱ��
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
*	�� �� ��: AppTaskGUI
*	����˵��: 	emWin��������		  
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ����OS_CFG_PRIO_MAX - 6u
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
	(void)p_arg;		/* ����������澯 */
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
*	�� �� ��: AppTaskControl
*	����˵��: 	���غ���		  
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ����OS_CFG_PRIO_MAX - 7u
*********************************************************************************************************
*/
short flag_saElect=0;
static  void   AppTaskControl(void *p_arg)
{
	OS_ERR      err;
	CPU_TS   ts;
   (void)p_arg;		/* ����������澯 */
  
    while(1)
	{
	  OSSemPend(&User_Sem1,0,OS_OPT_PEND_BLOCKING,&ts,&err);
	  OSTimeDlyHMSM(0, 0, 0, 10,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
	  
    

	/**********���ĳ���**************************/
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
			       OSSemPost ((OS_SEM  *)&User_avoidance,							   //�ͷ������ź���
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
*	�� �� ��: AppTaskKalman
*	����˵��: 	�����ʾ		  
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ����OS_CFG_PRIO_MAX - 5u
*********************************************************************************************************
*/
static  void   AppTaskKalman(void *p_arg)
{
	OS_ERR      err;
	CPU_TS      ts;
    (void)p_arg;		/* ����������澯 */
  	while (1) 
	{
	   OSSemPend((OS_SEM   *)&User_Kalman,
				(OS_TICK   )0,                     //��������������Ϊ0��һֱ�ȴ���ȥ
				(OS_OPT    )OS_OPT_PEND_BLOCKING,  //���û���ź������þ͵ȴ�
				(CPU_TS   *)&ts,             //���������ָ�����ź������ύ��ǿ�ƽ���ȴ�״̬�������ź�����ɾ����ʱ���        
				(OS_ERR   *)&err);
  	    Angle_Calculate();
        //	MPU6050ReadAcc(Accel);			
		//	printf("\r\n���ٶȣ� %8d%8d%8d    ",Accel[0],Accel[1],Accel[2]);
		//	MPU6050ReadGyro(Gyro);
		//	printf("�����ǣ� %8d%8d%8d    ",Gyro[0],Gyro[1],Gyro[2]);
      	//	MPU6050_ReturnTemp(&Temp);
		//	printf("�¶ȣ� %d",Temp);
				
	}
}
/*
*********************************************************************************************************
*	�� �� ��: AppTaskHeight
*	����˵��: 	�����ʾ		  
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ����OS_CFG_PRIO_MAX - 5u
*********************************************************************************************************
*/
static  void   AppTaskHeight(void *p_arg)
{
	OS_ERR      err;
	CPU_TS      ts;
    (void)p_arg;		/* ����������澯 */
  	while (1) 
	{
	   OSSemPend((OS_SEM   *)&User_Height,
				(OS_TICK   )0,                     //��������������Ϊ0��һֱ�ȴ���ȥ
				(OS_OPT    )OS_OPT_PEND_BLOCKING,  //���û���ź������þ͵ȴ�
				(CPU_TS   *)&ts,             //���������ָ�����ź������ύ��ǿ�ƽ���ȴ�״̬�������ź�����ɾ����ʱ���        
				(OS_ERR   *)&err);
	
	}
}
/*
*********************************************************************************************************
*	�� �� ��: AppTaskSHOT
*	����˵��: ��������			  
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ����3
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
		//��������ֱ�����������£�
		OSSemPend((OS_SEM   *)&SEM_KEY2,
						 (OS_TICK   )0,                     //��������������Ϊ0��һֱ�ȴ���ȥ
						 (OS_OPT    )OS_OPT_PEND_BLOCKING,  //���û���ź������þ͵ȴ�
						 (CPU_TS   *)&Ts,             //���������ָ�����ź������ύ��ǿ�ƽ���ȴ�״̬�������ź�����ɾ����ʱ���        
						 (OS_ERR   *)&err);
		sprintf(buf,"0:/picture%d.bmp",Pic_Name);
		printf("\nstart shot picture\n\r");
		result=f_unlink(buf);
		/* ������ͼ */
		result = f_open(&file,buf, FA_WRITE|FA_CREATE_ALWAYS);
		OSSchedLock(&err);
		/* ��SD������BMPͼƬ */
		GUI_BMP_Serialize(_WriteByte2File,&file);
		OSSchedUnlock(&err);
		/* ������ɺ�ر�file */
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
*	�� �� ��: AppTaskKEYBOARD
*	����˵��: ��Ӣ���뷨����			  
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ����4
*********************************************************************************************************
*/
static  void   AppTaskKEYBOARD(void *p_arg)
{
	OS_ERR   err;
//	uint16_t *msg;
//	OS_MSG_SIZE	msg_size;
//	CPU_TS			ts;
	(void)p_arg;		/* ����������澯 */	
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
		/************* GUI ���� *********************/
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
				 
	/************** SHOT ���� *********************/
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
				 
	/**************GUIˢ������*********************/	
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
	
	/**************���غ���*********************/	
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
	/**************�����ʾ����*********************/	
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
    /**************�߶ȿ��ƺ���*********************/	
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
	//����һ���ź������������µ�ʱ��postһ���ź�������������
	OSSemCreate ((OS_SEM      *)&SEM_KEY2,     //ָ���ź���������ָ��
			   (CPU_CHAR    *)"SEM_KEY2",    //�ź���������
			   (OS_SEM_CTR   )0,             //�ź���������ָʾ�¼����������Ը�ֵΪ0����ʾ�¼���û�з���
			   (OS_ERR      *)&err);         //��������
    OSSemCreate ((OS_SEM      *)&User_Wave,     //ָ���ź���������ָ��
			   (CPU_CHAR    *)"User Wave",    //�ź���������
			   (OS_SEM_CTR   )0,             //�ź���������ָʾ�¼����������Ը�ֵΪ0����ʾ�¼���û�з���
			   (OS_ERR      *)&err);         //��������
}

static void  User_EventCreate(void)
{
   OS_ERR  err;
   OSSemCreate(&User_Sem1,"User Sem1",(OS_SEM_CTR)0,&err);					
   OSSemCreate(&User_Kalman,"User Kalman",(OS_SEM_CTR)0,&err);
   OSSemCreate(&User_avoidance,"User avoidance",(OS_SEM_CTR)0,&err);
   OSSemCreate(&User_Height,"User Height",(OS_SEM_CTR)0,&err);
   OSQCreate((OS_Q     *)&User_Q1,		 //����һ����Ϣ����Q1�����ڴ�ų�����ģ����̽�����
             (CPU_CHAR*)"User Q1",
			 (OS_MSG_QTY)10,
			 (OS_ERR   *)&err);
}


/*********************************************END OF FILE**********************/
/************����USART1�жϴ������***************************/
 void USART1_IRQHandler(void)                	
{
     OS_ERR  err;
     
	 OSIntEnter();  //�����ж�
//	 OS_ENTER_CRITICAL();                        //�����ж�
//     OSIntNesting++;							 //�ж�Ƕ�ױ�־
//    OS_EXIT_CRITICAL();						 //�ָ�ȫ���жϱ�־
	 
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
     
	 OSIntExit();        //���������л����ж�	
  
}
/***************************************************/
void TIM7_IRQHandler(void)
{
    
	OS_ERR  err;
  	OSIntEnter();  //�����ж�
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
  {
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
		OSSemPost ((OS_SEM  *)&User_Kalman,							   //�ͷ������ź���
			      (OS_OPT   )OS_OPT_POST_ALL,
			      (OS_ERR  *)&err);
	}
	OSIntExit();        //���������л����ж�	
}
/***********************************************************
void EXTI0_IRQHandler(void)
{
    
  //	OSIntEnter();  //�����ж�
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		cross_flag=1;
		EXTI_ClearITPendingBit(EXTI_Line0);     //����жϱ�־λ
	} 
	OSIntExit();        //���������л����ж�	 
}
*************************************************************/

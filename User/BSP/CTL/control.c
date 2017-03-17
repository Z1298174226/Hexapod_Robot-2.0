/******************************************************************************
  * 文件路径  ：   HardWard/control.c
  * 作者      ：   倾听枫声   QQ：1298174226
  * 固件库版本：   V3.5.0
  * 日期      ：   2015-8-2
  * 文件描述  ：   主控函数
  ******************************************************************************/
/*******************主控函数**********************/
#include <includes.h>
#include <math.h>

//default angle of all the engine 
		       

//default angle of all the engine 

u16 ACTION_SPEED = 2;		 //range:0 - 10
u16 SHRINK_SPEED 	=2;                             //((u16)((0.6)*ACTION_SPEED))
u16 ACTION_DELAY 	=30;      //ms	    //20
u16 TURN_DELAY 		=30;    //4
u16 STRAIGHT_DELAY 	=30 ;    //ms	   //3
u16 STAMP_DELAY 	=30;      //stamp delay
u16 ALTITUDE_DELAY  =15;
u16 STAND_HIGHT 	=36	;
volatile u16 Raise=16;
volatile u16 Raise1=16;
extern u16 lean_flag;
extern int Height[2];
					


int pwm_init[26] = { 0,	   //no use	 
	 			     137,  135,   125,  138,  140,		   //( 1 - 5 )
				     108,  133,   127,  90,  76,	   	   //( 6 - 10)
				     75,  89,  70,  76,  112,	           //( 11- 15)
				    76,  89,   139,  125,  90,	           //( 16- 20)
				     85,   139,   99,  108,  108,	       //( 21- 25)
				    };    		

void PWM_Init(void)
{
		TIM_SetCompare1(TIM1,pwm_init[1]);	 //1-1-1  	   
		TIM_SetCompare2(TIM1,pwm_init[2]);	 //1-2-2  	   
		TIM_SetCompare3(TIM1,pwm_init[19]);	 //1-3-3  	   
		TIM_SetCompare4(TIM1,pwm_init[4]);	 //2-1-4

		TIM_SetCompare3(TIM2,pwm_init[5]);	 //2-2-5  	   
		TIM_SetCompare4(TIM2,pwm_init[6]);	 //2-3-6
																   
		TIM_SetCompare1(TIM3,pwm_init[7]);	 //3-1-7  	   
		TIM_SetCompare2(TIM3,pwm_init[8]);	 //3-2-8  	   
		TIM_SetCompare3(TIM3,pwm_init[20]);	 //3-3-9  	   
		TIM_SetCompare4(TIM3,pwm_init[10]);	 //4-1-10

		TIM_SetCompare1(TIM4,pwm_init[11]);	 //4-2-11  	   
		TIM_SetCompare2(TIM4,pwm_init[21]);	 //4-3-12  	   
		TIM_SetCompare3(TIM4,pwm_init[13]);	 //5-1-13  	   
		TIM_SetCompare4(TIM4,pwm_init[14]);	 //5-2-14

		TIM_SetCompare1(TIM5,pwm_init[15]);	 //5-3-15  	   
		TIM_SetCompare2(TIM5,pwm_init[16]);	 //6-1-16  	   
		TIM_SetCompare3(TIM5,pwm_init[17]);	 //6-2-17  	   
		TIM_SetCompare4(TIM5,pwm_init[22]);	 //6-3-18

		TIM_SetCompare1(TIM2,pwm_init[19]);	  //yun-1-19
		TIM_SetCompare2(TIM2,pwm_init[20]);	  //yun-2-20


}

void Sit_Down(int delay_time)
{
        OS_ERR      err;
		int pwm_tmp = 0;						//motor 2
					//motor 1
		for (;pwm_tmp<=STAND_HIGHT;)				 //150 efect the height of stand
		{
//            delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒
			TIM_SetCompare2(TIM3,pwm_init[8]  + pwm_tmp);	 //3-2-8  
		    TIM_SetCompare2(TIM1,pwm_init[2]  + pwm_tmp);    //1-2-2  
			TIM_SetCompare3(TIM2,pwm_init[5]  + pwm_tmp);	 //2-2-5  		   	   

			TIM_SetCompare1(TIM4,pwm_init[11] - pwm_tmp);	 //4-2-11  
			TIM_SetCompare3(TIM5,pwm_init[17] - pwm_tmp);	 //6-2-17  	   
			TIM_SetCompare4(TIM4,pwm_init[14] - pwm_tmp);	 //5-2-14
			
			TIM_SetCompare1(TIM1,pwm_init[1] + pwm_tmp);	 //1-1-1  	   
			TIM_SetCompare4(TIM1,pwm_init[4] + pwm_tmp);	 //2-1-4
			TIM_SetCompare1(TIM3,pwm_init[7] + pwm_tmp);	 //3-1-7  
			
			TIM_SetCompare4(TIM3,pwm_init[10]- pwm_tmp);	 //4-1-10 
			TIM_SetCompare3(TIM4,pwm_init[13]- pwm_tmp);	 //5-1-13  	 
			TIM_SetCompare2(TIM5,pwm_init[16]- pwm_tmp);	 //6-1-16  	   	   
				   
			pwm_tmp += ACTION_SPEED;	
		
		}
        OSTimeDlyHMSM(0, 0, 0, 100,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒
	  //	delay_ms(100);
	   //	Close_All_TIM();		//close all timer
}

void Stand_Up(int delay_time)
{
        OS_ERR      err;
 		int pwm_tmp = 0;
	
		Open_All_TIM();			//Open all timer
		TIM_SetCompare4(TIM2,pwm_init[6]);	 //2-3-6
     	TIM_SetCompare2(TIM4,pwm_init[21]);	 //4-3-12  	   
		TIM_SetCompare4(TIM5,pwm_init[22]);	 //6-3-18
	    TIM_SetCompare3(TIM1,pwm_init[19]);	 //1-3-3  	 
		TIM_SetCompare3(TIM3,pwm_init[20]);	 //3-3-9     
	    TIM_SetCompare1(TIM5,pwm_init[15]);	 //5-3-15  	   
		for (;pwm_tmp<=STAND_HIGHT;)				 //150 efect the height of stand
		{
//			delay_ms(delay_time);	
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒
			
		
		    TIM_SetCompare2(TIM3,pwm_init[8] + STAND_HIGHT - pwm_tmp);	 //3-2-8  	   
			TIM_SetCompare2(TIM1,pwm_init[2] + STAND_HIGHT - pwm_tmp);	 //1-2-2  	   
	   	   	TIM_SetCompare3(TIM2,pwm_init[5] + STAND_HIGHT - pwm_tmp);	 //2-2-5  	

		
			TIM_SetCompare1(TIM4,pwm_init[11]  - STAND_HIGHT + pwm_tmp);	 //4-2-11  	   	
			TIM_SetCompare3(TIM5,pwm_init[17]  - STAND_HIGHT + pwm_tmp);	 //6-2-17  	    
		    TIM_SetCompare4(TIM4,pwm_init[14]  - STAND_HIGHT + pwm_tmp);	 //5-2-14
			
			TIM_SetCompare1(TIM1,pwm_init[1] + STAND_HIGHT - pwm_tmp);	 //1-1-1  	
			TIM_SetCompare4(TIM1,pwm_init[4] + STAND_HIGHT - pwm_tmp);	 //2-1-4	   
		    TIM_SetCompare1(TIM3,pwm_init[7] + STAND_HIGHT - pwm_tmp);	 //3-1-7 
			 	 
			TIM_SetCompare4(TIM3,pwm_init[10] - STAND_HIGHT + pwm_tmp);	 //4-1-10  	   
		    TIM_SetCompare3(TIM4,pwm_init[13] - STAND_HIGHT + pwm_tmp);	 //5-1-13  	   	   
	     	TIM_SetCompare2(TIM5,pwm_init[16] - STAND_HIGHT + pwm_tmp);	 //6-1-16  	  	   
			
			pwm_tmp += ACTION_SPEED;	
				
		}
}

void Pace_135_Motor1(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);		
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒						 
		TIM_SetCompare1(TIM1,pwm_init[1]  - pwm_default - pwm_tmp);	 //1-1-1  	   
		TIM_SetCompare1(TIM3,pwm_init[7]  - pwm_default - pwm_tmp);	 //3-1-7 	   
		TIM_SetCompare3(TIM4,pwm_init[13] + pwm_default + pwm_tmp);	 //5-1-13
		TIM_SetCompare2(TIM1,pwm_init[2]  + pwm_default + pwm_tmp);  //1-2-2  
	    TIM_SetCompare2(TIM3,pwm_init[8]  +  pwm_default + pwm_tmp);	 //3-2-8  
		TIM_SetCompare4(TIM4,pwm_init[14]  - pwm_default - pwm_tmp);	 //5-2-14  	   	 
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_135_Motor2(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);	
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒						 
	
	    TIM_SetCompare2(TIM1,pwm_init[2]  + pwm_default + pwm_tmp);  //1-2-2  
	    TIM_SetCompare2(TIM3,pwm_init[8]  +  pwm_default + pwm_tmp);	 //3-2-8  
		TIM_SetCompare4(TIM4,pwm_init[14]  - pwm_default - pwm_tmp);	 //5-2-14
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_135_Motor3(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒	
	
		TIM_SetCompare3(TIM1,pwm_init[19] + pwm_default + pwm_tmp);	 //1-3-3  	 
	    TIM_SetCompare3(TIM3,pwm_init[20] + pwm_default + pwm_tmp);	 //3-3-9        
     	TIM_SetCompare1(TIM5,pwm_init[15] - pwm_default - pwm_tmp);	 //5-3-15  	   
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_135_Motor3_Different(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒	
	    TIM_SetCompare3(TIM1,pwm_init[19] - pwm_default - pwm_tmp);	 //1-3-3  
	    TIM_SetCompare3(TIM3,pwm_init[20] - pwm_default - pwm_tmp);	 //3-3-9      	   
	 	TIM_SetCompare1(TIM5,pwm_init[15] - pwm_default - pwm_tmp);	 //5-3-15  	       
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_246_Motor1(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
	   	TIM_SetCompare4(TIM1,pwm_init[4] - pwm_default - pwm_tmp);	//2-1-4
	    TIM_SetCompare4(TIM3,pwm_init[10] + pwm_default + pwm_tmp);	//4-1-10  	
	    TIM_SetCompare2(TIM5,pwm_init[16] + pwm_default + pwm_tmp);	 //6-1-16  
		TIM_SetCompare1(TIM4,pwm_init[11]  - pwm_default - pwm_tmp);	 //4-2-11  
		TIM_SetCompare3(TIM5,pwm_init[17]  - pwm_default - pwm_tmp);	 //6-2-17  	
	    TIM_SetCompare3(TIM2,pwm_init[5]  + pwm_default + pwm_tmp);	 //2-2-5 	   
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_246_Motor2(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
	   	TIM_SetCompare1(TIM4,pwm_init[11]  - pwm_default - pwm_tmp);	 //4-2-11  
		TIM_SetCompare3(TIM5,pwm_init[17]  - pwm_default - pwm_tmp);	 //6-2-17  	
	    TIM_SetCompare3(TIM2,pwm_init[5]  + pwm_default + pwm_tmp);	 //2-2-5  	   
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_246_Motor3(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
		TIM_SetCompare4(TIM2,pwm_init[6]  + pwm_default + pwm_tmp);	 //2-3-6  
	    TIM_SetCompare2(TIM4,pwm_init[21] - pwm_default - pwm_tmp);	 //4-3-12  	
		TIM_SetCompare4(TIM5,pwm_init[22] - pwm_default - pwm_tmp);	 //6-3-18  
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_246_Motor3_Different(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
		TIM_SetCompare4(TIM2,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-3-6  
	    TIM_SetCompare2(TIM4,pwm_init[21] - pwm_default - pwm_tmp);	 //4-3-12  	
		TIM_SetCompare4(TIM5,pwm_init[22] - pwm_default - pwm_tmp);	 //6-3-18  
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_135_246_Motor3(int pwm_def1,int pwm_def2,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//	    delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		

	
		TIM_SetCompare3(TIM1,pwm_init[19]  - pwm_def1 - pwm_tmp);	 //1-3-3  
		TIM_SetCompare3(TIM3,pwm_init[10]  - pwm_def1 - pwm_tmp);	 //3-3-9       
		TIM_SetCompare1(TIM5,pwm_init[15] - pwm_def1 - pwm_tmp);	 //5-3-15  	        

	    TIM_SetCompare4(TIM2,pwm_init[6]  - pwm_def2 - pwm_tmp);	 //2-3-6  
		TIM_SetCompare2(TIM4,pwm_init[21] - pwm_def2 - pwm_tmp);	 //4-3-12  	
		TIM_SetCompare4(TIM5,pwm_init[22] - pwm_def2 - pwm_tmp);	 //6-3-18  
		
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_135_246_Motor3_Go_Straight(int pwm_def1,int pwm_def2,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//	    delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		

	
		TIM_SetCompare3(TIM1,pwm_init[19]  + pwm_def1 + pwm_tmp);	 //1-3-3  
		TIM_SetCompare3(TIM3,pwm_init[10]  + pwm_def1 + pwm_tmp);	 //3-3-9       
		TIM_SetCompare1(TIM5,pwm_init[15] + pwm_def1 + pwm_tmp);	 //5-3-15  	        

	    TIM_SetCompare4(TIM2,pwm_init[6]  - pwm_def2 - pwm_tmp);	 //2-3-6  
		TIM_SetCompare2(TIM4,pwm_init[21] - pwm_def2 - pwm_tmp);	 //4-3-12  	
		TIM_SetCompare4(TIM5,pwm_init[22] - pwm_def2 - pwm_tmp);	 //6-3-18  
		
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
//void Go_straight(u8 Com_Num,u8 * Input_num)
void  Go_back (u8 Com_Num,u8 * Input_num)
{
    
   
	Pace_135_Motor2(0,Raise,STRAIGHT_DELAY);		//UP
	while(Com_Num == *Input_num)
	{
		Pace_246_Motor3(0,15,STRAIGHT_DELAY);		//FRONT
	   	Pace_135_Motor2(Raise,-Raise,STRAIGHT_DELAY);	//DOWM
		Pace_246_Motor2(0,Raise,STRAIGHT_DELAY);		//UP
		Pace_246_Motor3(15,-15,STRAIGHT_DELAY);		//RETURN
		Pace_135_Motor3(0,15,STRAIGHT_DELAY);		//FRONT
		Pace_246_Motor2(Raise,-Raise,STRAIGHT_DELAY);	//DOWM
		Pace_135_Motor2(0,Raise,STRAIGHT_DELAY);		//UP
		Pace_135_Motor3(15,-15,STRAIGHT_DELAY);		//RETURN
	}
	    Pace_135_Motor2(Raise,-Raise,STRAIGHT_DELAY);		//DOWM
}
//void Go_back(u8 Com_Num,u8 * Input_num)  
extern u8 condition;
 void Go_straight(u8 Com_Num,u8 * Input_num)
{
    
   
	Pace_135_Motor2(0,Raise,ACTION_DELAY);		//UP
//	printf("\r\n condition:%d \r\n",condition); 
//	while(Com_Num == *Input_num)
     while(Com_Num == *Input_num)
	{
		Pace_246_Motor3(0,-15,ACTION_DELAY);	//BACK
	   	Pace_135_Motor2(Raise,-Raise,ACTION_DELAY);	//DOWM
		Pace_246_Motor2(0,Raise,ACTION_DELAY);	//UP
		Pace_246_Motor3(-15,15,ACTION_DELAY);	//RETURN
		Pace_135_Motor3(0,-15,ACTION_DELAY);	//BACK
		Pace_246_Motor2(Raise,-Raise,ACTION_DELAY);	//DOWM
		Pace_135_Motor2(0,Raise,ACTION_DELAY);	//UP
		Pace_135_Motor3(-15,15,ACTION_DELAY);	//RETURN
		
	}
	    Pace_135_Motor2(Raise,-Raise,ACTION_DELAY);	//DOWM
}
//void Stamp(u8 Com_Num,u8 * Input_num)
//{

void Stamp(u8 Com_Num,u8 * Input_num)
{
    
	Pace_135_Motor2(0,Raise,STAMP_DELAY);		//UP
//	while(Com_Num == *Input_num)
    while(Com_Num == *Input_num)
	{
	   	Pace_135_Motor2(Raise,-Raise,STAMP_DELAY);	//DOWM
		Pace_246_Motor2(0,Raise,STAMP_DELAY);	//UP
		Pace_246_Motor2(Raise,-Raise,STAMP_DELAY);	//DOWM
		Pace_135_Motor2(0,Raise,STAMP_DELAY);	//UP
	}
	    Pace_135_Motor2(Raise,-Raise,STAMP_DELAY);	//DOWM
}
//void Turn(u8 Com_Num,u8 * Input_num)
void Turn(u8 Com_Num,u8 * Input_num)
{
    						 
//	while(Com_Num == *Input_num)
while(Com_Num == *Input_num)
	{
		
		
		Pace_135_246_Motor3(0,0,15,TURN_DELAY);     //turn body left

		Pace_246_Motor2(0,Raise,TURN_DELAY);			  //UP
		Pace_246_Motor3_Different(15,-15,TURN_DELAY);	  //2,4,6 leg return
		Pace_246_Motor2(Raise,-Raise,TURN_DELAY);		      //DOWM
		
		Pace_135_Motor2(0,Raise,TURN_DELAY);			  //UP
		Pace_135_Motor3_Different(15,-15,TURN_DELAY);	  //1,3,5 leg return
		Pace_135_Motor2(Raise,-Raise,TURN_DELAY);		      //DOWM
		
	}

}
void Turn_Left(u8 Com_Num,u8 * Input_num)
{
    						 
//	while(Com_Num == *Input_num)
while(Com_Num == *Input_num)
	{
		
		
		Pace_135_246_Motor3(0,0,-15,TURN_DELAY);     //turn body left

		Pace_246_Motor2(0,Raise,TURN_DELAY);			  //UP
		Pace_246_Motor3_Different(-15,15,TURN_DELAY);	  //2,4,6 leg return
		Pace_246_Motor2(Raise,-Raise,TURN_DELAY);		      //DOWM
		
		Pace_135_Motor2(0,Raise,TURN_DELAY);			  //UP
		Pace_135_Motor3_Different(-15,15,TURN_DELAY);	  //1,3,5 leg return
		Pace_135_Motor2(Raise,-Raise,TURN_DELAY);		      //DOWM
		
	}

}



void Pace_123_Motor1(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);		
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒						 
		TIM_SetCompare1(TIM1,pwm_init[1]  + pwm_default + pwm_tmp);	 //1-1-1 
		TIM_SetCompare4(TIM1,pwm_init[4]  + pwm_default + pwm_tmp);	 //2-1-4 	   
		TIM_SetCompare1(TIM3,pwm_init[7]  + pwm_default + pwm_tmp);	 //3-1-7 	   
	  	 
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_123_Motor2(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);	
       OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒						 
	
	    TIM_SetCompare2(TIM1,pwm_init[2]  + pwm_default + pwm_tmp);  //1-2-2 
	    TIM_SetCompare3(TIM2,pwm_init[5]  + pwm_default + pwm_tmp);	 //2-2-5   
	    TIM_SetCompare2(TIM3,pwm_init[8]  + pwm_default + pwm_tmp);	 //3-2-8  
	
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_456_Motor2(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
	   	TIM_SetCompare1(TIM4,pwm_init[11]  - pwm_default - pwm_tmp);	 //4-2-11  
		TIM_SetCompare3(TIM5,pwm_init[17]  - pwm_default - pwm_tmp);	 //6-2-17 
		TIM_SetCompare4(TIM4,pwm_init[14]  - pwm_default - pwm_tmp);	 //5-2-14 	
	      
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}

void Pace_456_Motor1(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
		OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
		TIM_SetCompare4(TIM3,pwm_init[10]  - pwm_default - pwm_tmp);	 //4-1-10
		TIM_SetCompare3(TIM4,pwm_init[13]  - pwm_default - pwm_tmp);	 //5-1-13 
		TIM_SetCompare2(TIM5,pwm_init[16]  - pwm_default - pwm_tmp);	 //6-1-16     
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
/**********************************************************************************/





void  Go_back_auto (void)
{
        if(lean_flag!=0)
		{
        Pace_135_Motor2(0,Raise,STRAIGHT_DELAY);	//UP
   		Pace_246_Motor3(0,Raise,STRAIGHT_DELAY);		//FRONT
	   	Pace_135_Motor2(Raise,-Raise,STRAIGHT_DELAY);	//DOWM
		Pace_246_Motor2(0,Raise,STRAIGHT_DELAY);		//UP
		Pace_246_Motor3(15,-15,STRAIGHT_DELAY);		//RETURN
		Pace_135_Motor3(0,15,STRAIGHT_DELAY);		//FRONT
		Pace_246_Motor2(Raise,-Raise,STRAIGHT_DELAY);	//DOWM
		Pace_135_Motor2(0,Raise,STRAIGHT_DELAY);		//UP
		Pace_135_Motor3(15,-15,STRAIGHT_DELAY);		//RETURN
		Pace_135_Motor2(Raise,-Raise,STRAIGHT_DELAY);	//DOWM
		}
		else
		{
		Pace_135_246_Motor3_Go_Straight(0,0,15,STRAIGHT_DELAY);  //BACk
		Pace_135_Motor2(0,Raise,STRAIGHT_DELAY);	//UP
		Pace_246_Motor3(15,-15,STRAIGHT_DELAY);		//RETURN 
		Pace_135_Motor2(Raise,-Raise,STRAIGHT_DELAY);	//DOWM
		Pace_246_Motor2(0,Raise,STRAIGHT_DELAY);		//UP
		Pace_246_Motor3(15,-15,STRAIGHT_DELAY);		//RETURN 
		Pace_246_Motor2(Raise,-Raise,STRAIGHT_DELAY);	//DOWM
		}
}
 

 void Go_straight_auto(void)
{
      	
//	Pace_135_Motor2(0,16,ACTION_DELAY);		//UP

//	while(Com_Num == *Input_num)
        if(lean_flag!=0)
	   {
        Pace_135_Motor2(0,Raise,ACTION_DELAY);		//UP
		Pace_246_Motor3(0,-15,ACTION_DELAY);	//BACK
	   	Pace_135_Motor2(Raise,-Raise,ACTION_DELAY);	//DOWM
		Pace_246_Motor2(0,Raise,ACTION_DELAY);	//UP
		Pace_246_Motor3(-15,15,ACTION_DELAY);	//RETURN
		Pace_135_Motor3(0,-15,ACTION_DELAY);	//BACK
		Pace_246_Motor2(Raise,-Raise,ACTION_DELAY);	//DOWM
		Pace_135_Motor2(0,Raise,ACTION_DELAY);	//UP
		Pace_135_Motor3(-15,15,ACTION_DELAY);	//RETURN	
	    Pace_135_Motor2(Raise,-Raise,ACTION_DELAY);	//DOWM
		}
		else
		{
		Pace_135_246_Motor3_Go_Straight(0,0,-15,STRAIGHT_DELAY);  //BACk
		Pace_135_Motor2(0,Raise,STRAIGHT_DELAY);	//UP
		Pace_246_Motor3(-15,15,STRAIGHT_DELAY);		//RETURN 
		Pace_135_Motor2(Raise,-Raise,STRAIGHT_DELAY);	//DOWM
		Pace_246_Motor2(0,Raise,STRAIGHT_DELAY);		//UP
		Pace_246_Motor3(-15,15,STRAIGHT_DELAY);		//RETURN 
		Pace_246_Motor2(Raise,-Raise,STRAIGHT_DELAY);	//DOWM
		}
}

void Go_Cross_auto(void)
{
     u8 cross_num=4;	
	 Pace_135_Motor1(0,Raise1,ACTION_DELAY);		//UP
	
     while(cross_num)
	 {  
        	
		Pace_246_Motor3(0,-15,ACTION_DELAY);	//BACK
		Pace_135_Motor1(Raise1,-Raise1,ACTION_DELAY);		//DOWN
		Pace_246_Motor1(0,Raise1,ACTION_DELAY);	//UP
		Pace_246_Motor3(-15,15,ACTION_DELAY);	//RETURN
		Pace_135_Motor3(0,-15,ACTION_DELAY);	//BACK
		Pace_246_Motor1(Raise1,-Raise1,ACTION_DELAY);	//DOWN
		Pace_135_Motor1(0,Raise1,ACTION_DELAY);		//UP
		Pace_135_Motor3(-15,15,ACTION_DELAY);	//RETURN	
	    cross_num--;
	}
	Pace_135_Motor1(Raise1,-Raise1,ACTION_DELAY);		//DOWN

		
	
}

void Turn_auto(void)
{
 
    						 
//	while(Com_Num == *Input_num)		
		Pace_135_246_Motor3(0,0,15,TURN_DELAY);     //turn body left

		Pace_246_Motor2(0,Raise,TURN_DELAY);			  //UP
		Pace_246_Motor3_Different(15,-15,TURN_DELAY);	  //2,4,6 leg return
		Pace_246_Motor2(Raise,-Raise,TURN_DELAY);		      //DOWM
		
		Pace_135_Motor2(0,Raise,TURN_DELAY);			  //UP
		Pace_135_Motor3_Different(15,-15,TURN_DELAY);	  //1,3,5 leg return
		Pace_135_Motor2(Raise,-Raise,TURN_DELAY);		      //DOWM

		


} 
void Stamp_auto(void)
{
    
	
	   	Pace_135_Motor2(Raise,-Raise,STAMP_DELAY);	//DOWM
		Pace_246_Motor2(0,Raise,STAMP_DELAY);	//UP
		Pace_246_Motor2(Raise,-Raise,STAMP_DELAY);	//DOWM
		Pace_135_Motor2(0,Raise,STAMP_DELAY);	//UP

}
/***********************************************************************/
/**********************分解程序*****************************************/
/***********************************************************************/
void Pace_25_Motor1(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
	    TIM_SetCompare4(TIM1,pwm_init[4]  + pwm_default + pwm_tmp);	 //2-1-4
	    TIM_SetCompare3(TIM4,pwm_init[13] - pwm_default - pwm_tmp);	 //5-1-13  	   
		
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
  void Pace_25_Motor2(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
	    TIM_SetCompare3(TIM2,pwm_init[5]  + pwm_default + pwm_tmp);	  //2-2-5   
	    TIM_SetCompare4(TIM4,pwm_init[14] - pwm_default - pwm_tmp);	 //5-2-14
		
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_25_Motor3(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
	    TIM_SetCompare4(TIM2,pwm_init[6] + pwm_default + pwm_tmp);	 //2-3-6
	   	TIM_SetCompare1(TIM5,pwm_init[15]- pwm_default - pwm_tmp);	 //5-3-15  
		
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_14_Motor1(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
	   	TIM_SetCompare1(TIM1,pwm_init[1]- pwm_default - pwm_tmp);	 //1-1-1  	  
	    TIM_SetCompare4(TIM3,pwm_init[10] + pwm_default + pwm_tmp);	 //4-1-10 
		
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_14_Motor1_diffirent(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
	   	TIM_SetCompare1(TIM1,pwm_init[1]- pwm_default - pwm_tmp);	 //1-1-1  	  
	    TIM_SetCompare4(TIM3,pwm_init[10] - pwm_default - pwm_tmp);	 //4-1-10 
		
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_14_Motor2(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
	    TIM_SetCompare2(TIM1,pwm_init[2]  - pwm_default - pwm_tmp);	 //1-2-2  
	   	TIM_SetCompare1(TIM4,pwm_init[11] + pwm_default + pwm_tmp);	 //4-2-11  	
		
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_14_Motor3(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
	    TIM_SetCompare3(TIM1,pwm_init[19]+ pwm_default + pwm_tmp);	 //1-3-3  	   
	   	TIM_SetCompare2(TIM4,pwm_init[21] - pwm_default - pwm_tmp);	 //4-3-12  	
		
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_36_Motor1(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
	    TIM_SetCompare1(TIM3,pwm_init[7]- pwm_default - pwm_tmp);	 //3-1-7  	 
	    TIM_SetCompare2(TIM5,pwm_init[16]+ pwm_default + pwm_tmp);	 //6-1-16  	   
		
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_36_Motor2(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
	    	TIM_SetCompare2(TIM3,pwm_init[8]- pwm_default - pwm_tmp);	 //3-2-8  	   
	    	TIM_SetCompare3(TIM5,pwm_init[17]+ pwm_default + pwm_tmp);	 //6-2-17  	   
		
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
void Pace_36_Motor3(int pwm_default,int amplitude,int delay_time)
{
    OS_ERR      err;	   
	int pwm_tmp = 0;
	for(;fabs(pwm_tmp)<=fabs(amplitude);)
	{
//		delay_ms(delay_time);
        OSTimeDlyHMSM(0, 0, 0, delay_time,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//延时，参数：时，分，秒，微秒		TIM_SetCompare1(TIM5,pwm_init[6]  - pwm_default - pwm_tmp);	 //2-2-6  
	
	    TIM_SetCompare3(TIM3,pwm_init[20]- pwm_default - pwm_tmp);	 //3-3-9  	   
	    TIM_SetCompare4(TIM5,pwm_init[22]+ pwm_default + pwm_tmp);	 //6-3-18
		
		if(amplitude > 0)
			pwm_tmp += ACTION_SPEED;
		else 
			pwm_tmp -= ACTION_SPEED;
	}
}
/***********************************************************************/
/**********            攀爬 楼梯高处                          **********/
/***********************************************************************/
void Molest(u8 Com_Num,u8 * Input_num)
{ 
     
       Pace_25_Motor2(0,16,ALTITUDE_DELAY);
	   Pace_25_Motor1(0,-15,ALTITUDE_DELAY);
	   Pace_25_Motor3(0,25,ALTITUDE_DELAY);
	   Pace_25_Motor2(16,-45,ALTITUDE_DELAY);
	   Pace_14_Motor3(0,22,ALTITUDE_DELAY); 
	   Pace_14_Motor2(0,-24,ALTITUDE_DELAY);  	
    while(Com_Num == *Input_num)
	{
	  
	   Pace_14_Motor1_diffirent(0,16,ALTITUDE_DELAY);
	   Pace_14_Motor1_diffirent(16,-32,ALTITUDE_DELAY);
	   Pace_14_Motor1_diffirent(-16,16,ALTITUDE_DELAY);
	}
	   Pace_14_Motor3(22,-22,ALTITUDE_DELAY);
	   Pace_14_Motor2(-18,18,ALTITUDE_DELAY); 
	   Pace_25_Motor2(-29,45,ALTITUDE_DELAY);
	   Pace_25_Motor3(25,-25,ALTITUDE_DELAY);
	   Pace_25_Motor1(-15,15,ALTITUDE_DELAY);
	   Pace_25_Motor2(16,-16,ALTITUDE_DELAY);
	  

}
void    Height_Adjust(int Height1)
{
  int Height2;
  u8 Height_num=fabs(Height1-Height[0]);
  if(Height1>=Height[0])
  {Height2=1;
  Height[0]=Height1;}
  else
  {Height2=-1;
  Height[0]=Height[1];}
  while(Height_num!=0)
  {
        pwm_init[1]=pwm_init[1]-Height2;	 //1-1-1  	   
		pwm_init[2]=pwm_init[2]-Height2;	 //1-2-2  	   
	//	pwm_init[19];	 //1-3-3  	   
		pwm_init[4]=pwm_init[4]-Height2;	 //2-1-4

		pwm_init[5]=pwm_init[5]-Height2;	 //2-2-5  	   
	//	pwm_init[6];	 //2-3-6
																   
		pwm_init[7]=pwm_init[7]-Height2;	 //3-1-7  	   
		pwm_init[8]=pwm_init[8]-Height2;	 //3-2-8  	   
	//	pwm_init[20];	 //3-3-9  	   
		pwm_init[10]=pwm_init[10]+Height2;	 //4-1-10

		pwm_init[11]=pwm_init[11]+Height2;	 //4-2-11  	   
	//	pwm_init[21];	 //4-3-12  	   
		pwm_init[13]=pwm_init[13]+Height2; //5-1-13  	   
		pwm_init[14]=pwm_init[14]+Height2;	 //5-2-14

	//	pwm_init[15];	 //5-3-15  	   
		pwm_init[16]=pwm_init[16]+Height2;	 //6-1-16  	   
		pwm_init[17]=pwm_init[17]+Height2;	 //6-2-17  	   
	//	pwm_init[22];	 //6-3-18
	PWM_Init();
	Height_num--;
	}
}

/**********************平衡模式******************/
 /***********************************************************************/
/**********            攀爬 楼梯高处                          **********/
/***********************************************************************/

/**********************平衡模式******************/


#ifndef __CONTROL_H_
#define __CONTROL_H_
#include "stm32f10x.h"
#define PWM_MAX  		180
#define PWM_MIN  		36
#define PWM_TO_ANGLE	((PWM_MAX-PWM_MIN)/180) 
/*****************************************************
#define ACTION_SPEED 	(u16)2		 //range:0 - 10
#define SHRINK_SPEED 	(u16)2                             //((u16)((0.6)*ACTION_SPEED))
#define ACTION_DELAY 	(u16)30      //ms	    //20
#define TURN_DELAY 		(u16)30    //4
#define STRAIGHT_DELAY 	(u16)30     //ms	   //3
#define STAMP_DELAY 	(u16)30      //stamp delay
#define STAND_HIGHT 	(u16)46
#define SHRINK_DEGREE 	(u16)18                 //((u16)((0.6)*STAND_HIGHT))
#define Init_Height		(u16)16
#define Height1         (u16)0
******************************************************/



void PWM_Init(void);
void Sit_Down(int delay_time);
void Stand_Up(int delay_time);

void Pace_135_Motor1(int pwm_default,int amplitude,int delay_time);
void Pace_135_Motor2(int pwm_default,int amplitude,int delay_time);
void Pace_135_Motor3(int pwm_default,int amplitude,int delay_time);
void Pace_246_Motor1(int pwm_default,int amplitude,int delay_time);
void Pace_246_Motor2(int pwm_default,int amplitude,int delay_time);
void Pace_246_Motor3(int pwm_default,int amplitude,int delay_time);
void Pace_123_Motor1(int pwm_default,int amplitude,int delay_time);
void Pace_123_Motor2(int pwm_default,int amplitude,int delay_time);
void Pace_456_Motor2(int pwm_default,int amplitude,int delay_time);
void Pace_456_Motor1(int pwm_default,int amplitude,int delay_time);

void Pace_135_246_Motor3_Go_Straight(int pwm_def1,int pwm_def2,int amplitude,int delay_time);

void Pace_25_Motor1(int pwm_default,int amplitude,int delay_time);
void Pace_25_Motor2(int pwm_default,int amplitude,int delay_time);
void Pace_25_Motor3(int pwm_default,int amplitude,int delay_time);
void Pace_14_Motor1(int pwm_default,int amplitude,int delay_time);
void Pace_14_Motor2(int pwm_default,int amplitude,int delay_time);
void Pace_14_Motor3(int pwm_default,int amplitude,int delay_time);
void Pace_36_Motor1(int pwm_default,int amplitude,int delay_time);
void Pace_36_Motor2(int pwm_default,int amplitude,int delay_time);
void Pace_36_Motor3(int pwm_default,int amplitude,int delay_time);



void Go_straight_auto(void);
void Go_Cross_auto(void);
void Go_straighe(u8 Com_Num,u8 * Input_num);
void Go_back_auto(void);
void Stamp_auto(void);
void Go_back(u8 Com_Num,u8 * Input_num);
//void Stamp(u8 Com_Num,u8 * Input_num);
void Turn_auto(void);
void Turn_Left(u8 Com_Num,u8 * Input_num);

void Turn(u8 Com_Num,u8 * Input_num);
void Stamp(u8 Com_Num,u8 * Input_num);
void Molest(u8 Com_Num,u8 * Input_num);
void    Height_Adjust(int Height1);

#endif

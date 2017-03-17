/******************************************************************************
  * 文件路径  ：   HardWard/kalman.c
  * 作者      ：   倾听枫声   QQ：1298174226
  * 固件库版本：   V3.5.0
  * 日期      ：   2015-8-2
  * 文件描述  ：   卡尔曼滤波算法
  ******************************************************************************/
/*******************卡尔曼滤波算法**********************/

#include "kalman.h"
#include "bsp_I2C.h"
#include "mpu6050.h"
#include "TIM6.h"

 float Gyro_y;        //Y轴陀螺仪数据暂存
 float Gyro_x;        //Y轴陀螺仪数据暂存
 float Angle_gy;      //由角速度计算的倾斜角度
 float Accel_x;	     //X轴加速度值暂存
 float Angle_ax;      //由加速度计算的倾斜角度
 float Accel_y;	     //X轴加速度值暂存
 float Angle_ay;      //由加速度计算的倾斜角度
 float Angle;         //最终测量角度
 extern short Accel[3];
 extern short Gyro[3];








void Kalman_Filter(float Accel,float Gyro)		
{
		
	static const float Q_angle=0.001;  
	static const float Q_gyro=0.003;
	static const float R_angle=0.5;
	static const float dt=0.01;	                  //dt为kalman滤波器采样时间;
	static const char  C_0 = 1;
	static float Q_bias, Angle_err;
	static float PCt_0, PCt_1, E;
	static float K_0, K_1, t_0, t_1;
	static float Pdot[4] ={0,0,0,0};
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
	
	Angle+=(Gyro - Q_bias) * dt; //先验估计

	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

	Pdot[1]= -PP[1][1];
	Pdot[2]= -PP[1][1];
	Pdot[3]=Q_gyro;
	
	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - Angle;	//zk-先验估计
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	Angle	+= K_0 * Angle_err;	 //后验估计
	Q_bias	+= K_1 * Angle_err;	 //后验估计
	Gyro_y   = Gyro - Q_bias;	 //输出值(后验估计)的微分=角速度
}

void Angle_Calculate(void)
{
/*************************************************
  unsigned long now = RTC_Get();//当前时间(ms)
  float dt = (now - preTime) / 500.0;//微分时间(s)
  preTime = now;//上一次采样时间(ms)
 
  MPU6050ReadAcc(Accel);
  MPU6050ReadGyro(Gyro);
 
   Accel_y = Accel[1] * ACC_Gain;
   Accel_z = Accel[2]* ACC_Gain;
   Accel_x = Accel[0]* ACC_Gain;
   Angle_ax= atan(Accel_y/Accel_z)* (-180)/ pi;                                                                  
   	Angle_ay= atan(Accel_x/Accel_z)* (-180)/ pi;
 
  Gyro_x = Gyro[0] + Gry_offset_X;
  Gyro_y = Gyro[1] + Gry_offset_Y;
  omega_X= Gyr_Gain* Gyro_x;
  omega_Y= Gyr_Gain* Gyro_y;
  angle_dt_X = omega_X * dt;
  angle_dt_Y = omega_Y * dt;
 
  angle_X+= (Gyr_Gain * (Gyro[0] + Gry_offset_X)) * dt;
  angle_Y+= (Gyr_Gain * (Gyro[1] + Gry_offset_Y)) * dt;
 
 
  A_X= K_x/ (K_x+ dt);
  A_Y= k_Y/ (k_Y+dt);

  angle_X= A_X* (angle_X+ omega_X* dt)+ (1-A_X)* Angle_ax;
  angle_Y= A_Y* (angle_Y+ omega_Y*dt )+ (1-A_Y)* Angle_ay;
***************************************************/
  
//	static	uint8_t DataBuffer[2];	//数据缓存
/****************************加速度****************************************/
	//I2C_EE_BufferRead(DataBuffer, MPU6050_RA_ACCEL_XOUT_H, 2);
	MPU6050ReadAcc(Accel);
	Accel_x=(short)Accel[0];


 //Accel_x  = (short)((DataBuffer[0]<<8)+DataBuffer[1]);	  //读取X轴加速度
 //   MPU6050ReadAcc(Acel);
	Angle_ax = (Accel_x +220) /16384;   //去除零点偏移,计算得到角度（弧度）
	Angle_ax = Angle_ax*1.2*180/3.14;     //弧度转换为度,
   
/****************************角速度****************************************/
//	I2C_EE_BufferRead(DataBuffer, MPU6050_RA_GYRO_YOUT_H, 2);
//	Gyro_y = (short)((DataBuffer[0]<<8)+DataBuffer[1]);	      //静止时角速度Y轴输出为-18左右
    MPU6050ReadGyro(Gyro);
    Gyro_y=(short)Gyro[1];
    Gyro_y = (Gyro_y + 18)/16.4;         //去除零点偏移，计算角速度值 
//		Angle_gy = Angle_gy + Gyro_y*0.01;  //角速度积分得到倾斜角度.	
//	printf("\r\n111111： %f%f    ",Angle_ax,Gyro_y);

/***************************卡尔曼滤波+角度融合*************************************/
	
	Kalman_Filter(Angle_ax,Gyro_y);       //卡尔曼滤波计算倾角


/*******************************互补滤波******************************************/

/*补偿原理是取当前倾角和加速度获
	得倾角差值进行放大，然后与陀螺
	仪角速度叠加后再积分，从而使倾
	角最跟踪为加速度获得的角度0.5
	为放大倍数，可调节补偿度;
	0.005为系统周期5ms	
*/	
	Angle = Angle + (((Angle_ax-Angle)*0.5 + Gyro_y)*0.01);
}															  		
/*******************************************************************
 
 
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
 
#define Gry_offset_X 45
#define Gry_offset_Y 271
#define Gyr_Gain -0.00763
 
#define ACC_Gain 0.000061
#define pi 3.1415926
#define K_x 0.715
#define k_Y 1.3
 
unsigned long preTime = 0;
float angle_X = 0;
float angle_Y = 0;
 
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  accelgyro.initialize();
}
 
void loop()
{
 
  unsigned long now = millis();
  float dt = (now - preTime) / 500.0;
  preTime = now;//???????(ms)
 
 /****************************************
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
 
  float Y_Accelerometer = ay * ACC_Gain;
  float Z_Accelerometer = az * ACC_Gain;
  float X_Accelerometer = ax * ACC_Gain;
  float angleA_X= atan(Y_Accelerometer/ Z_Accelerometer)* (-180)/ pi;
                                                                  
  float angleA_Y= atan(X_Accelerometer/Z_Accelerometer)*(-180)/pi;
 
  float gx_revised = gx + Gry_offset_X;
  float gy_revised = gy + Gry_offset_Y;
  float omega_X= Gyr_Gain* gx_revised;
  float omega_Y= Gyr_Gain* gy_revised;
  float angle_dt_X = omega_X * dt;
  float angle_dt_Y = omega_Y * dt;
 
  angle_X+= (Gyr_Gain * (gx + Gry_offset_X)) * dt;
  angle_Y+= (Gyr_Gain * (gy + Gry_offset_Y)) * dt;
 
 
  float A_X= K_x/ (K_x+ dt);
 
  float A_Y= k_Y/(k_Y+dt);
  angle_X= A_X* (angle_X+ omega_X* dt)+ (1-A_X)* angleA_X;
  angle_Y= A_Y*(angle_Y+omega_Y*dt)+(1-A_Y)*angleA_Y;
 ****************************************************************/ 
    
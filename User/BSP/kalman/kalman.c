/******************************************************************************
  * �ļ�·��  ��   HardWard/kalman.c
  * ����      ��   ��������   QQ��1298174226
  * �̼���汾��   V3.5.0
  * ����      ��   2015-8-2
  * �ļ�����  ��   �������˲��㷨
  ******************************************************************************/
/*******************�������˲��㷨**********************/

#include "kalman.h"
#include "bsp_I2C.h"
#include "mpu6050.h"
#include "TIM6.h"

 float Gyro_y;        //Y�������������ݴ�
 float Gyro_x;        //Y�������������ݴ�
 float Angle_gy;      //�ɽ��ٶȼ������б�Ƕ�
 float Accel_x;	     //X����ٶ�ֵ�ݴ�
 float Angle_ax;      //�ɼ��ٶȼ������б�Ƕ�
 float Accel_y;	     //X����ٶ�ֵ�ݴ�
 float Angle_ay;      //�ɼ��ٶȼ������б�Ƕ�
 float Angle;         //���ղ����Ƕ�
 extern short Accel[3];
 extern short Gyro[3];








void Kalman_Filter(float Accel,float Gyro)		
{
		
	static const float Q_angle=0.001;  
	static const float Q_gyro=0.003;
	static const float R_angle=0.5;
	static const float dt=0.01;	                  //dtΪkalman�˲�������ʱ��;
	static const char  C_0 = 1;
	static float Q_bias, Angle_err;
	static float PCt_0, PCt_1, E;
	static float K_0, K_1, t_0, t_1;
	static float Pdot[4] ={0,0,0,0};
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
	
	Angle+=(Gyro - Q_bias) * dt; //�������

	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

	Pdot[1]= -PP[1][1];
	Pdot[2]= -PP[1][1];
	Pdot[3]=Q_gyro;
	
	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - Angle;	//zk-�������
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	Angle	+= K_0 * Angle_err;	 //�������
	Q_bias	+= K_1 * Angle_err;	 //�������
	Gyro_y   = Gyro - Q_bias;	 //���ֵ(�������)��΢��=���ٶ�
}

void Angle_Calculate(void)
{
/*************************************************
  unsigned long now = RTC_Get();//��ǰʱ��(ms)
  float dt = (now - preTime) / 500.0;//΢��ʱ��(s)
  preTime = now;//��һ�β���ʱ��(ms)
 
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
  
//	static	uint8_t DataBuffer[2];	//���ݻ���
/****************************���ٶ�****************************************/
	//I2C_EE_BufferRead(DataBuffer, MPU6050_RA_ACCEL_XOUT_H, 2);
	MPU6050ReadAcc(Accel);
	Accel_x=(short)Accel[0];


 //Accel_x  = (short)((DataBuffer[0]<<8)+DataBuffer[1]);	  //��ȡX����ٶ�
 //   MPU6050ReadAcc(Acel);
	Angle_ax = (Accel_x +220) /16384;   //ȥ�����ƫ��,����õ��Ƕȣ����ȣ�
	Angle_ax = Angle_ax*1.2*180/3.14;     //����ת��Ϊ��,
   
/****************************���ٶ�****************************************/
//	I2C_EE_BufferRead(DataBuffer, MPU6050_RA_GYRO_YOUT_H, 2);
//	Gyro_y = (short)((DataBuffer[0]<<8)+DataBuffer[1]);	      //��ֹʱ���ٶ�Y�����Ϊ-18����
    MPU6050ReadGyro(Gyro);
    Gyro_y=(short)Gyro[1];
    Gyro_y = (Gyro_y + 18)/16.4;         //ȥ�����ƫ�ƣ�������ٶ�ֵ 
//		Angle_gy = Angle_gy + Gyro_y*0.01;  //���ٶȻ��ֵõ���б�Ƕ�.	
//	printf("\r\n111111�� %f%f    ",Angle_ax,Gyro_y);

/***************************�������˲�+�Ƕ��ں�*************************************/
	
	Kalman_Filter(Angle_ax,Gyro_y);       //�������˲��������


/*******************************�����˲�******************************************/

/*����ԭ����ȡ��ǰ��Ǻͼ��ٶȻ�
	����ǲ�ֵ���зŴ�Ȼ��������
	�ǽ��ٶȵ��Ӻ��ٻ��֣��Ӷ�ʹ��
	�������Ϊ���ٶȻ�õĽǶ�0.5
	Ϊ�Ŵ������ɵ��ڲ�����;
	0.005Ϊϵͳ����5ms	
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
    
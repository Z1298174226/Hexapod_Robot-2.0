/******************************************************************************
  * �ļ�·��  ��   HardWard/mpu6050.c
  * ����      ��   ��������   QQ��1298174226
  * �̼���汾��   V3.5.0
  * ����      ��   2015-8-2
  * �ļ�����  ��   mpu6050.c
  ******************************************************************************/
/*******************mpu6050.c**********************/
#include "mpu6050.h"
#include "bsp_usart1.h"
#include "bsp_i2c.h"

void PMU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
	I2C_EE_ByteWrite(reg_dat,reg_add);
}

void PMU6050_ReadData(u8 reg_add,unsigned char* Read,u8 num)
{
	I2C_EE_BufferRead(Read,reg_add,num);
}

void MPU6050_Init(void)
{
  int i=0,j=0;
  //�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	PMU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	     //�������״̬
	PMU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //�����ǲ�����
	PMU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	
	PMU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //���ü��ٶȴ�����������16Gģʽ
	PMU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
}
void MPU6050ReadID(void)
{
	unsigned char Re = 0;
    PMU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //��������ַ
     printf("%d\r\n",Re);
}
void MPU6050ReadAcc(short *accData)
{
    u8 buf[6];
    PMU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}
void MPU6050ReadGyro(short *gyroData)
{
    u8 buf[6];
    PMU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}

void MPU6050ReadTemp(short *tempData)
{
	u8 buf[2];
    PMU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    *tempData = (buf[0] << 8) | buf[1];
}

void MPU6050_ReturnTemp(short*Temperature)
{
	short temp3;
	u8 buf[2];
	
	PMU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
  temp3= (buf[0] << 8) | buf[1];
	*Temperature=(((double) (temp3 + 13200)) / 280)-13;
}

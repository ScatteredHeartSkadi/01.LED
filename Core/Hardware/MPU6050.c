#include "MPU6050_Reg.h"
#include "main.h"
#include "stm32f1xx_hal.h"

#define MPU6050_ADDRESS		0xD0		//MPU6050的I2C从机地址
extern I2C_HandleTypeDef hi2c2;  // I2C句柄声明

/**
  * 函    数：MPU6050等待事件
  * 参    数：同I2C_CheckEvent
  * 返 回 值：无
  */ 
 HAL_StatusTypeDef MPU6050_WaitEvent(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status)
 {
	 uint32_t tickstart = HAL_GetTick();    // 获取当前时间戳
	 uint32_t timeout = 100;                 // 超时时间(ms)
	 
	 /* 等待直到标志位变为指定状态或超时 */
	 while(__HAL_I2C_GET_FLAG(hi2c, Flag) != Status)
	 {
		 // 检查是否超时
		 if((HAL_GetTick() - tickstart) > timeout)
		 {
			 return HAL_TIMEOUT;
		 }
	 }
	 return HAL_OK;
 }

/**
  * 函    数：MPU6050写寄存器
  * 参    数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
  * 参    数：Data 要写入寄存器的数据，范围：0x00~0xFF
  * 返 回 值：无
  */
 HAL_StatusTypeDef MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
 {
	 uint8_t buffer[2];
	 buffer[0] = RegAddress;  // 寄存器地址
	 buffer[1] = Data;        // 要写入的数据
	 
	 // 使用HAL库的I2C发送函数
	 return HAL_I2C_Master_Transmit(
		 &hi2c2,                 // I2C句柄
		 MPU6050_ADDRESS << 1,   // 从机地址(左移1位)
		 buffer,                 // 数据缓冲区
		 2,                      // 发送2字节
		 HAL_MAX_DELAY          // 超时时间
	 );
 }

/**
  * 函    数：MPU6050读寄存器
  * 参    数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
  * 返 回 值：读取寄存器的数据，范围：0x00~0xFF
  */
 uint8_t MPU6050_ReadReg(uint8_t RegAddress)
 {
	 uint8_t Data;
	 HAL_StatusTypeDef status;
	 
	 // 1. 先写入要读取的寄存器地址
	 status = HAL_I2C_Master_Transmit(
		 &hi2c2,
		 MPU6050_ADDRESS << 1,    // 左移1位，腾出读写位
		 &RegAddress,
		 1,
		 HAL_MAX_DELAY
	 );
	 
	 if (status != HAL_OK) {
		 Error_Handler();
		 return 0;
	 }
	 
	 // 2. 读取该寄存器的数据
	 status = HAL_I2C_Master_Receive(
		 &hi2c2,
		 MPU6050_ADDRESS << 1 | 0x01,  // 读操作，最低位置1
		 &Data,
		 1,
		 HAL_MAX_DELAY
	 );
	 
	 if (status != HAL_OK) {
		 Error_Handler();
		 return 0;
	 }
	 
	 return Data;
}

/**
  * 函    数：MPU6050初始化
  * 参    数：无
  * 返 回 值：无
  */
 void MPU6050_Init(void)
 {
	HAL_StatusTypeDef status;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* 使能时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_I2C2_CLK_ENABLE();
    
    /* GPIO配置 */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;        // 复用开漏输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;            // 无上拉下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 高速
    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11; // SCL & SDA
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* I2C配置 */
    hi2c2.Instance = I2C2;
    hi2c2.Init.ClockSpeed = 50000;                 // 50KHz
    hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c2.Init.OwnAddress1 = 0x00;
    hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	 
	 /*MPU6050寄存器初始化，需要对照MPU6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
	 MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);				//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	 MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);				//电源管理寄存器2，保持默认值0，所有轴均不待机
	 MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);				//采样率分频寄存器，配置采样率
	 MPU6050_WriteReg(MPU6050_CONFIG, 0x06);					//配置寄存器，配置DLPF
	 MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);			//陀螺仪配置寄存器，选择满量程为±2000°/s
	 MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);			//加速度计配置寄存器，选择满量程为±16g
 }


/**
  * 函    数：MPU6050获取ID号
  * 参    数：无
  * 返 回 值：MPU6050的ID号
  */
uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);		//返回WHO_AM_I寄存器的值
}

/**
  * 函    数：MPU6050获取数据
  * 参    数：AccX AccY AccZ 加速度计X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
  * 参    数：GyroX GyroY GyroZ 陀螺仪X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
  * 返 回 值：无
  */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;								//定义数据高8位和低8位的变量
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);		//读取加速度计X轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);		//读取加速度计X轴的低8位数据
	*AccX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);		//读取加速度计Y轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);		//读取加速度计Y轴的低8位数据
	*AccY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);		//读取加速度计Z轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);		//读取加速度计Z轴的低8位数据
	*AccZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);		//读取陀螺仪X轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);		//读取陀螺仪X轴的低8位数据
	*GyroX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);		//读取陀螺仪Y轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);		//读取陀螺仪Y轴的低8位数据
	*GyroY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);		//读取陀螺仪Z轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);		//读取陀螺仪Z轴的低8位数据
	*GyroZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
}

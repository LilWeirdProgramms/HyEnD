---------------------------------
Activate I2C

---------------------------------
Implement:

- Init:
constructBMI088()
BMI088_Init()

- While:
BMI088_Measure_acceleration()
BMI088_Measure_angular_rate()

----------------------------------
Example:

#include "BMI088.h"

bmi088_type bmi088_1 = constructBMI088(HAL_Delay, i2c_write, i2c_read, BMI088_ACC1_ADDRESS, BMI088_GYRO1_ADDRESS,
	  RANGE_12G, ODR_100, OS_4, RANGE_1000, ODR_100_BW_12); // Acc = 100 Hz, 12g, Rate = 400 Hz, 1000°/s
bmi088_type bmi088_2 = constructBMI088(HAL_Delay, i2c_write, i2c_read, BMI088_ACC2_ADDRESS, BMI088_GYRO2_ADDRESS,
	  RANGE_24G, ODR_400, OS_2, RANGE_2000, ODR_1000_BW_116); // Acc = 400 Hz, 24g, Rate = 1000 Hz, 2000°/s
	  
BMI088_Init(&bmi088_1);
bmi088_Init(&bmi088_2);

AngularRateData ar_data1, ar_data2;
AccelerationData acc_data1, acc_data2;

while (1)
{
	ar_data1 = BMI088_Measure_acceleration(&bmi088_1);
	acc_data1 = BMI088_Measure_angular_rate(&bmi088_1);
	ar_data2 = BMI088_Measure_acceleration(&bmi088_2);
	acc_data2 = BMI088_Measure_angular_rate(&bmi088_2);
}

void i2c_write(uint16_t devAddress, uint16_t regAddress, uint8_t *msg,
		uint16_t size) {
	i2cERROR += HAL_I2C_Mem_Write(&hi2c1, devAddress, regAddress, 1, msg, size, 10);
}

void i2c_read(uint16_t devAddress, uint16_t regAddress, uint8_t *msg,
		uint16_t size) {
	i2cERROR += HAL_I2C_Mem_Read(&hi2c1, devAddress, regAddress, 1, msg, size, 10);
}

################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Julius/Documents/Master/HyEnd/CubeWork/HyEnD_OS/peripherals/pressure/MS5607.c 

OBJS += \
./HyEnD_OS/peripherals/pressure/MS5607.o 

C_DEPS += \
./HyEnD_OS/peripherals/pressure/MS5607.d 


# Each subdirectory must supply rules for building sources it contributes
HyEnD_OS/peripherals/pressure/MS5607.o: C:/Users/Julius/Documents/Master/HyEnd/CubeWork/HyEnD_OS/peripherals/pressure/MS5607.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F423xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/Julius/Documents/Master/HyEnd/CubeWork/HyEnD_OS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HyEnD_OS/peripherals/pressure/MS5607.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"


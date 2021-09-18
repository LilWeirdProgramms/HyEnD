################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/FatFs/src/option/subdir.mk
-include Middlewares/Third_Party/FatFs/src/subdir.mk
-include HyEnD_OS/systems/test/rsms/subdir.mk
-include HyEnD_OS/systems/test/ifc/subdir.mk
-include HyEnD_OS/systems/test/gse/subdir.mk
-include HyEnD_OS/systems/test/fsms/subdir.mk
-include HyEnD_OS/systems/compass/vas/subdir.mk
-include HyEnD_OS/systems/compass/rsms/subdir.mk
-include HyEnD_OS/systems/compass/ifc/subdir.mk
-include HyEnD_OS/systems/compass/fsms/subdir.mk
-include HyEnD_OS/systems/subdir.mk
-include HyEnD_OS/peripherals/timer/subdir.mk
-include HyEnD_OS/peripherals/pressure/subdir.mk
-include HyEnD_OS/peripherals/logger/subdir.mk
-include HyEnD_OS/peripherals/ina233/subdir.mk
-include HyEnD_OS/peripherals/imu/subdir.mk
-include HyEnD_OS/peripherals/gps/subdir.mk
-include HyEnD_OS/peripherals/adc/ADS8688/subdir.mk
-include HyEnD_OS/hyendOS/canProtocolImplementation/subdir.mk
-include HyEnD_OS/hyendOS/subdir.mk
-include HyEnD_OS/drivers/utils/xprintf/subdir.mk
-include HyEnD_OS/drivers/utils/strings/subdir.mk
-include HyEnD_OS/core/stateMachine/subdir.mk
-include HyEnD_OS/core/canProtocol/subdir.mk
-include HyEnD_OS/connectivity/sd/subdir.mk
-include HyEnD_OS/connectivity/canTri/subdir.mk
-include FATFS/Target/subdir.mk
-include FATFS/App/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
IFC_F423.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
IFC_F423.list \

OBJCOPY_BIN += \
IFC_F423.bin \


# All Target
all: IFC_F423.elf secondary-outputs

# Tool invocations
IFC_F423.elf: $(OBJS) $(USER_OBJS) C:\Users\Julius\Documents\Master\HyEnd\CubeWork\IFC_F423\STM32F423RHTX_FLASH.ld
	arm-none-eabi-gcc -o "IFC_F423.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Julius\Documents\Master\HyEnd\CubeWork\IFC_F423\STM32F423RHTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="IFC_F423.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

IFC_F423.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "IFC_F423.list"
	@echo 'Finished building: $@'
	@echo ' '

IFC_F423.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "IFC_F423.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DC_Motor/DC_Motor.cpp 

OBJS += \
./DC_Motor/DC_Motor.o 

CPP_DEPS += \
./DC_Motor/DC_Motor.d 


# Each subdirectory must supply rules for building sources it contributes
DC_Motor/%.o: ../DC_Motor/%.cpp DC_Motor/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Use -I../DC_Motor -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-DC_Motor

clean-DC_Motor:
	-$(RM) ./DC_Motor/DC_Motor.d ./DC_Motor/DC_Motor.o

.PHONY: clean-DC_Motor


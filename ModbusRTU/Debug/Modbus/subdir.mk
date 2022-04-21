################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modbus/mb.c \
../Modbus/mbcrc.c \
../Modbus/mbfunccoils.c \
../Modbus/mbfuncdiag.c \
../Modbus/mbfuncdisc.c \
../Modbus/mbfuncholding.c \
../Modbus/mbfuncinput.c \
../Modbus/mbfuncother.c \
../Modbus/mbrtu.c \
../Modbus/mbutils.c \
../Modbus/portevent.c \
../Modbus/portserial.c \
../Modbus/porttimer.c 

C_DEPS += \
./Modbus/mb.d \
./Modbus/mbcrc.d \
./Modbus/mbfunccoils.d \
./Modbus/mbfuncdiag.d \
./Modbus/mbfuncdisc.d \
./Modbus/mbfuncholding.d \
./Modbus/mbfuncinput.d \
./Modbus/mbfuncother.d \
./Modbus/mbrtu.d \
./Modbus/mbutils.d \
./Modbus/portevent.d \
./Modbus/portserial.d \
./Modbus/porttimer.d 

OBJS += \
./Modbus/mb.o \
./Modbus/mbcrc.o \
./Modbus/mbfunccoils.o \
./Modbus/mbfuncdiag.o \
./Modbus/mbfuncdisc.o \
./Modbus/mbfuncholding.o \
./Modbus/mbfuncinput.o \
./Modbus/mbfuncother.o \
./Modbus/mbrtu.o \
./Modbus/mbutils.o \
./Modbus/portevent.o \
./Modbus/portserial.o \
./Modbus/porttimer.o 


# Each subdirectory must supply rules for building sources it contributes
Modbus/%.o: ../Modbus/%.c Modbus/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Modbus -I../Use -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Modbus

clean-Modbus:
	-$(RM) ./Modbus/mb.d ./Modbus/mb.o ./Modbus/mbcrc.d ./Modbus/mbcrc.o ./Modbus/mbfunccoils.d ./Modbus/mbfunccoils.o ./Modbus/mbfuncdiag.d ./Modbus/mbfuncdiag.o ./Modbus/mbfuncdisc.d ./Modbus/mbfuncdisc.o ./Modbus/mbfuncholding.d ./Modbus/mbfuncholding.o ./Modbus/mbfuncinput.d ./Modbus/mbfuncinput.o ./Modbus/mbfuncother.d ./Modbus/mbfuncother.o ./Modbus/mbrtu.d ./Modbus/mbrtu.o ./Modbus/mbutils.d ./Modbus/mbutils.o ./Modbus/portevent.d ./Modbus/portevent.o ./Modbus/portserial.d ./Modbus/portserial.o ./Modbus/porttimer.d ./Modbus/porttimer.o

.PHONY: clean-Modbus


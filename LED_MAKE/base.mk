
CC = arm-none-eabi-gcc
GCC = arm-none-eabi-g++
SZ = arm-none-eabi-size
OBJDUMP = arm-none-eabi-objdump
OBJCOPY = arm-none-eabi-objcopy

F_DEBUG = Debug
TARGET = source

STARTUP = Core\Startup\startup_stm32f103c8tx.s
LD = STM32F103C8TX_FLASH.ld

C_SRC_HAL = 

INC = \
-ICore/Inc \
-IDrivers/CMSIS/Include \
-IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
-IUse

CPP_SRCS = \
Use/use.cpp \
Use/serial.cpp

C_SRCS = \
Core/Src/main.c

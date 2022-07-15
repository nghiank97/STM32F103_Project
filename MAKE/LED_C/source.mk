
PREFIX = arm-none-eabi-
GCC 		:= $(PREFIX)gcc
GPP 		:= $(PREFIX)g++
ARM_OBJCOPY	:= $(PREFIX)objcopy
ARM_OBJDUMP := $(PREFIX)objdump
ARM_SIZE 	:= $(PREFIX)size

TARGET = LED_C

DEBUG_PATH := Debug
LINKER_PATH := STM32F103C8TX_FLASH.ld
STARTUP_PATH := Core/Startup/startup_stm32f103c8tx.s

LIB_PATH = \
-ICore/Inc \
-IDrivers/STM32F1xx_HAL_Driver/Inc \
-IDrivers/STM32F1xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
-IDrivers/CMSIS/Include \
-IUse

CORE_SOURCE = \
Core/Src/main.c \
Core/Src/stm32f1xx_hal_msp.c \
Core/Src/stm32f1xx_it.c \
Core/Src/syscalls.c \
Core/Src/sysmem.c \
Core/Src/system_stm32f1xx.c

HAL_SOURCE = \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c

USE_SOURCE = \
Use/use.c

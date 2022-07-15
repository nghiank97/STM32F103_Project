-include "source.mk"

CORTEX := cortex-m3
STD_C := gnu11
STD_CPP := gnu++14
DEVICE := STM32F103xB
LIBRARY := USE_HAL_DRIVER

CORE_FLAG = \
-mcpu=$(CORTEX) \
-std=$(STD_C) \
-g3 \
-DDEBUG \
-D$(DEVICE) \
-D$(LIBRARY) \
-c \
$(LIB_PATH) \
-O0 \
-ffunction-sections \
-fdata-sections \
-Wall \
-fstack-usage \
-MMD \
-MP \
--specs=nano.specs \
-mfloat-abi=soft \
-mthumb

HAL_FLAG = $(CORE_FLAG)

STARTUP_FLAG = \
-mcpu=$(CORTEX) \
-g3 \
-DDEBUG \
-c \
-x assembler-with-cpp  \
-MMD \
-MP \
--specs=nano.specs \
-mfloat-abi=soft \
-mthumb

USE_FLAG = $(CORE_FLAG)

LINKER_FLAG = \
-Wl,--gc-sections \
-static \
--specs=nano.specs \
-mfloat-abi=soft \
-mthumb \
-u _printf_float \
-Wl,--start-group \
-lc \
-lm \
-lstdc++ \
-lsupc++ \
-Wl,--end-group
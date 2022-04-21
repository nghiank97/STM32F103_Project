
-include "base.mk"

CFLAGS_HAL = \
-mcpu=cortex-m3 \
-std=gnu11 \
-g3 \
-DDEBUG \
-DSTM32F103xB \
-c \
$(INC) \
-O0 \
-ffunction-sections \
-fdata-sections \
-Wall \
-fstack-usage \
--specs=nano.specs \
-mfloat-abi=soft \
-mthumb \
-MMD \
-MP

DFLAGS = \
-mcpu=cortex-m3 \
-g3 \
-DDEBUG \
-c \
-x assembler-with-cpp \
--specs=nano.specs \
-mfloat-abi=soft \
-mthumb \
-MMD \
-MP \

CFLAGS = \
-mcpu=cortex-m3 \
-std=gnu11 \
-g3 \
-DDEBUG \
-DSTM32F103xB \
-c \
$(INC) \
-O0 \
-ffunction-sections \
-fdata-sections \
-Wall \
-fstack-usage \
--specs=nano.specs \
-mfloat-abi=soft \
-mthumb \
-MMD \
-MP

CPPFLAGS = \
-mcpu=cortex-m3 \
-std=gnu++14 \
-g3 \
-DDEBUG \
-DSTM32F103xB \
-c \
$(INC) \
-O0 \
-ffunction-sections \
-fdata-sections \
-fno-exceptions \
-fno-rtti \
-fno-use-cxa-atexit \
-Wall \
-fstack-usage \
--specs=nano.specs \
-mfpu=fpv4-sp-d16 \
-mfloat-abi=soft \
-mthumb \
-MMD \
-MP

ELF_FLAGS = \
-mcpu=cortex-m3 \
-T"$(LD)" \
--specs=nosys.specs \
-Wl,--gc-sections \
-static \
--specs=nano.specs \
-mfloat-abi=soft \
-mthumb \
-Wl,--start-group \
-lc \
-lm \
-lstdc++ \
-lsupc++ \
-Wl,--end-group 


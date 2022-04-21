-include flag.mk
-include base.mk

all: clean $(F_DEBUG)/$(TARGET).elf

OBJS_HAL = $(addprefix $(F_DEBUG)/,$(notdir $(C_SRC_HAL:.c=.o)))
vpath %.c $(sort $(dir $(C_SRC_HAL)))

OBJS_STARTUP = $(addprefix $(F_DEBUG)/,$(notdir $(STARTUP:.s=.o)))
vpath %.s $(sort $(dir $(STARTUP)))

OBJS_CPP = $(addprefix $(F_DEBUG)/,$(notdir $(CPP_SRCS:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SRCS)))

OBJS_CORE = $(addprefix $(F_DEBUG)/,$(notdir $(C_SRCS:.c=.o)))
vpath %.c $(sort $(dir $(C_SRCS)))

$(F_DEBUG)/%.o: Core/Src/%.c 
	$(CC) $< $(CFLAGS) -MF"$(@:%.o=%.d)" -MT"$@" -o $@

$(F_DEBUG)/%.o: Use/%.cpp
	$(CC) $< $(CPPFLAGS) -MF"$(@:%.o=%.d)" -MT"$@" -o $@

$(F_DEBUG)/%.o: Drivers/STM32F1xx_HAL_Driver/Src/%.c 
	$(CC) $< $(CFLAGS_HAL) -MF"$(@:%.o=%.d)" -MT"$@" -o $@

$(F_DEBUG)/%.o: Core/Startup/%.s 
	$(CC) $(DFLAGS) $< -MF"$(@:%.o=%.d)" -MT"$@" -o $@

$(F_DEBUG)/$(TARGET).elf: $(OBJS_HAL) $(OBJS_STARTUP) $(OBJS_CPP) $(OBJS_CORE)
	$(GCC) -o $@ Debug/*.o $(ELF_FLAGS) -Wl,-Map="$(@:%.elf=%.map)"
	$(SZ) $(F_DEBUG)/$(TARGET).elf
	$(OBJDUMP) -h -S  $(F_DEBUG)/$(TARGET).elf > $(F_DEBUG)/$(TARGET).list
	$(OBJCOPY) -O binary  $(F_DEBUG)/$(TARGET).elf  $(F_DEBUG)/$(TARGET).bin

clean:
	del -f D:\ViDieuKien\STM32\STM32F103\LED_MAKE\Debug\*
	@echo "-------- deleted --------"

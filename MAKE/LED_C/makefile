-include source.mk
-include flag.mk

all: clean $(DEBUG_PATH)/$(TARGET).elf

OBJ_HAL = $(addprefix $(DEBUG_PATH)/,$(notdir $(HAL_SOURCE:.c=.o)))
vpath %.c $(sort $(dir $(HAL_SOURCE)))

OBJ_CORE = $(addprefix $(DEBUG_PATH)/,$(notdir $(CORE_SOURCE:.c=.o)))
vpath %.c $(sort $(dir $(CORE_SOURCE)))

OBJ_STARTUP = $(addprefix $(DEBUG_PATH)/, $(notdir $(STARTUP_PATH:.s=.o)))
vpath %.s $(sort $(dir $(STARTUP_PATH)))

OBJ_USE = $(addprefix $(DEBUG_PATH)/, $(notdir $(USE_SOURCE:.c=.o)))
vpath %.c $(sort $(dir $(USE_SOURCE)))

$(DEBUG_PATH)/%.o: Core/Src/%.c
	${GCC} $< $(CORE_FLAG) -MF"$(@:%.o=%.d)" -MT"$@" -o $@

$(DEBUG_PATH)/%.o: Drivers/STM32F1xx_HAL_Driver/Src/%.c
	${GCC} $< $(HAL_FLAG) -MF"$(@:%.o=%.d)" -MT"$@" -o $@

$(DEBUG_PATH)/%.o: Core/Startup/%.s
	${GCC} $(STARTUP_FLAG) -MF"$(@:%.o=%.d)" -MT"$@" -o $@

$(DEBUG_PATH)/%.o: Use/%.c
	${GPP} $< $(USE_FLAG) -MF"$(@:%.o=%.d)" -MT"$@" -o $@

$(DEBUG_PATH)/$(TARGET).elf: $(OBJ_HAL) $(OBJ_CORE) $(OBJ_USE)
	@echo "Create elf ..............................."

	$(GPP) -o "$@" $^ -mcpu=$(CORTEX) -T"$(LINKER_PATH)" --specs=nosys.specs -Wl,-Map="$(DEBUG_PATH)/$(TARGET).map" $(LINKER_FLAG) 
	@echo "....................................."
	@echo "Finished building target: LED.elf"

	$(ARM_SIZE) $(DEBUG_PATH)/$(TARGET).elf
	$(ARM_OBJCOPY) -O ihex $(DEBUG_PATH)/$(TARGET).elf $(DEBUG_PATH)/$(TARGET).hex
	$(ARM_SIZE) $(DEBUG_PATH)/$(TARGET).hex
	$(ARM_OBJDUMP) -h -S  $(DEBUG_PATH)/$(TARGET).elf  > $(DEBUG_PATH)/$(TARGET).list
	$(ARM_OBJCOPY) -O binary  $(DEBUG_PATH)/$(TARGET).elf  > $(DEBUG_PATH)/$(TARGET).bin

-include flash.mk

clean:
	@echo "clean..."
	del -f $(DEBUG_PATH)\*
	@echo "finish clean..."
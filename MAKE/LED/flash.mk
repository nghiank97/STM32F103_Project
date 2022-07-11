
-include "source"

FIRMWARE = $(DEBUG_PATH)/$(TARGET).elf
FIRMWARE_HEX = $(DEBUG_PATH)/$(TARGET).hex
OPENOCD := openocd -f interface/stlink.cfg \
		$(if $(value PROGRAMMER),-c 'hla_serial $(PROGRAMMER)',) \
		-f target/stm32f1x.cfg

erase:
	$(OPENOCD) -c init \
	-c reset \
	halt -c flash \
	erase_address 0 \
	-c reset run \
	-c exit

flash:
	$(OPENOCD) -c init \
		-c 'reset halt' \
		-c 'program $(DEBUG_PATH)/$(TARGET).elf 0x08000000 verify' \
		-c 'reset run' \
		-c exit

# flash:
# 	openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c "init" \
# 	-c "halt" \
# 	-c "wait_halt" \
# 	-c "stm32f1x mass_erase 0" \
# 	-c "sleep 200" \
# 	-c "flash write_bank 0 $(FIRMWARE) 0" \
# 	-c "reset run" \
# 	-c "exit"

# flash:
# 	openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c init \
# 		-c 'reset halt' \
# 		-c 'program $(DEBUG_PATH)/$(TARGET).elf 0x08000000 verify reset exit' \
# 		-c 'reset run' \
# 		-c exit

erase:
	$(OPENOCD) -c init -c reset\ halt -c flash\ erase_address\ 0x8000000\ 0x100000 -c reset\ run -c exit
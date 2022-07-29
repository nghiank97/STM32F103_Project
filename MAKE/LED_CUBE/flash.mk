
-include "source"

FIRMWARE = $(DEBUG_PATH)/$(TARGET).elf
FIRMWARE_HEX = $(DEBUG_PATH)/$(TARGET).hex

ST_LINK_PATH := "C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-Link_CLI.exe"

push:
	$(ST_LINK_PATH) -c port=SWD freq=4000 -w "$(FIRMWARE_HEX)" 0x08000000 -v
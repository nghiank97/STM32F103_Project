
#ifndef CRC_H
#define CRC_H
#include "main.h"
#include <inttypes.h>

uint16_t crc16(uint8_t buf[], uint8_t len);
uint32_t crc32(uint8_t *buf, uint16_t size);

#endif

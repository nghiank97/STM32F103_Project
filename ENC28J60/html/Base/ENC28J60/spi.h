#ifndef SPI_H
#define SPI_H
#include "binary.h"
#include "main.h"

void ENC28J60_CSL(void);
void ENC28J60_CSH(void);

void ENC28J60_RST_L(void);
void ENC28J60_RST_H(void);
u08 SPI1_ReadWrite(uint8_t data);
u16 SPI1_ReadBuff(u08* data, u16 len);
u16 SPI1_WriteBuff(u08* data, u16 len);
#endif

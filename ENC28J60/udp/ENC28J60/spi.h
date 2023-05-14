#ifndef SPI_H
#define SPI_H
#include "main.h"

void ENC28J60_CSL(void);
void ENC28J60_CSH(void);

void ENC28J60_RST_L(void);
void ENC28J60_RST_H(void);
u08 SPI1_ReadWrite(uint8_t data);

uint16_t SPI1_ReadPage(u08* data, u16 len);
uint16_t SPI1_WritePage(u08* data, u16 len);

#endif


/*
 * File   : spi.h
 * Author : hong xiem
 * Date   : 12-Step, 2024
 * */


#ifndef _SPI_H
#define _SPI_H

#include "binary.h"
#include "main.h"

#define size_w       	128
#define size_h       	160

#ifndef delay_ms
#define delay_ms(t)		HAL_Delay(t)
#endif

void spi1_cs_low(void);
void spi1_cs_high(void);

void spi1_init(void);
void spi1_write(u08 tx);
void spi1_writes(u08 *tx, u16 len);
u08 spi1_read(void);
void spi1_reads(u08 *rx, u16 len);
u08 spi1_transfer(u08 tx);
void spi1_transfers(u08 *tx, u08 *rx, u16 len);

#endif

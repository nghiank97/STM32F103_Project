#include "binary.h"
#include "spi.h"
#include "main.h"

#define SS		4
#define RST		3

void ENC28J60_CSL(void){
	GPIOA->ODR &=~ (1<<SS);
}

void ENC28J60_CSH(void){
	GPIOA->ODR |= (1<<SS);
}

void ENC28J60_RST_L(void){
	GPIOA->ODR &=~ (1<<RST);
}

void ENC28J60_RST_H(void){
	GPIOA->ODR |= (1<<RST);
}

u08 SPI1_ReadWrite(u08 data){
	u08 c = 0;
	HAL_SPI_TransmitReceive(&hspi1, &data, &c, 1, 1000);
	return c;
}

u16 SPI1_Writes(u08* data, u16 len){
	HAL_SPI_Transmit(&hspi1, data, len, 1000);
	return len;
}

u16 SPI1_Reads(u08* data, u16 len){
	HAL_SPI_Receive(&hspi1, data, len, 1000);
	return len;
}

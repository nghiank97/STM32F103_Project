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
	uint8_t c = 0;
	HAL_SPI_TransmitReceive(&hspi1, &data, &c, 1, 100);
	return c;
}

uint16_t SPI1_ReadPage(u08* data, u16 len){
	HAL_SPI_Receive(&hspi1, data, len, 100);
	return len;
}

uint16_t SPI1_WritePage(u08* data, u16 len){
	HAL_SPI_Transmit(&hspi1, data, len, 100);
	return len;
}

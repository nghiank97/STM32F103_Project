
#include "spi.h"

void spi1_cs_low(void){
	GPIOA->ODR &=~ (1 << 4);
}
void spi1_cs_high(void){
	GPIOA->ODR |= (1 << 4);
}
void spi1_init(void){
	// Enable PA4 CS CLock
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRL &=~ 0x000F0000;
	GPIOA->CRL |=  0x00010000;

	// Enable GPIOA
	RCC->APB2ENR |= (1<<2);
	// MOSI - PA7; MISO - PA6; SCK - PA5
	GPIOA->CRL &=~ 0xFFF00000;
	GPIOA->CRL |=  0x99900000;

	// Enable SPI1 CLock
	RCC->APB2ENR |= (1<<12);
	// SPI Config
	SPI1->CR1 &=~ (1<<1);     		// CPOL=0
	SPI1->CR1 |= (1<<0);     		// CPHA=1

	SPI1->CR1 |= (1<<2);            // Master Mode
	SPI1->CR1 |= (B011<<3);       	// BR[2:0] = 011: fPCLK/16, PCLK2 = 72MHz, SPI clk = 4.5 MHz
	SPI1->CR1 &= ~(1<<7);           // MSB first
	SPI1->CR1 |= (1<<8) | (1<<9);   // SSM=1, SSi=1 -> Software Slave Management
	SPI1->CR1 &= ~(1<<10);          // RXONLY = 0, full-duplex
	SPI1->CR1 &= ~(1<<11);          // DFF=0, 8 bit data
	SPI1->CR2 = 0;
	SPI1->CR1 |= (1<<6);            // SPI1 enabled
}
void spi1_write(u08 tx){
	SPI1->SR = 0;
	SPI1->DR = tx;
	while (!(SPI1->SR&(1<<1))) {}	// Transmit buffer not empty
	SPI1->SR = 0;
	SPI1->DR = 0;
}
void spi1_writes(u08 *tx, u16 len){
	for(int i=0;i<len;i++){
		spi1_write(tx[i]);
	}
}
u08 spi1_read(void){
	u08 rx = 0;
	while (!(SPI1->SR&(1<<0))) {}	// Receive buffer not empty
	rx = SPI1->DR;
	return rx;
}

void spi1_reads(u08 *rx, u16 len){

}

u08 spi1_transfer(u08 tx){
	u08 rx;
	SPI1->DR = tx;
	while (!(SPI1->SR&(1<<0))) {}	// Receive buffer empty
	rx = SPI1->DR;
	return rx;
}

void spi1_transfers(u08 *tx, u08 *rx, u16 len){

}


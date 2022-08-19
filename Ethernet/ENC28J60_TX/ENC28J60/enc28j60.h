
#ifndef ENC28J60_H
#define ENC28J60_H
#include "main.h"


/* ECON 1: Ethernet controler register 1 */
#define ECON1 	0x1F
#define TXRST		7
#define RXRST		6
#define DMAST		5
#define CSUMEN	4
#define TXRTS		3
#define RXEN		2
#define BSEL1		1
#define BSEL0		0

/* ESTAT : Ethernet status register */
#define ESTAT		0x1D
#define INT 		7
#define LATECOL 4
#define RXBUSY 	2
#define TXABRT 	1
#define CLKRDY 	0

/* Opcode */
#define RCR 		0x00
#define RBM			0x20	
#define WCR			0x40
#define WBM			0x60
#define BFS			0x80
#define BFC			0xA0
#define SRC			0xE0		

typedef enum{
	BANK_NONE =-1,
	BANK_0 =0x00,
	BANK_1 =0x01,
	BANK_2 =0x02,
	BANK_3 =0x03
}BANK;

#define ERDPTL 		0x00
#define ERDPTH 		0x01

#define EWRPTL 		0x02
#define EWRPTH 		0x03

#define ETXSTL 		0x04
#define ETXSTH 		0x05

#define ETXNDL 		0x06
#define ETXNDH 		0x07

#define ERXSTL 		0x08
#define ERXSTH 		0x09

#define ERXNDL 		0x0A
#define ERXNDH 		0x0B

#define ERXRDPTL 	0x0C
#define ERXRDPTH 	0x0D

#define ERXWRPTL 	0x0E
#define ERXWRPTH 	0x0F  


#define TX_START_ADD 		0x0000
#define TX_STOP_ADD			0x0BFF

#define RX_START_ADD 		0x0C00   
#define RX_STOP_ADD			0x11FF

#define DEBUG

extern void enc28j60_init(void);
extern void enc28j80_set_bank(BANK bank);

extern void enc28j60_write_cmd(uint8_t opcode, uint8_t address, uint8_t data);
extern uint8_t enc28j60_read_cmd(uint8_t opcode, uint8_t address);

extern void enc28j60_poll(void);

#endif

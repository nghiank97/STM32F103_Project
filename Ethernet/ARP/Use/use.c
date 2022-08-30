
#include "use.h"
#include "enc28j60.h"

/* Destination MAC */
/* Source MAC */
/* Ethertype : ARP */
/*--------------  ARP ------------*/  
/* 
Hardware Type : HTYPE
		Hard Addr.Length
		Prot.Addr.Type
Ethernet is 1
*/
/* 
Protocol type (PTYPE)
		Hard Addr.Length
		Prot.Addr.Type
For IPv4, this has the value 0x0800
*/
/* Hardware length (HLEN) */
/* Protocol length (PLEN) */
/* 
	Operation
		1 for request
		2 for reply
*/
/* Sender hardware address (SHA) */
/* Sender protocol address (SPA) */
/* Target hardware address (THA) */
/* Target protocol address (TPA) */
/* Crc */

uint8_t arp[62]={
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0x08, 0x10, 0x19, 0x97, 0x25, 0x25,
	0x08, 0x06,
	0x00, 0x01,
	0x08, 0x00,
	0x06, 0x04,
	0x00, 0x01,
	0x08, 0x10, 0x19, 0x97, 0x25, 0x25,
	192,168,137,100,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	192,168,137,10,
	
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,
};

uint8_t MAC[]= {0x08, 0x10, 0x19, 0x97, 0x25, 0x25};
extern void setup(void){
	enc28j60_init(MAC);
	
	// Enable clock GPIOC 
	RCC->APB2ENR |= (1<<4);
	
	GPIOC->CRH &=~ 0x00F00000;
	GPIOC->CRH |= 0x00100000;
}

#define LED_ON() 			{GPIOC->ODR &=~ (1<<13);}
#define LED_OFF() 		{GPIOC->ODR |= (1<<13);}
#define LED_TOGGLE() 	{GPIOC->ODR ^= (1<<13);}

extern void loop(void){
	enc28j60_send_packet(arp,42);
	LED_ON();
	HAL_Delay(1000);
	LED_OFF();
	enc28j60_receive_packet();
}

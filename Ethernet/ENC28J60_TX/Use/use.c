
#include "use.h"
#include "enc28j60.h"

uint8_t MAC[]= {0x08, 0x10, 0x19, 0x97, 0x25, 0x25};
uint8_t data_packet[58]={
	/* Destination MAC */
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	/* Source MAC */
	0x08, 0x10, 0x19, 0x97, 0x25, 0x25,
	/* Ethertype : ARP */
	0x08, 0x06,

	/*--------------  ARP ------------*/  
	/* 
	Hardware Type : HTYPE
			Hard Addr.Length
			Prot.Addr.Type
	Ethernet is 1
	*/
	0x00,
	0x01,

	/* 
	Protocol type (PTYPE)
			Hard Addr.Length
			Prot.Addr.Type
	For IPv4, this has the value 0x0800
	*/
	0x08, 0x06,
	
	
	/* Hardware length (HLEN) */
	0x06,
	/* Protocol length (PLEN) */
	0x04,
	/* 
		Operation
			1 for request
			2 for reply
	*/
	0x00, 0x01,
	
	/* Sender hardware address (SHA) */
	0x08, 0x10, 0x19, 0x97, 0x25, 0x25,
	/* Sender protocol address (SPA) */
	192,168,137,100,
	/* Target hardware address (THA) */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/* Target protocol address (TPA) */
	192,168,137,10,
	
	/* Padding */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	
	0x00,0x00,0x00,0x00,0x00,0x00
};

extern void setup(void){
	enc28j60_init(MAC);
}

extern void loop(void){
	enc28j80_send_packet(data_packet, 58);
	//enc28j60_poll();
	HAL_Delay(500);
}

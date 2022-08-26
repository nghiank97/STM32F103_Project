#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <inttypes.h>
#include "enc28j60.h"

#define OFFSET	6

typedef struct{
	uint16_t _Ethernet;
	
	uint16_t _HTYPE;
	uint16_t _PTYPE;
	
	uint8_t _HLEN;
	uint8_t _PLEN;
	
	uint16_t _OPER;
	
	uint8_t _SHA[6];
	uint8_t _SPA[4];
	
	uint8_t _THA[6];
	uint8_t _TPA[4];
	
	uint8_t _CRC[4];
	
	uint8_t _DES_MAC[6];
	uint8_t _SOU_MAC[6];
}ARP_Frame;

extern ARP_Frame arp_frame;

extern bool arp_get(uint8_t* data, uint16_t len);

#endif

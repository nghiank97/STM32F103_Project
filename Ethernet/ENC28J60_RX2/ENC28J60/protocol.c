#include <stdio.h>
#include <stdbool.h>
#include "protocol.h"
#include "crc32.h"

ARP_Frame arp_frame;

bool arp_check_crc(uint8_t* data, uint16_t len){
	uint32_t _crc32 = crc32(&(data[6]), 60);
	uint32_t real_crc32 = data[66] + (data[67]<<8) + (data[68]<<16) + (data[69]<<24);
	if (_crc32 == real_crc32){
		return true;
	}
	else{
		return false;
	}
}

extern bool arp_get(uint8_t* data, uint16_t len){
	if (arp_check_crc(data, len) == true){
		arp_frame._DES_MAC[0] = data[OFFSET];
		arp_frame._DES_MAC[1] = data[OFFSET+1];
		arp_frame._DES_MAC[2] = data[OFFSET+2];
		arp_frame._DES_MAC[3] = data[OFFSET+3];
		arp_frame._DES_MAC[4] = data[OFFSET+4];
		arp_frame._DES_MAC[5] = data[OFFSET+5];
		
		arp_frame._SOU_MAC[0] = data[OFFSET+6];
		arp_frame._SOU_MAC[1] = data[OFFSET+7];
		arp_frame._SOU_MAC[2] = data[OFFSET+8];
		arp_frame._SOU_MAC[3] = data[OFFSET+9];
		arp_frame._SOU_MAC[4] = data[OFFSET+10];
		arp_frame._SOU_MAC[5] = data[OFFSET+11];
		
		arp_frame._Ethernet = data[OFFSET+12]+ (data[OFFSET+13]<<8);
		arp_frame._HTYPE = data[OFFSET+14]+ (data[OFFSET+15]<<8);
		arp_frame._PTYPE = data[OFFSET+16]+ (data[OFFSET+17]<<8);
		arp_frame._HLEN = data[OFFSET+18];
		arp_frame._PLEN = data[OFFSET+19];
		arp_frame._OPER = data[OFFSET+20]+ (data[OFFSET+21]<<8);
		
		arp_frame._SHA[0] = data[OFFSET+22];
		arp_frame._SHA[1] = data[OFFSET+23];
		arp_frame._SHA[2] = data[OFFSET+24];
		arp_frame._SHA[3] = data[OFFSET+25];
		arp_frame._SHA[4] = data[OFFSET+26];
		arp_frame._SHA[5] = data[OFFSET+27];
		
		arp_frame._SPA[0] = data[OFFSET+28];
		arp_frame._SPA[1] = data[OFFSET+29];
		arp_frame._SPA[2] = data[OFFSET+30];
		arp_frame._SPA[3] = data[OFFSET+31];
		
		arp_frame._THA[0] = data[OFFSET+32];
		arp_frame._THA[1] = data[OFFSET+33];
		arp_frame._THA[2] = data[OFFSET+34];
		arp_frame._THA[3] = data[OFFSET+35];
		arp_frame._THA[4] = data[OFFSET+36];
		arp_frame._THA[5] = data[OFFSET+37];
		
		arp_frame._TPA[0] = data[OFFSET+38];
		arp_frame._TPA[1] = data[OFFSET+39];
		arp_frame._TPA[2] = data[OFFSET+40];
		arp_frame._TPA[3] = data[OFFSET+41];
		
		arp_frame._CRC[0] = data[OFFSET+60];
		arp_frame._CRC[1] = data[OFFSET+61];
		arp_frame._CRC[2] = data[OFFSET+62];
		arp_frame._CRC[3] = data[OFFSET+63];
		
		printf("TPA: %02x %02x %02x %02x \r\n",arp_frame._TPA[0],arp_frame._TPA[1],arp_frame._TPA[2],arp_frame._TPA[3]);
		return true;
	}
	return false;
}

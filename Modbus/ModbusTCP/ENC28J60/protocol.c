#include <stdio.h>
#include <stdbool.h>
#include "protocol.h"
#include "crc.h"

#define IP_VERSION 	0x04
#define IP_IHL			0x05
#define IP_DSCP			0x00
#define IP_ECN			0x00

#define IP_TOAL_LENGTH 		2
#define IP_IDENTIFICATION	4
#define IP_FLAGS					6

uint8_t ip_ip[4] = {192,168,137,25};
uint16_t ip_total_length = 0;
uint16_t ip_identification = 0;

#define 




extern void mb_tcp_ip_init(uint8_t* ip){
	static uint8_t _mb_tcpip_lookback[4] = {0x02, 0x00, 0x00, 0x00};
	static uint8_t _ip_frame[20] = {
		/* VERSION and IHL */
		IP_VERSION|IP_IHL,
		/* DSCP */
		IP_DSCP,
		/* ECN */
		IP_ECN,
		/* Total length */
		0x00,
		0x00,
		/* Identification */
		0x00,
		0x00,
		/* Flags */
		0x00,
		0x00,
		/* Time to live */
		127,
		/* Protocol : TCP*/
		0x06,
		/* check sum */
		0x00, 0x00
	};

	static uint8_t _tcp_frame[20] = {
		/* VERSION and IHL */
		
		/* DSCP */
		IP_DSCP,
		/* ECN */
		IP_ECN,
		/* Total length */
		0x00,
		0x00,
		/* Identification */
		0x00,
		0x00,
		/* Flags */
		0x00,
		0x00,
		/* Time to live */
		127,
		/* Protocol : TCP*/
		0x06,
		/* check sum */
		0x00, 0x00
	};
	
	

}
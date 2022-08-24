
#include "use.h"
#include "enc28j60.h"

uint8_t MAC[]= {0x08, 0x10, 0x19, 0x97, 0x25, 0x25};

uint16_t mb_index = 0;
uint8_t data_packet[12]={
	0x00,0x01,	// Transaction identifier
	0x00,0x00,	// Protocol identifier : MODBUS ~ 0
	0x00,0x06,	// Message length (2 bytes to follow)
	0x01,	    // The unit identifier (1 = 0x1)
	0x03,		// Function code 03
	0x03,0xE8,	// The data address : 0x0001
	0x00,0x06
};

extern void setup(void){
	enc28j60_init(MAC);
}

extern void loop(void){
	data_packet[0] = mb_index/256;
  data_packet[1] = mb_index%256;
	enc28j80_send_packet(data_packet, 12);
	mb_index+=1;
	//enc28j60_poll();
	HAL_Delay(1000);
}

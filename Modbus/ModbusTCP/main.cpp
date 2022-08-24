
#include<iostream>
#include<inttypes.h>
#include<stdio.h>
using namespace std;

uint16_t crc16(uint8_t buf[], uint8_t len)
{
  uint16_t crc = 0xFFFF;
  int pos,i;
  for (pos = 0; pos < len; pos++) {
    crc ^= (uint16_t)buf[pos]; 
    for (i = 8; i != 0; i--) {
      if ((crc & 0x0001) != 0) {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else    
        crc >>= 1; 
    }
  }
  return crc;  
}

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

int main(){
    data_packet[0] = mb_index/256;
    data_packet[1] = mb_index%256;

    uint16_t crc = crc16(&data_packet[6],6);

    printf("%02x\r\n",crc);

    return 1;
}

#include "use.h"
#include "main.h"
#include <string.h>
/*
Dominant is 0
Recessive is 1

1 . Arbitration Field: truong xac dinh ua tien (12 bit)
+ ID: 11 bit
+ RTR: 1 bit (0-Dominant Bit, 1-Recessive Bit)
2 . Identifier Extension (IDE- 1bit)
+ 0 : khung chuan
+ 1 : khung mo rong
3. R0
4. DLC (Data length Code - 4bit)
5. Data Field
6. CRC sequence
7. Acknowledge Field- ACK
8. End of Frame Field (EOF)
*/

#define Dominant 0
#define Recessive 1
#define RX_CAN_LENGTH 255

extern void can_send_package(uint8_t *data, uint8_t length){
	uint8_t data_send[50];
	uint16_t id = 25;
	uint32_t dummy = 0;
	/* Load lenght */
	dummy = length&0x0F;
	/* R0 =0 : */
	dummy &=~ (1<<4);
	/* IDE= 0 : Standard frame*/
	dummy &=~ (1<<5);
	/* RTR =0, Dominant Bit */
	dummy &=~ (1<<6);
	/* ID : 11 bit*/
	dummy |= (uint32_t)((id<<6)&0x3FF80);
	/* Start of frame */
	dummy &=~ (1<<18);
	dummy &=~ (0xF80000);
	
	data_send[0] = (uint8_t)((dummy&0xFF000000)>>24);
	data_send[1] = (uint8_t)((dummy&0x00FF0000)>>16);
	data_send[2] = (uint8_t)((dummy&0x0000FF00)>>8);
	data_send[3] = (uint8_t)((dummy&0x000000FF)>>0);
	memcpy(&data_send[4], data, length);
	data_send[4+length+1] = 
	
	HAL_CAN_AddTxMessage
	
}

extern void setup(void){
}

extern void loop(void){
}




#include "use.h"
#include "enc28j60.h"

uint8_t MAC[]= {0x08, 0x10, 0x19, 0x97, 0x25, 0x25};

extern void setup(void){
	enc28j60_init(MAC);
}

extern void loop(void){
	HAL_Delay(1000);
}

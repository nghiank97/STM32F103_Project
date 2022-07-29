
#include "use.h"

volatile float a,b,c;
volatile uint32_t d;

extern void setup(void){
	a = 1.618;
	b = 3.124;
	c = a+ b;
	d = 0xf0f0f0f0;
}

extern void loop(void){
}

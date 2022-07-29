
#include "use.h"

volatile float a,b;
volatile float sum, subtraction, multiplication, division;
volatile uint32_t end;

extern void setup(void){
	a = 1.618;
	b = 3.124;

	sum = a+ b;
	subtraction = a- b;
	multiplication  = a* b;
	division = a/ b;

	end = 0xf0f0f0f0;
}

extern void loop(void){
}



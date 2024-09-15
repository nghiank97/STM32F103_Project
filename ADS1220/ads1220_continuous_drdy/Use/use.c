
#include <stdio.h>

#include "binary.h"
#include "ads1220.h"
#include "use.h"

i32 raw;

extern void drdy_callback(){
	raw = ads1220_read_continuous();
}

extern void setup(void){
	ads1220_begin();
}

extern void loop(void){
	printf("%ld \r\n",raw);
}


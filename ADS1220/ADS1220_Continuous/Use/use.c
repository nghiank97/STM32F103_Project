
#include "use.h"
#include "ADS1220.h"

i32 raw = 0;

void EXTI3_IRQHandler(void)
{
	if (EXTI->PR & (1<<3)){
		if (!ads1220_read_drdy()){
			raw = ads1220_read_continuous();
		}
		EXTI->PR |= (1<<3);
	}
}

extern void setup()
{
	delay_ms(100);
	ads1220_begin();
    delay_ms(1);
}


extern void loop()
{
	printf("raw : %ld\r\n",raw);
	delay_ms(10);
}


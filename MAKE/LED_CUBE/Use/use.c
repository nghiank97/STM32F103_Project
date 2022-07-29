
#include "use.h"

void gpio_init(void){
	// Enable clock GPIOC 
	RCC->APB2ENR |= (1<<4);
	GPIOC->CRH |= (B0110<<20); 
}

extern void setup(void){
	gpio_init();
}

#define LED_ON() 	{GPIOC->ODR &=~ (1<<13);}
#define LED_OFF() 	{GPIOC->ODR |= (1<<13);}

extern void loop(void){
	LED_ON();
	HAL_Delay(1000);
	LED_OFF();
	HAL_Delay(1000);
}

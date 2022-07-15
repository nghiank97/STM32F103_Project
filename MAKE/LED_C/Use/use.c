
#include "use.h"

void gpio_init(void){
	// Enable clock GPIOC 
	RCC->APB2ENR |= (1<<4);
	
	GPIOC->CRH &=~ 0x00F00000;
	GPIOC->CRH |= 0x00100000;
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

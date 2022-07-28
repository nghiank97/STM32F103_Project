
#include "use.h"

extern void setup(void){
	RCC->APB2ENR |= (1<<4);
	GPIOC->CRH |= (B0110<<20);
}

#define LED_OFF() {GPIOC->ODR |= (1<<13);}
#define LED_ON() {GPIOC->ODR &=~ (1<<13);}

extern void loop(void){

	LED_OFF();
	HAL_Delay(250);
	LED_ON();
	HAL_Delay(250);
}




#include "use.h"

void gpio_init(void){
	// Enable clock GPIOC 
	RCC->APB2ENR |= (1<<4);
	
	GPIOC->CRH &=~ 0x00F00000;
	GPIOC->CRH |= 0x00100000;
}

void tim_3_encoder_init(void){
	/*
		Enable clock GPIOA : 
			+ Alternate function PA6,PA7
			+ Tim 3 encoder
	*/
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRL &=~ 0xFF000000;
	GPIOA->CRL |= 0x1100000;
	
	// Enable tim 3 
	RCC->APB1ENR |= (1<<1);
	TIM3->CNT = 0;
	TIM3->PSC = 0;
	TIM3->ARR = 0xFFFF;
	
	// TI1FP1 mapped in TI1
	TIM3->CCMR1 |= (B01<<0);
	TIM3->CCMR1 |= (B01<<8);
	
	// TI1FP1 non inverted
	TIM3->CCER |= (B1<<0);
	TIM3->CCER |= (B1<<4);
	
	// Encoder mode
	TIM3->SMCR &=~ (B111<<0);
	TIM3->SMCR |= (B001<<0);

	// enable tim 3
	TIM3->CR1 |= (1<<0);
}

extern void setup(void){
	gpio_init();
	
	/*
		Enable clock GPIOA : 
			+ Alternate function PA6,PA7
			+ Tim 3 encoder
	*/
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRL &=~ 0xFF000000;
	GPIOA->CRL |= 0x44000000;
	
	// Enable tim 3 
	RCC->APB1ENR |= (1<<1);

	// TI1FP1 mapped in TI1
	TIM3->CCMR1 |= (B01<<0);
	TIM3->CCMR1 |= (B01<<8);
	
	// TI1FP1 non inverted
	TIM3->CCER |= (B1<<0);
	TIM3->CCER |= (B1<<4);
	
	// Encoder mode
	TIM3->SMCR |= (B001<<0);
	
	TIM3->CNT = 0;
	TIM3->PSC = 0;
	TIM3->ARR = 0xFFFF;
	
	// enable tim 3
	TIM3->CR1 |= (1<<0);
}

#define LED_ON() 	{GPIOC->ODR &=~ (1<<13);}
#define LED_OFF() {GPIOC->ODR |= (1<<13);}
#define LED_TOGGLE() {GPIOC->ODR ^= (1<<13);}

int16_t count = 0;

extern void loop(void){
	count = TIM3->CNT;
//	LED_TOGGLE();
//	HAL_Delay(100);
}


#include "use.h"

void gpio_init(void){
	// Enable clock GPIOC 
	RCC->APB2ENR |= (1<<4);
	
	GPIOC->CRH &=~ 0x00F00000;
	GPIOC->CRH |= 0x00100000;
}

void encoder_init(void){

//	RCC->APB2ENR |= (1<<0);
//	
//	AFIO->EXTICR[3] |= (B1<<0)|(B1<<4);
//	EXTI->IMR |= (1<<12)|(1<<13);

//	EXTI->RTSR &=~ (1<<12)|(1<<13);
//	EXTI->FTSR |= (1<<12)|(1<<13);
//	
//	EXTI->SWIER |= (1<<12)|(1<<13);
	
//	/* 
//		NVIC EXTI Line3 interrupt : 9
//		9%4 = 1
//		(9-4*1)= 5 (5-1 = 4)
//	*/
//	NVIC->IP[1] = (0<<4);
//	/* 
//		NVIC EXTI Line3 interrupt enable
//		9/31 = 0 - 9
//		9-31*0= 9
//		NVIC->ISER[0] = (1<<9);
//	*/
//	NVIC->ISER[0] = (1<<9);

//	RCC->APB2ENR |= (1<<3);
//	GPIOB->CRL |= (8<<4);

//	RCC->APB2ENR |= (1<<0);
//	AFIO->EXTICR[0] |= (B1<<4);
//	
//	EXTI->IMR |= (1<<4);

//	EXTI->RTSR &=~ (1<<4);
//	EXTI->FTSR |= (1<<4);
//		
//	EXTI->SWIER |= (1<<4);

//	NVIC_SetPriority (EXTI1_IRQn, 1);  // Set Priority
//		
//	NVIC_EnableIRQ (EXTI1_IRQn);  // Enable Interrupt

//	/* 
//		NVIC EXTI Line 0 interrupt : 6
//		6%4 = 1
//		(6-4*1)= 2-> (2)*8 = 16
//	*/
//	NVIC->IP[1] = (0<<16);
//	/* 
//		NVIC EXTI Line3 interrupt enable
//		6/31 = 0 - 6
//		6-31*0= 6
//		NVIC->ISER[0] = (1<<6);
//	*/
//	NVIC->ISER[0] = (1<<6);

	RCC->APB2ENR |= (1<<0);  // Enable AFIO CLOCK
	
	AFIO->EXTICR[0] &= ~(0xf<<4);  // Bits[7:6:5:4] = (0:0:0:0)  -> configure EXTI1 line for PA1
	
	EXTI->IMR |= (1<<1);  // Bit[1] = 1  --> Disable the Mask on EXTI 1
	
	EXTI->RTSR &=~ (1<<1);  // Enable Rising Edge Trigger for PA1
	
	EXTI->FTSR |= (1<<1);  // Disable Falling Edge Trigger for PA1
	
	NVIC_SetPriority (EXTI1_IRQn, 1);  // Set Priority
	
	NVIC_EnableIRQ (EXTI1_IRQn);  // Enable Interrupt
}

extern void setup(void){
	gpio_init();
	encoder_init();
}

#define LED_ON() 	{GPIOC->ODR &=~ (1<<13);}
#define LED_OFF() {GPIOC->ODR |= (1<<13);}

uint16_t count = 0;

void EXTI1_IRQHandler(void)
{
	if (EXTI->PR & (1<<1))
	{
		count += 1;
		EXTI->PR |= (1<<1);
	}
}

extern void loop(void){
	LED_ON();
	HAL_Delay(500);
	LED_OFF();
	HAL_Delay(500);
}


#include "use.h"

void gpio_init(void){
	// Enable clock GPIOC 
	RCC->APB2ENR |= (1<<4);
	
	GPIOC->CRH &=~ 0x00F00000;
	GPIOC->CRH |= 0x00100000;
}

void adc_single(void){
	
	// 1. Enable clock GPIOA : ADC 1 PA6,PA7 ~ analog mode
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRL &=~ 0xFF000000;
	
	// 2. Enable clock adc 1
	RCC->APB2ENR |= (1<<9);
	
	// 3. Data Alignment RIGHT
	ADC1->CR2 &= ~(1<<11);
	
	// 4. sample time : 13.5
	ADC1->SMPR2 &=~(B111<<18);
	ADC1->SMPR2 |= (2<<18);
	
	ADC1->SMPR2 &=~(B111<<21);
	ADC1->SMPR2 |= (2<<21);
	
	// 5. External trigger conversion mode for regular channels
	ADC1->CR2 |= (1<<20);
	
	// 6. External event select for regular group
	ADC1->CR2 |= (B111<<17);
	
  // 7. Length convert = 2
	ADC1->SQR1 &=~(B1111<<20);
	ADC1->SQR1 |= (1<<20);
	
	// 8.  Discontinuous mode channel count
	ADC1->CR1 |= (0<<13);
	
	// 8.  Set index
	ADC1->SQR3 |= (6<<0);
	ADC1->SQR3 |= (7<<5);
	
	// 10. Discontinuous mode on regular channels
	ADC1->CR1 |= (1<<11);
	// 11. Scan mode
	ADC1->CR1 |= (1<<8);
	
	ADC1->CR2 |= (1<<22);
			
	// 12. ADC Convert on 
	
	ADC1->CR2 |= (1<<0);
	HAL_Delay(200);
}

uint16_t adc_a[2];

extern void setup(void){
	gpio_init();
	adc_single();

}

#define LED_ON() 	{GPIOC->ODR &=~ (1<<13);}
#define LED_OFF() {GPIOC->ODR |= (1<<13);}
#define LED_TOGGLE() {GPIOC->ODR ^= (1<<13);}

extern void loop(void){
	
	ADC1->CR2 &=~ (1<<0);
	ADC1->CR2 |= (1<<0);
	ADC1->CR2 |= (1<<22);
	ADC1->SR = 0;
	ADC1->DR = 0;
	while(ADC1->SR&(1<<1));
	adc_a[0] = ADC1->DR;
	
	HAL_Delay(100);
	
	ADC1->CR2 &=~ (1<<0);
	ADC1->CR2 |= (1<<0);
	ADC1->CR2 |= (1<<22);
	ADC1->SR = 0;
	ADC1->DR = 0;
	while(ADC1->SR&(1<<1));
	adc_a[1] = ADC1->DR;
	
	HAL_Delay(100);
}

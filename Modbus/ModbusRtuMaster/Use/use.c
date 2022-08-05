#include "use.h"

void gpio_init(void){
	RCC->APB2ENR |= (1<<4);
	GPIOC->CRH &=~ 0x00F00000;
	GPIOC->CRH |= (B0110<<20);
}

void tim_2_init(void){
	RCC->APB1ENR |= (1<<0);
	TIM2->CNT = 0;
	TIM2->PSC = 71;
	TIM2->ARR = 1749;
	TIM2->CR1 &=~	(1<<1);
	TIM2->DIER |= (1<<0);
	TIM2->BDTR |= (1<<15)|(1<<13); 

	NVIC->IP[6] = (0<<0);
	NVIC->ISER[0] = (1<<28);
}

#define LED_TOGGLE() {GPIOC->ODR ^= (1<<13);}

#define GPIOA_ENABLE_CLOCK() {RCC->APB2ENR |= (1<<2);}
#define GPIOC_ENABLE_CLOCK() {RCC->APB2ENR |= (1<<4);}
#define UART_2_ENABLE_CLOCK() {RCC->APB1ENR |= (1<<17);}

void uart_2_init(void){
	
	GPIOA_ENABLE_CLOCK();
	GPIOA->CRL &=~ 0x0000FF00;
	GPIOA->CRL |= (B1010<<8)|(B0100<<12);

	UART_2_ENABLE_CLOCK();
	USART2->CR1 &=~ (1<<12);
	USART2->CR1 &=~ (1<<9);
	USART2->CR1 |= (1<<2);
	USART2->CR1 |= (1<<3);
	USART2->BRR = 0x0139;
	USART2->CR1 |= (1<<5)|(1<<6);
	NVIC->IP[9] = (0<<16);
	NVIC->ISER[1] = (1<<6);
}

uint16_t mbrtu_crc(uint8_t buf[], uint8_t len)
{
  uint16_t crc = 0xFFFF;
	int pos,i;
  for (pos = 0; pos < len; pos++) {
    crc ^= (uint16_t)buf[pos]; 
    for (i = 8; i != 0; i--) {
      if ((crc & 0x0001) != 0) {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else    
        crc >>= 1; 
    }
  }
  return crc;  
}


#define LEN_BUFF 255

uint8_t _tx_buffer[LEN_BUFF];
volatile uint16_t _head = 0;
volatile uint16_t _tail = 0;

#define MB_ID 1
uint16_t crc = 0;

#define LED_OFF() 		{GPIOC->ODR |= (1<<13);}
#define LED_ON() 			{GPIOC->ODR &=~ (1<<13);}

void USART2_IRQHandler(void) {
	if( (USART2->SR & (1<<6)) && (USART2->CR1 & (1<<6))){
		LED_ON();
		if(_tail < _head){
			TIM2->ARR = 749;
			TIM2->CR1 |= (1<<0);
		}
		else{
			TIM2->ARR = 1749;
			TIM2->CR1 |= (1<<0);
			_tail = 0;
		}
	}
	USART2->SR &=~ (1<<6);
}

void TIM2_IRQHandler(void)
{
	if (TIM2->SR & (1<<0))
	{
		TIM2->CNT = 0;
		TIM2->CR1 &=~ (1<<0);
		USART2->DR = _tx_buffer[_tail++];
		LED_OFF();
	}
	TIM2->SR &=~ (1<<0);
}

extern void setup(void){
	gpio_init();
	tim_2_init();
	uart_2_init();
	
	_tx_buffer[0] = MB_ID;
	_tx_buffer[1] = 0x06;
	
	_tx_buffer[2] = 1000/256;
	_tx_buffer[3] = 1000%256;
	
	_tx_buffer[4] = 6/256;
	_tx_buffer[5] = 6%256;
	
	crc = mbrtu_crc(_tx_buffer,6);
	
	_tx_buffer[6] = crc%256;
	_tx_buffer[7] = crc/256;
	_head = 8;
	
	
	TIM2->CR1 |= (1<<0);
	USART2->CR1 |= (1<<13);
}

extern void loop(void){
	
}





#include "use.h"
#include "binary.h"

static void gui_main(){
	
}

#define MAX_LEN 120
uint8_t temp_1[MAX_LEN] = {0};
uint8_t temp_2[MAX_LEN] = {0};
uint8_t temp_3[MAX_LEN] = {0};
uint8_t index_temp = 0;

static void reload_array_temp(void){
	if (index_temp == MAX_LEN){
		// ssd1306_clear_graphic();
		for(int i=0;i<MAX_LEN;i++){
			if (i+25 >= MAX_LEN){
				temp_1[i] = 0;
				temp_2[i] = 0;
				temp_3[i] = 0;
			}
			else{
				temp_1[i] = temp_1[i+25];
				temp_2[i] = temp_2[i+25];
				temp_3[i] = temp_3[i+25];
			}
		}
		index_temp = MAX_LEN-25;
	}
}


void uart_2_init(void){
	/*
			pa2
			pa3
	*/
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRL |= (B1010<<8)|(B0100<<12);
	RCC->APB1ENR |= (1<<17);
	USART2->CR1 &=~ (1<<12);
	USART2->CR1 &=~ (1<<9);
	USART2->CR1 |= (1<<2);
	USART2->CR1 |= (1<<3);
	USART2->BRR = 0x0139;
	USART2->CR1 |= (1<<13);
	USART2->CR1 |= (1<<5);
	NVIC->IP[9] = (0<<16);
	NVIC->ISER[1] = (1<<6);
}

#define LEN_BUFF 255
uint8_t _rx_buffer[LEN_BUFF];
volatile uint16_t _head = 0;

void USART2_IRQHandler(void) {
	if( (USART2->SR & (1<<5)) && (USART2->CR1 & (1<<5))){
		uint8_t data = USART2->DR;
		if (data==':'){
			_head+=1;
			USART2->SR &=~ (1<<5);
			return;
		}
		else if (_head >= 1){
			if (data != (uint8_t)'\r'){
				_rx_buffer[_head++] = data;
			}
			else{
				_head = 0;
			}
		}
	}
}

extern void setup(void){
	ST7735_Init();
	ST7735_AddrSet(0,0,160,128);
	ST7735_Clear(0x0000);
}

extern void loop(void){
	
}


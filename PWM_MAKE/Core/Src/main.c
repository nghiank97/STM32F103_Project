
#include "main.h"
#include "use.h"

void SystemInit(void);
void system_clock_reinit(void);
bool wait_hse(void);


int main(void)
{
  SystemInit();
  setup();
  while (1)
  {
    loop();
  }
}


void system_clock_reinit(void)
{
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;
  /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
  RCC->CFGR &= (uint32_t)0xF8FF0000; 
  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;
  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;
  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;
  /* Disable all interrupts and clear pending bits  */
	RCC->CIR = 0x009F0000;
}

bool wait_hse(void){
	uint32_t status = 0x00;
	uint16_t timeout = 0;
	do{
		status = (RCC->CR & RCC_CR_HSERDY);

		timeout++;
	}while((status == 0x00) && (timeout != TIMEOUT_HSERDY));
	
	if (status == 0x00){
		return False;
	}
	return True;
}

/*
	HSE				8MHz
	HSE div 1 to PLL (PLLXTPRE = 0)
	PLL as source and PLL MULx4			- RCC_CFGR_PLLSRC, RCC_CFGR_PLLMULL8
	
	PLL selected as system clock		- ((uint32_t)0x00000002)
	AHP prescaler div 1 						- RCC_CFGR_HPRE_DIV1
	AHB 1 : div 2										- RCC_CFGR_PPRE1_DIV1
	AHB 2 : div 1										- RCC_CFGR_PPRE2_DIV1

	System clock : 8/1 *8*9 = 72 MHz
	AHB 1 : 72/2
	AHB 2 : 72/1
*/

void SystemInit(void){
	/* Reset clock config */
	system_clock_reinit();
	
	/* Reset HSEON bit */
  RCC->CR &= RCC_CR_HSEON;																								// bit HSEON - 16
  /* Reset HSEBYP bit */	
  RCC->CR &= RCC_CR_HSEBYP;																								// bit HSEBYP - 17
	/* Set HSEON bit */
	RCC->CR |= RCC_CR_HSEON;																								// bit HSEON - 16
	/* Wait still HES readly */
	if (wait_hse()){
		/* SYSCLK div : AHB prescaler */
		RCC->CFGR |= RCC_CFGR_HPRE_DIV1;																			// mask HPRE - 4~7
		/* PCLK1 : AHB 1 prescaler /1 */
		RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;																			// mask PPRE1 - 8~10
		/* PCLK2 : AHB 2 prescaler /1 */
		RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;																			// mask PPRE2 - 11~13
		
		/* PLL as source and PLL MULx8 */
		RCC->CFGR |= RCC_CFGR_PLLSRC;																					// bit PLLSRC - 16
		RCC->CFGR |= RCC_CFGR_PLLMULL8;																				// mask PLLMUL - 18~21
		/* enable PLL */
		RCC->CR |= RCC_CR_PLLON;																							// bit PLLON - 24
		while((RCC->CR & RCC_CR_PLLRDY) == 0){};															// bit PLLRDY - 25
				/*  PLL selected as system clock */
				RCC->CFGR |= ((uint32_t)0x00000002);															// mask SW - 0,1
				/* wait still PLL as clock system */
				while((RCC->CFGR & (uint32_t)0x0000000C) != (uint32_t)0x08){};		//mask SWS - 2,3
	}
	else{
		while(1);
	}
}
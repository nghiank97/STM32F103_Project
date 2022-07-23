#include "Config.h"
#include "Defaults.h"
#include "Use.h"
/*
	Creating the tim use the incremental encoder
*/
extern void tim_incremental_encoder_init(void){
	// Enable clock GPIOA
	RCC->APB2ENR |= (1<<2);
	// Alternate function PA6,PA7: tim 3 encoder
	GPIOA->CRL &=~ 0xFF000000;
	GPIOA->CRL |= 0x44000000;

	// 1. Enable clock tim 3
	RCC->APB1ENR |= (1<<1);
	// 2. Encoder mode  TI1FP1, TI2FP2
	TIM3->SMCR |= (B011<<0);
	// 3. TI1, TI2 select porarity : non invert
	TIM3->CCER &=~ (1<<1)|(1<<5);

	TIM3->CNT = 0;
	TIM3->PSC = 0;
	TIM3->ARR = 0xFFFF;

	// Enable tim 3
	TIM3->CR1 |= (1<<0);
	// Enable interrupt tim 3
	TIM3->DIER |= (1<<0);

	/*
		NVIC TIM 3 update : 29
		29/4 = 7
		(29-4*7)= 1-> (1)*8 = 8
	*/
	NVIC->IP[7] |= (1<<8);
	/*
		NVIC TIM 3 update : enable
		29/31 = 0
		29-31*0= 29
	*/
	NVIC->ISER[0] |= (1<<29);
}

volatile int32_t time_of_count = 0;
volatile int32_t count = 0;

void TIM3_IRQHandler(void)
{
	if (TIM3->SR &(1<<0)){
		if (TIM3->CR1 & (1<<4)){
			time_of_count -= 1;
		}
		else{
			time_of_count += 1;
		}
	}
	TIM3->SR &=~ (1<<0);
}

extern int32_t tim_encoder_get_raw_count(void){
	count = TIM3->CNT;
	count += time_of_count*65535;
	return count;
}



/*
	Creating the tim use to control the motor
	250us - 0.25ms
*/
extern void tim_handle_motor_init(void){
	// Enable tim 2 : 72MHz
	RCC->APB1ENR |= (1<<0);
	TIM2->CNT = 499;
	TIM2->PSC = 35;
	TIM2->ARR = 0xFFFF;
	TIM2->CR1 |= (1<<0);
	/*
		NVIC TIM 2 update : 28
		28/4 = 7
		(28-4*7)= 0-> (0)*8 = 8
	*/
	NVIC->IP[7] = (0<<0);
	/*
		NVIC TIM 1 update : enable
		28/31 = 0
		28-31*0= 3
	*/
	NVIC->ISER[0] = (1<<3);
}

extern void tim_start_motor_handle(void){
	TIM2->DIER |= (1<<0);
}

void TIM2_IRQHandler(void)
{
	if (((TIM2->SR &(1<<0)) != 0) && ((TIM2->DIER &(1<<0)) != 0)){
		motor_handle();
	}
	TIM2->SR &=~ (1<<0);
}



/*
	Creating the tim pwm center-align
*/
extern void tim_pwm_init(void){
	// Enable clock GPIOA : alternate function
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRH &=~ 0x0000000F;
	GPIOA->CRH |= 0x0000000B;

	// Enable tim 1
	RCC->APB2ENR |= (1<<11);
	TIM1->CNT = 0;
	TIM1->PSC = 0;
	TIM1->ARR = DEF_PWM_LIM;

	// Center-aligned mode 1
	TIM1->CR1 |= (1<<5);
	// PWM Mode 1 : CHANNEL 1
	TIM1->CCMR1 |= (B110<<4)|(1<<3);

	// Capture/Compare 1 output polarity
	TIM1->CCER |= (1<<0)|(1<<1);

	// enable tim 1
	TIM1->BDTR |= (1<<15)|(1<<13);
	TIM1->CR1 |= (1<<0);

	TIM1->CCR1 = 0;

	/*
		NVIC TIM 1 update : 25
		25/4 = 6
		(25-4*6)= 1-> (1)*8 = 8
	*/
	NVIC->IP[6] = (0<<8);
	/*
		NVIC TIM 1 update : enable
		25/31 = 0
		25-31*0= 25
	*/
	NVIC->ISER[0] = (1<<25);
}

extern void tim_pwm_set_value(uint16_t pwm){
	if (pwm < 0){
		pwm = 0;
	}
	if (pwm > DEF_PWM_LIM){
		pwm = DEF_PWM_LIM;
	}
	TIM1->CCR1 = pwm;
}

extern void tim_start_get_adc(void){
	// Update interrupt
	TIM1->DIER |= (1<<0);
}

void TIM1_IRQHandler(void)
{
	if (((TIM1->SR & (1<<0)) != 0) && ((TIM1->DIER &(1<<0)) != 0)){
		pwm_trig_adc_cb();
	}
	TIM1->SR &=~ (1<<0);
}


extern void drive_init(void){
	// Enable clock GPIOA : PA9, PA10
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRH &=~ 0x00000FF0;
	GPIOA->CRH |= 0x00000660;
}


extern void adc_init(void){
	// 1. Enable clock GPIOA : ADC 1 PA3
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRL &=~ 0x0000F000;
	// 2. Enable clock adc 1
	RCC->APB2ENR |= (1<<9);
	// 3. Data Alignment RIGHT
	ADC1->CR2 &= ~(1<<11);
	// 4. sample time : 13.5
	ADC1->SMPR2 &=~(B111<<9);
	ADC1->SMPR2 |= (2<<9);

	// 5. External trigger conversion mode for regular channels
	ADC1->CR2 |= (1<<20);
	// 6. External event select for regular group
	ADC1->CR2 |= (B111<<17);

  // 7. Length convert = 1
	ADC1->SQR1 &=~(B1111<<20);
	ADC1->SQR1 |= (0<<20);

	// 8. Continuous conversion : singel mode
	ADC1->CR2 &=~ (1<<1);
	// 9. set index
	ADC1->SQR3 |= (3<<0);
	// 10. ADC Convert on
	ADC1->CR2 |= (1<<0);
	HAL_Delay(100);
}

extern uint16_t read_adc(void){
	ADC1->DR = 0;
	ADC1->CR2 |= (1<<22);
	while(ADC1->SR&(1<<1));
	return ADC1->DR;
}









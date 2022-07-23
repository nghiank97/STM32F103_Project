
#ifndef _CONFIG_H
#define _CONFIG_H

#if __cplusplus
extern "C"{
#endif

#include "main.h"
#include <Binary.h>

extern void tim_incremental_encoder_init(void);
extern int32_t tim_encoder_get_raw_count(void);

extern void tim_handle_motor_init(void);
extern void tim_start_motor_handle(void);

extern void tim_pwm_init(void);
extern void tim_pwm_set_value(uint16_t pwm);
extern void tim_start_get_adc(void);


#define IN1_OFF() 	{GPIOA->ODR |= (1<<9);}
#define IN1_ON() 	{GPIOA->ODR &=~ (1<<9);}
#define IN2_OFF() 	{GPIOA->ODR |= (1<<10);}
#define IN2_ON() 	{GPIOA->ODR &=~ (1<<10);}
extern void drive_init(void);


extern void adc_init(void);
extern uint16_t read_adc(void);

#if __cplusplus
}
#endif

#endif

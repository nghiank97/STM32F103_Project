
#ifndef _LQR_H
#define _LQR_H

#if __cplusplus
extern "C"{
#endif

#include "main.h"
#include "Binary.h"

void debug_init(void);
extern void usart_2_init(void);
extern void encoder_init(void);
extern void tim_2_init(void);
extern void pwm_1_init(void);

#if __cplusplus
}
#endif

#endif

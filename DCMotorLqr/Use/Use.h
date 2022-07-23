#ifndef USE_H
#define USE_H

#ifdef __cplusplus
extern "C"{
#endif
#include "main.h"

extern void setup(void);
extern void loop(void);
extern void pwm_trig_adc_cb(void);
extern void motor_handle(void);

#ifdef __cplusplus
}
#endif
#endif

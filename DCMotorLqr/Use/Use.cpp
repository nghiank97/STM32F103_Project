
#include <stdio.h>
#include <math.h>
#include "DCMotor.h"
#include "use.h"

Drive drive;
Encoder encoder;
DCMotor dc_motor = DCMotor(&drive, &encoder);

extern void setup(void){
	dc_motor.init();

	tim_start_get_adc();
	tim_start_motor_handle();
}

extern void loop(void)
{
	dc_motor.loop();
}

extern void motor_handle(void){
	dc_motor.move();
}

extern void pwm_trig_adc_cb(void)
{
	dc_motor.getAdcVoltage();
}



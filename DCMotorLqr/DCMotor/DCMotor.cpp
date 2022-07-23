#include "DCMotor.h"

DCMotor::DCMotor(Drive* _l298, Encoder* _encoder){

	l298 = _l298;
	encoder = _encoder;
}

void DCMotor::init(void){

}

void DCMotor::loop(void){

}

void DCMotor::move(void){
	controller();
}

void DCMotor::controller(void){
	index+=1;
	encoder->update();
	if(index == 40){
		index = 0;

		position_sp = scuver(1000);

		position = encoder->getPosition();
		velocity_raw = encoder->getVelocity();
		velocity = velocity*0.125 + velocity_raw*0.875;

		velocity_sp = PID_angle(position_sp-position);
		i_a_sp = PID_velocity(velocity_sp-velocity);
	}
	control_current();
}

void DCMotor::control_current(void){

	i_a_raw = (adc_offset - adc_raw)*CURRENT_SCALE;
	i_a = i_a*0.125 + i_a_raw*0.875;

	voltage = PID_current(i_a_sp-i_a);
	l298->setVoltage((int16_t)voltage);
}

void DCMotor::getAdcVoltage(void){
	adc_raw = read_adc();
}

float DCMotor::shaftVelocity(void){
	return encoder->getVelocity();
}
float DCMotor::shaftAngle(void){
	return encoder->getPosition();
}

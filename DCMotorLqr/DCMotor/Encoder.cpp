#include "Encoder.h"
#include "Defaults.h"

Encoder::Encoder(void){
	#ifndef ENCODER_CPR
		#error "Don't define the encoder cpr"
	#endif
}

void Encoder::update(void){
	raw_count = tim_encoder_get_raw_count();
	position = raw_count/(float)ENCODER_CPR;
}

float Encoder::getPosition(void){
	return position;
}

float Encoder::getVelocity(void){
	velocity = (position - part_position)*POSITION_2_VEL;
	part_position = position;
	return velocity;
}


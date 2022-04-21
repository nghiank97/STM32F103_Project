
#ifndef DC_Motor_H
#define DC_Motor_H

#ifdef __cplusplus
extern "C"{
#endif
#include "main.h"
#include <inttypes.h>

class DCMotor{

private:
	int16_t current;
	int16_t current_sp;

public:
	DCMotor();
	void init();
	void loop();

};

#ifdef __cplusplus
}
#endif
#endif

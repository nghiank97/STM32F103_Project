

#ifndef DRIVE_H
#define DRIVE_H

#ifdef __cplusplus
extern "C"{
#endif

#include "Config.h"

class Drive{
public:
	Drive(void);
	void setVoltage(int16_t pwm);
};

#ifdef __cplusplus
}
#endif
#endif

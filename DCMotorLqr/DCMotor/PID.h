

#ifndef PID_H
#define PID_H

#include <inttypes.h>

class PIDController
{
private:
	float error_prev;

    float proportional;
    float integral;
    float derivative;

    float output;
    float P;
    float I;
    float D;
    uint16_t limit;
    float ts;

public:
    PIDController(float P, float I , float D, uint16_t limit);
	float operator() (float error);
    void reset(void);
};

#endif

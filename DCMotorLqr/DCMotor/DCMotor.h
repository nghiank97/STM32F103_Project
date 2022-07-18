

#ifndef DCMotor_h
#define DCMotor_h

#include <stdio.h>
#include <math.h>
#include "main.h"
#include "Config.h"

class DCMotor
{
public:
  DCMotor();
  void init(void);
  void enable(void);
  void disable(void);

  void control_current(void);
  void controller(void);
  void getAdcVoltage(void);

  float shaftVelocity(void);
  float shaftAngle(void);

  LSPB scuver{0, 3, 0.01};

  float i_a;
  float velocity, velocity_sp, velocity_raw;
  float position_sp, position;

public:

};

#endif

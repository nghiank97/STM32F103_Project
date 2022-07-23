

#ifndef DCMotor_h
#define DCMotor_h

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <math.h>
#include "main.h"
#include "Config.h"
#include "Encoder.h"
#include <Drive.h>
#include "LSPB.h"
#include "PID.h"

class DCMotor
{
public:
  DCMotor(Drive* _l298, Encoder* encoder);

  void init(void);

  void controller(void);
  void control_current(void);

  float shaftVelocity(void);
  float shaftAngle(void);

  void loop(void);
  void move(void);
  void getAdcVoltage(void);

private:

  int16_t index = 0;

  uint16_t adc_raw, adc_offset = 0;
  float i_a_raw,i_a_sp, i_a = 0;
  float velocity_raw, velocity_sp, velocity = 0;
  float position_sp, position = 0;
  float voltage = 0;

  LSPB scuver{200, 0.01};

  PIDController PID_current		{DEF_PID_CURR_P, DEF_PID_CURR_I, DEF_PID_CURR_D, DEF_PID_CURR_LIM};
  PIDController PID_velocity	{DEF_PID_VEL_P, DEF_PID_VEL_I, DEF_PID_VEL_D, DEF_PID_VEL_LIM};
  PIDController PID_angle		{DEF_PID_ANGLE_P, DEF_PID_ANGLE_I, DEF_PID_ANGLE_D, DEF_PID_ANG_LIM};

  Drive* l298;
  Encoder* encoder;
};

#ifdef __cplusplus
}
#endif

#endif

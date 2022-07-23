
#ifndef DEFAULTS_H
#define DEFAULTS_H

/*
 * Motor
 */
#define Ra 2.7
#define La 0.004
#define Jm 0.0001
#define Bm 0.0000093
#define Kt 0.105
#define Ke 0.105
#define Tc 2

#define ENCODER_CPR 2400
/*
	Tv = 0.01
	POSITION_2_VEL = (1/Tv)*10 = 1000
*/
#define POSITION_2_VEL 1000


#define CURRENT_SCALE 1


#define DEF_PWM_LIM                     	3599
#define DEF_VEL_LIM                         0
#define DEF_CURR_LIM                     	0

#define DEF_PID_CURR_P                   	0
#define DEF_PID_CURR_I                    	0
#define DEF_PID_CURR_D						0
#define DEF_PID_CURR_LIM                	(DEF_PWM_LIM)

#define DEF_PID_VEL_P                       0
#define DEF_PID_VEL_I                       0
#define DEF_PID_VEL_D                       0
#define DEF_PID_VEL_LIM                   	(DEF_CURR_LIM)

#define DEF_PID_ANGLE_P                     0
#define DEF_PID_ANGLE_I                     0
#define DEF_PID_ANGLE_D						0
#define DEF_PID_ANG_LIM                   	(DEF_VEL_LIM)

#endif

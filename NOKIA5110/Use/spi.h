
#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C"{
#endif

#include <inttypes.h>
#ifndef _contraint
#define _contraint(v,min,max) (v<min?min:(v>max?max:v))
#endif 

class PIController{
    private:
        uint8_t _P;
        uint8_t _I;
        uint16_t limit;
        int16_t pre_I;
        int16_t pre_error;

    public:
        PIController(uint8_t P,uint8_t I,int16_t limit);
        int16_t operator() (int16_t error,uint16_t ts);
};

#ifdef __cplusplus
}
#endif
#endif

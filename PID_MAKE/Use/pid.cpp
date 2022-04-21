
#include "pid.h"

PIController::PIController(uint8_t P,uint8_t I,int16_t limit)
    :_P(P),_I(I),limit(limit)
{
    // nothing
}

int16_t PIController::operator() (int16_t error,uint16_t ts){
    int16_t p_val = error*_P;
    int16_t i_val = pre_I + _I*ts*error;
    int16_t output = p_val+i_val;
    output = _contraint(output,-limit,limit);
    return output;
}
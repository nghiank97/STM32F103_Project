#include "use.h"
#include "pid.h"
#include "serial.h"
#include <stdio.h>

PIController PI_Speed{1,1,24000};

void setup(){

}

void loop(){
    uint16_t val = PI_Speed(10,2);
}
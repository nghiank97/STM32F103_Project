
#ifndef BASE_DISPLAY_H
#define BASE_DISPLAY_H
#include <stdint.h>
#include <math.h>
#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN
#undef BIN
#endif
#define BIN 2

class BaseDisplay{
private:
    uint8_t printNumber(unsigned long n, uint8_t base);
    uint8_t printFloat(double number, uint8_t digits);
public:
    virtual uint8_t write(uint8_t) = 0;

    uint8_t print(uint8_t);

    uint8_t print(const char *);
    uint8_t print(const char *,unsigned int);
    uint8_t print(unsigned char, int = DEC);
    uint8_t print(int, int = DEC);
    uint8_t print(unsigned int, int = DEC);
    uint8_t print(long, int = DEC);
    uint8_t print(unsigned long, int = DEC);
    uint8_t print(double, int = 2);
};

#endif


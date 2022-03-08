
#include "base_display.h"


uint8_t BaseDisplay::printNumber(unsigned long n, uint8_t base)
{
  char buf[8 * sizeof(long) + 1];
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';
  if (base < 2)
    base = 10;
  do
  {
    char c = n % base;
    n /= base;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while (n);

  return print(str);
}

uint8_t BaseDisplay::printFloat(double number, uint8_t digits)
{
  uint8_t n = 0;

  if (isnan(number))
    return print("nan");
  if (isinf(number))
    return print("inf");
  if (number > 4294967040.0)
    return print("ovf");
  if (number < -4294967040.0)
    return print("ovf");
  if (number < 0.0)
  {
    n += write('-');
    number = -number;
  }
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i)
    rounding /= 10.0;

  number += rounding;
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += print(int_part);
  if (digits > 0)
  {
    n += write('.');
  }
  while (digits-- > 0)
  {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)(remainder);
    n += print(toPrint);
    remainder -= toPrint;
  }

  return n;
}

uint8_t BaseDisplay::print(uint8_t data)
{
   return write(data);
}

uint8_t BaseDisplay::print(const char *data){
	uint8_t i = 0;
	while(*data != 0){
		i+= write(*data++);
	}
	return i;
}

uint8_t BaseDisplay::print(const char *data,unsigned int len){
	for(unsigned int i=0;i<len;i++){
		write(*data++);
	}
	return len;
}

uint8_t BaseDisplay::print(unsigned char b, int base)
{
  return print((unsigned long)b, base);
}

uint8_t BaseDisplay::print(int n, int base)
{
  return print((long)n, base);
}

uint8_t BaseDisplay::print(unsigned int n, int base)
{
  return print((unsigned long)n, base);
}

uint8_t BaseDisplay::print(long n, int base)
{
  if (base == 0)
  {
    return write((uint8_t)n);
  }
  else if (base == 10)
  {
    if (n < 0)
    {
      int t = write((uint8_t)'-');
      n = -n;
      return printNumber(n, 10) + t;
    }
    return printNumber(n, 10);
  }
  else
  {
    return printNumber(n, base);
  }
}

uint8_t BaseDisplay::print(unsigned long n, int base)
{
  if (base == 0)
    return write((uint8_t)n);
  else
    return printNumber(n, base);
}

uint8_t BaseDisplay::print(double n, int digits)
{
  return printFloat(n, digits);
}





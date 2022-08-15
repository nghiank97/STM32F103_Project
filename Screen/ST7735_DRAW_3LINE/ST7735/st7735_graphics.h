
#ifndef ST7735_GRAPHICS_H
#define ST7735_GRAPHICS_H
#include "st7735.h"

extern void ST7735_line(int y1, int x1, int y2, int x2,uint16_t color);
extern void ST7735_8_point(int xc, int yc, int x, int y, uint16_t color);
extern void ST7735_circle(int yc,int xc,int r,uint16_t color);

#endif



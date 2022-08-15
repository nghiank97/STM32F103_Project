#include "st7735_graphics.h"

#define _abs(x) (x>=0?x:-x)

extern void ST7735_line(int y1, int x1, int y2, int x2,uint16_t color){
	int x, y, Dx, Dy, p;
	Dx = _abs(x2 - x1);
	Dy = _abs(y2 - y1);
	p = 2*Dy - Dx;
	x = x1;
	y = y1;
	ST7735_Pixel(x,y,color);
	while(x != x2){
		if (p<0) p += 2*Dy;
		else{
			p += 2*(Dy-Dx);
			y ++;
		}
		x ++;
		ST7735_Pixel(x, y, color);
	}
}

extern void ST7735_8_point(int xc, int yc, int x, int y, uint16_t color)
{
    ST7735_Pixel( y + xc, x + yc, color);
    ST7735_Pixel(-y + xc, x + yc, color);
    ST7735_Pixel(y + xc, -x + yc, color);
    ST7735_Pixel(-y + xc, -x + yc, color);
    
    ST7735_Pixel(x + xc, y + yc, color);
    ST7735_Pixel(-x + xc, y + yc, color);
    ST7735_Pixel(x + xc, -y + yc, color);
    ST7735_Pixel(-x + xc, -y + yc, color);
}

extern void ST7735_circle(int yc,int xc,int r,uint16_t color){
    int x = 0; int y = r;
    int f = 1 - r;
    ST7735_8_point(yc, xc, y, x, color);
    while (x < y){
        if (f < 0){
            f += 2*x + 3;
        }
        else{
            y--;
            f += 2*(x-y) + 5;
        }
        x++;
        ST7735_8_point(yc, xc, y, x, color);
    }
}

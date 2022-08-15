
#include "use.h"

static void gui_main(){
	
}

extern void setup(void){
	ST7735_Init();
	ST7735_AddrSet(0,0,160,128);
	ST7735_Clear(0x0000);
}

extern void loop(void){
	
}


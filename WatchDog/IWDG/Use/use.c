
#include "use.h"
#include "iwdg.h"

extern void setup(){
    delay_ms(1);
    iwdg_init();
    iwdg_start();
}

extern void loop()
{
	printf("watch dog\r\n");
	delay_ms(1);
	iwdg_reload();
}


#include "iwdg.h"

// timeout ~ (prescaler*reload)/fLSI

void iwdg_init(void){
//	000: divider /4
//	001: divider /8
//	010: divider /16
//	011: divider /32
//	100: divider /64
//	101: divider /128
//	110: divider /256
//	111: divider /256

	// create timeout = 0.02s
	// reload = timeout*40k/prescaler;
	// 200 = 0.2*40000/4;
	IWDG->KR 	= RECONFI_IWDG;
	IWDG->PR 	= 0;		// PR = 4
	IWDG->RLR	= 2000;		// [0-4095]
}

void iwdg_reload(void){
	IWDG->KR 	= RELOAD_IWDG;
}

void iwdg_start(void){
	IWDG->KR 	= START_IWDG;
}

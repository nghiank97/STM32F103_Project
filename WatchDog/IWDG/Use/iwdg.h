
/*
 * File   : iwdg.h
 * Author : hong xiem
 * Date   : 12-Step, 2024
 * */


#ifndef _IWDG_H
#define _IWDG_H

#include "binary.h"
#include "main.h"

// timeout : 0.0002s ~ 0.2ms

#define START_IWDG		0xCCCC
#define RELOAD_IWDG		0xAAAA
#define RECONFI_IWDG	0x5555

void iwdg_init(void);
void iwdg_reload(void);
void iwdg_start(void);

#endif /* _IWDG_H */

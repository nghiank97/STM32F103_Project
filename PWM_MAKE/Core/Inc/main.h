
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f1xx.h"

typedef enum                {False, True}           bool;
typedef enum                {OFF,ON}                flask;
typedef enum                {LOW,HIGH}              level;

typedef unsigned						char 										byte;
typedef unsigned 						int			            		word;
#define TIMEOUT_HSERDY										          1000					

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

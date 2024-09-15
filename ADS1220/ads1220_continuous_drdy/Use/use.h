#ifndef _USE_H_
#define _USE_H_

#ifdef __cplusplus
extern "C"{
#endif
#include "main.h"

extern void setup(void);
extern void loop(void);
extern void drdy_callback(void);

#ifdef __cplusplus
}
#endif
#endif /* _USE_H_ */

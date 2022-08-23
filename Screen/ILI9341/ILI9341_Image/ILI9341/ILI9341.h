
#ifndef ILI9341_H
#define ILI9341_H
#include "main.h"
#include <inttypes.h>

typedef void (*_rst_low)(void);
typedef void (*_rst_high)(void);

typedef void (*_csx_low)(void);
typedef void (*_csx_high)(void);

typedef void (*_dcx_high)(void);
typedef void (*_dcx_low)(void);

typedef void (*_rdx_high)(void);
typedef void (*_rdx_low)(void);

typedef void (*_wdx_high)(void);
typedef void (*_wdx_low)(void);


typedef void (*_LCD0_low)(void);
typedef void (*_LCD1_high)(void);

typedef void (*_csx_low)(void);
typedef void (*_csx_high)(void);

typedef void (*_dcx_high)(void);
typedef void (*_dcx_low)(void);

typedef void (*_rdx_high)(void);
typedef void (*_rdx_low)(void);

typedef void (*_wdx_high)(void);
typedef void (*_wdx_low)(void);

typedef enum{
	NOP = 0,
	SOFTWARE_RESET = 0x01,
}ILI9341_COMMAND;

extern void ILI9341_write_command(ILI9341_COMMAND command);
extern void ILI9341_write_data(uint16_t data);


#endif


#ifndef USE_H
#define USE_H

#ifdef __cplusplus
extern "C"{
#endif

#include "mb.h"
#include "mbport.h"

#include "port.h"
#include "mbconfig.h"
#include <string.h>
#include <stdarg.h>
#include "mbutils.h"

#include "main.h"

void setup(void);
void loop(void);
	
eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode );
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode );
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete );


#ifdef __cplusplus
}
#endif

#endif



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

typedef enum
{
    ACCESS_ADDR_ERR, // Lỗi địa chỉ truy cập
    ACCESS_NO_ERR,  // Không hề có lỗi
    ACCESS_WRONG_DATA_TYPE // Lỗi sai kiểu dữ liệu modbus
} eMBAccessDataCode;

typedef enum
{
	REG_HOLDING,
	REG_INPUT
} MB_Data_Type;

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode );
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode );
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete );

eMBAccessDataCode MBSetData16Bits(MB_Data_Type DataType, USHORT Address, USHORT Value);
eMBAccessDataCode MBSetData32Bits(MB_Data_Type DataType, USHORT Address, ULONG Value);

void setup();
void loop();

#ifdef __cplusplus
}
#endif

#endif


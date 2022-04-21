
#include "use.h"

#define REG_INPUT_NREGS 16
#define REG_HOLDING_NREGS 16
#define REG_COIL_NREGS 16
#define REG_DESCRETES_INPUT_NREGS 16

#define REG_INPUT_START 1000
#define REG_HOLDING_START 2000
#define REG_COIL_START 0
#define REG_DESCRETES_INPUT_START 0

uint16_t usRegInputStart = REG_INPUT_START;
uint16_t usRegInputBuf[REG_INPUT_NREGS];                           // Mảng 16bit lưu giá trị của Register Input (Read Only)

uint16_t usRegHoldingStart = REG_HOLDING_START;
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS];                   		// Mảng 16bit lưu giá trị của Register Holding (Read/Write)

uint16_t usCoilStart = REG_COIL_START;
uint8_t usCoilBuf[(REG_COIL_NREGS/8)+1];                            // Mảng 1 bit lưu giá trị của Output Coil  (Read/Write )

uint16_t usDescreteStart = REG_DESCRETES_INPUT_START;
uint8_t usDescreteBuf[(REG_DESCRETES_INPUT_NREGS / 8)+1];       	// Mảng 1bit  lưu giá trị của Descretes Input (Read Only)

eMBErrorCode eStatus;

void setup(){

  usRegInputBuf[0] = 11;
  usRegInputBuf[1] = 22;
  usRegInputBuf[2] = 33;
  usRegInputBuf[3] = 44;
  usRegInputBuf[4] = 55;
  usRegInputBuf[5] = 66;
  usRegInputBuf[6] = 77;
  usRegInputBuf[7] = 88;

  eStatus = eMBInit( MB_RTU, 1, 3, 115200, MB_PAR_NONE);
  eStatus = eMBEnable();
}

void loop(){
	eMBPoll();
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) &&
        ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
                *pucRegBuffer++ = ( UCHAR )( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( UCHAR )( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
   eMBErrorCode    eStatus = MB_ENOERR;
   int             iRegIndex;

   if( ( usAddress >= REG_COIL_START ) &&
       ( usAddress + usNCoils <= REG_COIL_START + REG_COIL_NREGS ) )
   {
       iRegIndex = ( int )( usAddress - usCoilStart );
       switch ( eMode )
       {
           /* Pass current register values to the protocol stack. */
       case MB_REG_READ:
           while( usNCoils > 0 )
           {
        	   UCHAR ucResult = xMBUtilGetBits( usCoilBuf, iRegIndex, 1 );
        	   xMBUtilSetBits( pucRegBuffer, iRegIndex - ( usAddress - REG_COIL_START ), 1, ucResult );
               iRegIndex++;
               usNCoils--;
           }
           break;
       case MB_REG_WRITE:
           while( usNCoils > 0 )
           {
        	   UCHAR ucResult = xMBUtilGetBits( pucRegBuffer, iRegIndex - ( usAddress - REG_COIL_START ), 1 );
        	   xMBUtilSetBits( usCoilBuf, iRegIndex, 1, ucResult );
               iRegIndex++;
               usNCoils--;
           }
       }
   }
   else
   {
       eStatus = MB_ENOREG;
   }
   return eStatus;
}
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
   eMBErrorCode    eStatus = MB_ENOERR;
   int             iRegIndex;
   if( ( usAddress >= REG_DESCRETES_INPUT_START )
       && ( usAddress + usNDiscrete <= REG_DESCRETES_INPUT_START + REG_DESCRETES_INPUT_NREGS) )
   {
       iRegIndex = ( int )( usAddress - usDescreteStart );
       while( usNDiscrete > 0 )
       {
    	   UCHAR ucResult = xMBUtilGetBits( usDescreteBuf, iRegIndex, 1 );
    	   xMBUtilSetBits( pucRegBuffer, iRegIndex - ( usAddress - usDescreteStart ), 1, ucResult );
           iRegIndex++;
           usNDiscrete--;
       }
   }
   else
   {
       eStatus = MB_ENOREG;
   }
   return eStatus;
}

/*
*   Write: facebook.com/microcontroler
*   Hàm này dùng để thay đổi dữ liệu 16bit của thanh ghi Modbus
* Ví dụ muốn gán giá trị 325 cho thanh ghi Holding tại địa chỉ 170 thì thực hiện như sau:
* MBSetData16Bits(REG_HOLDING, 170, 325);
* Hàm trả về giá trị
*   ACCESS_NO_ERR: Nếu không có lỗi gì
*   ACCESS_ADDR_ERR: Nếu truy cập vào 1 địa chỉ thanh ghi mà không được cấp phát bộ nhớ
*               Ví dụ: #define REG_HOLDING_START       30 -> Tức là vùng nhớ Holding được truy cập từ địa chỉ 30 trở lên
*                   Nếu địa chỉ < 30 thì Hàm sẽ trả về lỗi ACCESS_ADDR_ERR
*       ACCESS_WRONG_DATA_TYPE: Nghĩa là loại dữ liệu bị sai, hàm này chỉ được truy cập vào 2 thanh ghi dữ liệu là
*               REG_INPUT và REG_HOLDING
*/
eMBAccessDataCode MBSetData16Bits(MB_Data_Type DataType, USHORT Address, USHORT Value)
{
    USHORT BeginAddress;
    USHORT LastAddress16;

    switch (DataType)
    {
        case    REG_INPUT:
            BeginAddress = REG_INPUT_START;
            if (Address < (BeginAddress -1 )) return ACCESS_ADDR_ERR;
            LastAddress16 = REG_INPUT_START + REG_INPUT_NREGS -1;

            if (Address > LastAddress16) return ACCESS_ADDR_ERR;

            usRegInputBuf[Address - BeginAddress ] = Value;

            break;
        case REG_HOLDING:
            BeginAddress = REG_HOLDING_START;
            if (Address <( BeginAddress -1)) return ACCESS_ADDR_ERR;
            LastAddress16 = REG_HOLDING_START + REG_HOLDING_NREGS -1;

            if (Address > LastAddress16) return ACCESS_ADDR_ERR;

            usRegHoldingBuf[Address - BeginAddress ] = Value;
            break;
        default:
            return ACCESS_WRONG_DATA_TYPE;

    }
    return ACCESS_NO_ERR;
}

/*
*   Write: facebook.com/microcontroler
*   hàm này dùng để thay đổi dữ liệu 32bit của thanh ghi Modbus
* Ví dụ: Cần ghi giá trị 0x554433 vào địa chỉ 100 của thanh ghi HOLDING
* MBSetData32Bits(REG_HOLDING, 100, 0x554433);
* Hàm trả về giá trị
*   ACCESS_NO_ERR: Nếu không có lỗi gì
*   ACCESS_ADDR_ERR: Nếu truy cập vào 1 địa chỉ thanh ghi mà không được cấp phát bộ nhớ
*               Ví dụ: #define REG_HOLDING_START       30 -> Tức là vùng nhớ Holding được truy cập từ địa chỉ 30 trở lên
*                   Nếu địa chỉ < 30 thì Hàm sẽ trả về lỗi ACCESS_ADDR_ERR
*       ACCESS_WRONG_DATA_TYPE: Nghĩa là loại dữ liệu bị sai, hàm này chỉ được truy cập vào 2 thanh ghi dữ liệu là
*               REG_INPUT và REG_HOLDING
*/
eMBAccessDataCode MBSetData32Bits(MB_Data_Type DataType, USHORT Address, ULONG Value)
{
    USHORT BeginAddress;
    USHORT LastAddress32;

    USHORT HightData = (Value & 0xFFFF0000)>>16;
    USHORT LowData = (Value & 0x0000FFFF);

    switch (DataType)
    {
        case    REG_INPUT:

            /*  --- Check Address Correction --- */
            BeginAddress = REG_INPUT_START;
            if (Address < (BeginAddress -1)) return ACCESS_ADDR_ERR;
            LastAddress32 = REG_INPUT_START + REG_INPUT_NREGS -2;
            if (Address > LastAddress32) return ACCESS_ADDR_ERR;


            /* -------- Assign Value to Reg ----- */
        usRegInputBuf[Address - BeginAddress ] = LowData;   // Origin: HightData
        usRegInputBuf[Address - BeginAddress + 1] = HightData;  // Origin: LowData

            break;
        case REG_HOLDING:

            /*  --- Check Address Correction --- */
            BeginAddress = REG_HOLDING_START;
            if (Address < (BeginAddress -1)) return ACCESS_ADDR_ERR;
            LastAddress32 = REG_HOLDING_START + REG_HOLDING_NREGS -2;
            if (Address > LastAddress32) return ACCESS_ADDR_ERR;

            /* -------- Assign Value to Reg ----- */
            usRegHoldingBuf[Address - BeginAddress ] = LowData;
            usRegHoldingBuf[Address - BeginAddress + 1] = HightData;

            break;
        default:
            return ACCESS_WRONG_DATA_TYPE;
    }
    return ACCESS_NO_ERR;
}



#include "use.h"
#include "MCP2515.h"
#include "CANSPI.h"

uCAN_MSG txMessage;
uCAN_MSG rxMessage;

extern void setup(void){
	CANSPI_Initialize();
	
	txMessage.frame.idType = dSTANDARD_CAN_MSG_ID_2_0B;
	txMessage.frame.id = 0x0A;
	txMessage.frame.dlc = 8;
	txMessage.frame.data0 = 0;
	txMessage.frame.data1 = 1;
	txMessage.frame.data2 = 2;
	txMessage.frame.data3 = 3;
	txMessage.frame.data4 = 4;
	txMessage.frame.data5 = 5;
	txMessage.frame.data6 = 6;
	txMessage.frame.data7 = 7;
}

extern void loop(void){
    CANSPI_Transmit(&txMessage);
    HAL_Delay(1000);
}

#include <stdio.h>
#include "config.h"
#include "enc28j60.h"

volatile BANK enc28j60_bank = BANK_NONE;
extern void enc28j80_set_bank(BANK bank){
	if(bank != enc28j60_bank){
		enc28j60_bank = bank;
		enc28j60_write_cmd(BFC, ECON1, 0x03);
		enc28j60_write_cmd(BFS, ECON1, bank);
	}
}

extern uint8_t enc28j80_read_control_res(BANK bank,uint8_t address){
	uint8_t data;
	enc28j80_set_bank(bank);
	data = enc28j60_read_cmd(RCR,address);
	return data;
}

extern void enc28j80_write_control_res(BANK bank,uint8_t address,uint8_t data){
	enc28j80_set_bank(bank);
	enc28j60_write_cmd(RCR,address,data);
}

uint8_t status = 0;

extern void enc28j60_init(void){
	uint8_t status = 0;
	uint16_t timeout = 1000;
	enc28j60_RST_low();
	delay_ms(1);
	enc28j60_RST_high();
	delay_ms(1000);
	enc28j60_spi_init();
	
	/* reset software */
	enc28j60_write_cmd(SRC, 0x1F,0);
	delay_ms(2);
	/* check  */
	do{
		status = enc28j60_read_cmd(RCR, ESTAT);
		if ((status&(1<<CLKRDY)) != 0x00){
			break;
		}
	}while(timeout--);

#ifdef DEBUG
	if (timeout == 0){
		printf("Time out \r\n");
	}
	else{
		printf("Finish {%2d}\r\n",status);
	}
#endif

	/* Set address rx, tx buff */
	enc28j80_write_control_res(BANK_0, ETXSTL, TX_START_ADD%256);
	enc28j80_write_control_res(BANK_0, ETXSTH, TX_START_ADD/256);
	
	enc28j80_write_control_res(BANK_0, ETXNDL, TX_STOP_ADD%256);
	enc28j80_write_control_res(BANK_0, ETXNDH, TX_STOP_ADD/256);
	
	enc28j80_write_control_res(BANK_0, ERXSTL, RX_START_ADD%256);
	enc28j80_write_control_res(BANK_0, ERXSTH, RX_START_ADD/256);
	
	enc28j80_write_control_res(BANK_0, ERXNDL, RX_STOP_ADD%256);
	enc28j80_write_control_res(BANK_0, ERXNDH, RX_STOP_ADD/256);
	
	/* Reset address rx, tx buff to 0*/
	enc28j80_write_control_res(BANK_0, EWRPTL, 0);
	enc28j80_write_control_res(BANK_0, EWRPTH, 0);
	
	enc28j80_write_control_res(BANK_0, ERDPTL, RX_START_ADD%256);
	enc28j80_write_control_res(BANK_0, ERDPTH, RX_START_ADD/256);

	
#ifdef DEBUG
	uint8_t h,l= 0 ;
	
	printf("Tx space {%04x}:{%04x}\r\n",TX_START_ADD, TX_STOP_ADD);
	printf("Rx space {%04x}:{%04x}\r\n",RX_START_ADD, RX_STOP_ADD);
	
	l = enc28j80_read_control_res(BANK_0, EWRPTL);
	h = enc28j80_read_control_res(BANK_0, EWRPTH);
	printf("Tx pointer {%04x}\r\n",h*256+l);
	
	l = enc28j80_read_control_res(BANK_0, ERDPTL);
	h = enc28j80_read_control_res(BANK_0, ERDPTH);
	printf("Rx pointer {%04x}\r\n",h*256+l);
#endif

}

extern void enc28j60_poll(void){
	HAL_Delay(500);
}



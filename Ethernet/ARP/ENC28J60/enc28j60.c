#include <stdio.h>
#include "config.h"
#include "binary.h"
#include "enc28j60.h"
#include "crc32.h"

volatile BANK enc28j60_bank = BANK_NONE;
extern void enc28j60_set_bank(BANK bank){
	if(bank != enc28j60_bank){
		enc28j60_bank = bank;
		enc28j60_write_cmd(BFC, ECON1, B11);
		enc28j60_write_cmd(BFS, ECON1, bank);
	}
}

extern uint8_t enc28j60_read_control_res(BANK bank,uint8_t address){
	uint8_t data;
	enc28j60_set_bank(bank);
	data = enc28j60_read_cmd(RCR,address);
	return data;
}

extern void enc28j60_write_control_res(BANK bank,uint8_t address,uint8_t data){
	enc28j60_set_bank(bank);
	enc28j60_write_cmd(WCR,address,data);
}

extern uint8_t enc28j60_init(uint8_t MAC[]){
	
#ifdef DEBUG
	printf("Start init ...\r\n");
#endif
	uint8_t i = 0;
	uint8_t status = 0;
	int16_t timeout = 1000;
	uint8_t MY_MAC[6]= {0};
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
	if (timeout == -1){
		printf("Time out \r\n");
		return false;
	}
	else{
		printf("Finish 0x%02x\r\n",status);
	}
#endif

	/* Set address rx, tx buff */
	enc28j60_write_control_res(BANK_0, ETXSTL, TX_START_ADD%256);
	enc28j60_write_control_res(BANK_0, ETXSTH, TX_START_ADD/256);
	
	enc28j60_write_control_res(BANK_0, ETXNDL, TX_STOP_ADD%256);
	enc28j60_write_control_res(BANK_0, ETXNDH, TX_STOP_ADD/256);
	
	enc28j60_write_control_res(BANK_0, ERXSTL, RX_START_ADD%256);
	enc28j60_write_control_res(BANK_0, ERXSTH, RX_START_ADD/256);
	
	enc28j60_write_control_res(BANK_0, ERXNDL, RX_STOP_ADD%256);
	enc28j60_write_control_res(BANK_0, ERXNDH, RX_STOP_ADD/256);
	
	/* Reset address rx, tx buff to 0*/
	enc28j60_write_control_res(BANK_0, EWRPTL, TX_START_ADD%256);
	enc28j60_write_control_res(BANK_0, EWRPTH, TX_START_ADD>>8);
	
	enc28j60_write_control_res(BANK_0, ERDPTL, RX_START_ADD%256);
	enc28j60_write_control_res(BANK_0, ERDPTH, RX_START_ADD/256);

#ifdef DEBUG
	uint8_t h,l= 0 ;
	
	printf("Tx space 0x%04x:0x%04x\r\n",TX_START_ADD, TX_STOP_ADD);
	printf("Rx space 0x%04x:0x%04x\r\n",RX_START_ADD, RX_STOP_ADD);
	
	l = enc28j60_read_control_res(BANK_0, EWRPTL);
	h = enc28j60_read_control_res(BANK_0, EWRPTH);
	printf("Tx pointer 0x%04x\r\n",h*256+l);
	
	l = enc28j60_read_control_res(BANK_0, ERDPTL);
	h = enc28j60_read_control_res(BANK_0, ERDPTH);
	printf("Rx pointer 0x%04x\r\n",h*256+l);
#endif
	/* MAC configuration P34*/
	/* 1 */
	enc28j60_write_control_res(BANK_2, MACON1, (1<<TXPAUS)|(1<<RXPAUS)|(1<<MARXEN));
 	/* 2 */
	enc28j60_write_control_res(BANK_2, MACON3, (B101<<PADCFG0)|(1<<TXCRCEN)|(1<<FRMLNEN)|(1<<FULDPX));
	/* 3 */
	/* 4 : 1518 */
	enc28j60_write_control_res(BANK_2, MAMXFL, 1518%256);
	enc28j60_write_control_res(BANK_2, MAMXFH, 1518/256);
	/* 5 : 15h */
	enc28j60_write_control_res(BANK_2, MABBIPG, 0x15);
	/* 6 : 12h */
	enc28j60_write_control_res(BANK_2, MAIPGL, 0x12);
	/* 7,8 */
	/* 9. MAC address*/
	enc28j60_write_control_res(BANK_3, MAADR1, MAC[0]);
	enc28j60_write_control_res(BANK_3, MAADR2, MAC[1]);
	enc28j60_write_control_res(BANK_3, MAADR3, MAC[2]);
	enc28j60_write_control_res(BANK_3, MAADR4, MAC[3]);
	enc28j60_write_control_res(BANK_3, MAADR5, MAC[4]);
	enc28j60_write_control_res(BANK_3, MAADR6, MAC[5]);

	MY_MAC[0] = enc28j60_read_control_res(BANK_3, MAADR1);
	MY_MAC[1] = enc28j60_read_control_res(BANK_3, MAADR2);
	MY_MAC[2] = enc28j60_read_control_res(BANK_3, MAADR3);
	MY_MAC[3] = enc28j60_read_control_res(BANK_3, MAADR4);
	MY_MAC[4] = enc28j60_read_control_res(BANK_3, MAADR5);
	MY_MAC[5] = enc28j60_read_control_res(BANK_3, MAADR6);
	
#ifdef DEBUG
	printf("MAC : 0x%02x-0x%02x-0x%02x-0x%02x-0x%02x-0x%02x\r\n",MY_MAC[0],MY_MAC[1],MY_MAC[2],MY_MAC[3],MY_MAC[4],MY_MAC[5]);
#endif
	for (i=0;i<6;i++){
		if (MY_MAC[i] != MAC[i])
			return false;
	}

	/* 	PHY config
			To check PHY config, you can read a phy config address is 0x14h.
		Because the reset value is 
	*/
	enc28j60_write_phy(PHCON1,(1<<PDPXMD));
	/* Config LED */
	enc28j60_write_phy(PHLCON, (1<<STRCH)|(B01<<LFRQ0)|(B0100<<LACFG0)|(B0011<<LBCFG0));
	/* Setup interrupt */
	enc28j60_write_cmd(BFC, EIE, (1<<INTIE)|(1<<PKTIE));
	enc28j60_write_cmd(BFS, EIE, (1<<INTIE)|(1<<PKTIE));
	/* Setup clock */
	enc28j60_write_control_res(BANK_3, ECOCON, (1<<COCON1));
	/* Receive enable */
	enc28j60_write_cmd(BFS, ECON1, (1<<RXEN));
	/* Increment address when write,read */
	enc28j60_write_cmd(BFS, ECON2, (1<<AUTOINC));
	
#ifdef DEBUG
	printf("Finish !\r\n");
#endif
	return 1;
}

/* Read PHY config P19 */
extern uint16_t enc28j60_read_phy(uint8_t address){
	uint8_t status_h = 0;
	uint8_t status_l = 0;
	int16_t timeout = 1000;
	/* 1. Write address of the PHY register */
	enc28j60_write_control_res(BANK_2, MIREGADR, address);
	/* 2.1 Set the MICMD.MIIRD bit */
	enc28j60_write_control_res(BANK_2, MICMD, (1<<MIISCAN));
	enc28j60_write_control_res(BANK_2, MICMD, (1<<MIISCAN)|(1<<MIIRD));
	/* 2.2 Wait 10.24 µs. Read the MISTAT.BUSY bit */
	delay_ms(1);
	do{
		status_h = enc28j60_read_control_res(BANK_3, MISTAT);
		if (status_h&(1<<BUSY)){
			break;
		}
	}while(timeout--);
	
	if (timeout == -1){
		#ifdef DEBUG
				printf("Time out {%d}-{%02x}\r\n",timeout,status_h);
		#endif
		return 0xFFFF;
	}
	#ifdef DEBUG
	printf("MISTAT : 0x%02x {%d}\r\n",status_h, timeout);
	#endif
	/* 4. Clear the MICMD.MIIRD bit */
	enc28j60_write_control_res(BANK_2, MICMD, 0);
	/* 5. Read the desired data from the MIRDL and MIRDH registers */
	status_l = enc28j60_read_control_res(BANK_2, MIRDL);
	status_h = enc28j60_read_control_res(BANK_2, MIRDH);
	
	#ifdef DEBUG
	printf("MIRDL, MIRDH : 0x%02x-0x%02x\r\n",status_l, status_h);
	#endif
	
	return ((status_h<<8) + status_l);
}

/* Write PHY config P19 */
extern bool enc28j60_write_phy(uint8_t address,uint16_t data){
	uint8_t status = 0;
	int16_t timeout = 1000;
	/* 1. Write address of the PHY register */
	enc28j60_write_control_res(BANK_2, MIREGADR, address);
	/* 2. Write the lower 8 to write into the MIWRL */
	enc28j60_write_control_res(BANK_2, MIWRL, data%256);
	/* 3. Write the lower 8 to write into the MIWRH */
	enc28j60_write_control_res(BANK_2, MIWRH, data/256);
	do{
		status = enc28j60_read_control_res(BANK_3, MISTAT);
		if (status&(1<<BUSY)){
			break;
		}
	}while(timeout--);
	
	if (timeout == -1){
		return false;
	}
	return true;
}

extern void enc28j60_write_buffs(uint8_t* data,uint16_t len){
	uint16_t i = 0;
	uint32_t crc = crc32(&(data[14]), 28);
	enc28j60_CS_low();
	enc28j60_spi_write(0x7A);
	enc28j60_spi_write(0x00);
	for(i=0;i<len;i++){
			enc28j60_spi_write(*data++);
	}
	enc28j60_spi_write((crc&0xFF000000)>>24);
	enc28j60_spi_write((crc&0x00FF0000)>>16);
	enc28j60_spi_write((crc&0x0000FF00)>>8);
	enc28j60_spi_write(crc&0x000000FF);
	enc28j60_CS_high();
}

extern bool enc28j60_send_packet(uint8_t *data, uint16_t len){
	#ifdef DEBUG
		printf("Send .....\r\n");
	#endif
	int32_t timeout = 10000;
	uint8_t status = 0;
	/* Calculate CRC */
	/* 1. Appropriately program the ETXST Pointer */
	enc28j60_write_control_res(BANK_0, ETXSTL, TX_START_ADD%256);
	enc28j60_write_control_res(BANK_0, ETXSTH, TX_START_ADD>>8);
	/* 2. Write data into buffer */
	enc28j60_write_buffs(data, len);
	/* 3. Appropriately program the ETXND Pointer */
	enc28j60_write_control_res(BANK_0, ETXNDL, (TX_START_ADD+len+4)%256);
	enc28j60_write_control_res(BANK_0, ETXNDH, (TX_START_ADD+len+4)>>8);
	/* Reset start address write */
	enc28j60_write_control_res(BANK_0, EWRPTL, TX_START_ADD%256);
	enc28j60_write_control_res(BANK_0, EWRPTH, TX_START_ADD>>8);
	/* 3,4 */
	/* 5. Start the transmission */
	enc28j60_write_cmd(BFS, ECON1, (1<<TXRTS));
	do{
		status = enc28j60_read_control_res(BANK_0, ECON1);
		// when the packet is finished or was aborted, the ECON1.TXRTS will be cleaned
		if((status&(1<<TXRTS)) == 0){
			break;
		}
//		status = enc28j60_read_control_res(BANK_0, ESTAT);
	}while(timeout--);
	if(timeout == -1){
		#ifdef DEBUG
			printf("Timeout %2d 0x%02x\r\n",timeout, status);
		#endif
		return false;
	}
	#ifdef DEBUG
		printf("Finished ! \r\n");
	#endif
	return true;
}

/*
	08 10 19 97 25 25
  28 d2 44 cc 71 41
  08 06
  00 01
	08 00
  06 04
  00 02
  28 d2 44 cc 71 41
  c0 a8 89 0a
	08 10 19 97 25 25
  c0 a8 89 64
*/

uint8_t mac_device[6] = {0};

bool check_arp(arp_frame arp, uint8_t* crc){

	/* Check mac of device*/
	if ((arp.rx[0] == 0x08)&&(arp.rx[1] == 0x10)&&(arp.rx[2] == 0x19) &&
			(arp.rx[3] == 0x97)&&(arp.rx[4] == 0x25)&&(arp.rx[5] == 0x25)){
				
			/* Check arp: 0x0806 */
			if ((arp.rx[12] == 0x08)&&(arp.rx[13] == 0x06)){
				/* Hardware Type */
				if ((arp.rx[14] == 0x00)&&(arp.rx[15] == 0x01)){
					/* Protocol type */
					if ((arp.rx[16] == 0x80)&&(arp.rx[17] == 0x00)){
						/* protocol size, hardware size */
						if ((arp.rx[18] == 0x06)&&(arp.rx[19] == 0x04)){
							/* Opcode */
							if ((arp.rx[20] == 0x00)&&(arp.rx[21] == 0x02)){
								/* Check sender ip address */
								if ((arp.rx[28] == 192)&&(arp.rx[29] == 168)&&(arp.rx[30] == 137)&&(arp.rx[31] == 10)){
										/* Check target mac address */
										if ((arp.rx[32] == 0x08)&&(arp.rx[33] == 0x10)&&(arp.rx[34] == 0x19) &&
											(arp.rx[35] == 0x97)&&(arp.rx[36] == 0x25)&&(arp.rx[37] == 0x25)){
												/* Check target ip address */
												if ((arp.rx[38] == 192)&&(arp.rx[39] == 168)&&(arp.rx[40] == 137)&&(arp.rx[41] == 100)){
													
													mac_device[0] = arp.rx[22];
													mac_device[1] = arp.rx[23];
													mac_device[2] = arp.rx[24];
													mac_device[3] = arp.rx[25];
													mac_device[4] = arp.rx[26];
													mac_device[5] = arp.rx[27];
												
													#ifdef DEBUG
														printf("Crc: %02x %02x %02x %02x \r\n",arp.rx[42],arp.rx[43],arp.rx[44],arp.rx[45]);
														printf("Crc: %02x %02x %02x %02x \r\n",crc[0],crc[1],crc[2],crc[3]);
														printf("Receiver: %02x %02x %02x %02x %02x %02x\r\n",mac_device[0], mac_device[1], mac_device[2], mac_device[3], mac_device[4], mac_device[5]);
													#endif
													return true;
												}
										}
								}
						}
					}
				}
			}
		}
	}
	return false;
}

arp_frame arp_data;

extern bool enc28j60_receive_packet(void){
	uint16_t i =0;
	if( enc28j60_read_control_res(BANK_1,EPKTCNT) == 0 )
	{
		return false;
	}
	// Set the read pointer to the start of the received packet
	enc28j60_write_control_res(BANK_0,ERDPTL,arp_data.pointer_rx/256);
	enc28j60_write_control_res(BANK_0,ERDPTH,arp_data.pointer_rx>>8);
	// read the next packet pointer
	arp_data.pointer_rx = enc28j60_read_cmd(RBM, 0);
	arp_data.pointer_rx |= (enc28j60_read_cmd(RBM, 0)<<8);
	// read the packet length (see datasheet page 43)
	arp_data.len  = enc28j60_read_cmd(RBM, 0);
	arp_data.len |= (enc28j60_read_cmd(RBM, 0)<<8);
	arp_data.len -=4;
	
	arp_data.status = enc28j60_read_cmd(RBM, 0);
	arp_data.status |= (enc28j60_read_cmd(RBM, 0)<<8);
	
	if ((arp_data.status & 0x80)==0){
		arp_data.len = 0;
	}
	else{
		for(i=0;i<arp_data.len;i++){
			arp_data.rx[i] = enc28j60_read_cmd(RBM, 0);
		}
		for(i=0;i<4;i++){
			arp_data.crc[i] = enc28j60_read_cmd(RBM, 0);
		}
		mac_device[0] = arp_data.rx[22];
		mac_device[1] = arp_data.rx[23];
		mac_device[2] = arp_data.rx[24];
		mac_device[3] = arp_data.rx[25];
		mac_device[4] = arp_data.rx[26];
		mac_device[5] = arp_data.rx[27];
		#ifdef DEBUG
		printf("Receiver: %02x %02x %02x %02x %02x %02x\r\n",mac_device[0], mac_device[1], mac_device[2], mac_device[3], mac_device[4], mac_device[5]);
		#endif
		return true;
	}
	return true;
}

extern void enc28j60_poll(void){
}

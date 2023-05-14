#include "main.h"
#include "net.h"

u16 source_port = 80;
u08 mac_addr[6] = {0};
u08 ip_addr[4] = {0};

u08 mac_pc[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
u08 ip_pc[4] = {0};

extern void net_init(u08 mymac[6], u08 myip[4], u16 myport, u08 ip_of_pc[4]){
/* 
	u08 debug = 0xFF;
	enc28j60Init(mymac);  
	debug = enc28j60getrev();
 */
	/* INIT ENC28J60 */
	enc28j60Init(mymac);  
	enc28j60getrev();
	enc28j60PhyWrite(PHLCON, 0x476);
	/* change clkout from 6.25MHz to 12.5MHz */
	enc28j60clkout(2);
	/* INIT NET */
	source_port = myport;
	copy_arr(mac_addr, mymac, 6);
	copy_arr(ip_addr, myip, 4);
	copy_arr(ip_pc, ip_of_pc, 4);
	net_arp_get_mac_ip_pc(mac_pc, ip_pc, 500);
	
	// enable interrutps
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
}

extern bool net_poll(void){
	static u08 rx_buf[BUFFER_SIZE+1] = {0};
	static u16 plen = 0;

	ProtocolIP protocol = NONE;
	plen = 0;
	plen = enc28j60PacketReceive(BUFFER_SIZE, (u08*)rx_buf);
	if (plen == 0){ 
		return false;
	}
	else{
		protocol = NONE;
		rx_buf[plen] = '\0';
		if(rx_buf[I_ARP_ETHERNET_TYPE_H] == 0x08 && \
			rx_buf[I_ARP_ETHERNET_TYPE_L] == 0x06){	
			protocol = ARP;
		}
		else if(rx_buf[I_IPV4_PROTOCOL] == IPV4_PROTOCOL_ICMP){
			protocol = ICMP;
		}
		else if(rx_buf[I_IPV4_PROTOCOL] == IPV4_PROTOCOL_TCP){
			protocol = TCP_IP;
		}
		switch (protocol){
			case ARP:{
				if (net_arp_check_broadcast((u08*)rx_buf, plen)){
					copy_arr(mac_pc, &rx_buf[I_ARP_MAC_SENDER], 6);
					copy_arr(ip_pc, &rx_buf[I_ARP_IP_SENDER], 4);
					
					net_arp_reply((u08*)rx_buf, plen);
					return true;
				}
				break;
			}
			case ICMP:{
				if (net_icmp_check((u08*)rx_buf, plen)){
					net_icmp_reply((u08*)rx_buf, plen);
					return true;
				}
				break;
			}
			case TCP_IP:{
				if (net_tcp_ip_check((u08*)rx_buf, plen)){
					if(net_tcp_ip_reply((u08*)rx_buf, plen)){
						net_tcp_ip_handle((u08*)rx_buf, plen);
						return true;
					}
				}
				break;
			}
			default:
				break;
		}
		return false;
	}
}

extern bool net_arp_check_broadcast(u08* ping, u16 len){
	if (len<41){
		return false;
	}
	u08 mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

	if(!com_arr(&ping[I_ARP_MAC_DEST], mac, 6)){
		return false;
	}
	if(!com_arr(&ping[I_ARP_IP_TARGET], ip_addr, 4)){
		return false;
	}
	/* check  protocol IPv4 - 0x0800*/
	if(!(ping[I_ARP_PROTOCOL_TYPE_H] == 0x08 && \
		ping[I_ARP_PROTOCOL_TYPE_L] == 0x00)){
		return false;
	}
	/* check  protocol IPv4 - 0x0806*/
	if(!(ping[I_ARP_ETHERNET_TYPE_H] == 0x08 && \
		ping[I_ARP_ETHERNET_TYPE_L] == 0x06)){
			return false;
	}
	return true;
}

extern void net_arp_reply(u08* data, u16 len){
	ARP_Frame arp_data;
	copy_arr(arp_data.MAC_dest, &data[I_ARP_MAC_SOURCE], 6);
	copy_arr(arp_data.MAC_source, mac_addr, 6);
	arp_data.Ethernet_type = swap16(ARP_ETHERNET_TYPE);
	arp_data.Hardwave_type = swap16(ARP_HARDWAVE_TYPE);
	arp_data.Protocol_type = swap16(ARP_PROTOCOL_TYPE);
	arp_data.Size = swap16(ARP_SIZE);
	arp_data.Opcode = swap16(ARP_OPCODE_REPLY);
	copy_arr(arp_data.MAC_sender, mac_addr, 6);
	copy_arr(arp_data.IP_sender, ip_addr, 4);
	copy_arr(arp_data.MAC_target, &data[I_ARP_MAC_SENDER], 6);
	copy_arr(arp_data.IP_target, &data[I_ARP_IP_SENDER], 4);
	enc28j60PacketSend(ARP_PACKET_LEN, (u08*)&arp_data);
}

extern bool net_arp_get_mac_ip_pc(u08 mac_target[6], u08 ip_target[4], u16 timeout){
	u08 data[100] = {0};
	u16 len = 0;
	ARP_Frame arp_struct;
	
	copy_arr(arp_struct.MAC_dest, mac_target, 6);
	copy_arr(arp_struct.MAC_source, mac_addr, 6);
	
	arp_struct.Ethernet_type = swap16(ARP_ETHERNET_TYPE);
	arp_struct.Hardwave_type = swap16(ARP_HARDWAVE_TYPE);
	arp_struct.Protocol_type = swap16(ARP_PROTOCOL_TYPE);
	arp_struct.Size = swap16(ARP_SIZE);
	arp_struct.Opcode = swap16(ARP_OPCODE_REQUEST);
	
	copy_arr(arp_struct.MAC_sender, mac_addr, 6);
	copy_arr(arp_struct.IP_sender, ip_addr, 4);
	
	mac_target[0] = 0x00;
	mac_target[1] = 0x00;
	mac_target[2] = 0x00;
	
	mac_target[3] = 0x00;
	mac_target[4] = 0x00;
	mac_target[5] = 0x00;

	copy_arr(arp_struct.MAC_target, mac_target, 6);
	copy_arr(arp_struct.IP_target, ip_target, 4);

	while(timeout--){
		enc28j60PacketSend(ARP_PACKET_LEN, (u08 *)&arp_struct);
		delay_ms(200);
		len = enc28j60PacketReceive(BUFFER_SIZE, data); 
		if (len >= 42){
			/* ARP OPCODE request */
			u16 arp_opcode = (data[I_ARP_OPCODE_H]<<8) + data[I_ARP_OPCODE_L];
			if(arp_opcode == ARP_OPCODE_REPLY){
				/* compare MAC source */
				if(com_arr(&data[I_ARP_MAC_TARGET], (u08*)mac_addr, 6)){
					/* compare IP source */
					if(com_arr(&data[I_ARP_IP_TARGET], (u08*)ip_addr, 4)){
						copy_arr(mac_target, &data[I_ARP_MAC_SENDER], 6);
#ifdef DEBUG
	printf("%02x %02x %02x %02x %02x %02x \n", mac_target[0], mac_target[1], mac_target[2], mac_target[3], mac_target[4], mac_target[5]);
#endif
						return true;
					}
				}
			}
		}
		if (timeout == 0){
			break;
		}
	}
	return false;
}

extern bool net_icmp_check(u08* ping, u16 len){
	if (len<41){
		return false;
	}
	/* compare mac addr */
	if (!com_arr(&ping[I_IPV4_MAC_SOURCE], mac_addr, 6)){
			return false;
	}
	/* compare mac pc */
	if (!com_arr(&ping[I_IPV4_MAC_DEST], mac_pc, 6)){
			return false;
	}
	/* compare ip pc */
	if (!com_arr(&ping[I_IPV4_SOURCE_IP], ip_pc, 4)){
			return false;
	}
	/* compare ip addr */
	if (!com_arr(&ping[I_IPV4_DEST_IP], ip_addr, 4)){
			return false;
	}
	/* compare ICMP request */
	if (!(ping[I_ICMP_TYPE] == 0x08)){
			return false;
	}
	/* check crc */
	uint16_t real_crc, crc = 0;
	real_crc = ping[I_ICMP_CHECKSUM_H] + (ping[I_ICMP_CHECKSUM_L]<<8);
	ping[I_ICMP_CHECKSUM_H] = 0;
	ping[I_ICMP_CHECKSUM_L] = 0;
	crc = icmp_checksum(&ping[I_ICMP_TYPE], ICMP_SIZE+ len-IPV4_ICMP_SIZE);
	if(real_crc != crc){
		return false;
	}
	/* TCP protocol : 0x0800 */
	if(ping[I_IPV4_ETHERNET_TYPE_H] == IPV4_ETHERNET_TYPE_H && \
	 ping[I_IPV4_ETHERNET_TYPE_L] != IPV4_ETHERNET_TYPE_L){
		return false;
	}
	/* ICMP protocol : 0x01 */
	if (ping[I_IPV4_PROTOCOL] != IPV4_PROTOCOL_ICMP){
		return false;
	}
	return true;
}

extern void net_icmp_reply(u08* ping, u16 len){
	ICMP_Frame icmp_struct;
	copy_arr((u08*)&icmp_struct, ping, len);
	copy_arr(icmp_struct.MAC_dest, &ping[6], 6);
	copy_arr(icmp_struct.MAC_source,  &ping[0], 6);
	icmp_struct.CheckSum = 0x0000;
	icmp_struct.CheckSum = ipv4_checksum((u08 *)&icmp_struct.Header_length, IPV4_SIZE);
	copy_arr(icmp_struct.SourceIP, &ping[I_IPV4_DEST_IP], 4);
	copy_arr(icmp_struct.DestIP, &ping[I_IPV4_SOURCE_IP], 4);
	icmp_struct.ICMP_Type = ICMP_REPLY;
	icmp_struct.ICMP_Checksum = 0x0000;
	copy_arr(icmp_struct.ICMP_Data, &ping[IPV4_ICMP_SIZE], len-IPV4_ICMP_SIZE);
	icmp_struct.ICMP_Checksum = icmp_checksum((u08*)&(icmp_struct.ICMP_Type), ICMP_SIZE+ len-IPV4_ICMP_SIZE);
	enc28j60PacketSend(len, (u08*)&icmp_struct);
}

extern bool net_tcp_ip_check(u08* request, u16 len){
	if (len<41){
		return false;
	}
	/* compare mac addr */
	if (!com_arr(&request[I_IPV4_MAC_SOURCE], mac_addr, 6)){
			return false;
	}
	/* compare ip addr */
	if (!com_arr(&request[I_IPV4_DEST_IP], ip_addr, 4)){
			return false;
	}
	/* compare port source */
	u16 port = (request[I_TCP_DST_PORT_H]<<8) + request[I_TCP_DST_PORT_L];
	if (port != source_port){
			return false;
	}
	/* check  protocol IPv4 - 0x0800*/
	if(!(request[I_IPV4_ETHERNET_TYPE_H] == 0x08 && \
	 request[I_IPV4_ETHERNET_TYPE_L] == 0x00)){
		return false;
	}
	/* TCP protocol : 0x06 */
	if (!(request[I_IPV4_PROTOCOL] == IPV4_PROTOCOL_TCP)){
		return false;
	}
	return true;
}

u32 seq_num_local, ack_num_local = 0;

extern bool net_tcp_ip_reply(u08* request, u16 len){
	u32 total_len = 0;
	u32 seq_num, ack_num = 0;
	u08 flags = request[I_TCP_FLAGS_L];
	TCP_Frame tcp_struct;
	copy_arr((u08*)&tcp_struct, request, len);

	switch (flags){
		case TCP_FLAGS_SYN:{
			/* Check SYN */
			copy_arr(tcp_struct.MAC_dest, &request[6], 6);
			copy_arr(tcp_struct.MAC_source, &request[0], 6);
			tcp_struct.CheckSum = 0x0000;
			tcp_struct.CheckSum = ipv4_checksum((u08 *)&tcp_struct.Header_length, IPV4_SIZE);
			copy_arr(tcp_struct.SourceIP, &request[I_IPV4_DEST_IP], 4);
			copy_arr(tcp_struct.DestIP, &request[I_IPV4_SOURCE_IP], 4);
			tcp_struct.TCP_Source_Port = swap16((request[I_TCP_DST_PORT_H]<<8) + request[I_TCP_DST_PORT_L]);
			tcp_struct.TCP_Dest_Port = swap16((request[I_TCP_SRC_PORT_H]<<8) + request[I_TCP_SRC_PORT_L]);
			
			seq_num =  swap32((request[I_TCP_SEQ_NUM]<<24) + (request[I_TCP_SEQ_NUM+1]<<16) + \
			(request[I_TCP_SEQ_NUM+2]<<8) + request[I_TCP_SEQ_NUM+3] + 1);
			ack_num = swap32(0x08101997);

			copy_arr(tcp_struct.TCP_Seq_Number, (u08*)&ack_num, 4);
			copy_arr(tcp_struct.TCP_Ack_Number, (u08*)&seq_num, 4);
			
			tcp_struct.TCP_Flags = TCP_FLAGS_SYN|TCP_FLAGS_ACK;
			total_len = (request[I_IPV4_TOTAL_LENGTH_H]<<8) + request[I_IPV4_TOTAL_LENGTH_L];
			tcp_struct.TCP_Checksums = 0x0000;
			tcp_struct.TCP_Checksums = tcp_checksum((u08*)tcp_struct.SourceIP, total_len - 20 +8);
			enc28j60PacketSend(len, (u08*)&tcp_struct);
			
			return false;
		}
		case TCP_FLAGS_ACK:{
			/* Check ACK */
			return false;
		}
		case (TCP_FLAGS_FIN|TCP_FLAGS_ACK):{
			/* Check fin */
			copy_arr(tcp_struct.MAC_dest, &request[6], 6);
			copy_arr(tcp_struct.MAC_source, &request[0], 6);
			tcp_struct.CheckSum = 0x0000;
			tcp_struct.CheckSum = ipv4_checksum((u08 *)&tcp_struct.Header_length, IPV4_SIZE);
			copy_arr(tcp_struct.SourceIP, &request[I_IPV4_DEST_IP], 4);
			copy_arr(tcp_struct.DestIP, &request[I_IPV4_SOURCE_IP], 4);
			tcp_struct.TCP_Source_Port = swap16((request[I_TCP_DST_PORT_H]<<8) + request[I_TCP_DST_PORT_L]);
			tcp_struct.TCP_Dest_Port = swap16((request[I_TCP_SRC_PORT_H]<<8) + request[I_TCP_SRC_PORT_L]);
			
			seq_num =  swap32((request[I_TCP_SEQ_NUM]<<24) + (request[I_TCP_SEQ_NUM+1]<<16) + \
			(request[I_TCP_SEQ_NUM+2]<<8) + request[I_TCP_SEQ_NUM+3] + 1);
			ack_num = swap32((request[I_TCP_ACK_NUM]<<24) + (request[I_TCP_ACK_NUM+1]<<16) + \
			(request[I_TCP_ACK_NUM+2]<<8) + request[I_TCP_ACK_NUM+3]);

			copy_arr(tcp_struct.TCP_Seq_Number, (u08*)&ack_num, 4);
			copy_arr(tcp_struct.TCP_Ack_Number, (u08*)&seq_num, 4);
			
			tcp_struct.TCP_Flags = TCP_FLAGS_ACK;
			total_len = (request[I_IPV4_TOTAL_LENGTH_H]<<8) + request[I_IPV4_TOTAL_LENGTH_L];
			tcp_struct.TCP_Checksums = 0x0000;
			tcp_struct.TCP_Checksums = tcp_checksum((u08*)tcp_struct.SourceIP, total_len - 20 +8);
			enc28j60PacketSend(len, (u08*)&tcp_struct);
			
			tcp_struct.TCP_Flags = TCP_FLAGS_FIN|TCP_FLAGS_ACK;
			
			tcp_struct.TCP_Checksums = 0x0000;
			tcp_struct.TCP_Checksums = tcp_checksum((u08*)tcp_struct.SourceIP, total_len - 20 +8);
			enc28j60PacketSend(len, (u08*)&tcp_struct);
			break;
		}
		
		case TCP_FLAGS_ACK|TCP_FLAGS_PUSH:{
			/* Check PUSH */
			u16 datalength;
			copy_arr((u08*)&tcp_struct, request, len);
			datalength= swap16(tcp_struct.TotalLength) -20 - (tcp_struct.TCP_Data_Offset >> 2);
			copy_arr(tcp_struct.MAC_dest, &request[I_ARP_MAC_SOURCE], 6);
			copy_arr(tcp_struct.MAC_source, &request[I_ARP_MAC_DEST], 6);
			tcp_struct.TotalLength = swap16(TCP_DATA_SIZE - ETHERNET_II_SIZE);
			tcp_struct.CheckSum = 0x0000;
			tcp_struct.CheckSum = ipv4_checksum((u08 *)&tcp_struct.Header_length, IPV4_SIZE);
			copy_arr(tcp_struct.SourceIP, &request[I_IPV4_DEST_IP], 4);
			copy_arr(tcp_struct.DestIP, &request[I_IPV4_SOURCE_IP], 4);
			tcp_struct.TCP_Source_Port = swap16((request[I_TCP_DST_PORT_H]<<8) + request[I_TCP_DST_PORT_L]);
			tcp_struct.TCP_Dest_Port = swap16((request[I_TCP_SRC_PORT_H]<<8) + request[I_TCP_SRC_PORT_L]);
			
			seq_num = swap32((request[I_TCP_SEQ_NUM]<<24) + (request[I_TCP_SEQ_NUM+1]<<16) + \
			(request[I_TCP_SEQ_NUM+2]<<8) + request[I_TCP_SEQ_NUM+3] + datalength);
			ack_num = swap32((request[I_TCP_ACK_NUM]<<24) + (request[I_TCP_ACK_NUM+1]<<16) + \
			(request[I_TCP_ACK_NUM+2]<<8) + request[I_TCP_ACK_NUM+3]);
			 
			copy_arr(tcp_struct.TCP_Seq_Number, (u08*)&ack_num, 4);
			copy_arr(tcp_struct.TCP_Ack_Number, (u08*)&seq_num, 4);

			tcp_struct.TCP_Flags = TCP_FLAGS_ACK;
			tcp_struct.TCP_Checksums = 0x0000;
			tcp_struct.TCP_Checksums = tcp_checksum((u08*)tcp_struct.SourceIP, swap16(tcp_struct.TotalLength) - 20 + 8);
			enc28j60PacketSend(TCP_DATA_SIZE , (u08*)&tcp_struct);
			
			return true;
		}
	}
	return false;
}

extern void net_tcp_ip_response(u08* request, u16 len, u08* data, u16 len_of_data){
	/* ACK */
	u32 seq_num, ack_num = 0;
	u16 datalength;
	
	TCP_Frame tcp_struct;
	copy_arr((u08*)&tcp_struct, request, len);
	copy_arr((u08*)&tcp_struct.TCP_Data[0] , data, len_of_data);
	
	datalength = swap16(tcp_struct.TotalLength) -20 - (tcp_struct.TCP_Data_Offset >> 2);
	
	copy_arr(tcp_struct.MAC_dest, &request[I_ARP_MAC_SOURCE], 6);
	copy_arr(tcp_struct.MAC_source, &request[I_ARP_MAC_DEST], 6);
	tcp_struct.TotalLength = swap16(TCP_DATA_SIZE + len_of_data - ETHERNET_II_SIZE);
	tcp_struct.CheckSum = 0x0000;
	tcp_struct.CheckSum = ipv4_checksum((u08 *)&tcp_struct.Header_length, IPV4_SIZE);
	copy_arr(tcp_struct.SourceIP, &request[I_IPV4_DEST_IP], 4);
	copy_arr(tcp_struct.DestIP, &request[I_IPV4_SOURCE_IP], 4);
	tcp_struct.TCP_Source_Port = swap16((request[I_TCP_DST_PORT_H]<<8) + request[I_TCP_DST_PORT_L]);
	tcp_struct.TCP_Dest_Port = swap16((request[I_TCP_SRC_PORT_H]<<8) + request[I_TCP_SRC_PORT_L]);
	
	seq_num = swap32((request[I_TCP_SEQ_NUM]<<24) + (request[I_TCP_SEQ_NUM+1]<<16) + \
	(request[I_TCP_SEQ_NUM+2]<<8) + request[I_TCP_SEQ_NUM+3] + datalength);
	ack_num = swap32((request[I_TCP_ACK_NUM]<<24) + (request[I_TCP_ACK_NUM+1]<<16) + \
	(request[I_TCP_ACK_NUM+2]<<8) + request[I_TCP_ACK_NUM+3]);
	 
	copy_arr(tcp_struct.TCP_Seq_Number, (u08*)&ack_num, 4);
	copy_arr(tcp_struct.TCP_Ack_Number, (u08*)&seq_num, 4);
	/* PUSH */
	tcp_struct.TCP_Flags = TCP_FLAGS_ACK|TCP_FLAGS_PUSH;
	tcp_struct.TCP_Checksums = 0x0000;
	tcp_struct.TCP_Checksums = tcp_checksum((u08*)tcp_struct.SourceIP,  swap16(tcp_struct.TotalLength) - 20 + 8);
	
	enc28j60PacketSend(TCP_DATA_SIZE + len_of_data, (u08*)&tcp_struct);
}

extern void net_tcp_ip_handle(u08* request, u16 len){
	u16 len_of_data = len - TCP_DATA_SIZE;
	if (com_arr(&request[I_TCP_DATA], (u08*)"LED13=HIGH\r\n", len_of_data)){
		net_tcp_ip_response(request, len, (u08*)"LED13=HIGH ok\r\n", 15);
	}
	else if (com_arr(&request[I_TCP_DATA], (u08*)"LED13=LOW\r\n", len_of_data)){
		net_tcp_ip_response(request, len, (u08*)"LED13=LOW ok\r\n", 14);
	}
	else{
		net_tcp_ip_response(request, len, (u08*)"test123", 7);
	}
}

extern bool net_check_enit(void){
	if(enc28j60Read(EIR)&EIR_PKTIF){
		enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR ,EIR, EIR_PKTIF);
		return true;
	}
	return false;
}

extern void net_ethercat_example(void){
	u08 buf[] = {
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x88,0xA4,0x4E,0x10, 
		0x0C,0x00,0x00,0x00,0x01,0x00,0x06,0x80, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x0B,0x02,0x00,0x08,0x01,0x00, 0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x0A, 
		0x00,0x00,0x10,0x01,0x00,0x08,0x80,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x08, 0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00, 
		0x07,0x00,0x00,0x00,0x30,0x01,0x02,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,
		0x05,0x10,0x00,0x00,0x80,0x00,0xF8,0x24, 0x42,0x8A,0x4F,0x08,0x00,0x00
	};
	copy_arr(&buf[0], mac_pc, 6);
	copy_arr(&buf[6],  mac_addr, 6);
	enc28j60PacketSend(110, buf);
}

extern void net_ethercat_example2(u08*data, u16 len_of_data){
	u16 dummy = 0;
	u08 buf[50] = {0};
	u16 total_len;

	// Ethernet Header
	copy_arr(&buf[0], mac_pc, 6);
	copy_arr(&buf[6],  mac_addr, 6);
	dummy =  0x88A4;
	buf[12] = (dummy>>8)&0xFF;
	buf[13] = dummy&0xFF;
	
	// EtherCAT Datagrams
	buf[16] = LRW;								// Cmd
	buf[17] = 0x0;								// Idx
	// Address : address for logic memory r/w
	buf[18]= 0x00;
	buf[19]= 0x00;
	buf[20]= 1000/256;
	buf[21]= 1000%256;
	// len-R-C-M
	//  Last indicator: More EtherCAT datagrams will follow
	dummy = 0;
	dummy = (len_of_data&0x07FF)|(B000<<12)|(1<<14)|(0<<15);
	buf[22] = dummy&0xFF;
	buf[23] = (dummy>>8)&0xFF;
	// IRQ
	dummy = 0;
	buf[24] = dummy&0xFF;
	buf[25] = (dummy>>8)&0xFF;
	copy_arr(&buf[26],  data, len_of_data);
	
	// Working Cnt
	dummy = 0;
	buf[27 + len_of_data] = (dummy>>8)&0xFF;
	buf[28 + len_of_data] = dummy&0xFF;
	
	// EtherCAT frame header
	total_len = 12 + len_of_data;
	dummy = (B0001<<12)|(0<<11) + (total_len);
	
	buf[14] = dummy&0xFF;
	buf[15] = (dummy>>8)&0xFF;
	
	enc28j60PacketSend(29 + len_of_data, buf);
}

extern u16 net_ethercat_load_datagram(u08* dest,u08 cmd, u08 idx, u32 add, u16 RCM, u16 IRQ, u16 workingcount, u08* data, u16 len){
	// Cmd
	dest[0] = cmd;
	// Idx
	dest[1] = idx;
	// Address : address for logic memory r/w
	dest[2]= (add&0x0000FFFF)%256;
	dest[3]= (add&0x0000FFFF)/256;
	dest[4]= ((add&0xFFFF0000)>>16)/256;
	dest[5]= ((add&0xFFFF0000)>>16)%256;
	// len-R-C-M
	//  Last indicator: More EtherCAT datagrams will follow
	RCM = RCM + (len&0x07FF);
	dest[6] = RCM&0xFF;
	dest[7] = (RCM>>8)&0xFF;
	// IRQ
	dest[8] = IRQ&0xFF;
	dest[9] = (IRQ>>8)&0xFF;
	copy_arr(&dest[10],  data, len);
	// Working Cnt
	dest[10 + len] = (workingcount>>8)&0xFF;
	dest[11 + len] = workingcount&0xFF;
	return 12 + len;
}

extern void net_ethercat_send(void){
	u16 dummy = 0;
	u08 buf[100] = {0};
	u16 index = 0;

	// Ethernet Header
	copy_arr(&buf[0], mac_pc, 6);
	copy_arr(&buf[6],  mac_addr, 6);
	dummy =  0x88A4;
	buf[12] = (dummy>>8)&0xFF;
	buf[13] = dummy&0xFF;
	// EtherCAT Datagrams
	index = 16;
	index += net_ethercat_load_datagram(&buf[index],LRW, 0x01, 1000, (1<<15)|(1<<14)|(B000<<12), 0, 0, (u08*)"nguyen",6);
	index += net_ethercat_load_datagram(&buf[index],LWR, 0x02, 2000, (0<<15)|(1<<14)|(B000<<12), 0, 1, (u08*)"khac",4);
	
	// EtherCAT frame header
	dummy = (B0001<<12)|(0<<11) + (index - 16);
	buf[14] = dummy&0xFF;
	buf[15] = (dummy>>8)&0xFF;

	enc28j60PacketSend(index, buf);
}

extern void net_udp_ethercat_send(void){
	u16 dummy = 0;
	u16 index = 0;
	u16 len_of_data = 0;
	u08 buf[100] = {0};
	
	index = 44;
	index += net_ethercat_load_datagram(&buf[index],LRW, 0x01, 1000, (1<<15)|(1<<14)|(B000<<12), 0, 0, (u08*)"nguyen",6);
	index += net_ethercat_load_datagram(&buf[index],LWR, 0x02, 2000, (0<<15)|(1<<14)|(B000<<12), 0, 0, (u08*)"khac",4);
	len_of_data = index - 42;
	
	// EtherCAT frame header
	dummy = (B0001<<12)|(0<<11)+(index-42);
	buf[42] = dummy&0xFF;
	buf[43] = (dummy>>8)&0xFF;
	
	/* Ethernet header */
	copy_arr(&buf[0], mac_pc, 6);
	copy_arr(&buf[6], mac_addr, 6);
	buf[12] = 0x0800/256;
	buf[13] = 0x0800%256;
	/* IP */
	buf[12] = 0x0800/256;
	buf[13] = 0x0800%256;
	/* IP */
	buf[14] = 0x45;
	buf[15] = 0;
	// total length
	buf[16] = (index-14)/256;
	buf[17] = (index-14)%256;
	// ip destination
	buf[18] = 0x1234/256;
	buf[19] = 0x1234%256;
	// flags
	buf[20] = 0x0000/256;
	buf[21] = 0x0000%256;
	// time to live
	buf[22] = 128;
	// UDP
	buf[23] = 17;
	// ip source
	copy_arr(&buf[26], ip_addr, 4);
	// ip destination
	copy_arr(&buf[30], ip_pc, 4);
	
	// checksum
	dummy = ipv4_checksum(&buf[14], IPV4_SIZE);
	buf[24] = dummy%256;
	buf[25] = dummy/256;

	// source port
	buf[34] = 5002/256;
	buf[35] = 5002%256;
	// destination port
	buf[36] = 0x88A4/256;
	buf[37] = 0x88A4%256;
	// udp lenght
	buf[38] = (UDP_SIZE + len_of_data)/256;
	buf[39] = (UDP_SIZE + len_of_data)%256;
	dummy = udp_checksum(&buf[26],UDP_SIZE + len_of_data + 8);
	buf[40] = dummy%256;
	buf[41] = dummy/256;
	
	printf("\n");
	for(u16 i=0;i<index;i++){
		if(i%16 == 0){
			printf("\n");
		}
		printf("%02x ",buf[i]);
	}
	enc28j60PacketSend(index, &buf[0]);	
}






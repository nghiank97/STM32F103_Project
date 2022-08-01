
#include "use.h"
#include "socket.h"
#include <stdio.h>
#include <string.h>

#define HTTP_SOCKET     0
#define PORT_TCPS		5000
#define DATA_BUF_SIZE   2048
uint8_t gDATABUF[DATA_BUF_SIZE];

wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},
                            .ip = {192, 168, 88, 207},
                            .sn = {255, 255, 255, 0},
                            .gw = {192, 168, 88, 1},
                            .dns = {0, 0, 0, 0},
                            .dhcp = NETINFO_STATIC };


void W5500_Select(void)
{
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
}

void W5500_Unselect(void)
{
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len)
{
    HAL_SPI_Receive(&hspi2, buff, len, HAL_MAX_DELAY);
}

void W5500_WriteBuff(uint8_t* buff, uint16_t len)
{
    HAL_SPI_Transmit(&hspi2, buff, len, HAL_MAX_DELAY);
}

uint8_t W5500_ReadByte(void)
{
    uint8_t byte;
    W5500_ReadBuff(&byte, sizeof(byte));
    return byte;
}

void W5500_WriteByte(uint8_t byte)
{
    W5500_WriteBuff(&byte, sizeof(byte));
}

uint8_t stat;
uint8_t reqnr;
char Message[128];

extern void setup(void){
	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);

	reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
    reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
    reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);

	uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2};

    wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);

    wizchip_setnetinfo(&gWIZNETINFO);

	ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
	HAL_Delay(1000);
}

extern void loop(void){
	  printf("Creating socket...\r\n");
  stat = socket(HTTP_SOCKET, Sn_MR_TCP, 80, 0);
  if(stat != HTTP_SOCKET){
	  printf("socket() failed, code = %d\r\n", stat);
  }
  else{
	  printf("Socket created, connecting...\r\n");
  }

  stat = listen(HTTP_SOCKET);
  if(stat != SOCK_OK){
	  printf("listen() failed, code = %d\r\n", stat);
  }
  else{
	  printf("listen() OK\r\n");
  }

  while(getSn_SR(HTTP_SOCKET) == SOCK_LISTEN)
  {
			HAL_Delay(2);
  }

  printf("Input connection\r\n");
  if(getSn_SR(HTTP_SOCKET) != SOCK_ESTABLISHED){
	  printf("Error socket status\r\n");
  }

  uint8_t rIP[4];
  getsockopt(HTTP_SOCKET, SO_DESTIP, rIP);
  printf("IP:  %d.%d.%d.%d\r\n", rIP[0], rIP[1], rIP[2], rIP[3]);

  sprintf(Message, "input connection nr - %d", reqnr);

  send(0, (uint8_t*)Message, strlen(Message));

  disconnect(HTTP_SOCKET);

  printf("Closing socket.\r\n");
  close(HTTP_SOCKET);
  HAL_Delay(1000);
  reqnr++;
}


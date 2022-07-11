
#include "pcf8574.h"

I2C_LCD::I2C_LCD(I2C_HandleTypeDef *_hi2c1,uint8_t _address,uint8_t _row,uint8_t _col,uint8_t a2,uint8_t a1,uint8_t a0){
	hi2c1 = _hi2c1;
	address = _address+a2*8+a1*4+a0*2;
	
	row = _row;
	col = _col;
}

void I2C_LCD::send_cmd(uint8_t cmd){
	uint8_t data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;
	data_t[1] = data_u|0x08;
	data_t[2] = data_l|0x0C;
	data_t[3] = data_l|0x08;
	HAL_I2C_Master_Transmit(hi2c1, address,(uint8_t *) data_t, 4, 100);
}

void I2C_LCD::cursor(uint8_t r,uint8_t c){
	if ((row==2)&&(col==16)){
		if (r==0)
        send_cmd(0x80+c);
    else
        send_cmd(0x80+0x40+c);

	}
	else if ((row==4)&&(col==16)){
    switch (r){
		case 0:{
			send_cmd(0x80+c);
			return;
			}
		case 1:{
			send_cmd(0x80+0x40+c);
			return;
			}
		case 2:{
			send_cmd(0x80+0x10+c);
			return;
			}
		case 3:{
			send_cmd(0x80+0x50+c);
			return;
			}
		}
	}
	else if ((row==4)&&(col==20)){
    switch (r){
		case 0:{
			send_cmd(0x80+c);
			return;
			}
		case 1:{
			send_cmd(0x80+0x40+c);
			return;
			}
		case 2:{
			send_cmd(0x80+0x14+c);
			return;
			}
		case 3:{
			send_cmd(0x80+0x54+c);
			return;
			}
		}
	}
}

uint8_t I2C_LCD::write(uint8_t data){
	uint8_t data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;
	data_t[1] = data_u|0x09;
	data_t[2] = data_l|0x0D;
	data_t[3] = data_l|0x09;
	HAL_I2C_Master_Transmit (hi2c1, address,(uint8_t *) data_t, 4, 100);
	return 1;
}

void I2C_LCD::init (void)
{
	HAL_Delay(50);
	send_cmd (0x33); /* set 4-bits interface */
	HAL_Delay(50);
	send_cmd (0x32);
	HAL_Delay(50);
	send_cmd (0x28); /* start to set LCD function */
	HAL_Delay(50);

	send_cmd (0x01); /* clear display */
	HAL_Delay(50);
	send_cmd (0x06); /* set entry mode */
	HAL_Delay(50);
	send_cmd (0x0c); /* set display to on */
	HAL_Delay(50);
}

uint8_t I2C_LCD::printXY(uint8_t r,uint8_t c,const char *data){
	cursor(r,c);
	return print(data);
}
uint8_t I2C_LCD::printXY(uint8_t r,uint8_t c,const char *data,unsigned int base){
	cursor(r,c);
	return print(data,base);
}
uint8_t I2C_LCD::printXY(uint8_t r,uint8_t c,unsigned char data, int base){
	cursor(r,c);
	return print(data,base);
}
uint8_t I2C_LCD::printXY(uint8_t r,uint8_t c,int data, int base){
	cursor(r,c);
	return print(data,base);
}
uint8_t I2C_LCD::printXY(uint8_t r,uint8_t c,unsigned int data, int base){
	cursor(r,c);
	return print(data,base);
}
uint8_t I2C_LCD::printXY(uint8_t r,uint8_t c,long data, int base){
	cursor(r,c);
	return print(data,base);
}
uint8_t I2C_LCD::printXY(uint8_t r,uint8_t c,unsigned long data, int base){
	cursor(r,c);
	return print(data,base);
}
uint8_t I2C_LCD::printXY(uint8_t r,uint8_t c,double data, int base){
	cursor(r,c);
	return print(data,base);
}

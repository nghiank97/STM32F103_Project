#include "spi.h"
#include "ads1220.h"

#define ads1220_cs_low()		spi1_cs_low()
#define ads1220_cs_high() 		spi1_cs_high()
#ifndef delay_ms
#define delay_ms(t)		HAL_Delay(t)
#endif

void delay_us(void)
{
	for(int i=0;i<100;i++);
}

void ads1220_write_reg(u08 address, u08 value){
    ads1220_cs_low();
    //delay_us;
    spi1_write(WREG|(address<<2));
    spi1_write(value);
    //delay_us;
    ads1220_cs_high();
}

u08 ads1220_read_reg(u08 address){
    u08 data;
    ads1220_cs_low();
    delay_us();
    spi1_write(RREG|(address<<2));
    data = spi1_transfer(SPI_MASTER_DUMMY);
    delay_us();
    ads1220_cs_high();
    return data;
}

void ads1220_begin(){

	u08 m_config_reg0;
	u08 m_config_reg1;
	u08 m_config_reg2;
	u08 m_config_reg3;

	spi1_init();

	ads1220_cs_high();
	ads1220_reset();
    delay_ms(10);

    /******************** Configuration Register 0 ********************/

    m_config_reg0 =
    		(0b0000<<4)|	// AINP=AIN0, AINN=AIN1
    		(0b101<<1)|		// Gain 32
			(0b0<<0);		// PGA enabled

    /******************** Configuration Register 1 ********************/

    m_config_reg1 =
    		(0b110<<5)|		// DR= 1000SPS
    		(0b00<<3)|		// Mode= Normal
			(0b1<<2)|		// Conv mode=continuous
			(0b0<<1)|		// Temp Sensor disabled
			(0b0<<0);		// Current Source off

    /******************** Configuration Register 2 ********************/

    m_config_reg2 =
    		(0b01<<6)|		// External reference selected using AIN0/REFP1 and AIN3/REFN1 inputs
    		(0b11<<4)|	  	// simultaneous 50-Hz and 60-Hz rejection, IDAC = 500 µA
			(0b0<<3)|
			(0b101<<0);

    /******************** Configuration Register 3 ********************/

    m_config_reg3 = 0x00;   		// IDAC1 disabled, IDAC2 disabled, DRDY pin only

    ads1220_write_reg( CONFIG_REG0_ADDRESS , m_config_reg0);delay_ms(1);
    ads1220_write_reg( CONFIG_REG1_ADDRESS , m_config_reg1);delay_ms(1);
    ads1220_write_reg( CONFIG_REG2_ADDRESS , m_config_reg2);delay_ms(1);
    ads1220_write_reg( CONFIG_REG3_ADDRESS , m_config_reg3);delay_ms(1);
    ads1220_start_conv();									delay_ms(1);
    ads1220_cs_low();										delay_ms(1);

    /***************************************************************************/

	// Enable PA3 CLock : input pullup
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRL &=~ 0x0000F000;
	GPIOA->CRL |= 0x000008000;
	// enable the AFIO clock
	RCC->APB2ENR |= (1<<0);
	// Configure EXTI1 line for PA3
	AFIO->EXTICR[0] &=~ (B0000<<12);
	// Disable the Mask on EXTI 3
	EXTI->IMR |= (1<<3);
	// Enable Falling Edge for PA3
	EXTI->FTSR |= (1<<3);
	/*
		NVIC EXTI Line 3 interrupt : 9
		9/4 = 2
		(9-4*2)= 1 -> (1)*8 = 8
	*/
	NVIC->IP[2] = (0<<8);
	/*
		NVIC EXTI Line 3
		9/32 = 0
		9-31*0 = 9
	*/
	NVIC->ISER[0] = (1<<9);
}

void ads1220_cmd(u08 data_in){
    ads1220_cs_low();
    delay_us();
    spi1_write(data_in);
    delay_us();
    ads1220_cs_high();
}

void ads1220_reset()
{
	ads1220_cmd(RESET);
}

void ads1220_start_conv()
{
	ads1220_cmd(START);
}

void ads1220_stop_conv()
{
	ads1220_cmd(STOP);
}

u08 ads1220_read_drdy(){
	return (u08)((GPIOA->IDR>>3)&0x01);
}

i32 ads1220_read_continuous(){
	u08 rx[3] = {0};
	i32 result = 0;

	rx[0] = spi1_transfer(0);
	rx[1] = spi1_transfer(0);
	rx[2] = spi1_transfer(0);

    result = rx[0];
    result = (result << 8) | rx[1];
    result = (result << 8) | rx[2];
    if (rx[0] & (1<<7)) {
        result |= 0xFF000000;
    }
    return result;
}


#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "base_display.h"

class I2C_LCD : public BaseDisplay{
	private:
		uint8_t address;
		I2C_HandleTypeDef *hi2c1;
		uint8_t row;
		uint8_t col;
			
	public:
		I2C_LCD(I2C_HandleTypeDef *_hi2c1,uint8_t _address = 64,uint8_t _row=2,uint8_t _col=16,uint8_t a2=0,uint8_t a1=0,uint8_t a0=0);
		void send_cmd(uint8_t cmd);
		uint8_t write(uint8_t) override;
		void cursor(uint8_t r,uint8_t c);
		void init(void);

	    uint8_t printXY(uint8_t ,uint8_t ,const char *);
	    uint8_t printXY(uint8_t ,uint8_t ,const char *,unsigned int);
	    uint8_t printXY(uint8_t ,uint8_t ,unsigned char, int = DEC);
	    uint8_t printXY(uint8_t ,uint8_t ,int, int = DEC);
	    uint8_t printXY(uint8_t ,uint8_t ,unsigned int, int = DEC);
	    uint8_t printXY(uint8_t ,uint8_t ,long, int = DEC);
	    uint8_t printXY(uint8_t ,uint8_t ,unsigned long, int = DEC);
	    uint8_t printXY(uint8_t ,uint8_t ,double, int = 2);
};

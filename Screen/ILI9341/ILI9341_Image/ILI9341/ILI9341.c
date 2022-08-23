#include "ILI9341.h"

_csx_low CSX_LOW;
_csx_high CSX_HIGH;

_rst_low RST_LOW;
_rst_high RST_HIGH;

_dcx_high DCX_HIGH;
_dcx_low DCX_LOW;

_rdx_high RDX_HIGH;
_rdx_low RDX_LOW;

_wdx_high WCX_HIGH;
_wdx_low WCX_LOW;


extern void ILI9341_setup_data(uint16_t data){
	
}

extern void ILI9341_write_command(ILI9341_COMMAND command){
	CSX_LOW();
	DCX_LOW();
	RDX_HIGH();
	WCX_HIGH();
	
	
	
	WCX_LOW();
}

extern void ILI9341_write_data(uint16_t data){
	
}

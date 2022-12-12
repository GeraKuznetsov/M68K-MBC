#include "sys.h"

void EINT(){
	__asm__("and.w  #0xF8FF, %sr ");
}

void DINT(){
	__asm__("or.w  #0x0700, %sr ");
}

//volatile u16* const IO_PORT = REG3;
volatile u16* const VGA_PORT = REG4;

volatile u8* const PS2R_SYSW = REG3 + 1; //read = SP2, write = sys register
volatile u8* const SPI_PORT = REG3 + 3; //read = SP2, write = sys register

volatile u8 sysPortValue = 0; 

void SYS_PORT_SET(u8 mask){
	*PS2R_SYSW = sysPortValue = sysPortValue | mask;
}

void SYS_PORT_RESET(u8 mask){
	*PS2R_SYSW = sysPortValue = sysPortValue & ~mask;
}

u8 READ_PS2_PORT(){
	return *PS2R_SYSW;
}

u8 SPI_SEND(u8 data){
	*SPI_PORT = data;
	__asm__("nop ");
	__asm__("nop ");
	__asm__("nop ");
	__asm__("nop ");
	return *SPI_PORT;
}
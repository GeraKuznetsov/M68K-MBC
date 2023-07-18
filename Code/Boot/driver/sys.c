#include "sys.h"
#define IO_PORT REG3

volatile u16* const VGA_PORT = (u16*) REG4;

volatile u8* const PS2R_SYSW = (u8*) (IO_PORT + (2 * 0) + 1); //read = PS2, write = sys out register
volatile u8* const SYSR_INTW = (u8*) (IO_PORT + (2 * 1) + 1); //read = sys in register, write = interupt register
volatile u8* const SPI_PORT = (u8*) (IO_PORT + (2 * 2) + 1); //read = SPI, write = SPI
volatile u8* const GPIO_PORT = (u8*) (IO_PORT + (2 * 3) + 1); //read = GPI, write = GPO

volatile u8 SYS_PORT_VALUE = 0; 
volatile u8 INTERRUPT_MASK_VALUE = 0; 

u8 READ_PS2_PORT(){
	return *PS2R_SYSW;
}

void INT_E_SET(u8 mask){
	INTERRUPT_MASK_VALUE = *SYSR_INTW = INTERRUPT_MASK_VALUE | mask;
}

void INT_E_RESET(u8 mask){
	*SYSR_INTW = INTERRUPT_MASK_VALUE = INTERRUPT_MASK_VALUE & ~mask;
}


/*u8 SYS_PORT_READ(){
	return *SYSR_INTW;
}*/

u8 spiSpeed;

u8 SPI_SET_SPEED(u8 spd){
	if(spiSpeed = spd){
		SYS_PORT_SET(SYS_OUT_PORT_SPI_SPEED);
	}else{
		SYS_PORT_RESET(SYS_OUT_PORT_SPI_SPEED);
	}
}

u8 SPI_SEND(u8 data){
	*SPI_PORT = data;
	return *SPI_PORT;
}


void GPO_WRITE(u8 data){
	*GPIO_PORT = data;
}

u8 GPO_READ(){
	return *GPIO_PORT;
}
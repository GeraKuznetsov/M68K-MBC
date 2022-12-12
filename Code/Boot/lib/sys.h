#include "../std.h"
#pragma once

//volatile u16* const IO_PORT = REG3;
extern volatile u16* const VGA_PORT;

//extern volatile u8* const PS2R_SYSW ;

#define SYS_PORT_TEMP (1<<0)
#define SYS_PORT_CS0 (1<<1)

void EINT();

void DINT();


void SYS_PORT_SET(u8 mask);

void SYS_PORT_RESET(u8 mask);

u8 READ_PS2_PORT();

u8 SPI_SEND(u8 data);
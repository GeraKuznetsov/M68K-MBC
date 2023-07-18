#include "std.h"
#pragma once

extern volatile u16* const VGA_PORT;

extern volatile u8* const PS2R_SYSW;
extern volatile u8* const SYSR_INTW;
extern volatile u8* const SPI_PORT;
extern volatile u8* const GPIO_PORT;

#define SYS_OUT_PORT_PS2_CLOCK	(1<<0)
#define SYS_OUT_PORT_PS2_DATA 	(1<<1)
#define SYS_OUT_PORT_SPI_CE0 	(1<<2)
#define SYS_OUT_PORT_SDA 		(1<<3)
#define SYS_OUT_PORT_SCL 		(1<<4)
#define SYS_OUT_PORT_SPI_SPEED	(1<<5)
#define SYS_OUT_PORT_6 			(1<<6)
#define SYS_OUT_PORT_7 			(1<<7)

#define SYS_IN_PORT_PS2_CLOCK	(1<<0)
#define SYS_IN_PORT_PS2_DATA	(1<<1)
#define SYS_IN_PORT_SDA			(1<<2)
#define SYS_IN_PORT_3			(1<<3)
#define SYS_IN_PORT_4			(1<<4)
#define SYS_IN_PORT_5			(1<<5)
#define SYS_IN_PORT_6			(1<<6)
#define SYS_IN_PORT_7			(1<<7)

#define INT_SOURCE_1			(1<<0)
#define INT_SOURCE_2			(1<<1)
#define INT_SOURCE_3			(1<<2)
#define INT_SOURCE_4			(1<<3)
#define INT_SOURCE_5			(1<<4)
#define INT_SOURCE_6			(1<<5)
#define INT_SOURCE_PS2 			(1<<6)
#define INT_SOURCE_GLOBAL 		(1<<7)

extern volatile u8 SYS_PORT_VALUE; 
extern volatile u8 INTERRUPT_MASK_VALUE; 

#define EINT() INT_E_SET(INT_SOURCE_GLOBAL)
#define DINT() INT_E_RESET(INT_SOURCE_GLOBAL)

//void SYS_PORT_SET(u8 mask);
//void SYS_PORT_RESET(u8 mask);
//#define SYS_PORT_STATE(x, maskOff, maskOn) *PS2R_SYSW = SYS_PORT_VALUE = (SYS_PORT_VALUE & ~maskOff) | maskOn;
#define SYS_PORT_SET(x)	*PS2R_SYSW = SYS_PORT_VALUE = SYS_PORT_VALUE | x
#define SYS_PORT_RESET(x) *PS2R_SYSW = SYS_PORT_VALUE = SYS_PORT_VALUE & ~x;
#define SYS_PORT_TOGGLE(x) *PS2R_SYSW = SYS_PORT_VALUE = SYS_PORT_VALUE ^ x;

u8 READ_PS2_PORT();

//u8 SYS_PORT_READ();
#define SYS_PORT_READ() (*SYSR_INTW)

void INT_E_SET(u8 mask);
void INT_E_RESET(u8 mask);

#define SPI_SLOW 1
#define SPI_FAST 0

u8 SPI_SET_SPEED(u8 spd);
//__attribute__((always_inline)) u8 SPI_SEND(u8 data);
u8 SPI_SEND(u8 data);
//#define SPI_SEND(data) (*SPI_PORT = (data))

void GPO_WRITE(u8 data);
u8 GPO_READ();
#include "std.h"
#include "lib/sys.h"
#define DELAY 0xFFFF

void writeString(u8* s, volatile u16* vram, u32 x, u32 y, u32 color){
	SYS_PORT_RESET(SYS_PORT_CS0);
	while(*s){
		vram[(y * (u32)256) + (u32)(x * 2)] = *s;
		vram[(y * (u32)256) + (u32)(x * 2 + 1)] = color;
		x++;
		s++;
	}
	SYS_PORT_SET(SYS_PORT_CS0);
}


u8 y = 0; 

int main(){ 
	SYS_PORT_RESET(SYS_PORT_CS0);
	for(int x = 0; x < 80; x++){
		for(int y = 0; y < 60; y++){
			VGA_PORT[(y) * 256 + (x * 2)] = 0;
			VGA_PORT[(y) * 256 + (x * 2) + 1] = 0;
		}
	}
	SYS_PORT_SET(SYS_PORT_CS0);


	writeString("Hello World!", VGA_PORT, 0, y++, 0b111000);

	EINT();
	
	while(1){};
}

void __attribute__((interrupt)) IRQ1(void){
	writeString("IRQ1", VGA_PORT, 0, y++, 0b111000);
}

void __attribute__((interrupt)) IRQ2(void){
	writeString("IRQ2", VGA_PORT, 0, y++, 0b111000);
}

void __attribute__((interrupt)) IRQ3(void){
	writeString("IRQ3", VGA_PORT, 0, y++, 0b111000);
}

const u8 *toHex = "0123456789ABCDEF";

void __attribute__((interrupt)) IRQ4(void){
	//writeString("IRQ4", VGA_PORT, 0, (u32)(y++), 0b111000);
	u8 val = READ_PS2_PORT();
	u8 b[] = {toHex[(val >> 4)& 0x0F], toHex[val & 0x0F], 0};
	writeString(b, VGA_PORT, 0, (u32)(y++), 0b111000);
}

void __attribute__((interrupt)) IRQ5(void){
	writeString("IRQ5", VGA_PORT, 0, y++, 0b111000);
}

void __attribute__((interrupt)) IRQ6(void){
	writeString("IRQ6", VGA_PORT, 0, y++, 0b111000);
}

void __attribute__((interrupt)) IRQ7(void){
	writeString("IRQ7", VGA_PORT, 0, y++, 0b111000);
}
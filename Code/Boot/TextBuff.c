#include <TextBuff.h>

void Clear(volatile u16 *buffer){
	for(int x = 0; x < 80; x++){
		for(int y = 0; y < 60; y++){
			buffer[(y) * 256 + (x * 2)] = 0;
			buffer[(y) * 256 + (x * 2) + 1] = 0;
		}
	}
}

void WriteString(volatile u16 *buffer, u8* s, u16 x, u16 y, u16 color){
	u16 xOrig = x;
	while(*s){
		if(*s == '\n'){
			x = xOrig;
			y++;
		}else{
			buffer[(y * (u32)256) + (u32)(x * 2)] = *s;
			buffer[(y * (u32)256) + (u32)(x * 2 + 1)] = color;
			x++;
		}
		s++;
	}
}
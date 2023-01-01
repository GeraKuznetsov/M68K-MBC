#include "std.h"
#include "driver/sys.h"
#include "driver/PS2.h"
#include "driver/SPI.h"
#include "sprintf.h"
#include "driver/EventQ.h"
#include "malloc.h"
#include <TextBuff.h>

void panic(u8* message){
	//DINT();
	Clear(VGA_PORT);
	int y = 0;
	WriteString(VGA_PORT, "       _______________________________", 0, (u32)(y++), 0b111000);
	WriteString(VGA_PORT, "     <                               >", 0, (u32)(y), 0b111000);
	WriteString(VGA_PORT, message, 8, (u32)(y++), 0b111000);
	WriteString(VGA_PORT, "       -------------------------------", 0, (u32)(y++), 0b111000);
	WriteString(VGA_PORT, "              \\   ^__^", 0, (u32)(y++), 0b111000);
	WriteString(VGA_PORT, "               \\  (xx)\\_______", 0, (u32)(y++), 0b111000);
	WriteString(VGA_PORT, "                  (__)\\       )\\/\\", 0, (u32)(y++), 0b111000);
	WriteString(VGA_PORT, "                   U  ||----w |", 0, (u32)(y++), 0b111000);
	WriteString(VGA_PORT, "                      ||     ||", 0, (u32)(y++), 0b111000);
	SYS_PORT_TOGGLE(SYS_OUT_PORT_7);
	while(1);
}


EventQ *OS_EventQ = 0;

u16 testRam(){
	for(int i = 0; i < 16; i++){
		((u16*)REG2)[i] = i;
	}
	for(int i = 0; i < 16; i++){
		if(((u16*)REG2)[i] != i){
			return 0;
		}
	}
	return 1;
}
#define WHITE_ON_BLACK (0b111000)
#define BLACK_ON_WHITE (0b000111)

u8 buff[64];
extern u8 ps2CommandState;

const u8 *toHex = "0123456789ABCDEF";
extern const u32 _HEEP_START;

u32 RoundKB(u32 i){
	u32 out = i >> 10;
	return out + ((i & 0b1111111111) != 0);
}

EventQ MainEventQ;

u8 testBuffer[128];

int main(){ 
	SYS_PORT_SET(SYS_OUT_PORT_7);

	for(int i = 0; i < 128; i++){
		testBuffer[i] = 0;
	}

	int ram = testRam();
	MallocInit(ram);

	/*Clear(VGA_PORT);

	
	void* p0 = malloc(0x100);
	void* p1 = malloc(0x200);
	void* p2 = malloc(0x300);
	void* p3 = malloc(0x400);
	
	int y = 0;
	
	printMallocInfo(buffTemp);
	WriteString(VGA_PORT, buffTemp, 0, y, WHITE_ON_BLACK); y+=8;

	free(p3);
	printMallocInfo(buffTemp);
	WriteString(VGA_PORT, buffTemp, 0, y, WHITE_ON_BLACK); y+=5;

	free(p2);
	printMallocInfo(buffTemp);
	WriteString(VGA_PORT, buffTemp, 0, y, WHITE_ON_BLACK); y+=4;

	free(p0);
	printMallocInfo(buffTemp);
	WriteString(VGA_PORT, buffTemp, 0, y, WHITE_ON_BLACK); y+=4;
	
	free(p1);
	printMallocInfo(buffTemp);
	WriteString(VGA_PORT, buffTemp, 0, y, WHITE_ON_BLACK); y+=8;
	
	p0 = malloc(0x100);
	p1 = malloc(0x200);
	p2 = malloc(0x100);
	free(p1);
	p1 = malloc(0x100);
	printMallocInfo(buffTemp);
	WriteString(VGA_PORT, buffTemp, 0, y, WHITE_ON_BLACK); y+=8;
	
	SYS_PORT_TOGGLE(SYS_OUT_PORT_7);
	while(1);*/
	
	if(PS2_INIT()){
		panic("KB init error");		
	}
	
	OS_EventQ = &MainEventQ;
	EventQInit(OS_EventQ);
	
	INT_E_SET(INT_SOURCE_PS2 | INT_SOURCE_GLOBAL);

	int testBufferInc = 0;
	
	Clear(VGA_PORT);
	SYS_PORT_TOGGLE(SYS_OUT_PORT_7);
	Clear(VGA_PORT);

	while(1){
		while(1){
			event e = GetEvent(OS_EventQ);
			if(e.type & EVENT_KEY_PRESSED){
				if(e.data == KEY_ENTER){ 
					
				}
			}else if(e.type & EVENT_ASCII_TYPED){
				testBuffer[testBufferInc++] = e.data;
			}
			if(e.type == EVENT_NONE) break;
		}
		Clear(VGA_PORT);
		WriteString(VGA_PORT, "m68k Multitasking OS @German Kuznetsov", 0, 0, WHITE_ON_BLACK);
		sprintf(buff, "System Ram: %dMB", ram + 1);
		WriteString(VGA_PORT, buff, 0, 1, WHITE_ON_BLACK);
		sprintf(buff, "Malloc Pool: %dKB / %dKB", 123, RoundKB(MallocTotalMEM));
		WriteString(VGA_PORT, buff, 0, 2, WHITE_ON_BLACK);
		
		int y = 3;
		WriteString(VGA_PORT, "Running processes:", 0, y, WHITE_ON_BLACK); y++;
		WriteString(VGA_PORT, "<Launch Shell>:", 3, y, BLACK_ON_WHITE); y++;
				
		WriteString(VGA_PORT, testBuffer, 0, 40, WHITE_ON_BLACK);

		SYS_PORT_TOGGLE(SYS_OUT_PORT_7);
	}
	//register u32 p1 asm ("a4") = 5;
	
	while(1){
	}
	

//	u8 stat;
//	
//	stat = 0;
//	res = f_mount(&fs, "", 1);
//	if(res != FR_OK) goto fail;
//	//
//	stat = 1;
//	res = f_opendir(&dir, path); 
//	if(res != FR_OK) goto fail;
//	
//	stat = 2;
//	while(1) {
//		res = f_readdir(&dir, &fno);
//		writeString(fno.fname, VGA_PORT, 0, (u32)(y++), 0b111000);
//		if (res != FR_OK || fno.fname[0] == 0) break;
//		if (fno.fattrib & AM_DIR) {   
//			//writeString(fno.fname, VGA_PORT, 0, (u32)(y++), 0b111000);
//		}
//	}
//		
//	//stat = 0;
//	//res = mmc_disk_initialize();
//	//stat = 1;
//		
//	fail:;
//	//stat = mmc_disk_initialize();
//	u8 b[] = {toHex[stat >> 4], toHex[stat & 0x0F], 0};
//	writeString(b, VGA_PORT, 0, (u32)(y++), 0b111000);
//	u8 b2[] = {toHex[res >> 4], toHex[res & 0x0F], 0};
//	writeString(b2, VGA_PORT, 0, (u32)(y++), 0b111000);
//	
//	//PS2_INIT();
//	//PS2_ASCII_CB = ps2_ascii_cb;
//
//	//EINT();
//	//INT_E_SET(INT_SOURCE_PS2 | INT_SOURCE_GLOBAL);
//	
//	while(1){};
}

void __attribute__((interrupt)) IRQ1(void){
	//writeString("IRQ1", VGA_PORT, 0, y++, 0b111000);
}

void __attribute__((interrupt)) IRQ2(void){
	//writeString("IRQ2", VGA_PORT, 0, y++, 0b111000);
}

void __attribute__((interrupt)) IRQ3(void){
	//writeString("IRQ3", VGA_PORT, 0, y++, 0b111000);

		for(int i = 0; i < 256; i++){
			*SYSR_INTW  = (u8) i;
		}

}

void AddressError(void){
	panic("AddressError");
}

//const u8 *toHex = "0123456789ABCDEF";

void __attribute__((interrupt)) IRQ4(void){
	//writeString("IRQ4", VGA_PORT, 0, (u32)(y++), 0b111000);
	//u8 val = READ_PS2_PORT();
	//u8 b[] = {toHex[(val >> 4)& 0x0F], toHex[val & 0x0F], 0};
	//writeString(b, VGA_PORT, 0, (u32)(y++), 0b111000);
}   

void __attribute__((interrupt)) IRQ5(void){
	//writeString("IRQ5", VGA_PORT, 0, y++, 0b111000);
}

void __attribute__((interrupt)) IRQ6(void){
	//writeString("IRQ6", VGA_PORT, 0, y++, 0b111000);
}

void __attribute__((interrupt)) IRQ7(void){
	//writeString("IRQ7", VGA_PORT, 0, y++, 0b111000);
	PS2_INT();
}
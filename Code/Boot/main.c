#include "std.h"
#include "driver/sys.h"
#include "driver/PS2.h"
#include "malloc.h"
#include <TextBuff.h>
#include "Proc.h"
#include <string.h>
#include "driver/EventQ.h"
#include <ff.h>

	
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

u8 _2MB;
u8 buff[256];

extern const u32 _HEEP_START;

event Sys_GetEvent(){
	return GetEvent();
}

void kern(){
	asm __volatile__("movea.l #_stack_end, %sp"); //fresh stack every time we enter kernel 
	asm __volatile__("jmp kern_ent"); 
}

FATFS driveFATFS[1];

FATFS* GetFATFS(u16 *drives){
	*drives = 1;
	return driveFATFS;
}

//void __attribute__((interrupt)) TRAP15(void){
//	
//}



u16 trapRunPid = 0xFFFF;

void launchProcTrap15(){
	u16 trp = trapRunPid;
	trapRunPid = 0xFFFF;
	LaunchProc(trp);
}

int Sys_RunProc(char* path){
	//u16 pid_return;
	u16 i = LoadProc(path, 0, &trapRunPid);
	if(i) return i;
	//trapRunPid = 
	asm("trap #15");
	return 0;
}

extern u32 const _bss_start;
extern u32 const _bss_end;
extern u32 const _rodata_end;
extern u32 const _data_start;
extern u32 const _data_end;

void main(){
	asm __volatile__("movea.l #0x00100B50, %sp"); //fresh stack every time we enter kernel 
	asm __volatile__("jmp main_ent"); //fresh stack every time we enter kernel 
}

void main_ent(){
	asm __volatile__("movea.l #_stack_end, %sp"); //we need a stack to use function calls

	memset((void*) &_bss_start, 0, (u32)(&_bss_end) - (u32)(&_bss_start));
	memcpy((void*)&_data_start, (void*) &_rodata_end, (u32)(&_data_end) -  (u32)(&_data_start));
	
	INT_E_RESET(INT_SOURCE_GLOBAL);	//no interrupts

	asm __volatile__("move.w  #0x2000, %sr");
	
	//Do we have a second MB?
	_2MB = 1;
	for(int i = 0; i < 16; i++){
		((u16*)REG2)[i] = i;
	}
	for(int i = 0; i < 16; i++){
		if(((u16*)REG2)[i] != i){
			_2MB = 0;
		}
	}
	
	MallocInit(_2MB);
	
	for(int i =0 ; i< TOTAL_PIDS; i++){
		runningProc[i] = 0;
	}
	
	currentProc = 0xFFFF;
	//Init PS2 Keyboard
	if(PS2_INIT()){
		panic("KB init error");		
	}
	INT_E_SET(INT_SOURCE_PS2 | INT_SOURCE_GLOBAL);
	
	driveFATFS[0].fs_type = 0;
	
	kern();
}

void UNDEF_SYSCALL(){
	panic("UNDEF_SYSCALL");
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

void PrivilegeViolation(void){
	u32 PC = 0;
	__asm__ __volatile__(" move.l (%%sp)+, %[pc]\n"
						: [ pc ] "=d"(PC)
						: 
						:);
	
	//sprintf(buff, "PrivilegeViolation @%p", (u32) PC);
 	panic("PrivilegeViolation @%p");
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

void core_dump(){
	u8 log_drive[] = "0:";
	if(!driveFATFS[0].fs_type){
		FRESULT res = f_mount(&driveFATFS[0], log_drive, 1);
		if(res) panic("dump failed");
	}
	u8 log_file[] = "0:/dump.bin";
	FIL fp;
	f_open(&fp, log_file, FA_CREATE_ALWAYS | FA_WRITE);
	UINT br, bw;
	f_write(&fp, (void*) 0, (u32)0x200000, &bw);  
	f_close(&fp);
}

asm(R"(
.global returnToProc
returnToProc:
	moveq #0,%d0
	move.w currentProc,%d0
	add.l %d0,%d0
	add.l %d0,%d0
	lea runningProc,%a0
	move.l (%a0,%d0.l),%a0
	add #32, %a0
	
	#get d2-d7 / a2-a6
	move.l (%a0)+, %d2 
	move.l (%a0)+, %d3 
	move.l (%a0)+, %d4 
	move.l (%a0)+, %d5 
	move.l (%a0)+, %d6 
	move.l (%a0)+, %d7 
	move.l (%a0)+, %a2 
	move.l (%a0)+, %a3 
	move.l (%a0)+, %a4 
	move.l (%a0)+, %a5 
	move.l (%a0)+, %a6 
	
	lea (-22,%sp),%sp

	#get d0/d1/a0/a1 and put on stack
	move.l (%a0)+, (%sp)
	move.l (%a0)+, 4(%sp)
	move.l (%a0)+, 8(%sp)
	move.l (%a0)+, 12(%sp)
	
	#sr
	move.w (%a0)+, 16(%sp)
	
	#get pc
	move.l (%a0)+, 18(%sp)
		
	#get sp
	move.l (%a0)+, %a1
	move.l %a1, %usp
	
	#get d0/d1/a0/a1 from stack
	movem.l (%sp)+,#771
	rte
	)");

asm(R"(
.global TRAP15
TRAP15:
	movem.l #49344,-(%sp)

	moveq #0,%d0
	move.w currentProc,%d0
	add.l %d0,%d0
	add.l %d0,%d0
	lea runningProc,%a0
	move.l (%a0,%d0.l),%a0
	add #32, %a0
	
	#save d2-d7 / a2-a6
	move.l %d2, (%a0)+
	move.l %d3, (%a0)+
	move.l %d4, (%a0)+
	move.l %d5, (%a0)+
	move.l %d6, (%a0)+
	move.l %d7, (%a0)+
	move.l %a2, (%a0)+
	move.l %a3, (%a0)+
	move.l %a4, (%a0)+
	move.l %a5, (%a0)+
	move.l %a6, (%a0)+

	#save d0/d1/a0/a1
	move.l (%sp)+, (%a0)+
	move.l (%sp)+, (%a0)+
	move.l (%sp)+, (%a0)+
	move.l (%sp)+, (%a0)+
	
	#save sr
	move.w (%sp)+, (%a0)+
	
	#save pc
	move.l (%sp)+, (%a0)+
	
	#save sp/a7
	move.l %usp, %a1
	move.l %a1, (%a0)+
	
	jmp launchProcTrap15
	)");
	
asm(R"(
.global saveRegAndReturnToKern
saveRegAndReturnToKern:
	moveq #0,%d0
	move.w currentProc,%d0
	add.l %d0,%d0
	add.l %d0,%d0
	lea runningProc,%a0
	move.l (%a0,%d0.l),%a0
	add #32, %a0
	
	#save d2-d7 / a2-a6
	move.l %d2, (%a0)+
	move.l %d3, (%a0)+
	move.l %d4, (%a0)+
	move.l %d5, (%a0)+
	move.l %d6, (%a0)+
	move.l %d7, (%a0)+
	move.l %a2, (%a0)+
	move.l %a3, (%a0)+
	move.l %a4, (%a0)+
	move.l %a5, (%a0)+
	move.l %a6, (%a0)+

	#save d0/d1/a0/a1
	move.l (%sp)+, (%a0)+
	move.l (%sp)+, (%a0)+
	move.l (%sp)+, (%a0)+
	move.l (%sp)+, (%a0)+
	
	#save sr
	move.w (%sp)+, (%a0)+
	
	#save pc
	move.l (%sp)+, (%a0)+
	
	#save sp/a7
	move.l %usp, %a1
	move.l %a1, (%a0)+
	
	jmp kern
	)");

asm(R"(
.section .text
	.global shell_exe_start
shell_exe_start:
	.incbin "../Shell/bin/shell.bin"
)");

void __attribute__((interrupt)) DefaultInterrupt(){
	while(1){}
}

void __attribute__((interrupt)) IRQ7(void){
	int ret = PS2_INT();
	if(!ret) return;
	//if(ret == PS2_INT_RETURN_DUMP){
	//	//core_dump();
	//	asm("bra saveRegAndReturnToKern");
	//}
	if(ret == PS2_INT_RETURN_OS && currentProc != 0xFFFF){
		asm("bra saveRegAndReturnToKern");
	}
}
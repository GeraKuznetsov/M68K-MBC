#include "PS2.h"
#include "sys.h"
#include "EventQ.h"

extern EventQ *OS_EventQ;

u8 brk = 0, ext = 0;
u8 PS2_LOCKS = 0;
#define LOCK_CAPS 4
#define LOCK_NUM 2
#define LOCK_SCROLL 1

#define CS_SET_LEDS 1
#define CS_IGNORE 2
#define CS_RESET 3

u8 ps2CommandState;

//void (*PS2_MAKE_CB) (u8);
//void (*PS2_BREAK_CB) (u8);
//void (*PS2_TYPED_CB) (u8);
//void (*PS2_ASCII_CB) (u8);

u8 PS2_KEY_DOWN[KEY_NUM_KEYS];

u8 keymap[256] = {
	0, KEY_F9, 0, KEY_F5, KEY_F3, KEY_F1, KEY_F2, KEY_F12,	0, KEY_F10, KEY_F8, KEY_F6, KEY_F4, KEY_TAB, KEY_BACKTICK, 0,
	0, KEY_LEFT_ALT, KEY_LEFT_SHIFT, 0, KEY_LEFT_CNTR, 'Q', KEY_1, 0,	0, 0, 'Z', 'S', 'A', 'W', KEY_2, 0,
	0, 'C', 'X', 'D', 'E', KEY_4, KEY_3, 0,	0, KEY_SPACEBAR, 'V', 'F', 'T', 'R', KEY_5, 0,
	0, 'N', 'B', 'H', 'G', 'Y', KEY_6, 0,	0, 0, 'M', 'J', 'U', KEY_7, KEY_8, 0,
	0, KEY_COMMA, 'K', 'I', 'O', KEY_0, KEY_9, 0,	0, KEY_PERIOD, KEY_FORWARD_SLASH, 'L', KEY_SEMICOLON, 'P', KEY_MINUS, 0,
	0, 0, KEY_BACK_SLASH, 0, KEY_OPEN_SQUARE, KEY_EQUALS, 0, 0, KEY_CAPS_LOCK, KEY_RIGHT_SHIFT, KEY_ENTER, KEY_CLOSE_SQUARE, 0, KEY_BACK_SLASH, 0, 0,
	0, 0, 0, 0, 0, 0, KEY_BACKSPACE, 0,	0, KEY_KP_1, 0, KEY_KP_4, KEY_KP_7, 0, 0, 0,
	KEY_KP_0, KEY_KP_PERIOD, KEY_KP_2, KEY_KP_5, KEY_KP_6, KEY_KP_8, KEY_ESC, KEY_NUM_LOCK, KEY_F11, KEY_KP_PLUS, KEY_KP_3, KEY_KP_MINUS, KEY_KP_MULT, KEY_KP_9, KEY_SCROLL_LOCK, 0,
	0, 0, 0, KEY_F7
};

u8 symbolMapL[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '`', '-', '=', ';', '\'', '/', ',', '.',  ' ', '[', ']', '\\'};
u8 symbolMapU[] = { ')', '!', '@', '#', '$', '%', '^', '&', '*', '(', '~', '_', '+', ':', '"', '?', '<', '>', ' ', '{', '}', '|', };
u8 kpsymmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '*', '/', '+', '.'};
u8 kpActionMap[] = {KEY_INSERT, KEY_END, KEY_DOWN, KEY_PAGE_DOWN, KEY_LEFT, 0, KEY_RIGHT, KEY_HOME, KEY_UP, KEY_PAGE_UP, 0, 0, 0, 0, KEY_DELETE};

#define ATTEMPT() attempt--; if(!attempt) return 1;

//const u8 *toHex = "0123456789ABCDEF";
//static int y = 0;
//void WriteString(volatile u16 *buffer, u8* s, u16 x, u16 y, u16 color);

u16 PS2_SEND(u8 data){
	u32 attempt = 100000;
	
	u8 parity = data;
	parity ^= parity >> 4;
	parity ^= parity >> 2;
	parity ^= parity >> 1;
	parity = (~parity) & 1;
	
	//Bring the Clock line low for at least 100 microseconds.
	SYS_PORT_SET(SYS_OUT_PORT_PS2_CLOCK);
	for(int i = 0; i < 50; i++){ //not exceact (duh) 
		asm("nop");
	}
	
	//Bring the Data line low.
	SYS_PORT_SET(SYS_OUT_PORT_PS2_DATA);
	
	//Release the Clock line.
	SYS_PORT_RESET(SYS_OUT_PORT_PS2_CLOCK);
	
	//Wait for the device to bring the Clock line low.
	while(!(SYS_PORT_READ() & SYS_IN_PORT_PS2_CLOCK)){
		ATTEMPT();
	}

	//Repeat steps 5-7 for the other seven data bits
	for(int bit = 1; bit < 256; bit <<= 1){
		if(data & bit){	
			SYS_PORT_RESET(SYS_OUT_PORT_PS2_DATA);
		}else{ 	
			SYS_PORT_SET(SYS_OUT_PORT_PS2_DATA);
		}
		//Wait for the device to bring Clock high.
		while((SYS_PORT_READ() & SYS_IN_PORT_PS2_CLOCK)){
			ATTEMPT();
		}
		//Wait for the device to bring Clock low.
		while(!(SYS_PORT_READ() & SYS_IN_PORT_PS2_CLOCK)){
			ATTEMPT();
		}
	}
	
	// and the parity bit
	if(parity){	
		SYS_PORT_RESET(SYS_OUT_PORT_PS2_DATA);
	}else{ 	
		SYS_PORT_SET(SYS_OUT_PORT_PS2_DATA);
	}
	//Wait for the device to bring Clock high.
	while((SYS_PORT_READ() & SYS_IN_PORT_PS2_CLOCK)){
		ATTEMPT();
	}
	//Wait for the device to bring Clock low.
	while(!(SYS_PORT_READ() & SYS_IN_PORT_PS2_CLOCK)){
		ATTEMPT();
	}
	
	//Release the Data line.
	SYS_PORT_RESET(SYS_OUT_PORT_PS2_DATA);
	
	//Wait for the device to bring Data low.
	while(SYS_PORT_READ() & SYS_IN_PORT_PS2_DATA){
		ATTEMPT();
	}
	
	//Wait for the device to bring Clock  low.
	while(!(SYS_PORT_READ() & SYS_IN_PORT_PS2_CLOCK)){
		ATTEMPT();
	}

	//Wait for the device to release Data and Clock 
	while(!(SYS_PORT_READ() & SYS_IN_PORT_PS2_DATA)){
		ATTEMPT();
	}
	while((SYS_PORT_READ() & SYS_IN_PORT_PS2_CLOCK)){
		ATTEMPT();
	}
	
	return 0;
}

u16 PS2_INIT(){
	//PS2_MAKE_CB = PS2_BREAK_CB = PS2_TYPED_CB = 0;
	for(int i = 0; i < KEY_NUM_KEYS; i++){
		PS2_KEY_DOWN[i] = 0;
	}	

	ps2CommandState = CS_RESET;
	return PS2_SEND(0xFF);
}

void panic(const char* message);

void PS2_INT(){
	u8 code = READ_PS2_PORT();
	
	//SYS_PORT_TOGGLE(SYS_OUT_PORT_7);
	//u8 b[] = {toHex[(code >> 4)& 0x0F], toHex[code & 0x0F], 0};
	//WriteString(VGA_PORT, b, 50, (u32)(y++), 0b111000);
	//SYS_PORT_TOGGLE(SYS_OUT_PORT_7);
	

	switch(ps2CommandState){
		case CS_SET_LEDS:
			if(code == 0xFA){
				PS2_SEND(PS2_LOCKS);
				ps2CommandState = CS_IGNORE;
			}
			return;
		case CS_RESET:
			if(code == 0xAA){
				ps2CommandState = 0;
			}
			return;
		case CS_IGNORE:
			ps2CommandState = 0;
			return;
	}
	
	//u8 b2[] = {toHex[(code >> 4)& 0x0F], toHex[code & 0x0F], 0};
	//writeString(b2, VGA_PORT, 4, (u32)(y-1), 0b111000);
	
	if(code == 0xF0){
		brk = 1;
		return;
	}else if(code == 0xE0){
		ext = 1;
		return;
	}
	
	if(ext){
		switch(code){
			case 0x1F:
				code = KEY_LEFT_WIN; break;
			case 0x11:
				code = KEY_LEFT_ALT; break;
			case 0x27:
				code = KEY_RIGHT_WIN; break;
			case 0x2F:
				code = KEY_MENU; break;
			case 0x14:
				code = KEY_RIGHT_CONTROL; break;
			case 0x4A:
				code = KEY_KP_FORWARD_SLASH; break;
			case 0x6C:
				code = KEY_HOME; break;
			case 0x75:     
				code = KEY_UP; break;
			case 0x7D:     
				code = KEY_PAGE_UP; break;	
			case 0x6B:     
				code = KEY_LEFT; break;
			case 0x74:     
				code = KEY_RIGHT; break;
			case 0x69:     
				code = KEY_END; break;
			case 0x72:     
				code = KEY_DOWN; break;
			case 0x7A:     
				code = KEY_PAGE_DOWN; break;	
			case 0x70:     
				code = KEY_INSERT; break;
			case 0x71:     
				code = KEY_DELETE; break;
			case 0x5A:
				code = KEY_ENTER;
			default: 
				code = 0;
		}
		ext = 0;
	}else{
		code = keymap[code];	
	}
	
	if(code == 0){
		ext = 0;
		brk = 0;
	}
	
	if(code >= KEY_KP_0 && code <= KEY_KP_PERIOD){
		if(!(	!!(PS2_KEY_DOWN[KEY_LEFT_SHIFT] | PS2_KEY_DOWN[KEY_RIGHT_SHIFT])	^	!!(PS2_LOCKS & LOCK_NUM)	)){
			u8 action = kpActionMap[code - KEY_KP_0];
			if(action) code = action;
		}
	}		

	if(!brk){
		u16 down = 0;
		if(!PS2_KEY_DOWN[code]){
			if(code == KEY_CAPS_LOCK){
				PS2_LOCKS ^= LOCK_CAPS;
				INT_E_RESET(INT_SOURCE_PS2);
				PS2_SEND(0xED);
				ps2CommandState = CS_SET_LEDS;
				INT_E_SET(INT_SOURCE_PS2);
			}else if(code == KEY_NUM_LOCK){
				PS2_LOCKS ^= LOCK_NUM;
				INT_E_RESET(INT_SOURCE_PS2);
				PS2_SEND(0xED);
				ps2CommandState = CS_SET_LEDS;
				INT_E_SET(INT_SOURCE_PS2);
			}else if(code == KEY_SCROLL_LOCK){
				PS2_LOCKS ^= LOCK_SCROLL;
				INT_E_RESET(INT_SOURCE_PS2);
				PS2_SEND(0xED);
				ps2CommandState = CS_SET_LEDS;
				INT_E_SET(INT_SOURCE_PS2);
			}
			//if(PS2_MAKE_CB)	PS2_MAKE_CB(code);
			
			down = EVENT_KEY_PRESSED;
			//event e = {EVENT_KEY_PRESSED, code};
			//EventQAdd(OS_EventQ, e);
		}
		PS2_KEY_DOWN[code] = 1;
		//if(PS2_TYPED_CB) PS2_TYPED_CB(code);
		//if(PS2_ASCII_CB){
		event e = {EVENT_KEY_REPEAT | down, code};
		EventQAdd(OS_EventQ, e);
			
			
		if(1){
			if(code >= 'A' && code <= 'Z'){ //letters
				if(!(	!!(PS2_KEY_DOWN[KEY_LEFT_SHIFT] | PS2_KEY_DOWN[KEY_RIGHT_SHIFT])	^	!!(PS2_LOCKS & LOCK_CAPS)	)){
					code += 32;
				}

				event e = {EVENT_ASCII_TYPED, code};
				EventQAdd(OS_EventQ, e);

			}else if(code >= KEY_0 && code <= KEY_BACK_SLASH){ //SYMBOLS
				if(PS2_KEY_DOWN[KEY_LEFT_SHIFT] | PS2_KEY_DOWN[KEY_RIGHT_SHIFT]){
					code = symbolMapU[code - KEY_0];
				}else{
					code = symbolMapL[code - KEY_0];
				}
				
				event e = {EVENT_ASCII_TYPED, code};
				EventQAdd(OS_EventQ, e);

			}else if(code >= KEY_KP_0 && code <= KEY_KP_PERIOD){ //Keypad
				code = kpsymmap[code - KEY_KP_0];
				
				event e = {EVENT_ASCII_TYPED, code};
				EventQAdd(OS_EventQ, e);
			}
		}
		


		
	}else{
		brk = 0;
		PS2_KEY_DOWN[code] = 0;
		//EventQAdd(KEY_)
		//if(PS2_BREAK_CB) PS2_BREAK_CB(code);
	}
}
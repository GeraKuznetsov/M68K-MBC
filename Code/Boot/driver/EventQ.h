#include <std.h>

//MUST BE 2^x
#define EVENT_Q_SIZE 16

#define EVENT_NONE 0
#define EVENT_KEY_PRESSED 1
#define EVENT_KEY_REPEAT 2
#define EVENT_ASCII_TYPED 4

typedef struct{
	u8 type;
	u8 data;
} event;

typedef struct{
	event events[EVENT_Q_SIZE];
	u8 write, read;
} EventQ;

void EventQInit(EventQ *eq);

void EventQAdd(EventQ *eq, event e);

event GetEvent(EventQ *eq);
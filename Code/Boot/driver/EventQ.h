#include <std.h>
#include <Event.h>

//MUST BE 2^x
#define EVENT_Q_SIZE 16

struct{
	event events[EVENT_Q_SIZE];
	u8 write, read;
} EventQ;

void EventQInit();

void EventQAdd(event e);

event GetEvent();
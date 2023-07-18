#include "EventQ.h"
#include "sys.h"

void EventQInit(){
	EventQ.write = EventQ.read = 0;
}

__attribute__((always_inline)) inline int Inc(u8 i){
	return (i+1)  & (EVENT_Q_SIZE - 1);
	return i;
}

void EventQAdd(event e){
	u8 writeInc = Inc(EventQ.write);
	if(writeInc == EventQ.read){ //drop event
		EventQ.read = Inc(EventQ.read);
	}
	EventQ.events[EventQ.write] = e;
	EventQ.write = writeInc;
}

event GetEvent(){
	event e = {EVENT_NONE, 0};
	INT_E_RESET(INT_SOURCE_GLOBAL);

	if(EventQ.read != EventQ.write){ 
		e = EventQ.events[EventQ.read];
		EventQ.read = Inc(EventQ.read);
	}

	INT_E_SET(INT_SOURCE_GLOBAL);
	
	return e;
}
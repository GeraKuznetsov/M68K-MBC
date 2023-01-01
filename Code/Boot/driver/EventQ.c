#include "EventQ.h"
#include "sys.h"

void EventQInit(EventQ *eq){
	eq->write = eq->read = 0;
}

__attribute__((always_inline)) inline int Inc(u8 i){
	return (i+1)  & (EVENT_Q_SIZE - 1);
	return i;
}

void EventQAdd(EventQ *eq, event e){
	if(!eq) return;
	u8 writeInc = Inc(eq->write);
	if(writeInc == eq->read){ //drop event
		eq->read = Inc(eq->read);
		//panic("dropped");
	}
	eq->events[eq->write] = e;
	eq->write = writeInc;
}

event GetEvent(EventQ *eq){
	
	
	event e = {EVENT_NONE, 0};
	INT_E_RESET(INT_SOURCE_GLOBAL);
	if(eq){
		if(eq->read != eq->write){ 
			e = eq->events[eq->read];
			eq->read = Inc(eq->read);
		}
	}
	INT_E_SET(INT_SOURCE_GLOBAL);
	
	return e;
}
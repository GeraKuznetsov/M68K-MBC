#include <std.h>

extern u32 MallocTotalMEM; 

void MallocInit(u16 mb);
	
void *malloc(u32 size, u16 proc);
void free(void *ptr);
u32 getMemUsage(u16 proc);
void freePID(u16 PID);

void printMallocInfo(char *buff);

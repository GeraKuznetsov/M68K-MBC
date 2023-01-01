#include <std.h>

extern u32 MallocTotalMEM; 

void MallocInit(u16 mb);
	
void *malloc(u32 size);
void free(void *ptr);

#include <sprintf.h>
void printMallocInfo(char *buff);

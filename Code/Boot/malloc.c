#include "malloc.h"

typedef struct MallocNode{
	struct MallocNode *next;
	struct MallocNode *prev;
	u32 size;
	u16 proc;
} MallocNode;

extern const u32 _HEAP_START;

#define REGION_SIZE (0x1000)
#define _1MB (0x100000)
#define _4KB (0x1000)
#define REG_PER_MB (_1MB / _4KB)

MallocNode *firstNode;
MallocNode *endNode;
u32 MallocTotalMEM; 

void MallocInit(u16 mb){
	endNode = (MallocNode*) ((u32)REG2 + (u32)_1MB * (u32)mb);
	firstNode = (MallocNode*) (&_HEAP_START);
	MallocTotalMEM = (u32)endNode - (u32)firstNode;
	
	MallocNode mn = {endNode, 0, 0, 0};
	*(firstNode) = mn;
}
	
#define ALLIGNMENT 2	
	
void *malloc(u32 size, u16 proc){
	size += sizeof(MallocNode) * 2;
	size = (size + ALLIGNMENT - 1) & ~(ALLIGNMENT - 1);
	
	MallocNode *curNode = firstNode;
	while(curNode != endNode){
		MallocNode *nextNode = curNode->next;
		
		u32 freeSpace = ((u32)nextNode) - ((u32)curNode);
		if(!curNode->size && (freeSpace >= size)){
			curNode->size = size;
			curNode->proc = proc;
			
			MallocNode* placeNode = (MallocNode*)(((u8*)curNode) + size);
			
			MallocNode mn = {nextNode, curNode, 0, 0};
			*placeNode = mn;
			
			curNode->next = placeNode;
			if(nextNode != endNode) nextNode->prev = placeNode;
			
			return ((u8*)curNode) + sizeof(MallocNode);
		}		
		
		curNode = nextNode;
	}
	return 0;
}

u32 getMemUsage(u16 proc){
	u32 size = 0;
	MallocNode *curNode = firstNode;
	while(curNode < endNode){
		if(proc == 0xFFFF || curNode->proc == proc){ //(curNode->size && (proc == 0xFFFF || curNode->proc == proc))
			size += curNode->size;
		}
		curNode = curNode->next;
	}
	return size;
}


#include <sprintf.h>

void printMallocInfo(char *buff){
	MallocNode *curNode = firstNode;
	while(curNode < endNode){
		MallocNode *nextNode = curNode->next;
		u32 length = sprintf(buff, "Loc: %p Next: %p size: %p Prev: %p\n", curNode, nextNode, curNode->size, curNode->prev);
		buff += length - 1;
		curNode = nextNode;
	}
}

__attribute__((always_inline)) inline 
 void deleteForward(MallocNode* node){
	if(!node->size && node->next != endNode && !node->next->size){
		node->next = node->next->next;
		if(node->next != endNode) node->next->prev = node;
	}
}

void free(void *ptr){
	if(!ptr) return;
	MallocNode* freeNode = (MallocNode*) (((u8*)ptr) - sizeof(MallocNode));
	freeNode->size = 0;
	freeNode->proc = 0xFFFF;
	deleteForward(freeNode);
	if(freeNode->prev) deleteForward(freeNode->prev);
}

void freePID(u16 PID){
	MallocNode *curNode = firstNode;
	while(curNode < endNode){
		MallocNode *nextNode = curNode->next;
		if((PID == 0xFFFF || curNode->proc == PID) && curNode->size){
			curNode->size = 0;
			deleteForward(curNode);
			if(curNode->prev) deleteForward(curNode->prev);
			
		}
		curNode = nextNode;
	}
}
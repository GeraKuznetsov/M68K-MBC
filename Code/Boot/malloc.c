#include "malloc.h"

typedef struct MallocNode{
	struct MallocNode *next;
	struct MallocNode *prev;
	u32 size;
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
	endNode = (MallocNode*) (REG2 + _1MB * mb);
	firstNode = (MallocNode*) (&_HEAP_START);
	MallocTotalMEM = (u32)endNode - (u32)firstNode;
	
	MallocNode mn = {endNode, 0, 0};
	*(firstNode) = mn;
}
	
#define ALLIGNMENT 2	
	
void *malloc(u32 size){
	size += sizeof(MallocNode);
	size = (size + ALLIGNMENT - 1) & ~(ALLIGNMENT - 1);
	
	MallocNode *curNode = firstNode;
	while(curNode != endNode){
		MallocNode *nextNode = curNode->next;
		
		if(!curNode->size && ((u32)nextNode) - ((u32)curNode) >= size){
			curNode->size = size;
			
			MallocNode* placeNode = (MallocNode*)(((u8*)curNode) + size);
			
			MallocNode mn = {nextNode, curNode, 0};
			*placeNode = mn;
			
			curNode->next = placeNode;
			if(nextNode != endNode) nextNode->prev = placeNode;
			
			return ((u8*)curNode) + sizeof(MallocNode);
		}		
		
		curNode = nextNode;
	}
	return 0;
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
	if(node->next != endNode && !node->next->size){
		node->next = node->next->next;
		if(node->next != endNode) node->next->prev = node;
	}
}

void free(void *ptr){
	if(!ptr) return;
	MallocNode* freeNode = (MallocNode*) (((u8*)ptr) - sizeof(MallocNode));
	freeNode->size = 0;
	deleteForward(freeNode);
	if(freeNode->prev) deleteForward(freeNode->prev);

}
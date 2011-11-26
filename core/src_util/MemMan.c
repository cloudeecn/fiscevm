/*
 * MemMan.c
 *
 *  Created on: Nov 24, 2011
 *      Author: cloudee
 */

#include "fy_util/MemMan.h"
typedef struct fy_memblockNode {
	struct fy_memblockNode *prev;
	struct fy_memblockNode *next;
	_FY_VLS(fy_byte,data);
} fy_memblockNode;

void fy_mmInit(fy_memblock *block, fy_exception *exception) {
	block->last = block->first = fy_allocate(sizeof(fy_memblockNode),
			exception);
	fy_exceptionCheckAndReturn(exception);
	block->blocks = 0;
}

void fy_mmDestroy(fy_memblock *block) {
	fy_memblockNode *node, *next;
#ifdef _DEBUG
	printf("Releasing %d managed memory blocks\n", block->blocks);
#endif
	node = block->first;
	do {
		next = node->next;
		fy_free(node);
		node = next;
	} while (node != NULL);
}

void *fy_mmAllocate(fy_memblock *block, int size, fy_exception *exception) {
	fy_memblockNode *node = fy_allocate(sizeof(fy_memblockNode) + size,
			exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	((fy_memblockNode*) block->last)->next = node;
	node->prev = block->last;
	block->last = node;
	block->blocks++;
	return node->data;
}

void fy_mmFree(fy_memblock *block, void *address) {

	fy_memblockNode *base = (fy_memblockNode *) ((fy_byte*) address
			- (int) ((fy_memblockNode*) 0)->data);
	base->prev->next = base->next;
	if (base == block->last) {
		block->last = base->prev;
	} else {
		base->next->prev = base->prev;
	}
	fy_free(base);
	block->blocks--;
}

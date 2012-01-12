/**
 *  Copyright 2010-2011 Yuxuan Huang. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fy_util/MemMan.h"
typedef struct fy_memblockNode {
	struct fy_memblockNode *prev;
	struct fy_memblockNode *next;
	FY_VLS(fy_byte,data);
} fy_memblockNode;

FY_ATTR_EXPORT void fy_mmInit(fy_memblock *block, fy_exception *exception) {
	block->last = block->first = fy_allocate(sizeof(fy_memblockNode),
			exception);
	FYEH();
	block->blocks = 0;
}

FY_ATTR_EXPORT void fy_mmDestroy(fy_memblock *block) {
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

FY_ATTR_EXPORT void *fy_mmAllocate(fy_memblock *block, int size, fy_exception *exception) {
	fy_memblockNode *node = fy_allocate(sizeof(fy_memblockNode) + size,
			exception);
	FYEH()NULL;
	((fy_memblockNode*) block->last)->next = node;
	node->prev = block->last;
	block->last = node;
	block->blocks++;
	return node->data;
}

FY_ATTR_EXPORT void fy_mmFree(fy_memblock *block, void *address) {

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

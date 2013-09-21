/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part offiscevm
 *
 *fiscevmis free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 *fiscevmis distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along withfiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fy_util/MemMan.h"
typedef struct fy_memblockNode {
	struct fy_memblockNode *prev;
	struct fy_memblockNode *next;
#ifdef FY_DEBUG
	fy_uint size;
#endif
	FY_VLS(fy_byte,data);
} fy_memblockNode;

FY_ATTR_EXPORT void fy_mmInit(fy_memblock *block, fy_exception *exception) {
	block->last = block->first = /*MANAGED*/fy_allocate(sizeof(fy_memblockNode),
			exception);
	FYEH();
	block->blocks = 0;
	block->oldTop = OLD_ENTRIES;
}

FY_ATTR_EXPORT void fy_mmDestroy(fy_memblock *block) {
	fy_memblockNode *node, *next;
#ifdef FY_DEBUG
	printf("Releasing %d managed memory blocks\n", block->blocks);
#endif
	node = block->first;
	do {
		next = node->next;
		fy_free(node);
		node = next;
	} while (node != NULL);
}

FY_ATTR_EXPORT void *fy_mmAllocate(fy_memblock *block, int size,
		fy_exception *exception) {
	fy_memblockNode *node = /*MANAGED*/fy_allocate(
			sizeof(fy_memblockNode) + size, exception);
	FYEH()NULL;
#ifdef FY_DEBUG
	block->size += node->size = sizeof(fy_memblockNode) + size;
#endif
	((fy_memblockNode*) block->last)->next = node;
	node->prev = block->last;
	block->last = node;
	block->blocks++;
	return node->data;
}

FY_ATTR_EXPORT void fy_mmFree(fy_memblock *block, void *address) {

	fy_memblockNode *base = (fy_memblockNode *) ((fy_byte*) address
			- (size_t) ((fy_memblockNode*) 0)->data);
#ifdef FY_DEBUG
	block->size -= base->size;
#endif
	base->prev->next = base->next;
	if (base == block->last) {
		block->last = base->prev;
	} else {
		base->next->prev = base->prev;
	}
	fy_free(base);
	block->blocks--;
}

void* fy_mmAllocatePerm(fy_memblock *block, size_t size,
		fy_exception *exception) {
	fy_int blocks = (size + 3) >> 2;
	if (block->posInOld >= block->oldTop - blocks) {
		block->gcProvider(block->gcContext, TRUE, exception);
		FYEH()NULL;
	}
	if (block->posInOld >= block->oldTop - blocks) {
		fy_fault(exception, NULL, "Out of memory: perm");
		FYEH()NULL;
	}
	/*memset(block->old + (block->oldTop -= blocks), 0, size);*/
	return block->old + (block->oldTop -= blocks);
}

fy_int fy_mmPermSize(fy_memblock *block) {
	return OLD_ENTRIES - block->oldTop;
}

void *fy_mmAllocateInEden(fy_memblock *block, fy_uint handle, fy_int size,
		fy_boolean gced, fy_exception *exception) {
	void *ret;
	if (size + block->posInEden >= EDEN_ENTRIES) {
		/*OOM, gc first*/
		if (gced) {
			fy_fault(exception, NULL,
					"Out of memoryE! Memory overflow size=%d EDEN:%d/%d COPY:%d/%d OLD:%d/%d\n",
					size, block->posInEden, EDEN_ENTRIES, block->posInYong,
					COPY_ENTRIES, block->posInOld, block->oldTop);
			return NULL;
		} else {
			block->gcProvider(block->gcContext, FALSE, exception);
			FYEH()NULL;
			return fy_mmAllocateInEden(block, handle, size, TRUE, exception);
		}
	}
	ret = block->eden + block->posInEden;
	block->posInEden += size;
	return ret;
}

void *fy_mmAllocateInOld(fy_memblock *block, fy_uint handle, fy_int size,
		fy_boolean gced, fy_exception *exception) {
	void *ret;
	if (size + block->posInOld >= block->oldTop) {
		/*OOM, gc first*/
		if (gced) {
			fy_fault(exception, NULL,
					"Out of memoryO! Memory overflow size=%d EDEN:%d/%d COPY:%d/%d OLD:%d/%d\n",
					size, block->posInEden, EDEN_ENTRIES, block->posInYong,
					COPY_ENTRIES, block->posInOld, block->oldTop);
			return NULL;
		} else {
			block->gcProvider(block->gcContext, TRUE, exception);
			return fy_mmAllocateInOld(block, handle, size, TRUE, exception);
		}
	}
	ret = block->old + block->posInOld;
	block->posInOld += size;
	return ret;
}

void *fy_mmAllocateDirectInEden(fy_memblock *block, fy_int size,
		fy_exception *exception) {
	void *ret;
	if (size + block->posInEden >= EDEN_ENTRIES) {
		/*OOM, gc first*/
		fy_fault(exception, NULL,
				"Out of memoryDE! Memory overflow size=%d EDEN:%d/%d COPY:%d/%d OLD:%d/%d\n",
				size, block->posInEden, EDEN_ENTRIES, block->posInYong,
				COPY_ENTRIES, block->posInOld, block->oldTop);
		return NULL;
	}
	ret = block->eden + block->posInEden;
	block->posInEden += size;
	return ret;
}

void *fy_mmAllocateDirectInCopy(fy_memblock *block, fy_int size,
		fy_exception *exception) {
	void *ret;
	if (size + block->posInYong >= COPY_ENTRIES) {
		/*OOM, gc first*/
		fy_fault(exception, NULL,
				"Out of memoryDC! Memory overflow size=%d EDEN:%d/%d COPY:%d/%d OLD:%d/%d\n",
				size, block->posInEden, EDEN_ENTRIES, block->posInYong,
				COPY_ENTRIES, block->posInOld, block->oldTop);
		return NULL;
	}
	ret = block->young + block->posInYong;
	block->posInYong += size;
	return ret;
}

void *fy_mmAllocateDirectInOld(fy_memblock *block, fy_int size,
		fy_exception *exception) {
	void *ret;
	if (size + block->posInOld >= block->oldTop) {
		/*OOM, gc first*/
		fy_fault(exception, NULL,
				"Out of memoryDO! Memory overflow size=%d EDEN:%d/%d COPY:%d/%d OLD:%d/%d\n",
				size, block->posInEden, EDEN_ENTRIES, block->posInYong,
				COPY_ENTRIES, block->posInOld, block->oldTop);
		return NULL;
	}
	ret = block->old + block->posInOld;
	block->posInOld += size;
	return ret;
}

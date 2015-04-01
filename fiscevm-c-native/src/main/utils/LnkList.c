/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of fiscevm
 *
 * fiscevm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * fiscevm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with fiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fy_util/LnkList.h"
#include "fy_util/Debug.h"

/*************private*****************/
/*
 //static struct fy_linkedListNode* lookupNode(fy_linkedList* list, void* content) {
 //	struct fy_linkedListNode* node = list->head;
 //	while ((node = node->link) != NULL) {
 //		if (node->info == content) {
 //			return node;
 //		}
 //	}
 //	return NULL;
 //}
 */
/*************public****************/

FY_ATTR_EXPORT void fy_linkedListInit(fy_memblock *block, fy_linkedList* list,
		fisce_exception *exception) {
	struct fy_linkedListNode* node = fy_mmAllocate(block,
			sizeof(struct fy_linkedListNode), exception);
	FYEH();
	list->head = node;
	list->last = node;
	list->count = 0;
}

static void fy_linkedListReleaser(fy_memblock *block, fy_linkedListNode *node,
		fisce_exception *exception) {
	fy_mmFree(block, node);
}

FY_ATTR_EXPORT void fy_linkedListDestroy(fy_memblock *block,
		fy_linkedList *list) {
	fy_linkedListTraverse(block, list, fy_linkedListReleaser, NULL);
	fy_mmFree(block, list->head);
}

FY_ATTR_EXPORT void* fy_linkedListRemove(fy_memblock *block,
		fy_linkedList* list, void* content) {
	struct fy_linkedListNode* node = list->head;
	while (node->next != NULL) {
		if (node->next->info == content) {
			return fy_linkedListRemoveNode(block, list, node->next);
		}
		node = node->next;
	}
	return NULL;
}

FY_ATTR_EXPORT void* fy_linkedListRemoveNode(fy_memblock *block,
		fy_linkedList* list, fy_linkedListNode *node) {
	void *ret;
	node->prev->next = node->next;
	if (node == list->last) {
		list->last = node->prev;
	} else {
		node->next->prev = node->prev;
	}
	ret = node->info;
	fy_mmFree(block, node);
	list->count--;
	return ret;
}

FY_ATTR_EXPORT fy_linkedListNode* fy_linkedListAppend(fy_memblock *block,
		fy_linkedList* list, void* content, fisce_exception *exception) {
	struct fy_linkedListNode* node = fy_mmAllocate(block,
			sizeof(struct fy_linkedListNode), exception);
	if (node == NULL) {
		fy_fault(exception, NULL, "OUT OF MEMORY!!");
		return NULL;
	}

	list->last->next = node;
	node->prev = list->last;
	node->info = content;
	list->last = node;

	list->count++;
	return node;
}

FY_ATTR_EXPORT void fy_linkedListTraverse(fy_memblock *block,
		fy_linkedList* list,
		void (*fun)(fy_memblock *block, struct fy_linkedListNode* node,
				fisce_exception *exception), fisce_exception *exception) {
	struct fy_linkedListNode* node = list->head;
	struct fy_linkedListNode* next = node->next;
#ifdef FY_DEBUG
	int i = 0;
#endif
	while ((node = next) != NULL) {
#ifdef FY_DEBUG
		i++;
#endif
		next = node->next;
		fun(block, node, exception);
		FYEH();
	}
#ifdef FY_DEBUG
	if (i != list->count) {
		fy_fault(exception, NULL, "err: %d %d", i, list->count);
		return;
	}
#endif
}

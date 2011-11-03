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

#include "fyc/LinkedList.h"

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

void fy_linkedListInit(fy_linkedList* list) {
	struct fy_linkedListNode* node = vm_allocate(
			sizeof(struct fy_linkedListNode));
	list->head = node;
	list->last = node;
	list->count = 0;
}

static void fy_linkedListReleaser(fy_linkedListNode *node) {
	vm_free(node);
}

void fy_linkedListDestroy(fy_linkedList *list) {
	fy_linkedListTraverse(list, fy_linkedListReleaser);
	vm_free(list->head);
}

void* fy_linkedListRemove(fy_linkedList* list, void* content) {
	struct fy_linkedListNode* node = list->head;
	while (node->next != NULL) {
		if (node->next->info == content) {
			return fy_linkedListRemoveNode(list, node->next);
		}
		node = node->next;
	}
	return NULL;
}

void* fy_linkedListRemoveNode(fy_linkedList* list, fy_linkedListNode *node) {
	void *ret;
	node->prev->next = node->next;
	if (node == list->last) {
		list->last = node->prev;
	} else {
		node->next->prev = node->prev;
	}
	ret = node->info;
	vm_free(node);
	list->count--;
	return ret;
}

fy_linkedListNode* fy_linkedListAppend(fy_linkedList* list, void* content) {
	struct fy_linkedListNode* node = vm_allocate(
			sizeof(struct fy_linkedListNode));
	if (node == NULL) {
		vm_die("OUT OF MEMORY!!");
	}

	list->last->next = node;
	node->prev = list->last;
	node->info = content;
	list->last = node;

	list->count++;
	return node;
}

void fy_linkedListTraverse(fy_linkedList* list,
		void(*fun)(struct fy_linkedListNode* node)) {
	struct fy_linkedListNode* node = list->head;
	struct fy_linkedListNode* next = node->next;
#ifdef _DEBUG
	int i = 0;
#endif
	while ((node = next) != NULL) {
#ifdef _DEBUG
		i++;
#endif
		next = node->next;
		fun(node);
	}
#ifdef _DEBUG
	if (i != list->count) {
		vm_die("err: %d %d", i, list->count);
	}
#endif
}

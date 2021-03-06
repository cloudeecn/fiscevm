/**
 *  Copyright 2010-2015 Yuxuan Huang. All rights reserved.
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

#ifndef FY_LINKEDLIST_H_
#define FY_LINKEDLIST_H_

#include "fy_util/Portable.h"
#include "fy_util/MemMan.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct fy_linkedListNode {
	void* info;
	struct fy_linkedListNode *next; /* Point to next node */
	struct fy_linkedListNode *prev; /* Point to prev node */
} fy_linkedListNode;

typedef struct fy_linkedList {
	struct fy_linkedListNode *head; /* Header node(persist, just for convenient, doesn't store data) */
	struct fy_linkedListNode *last; /* Point to the last node of the link */
	int count; /* count of the node except the head node */
} fy_linkedList;

FY_ATTR_EXPORT void fy_linkedListInit(fy_memblock *block, fy_linkedList* list,
		fisce_exception *exception);
FY_ATTR_EXPORT void fy_linkedListDestroy(fy_memblock *block, fy_linkedList *list);
FY_ATTR_EXPORT void* fy_linkedListRemove(fy_memblock *block, fy_linkedList* list,
		void* content);

FY_ATTR_EXPORT void* fy_linkedListRemoveNode(fy_memblock *block,
		fy_linkedList* list, fy_linkedListNode *node);
FY_ATTR_EXPORT fy_linkedListNode* fy_linkedListAppend(fy_memblock *block,
		fy_linkedList* list, void* content, fisce_exception *exception);

FY_ATTR_EXPORT void fy_linkedListTraverse(
		fy_memblock *block,
		fy_linkedList* list,
		void(*fun)(fy_memblock *block, struct fy_linkedListNode*,
				fisce_exception *exception), fisce_exception *exception);

#ifdef	__cplusplus
}
#endif

#endif /* FY_LINKEDLIST_H_ */

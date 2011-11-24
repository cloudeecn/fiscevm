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

#ifndef FY_LINKEDLIST_H_
#define FY_LINKEDLIST_H_

#include "../fisceprt.h"
#include "../fiscestu.h"
#include "Debug.h"

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

void fy_linkedListInit(fy_linkedList* list);
void fy_linkedListDestroy(fy_linkedList *list);
void* fy_linkedListRemove(fy_linkedList* list, void* content);

void* fy_linkedListRemoveNode(fy_linkedList* list, fy_linkedListNode *node);
fy_linkedListNode* fy_linkedListAppend(fy_linkedList* list, void* content);

void fy_linkedListTraverse(fy_linkedList* list,
		void(*fun)(struct fy_linkedListNode*));

#ifdef	__cplusplus
}
#endif

#endif /* FY_LINKEDLIST_H_ */

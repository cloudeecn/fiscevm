/**
 *  Copyright 2010 Yuxuan Huang. All rights reserved.
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
#include "fy_util/ArrList.h"

void fy_arrayListInit(fy_memblock *block, fy_arrayList *list, fy_int initCap,
		fy_exception *exception) {
	list->maxLength = initCap;
	list->length = 0;
	fy_mmAllocate(block, sizeof(void*) * initCap, exception);
	fy_exceptionCheckAndReturn(exception);
}

void fy_arrayListDestroy(fy_memblock *block, fy_arrayList *list) {
	fy_mmFree(block, list);
	list->length = -1;
	list->maxLength = -1
}

static void ensureCap(fy_memblock *block, fy_arrayList *list, fy_int length,
		fy_exception *exception) {

}

void fy_arrayListAdd(fy_memblock *block, fy_arrayList *list, void *entry,
		fy_exception *exception) {
	fy_int len = list->length + 1;
	ensureCap(block, list, len, exception);
	fy_exceptionCheckAndReturn(exception);
	list->data[list->length] = entry;
	list->length = len;
}

void fy_arrayListRemove(fy_memblock *block, fy_arrayList *list, fy_int pos,
		fy_exception *exception) {
	if (pos < 0 || pos >= list->length) {
		fy_fault(exception, NULL, "Index out of bound %d/%d", pos,
				list->length);
		return;
	}
	if (pos == list->length - 1) {
		list->length--;
	} else {
		memmove(list->data + pos, list->data + pos + 1, list->length - pos - 1);
	}
}

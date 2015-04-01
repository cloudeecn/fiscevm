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

#include "fy_util/ArrList.h"

FY_ATTR_EXPORT fy_arrayList* fy_arrayListCreatePerm(fy_memblock *block,
		fisce_short entrySize, fisce_int maxCap, fisce_exception *exception) {
	fy_arrayList *list = fy_mmAllocatePerm(block,
			sizeof(fy_arrayList) + entrySize * maxCap, exception);
	FYEH()NULL;
	list->maxLength = maxCap;
	list->data = list->staticData;
	list->perm = TRUE;
	list->entrySize = entrySize;
	return list;
}

FY_ATTR_EXPORT void fy_arrayListInit(fy_memblock *block, fy_arrayList *list,
		fisce_short entrySize, fisce_int initCap, fisce_exception *exception) {
	list->maxLength = initCap;
	list->length = 0;
	list->entrySize = entrySize;
	list->perm = FALSE;
	list->data = fy_mmAllocate(block, entrySize * initCap, exception);
	FYEH();
}

FY_ATTR_EXPORT void fy_arrayListDestroy(fy_memblock *block, fy_arrayList *list) {
#ifdef FY_STRICT_CHECK
	if(list->perm){
		fy_fault(NULL, NULL, "Can't destroy perm list");
	} else {
		fy_mmValidate(list->data);
	}
#endif
	fy_mmFree(block, list->data);
	list->data = NULL;
	list->length = -1;
	list->maxLength = -1;
}

static void ensureCap(fy_memblock *block, fy_arrayList *list, fisce_int length,
		fisce_exception *exception) {
	void *data;
#ifdef FY_STRICT_CHECK
	if(!list->perm){
		fy_mmValidate(list->data);
	}
#endif
	if (list->maxLength < length) {
#ifdef FY_STRICT_CHECK
	if(list->perm){
		fy_fault(NULL, NULL, "Can't enlarge perm list");
	}
#endif
		while (list->maxLength < length)
			list->maxLength <<= 1;
		data = fy_mmAllocate(block, list->maxLength * list->entrySize,
				exception);
		FYEH();
		memcpy(data, list->data, list->length * list->entrySize);
		fy_mmFree(block, list->data);
		list->data = data;
#ifdef FY_STRICT_CHECK
		fy_mmValidate(list->data);
#endif
	}
}

FY_ATTR_EXPORT void fy_arrayListAdd(fy_memblock *block, fy_arrayList *list,
		void *data, fisce_exception *exception) {
#ifdef FY_STRICT_CHECK
	if(!list->perm){
		fy_mmValidate(list->data);
	}
#endif
	ensureCap(block, list, list->length + 1, exception);
	FYEH();

	memcpy((fisce_byte*) list->data + (list->length++) * list->entrySize, data,
			list->entrySize);
#ifdef FY_STRICT_CHECK
	if(!list->perm){
		fy_mmValidate(list->data);
	}
#endif
}

FY_ATTR_EXPORT void fy_arrayListRemove(fy_memblock *block, fy_arrayList *list,
		fisce_int pos, fisce_exception *exception) {
#ifdef FY_STRICT_CHECK
	if(!list->perm){
		fy_mmValidate(list->data);
	}
#endif
	if (pos < 0 || pos >= list->length) {
		fy_fault(exception, NULL, "Index out of bound %d/%d", pos,
				list->length);
		return;
	}
	if (pos < list->length - 1) {
		memmove((fisce_byte*) list->data + pos * list->entrySize,
				(fisce_byte*) list->data + (pos + 1) * list->entrySize,
				list->entrySize * (list->length - pos - 1));
	}
	list->length--;
#ifdef FY_STRICT_CHECK
	if(!list->perm){
		fy_mmValidate(list->data);
	}
#endif
}

static void* get(fy_arrayList *list, fisce_int pos, void *storage) {
	void *ret = (fisce_byte*) list->data + pos * list->entrySize;
#ifdef FY_STRICT_CHECK
	if(!list->perm){
		fy_mmValidate(list->data);
	}
#endif
	if (storage != NULL) {
		memcpy(storage, ret, list->entrySize);
		return storage;
	}
	return ret;
}

FY_ATTR_EXPORT void *fy_arrayListGet(fy_memblock *block, fy_arrayList *list,
		fisce_int pos, void *storage) {
	if (pos < 0 || pos >= list->length) {
		return NULL;
	}
	return get(list, pos, storage);
}
FY_ATTR_EXPORT void *fy_arrayListPop(fy_memblock *block, fy_arrayList *list,
		void *storage) {
	if (list->length == 0) {
		return NULL;
	}
	return get(list, --list->length, storage);
}

FY_ATTR_EXPORT void fy_arrayListClear(fy_memblock *block, fy_arrayList *list) {
	list->length = 0;
}

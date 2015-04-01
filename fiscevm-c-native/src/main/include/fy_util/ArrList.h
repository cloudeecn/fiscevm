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

#ifndef FY_ARRLIST_H_
#define FY_ARRLIST_H_

#include "fy_util/Portable.h"
#include "fy_util/MemMan.h"
#ifdef	__cplusplus
extern "C" {
#endif

typedef struct fy_arrayList {
	fisce_int length;
	fisce_int maxLength;
	fisce_short entrySize;
	fisce_short perm;
	void *data;
	FY_VLS(fisce_uint,staticData);
} fy_arrayList;
FY_ATTR_EXPORT fy_arrayList* fy_arrayListCreatePerm(fy_memblock *block,
		fisce_short entrySize, fisce_int maxCap, fisce_exception *exception);

FY_ATTR_EXPORT void fy_arrayListInit(fy_memblock *block, fy_arrayList *list,
		fisce_short entrySize, fisce_int initCap, fisce_exception *exception);

FY_ATTR_EXPORT void fy_arrayListDestroy(fy_memblock *block, fy_arrayList *list);

FY_ATTR_EXPORT void fy_arrayListAdd(fy_memblock *block, fy_arrayList *list,
		void *data, fisce_exception *exception);

FY_ATTR_EXPORT void *fy_arrayListGet(fy_memblock *block, fy_arrayList *list,
		fisce_int pos, void *storage);

FY_ATTR_EXPORT void *fy_arrayListPop(fy_memblock *block, fy_arrayList *list,
		void *storage);

FY_ATTR_EXPORT void fy_arrayListRemove(fy_memblock *block, fy_arrayList *list,
		fisce_int pos, fisce_exception *exception);

FY_ATTR_EXPORT void fy_arrayListClear(fy_memblock *block, fy_arrayList *list);

#ifdef	__cplusplus
}
#endif

#endif /* ARRLIST_H_ */

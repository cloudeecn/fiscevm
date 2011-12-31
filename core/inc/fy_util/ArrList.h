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

#ifndef FY_ARRLIST_H_
#define FY_ARRLIST_H_

#include "../fisceprt.h"
#include "MemMan.h"
#ifdef	__cplusplus
extern "C" {
#endif

typedef struct fy_arrayList {
	fy_int length;
	fy_int maxLength;
	size_t entrySize;
	void *data;
} fy_arrayList;
FY_ATTR_EXPORT void fy_arrayListInit(fy_memblock *block, fy_arrayList *list, size_t entrySize,
		fy_int initCap, fy_exception *exception);

FY_ATTR_EXPORT void fy_arrayListDestroy(fy_memblock *block, fy_arrayList *list);

FY_ATTR_EXPORT void fy_arrayListAdd(fy_memblock *block, fy_arrayList *list, void *data,
		fy_exception *exception);

FY_ATTR_EXPORT void *fy_arrayListGet(fy_memblock *block, fy_arrayList *list, fy_uint pos,
		void *storage);

FY_ATTR_EXPORT void *fy_arrayListPop(fy_memblock *block, fy_arrayList *list, void *storage);

FY_ATTR_EXPORT void fy_arrayListRemove(fy_memblock *block, fy_arrayList *list, fy_int pos,
		fy_exception *exception);

FY_ATTR_EXPORT void fy_arrayListClear(fy_memblock *block, fy_arrayList *list);

#ifdef	__cplusplus
}
#endif

#endif /* ARRLIST_H_ */

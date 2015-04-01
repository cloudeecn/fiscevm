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

#ifndef FY_MEMMAN_H_
#define FY_MEMMAN_H_

#include "fy_util/Portable.h"
#include "fyc/Config.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct fy_memblock {
	void (*gcProvider)(void *context,fisce_boolean memoryStressed, fisce_exception *exception);
	void *first;
	void *last;
	fisce_int blocks;
#ifdef FY_DEBUG
	fisce_uint size;
#endif
	void *gcContext;
	fisce_int posInEden;
	fisce_uint eden[EDEN_ENTRIES];
	fisce_int posInYong;
	fisce_uint youngId;
	fisce_uint young[COPY_ENTRIES * 2];
	fisce_int posInOld;
	fisce_int oldReleasedSize;
	fisce_int oldTop;
	fisce_uint old[OLD_ENTRIES];
} fy_memblock;

FY_ATTR_EXPORT void fy_mmInit(fy_memblock *block, fisce_exception *exception);
FY_ATTR_EXPORT void fy_mmDestroy(fy_memblock *block);

FY_ATTR_EXPORT void* fy_mmAllocate(fy_memblock *block, int size,
		fisce_exception *exception);
FY_ATTR_EXPORT void fy_mmFree(fy_memblock *block, void *address);
FY_ATTR_EXPORT void fy_mmValidate(void *address);

void* fy_mmAllocatePerm(fy_memblock *block, size_t size,
		fisce_exception *exception);

fisce_int fy_mmPermSize(fy_memblock *block);

void *fy_mmAllocateInEden(fy_memblock *block, fisce_uint handle, fisce_int size,
		fisce_boolean gced, fisce_exception *exception);

void *fy_mmAllocateInOld(fy_memblock *block, fisce_uint handle, fisce_int size,
		fisce_boolean gced, fisce_exception *exception);

void *fy_mmAllocateDirectInEden(fy_memblock *block, fisce_int size,
		fisce_exception *exception);

void *fy_mmAllocateDirectInCopy(fy_memblock *block, fisce_int size,
		fisce_exception *exception);

void *fy_mmAllocateDirectInOld(fy_memblock *block, fisce_int size,
		fisce_exception *exception);

#ifdef	__cplusplus
}
#endif
#endif /* MEMMAN_H_ */

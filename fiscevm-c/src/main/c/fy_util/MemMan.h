/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of libfisce.
 *
 * libfisce is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * libfisce is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libfisce.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FY_MEMMAN_H_
#define FY_MEMMAN_H_

#include "../fisceprt.h"
#define EDEN_ENTRIES  32768
#define COPY_ENTRIES  32768
#define OLD_ENTRIES 1800000

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct fy_memblock {
	void (*gcProvider)(void *context,fy_boolean memoryStressed, fy_exception *exception);
	void *first;
	void *last;
	fy_int blocks;
#ifdef FY_DEBUG
	fy_uint size;
#endif
	void *gcContext;
	fy_int posInEden;
	fy_uint eden[EDEN_ENTRIES];
	fy_int posInYong;
	fy_uint youngId;
	fy_uint young[COPY_ENTRIES * 2];
	fy_int posInOld;
	fy_int oldReleasedSize;
	fy_int oldTop;
	fy_uint old[OLD_ENTRIES];
} fy_memblock;

FY_ATTR_EXPORT void fy_mmInit(fy_memblock *block, fy_exception *exception);
FY_ATTR_EXPORT void fy_mmDestroy(fy_memblock *block);

FY_ATTR_EXPORT void* fy_mmAllocate(fy_memblock *block, int size,
		fy_exception *exception);
FY_ATTR_EXPORT void fy_mmFree(fy_memblock *block, void *address);

void* fy_mmAllocatePerm(fy_memblock *block, size_t size,
		fy_exception *exception);

fy_int fy_mmPermSize(fy_memblock *block);

void *fy_mmAllocateInEden(fy_memblock *block, fy_uint handle, fy_int size,
		fy_boolean gced, fy_exception *exception);

void *fy_mmAllocateInOld(fy_memblock *block, fy_uint handle, fy_int size,
		fy_boolean gced, fy_exception *exception);

void *fy_mmAllocateDirectInEden(fy_memblock *block, fy_int size,
		fy_exception *exception);

void *fy_mmAllocateDirectInCopy(fy_memblock *block, fy_int size,
		fy_exception *exception);

void *fy_mmAllocateDirectInOld(fy_memblock *block, fy_int size,
		fy_exception *exception);

#ifdef	__cplusplus
}
#endif
#endif /* MEMMAN_H_ */

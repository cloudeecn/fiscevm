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

#ifndef FY_MEMMAN_H_
#define FY_MEMMAN_H_

#include "../fisceprt.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct fy_memblock {
	void *first;
	void *last;
	fy_int blocks;
} fy_memblock;

FY_ATTR_EXPORT void fy_mmInit(fy_memblock *block, fy_exception *exception);
FY_ATTR_EXPORT void fy_mmDestroy(fy_memblock *block);

FY_ATTR_EXPORT void* fy_mmAllocate(fy_memblock *block, int size, fy_exception *exception);
FY_ATTR_EXPORT void fy_mmFree(fy_memblock *block, void *address);

#ifdef	__cplusplus
}
#endif
#endif /* MEMMAN_H_ */

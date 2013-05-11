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

#ifndef FY_HASHMAP_H_
#define FY_HASHMAP_H_

#include "../fisceprt.h"
#include "String.h"
#include <string.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct fy_hashMap {
	fy_char loadFactor;
	fy_char perm;
	fy_uint bucketsCount;
	void **buckets;
	fy_uint size;
} fy_hashMap;

FY_ATTR_EXPORT void fy_hashMapInit(fy_memblock *mem, fy_hashMap *this,
		fy_uint initSize, fy_uint loadFactor, fy_exception *exception);
FY_ATTR_EXPORT void fy_hashMapInitPerm(fy_memblock *mem, fy_hashMap *this,
		fy_uint initSize, fy_exception *exception);
FY_ATTR_EXPORT void *fy_hashMapPut(fy_memblock *mem, fy_hashMap *this,
		fy_str *key, void *value, fy_exception *exception);
/*
FY_ATTR_EXPORT void *fy_hashMapPutUtf8(fy_memblock *mem, fy_hashMap *this,
		const char *keyUtf8, void *value, fy_exception *exception);
*/
FY_ATTR_EXPORT void *fy_hashMapPutVA(fy_memblock *mem, fy_hashMap *this,
		fy_strVA *va, void *value, fy_exception *exception);
FY_ATTR_EXPORT void *fy_hashMapGet(fy_memblock *mem, fy_hashMap *this,
		fy_str *key);
FY_ATTR_EXPORT void* fy_hashMapGetVA(fy_memblock *mem, fy_hashMap *map,
		fy_strVA *va);
FY_ATTR_EXPORT void fy_hashMapDestroy(fy_memblock *mem, fy_hashMap *this);
FY_ATTR_EXPORT void fy_hashMapEachValue(fy_memblock *mem, fy_hashMap *map,
		void (*fn)(fy_str *key, void *value, void *addition), void *addition);
#ifdef	__cplusplus
}
#endif

#endif /* FY_HASHMAP_H_ */

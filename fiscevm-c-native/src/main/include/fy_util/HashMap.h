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

#ifndef FY_HASHMAP_H_
#define FY_HASHMAP_H_

#include "fy_util/Portable.h"
#include "fy_util/MemMan.h"
#include "fy_util/String.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct fy_hashMap {
	fisce_char loadFactor;
	fisce_char perm;
	fisce_uint bucketsCount;
	void **buckets;
	fisce_uint size;
} fy_hashMap;

FY_ATTR_EXPORT void fy_hashMapInit(fy_memblock *mem, fy_hashMap *this,
		fisce_uint initSize, fisce_uint loadFactor, fisce_exception *exception);
FY_ATTR_EXPORT void fy_hashMapInitPerm(fy_memblock *mem, fy_hashMap *this,
		fisce_uint initSize, fisce_exception *exception);
FY_ATTR_EXPORT void *fy_hashMapPut(fy_memblock *mem, fy_hashMap *this,
		fy_str *key, void *value, fisce_exception *exception);
FY_ATTR_EXPORT void *fy_hashMapPutConst(fy_memblock *mem, fy_hashMap *this,
        fy_str *key, const void *value, fisce_exception *exception);
/*
FY_ATTR_EXPORT void *fy_hashMapPutUtf8(fy_memblock *mem, fy_hashMap *this,
		const char *keyUtf8, void *value, fisce_exception *exception);
*/
FY_ATTR_EXPORT void *fy_hashMapPutVA(fy_memblock *mem, fy_hashMap *this,
		fy_strVA *va, void *value, fisce_exception *exception);
FY_ATTR_EXPORT void *fy_hashMapGet(fy_memblock *mem, fy_hashMap *this,
		fy_str *key);
FY_ATTR_EXPORT const void* fy_hashMapGetConst(fy_memblock *mem, fy_hashMap *this,
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

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

#ifndef FY_HASHMAPI_H_
#define FY_HASHMAPI_H_

#include "fy_util/Portable.h"
#include "fy_util/MemMan.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct fy_hashMapI {
	fisce_char loadFactor;
	fisce_char perm;
	fisce_uint bucketsFact;
	fisce_uint bucketsSizeM1;
	void **buckets;
	fisce_uint size;
	fisce_int nullValue;
} fy_hashMapI;

FY_ATTR_EXPORT void fy_hashMapIInit(fy_memblock *mem, fy_hashMapI *this,
		fisce_uint initSize, fisce_uint loadFactor, fisce_int nullValue,
		fisce_exception *exception);
FY_ATTR_EXPORT void fy_hashMapIInitPerm(fy_memblock *mem, fy_hashMapI *this,
		fisce_uint initFact, fisce_int nullValue, fisce_exception *exception);
FY_ATTR_EXPORT fisce_int fy_hashMapIPut(fy_memblock *mem, fy_hashMapI *this,
		fisce_int, fisce_int, fisce_exception *exception);
FY_ATTR_EXPORT fisce_int fy_hashMapIRemove(fy_memblock *mem, fy_hashMapI *this,
		fisce_int key);
FY_ATTR_EXPORT fisce_int fy_hashMapIGet(fy_memblock *mem, fy_hashMapI *this,
		fisce_int key);
FY_ATTR_EXPORT fisce_int fy_hashMapIInc(fy_memblock *mem, fy_hashMapI *this,
		fisce_int key, fisce_int value, fisce_exception *exception);
FY_ATTR_EXPORT void fy_hashMapIDestroy(fy_memblock *mem, fy_hashMapI *this);
FY_ATTR_EXPORT void fy_hashMapIEachValue(fy_memblock *mem, fy_hashMapI *map,
		void (*fn)(fisce_int key, fisce_int value, fisce_int nullValue, void *addition),
		void *addition);

#ifdef	__cplusplus
}
#endif

#endif /* FY_HASHMAPI_H_ */

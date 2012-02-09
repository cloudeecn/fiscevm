/**
 *  Copyright 2010-2011 Yuxuan Huang. All rights reserved.
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

#ifndef FY_HASHMAPI_H_
#define FY_HASHMAPI_H_

#include "../fisceprt.h"
#include "String.h"
#include <string.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct fy_hashMapI {
	fy_char loadFactor;
	fy_uint bucketsFact;
	fy_uint bucketsSizeM1;
	void **buckets;
	fy_uint size;
	fy_int nullValue;
} fy_hashMapI;

FY_ATTR_EXPORT void fy_hashMapIInit(fy_memblock *mem, fy_hashMapI *this,
		fy_uint initSize, fy_uint loadFactor, fy_int nullValue,
		fy_exception *exception);
FY_ATTR_EXPORT void fy_hashMapIInitSimple(fy_memblock *mem, fy_hashMapI *this,
		fy_int nullValue, fy_exception *exception);
FY_ATTR_EXPORT fy_int fy_hashMapIPut(fy_memblock *mem, fy_hashMapI *this,
		fy_int, fy_int, fy_exception *exception);
FY_ATTR_EXPORT fy_int fy_hashMapIGet(fy_memblock *mem, fy_hashMapI *this,
		fy_int key);
FY_ATTR_EXPORT void fy_hashMapIDestroy(fy_memblock *mem, fy_hashMapI *this);
FY_ATTR_EXPORT void fy_hashMapIEachValue(fy_memblock *mem, fy_hashMapI *map,
		void(*fn)(fy_int key, fy_int value, fy_int nullValue, void *addition),
		void *addition);

#ifdef	__cplusplus
}
#endif

#endif /* FY_HASHMAPI_H_ */

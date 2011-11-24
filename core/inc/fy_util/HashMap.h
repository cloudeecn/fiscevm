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

#ifndef FY_HASHMAP_H_
#define FY_HASHMAP_H_

#include "../fisceprt.h"
#include "../fiscestu.h"
#include "String.h"
#include "VMContext.h"
#include <string.h>

#ifdef	__cplusplus
extern "C" {
#endif



typedef struct fy_hashMap {
	fy_char loadFactor;
	fy_uint bucketsCount;
	fy_hashMapEntry **buckets;
	fy_uint size;
} fy_hashMap;

void fy_hashMapInit(fy_context *context, fy_hashMap *this, fy_uint initSize,
		fy_uint loadFactor);
void fy_hashMapInitSimple(fy_context *context, fy_hashMap *this);
void *fy_hashMapPut(fy_context *context, fy_hashMap *this, fy_str *key,
		void *value);
int fy_hashMapPutInt(fy_context *context, fy_hashMap *this, fy_str *key,
		int value);
void *fy_hashMapPutUtf8(fy_context *context, fy_hashMap *this,
		const char *keyUtf8, void *value);
void *fy_hashMapGet(fy_context *context, fy_hashMap *this, fy_str *key);
int fy_hashMapGetInt(fy_context *context, fy_hashMap *this, fy_str *key);
void fy_hashMapDestroy(fy_context *context, fy_hashMap *this);
#ifdef	__cplusplus
}
#endif

#endif /* FY_HASHMAP_H_ */

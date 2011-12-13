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

#ifndef FY_HEAP_H_
#define FY_HEAP_H_

#include "../fisceprt.h"
#include "../fy_util/Debug.h"
#include "../fy_util/BitSet.h"
#include "../fiscestu.h"
#include "VMContext.h"

#ifdef	__cplusplus
extern "C" {
#endif

void fy_heapRelease(fy_context *context, fy_uint handle);
int fy_heapAllocate(fy_context *context, fy_class *clazz,
		fy_exception *exception);

#define fy_heapGetObject(CONTEXT,HANDLE) ((CONTEXT)->objects+(HANDLE))

fy_class* fy_heapGetClassOfObject(fy_context *context, fy_int handle);

int fy_heapAllocateArray(fy_context *context, fy_class *clazz, int length,
		fy_exception *exception);

fy_int fy_heapArrayLength(fy_context *context, fy_int handle,
		fy_exception *exception);
void fy_heapArrayCopy(fy_context *context, fy_int src, fy_int srcPos,
		fy_int dest, fy_int destPos, fy_int len, fy_exception *exception);
fy_int fy_heapClone(fy_context *context, fy_int src, fy_exception *exception);
void fy_heapGC(fy_context *context, fy_exception *exception);

fy_str* fy_heapGetString(fy_context *context, fy_int handle, fy_str *target,
		fy_exception *exception);
fy_int fy_heapMakeString(fy_context *context, fy_str *target,
		fy_exception *exception);
fy_int fy_heapLiteral(fy_context *context, fy_str *str, fy_exception *exception);

void fy_heapBeginProtect(fy_context *context);

void fy_heapEndProtect(fy_context *context);

fy_boolean fy_heapGetArrayBoolean(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception);
fy_int fy_heapGetArrayHandle(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception);
fy_byte fy_heapGetArrayByte(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception);
fy_byte *fy_heapGetArrayBytes(fy_context *context, fy_int handle,
		fy_exception *exception);
fy_short fy_heapGetArrayShort(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception);
fy_char fy_heapGetArrayChar(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception);
fy_int fy_heapGetArrayInt(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception);
fy_long fy_heapGetArrayLong(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception);
fy_float fy_heapGetArrayFloat(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception);
fy_double fy_heapGetArrayDouble(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception);

void fy_heapPutArrayBoolean(fy_context *context, fy_int handle, fy_int index,
		fy_boolean value, fy_exception *exception);
void fy_heapPutArrayHandle(fy_context *context, fy_int handle, fy_int index,
		fy_int value, fy_exception *exception);
void fy_heapPutArrayByte(fy_context *context, fy_int handle, fy_int index,
		fy_byte value, fy_exception *exception);
void fy_heapPutArrayShort(fy_context *context, fy_int handle, fy_int index,
		fy_short value, fy_exception *exception);
void fy_heapPutArrayChar(fy_context *context, fy_int handle, fy_int index,
		fy_char value, fy_exception *exception);
void fy_heapPutArrayInt(fy_context *context, fy_int handle, fy_int index,
		fy_int value, fy_exception *exception);
void fy_heapPutArrayLong(fy_context *context, fy_int handle, fy_int index,
		fy_long value, fy_exception *exception);
void fy_heapPutArrayFloat(fy_context *context, fy_int handle, fy_int index,
		fy_float value, fy_exception *exception);
void fy_heapPutArrayDouble(fy_context *context, fy_int handle, fy_int index,
		fy_double value, fy_exception *exception);

fy_boolean fy_heapGetFieldBoolean(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception);
fy_int fy_heapGetFieldHandle(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception);
fy_byte fy_heapGetFieldByte(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception);
fy_short fy_heapGetFieldShort(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception);
fy_char fy_heapGetFieldChar(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception);
fy_int fy_heapGetFieldInt(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception);
fy_long fy_heapGetFieldLong(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception);
fy_float fy_heapGetFieldFloat(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception);
fy_double fy_heapGetFieldDouble(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception);

void fy_heapPutFieldBoolean(fy_context *context, fy_int handle, fy_field *field,
		fy_boolean value, fy_exception *exception);
void fy_heapPutFieldHandle(fy_context *context, fy_int handle, fy_field *field,
		fy_int value, fy_exception *exception);
void fy_heapPutFieldByte(fy_context *context, fy_int handle, fy_field *field,
		fy_byte value, fy_exception *exception);
void fy_heapPutFieldShort(fy_context *context, fy_int handle, fy_field *field,
		fy_short value, fy_exception *exception);
void fy_heapPutFieldChar(fy_context *context, fy_int handle, fy_field *field,
		fy_char value, fy_exception *exception);
void fy_heapPutFieldInt(fy_context *context, fy_int handle, fy_field *field,
		fy_int value, fy_exception *exception);
void fy_heapPutFieldLong(fy_context *context, fy_int handle, fy_field *field,
		fy_long value, fy_exception *exception);
void fy_heapPutFieldFloat(fy_context *context, fy_int handle, fy_field *field,
		fy_float value, fy_exception *exception);
void fy_heapPutFieldDouble(fy_context *context, fy_int handle, fy_field *field,
		fy_double value, fy_exception *exception);

fy_boolean fy_heapGetStaticBoolean(fy_context *context, fy_field *field,
		fy_exception *exception);
fy_int fy_heapGetStaticHandle(fy_context *context, fy_field *field,
		fy_exception *exception);
fy_byte fy_heapGetStaticByte(fy_context *context, fy_field *field,
		fy_exception *exception);
fy_short fy_heapGetStaticShort(fy_context *context, fy_field *field,
		fy_exception *exception);
fy_char fy_heapGetStaticChar(fy_context *context, fy_field *field,
		fy_exception *exception);
fy_int fy_heapGetStaticInt(fy_context *context, fy_field *field,
		fy_exception *exception);
fy_long fy_heapGetStaticLong(fy_context *context, fy_field *field,
		fy_exception *exception);
fy_float fy_heapGetStaticFloat(fy_context *context, fy_field *field,
		fy_exception *exception);
fy_double fy_heapGetStaticDouble(fy_context *context, fy_field *field,
		fy_exception *exception);

void fy_heapPutStaticBoolean(fy_context *context, fy_field *field,
		fy_boolean value, fy_exception *exception);
void fy_heapPutStaticHandle(fy_context *context, fy_field *field, fy_int value,
		fy_exception *exception);
void fy_heapPutStaticByte(fy_context *context, fy_field *field, fy_byte value,
		fy_exception *exception);
void fy_heapPutStaticShort(fy_context *context, fy_field *field, fy_short value,
		fy_exception *exception);
void fy_heapPutStaticChar(fy_context *context, fy_field *field, fy_char value,
		fy_exception *exception);
void fy_heapPutStaticInt(fy_context *context, fy_field *field, fy_int value,
		fy_exception *exception);
void fy_heapPutStaticLong(fy_context *context, fy_field *field, fy_long value,
		fy_exception *exception);
void fy_heapPutStaticFloat(fy_context *context, fy_field *field, fy_float value,
		fy_exception *exception);
void fy_heapPutStaticDouble(fy_context *context, fy_field *field,
		fy_double value, fy_exception *exception);

#ifdef	__cplusplus
}
#endif
#endif /* FY_HEAP_H_ */

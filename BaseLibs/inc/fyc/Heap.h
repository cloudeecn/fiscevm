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

#ifndef HEAP_H_
#define HEAP_H_

#include "Portable.h"
#include "Structs.h"
#include "Debug.h"

#ifdef	__cplusplus
extern "C" {
#endif

void fy_heapRelease(fy_VMContext *context, juint handle);
int fy_heapAllocate(fy_VMContext *context, fy_class *clazz,
		fy_exception *exception);

fy_class* fy_heapGetClassOfObject(fy_VMContext *context, jint handle);

int fy_heapAllocateArray(fy_VMContext *context, fy_class *clazz, int length,
		fy_exception *exception);

jint fy_heapArrayLength(fy_VMContext *context, jint handle,
		fy_exception *exception);
void fy_heapArrayCopy(fy_VMContext *context, jint src, jint srcPos, jint dest,
		jint destPos, jint len, fy_exception *exception);

fy_str* fy_heapGetString(fy_VMContext *context, jint handle, fy_str *target,
		fy_exception *exception);
jint fy_heapMakeString(fy_VMContext *context, fy_str *target,
		fy_exception *exception);
jint fy_heapLiteral(fy_VMContext *context, fy_str *str, fy_exception *exception);

jboolean fy_heapGetArrayBoolean(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception);
jint fy_heapGetArrayHandle(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception);
jbyte fy_heapGetArrayByte(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception);
jbyte *fy_heapGetArrayBytes(fy_VMContext *context, jint handle,
		fy_exception *exception);
jshort fy_heapGetArrayShort(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception);
jchar fy_heapGetArrayChar(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception);
jint fy_heapGetArrayInt(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception);
jlong fy_heapGetArrayLong(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception);
jfloat fy_heapGetArrayFloat(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception);
jdouble fy_heapGetArrayDouble(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception);

void fy_heapPutArrayBoolean(fy_VMContext *context, jint handle, jint index,
		jboolean value, fy_exception *exception);
void fy_heapPutArrayHandle(fy_VMContext *context, jint handle, jint index,
		jint value, fy_exception *exception);
void fy_heapPutArrayByte(fy_VMContext *context, jint handle, jint index,
		jbyte value, fy_exception *exception);
void fy_heapPutArrayShort(fy_VMContext *context, jint handle, jint index,
		jshort value, fy_exception *exception);
void fy_heapPutArrayChar(fy_VMContext *context, jint handle, jint index,
		jchar value, fy_exception *exception);
void fy_heapPutArrayInt(fy_VMContext *context, jint handle, jint index,
		jint value, fy_exception *exception);
void fy_heapPutArrayLong(fy_VMContext *context, jint handle, jint index,
		jlong value, fy_exception *exception);
void fy_heapPutArrayFloat(fy_VMContext *context, jint handle, jint index,
		jfloat value, fy_exception *exception);
void fy_heapPutArrayDouble(fy_VMContext *context, jint handle, jint index,
		jdouble value, fy_exception *exception);

jboolean fy_heapGetFieldBoolean(fy_VMContext *context, jint handle,
		fy_field *field, fy_exception *exception);
jint fy_heapGetFieldHandle(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception);
jbyte fy_heapGetFieldByte(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception);
jshort fy_heapGetFieldShort(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception);
jchar fy_heapGetFieldChar(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception);
jint fy_heapGetFieldInt(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception);
jlong fy_heapGetFieldLong(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception);
jfloat fy_heapGetFieldFloat(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception);
jdouble fy_heapGetFieldDouble(fy_VMContext *context, jint handle,
		fy_field *field, fy_exception *exception);

void fy_heapPutFieldBoolean(fy_VMContext *context, jint handle, fy_field *field,
		jboolean value, fy_exception *exception);
void fy_heapPutFieldHandle(fy_VMContext *context, jint handle, fy_field *field,
		jint value, fy_exception *exception);
void fy_heapPutFieldByte(fy_VMContext *context, jint handle, fy_field *field,
		jbyte value, fy_exception *exception);
void fy_heapPutFieldShort(fy_VMContext *context, jint handle, fy_field *field,
		jshort value, fy_exception *exception);
void fy_heapPutFieldChar(fy_VMContext *context, jint handle, fy_field *field,
		jchar value, fy_exception *exception);
void fy_heapPutFieldInt(fy_VMContext *context, jint handle, fy_field *field,
		jint value, fy_exception *exception);
void fy_heapPutFieldLong(fy_VMContext *context, jint handle, fy_field *field,
		jlong value, fy_exception *exception);
void fy_heapPutFieldFloat(fy_VMContext *context, jint handle, fy_field *field,
		jfloat value, fy_exception *exception);
void fy_heapPutFieldDouble(fy_VMContext *context, jint handle, fy_field *field,
		jdouble value, fy_exception *exception);

jboolean fy_heapGetStaticBoolean(fy_VMContext *context, fy_field *field,
		fy_exception *exception);
jint fy_heapGetStaticHandle(fy_VMContext *context, fy_field *field,
		fy_exception *exception);
jbyte fy_heapGetStaticByte(fy_VMContext *context, fy_field *field,
		fy_exception *exception);
jshort fy_heapGetStaticShort(fy_VMContext *context, fy_field *field,
		fy_exception *exception);
jchar fy_heapGetStaticChar(fy_VMContext *context, fy_field *field,
		fy_exception *exception);
jint fy_heapGetStaticInt(fy_VMContext *context, fy_field *field,
		fy_exception *exception);
jlong fy_heapGetStaticLong(fy_VMContext *context, fy_field *field,
		fy_exception *exception);
jfloat fy_heapGetStaticFloat(fy_VMContext *context, fy_field *field,
		fy_exception *exception);
jdouble fy_heapGetStaticDouble(fy_VMContext *context, fy_field *field,
		fy_exception *exception);

void fy_heapPutStaticBoolean(fy_VMContext *context, fy_field *field,
		jboolean value, fy_exception *exception);
void fy_heapPutStaticHandle(fy_VMContext *context, fy_field *field, jint value,
		fy_exception *exception);
void fy_heapPutStaticByte(fy_VMContext *context, fy_field *field, jbyte value,
		fy_exception *exception);
void fy_heapPutStaticShort(fy_VMContext *context, fy_field *field, jshort value,
		fy_exception *exception);
void fy_heapPutStaticChar(fy_VMContext *context, fy_field *field, jchar value,
		fy_exception *exception);
void fy_heapPutStaticInt(fy_VMContext *context, fy_field *field, jint value,
		fy_exception *exception);
void fy_heapPutStaticLong(fy_VMContext *context, fy_field *field, jlong value,
		fy_exception *exception);
void fy_heapPutStaticFloat(fy_VMContext *context, fy_field *field, jfloat value,
		fy_exception *exception);
void fy_heapPutStaticDouble(fy_VMContext *context, fy_field *field,
		jdouble value, fy_exception *exception);

#ifdef	__cplusplus
}
#endif
#endif /* HEAP_H_ */

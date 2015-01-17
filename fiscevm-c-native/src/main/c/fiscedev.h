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

#ifndef FISCEDEV_H_
#define FISCEDEV_H_

#include "fiscestu.h"

#ifdef	__cplusplus
extern "C" {
#endif

FY_ATTR_EXPORT void fy_nativeReturnInt(fy_context *context, fy_thread *thread,
		fy_int value);
FY_ATTR_EXPORT void fy_nativeReturnHandle(fy_context *context,
		fy_thread *thread, fy_int value);
FY_ATTR_EXPORT void fy_nativeReturnLong(fy_context *context, fy_thread *thread,
		fy_long value);
#define fy_nativeReturnFloat(C,T,V) fy_nativeReturnInt(C,T,fy_floatToInt(V))
#define fy_nativeReturnDouble(C,T,V) fy_nativeReturnLong(C,T,fy_doubleToLong(V))

FY_ATTR_EXPORT fy_class *fy_nativeLookupClass(fy_context *context, fy_str *name,
		fy_exception *exception);
FY_ATTR_EXPORT fy_field *fy_nativeGetField(fy_context *context, fy_str *name);
FY_ATTR_EXPORT fy_field *fy_nativeLookupFieldVirtual(fy_context *context,
		fy_class *clazz, fy_str *name, fy_exception *exception);
FY_ATTR_EXPORT fy_method *fy_nativeGetMethod(fy_context *context, fy_str *name);
FY_ATTR_EXPORT fy_method *fy_nativeLookupMethodVirtual(fy_context *context,
		fy_class *clazz, fy_str *name, fy_exception *exception);
FY_ATTR_EXPORT fy_uint fy_nativePrepareThrowable(fy_context *context,
		fy_thread *thread, FY_ATTR_RESTRICT fy_exception* toPrepare,
		FY_ATTR_RESTRICT fy_exception* exception);

FY_ATTR_EXPORT int fy_nativeAllocate(fy_context *context, fy_class *clazz,
		fy_exception *exception);
FY_ATTR_EXPORT fy_class* fy_nativeGetClassOfObject(fy_context *context,
		fy_int handle, fy_exception *exception);

FY_ATTR_EXPORT int fy_nativeAllocateArray(fy_context *context, fy_class *clazz,
		int length, fy_exception *exception);
FY_ATTR_EXPORT fy_int fy_nativeArrayLength(fy_context *context, fy_int handle,
		fy_exception *exception);

FY_ATTR_EXPORT void fy_nativeArrayCopy(fy_context *context, fy_int src,
		fy_int srcPos, fy_int dest, fy_int destPos, fy_int len,
		fy_exception *exception);
FY_ATTR_EXPORT fy_int fy_nativeClone(fy_context *context, fy_int src,
		fy_exception *exception);
FY_ATTR_EXPORT fy_str* fy_nativeGetString(fy_context *context, fy_int handle,
		fy_str *target, fy_exception *exception);
FY_ATTR_EXPORT fy_int fy_nativeMakeString(fy_context *context, fy_str *target,
		fy_exception *exception);
FY_ATTR_EXPORT fy_int fy_nativeLiteral(fy_context *context, fy_str *str,
		fy_exception *exception);

FY_ATTR_EXPORT void fy_nativeBeginProtect(fy_context *context);

FY_ATTR_EXPORT void fy_nativeEndProtect(fy_context *context);
FY_ATTR_EXPORT fy_boolean fy_nativeGetArrayBoolean(fy_context *context,
		fy_int handle, fy_int index, fy_exception *exception);
FY_ATTR_EXPORT fy_int fy_nativeGetArrayHandle(fy_context *context,
		fy_int handle, fy_int index, fy_exception *exception);
FY_ATTR_EXPORT fy_byte fy_nativeGetArrayByte(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception);
FY_ATTR_EXPORT fy_byte *fy_nativeGetArrayBytes(fy_context *context,
		fy_int handle, fy_exception *exception);
FY_ATTR_EXPORT fy_short fy_nativeGetArrayShort(fy_context *context,
		fy_int handle, fy_int index, fy_exception *exception);
FY_ATTR_EXPORT fy_char fy_nativeGetArrayChar(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception);
FY_ATTR_EXPORT fy_int fy_nativeGetArrayInt(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception);
FY_ATTR_EXPORT fy_long fy_nativeGetArrayLong(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception);
FY_ATTR_EXPORT fy_float fy_nativeGetArrayFloat(fy_context *context,
		fy_int handle, fy_int index, fy_exception *exception);
FY_ATTR_EXPORT fy_double fy_nativeGetArrayDouble(fy_context *context,
		fy_int handle, fy_int index, fy_exception *exception);

FY_ATTR_EXPORT void fy_nativePutArrayBoolean(fy_context *context, fy_int handle,
		fy_int index, fy_boolean value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutArrayHandle(fy_context *context, fy_int handle,
		fy_int index, fy_int value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutArrayByte(fy_context *context, fy_int handle,
		fy_int index, fy_byte value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutArrayShort(fy_context *context, fy_int handle,
		fy_int index, fy_short value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutArrayChar(fy_context *context, fy_int handle,
		fy_int index, fy_char value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutArrayInt(fy_context *context, fy_int handle,
		fy_int index, fy_int value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutArrayLong(fy_context *context, fy_int handle,
		fy_int index, fy_long value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutArrayFloat(fy_context *context, fy_int handle,
		fy_int index, fy_float value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutArrayDouble(fy_context *context, fy_int handle,
		fy_int index, fy_double value, fy_exception *exception);
FY_ATTR_EXPORT fy_boolean fy_nativeGetFieldBoolean(fy_context *context,
		fy_int handle, fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_int fy_nativeGetFieldHandle(fy_context *context,
		fy_int handle, fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_byte fy_nativeGetFieldByte(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_short fy_nativeGetFieldShort(fy_context *context,
		fy_int handle, fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_char fy_nativeGetFieldChar(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_int fy_nativeGetFieldInt(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_long fy_nativeGetFieldLong(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_float fy_nativeGetFieldFloat(fy_context *context,
		fy_int handle, fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_double fy_nativeGetFieldDouble(fy_context *context,
		fy_int handle, fy_field *field, fy_exception *exception);

FY_ATTR_EXPORT void fy_nativePutFieldBoolean(fy_context *context, fy_int handle,
		fy_field *field, fy_boolean value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutFieldHandle(fy_context *context, fy_int handle,
		fy_field *field, fy_int value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutFieldByte(fy_context *context, fy_int handle,
		fy_field *field, fy_byte value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutFieldShort(fy_context *context, fy_int handle,
		fy_field *field, fy_short value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutFieldChar(fy_context *context, fy_int handle,
		fy_field *field, fy_char value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutFieldInt(fy_context *context, fy_int handle,
		fy_field *field, fy_int value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutFieldLong(fy_context *context, fy_int handle,
		fy_field *field, fy_long value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutFieldFloat(fy_context *context, fy_int handle,
		fy_field *field, fy_float value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutFieldDouble(fy_context *context, fy_int handle,
		fy_field *field, fy_double value, fy_exception *exception);
FY_ATTR_EXPORT fy_boolean fy_nativeGetStaticBoolean(fy_context *context,
		fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_int fy_nativeGetStaticHandle(fy_context *context,
		fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_byte fy_nativeGetStaticByte(fy_context *context,
		fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_short fy_nativeGetStaticShort(fy_context *context,
		fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_char fy_nativeGetStaticChar(fy_context *context,
		fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_int fy_nativeGetStaticInt(fy_context *context,
		fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_long fy_nativeGetStaticLong(fy_context *context,
		fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_float fy_nativeGetStaticFloat(fy_context *context,
		fy_field *field, fy_exception *exception);
FY_ATTR_EXPORT fy_double fy_nativeGetStaticDouble(fy_context *context,
		fy_field *field, fy_exception *exception);

FY_ATTR_EXPORT void fy_nativePutStaticBoolean(fy_context *context,
		fy_field *field, fy_boolean value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutStaticHandle(fy_context *context,
		fy_field *field, fy_int value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutStaticByte(fy_context *context, fy_field *field,
		fy_byte value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutStaticShort(fy_context *context,
		fy_field *field, fy_short value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutStaticChar(fy_context *context, fy_field *field,
		fy_char value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutStaticInt(fy_context *context, fy_field *field,
		fy_int value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutStaticLong(fy_context *context, fy_field *field,
		fy_long value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutStaticFloat(fy_context *context,
		fy_field *field, fy_float value, fy_exception *exception);
FY_ATTR_EXPORT void fy_nativePutStaticDouble(fy_context *context,
		fy_field *field, fy_double value, fy_exception *exception);

FY_ATTR_EXPORT void fy_nativeRegisterNativeHandler(fy_context *context,
		const char *name, void *data, fy_nhFunction handler,
		fy_exception *exception);
FY_ATTR_EXPORT void fy_nativeUnRegisterNativeHandler(fy_context *context,
		const char *name, fy_exception *exception);
#ifdef	__cplusplus
}
#endif

#endif /* FISCEDEV_H_ */

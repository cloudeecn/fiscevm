/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part offiscevm
 *
 *fiscevmis free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 *fiscevmis distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along withfiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fisce.h"
#include "fyc/VMContext.h"
#include "fyc/Thread.h"

FY_ATTR_EXPORT void fisceInitContext(fy_context *context,
		fy_exception *exception) {
	fy_vmContextInit(context, exception);
}

FY_ATTR_EXPORT void fisceDestroyContext(fy_context *context) {
	fy_vmContextDestroy(context);
}
FY_ATTR_EXPORT void fisceBootFromMain(fy_context *context, const char *name,
		fy_exception *exception) {
	fy_vmBootup(context, name, exception);
}

FY_ATTR_EXPORT void fisceBootFromData(fy_context *context,
                                      fy_exception *exception) {
    fy_vmBootFromData(context, exception);
}

FY_ATTR_EXPORT void fisceSave(fy_context *context,
                                      fy_exception *exception) {
    fy_vmSave(context, exception);
}

FY_ATTR_EXPORT void fisceRun(fy_context *context, fy_message *message,
		fy_exception *exception) {
	fy_tmRun(context, message, exception);
}

FY_ATTR_EXPORT fy_class *fy_nativeLookupClass(fy_context *context, fy_str *name,
		fy_exception *exception) {
	return fy_vmLookupClass(context, name, exception);
}

FY_ATTR_EXPORT fy_field *fy_nativeGetField(fy_context *context, fy_str *name) {
	return fy_vmGetField(context, name);
}

FY_ATTR_EXPORT fy_field *fy_nativeLookupFieldVirtual(fy_context *context,
		fy_class *clazz, fy_str *name, fy_exception *exception) {
	return fy_vmLookupFieldVirtual(context, clazz, name, exception);
}
FY_ATTR_EXPORT fy_method *fy_nativeGetMethod(fy_context *context, fy_str *name) {
	return fy_vmGetMethod(context, name);
}
FY_ATTR_EXPORT fy_method *fy_nativeLookupMethodVirtual(fy_context *context,
		fy_class *clazz, fy_str *name, fy_exception *exception) {
	return fy_vmLookupMethodVirtual(context, clazz, name, exception);
}
FY_ATTR_EXPORT fy_uint fy_nativePrepareThrowable(fy_context *context,
		fy_thread *thread, FY_ATTR_RESTRICT fy_exception* toPrepare,
		FY_ATTR_RESTRICT fy_exception* exception) {
	return fy_threadPrepareThrowable(context, thread, toPrepare, exception);
}

FY_ATTR_EXPORT int fy_nativeAllocate(fy_context *context, fy_class *clazz,
		fy_exception *exception) {
	return fy_heapAllocate(context, clazz, exception);
}

FY_ATTR_EXPORT fy_class* fy_nativeGetClassOfObject(fy_context *context,
		fy_int handle, fy_exception *exception) {
	return fy_heapGetClassOfObject(context, handle, exception);
}

FY_ATTR_EXPORT int fy_nativeAllocateArray(fy_context *context, fy_class *clazz,
		int length, fy_exception *exception) {
	return fy_heapAllocateArray(context, clazz, length, exception);
}

FY_ATTR_EXPORT fy_int fy_nativeArrayLength(fy_context *context, fy_int handle,
		fy_exception *exception) {
	return fy_heapArrayLength(context, handle, exception);
}

FY_ATTR_EXPORT void fy_nativeArrayCopy(fy_context *context, fy_int src,
		fy_int srcPos, fy_int dest, fy_int destPos, fy_int len,
		fy_exception *exception) {
	fy_heapArrayCopy(context, src, srcPos, dest, destPos, len, exception);
}

FY_ATTR_EXPORT fy_int fy_nativeClone(fy_context *context, fy_int src,
		fy_exception *exception) {
	return fy_heapClone(context, src, exception);
}

FY_ATTR_EXPORT fy_str* fy_nativeGetString(fy_context *context, fy_int handle,
		fy_str *target, fy_exception *exception) {
	return fy_heapGetString(context, handle, target, exception);
}

FY_ATTR_EXPORT fy_int fy_nativeMakeString(fy_context *context, fy_str *target,
		fy_exception *exception) {
	return fy_heapMakeString(context, target, exception);
}
FY_ATTR_EXPORT fy_int fy_nativeLiteral(fy_context *context, fy_str *str,
		fy_exception *exception) {
	return fy_heapLiteral(context, str, exception);
}

FY_ATTR_EXPORT void fy_nativeBeginProtect(fy_context *context) {
	fy_heapBeginProtect(context);
}

FY_ATTR_EXPORT void fy_nativeEndProtect(fy_context *context) {
	fy_heapEndProtect(context);
}

FY_ATTR_EXPORT fy_boolean fy_nativeGetArrayBoolean(fy_context *context,
		fy_int handle, fy_int index, fy_exception *exception) {
	return fy_heapGetArrayBoolean(context, handle, index, exception);
}
FY_ATTR_EXPORT fy_int fy_nativeGetArrayHandle(fy_context *context,
		fy_int handle, fy_int index, fy_exception *exception) {
	return fy_heapGetArrayHandle(context, handle, index, exception);
}
FY_ATTR_EXPORT fy_byte fy_nativeGetArrayByte(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception) {
	return fy_heapGetArrayByte(context, handle, index, exception);
}
FY_ATTR_EXPORT fy_byte *fy_nativeGetArrayBytes(fy_context *context,
		fy_int handle, fy_exception *exception) {
	return fy_heapGetArrayBytes(context, handle, exception);
}

FY_ATTR_EXPORT fy_short fy_nativeGetArrayShort(fy_context *context,
		fy_int handle, fy_int index, fy_exception *exception) {
	return fy_heapGetArrayShort(context, handle, index, exception);
}
FY_ATTR_EXPORT fy_char fy_nativeGetArrayChar(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception) {
	return fy_heapGetArrayChar(context, handle, index, exception);
}
FY_ATTR_EXPORT fy_int fy_nativeGetArrayInt(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception) {
	return fy_heapGetArrayInt(context, handle, index, exception);
}
FY_ATTR_EXPORT fy_long fy_nativeGetArrayLong(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception) {
	return fy_heapGetArrayLong(context, handle, index, exception);
}
FY_ATTR_EXPORT fy_float fy_nativeGetArrayFloat(fy_context *context,
		fy_int handle, fy_int index, fy_exception *exception) {
	return fy_heapGetArrayFloat(context, handle, index, exception);
}
FY_ATTR_EXPORT fy_double fy_nativeGetArrayDouble(fy_context *context,
		fy_int handle, fy_int index, fy_exception *exception) {
	return fy_heapGetArrayDouble(context, handle, index, exception);
}

FY_ATTR_EXPORT void fy_nativePutArrayBoolean(fy_context *context, fy_int handle,
		fy_int index, fy_boolean value, fy_exception *exception) {
	fy_heapPutArrayBoolean(context, handle, index, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutArrayHandle(fy_context *context, fy_int handle,
		fy_int index, fy_int value, fy_exception *exception) {
	fy_heapPutArrayHandle(context, handle, index, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutArrayByte(fy_context *context, fy_int handle,
		fy_int index, fy_byte value, fy_exception *exception) {
	fy_heapPutArrayByte(context, handle, index, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutArrayShort(fy_context *context, fy_int handle,
		fy_int index, fy_short value, fy_exception *exception) {
	fy_heapPutArrayShort(context, handle, index, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutArrayChar(fy_context *context, fy_int handle,
		fy_int index, fy_char value, fy_exception *exception) {
	fy_heapPutArrayChar(context, handle, index, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutArrayInt(fy_context *context, fy_int handle,
		fy_int index, fy_int value, fy_exception *exception) {
	fy_heapPutArrayInt(context, handle, index, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutArrayLong(fy_context *context, fy_int handle,
		fy_int index, fy_long value, fy_exception *exception) {
	fy_heapPutArrayLong(context, handle, index, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutArrayFloat(fy_context *context, fy_int handle,
		fy_int index, fy_float value, fy_exception *exception) {
	fy_heapPutArrayFloat(context, handle, index, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutArrayDouble(fy_context *context, fy_int handle,
		fy_int index, fy_double value, fy_exception *exception) {
	fy_heapPutArrayDouble(context, handle, index, value, exception);
}

FY_ATTR_EXPORT fy_boolean fy_nativeGetFieldBoolean(fy_context *context,
		fy_int handle, fy_field *field, fy_exception *exception) {
	return fy_heapGetFieldBoolean(context, handle, field, exception);
}
FY_ATTR_EXPORT fy_int fy_nativeGetFieldHandle(fy_context *context,
		fy_int handle, fy_field *field, fy_exception *exception) {
	return fy_heapGetFieldHandle(context, handle, field, exception);
}
FY_ATTR_EXPORT fy_byte fy_nativeGetFieldByte(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	return fy_heapGetFieldByte(context, handle, field, exception);
}
FY_ATTR_EXPORT fy_short fy_nativeGetFieldShort(fy_context *context,
		fy_int handle, fy_field *field, fy_exception *exception) {
	return fy_heapGetFieldShort(context, handle, field, exception);
}
FY_ATTR_EXPORT fy_char fy_nativeGetFieldChar(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	return fy_heapGetFieldChar(context, handle, field, exception);
}
FY_ATTR_EXPORT fy_int fy_nativeGetFieldInt(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	return fy_heapGetFieldInt(context, handle, field, exception);
}
FY_ATTR_EXPORT fy_long fy_nativeGetFieldLong(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	return fy_heapGetFieldLong(context, handle, field, exception);
}
FY_ATTR_EXPORT fy_float fy_nativeGetFieldFloat(fy_context *context,
		fy_int handle, fy_field *field, fy_exception *exception) {
	return fy_heapGetFieldFloat(context, handle, field, exception);
}
FY_ATTR_EXPORT fy_double fy_nativeGetFieldDouble(fy_context *context,
		fy_int handle, fy_field *field, fy_exception *exception) {
	return fy_heapGetFieldDouble(context, handle, field, exception);
}

FY_ATTR_EXPORT void fy_nativePutFieldBoolean(fy_context *context, fy_int handle,
		fy_field *field, fy_boolean value, fy_exception *exception) {
	fy_heapPutFieldBoolean(context, handle, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutFieldHandle(fy_context *context, fy_int handle,
		fy_field *field, fy_int value, fy_exception *exception) {
	fy_heapPutFieldHandle(context, handle, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutFieldByte(fy_context *context, fy_int handle,
		fy_field *field, fy_byte value, fy_exception *exception) {
	fy_heapPutFieldByte(context, handle, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutFieldShort(fy_context *context, fy_int handle,
		fy_field *field, fy_short value, fy_exception *exception) {
	fy_heapPutFieldShort(context, handle, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutFieldChar(fy_context *context, fy_int handle,
		fy_field *field, fy_char value, fy_exception *exception) {
	fy_heapPutFieldChar(context, handle, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutFieldInt(fy_context *context, fy_int handle,
		fy_field *field, fy_int value, fy_exception *exception) {
	fy_heapPutFieldInt(context, handle, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutFieldLong(fy_context *context, fy_int handle,
		fy_field *field, fy_long value, fy_exception *exception) {
	fy_heapPutFieldLong(context, handle, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutFieldFloat(fy_context *context, fy_int handle,
		fy_field *field, fy_float value, fy_exception *exception) {
	fy_heapPutFieldFloat(context, handle, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutFieldDouble(fy_context *context, fy_int handle,
		fy_field *field, fy_double value, fy_exception *exception) {
	fy_heapPutFieldDouble(context, handle, field, value, exception);
}

FY_ATTR_EXPORT fy_boolean fy_nativeGetStaticBoolean(fy_context *context,
		fy_field *field, fy_exception *exception) {
	return fy_heapGetStaticBoolean(context, field, exception);
}
FY_ATTR_EXPORT fy_int fy_nativeGetStaticHandle(fy_context *context,
		fy_field *field, fy_exception *exception) {
	return fy_heapGetStaticHandle(context, field, exception);
}
FY_ATTR_EXPORT fy_byte fy_nativeGetStaticByte(fy_context *context,
		fy_field *field, fy_exception *exception) {
	return fy_heapGetStaticByte(context, field, exception);
}
FY_ATTR_EXPORT fy_short fy_nativeGetStaticShort(fy_context *context,
		fy_field *field, fy_exception *exception) {
	return fy_heapGetStaticShort(context, field, exception);
}
FY_ATTR_EXPORT fy_char fy_nativeGetStaticChar(fy_context *context,
		fy_field *field, fy_exception *exception) {
	return fy_heapGetStaticChar(context, field, exception);
}
FY_ATTR_EXPORT fy_int fy_nativeGetStaticInt(fy_context *context,
		fy_field *field, fy_exception *exception) {
	return fy_heapGetStaticInt(context, field, exception);
}
FY_ATTR_EXPORT fy_long fy_nativeGetStaticLong(fy_context *context,
		fy_field *field, fy_exception *exception) {
	return fy_heapGetStaticLong(context, field, exception);
}
FY_ATTR_EXPORT fy_float fy_nativeGetStaticFloat(fy_context *context,
		fy_field *field, fy_exception *exception) {
	return fy_heapGetStaticFloat(context, field, exception);
}
FY_ATTR_EXPORT fy_double fy_nativeGetStaticDouble(fy_context *context,
		fy_field *field, fy_exception *exception) {
	return fy_heapGetStaticDouble(context, field, exception);
}

FY_ATTR_EXPORT void fy_nativePutStaticBoolean(fy_context *context,
		fy_field *field, fy_boolean value, fy_exception *exception) {
	fy_heapPutStaticBoolean(context, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutStaticHandle(fy_context *context,
		fy_field *field, fy_int value, fy_exception *exception) {
	fy_heapPutStaticHandle(context, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutStaticByte(fy_context *context, fy_field *field,
		fy_byte value, fy_exception *exception) {
	fy_heapPutStaticByte(context, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutStaticShort(fy_context *context,
		fy_field *field, fy_short value, fy_exception *exception) {
	fy_heapPutStaticShort(context, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutStaticChar(fy_context *context, fy_field *field,
		fy_char value, fy_exception *exception) {
	fy_heapPutStaticChar(context, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutStaticInt(fy_context *context, fy_field *field,
		fy_int value, fy_exception *exception) {
	fy_heapPutStaticInt(context, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutStaticLong(fy_context *context, fy_field *field,
		fy_long value, fy_exception *exception) {
	fy_heapPutStaticLong(context, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutStaticFloat(fy_context *context,
		fy_field *field, fy_float value, fy_exception *exception) {
	fy_heapPutStaticFloat(context, field, value, exception);
}
FY_ATTR_EXPORT void fy_nativePutStaticDouble(fy_context *context,
		fy_field *field, fy_double value, fy_exception *exception) {
	fy_heapPutStaticDouble(context, field, value, exception);
}

FY_ATTR_EXPORT void fy_nativeRegisterNativeHandler(fy_context *context, const char *name,
    void *data, fy_nhFunction handler, fy_exception *exception){
    fy_vmRegisterNativeHandler(context, name, data, handler, exception);
}

FY_ATTR_EXPORT void fy_nativeUnRegisterNativeHandler(fy_context *context,
		const char *name, fy_exception *exception) {
	fy_vmUnRegisterNativeHandler(context, name, exception);
}

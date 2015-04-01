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

#ifndef FY_HEAP_H_
#define FY_HEAP_H_

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fy_util/MemMan.h"
#include "fy_util/BitSet.h"
#include "fy_util/String.h"
#include "fyc/ClassStruct.h"
#include "fyc/VMContext.h"
#include "fyc/typedefs.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum fy_heapPos {
	automatic = 0, eden = 1, young = 2, old = 3
} fy_heapPos;

struct fy_object_data {
	fy_class *clazz;
#ifdef FY_ENUM_BITS_SUPPORTED
	enum fy_heapPos position :2;
	enum {
		not_finalized, in_finalize_array, finalized
	} finalizeStatus :2;
#else
	enum fy_heapPos position;
	enum {
		not_finalized, in_finalize_array, finalized
	} finalizeStatus;
#endif
	fisce_int gen :8;
	/*Union data for different class types (array/normal object/references)*/
	union {
		fisce_int multiUsageData;
		/*Length of the array object*/
		fisce_int arrayLength;
		fisce_int threadId;
		fisce_int methodId;
		fisce_int fieldId;
		fisce_int classId;
		fisce_int streamId;
	}m;
	fisce_uint monitorOwnerId;
	fisce_int monitorOwnerTimes;
	FY_VLS(fisce_byte,data);
};

#define fy_heapGetObject(CONTEXT,HANDLE) ((CONTEXT)->objects+(HANDLE))
#define fy_heapValue(CONTEXT, HANDLE, POS) (((fisce_int*) fy_heapGetObject(CONTEXT, HANDLE)->object_data->data)[POS])
#define fy_heapValueOfTpye(CONTEXT, HANDLE, TYPE, POS) (((TYPE*) fy_heapGetObject(CONTEXT, HANDLE)->object_data->data)[POS])
#define fy_heapArrayLengthFast(CONTEXT, HANDLE) (fy_heapGetObject(CONTEXT, HANDLE)->object_data->m.arrayLength)

fisce_uint fy_heapAllocateDirect(fy_context *context, fisce_int size, fy_class *clazz,
		fisce_int length, fisce_uint toHandle, enum fy_heapPos pos,
		fisce_exception *exception);

fisce_uint fy_heapAllocate(fy_context *context, fy_class *clazz,
		fisce_exception *exception);

fy_class* fy_heapGetClassOfObject(fy_context *context, fisce_int handle,
		fisce_exception *exception);

fisce_int fy_heapGetArraySizeFromLength(fy_class *clazz, fisce_int length);

fisce_uint fy_heapAllocateArray(fy_context *context, fy_class *clazz, int length,
		fisce_exception *exception);
fisce_uint fy_heapAllocateArrayWithContentType(fy_context *context, fy_class *clazz, fisce_int length,fisce_exception *exception);

fisce_int fy_heapArrayLength(fy_context *context, fisce_int handle,
		fisce_exception *exception);
void fy_heapArrayCopy(fy_context *context, fisce_int src, fisce_int srcPos,
		fisce_int dest, fisce_int destPos, fisce_int len, fisce_exception *exception);
fisce_int fy_heapClone(fy_context *context, fisce_int src, fisce_exception *exception);
void fy_heapGC(void *context,fisce_boolean memoryStressed, fisce_exception *exception);

fy_str* fy_heapGetString(fy_context *context, fisce_int handle, fy_str *target,
		fisce_exception *exception);
fisce_int fy_heapMakeString(fy_context *context, fy_str *target,
		fisce_exception *exception);
fisce_int fy_heapLiteral(fy_context *context, fy_str *str,
		fisce_exception *exception);

void fy_heapBeginProtect(fy_context *context);

void fy_heapEndProtect(fy_context *context);

fisce_boolean fy_heapGetArrayBoolean(fy_context *context, fisce_int handle,
		fisce_int index, fisce_exception *exception);
fisce_int fy_heapGetArrayHandle(fy_context *context, fisce_int handle, fisce_int index,
		fisce_exception *exception);
fisce_byte fy_heapGetArrayByte(fy_context *context, fisce_int handle, fisce_int index,
		fisce_exception *exception);
fisce_byte *fy_heapGetArrayBytes(fy_context *context, fisce_int handle,
		fisce_exception *exception);
fisce_short fy_heapGetArrayShort(fy_context *context, fisce_int handle, fisce_int index,
		fisce_exception *exception);
fisce_char fy_heapGetArrayChar(fy_context *context, fisce_int handle, fisce_int index,
		fisce_exception *exception);
fisce_int fy_heapGetArrayInt(fy_context *context, fisce_int handle, fisce_int index,
		fisce_exception *exception);
fisce_long fy_heapGetArrayLong(fy_context *context, fisce_int handle, fisce_int index,
		fisce_exception *exception);
fisce_float fy_heapGetArrayFloat(fy_context *context, fisce_int handle, fisce_int index,
		fisce_exception *exception);
fisce_double fy_heapGetArrayDouble(fy_context *context, fisce_int handle,
		fisce_int index, fisce_exception *exception);

void fy_heapPutArrayBoolean(fy_context *context, fisce_int handle, fisce_int index,
		fisce_boolean value, fisce_exception *exception);
void fy_heapPutArrayHandle(fy_context *context, fisce_int handle, fisce_int index,
		fisce_int value, fisce_exception *exception);
void fy_heapPutArrayByte(fy_context *context, fisce_int handle, fisce_int index,
		fisce_byte value, fisce_exception *exception);
void fy_heapPutArrayShort(fy_context *context, fisce_int handle, fisce_int index,
		fisce_short value, fisce_exception *exception);
void fy_heapPutArrayChar(fy_context *context, fisce_int handle, fisce_int index,
		fisce_char value, fisce_exception *exception);
void fy_heapPutArrayInt(fy_context *context, fisce_int handle, fisce_int index,
		fisce_int value, fisce_exception *exception);
void fy_heapPutArrayLong(fy_context *context, fisce_int handle, fisce_int index,
		fisce_long value, fisce_exception *exception);
void fy_heapPutArrayFloat(fy_context *context, fisce_int handle, fisce_int index,
		fisce_float value, fisce_exception *exception);
void fy_heapPutArrayDouble(fy_context *context, fisce_int handle, fisce_int index,
		fisce_double value, fisce_exception *exception);

fisce_boolean fy_heapGetFieldBoolean(fy_context *context, fisce_int handle,
		fy_field *field, fisce_exception *exception);
fisce_int fy_heapGetFieldHandle(fy_context *context, fisce_int handle,
		fy_field *field, fisce_exception *exception);
fisce_byte fy_heapGetFieldByte(fy_context *context, fisce_int handle, fy_field *field,
		fisce_exception *exception);
fisce_short fy_heapGetFieldShort(fy_context *context, fisce_int handle,
		fy_field *field, fisce_exception *exception);
fisce_char fy_heapGetFieldChar(fy_context *context, fisce_int handle, fy_field *field,
		fisce_exception *exception);
fisce_int fy_heapGetFieldInt(fy_context *context, fisce_int handle, fy_field *field,
		fisce_exception *exception);
fisce_long fy_heapGetFieldLong(fy_context *context, fisce_int handle, fy_field *field,
		fisce_exception *exception);
fisce_float fy_heapGetFieldFloat(fy_context *context, fisce_int handle,
		fy_field *field, fisce_exception *exception);
fisce_double fy_heapGetFieldDouble(fy_context *context, fisce_int handle,
		fy_field *field, fisce_exception *exception);

void fy_heapPutFieldBoolean(fy_context *context, fisce_int handle, fy_field *field,
		fisce_boolean value, fisce_exception *exception);
void fy_heapPutFieldHandle(fy_context *context, fisce_int handle, fy_field *field,
		fisce_int value, fisce_exception *exception);
void fy_heapPutFieldByte(fy_context *context, fisce_int handle, fy_field *field,
		fisce_byte value, fisce_exception *exception);
void fy_heapPutFieldShort(fy_context *context, fisce_int handle, fy_field *field,
		fisce_short value, fisce_exception *exception);
void fy_heapPutFieldChar(fy_context *context, fisce_int handle, fy_field *field,
		fisce_char value, fisce_exception *exception);
void fy_heapPutFieldInt(fy_context *context, fisce_int handle, fy_field *field,
		fisce_int value, fisce_exception *exception);
void fy_heapPutFieldLong(fy_context *context, fisce_int handle, fy_field *field,
		fisce_long value, fisce_exception *exception);
void fy_heapPutFieldFloat(fy_context *context, fisce_int handle, fy_field *field,
		fisce_float value, fisce_exception *exception);
void fy_heapPutFieldDouble(fy_context *context, fisce_int handle, fy_field *field,
		fisce_double value, fisce_exception *exception);

fisce_boolean fy_heapGetStaticBoolean(fy_context *context, fy_field *field,
		fisce_exception *exception);
fisce_int fy_heapGetStaticHandle(fy_context *context, fy_field *field,
		fisce_exception *exception);
fisce_byte fy_heapGetStaticByte(fy_context *context, fy_field *field,
		fisce_exception *exception);
fisce_short fy_heapGetStaticShort(fy_context *context, fy_field *field,
		fisce_exception *exception);
fisce_char fy_heapGetStaticChar(fy_context *context, fy_field *field,
		fisce_exception *exception);
fisce_int fy_heapGetStaticInt(fy_context *context, fy_field *field,
		fisce_exception *exception);
fisce_long fy_heapGetStaticLong(fy_context *context, fy_field *field,
		fisce_exception *exception);
fisce_float fy_heapGetStaticFloat(fy_context *context, fy_field *field,
		fisce_exception *exception);
fisce_double fy_heapGetStaticDouble(fy_context *context, fy_field *field,
		fisce_exception *exception);

void fy_heapPutStaticBoolean(fy_context *context, fy_field *field,
		fisce_boolean value, fisce_exception *exception);
void fy_heapPutStaticHandle(fy_context *context, fy_field *field, fisce_int value,
		fisce_exception *exception);
void fy_heapPutStaticByte(fy_context *context, fy_field *field, fisce_byte value,
		fisce_exception *exception);
void fy_heapPutStaticShort(fy_context *context, fy_field *field, fisce_short value,
		fisce_exception *exception);
void fy_heapPutStaticChar(fy_context *context, fy_field *field, fisce_char value,
		fisce_exception *exception);
void fy_heapPutStaticInt(fy_context *context, fy_field *field, fisce_int value,
		fisce_exception *exception);
void fy_heapPutStaticLong(fy_context *context, fy_field *field, fisce_long value,
		fisce_exception *exception);
void fy_heapPutStaticFloat(fy_context *context, fy_field *field, fisce_float value,
		fisce_exception *exception);
void fy_heapPutStaticDouble(fy_context *context, fy_field *field,
		fisce_double value, fisce_exception *exception);

void fy_heapRegisterReference(fy_context *context, fisce_int reference,
		fisce_int referent, fisce_exception *exception);

fisce_int fy_heapGetReferent(fy_context *context, fisce_int reference);

fisce_uint fy_heapWrapBoolean(fy_context *context, fisce_boolean value,
		fisce_exception *exception);

fisce_boolean fy_heapUnwrapBoolean(fy_context *context, fisce_uint handle,
		fisce_exception *exception);

fisce_uint fy_heapWrapByte(fy_context *context, fisce_byte value,
		fisce_exception *exception);

fisce_byte fy_heapUnwrapByte(fy_context *context, fisce_uint handle,
		fisce_exception *exception);

fisce_uint fy_heapWrapShort(fy_context *context, fisce_short value,
		fisce_exception *exception);

fisce_short fy_heapUnwrapShort(fy_context *context, fisce_uint handle,
		fisce_exception *exception);

fisce_uint fy_heapWrapChar(fy_context *context, fisce_char value,
		fisce_exception *exception);

fisce_char fy_heapUnwrapChar(fy_context *context, fisce_uint handle,
		fisce_exception *exception);

fisce_uint fy_heapWrapInt(fy_context *context, fisce_int value,
		fisce_exception *exception);

fisce_int fy_heapUnwrapInt(fy_context *context, fisce_uint handle,
		fisce_exception *exception);

fisce_uint fy_heapWrapFloat(fy_context *context, fisce_float value,
		fisce_exception *exception);

fisce_float fy_heapUnwrapFloat(fy_context *context, fisce_uint handle,
		fisce_exception *exception);

fisce_uint fy_heapWrapLong(fy_context *context, fisce_long value,
		fisce_exception *exception);

fisce_long fy_heapUnwrapLong(fy_context *context, fisce_uint handle,
		fisce_exception *exception);

fisce_uint fy_heapWrapDouble(fy_context *context, fisce_double value,
		fisce_exception *exception);

fisce_double fy_heapUnwrapDouble(fy_context *context, fisce_uint handle,
		fisce_exception *exception);

fisce_uint fy_heapLookupStringFromConstant(fy_context *context,
		ConstantStringInfo *constantStringInfo, fisce_exception *exception);

fisce_int fy_heapMultiArray(fy_context *context, fy_class *clazz, fisce_int layers,
		fisce_int *elementsForEachLayer, fisce_exception *exception);
void fy_heapCheckCast(fy_context *context, fisce_int fromHandle, fy_class *toClass, fisce_exception *exception);
#ifdef	__cplusplus
}
#endif
#endif /* FY_HEAP_H_ */

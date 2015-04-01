/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of fiscevm
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
	fy_int gen :8;
	/*Union data for different class types (array/normal object/references)*/
	union {
		fy_int multiUsageData;
		/*Length of the array object*/
		fy_int arrayLength;
		fy_int threadId;
		fy_int methodId;
		fy_int fieldId;
		fy_int classId;
		fy_int streamId;
	}m;
	fy_uint monitorOwnerId;
	fy_int monitorOwnerTimes;
	FY_VLS(fy_byte,data);
};

#define fy_heapGetObject(CONTEXT,HANDLE) ((CONTEXT)->objects+(HANDLE))
#define fy_heapValue(CONTEXT, HANDLE, POS) (((fy_int*) fy_heapGetObject(CONTEXT, HANDLE)->object_data->data)[POS])
#define fy_heapValueOfTpye(CONTEXT, HANDLE, TYPE, POS) (((TYPE*) fy_heapGetObject(CONTEXT, HANDLE)->object_data->data)[POS])
#define fy_heapArrayLengthFast(CONTEXT, HANDLE) (fy_heapGetObject(CONTEXT, HANDLE)->object_data->m.arrayLength)

fy_uint fy_heapAllocateDirect(fy_context *context, fy_int size, fy_class *clazz,
		fy_int length, fy_uint toHandle, enum fy_heapPos pos,
		fy_exception *exception);

fy_uint fy_heapAllocate(fy_context *context, fy_class *clazz,
		fy_exception *exception);

fy_class* fy_heapGetClassOfObject(fy_context *context, fy_int handle,
		fy_exception *exception);

fy_int fy_heapGetArraySizeFromLength(fy_class *clazz, fy_int length);

fy_uint fy_heapAllocateArray(fy_context *context, fy_class *clazz, int length,
		fy_exception *exception);
fy_uint fy_heapAllocateArrayWithContentType(fy_context *context, fy_class *clazz, fy_int length,fy_exception *exception);

fy_int fy_heapArrayLength(fy_context *context, fy_int handle,
		fy_exception *exception);
void fy_heapArrayCopy(fy_context *context, fy_int src, fy_int srcPos,
		fy_int dest, fy_int destPos, fy_int len, fy_exception *exception);
fy_int fy_heapClone(fy_context *context, fy_int src, fy_exception *exception);
void fy_heapGC(void *context,fy_boolean memoryStressed, fy_exception *exception);

fy_str* fy_heapGetString(fy_context *context, fy_int handle, fy_str *target,
		fy_exception *exception);
fy_int fy_heapMakeString(fy_context *context, fy_str *target,
		fy_exception *exception);
fy_int fy_heapLiteral(fy_context *context, fy_str *str,
		fy_exception *exception);

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

void fy_heapRegisterReference(fy_context *context, fy_int reference,
		fy_int referent, fy_exception *exception);

fy_int fy_heapGetReferent(fy_context *context, fy_int reference);

fy_uint fy_heapWrapBoolean(fy_context *context, fy_boolean value,
		fy_exception *exception);

fy_boolean fy_heapUnwrapBoolean(fy_context *context, fy_uint handle,
		fy_exception *exception);

fy_uint fy_heapWrapByte(fy_context *context, fy_byte value,
		fy_exception *exception);

fy_byte fy_heapUnwrapByte(fy_context *context, fy_uint handle,
		fy_exception *exception);

fy_uint fy_heapWrapShort(fy_context *context, fy_short value,
		fy_exception *exception);

fy_short fy_heapUnwrapShort(fy_context *context, fy_uint handle,
		fy_exception *exception);

fy_uint fy_heapWrapChar(fy_context *context, fy_char value,
		fy_exception *exception);

fy_char fy_heapUnwrapChar(fy_context *context, fy_uint handle,
		fy_exception *exception);

fy_uint fy_heapWrapInt(fy_context *context, fy_int value,
		fy_exception *exception);

fy_int fy_heapUnwrapInt(fy_context *context, fy_uint handle,
		fy_exception *exception);

fy_uint fy_heapWrapFloat(fy_context *context, fy_float value,
		fy_exception *exception);

fy_float fy_heapUnwrapFloat(fy_context *context, fy_uint handle,
		fy_exception *exception);

fy_uint fy_heapWrapLong(fy_context *context, fy_long value,
		fy_exception *exception);

fy_long fy_heapUnwrapLong(fy_context *context, fy_uint handle,
		fy_exception *exception);

fy_uint fy_heapWrapDouble(fy_context *context, fy_double value,
		fy_exception *exception);

fy_double fy_heapUnwrapDouble(fy_context *context, fy_uint handle,
		fy_exception *exception);

fy_uint fy_heapLookupStringFromConstant(fy_context *context,
		ConstantStringInfo *constantStringInfo, fy_exception *exception);

fy_int fy_heapMultiArray(fy_context *context, fy_class *clazz, fy_int layers,
		fy_int *elementsForEachLayer, fy_exception *exception);
void fy_heapCheckCast(fy_context *context, fy_int fromHandle, fy_class *toClass, fy_exception *exception);
#ifdef	__cplusplus
}
#endif
#endif /* FY_HEAP_H_ */

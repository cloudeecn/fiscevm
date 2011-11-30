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

#include "fyc/Heap.h"

static int allocate(fy_context *context, int size, fy_exception *exception) {
	int handle = context->nextHandle;
	while (1) {
		if (fy_heapGetObject(context, handle)->clazz == NULL) {
			break;
		}
		handle++;
		if (handle >= MAX_OBJECTS) {
			handle = 1;
		}
		if (handle == context->nextHandle) {
			/*TODO OOM*/
			fy_fault(exception, NULL, "Out of memory! Handle overflow");
			return 0;
		}
	}
	return handle;
}

int fy_heapAllocate(fy_context *context, fy_class *clazz,
		fy_exception *exception) {
	int length = clazz->sizeAbs;
	int size = length << FY_SIZE_SHIFT_INT;
	fy_int *idata;
	int handle;

	if (clazz->type != obj) {
		fy_fault(exception, NULL, "Cannot instance Array without size");
		return 0;
	}

	handle = allocate(context, size, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	idata = fy_allocate(size, exception);
	fy_exceptionCheckAndReturn(exception) 0;
	fy_heapGetObject(context, handle)->length = length;
	fy_heapGetObject(context, handle)->clazz = clazz;
	fy_heapGetObject(context, handle)->sizeShift = FY_SIZE_SHIFT_INT;
	fy_heapGetObject(context, handle)->data.idata = idata;

	return handle;
}

int fy_heapAllocateArray(fy_context *context, fy_class *clazz, int length,
		fy_exception *exception) {
	int sizeShift = clazz->ci.arr.sizeShift;
	int size = length << sizeShift;
	void *data;
	int handle;

	if (clazz->type != arr) {
		fy_fault(exception, NULL, "Cannot instance Array without size");
		return 0;
	}

	handle = allocate(context, size, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	data = fy_allocate(size, exception);
	fy_exceptionCheckAndReturn(exception) 0;
	fy_heapGetObject(context, handle)->length = length;
	fy_heapGetObject(context, handle)->clazz = clazz;
	fy_heapGetObject(context, handle)->sizeShift = sizeShift;
	switch (sizeShift) {
	case FY_SIZE_SHIFT_BYTE:
		fy_heapGetObject(context, handle)->data.bdata = (fy_byte*) data;
		break;
	case FY_SIZE_SHIFT_LONG:
		fy_heapGetObject(context, handle)->data.ldata = (fy_long*) data;
		break;
	default:
		fy_heapGetObject(context, handle)->data.idata = (fy_int*) data;
		break;
	}
	return handle;
}

fy_class* fy_heapGetClassOfObject(fy_context *context, fy_int handle) {
	return fy_heapGetObject(context, handle)->clazz;
}

fy_str* fy_heapGetString(fy_context *context, fy_int handle, fy_str *target,
		fy_exception *exception) {
	int i;
	fy_field *valueField, *offsetField, *countField;
	fy_int ofs, len;
	fy_int cah;
	if (handle == 0) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/NullPointerException");
		exception->exceptionDesc[0] = 0;
		return target;
	}
	fy_vmLookupClass(context, context->sString, exception);
	if (exception->exceptionType != exception_none) {
		return NULL;
	}
	valueField = fy_vmGetField(context, context->sStringValue);
	countField = fy_vmGetField(context, context->sStringCount);
	offsetField = fy_vmGetField(context, context->sStringOffset);
	/*No exception except NPT will be thrown, and NPT is processed before,so no need to check
	 * exception.*/
	ofs = fy_heapGetFieldInt(context, handle, offsetField, exception);
	len = fy_heapGetFieldInt(context, handle, countField, exception);
	cah = fy_heapGetFieldInt(context, handle, valueField, exception);
	ASSERT(exception->exceptionType == exception_none);
	if (cah == 0) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/NullPointerException");
		exception->exceptionDesc[0] = 0;
		return target;
	}
	for (i = 0; i < len; i++) {
		fy_strAppendChar(context->memblocks, target,
				fy_heapGetArrayChar(context, cah, ofs + i, exception),
				exception);
		if (exception->exceptionType != exception_none) {
			return 0;
		}
	}

	return target;
}

fy_int fy_heapMakeString(fy_context *context, fy_str *target,
		fy_exception *exception) {
	int i;
	fy_field *valueField, *offsetField, *countField;
	fy_int len;
	fy_int cah;
	fy_int ret;
	if (target == NULL) {
		return 0;
	}
	len = target->length;
	cah = fy_heapAllocateArray(context,
			fy_vmLookupClass(context, context->sArrayChar, exception),
			target->length, exception);
	fy_vmLookupClass(context, context->sString, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	valueField = fy_vmGetField(context, context->sStringValue);
	countField = fy_vmGetField(context, context->sStringCount);
	offsetField = fy_vmGetField(context, context->sStringOffset);
	/*No exception except NPT will be thrown, and NPT is processed before,so no need to check
	 * exception.*/
	ret = fy_heapAllocate(context,
			fy_vmLookupClass(context, context->sString, exception), exception);
	ASSERT(exception->exceptionType == exception_none);
	fy_heapPutFieldHandle(context, ret, valueField, cah, exception);
	fy_heapPutFieldInt(context, ret, countField, len, exception);
	fy_heapPutFieldInt(context, ret, offsetField, 0, exception);
	ASSERT(exception->exceptionType==exception_none);
	for (i = 0; i < len; i++) {
		fy_heapPutArrayChar(context, cah, i, target->content[i], exception);
	}

	return ret;
}

fy_int fy_heapLiteral(fy_context *context, fy_str *str, fy_exception *exception) {
	fy_int *pInt;
	fy_memblock *block = context->memblocks;
	pInt = fy_hashMapGet(block, context->literals, str);
	if (pInt == NULL) {
		pInt = fy_mmAllocate(block, sizeof(fy_int), exception);
		fy_exceptionCheckAndReturn(exception) 0;
		*pInt = fy_heapMakeString(context, str, exception);
		if (exception->exceptionType != exception_none) {
			fy_mmFree(block, pInt);
			return 0;
		}
		fy_hashMapPut(block, context->literals, str, pInt, exception);
		fy_exceptionCheckAndReturn(exception) 0;
	}
	return *pInt;
}

void fy_heapArrayCopy(fy_context *context, fy_int src, fy_int srcPos,
		fy_int dest, fy_int destPos, fy_int len, fy_exception *exception) {
	fy_object *sObject, *dObject;
	if (src == 0 || dest == 0) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/ArrayStoreException");
	}
	sObject = fy_heapGetObject(context, src);
	dObject = fy_heapGetObject(context, dest);
	if (srcPos < 0 || destPos < 0 || len < 0 || (srcPos + len > sObject->length)
			|| (destPos + len > dObject->length)) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/ArrayIndexOutOfBoundException");
		exception->exceptionDesc[0] = 0;
		return;
	}
	if (sObject->clazz->type != arr || dObject->clazz->type != arr
			|| !fy_classCanCastTo(context, sObject->clazz, dObject->clazz)) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/ArrayStoreException");
		exception->exceptionDesc[0] = 0;
		return;
	}
	switch (sObject->sizeShift) {
	case FY_SIZE_SHIFT_BYTE:
		memcpy(dObject->data.bdata + destPos, sObject->data.bdata + srcPos,
				len);
		break;
	case FY_SIZE_SHIFT_INT:
		memcpy(dObject->data.idata + destPos, sObject->data.idata + srcPos,
				len << FY_SIZE_SHIFT_INT);
		break;
	case FY_SIZE_SHIFT_LONG:
		memcpy(dObject->data.ldata + destPos, sObject->data.ldata + srcPos,
				len << FY_SIZE_SHIFT_LONG);
		break;
	}
}

#define CHECK_NPT(X) if (handle == 0) { \
exception->exceptionType = exception_normal; \
strcpy_s(exception->exceptionName,sizeof(exception->exceptionName), "java/lang/NullPointerException"); \
exception->exceptionDesc[0] = 0; \
return X; \
}else ASSERT(obj->clazz!=NULL);

#define CHECK_IOOB(X) if (index < 0 || index >= obj->length) {\
exception->exceptionType = exception_normal;\
strcpy_s(exception->exceptionName,sizeof(exception->exceptionName), "java/lang/IndexOutOfBoundException");\
sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),\
		"%I32d / %I32d", index, obj->length);\
return X;\
}

#define CHECK_STATIC(X) if((field->access_flags & FY_ACC_STATIC)==0){\
		exception->exceptionType=exception_normal;\
		strcpy_s(exception->exceptionName,sizeof(exception->exceptionName),"java/lang/IncompatibleClassChangeError");\
		strcpy_s(exception->exceptionDesc,sizeof(exception->exceptionDesc),"get/set static field is not static!");\
		return X;\
	}

#ifdef _DEBUG
static fy_boolean validate(fy_context *context, fy_int handle, fy_field *field) {
	fy_class *handleClass = fy_heapGetObject(context, handle)->clazz;
	fy_class *fieldClass = field->owner;
	return fy_classCanCastTo(context, handleClass, fieldClass);
}
#endif

fy_int fy_heapArrayLength(fy_context *context, fy_int handle,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	return obj->length;
}

fy_boolean fy_heapGetArrayBoolean(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception) {

	fy_object *obj = fy_heapGetObject(context, handle);

	CHECK_NPT(0)CHECK_IOOB(0)

	return obj->data.bdata[index];

}
fy_int fy_heapGetArrayHandle(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);

	CHECK_NPT(0)CHECK_IOOB(0)

	return (fy_int) obj->data.idata[index];
}
fy_byte fy_heapGetArrayByte(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);

	CHECK_NPT(0)CHECK_IOOB(0)

	return obj->data.bdata[index];
}

fy_byte *fy_heapGetArrayBytes(fy_context *context, fy_int handle,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	return obj->data.bdata;
}

fy_short fy_heapGetArrayShort(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)CHECK_IOOB(0)

	return (fy_short) obj->data.idata[index];
}
fy_char fy_heapGetArrayChar(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)CHECK_IOOB(0)

	return (fy_char) obj->data.idata[index];
}
fy_int fy_heapGetArrayInt(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)CHECK_IOOB(0)

	return obj->data.idata[index];
}
fy_long fy_heapGetArrayLong(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)CHECK_IOOB(0)

	return obj->data.ldata[index];
}
fy_float fy_heapGetArrayFloat(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)CHECK_IOOB(0)

	return *((fy_float*) (obj->data.idata + index));
}
fy_double fy_heapGetArrayDouble(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)CHECK_IOOB(0)

	return *((fy_double*) (obj->data.ldata + index));
}

void fy_heapPutArrayBoolean(fy_context *context, fy_int handle, fy_int index,
		fy_boolean value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	obj->data.bdata[index] = value;
}
void fy_heapPutArrayHandle(fy_context *context, fy_int handle, fy_int index,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = value;
}
void fy_heapPutArrayByte(fy_context *context, fy_int handle, fy_int index,
		fy_byte value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	obj->data.bdata[index] = value;
}
void fy_heapPutArrayShort(fy_context *context, fy_int handle, fy_int index,
		fy_short value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = value;
}
void fy_heapPutArrayChar(fy_context *context, fy_int handle, fy_int index,
		fy_char value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = value;
}
void fy_heapPutArrayInt(fy_context *context, fy_int handle, fy_int index,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = value;
}
void fy_heapPutArrayLong(fy_context *context, fy_int handle, fy_int index,
		fy_long value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	obj->data.ldata[index] = value;
}
void fy_heapPutArrayFloat(fy_context *context, fy_int handle, fy_int index,
		fy_float value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = fy_floatToInt(value);
}
void fy_heapPutArrayDouble(fy_context *context, fy_int handle, fy_int index,
		fy_double value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	obj->data.ldata[index] = fy_doubleToLong(value);
}

fy_boolean fy_heapGetFieldBoolean(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_boolean) obj->data.idata[field->posAbs];
}
fy_int fy_heapGetFieldHandle(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (fy_int) obj->data.idata[field->posAbs];
}
fy_byte fy_heapGetFieldByte(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_byte) obj->data.idata[field->posAbs];
}
fy_short fy_heapGetFieldShort(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_short) obj->data.idata[field->posAbs];
}
fy_char fy_heapGetFieldChar(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_char) obj->data.idata[field->posAbs];
}
fy_int fy_heapGetFieldInt(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_int) obj->data.idata[field->posAbs];
}
fy_long fy_heapGetFieldLong(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return fy_I2TOL(obj->data.idata[field->posAbs],obj->data.idata[field->posAbs+1]);
}
fy_float fy_heapGetFieldFloat(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return *((fy_float*) ((obj->data.idata) + (field->posAbs)));
}
fy_double fy_heapGetFieldDouble(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return fy_longToDouble(
			fy_I2TOL(obj->data.idata[field->posAbs],obj->data.idata[field->posAbs+1]));
}

void fy_heapPutFieldBoolean(fy_context *context, fy_int handle, fy_field *field,
		fy_boolean value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldHandle(fy_context *context, fy_int handle, fy_field *field,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldByte(fy_context *context, fy_int handle, fy_field *field,
		fy_byte value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldShort(fy_context *context, fy_int handle, fy_field *field,
		fy_short value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldChar(fy_context *context, fy_int handle, fy_field *field,
		fy_char value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldInt(fy_context *context, fy_int handle, fy_field *field,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldLong(fy_context *context, fy_int handle, fy_field *field,
		fy_long value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	obj->data.idata[field->posAbs] = (fy_int) (value >> 32);
	obj->data.idata[field->posAbs + 1] = (fy_int) value;
}
void fy_heapPutFieldFloat(fy_context *context, fy_int handle, fy_field *field,
		fy_float value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	obj->data.idata[field->posAbs] = fy_floatToInt(value);
}
void fy_heapPutFieldDouble(fy_context *context, fy_int handle, fy_field *field,
		fy_double value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	fy_long longValue;
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	longValue = fy_doubleToLong(value);
	obj->data.idata[field->posAbs] = (fy_int) (longValue >> 32);
	obj->data.idata[field->posAbs + 1] = (fy_int) longValue;
}

fy_boolean fy_heapGetStaticBoolean(fy_context *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return field->owner->staticArea[field->posAbs];
}
fy_int fy_heapGetStaticHandle(fy_context *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return field->owner->staticArea[field->posAbs];
}
fy_byte fy_heapGetStaticByte(fy_context *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return field->owner->staticArea[field->posAbs];
}
fy_short fy_heapGetStaticShort(fy_context *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return field->owner->staticArea[field->posAbs];
}
fy_char fy_heapGetStaticChar(fy_context *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return field->owner->staticArea[field->posAbs];
}
fy_int fy_heapGetStaticInt(fy_context *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return field->owner->staticArea[field->posAbs];
}
fy_long fy_heapGetStaticLong(fy_context *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return ((fy_long) field->owner->staticArea[field->posAbs] << 32)
			| ((fy_long) field->owner->staticArea[field->posAbs + 1]);
}
fy_float fy_heapGetStaticFloat(fy_context *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return *(float*) (field->owner->staticArea + field->posAbs);
}
fy_double fy_heapGetStaticDouble(fy_context *context, fy_field *field,
		fy_exception *exception) {
	fy_long lvalue;
	CHECK_STATIC(0)
	lvalue = ((fy_long) field->owner->staticArea[field->posAbs] << 32)
			| ((fy_long) field->owner->staticArea[field->posAbs + 1]);
	return fy_longToDouble(lvalue);
}

void fy_heapPutStaticBoolean(fy_context *context, fy_field *field,
		fy_boolean value, fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = value;
}
void fy_heapPutStaticHandle(fy_context *context, fy_field *field, fy_int value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = value;
}
void fy_heapPutStaticByte(fy_context *context, fy_field *field, fy_byte value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = value;
}
void fy_heapPutStaticShort(fy_context *context, fy_field *field, fy_short value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = value;
}
void fy_heapPutStaticChar(fy_context *context, fy_field *field, fy_char value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = value;
}
void fy_heapPutStaticInt(fy_context *context, fy_field *field, fy_int value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = value;
}
void fy_heapPutStaticLong(fy_context *context, fy_field *field, fy_long value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = (fy_int) (value >> 32);
	field->owner->staticArea[field->posAbs + 1] = (fy_int) value;
}
void fy_heapPutStaticFloat(fy_context *context, fy_field *field, fy_float value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = fy_floatToInt(value);
}
void fy_heapPutStaticDouble(fy_context *context, fy_field *field,
		fy_double value, fy_exception *exception) {
	fy_long lvalue;
	CHECK_STATIC()
	lvalue = fy_doubleToLong(value);
	field->owner->staticArea[field->posAbs] = (fy_int) (lvalue >> 32);
	field->owner->staticArea[field->posAbs + 1] = (fy_int) lvalue;
}

void fy_heapGC(fy_context *context, fy_exception *exception) {

}

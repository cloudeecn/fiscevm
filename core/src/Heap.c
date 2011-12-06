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

static int fetchNextHandle(fy_context *context, fy_boolean gced,
		fy_exception *exception) {
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
			if (!gced) {
				fy_fault(exception, NULL, "Out of memory! Handle overflow");
				return 0;
			} else {
				fy_heapGC(context, exception);
				return fetchNextHandle(context, TRUE, exception);
			}
		}
	}
	return handle;
}

static void *allocateInEden(fy_context *context, fy_uint handle, fy_int size,
		fy_boolean gced, fy_exception *exception) {
	void *ret;
	if (size + context->posInEden >= EDEN_SIZE) {
		/*OOM, gc first*/
		if (gced) {
			fy_fault(
					exception,
					NULL,
					"Out of memory! Memory overflow EDEN:%d/%d COPY:%d/%d OLD:%d/%d",
					context->posInEden, EDEN_SIZE, context->posInYong,
					COPY_SIZE, context->posInOld, OLD_ENTRIES);
			return NULL;
		} else {
			fy_heapGC(context, exception);
			return allocateInEden(context, handle, size, TRUE, exception);
		}
	}
	ret = context->eden + context->posInEden;
	context->posInEden += size;
	return ret;
}

static void *allocateInOld(fy_context *context, fy_uint handle, fy_int size,
		fy_boolean gced, fy_exception *exception) {
	void *ret;
	if (size + context->posInOld >= OLD_ENTRIES) {
		/*OOM, gc first*/
		if (gced) {
			fy_fault(
					exception,
					NULL,
					"Out of memory! Memory overflow EDEN:%d/%d COPY:%d/%d OLD:%d/%d",
					context->posInEden, EDEN_SIZE, context->posInYong,
					COPY_SIZE, context->posInOld, OLD_ENTRIES);
			return NULL;
		} else {
			fy_heapGC(context, exception);
			return allocateInOld(context, handle, size, TRUE, exception);
		}
	}
	ret = context->old + context->posInOld;
	context->posInOld += size;
	return ret;
}

static int allocate(fy_context *context, fy_int size, fy_class *clazz,
		fy_int length, fy_exception *exception) {
	int handle;
	fy_object *obj;

	handle = fetchNextHandle(context, FALSE, exception);
	fy_exceptionCheckAndReturn(exception)0;
	obj = fy_heapGetObject(context,handle);

	if (size > (COPY_SIZE >> 1)) {
		obj->data = allocateInOld(context, handle, size, FALSE, exception);
	} else {
		obj->data = allocateInEden(context, handle, size, FALSE, exception);
	}
	fy_exceptionCheckAndReturn(exception)0;
	obj->length = length;
	obj->clazz = clazz;
	return handle;
}

int fy_heapAllocate(fy_context *context, fy_class *clazz,
		fy_exception *exception) {
	int length = clazz->sizeAbs;
	int size = length;

	if (clazz->type != obj) {
		fy_fault(exception, NULL, "Cannot instance Array without size");
		return 0;
	}

	return allocate(context, size, clazz, length, exception);
}

int fy_heapAllocateArray(fy_context *context, fy_class *clazz, int length,
		fy_exception *exception) {
	fy_arrayType at = clazz->ci.arr.arrayType;
	int size = length;
	if (clazz->type != arr) {
		fy_fault(exception, NULL, "Cannot instance Array without size");
		return 0;
	}

	switch (at) {
	case fy_at_long:
		size <<= 1;
		break;
	case fy_at_int:
		break;
	case fy_at_byte:
		size = (size + 3) >> 2;
		break;
	case fy_at_short:
		size = (size + 1) >> 1;
		break;
	default:
		fy_fault(exception, NULL, "Illegal array type %d", at);
		return 0;
	}

	return allocate(context, size, clazz, length, exception);
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
	fy_class *sClass, *dClass;
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
	sClass = sObject->clazz;
	dClass = dObject->clazz;
	if (sClass->type != arr || dClass->type != arr
			|| !fy_classCanCastTo(context, sClass, dClass)) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/ArrayStoreException");
		exception->exceptionDesc[0] = 0;
		return;
	}

	switch (sClass->ci.arr.arrayType) {
	case fy_at_byte:
		memcpy(((fy_ubyte*) dObject->data) + destPos,
				((fy_ubyte*) sObject->data) + srcPos, len);
		break;
	case fy_at_short:
		memcpy(((fy_char*) dObject->data) + destPos,
				((fy_char*) sObject->data) + srcPos, len << 1);
		break;
	case fy_at_int:
		memcpy(((fy_uint*) dObject->data) + destPos,
				((fy_uint*) sObject->data) + srcPos, len << 2);
		break;
	case fy_at_long:
		memcpy(((fy_ulong*) dObject->data) + destPos,
				((fy_ulong*) sObject->data) + srcPos, len << 3);
		break;
	}
}

fy_int fy_heapClone(fy_context *context, fy_int src, fy_exception *exception) {
	fy_class *clazz;
	fy_int ret;
	fy_object *sobj, *dobj;
	if (src == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return 0;
	}
	sobj = fy_heapGetObject(context,src);
	clazz = fy_heapGetClassOfObject(context, src);
	if (clazz->type == obj) {
		ret = fy_heapAllocate(context, clazz, exception);
		fy_exceptionCheckAndReturn(exception)0;
		dobj = fy_heapGetObject(context,ret);
		memcpy(dobj->data, sobj->data, clazz->sizeAbs << 2);
	} else if (clazz->type == arr) {
		ret = fy_heapAllocateArray(context, clazz, sobj->length, exception);
		fy_exceptionCheckAndReturn(exception)0;
		dobj = fy_heapGetObject(context,ret);
		switch (clazz->ci.arr.arrayType) {
		case fy_at_byte:
			memcpy(dobj->data, sobj->data, sobj->length);
			break;
		case fy_at_short:
			memcpy(dobj->data, sobj->data, sobj->length << 1);
			break;
		case fy_at_int:
			memcpy(dobj->data, sobj->data, sobj->length << 2);
			break;
		case fy_at_long:
			memcpy(dobj->data, sobj->data, sobj->length << 3);
			break;
		}
	} else {
		fy_fault(exception, NULL, "Illegal object type for clone: %d",
				clazz->type);
	}
	return ret;
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

	return ((fy_byte*) obj->data)[index];

}
fy_int fy_heapGetArrayHandle(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);

	CHECK_NPT(0)CHECK_IOOB(0)

	return ((fy_int*) obj->data)[index];
}
fy_byte fy_heapGetArrayByte(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);

	CHECK_NPT(0)CHECK_IOOB(0)

	return ((fy_byte*) obj->data)[index];
}

fy_byte *fy_heapGetArrayBytes(fy_context *context, fy_int handle,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	return (fy_byte*) obj->data;
}

fy_short fy_heapGetArrayShort(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)CHECK_IOOB(0)

	return ((fy_short*) obj->data)[index];
}
fy_char fy_heapGetArrayChar(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)CHECK_IOOB(0)

	return ((fy_char*) obj->data)[index];
}
fy_int fy_heapGetArrayInt(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)CHECK_IOOB(0)

	return ((fy_int*) obj->data)[index];
}
fy_long fy_heapGetArrayLong(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)CHECK_IOOB(0)

	return ((fy_long*) obj->data)[index];
}
fy_float fy_heapGetArrayFloat(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)CHECK_IOOB(0)

	return ((fy_float*) obj->data)[index];
}
fy_double fy_heapGetArrayDouble(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)CHECK_IOOB(0)

	return ((fy_double*) obj->data)[index];
}

void fy_heapPutArrayBoolean(fy_context *context, fy_int handle, fy_int index,
		fy_boolean value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	((fy_byte*) obj->data)[index] = value;
}
void fy_heapPutArrayHandle(fy_context *context, fy_int handle, fy_int index,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	((fy_int*) obj->data)[index] = value;
}
void fy_heapPutArrayByte(fy_context *context, fy_int handle, fy_int index,
		fy_byte value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	((fy_byte*) obj->data)[index] = value;
}
void fy_heapPutArrayShort(fy_context *context, fy_int handle, fy_int index,
		fy_short value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	((fy_short*) obj->data)[index] = value;
}
void fy_heapPutArrayChar(fy_context *context, fy_int handle, fy_int index,
		fy_char value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()
	((fy_char*) obj->data)[index] = value;
}
void fy_heapPutArrayInt(fy_context *context, fy_int handle, fy_int index,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	((fy_int*) obj->data)[index] = value;
}
void fy_heapPutArrayLong(fy_context *context, fy_int handle, fy_int index,
		fy_long value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	((fy_long*) obj->data)[index] = value;
}
void fy_heapPutArrayFloat(fy_context *context, fy_int handle, fy_int index,
		fy_float value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	((fy_int*) obj->data)[index] = fy_floatToInt(value);
}
void fy_heapPutArrayDouble(fy_context *context, fy_int handle, fy_int index,
		fy_double value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()CHECK_IOOB()

	((fy_long*) obj->data)[index] = fy_doubleToLong(value);
}

fy_boolean fy_heapGetFieldBoolean(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)ASSERT(validate(context,handle,field));

	return (fy_boolean) (((fy_int*) obj->data)[field->posAbs]);
}
fy_int fy_heapGetFieldHandle(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (fy_int) (((fy_int*) obj->data)[field->posAbs]);
}
fy_byte fy_heapGetFieldByte(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)ASSERT(validate(context,handle,field));

	return (fy_byte) (((fy_int*) obj->data)[field->posAbs]);
}
fy_short fy_heapGetFieldShort(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)ASSERT(validate(context,handle,field));

	return (fy_short) (((fy_int*) obj->data)[field->posAbs]);
}
fy_char fy_heapGetFieldChar(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)ASSERT(validate(context,handle,field));

	return (fy_char) (((fy_int*) obj->data)[field->posAbs]);
}
fy_int fy_heapGetFieldInt(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)ASSERT(validate(context,handle,field));

	return (fy_int) (((fy_int*) obj->data)[field->posAbs]);
}
fy_long fy_heapGetFieldLong(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)ASSERT(validate(context,handle,field));

	return fy_I2TOL((((fy_int*)obj->data)[field->posAbs]),(((fy_int*)obj->data)[field->posAbs+1]));
}
fy_float fy_heapGetFieldFloat(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)ASSERT(validate(context,handle,field));

	return fy_intToFloat(fy_heapGetFieldInt(context, handle, field, exception));
}
fy_double fy_heapGetFieldDouble(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)ASSERT(validate(context,handle,field));

	return fy_longToDouble(
			fy_I2TOL(((fy_int*)obj->data)[field->posAbs],((fy_int*)obj->data)[field->posAbs+1]));
}

void fy_heapPutFieldBoolean(fy_context *context, fy_int handle, fy_field *field,
		fy_boolean value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = value;
}
void fy_heapPutFieldHandle(fy_context *context, fy_int handle, fy_field *field,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = value;
}
void fy_heapPutFieldByte(fy_context *context, fy_int handle, fy_field *field,
		fy_byte value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = value;
}
void fy_heapPutFieldShort(fy_context *context, fy_int handle, fy_field *field,
		fy_short value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = value;
}
void fy_heapPutFieldChar(fy_context *context, fy_int handle, fy_field *field,
		fy_char value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = value;
}
void fy_heapPutFieldInt(fy_context *context, fy_int handle, fy_field *field,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = value;
}
void fy_heapPutFieldLong(fy_context *context, fy_int handle, fy_field *field,
		fy_long value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = (fy_int) (value >> 32);
	((fy_int*) obj->data)[field->posAbs + 1] = (fy_int) value;
}
void fy_heapPutFieldFloat(fy_context *context, fy_int handle, fy_field *field,
		fy_float value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = fy_floatToInt(value);
}
void fy_heapPutFieldDouble(fy_context *context, fy_int handle, fy_field *field,
		fy_double value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	fy_long longValue;
	CHECK_NPT()ASSERT(validate(context,handle,field));

	longValue = fy_doubleToLong(value);
	((fy_int*) obj->data)[field->posAbs] = (fy_int) (longValue >> 32);
	((fy_int*) obj->data)[field->posAbs + 1] = (fy_int) longValue;
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

typedef struct fy_fill_literals_data {
	fy_context *context;
	fy_uint *makrs;
	fy_exception *exception;
} fy_fill_literals_data;

#define SHIFT 5
#define MASK ((1<<SHIFT)-1)
#define expandToInt(X) (((X)+MASK)>>SHIFT)

static void setBit(fy_uint *marks, fy_uint pos) {
	marks[pos >> SHIFT] |= 1 << (pos & MASK);
}

static void clearBit(fy_uint *marks, fy_uint pos) {
	marks[pos >> SHIFT] &= ~(fy_uint) (1 << (pos & MASK));
}

static fy_boolean getBit(fy_uint *marks, fy_uint pos) {
	return (marks[pos >> SHIFT] >> (pos & MASK)) & 1;
}

static void fillLiterals(fy_str *key, void *value, void *data) {
	fy_fill_literals_data *ffld = (fy_fill_literals_data*) data;
	fy_uint handle = *(fy_uint*) value;
	setBit(ffld->makrs, handle);
}

static void fillInitialHandles(fy_context *context, fy_uint *marks,
		fy_exception *exception) {
	fy_uint i, imax, j, jmax;
	fy_class *clazz;
	fy_field *field;
	fy_fill_literals_data ffld;
	fy_thread *thread;

	/*Classes*/
	imax = context->classesCount;
	for (i = 1; i <= imax; i++) {
		/*Class object*/
		clazz = context->classes[i];
		setBit(marks, clazz->classObjId);

		/*Class static area*/
		jmax = clazz->staticSize;
		for (j = 0; i < jmax; j++) {
			field = clazz->fields[j];
			if (field->descriptor->content[0] == FY_TYPE_HANDLE
					|| field->descriptor->content[0] == FY_TYPE_ARRAY) {
				setBit(marks, clazz->staticArea[j]);
			}
		}
	}

	/*Literals*/
	{
		ffld.context = context;
		ffld.makrs = marks;
		ffld.exception = exception;

		fy_hashMapEachValue(context->memblocks, context->literals, fillLiterals,
				&ffld);
	}
	/*Thread objects*/
	for (i = 1; i < MAX_THREADS; i++) {
		thread = context->threads[i];
		if (thread != NULL) {
			setBit(marks, thread->handle);
		}
	}

	imax = context->toFinalize->length;
	for (i = 0; i < imax; i++) {
		setBit(marks, context->toFinalize->data->iValue);
	}
}

static void scanRef(fy_context *context, fy_uint *marks,
		fy_exception *exception) {
	fy_stack stack;
	fy_object *object;
	fy_field *field;
	fy_uint handle1, handle2;
	struct stackData {
		fy_uint handle;
		fy_uint fieldId;
	} data;
	fy_uint i, j;

	fy_stackInit(context->memblocks, &stack, sizeof(struct stackData), 128,
			exception);
	fy_exceptionCheckAndReturn(exception);

	/*push initial handles*/
	for (i = expandToInt(MAX_OBJECTS); i >= 0; i--) {
		if (marks[i]) {
			for (j = MASK; j >= 0; j--) {
				if (marks[i] & (1 << j)) {
					data.handle = (i << SHIFT) + j;
					data.fieldId = 0;
					fy_stackPush(context->memblocks, &stack, &data, exception);
					fy_exceptionCheckAndReturn(exception);
				}
			}
		}
	}

	while (fy_stackPop(context->memblocks, &stack, &data)) {
		handle1 = data.handle;
		object = context->objects + handle1;
		if (object->clazz->type == arr) {
			if (object->clazz->ci.arr.contentClass->type != prm) {
				for (i = object->length - 1; i >= 0; i--) {
					handle2 = fy_heapGetArrayHandle(context, handle1, i,
							exception);
					fy_exceptionCheckAndReturn(exception);
					if (!getBit(marks, handle2)) {
						setBit(marks, handle2);
						data.handle = handle2;
						data.fieldId = 0;
						fy_stackPush(context->memblocks, &stack, &data,
								exception);
						fy_exceptionCheckAndReturn(exception);
					}
				}
			}
		} else if (object->clazz->type == obj) {
			for (i = object->clazz->sizeAbs - 1; i >= 0; i--) {

			}
		} else {
			fy_fault(exception, NULL, "Illegal object type for object %d.",
					handle1);
		}
	}
}

void fy_heapGC(fy_context *context, fy_exception *exception) {
	/*Init scan for all handles*/
	fy_uint *marks;
	fy_uint i;

	marks = fy_allocate(expandToInt(MAX_OBJECTS) << SHIFT, exception);
	fy_exceptionCheckAndReturn(exception);
	/*Class handle*/

	fillInitialHandles(context, marks, exception);
	fy_exceptionCheckAndReturn(exception);
}

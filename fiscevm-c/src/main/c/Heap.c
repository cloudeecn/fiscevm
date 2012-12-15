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
#include "fyc/NConfig.h"

static int fetchNextHandle(fy_context *context, fy_boolean gced,
		fy_exception *exception) {
	int handle = context->nextHandle;
	while (1) {
		if (fy_heapGetObject(context, handle)->object_data == NULL ) {
			break;
		}
		handle++;
		if (handle >= MAX_OBJECTS) {
			handle = 1;
		}
		if (handle == context->nextHandle) {
			/*TODO OOM*/
			if (gced) {
				fy_fault(exception, NULL, "Out of memory! Handle overflow");
				return 0;
			} else {
				DLOG(context, "Call GC due to handle full\n")
;				fy_heapGC(context, exception);
				return fetchNextHandle(context, TRUE, exception);
			}
		}
	}
	return handle;
}

static int allocate(fy_context *context, fy_int size, fy_class *clazz,
		fy_int length, fy_uint toHandle, enum fy_heapPos pos,
		fy_exception *exception) {
	int handle;
	fy_object *obj;

	handle =
			toHandle == 0 ?
					fetchNextHandle(context, FALSE, exception) : toHandle;
	FYEH()0;
#ifdef FY_VERBOSE
	context->logDVar(context, "Allocating object #%d for class", handle);
	context->logDStr(context, clazz->className);
	context->logDVarLn(context, "");
#endif
	obj = fy_heapGetObject(context,handle);

	switch (pos) {
	case automatic:
		if (size > (COPY_ENTRIES >> 1)) {
			obj->object_data = fy_mmAllocateInOld(context->memblocks, handle,
					size + ((sizeof(fy_object_data) + 3) >> 2) + 1, FALSE,
					exception);
			*(fy_uint*) (obj->object_data) = handle;
			obj->object_data = (fy_object_data *) ((fy_uint*) (obj->object_data)
					+ 1);
			memset(obj->object_data, 0,
					(size + ((sizeof(fy_object_data) + 3) >> 2))
							* sizeof(fy_uint));
			obj->object_data->position = old;
		} else {
			obj->object_data = fy_mmAllocateInEden(context->memblocks, handle,
					size + ((sizeof(fy_object_data) + 3) >> 2), FALSE,
					exception);
			memset(obj->object_data, 0,
					(size + ((sizeof(fy_object_data) + 3) >> 2))
							* sizeof(fy_uint));
			obj->object_data->position = eden;
		}
		break;
	case eden:
		obj->object_data = fy_mmAllocateDirectInEden(context->memblocks,
				size + ((sizeof(fy_object_data) + 3) >> 2), exception);
		break;
	case young:
		obj->object_data = fy_mmAllocateDirectInCopy(context->memblocks,
				size + ((sizeof(fy_object_data) + 3) >> 2), exception);
		break;
	case old:
		obj->object_data = fy_mmAllocateDirectInOld(context->memblocks,
				size + ((sizeof(fy_object_data) + 3) >> 2) + 1, exception);
		*(fy_uint*) (obj->object_data) = handle;
		obj->object_data =
				(fy_object_data *) ((fy_uint*) (obj->object_data) + 1);
		break;
	default:
		fy_fault(exception, NULL, "Illegal pos in heap %d.", pos);
		return 0;
	}
	FYEH()0;
	obj->object_data->length = length;
	obj->object_data->clazz = clazz;
	if (context->protectMode) {
		fy_arrayListAdd(context->memblocks, context->protected, &handle,
				exception);
		FYEH()0;
	}
	return handle;
}

fy_uint fy_heapAllocateDirect(fy_context *context, fy_int size, fy_class *clazz,
		fy_int length, fy_uint toHandle, enum fy_heapPos pos,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context,toHandle);
	if (obj->object_data != NULL ) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
				"Handle %d already allocated.", toHandle);
	}
	return allocate(context, size, clazz, length, toHandle, pos, exception);
}

fy_uint fy_heapAllocate(fy_context *context, fy_class *clazz,
		fy_exception *exception) {
	fy_int length = clazz->sizeAbs;
	fy_int size = length;

	if (clazz->type != object_class) {
		fy_fault(exception, NULL, "Cannot instance Array without size");
		return 0;
	}

	return allocate(context, size, clazz, length, 0, automatic, exception);
}

void fy_heapBeginProtect(fy_context *context) {
	context->protectMode = TRUE;
}

void fy_heapEndProtect(fy_context *context) {
	context->protectMode = FALSE;
	fy_arrayListClear(context->memblocks, context->protected);
}

fy_int fy_heapGetArraySizeFromLength(fy_class *clazz, fy_int length) {
	switch (clazz->ci.arr.arrayType) {
	case fy_at_long:
		return length <<= 1;
	case fy_at_int:
		return length;
	case fy_at_byte:
		return (length + 3) >> 2;
	case fy_at_short:
		return (length + 1) >> 1;
		break;
	default:
		fy_fault(NULL, NULL, "Illegal array type %d", clazz->ci.arr.arrayType);
		return 0;
	}
}

fy_uint fy_heapAllocateArray(fy_context *context, fy_class *clazz,
		fy_int length, fy_exception *exception) {
	fy_int size;

	if (clazz->type != array_class) {
		fy_fault(exception, NULL, "Cannot instance Array with object class");
		return 0;
	}

	size = fy_heapGetArraySizeFromLength(clazz, length);

	return allocate(context, size, clazz, length, 0, automatic, exception);
}

fy_class* fy_heapGetClassOfObject(fy_context *context, fy_int handle,
		fy_exception *exception) {
	if (handle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return 0;
	}
	return fy_heapGetObject(context, handle)->object_data->clazz;
}

fy_str* fy_heapGetString(fy_context *context, fy_int handle, fy_str *target,
		fy_exception *exception) {
	fy_int i;
	fy_field *valueField, *offsetField, *countField;
	fy_int ofs, len;
	fy_int cah;
	if (handle == 0) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				FY_EXCEPTION_NPT);
		exception->exceptionDesc[0] = 0;
		return target;
	}
	fy_vmLookupClass(context, context->sString, exception);
	if (exception->exceptionType != exception_none) {
		return NULL ;
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
				FY_EXCEPTION_NPT);
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
	if (target == NULL ) {
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
	/*GC Safe*/
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
	if (pInt == NULL ) {
		pInt = fy_mmAllocatePerm(block, sizeof(fy_int), exception);
		FYEH()0;
		*pInt = fy_heapMakeString(context, str, exception);
		if (exception->exceptionType != exception_none) {
			fy_mmFree(block, pInt);
			return 0;
		}
		fy_hashMapPut(block, context->literals, str, pInt, exception);
		FYEH()0;
	}
	return *pInt;
}

void fy_heapArrayCopy(fy_context *context, fy_int src, fy_int srcPos,
		fy_int dest, fy_int destPos, fy_int len, fy_exception *exception) {
	fy_object *sObject, *dObject;
	fy_class *sClass, *dClass;
	if (src == 0 || dest == 0) {
		fy_fault(exception, FY_EXCEPTION_STORE, "%s is null",
				src == 0 ? dest == 0 ? "both" : "src":"dest");
		return;
	}
	sObject = fy_heapGetObject(context, src);
	dObject = fy_heapGetObject(context, dest);
	if (srcPos < 0 || destPos < 0 || len < 0
			|| (srcPos + len > sObject->object_data->length)
			|| (destPos + len > dObject->object_data->length)) {
		fy_fault(exception, FY_EXCEPTION_AIOOB, "0");
		return;
	}
	sClass = sObject->object_data->clazz;
	dClass = dObject->object_data->clazz;
	if (sClass->type != array_class || dClass->type != array_class
	/*TODO still need more study...
	 * || !fy_classCanCastTo(context, sClass, dClass)
	 * @see also void com.cirnoworks.fisce.vm.default_impl.ArrayHeap.arrayCopy
	 * (int srcHandle, int srcOfs, int dstHandle, int dstOfs, int count) throws VMException
	 * */) {
		fy_fault(exception, FY_EXCEPTION_STORE, "class cast");
		return;
	}

	switch (sClass->ci.arr.arrayType) {
	case fy_at_byte:
		memcpy(((fy_ubyte*) dObject->object_data->data) + destPos,
				((fy_ubyte*) sObject->object_data->data) + srcPos, len);
		break;
	case fy_at_short:
		memcpy(((fy_char*) dObject->object_data->data) + destPos,
				((fy_char*) sObject->object_data->data) + srcPos, len << 1);
		break;
	case fy_at_int:
		memcpy(((fy_uint*) dObject->object_data->data) + destPos,
				((fy_uint*) sObject->object_data->data) + srcPos, len << 2);
		break;
	case fy_at_long:
		memcpy(((fy_ulong*) dObject->object_data->data) + destPos,
				((fy_ulong*) sObject->object_data->data) + srcPos, len << 3);
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
	clazz = fy_heapGetClassOfObject(context, src, exception);
	FYEH()0;
	if (clazz->type == object_class) {
		ret = fy_heapAllocate(context, clazz, exception);
		FYEH()0;
		dobj = fy_heapGetObject(context,ret);
		memcpy(dobj->object_data, sobj->object_data,
				sizeof(fy_object_data) + (clazz->sizeAbs << 2));
	} else if (clazz->type == array_class) {
		ret = fy_heapAllocateArray(context, clazz, sobj->object_data->length,
				exception);
		FYEH()0;
		dobj = fy_heapGetObject(context,ret);
		switch (clazz->ci.arr.arrayType) {
		case fy_at_byte:
			memcpy(dobj->object_data->data, sobj->object_data->data,
					sobj->object_data->length);
			break;
		case fy_at_short:
			memcpy(dobj->object_data->data, sobj->object_data->data,
					sobj->object_data->length << 1);
			break;
		case fy_at_int:
			memcpy(dobj->object_data->data, sobj->object_data->data,
					sobj->object_data->length << 2);
			break;
		case fy_at_long:
			memcpy(dobj->object_data->data, sobj->object_data->data,
					sobj->object_data->length << 3);
			break;
		}
	} else {
		fy_fault(exception, NULL, "Illegal object type for clone: %d",
				clazz->type);
		return 0;
	}
	return ret;
}

#define CHECK_NPT(X) if (handle == 0) { \
		exception->exceptionType = exception_normal; \
		strcpy_s(exception->exceptionName,sizeof(exception->exceptionName), FY_EXCEPTION_NPT); \
		exception->exceptionDesc[0] = 0; \
		return X; \
	} else ASSERT(obj->object_data!=NULL);

#define CHECK_IOOB(X) if (index < 0 || index >= obj->object_data->length) {\
		exception->exceptionType = exception_normal;\
		strcpy_s(exception->exceptionName,sizeof(exception->exceptionName), FY_EXCEPTION_AIOOB);\
		sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),\
				"%"FY_PRINT32"d / %"FY_PRINT32"d", index, obj->object_data->length);\
		return X;\
	}

#define CHECK_STATIC(X) if((field->access_flags & FY_ACC_STATIC)==0){\
		exception->exceptionType=exception_normal;\
		strcpy_s(exception->exceptionName,sizeof(exception->exceptionName),FY_EXCEPTION_INCOMPAT_CHANGE);\
		strcpy_s(exception->exceptionDesc,sizeof(exception->exceptionDesc),"get/set static field is not static!");\
		return X;\
	}

#ifdef FY_DEBUG
static fy_boolean validate(fy_context *context, fy_int handle, fy_field *field) {
	fy_class *handleClass =
			fy_heapGetObject(context, handle)->object_data->clazz;
	fy_class *fieldClass = field->owner;
	return fy_classCanCastTo(context, handleClass, fieldClass);
}
#endif

fy_int fy_heapArrayLength(fy_context *context, fy_int handle,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	return obj->object_data->length;
}

fy_boolean fy_heapGetArrayBoolean(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception) {

	fy_object *obj = fy_heapGetObject(context, handle);

	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_byte*) obj->object_data->data)[index];

}
fy_int fy_heapGetArrayHandle(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);

	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_int*) obj->object_data->data)[index];
}
fy_byte fy_heapGetArrayByte(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);

	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_byte*) obj->object_data->data)[index];
}

fy_byte *fy_heapGetArrayBytes(fy_context *context, fy_int handle,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	return (fy_byte*) obj->object_data->data;
}

fy_short fy_heapGetArrayShort(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_short*) obj->object_data->data)[index];
}
fy_char fy_heapGetArrayChar(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_char*) obj->object_data->data)[index];
}
fy_int fy_heapGetArrayInt(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_int*) obj->object_data->data)[index];
}
fy_long fy_heapGetArrayLong(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_long*) obj->object_data->data)[index];
}
fy_float fy_heapGetArrayFloat(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_float*) obj->object_data->data)[index];
}
fy_double fy_heapGetArrayDouble(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_double*) obj->object_data->data)[index];
}

void fy_heapPutArrayBoolean(fy_context *context, fy_int handle, fy_int index,
		fy_boolean value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_byte*) obj->object_data->data)[index] = value;
}
void fy_heapPutArrayHandle(fy_context *context, fy_int handle, fy_int index,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_int*) obj->object_data->data)[index] = value;
}
void fy_heapPutArrayByte(fy_context *context, fy_int handle, fy_int index,
		fy_byte value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_byte*) obj->object_data->data)[index] = value;
}
void fy_heapPutArrayShort(fy_context *context, fy_int handle, fy_int index,
		fy_short value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_short*) obj->object_data->data)[index] = value;
}
void fy_heapPutArrayChar(fy_context *context, fy_int handle, fy_int index,
		fy_char value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()
	((fy_char*) obj->object_data->data)[index] = value;
}
void fy_heapPutArrayInt(fy_context *context, fy_int handle, fy_int index,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_int*) obj->object_data->data)[index] = value;
}
void fy_heapPutArrayLong(fy_context *context, fy_int handle, fy_int index,
		fy_long value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_long*) obj->object_data->data)[index] = value;
}
void fy_heapPutArrayFloat(fy_context *context, fy_int handle, fy_int index,
		fy_float value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_int*) obj->object_data->data)[index] = fy_floatToInt(value);
}
void fy_heapPutArrayDouble(fy_context *context, fy_int handle, fy_int index,
		fy_double value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_long*) obj->object_data->data)[index] = fy_doubleToLong(value);
}

fy_boolean fy_heapGetFieldBoolean(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_boolean) (((fy_int*) obj->object_data->data)[field->posAbs]);
}
fy_int fy_heapGetFieldHandle(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->object_data!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (fy_int) (((fy_int*) obj->object_data->data)[field->posAbs]);
}
fy_byte fy_heapGetFieldByte(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_byte) (((fy_int*) obj->object_data->data)[field->posAbs]);
}
fy_short fy_heapGetFieldShort(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_short) (((fy_int*) obj->object_data->data)[field->posAbs]);
}
fy_char fy_heapGetFieldChar(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_char) (((fy_int*) obj->object_data->data)[field->posAbs]);
}
fy_int fy_heapGetFieldInt(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_int) (((fy_int*) obj->object_data->data)[field->posAbs]);
}
fy_long fy_heapGetFieldLong(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));
#ifdef FY_VERBOSE
	context->logDVar(context, "high=%ud\nlow=%ud\n",
			((fy_int*) obj->object_data->data)[field->posAbs],
			((fy_int*) obj->object_data->data)[field->posAbs + 1]);
#endif
	return fy_I2TOL((((fy_int*)obj->object_data->data)[field->posAbs]),(((fy_int*)obj->object_data->data)[field->posAbs+1])) ;
}
fy_float fy_heapGetFieldFloat(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
#ifdef FY_DEBUG
	fy_object *obj = fy_heapGetObject(context, handle);
#endif
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return fy_intToFloat(fy_heapGetFieldInt(context, handle, field, exception));
}
fy_double fy_heapGetFieldDouble(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return fy_longToDouble(
			fy_I2TOL(((fy_int*)obj->object_data->data)[field->posAbs],((fy_int*)obj->object_data->data)[field->posAbs+1]) );
}

void fy_heapPutFieldBoolean(fy_context *context, fy_int handle, fy_field *field,
		fy_boolean value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));
#ifdef FY_VERBOSE
	context->logDVarLn(context, "address=%"FY_PRINT64"x",
			(fy_long) ((fy_int*) obj->object_data->data + field->posAbs));
#endif
	((fy_int*) obj->object_data->data)[field->posAbs] = value;
}
void fy_heapPutFieldHandle(fy_context *context, fy_int handle, fy_field *field,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));
#ifdef FY_VERBOSE
	context->logDVarLn(context, "address=%"FY_PRINT64"x",
			(fy_long) ((fy_int*) obj->object_data->data + field->posAbs));
#endif
	((fy_int*) obj->object_data->data)[field->posAbs] = value;
}
void fy_heapPutFieldByte(fy_context *context, fy_int handle, fy_field *field,
		fy_byte value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));
#ifdef FY_VERBOSE
	context->logDVarLn(context, "address=%"FY_PRINT64"x",
			(fy_long) ((fy_int*) obj->object_data->data + field->posAbs));
#endif
	((fy_int*) obj->object_data->data)[field->posAbs] = value;
}
void fy_heapPutFieldShort(fy_context *context, fy_int handle, fy_field *field,
		fy_short value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));
#ifdef FY_VERBOSE
	context->logDVarLn(context, "address=%"FY_PRINT64"x",
			(fy_long) ((fy_int*) obj->object_data->data + field->posAbs));
#endif
	((fy_int*) obj->object_data->data)[field->posAbs] = value;
}
void fy_heapPutFieldChar(fy_context *context, fy_int handle, fy_field *field,
		fy_char value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));
#ifdef FY_VERBOSE
	context->logDVarLn(context, "address=%"FY_PRINT64"x",
			(fy_long) ((fy_int*) obj->object_data->data + field->posAbs));
#endif
	((fy_int*) obj->object_data->data)[field->posAbs] = value;
}
void fy_heapPutFieldInt(fy_context *context, fy_int handle, fy_field *field,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));
#ifdef FY_VERBOSE
	context->logDVarLn(context, "address=%"FY_PRINT64"x",
			(fy_long) ((fy_int*) obj->object_data->data + field->posAbs));
#endif
	((fy_int*) obj->object_data->data)[field->posAbs] = value;
}
void fy_heapPutFieldLong(fy_context *context, fy_int handle, fy_field *field,
		fy_long value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));
#ifdef FY_VERBOSE
	context->logDVar(context, "value=%"FY_PRINT64"d\nhigh=%ud\nlow=%ud\n",
			value, fy_HOFL(value), fy_LOFL(value) );
	context->logDVarLn(context, "address=%"FY_PRINT64"x",
			(fy_long) ((fy_int*) obj->object_data->data + field->posAbs));
#endif
	((fy_int*) obj->object_data->data)[field->posAbs] = fy_HOFL(value);
	((fy_int*) obj->object_data->data)[field->posAbs + 1] = fy_LOFL(value);
}
void fy_heapPutFieldFloat(fy_context *context, fy_int handle, fy_field *field,
		fy_float value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));
#ifdef FY_VERBOSE
	context->logDVarLn(context, "address=%"FY_PRINT64"x",
			(fy_long) ((fy_int*) obj->object_data->data + field->posAbs));
#endif
	((fy_int*) obj->object_data->data)[field->posAbs] = fy_floatToInt(value);
}
void fy_heapPutFieldDouble(fy_context *context, fy_int handle, fy_field *field,
		fy_double value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	fy_long longValue;
	CHECK_NPT()
	ASSERT(validate(context,handle,field));
#ifdef FY_VERBOSE
	context->logDVarLn(context, "address=%"FY_PRINT64"x",
			(fy_long) ((fy_int*) obj->object_data->data + field->posAbs));
#endif
	longValue = fy_doubleToLong(value);
	((fy_int*) obj->object_data->data)[field->posAbs] = fy_HOFL(longValue);
	((fy_int*) obj->object_data->data)[field->posAbs + 1] = fy_LOFL(longValue);
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
	return fy_I2TOL(field->owner->staticArea[field->posAbs],
			field->owner->staticArea[field->posAbs + 1]) ;
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
	lvalue = fy_I2TOL(field->owner->staticArea[field->posAbs],
			field->owner->staticArea[field->posAbs + 1]);
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
	field->owner->staticArea[field->posAbs] = fy_HOFL(value);
	field->owner->staticArea[field->posAbs + 1] = fy_LOFL( value);
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
	field->owner->staticArea[field->posAbs] = fy_HOFL(lvalue);
	field->owner->staticArea[field->posAbs + 1] = fy_LOFL(lvalue);
}

typedef struct fy_fill_literals_data {
	fy_context *context;
	fy_uint *makrs;
	fy_exception *exception;
} fy_fill_literals_data;

static void fillLiterals(fy_str *key, void *value, void *data) {
	fy_fill_literals_data *ffld = (fy_fill_literals_data*) data;
	fy_uint handle = *(fy_uint*) value;
	fy_bitSet(ffld->makrs, handle);
}

static void fillInitialHandles(fy_context *context, fy_uint *marks,
		fy_exception *exception) {
	fy_uint i, imax, j, jmax;
	fy_class *clazz;
	fy_field *field;
	fy_fill_literals_data ffld;
	fy_thread *thread;
	fy_class *classClass, *classMethod, *classField, *classConstructor;
	fy_object *object;

	classClass = fy_vmLookupClass(context, context->sClassClass, exception);
	FYEH();
	classMethod = fy_vmLookupClass(context, context->sClassMethod, exception);
	FYEH();
	classField = fy_vmLookupClass(context, context->sClassField, exception);
	FYEH();
	classConstructor = fy_vmLookupClass(context, context->sClassConstructor,
			exception);
	FYEH();

	/*Reflection objects*/
	imax = MAX_OBJECTS;
	for (i = 1; i < MAX_OBJECTS; i++) {
		object = context->objects + i;
		if (object->object_data != NULL ) {
			clazz = object->object_data->clazz;
			if (clazz == classClass || clazz == classMethod
					|| clazz == classField || clazz == classConstructor) {
				fy_bitSet(marks, i);
			}
		}
	}

	/*Class static area*/
	imax = context->classesCount;
	for (i = 1; i <= imax; i++) {
		clazz = context->classes[i];
		if (clazz->phase == 2) {
			jmax = clazz->staticSize;
			for (j = 0; j < jmax; j++) {
				field = clazz->fieldStatic[j];
				if (field != NULL
						&& (field->descriptor->content[0] == FY_TYPE_HANDLE
								|| field->descriptor->content[0]
										== FY_TYPE_ARRAY)) {
					fy_bitSet(marks, clazz->staticArea[j]);
				}
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
		if (thread != NULL ) {
			fy_bitSet(marks, thread->handle);
			fy_bitSet(marks, thread->waitForLockId);
			fy_bitSet(marks, thread->waitForNotifyId);
			fy_bitSet(marks, thread->currentThrowable);
			fy_threadScanRef(context, thread, marks);
			FYEH();
		}
	}

	imax = context->toFinalize->length;
	for (i = 0; i < imax; i++) {
		fy_bitSet(marks,
				*(fy_int*) fy_arrayListGet(context->memblocks, context->toFinalize, i, NULL));
	}

	imax = context->protected->length;
	for (i = 0; i < imax; i++) {
		fy_bitSet(marks,
				*(fy_int*) fy_arrayListGet(context->memblocks, context->protected, i, NULL));
	}

	/*clear NULL*/
	fy_bitClear(marks, 0);
}

static void scanRef(fy_context *context, fy_arrayList *from, fy_uint *marks,
		fy_exception *exception) {

	fy_object *object;
	fy_int i;
	fy_uint handle;

#ifndef FY_LATE_DECLARATION
	fy_uint handle1, handle2;
	fy_class *clazz;
	fy_field *field;
	fy_char fieldType;
#endif

	while (fy_arrayListPop(context->memblocks, from, &handle)) {
#ifdef FY_LATE_DECLARATION
		fy_class *clazz;
		fy_uint handle2;
		fy_field *field;
		fy_char fieldType;
#endif
		ASSERT(handle>0 && handle<MAX_OBJECTS);
		object = context->objects + handle;
		clazz = object->object_data->clazz;
		ASSERT(clazz!=NULL);
		switch (clazz->type) {
		case array_class:
			if (clazz->ci.arr.contentClass->type != primitive_class) {
				for (i = object->object_data->length - 1; i >= 0; i--) {
					handle2 = fy_heapGetArrayHandle(context, handle, i,
							exception);
					FYEH();
					if (handle2 == 0) {
						continue;
					}/**/
					ASSERT(
							handle2 > 0 && handle2 < MAX_OBJECTS && fy_heapGetObject(context,handle2)->object_data != NULL);
					if (!fy_bitGet(marks, handle2)) {
						fy_bitSet(marks, handle2);
						fy_arrayListAdd(context->memblocks, from, &handle2,
								exception);
						FYEH();
					}
				}
			}
			break;
			case object_class:
			for (i = clazz->sizeAbs - 1; i >= 0; i--) {
				field = clazz->fieldAbs[i];
				if (field == NULL) {
					continue;
				}
				fieldType = field->descriptor->content[0];
				if (fieldType == FY_TYPE_HANDLE || fieldType == FY_TYPE_ARRAY) {
					handle2 = fy_heapGetFieldHandle(context, handle,
							clazz->fieldAbs[i], exception);
					FYEH();
					if (handle2 == 0) {
						continue;
					}/**/
					ASSERT(
							handle2 > 0 && handle2 < MAX_OBJECTS && fy_heapGetObject(context,handle2)->object_data != NULL);
					if (!fy_bitGet(marks, handle2)) {
						fy_bitSet(marks, handle2);
						fy_arrayListAdd(context->memblocks, from, &handle2,
								exception);
						FYEH();
					}
				}
			}
			break;
			default:
			fy_fault(exception, NULL, "Illegal object type for object %d.",
					handle);
			break;
		}
	}
}

static fy_int getSizeFromObject(fy_context *context, fy_object *object) {
	fy_class *clazz = object->object_data->clazz;
	switch (clazz->type) {
	case array_class:
		return fy_heapGetArraySizeFromLength(clazz, object->object_data->length)
				+ ((sizeof(fy_object_data) + 3) >> 2);
		break;
	case object_class:
		return clazz->sizeAbs + ((sizeof(fy_object_data) + 3) >> 2);
		break;
	default:
		fy_fault(NULL, NULL, "Illegal class type %d in GC", clazz->type);
		return 0;
	}
}

static void compactOld(fy_context *context, fy_exception *exception) {
	fy_uint i, imax;
	fy_uint newPos = 0;
	fy_uint handle;
	fy_uint size;
	fy_object *object;
	fy_memblock *block = context->memblocks;
	imax = block->posInOld;
	for (i = 0; i < imax; i++) {
		handle = block->old[i];
		if (handle > 0 && handle < MAX_OBJECTS) {
			object = context->objects + handle;
			if (object->object_data != NULL
					&& (void*) object->object_data == block->old + i + 1) {
				/*It's a real object*/
				size = getSizeFromObject(context, object) + 1;
				if (newPos != i) {
#ifdef FY_GC_DEBUG
					context->logDVarLn(context,
							"Move %d(%d) from %d to %d size=%d", handle,
							object->object_data->clazz->type, i, newPos, size);
#endif
					memmove(block->old + newPos, block->old + i,
							size * sizeof(fy_uint));
					object->object_data = (void*) (block->old + newPos + 1);
				}
#ifdef FY_GC_DEBUG
				else {
					context->logDVarLn(context, "Hold %d(%d) at %d size=%d",
							handle, object->object_data->clazz->type, i, size);
				}
#endif
				newPos += size;
				i += size - 1;
			}
#ifdef FY_GC_DEBUG
			else {
				context->logDVarLn(context, "Ignore %d at %d", handle, i);
			}
#endif
		}
#ifdef FY_GC_DEBUG
		else {
			context->logDVarLn(context, "Ignore2 %d at %d", handle, i);
		}
#endif
	}
	block->posInOld = newPos;
	block->oldReleasedSize = 0;
}

static void moveToOld(fy_context *context, fy_class *clazz, fy_uint handle,
		fy_object *object, fy_int size, fy_exception *exception) {
	fy_memblock *block = context->memblocks;
	fy_int pos = block->posInOld;
	if (pos + size + 1 >= block->oldTop) {
		compactOld(context, exception);
		pos = block->posInOld;
		if (pos + size + 1 >= block->oldTop) {
			/*Really OOM*/
			fy_fault(exception, NULL, "Old area full");
		}
	}
	block->old[pos] = handle;
	memcpy(block->old + pos + 1, object->object_data, size * sizeof(fy_uint));
	block->posInOld = pos + size + 1;
	object->object_data = (void*) (block->old + pos + 1);
	object->object_data->position = old;
}
static void moveToYoung(fy_context *context, fy_class *clazz, fy_uint handle,
		fy_object *object, fy_int size, fy_int youngId, fy_exception *exception) {
	fy_memblock *block = context->memblocks;
	fy_int pos = block->posInYong;
	if (pos + size >= COPY_ENTRIES) {
		/*move to old*/
		moveToOld(context, clazz, handle, object, size, exception);
	} else {
		/*move to young*/
		memcpy(block->young + youngId * COPY_ENTRIES + pos, object->object_data,
				size * sizeof(fy_uint));
		block->posInYong = pos + size;
		object->object_data = (void*) (block->young + youngId * COPY_ENTRIES
				+ pos);
		object->object_data->position = young;
		object->object_data->gen++;
	}
}

static void move(fy_context *context, fy_class *clazz, fy_uint handle,
		fy_object *object, fy_int youngId, fy_exception *exception) {
	fy_int size;

	size = getSizeFromObject(context, object);

	if (object->object_data->gen > MAX_GEN) {
		moveToOld(context, clazz, handle, object, size, exception);
	} else {
		moveToYoung(context, clazz, handle, object, size, youngId, exception);
	}
}

static void release(fy_context *context, fy_uint handle) {
	fy_memblock *block = context->memblocks;
	fy_object *object = fy_heapGetObject(context,handle);
	if (object->object_data->position == old) {
		block->oldReleasedSize += getSizeFromObject(context, object) + 1;
	}
	memset(object->object_data, 0, sizeof(fy_object_data));
	object->object_data = NULL;
}

void fy_heapGC(void *ctx, fy_exception *exception) {
	/*Init scan for all handles*/
	fy_uint *marks;
	fy_object *object;
	fy_class *clazz;
	fy_arrayList from;
	fy_uint handle;
	fy_int i, j;
	fy_context *context = ctx;
	fy_memblock *block = context->memblocks;
	fy_int youngId = block->youngId;
	fy_long timeStamp;
	fy_long t1, t2, t3, t4, t5, t6, t7;
#ifdef FY_DEBUG
	context->logDVar(context,
			"#FISCE GC BEFORE %d+%d+%d total %dbytes, %d managed native bytes, %d perm bytes\n",
			block->posInEden * (fy_int) sizeof(fy_uint),
			block->posInYong * (fy_int) sizeof(fy_uint),
			block->posInOld * (fy_int) sizeof(fy_uint),
			(fy_int) ((block->posInEden + block->posInYong + block->posInOld)
					* (fy_int) sizeof(fy_uint)), context->memblocks->size,
			(fy_int) ((OLD_ENTRIES - context->memblocks->oldTop)
					* (fy_int) sizeof(fy_uint)));
#else
	context->logDVar(context,"#FISCE GC BEFORE %d+%d+%d total %dbytes, %d perm bytes\n",
			block->posInEden* (fy_int)sizeof(fy_uint), block->posInYong* (fy_int)sizeof(fy_uint), block->posInOld* (fy_int)sizeof(fy_uint),
			(fy_int) ((block->posInEden + block->posInYong + block->posInOld)
					* (fy_int)sizeof(fy_uint)),
			(fy_int) ((OLD_ENTRIES - context->memblocks->oldTop)
					* (fy_int)sizeof(fy_uint)));
#endif

	timeStamp = fy_portTimeMillSec(context->port);
	marks = /*TEMP*/fy_allocate(fy_bitSizeToInt(MAX_OBJECTS) << fy_bitSHIFT,
			exception);
	FYEH();

	t1 = fy_portTimeMillSec(context->port);
	fillInitialHandles(context, marks, exception);
	if (exception->exceptionType != exception_none) {
		fy_free(marks);
		return;
	}

	fy_arrayListInit(context->memblocks, &from, sizeof(fy_uint), 1024,
			exception);
	if (exception->exceptionType != exception_none) {
		fy_free(marks);
		return;
	}

	t2 = fy_portTimeMillSec(context->port);
	/*push initial handles*/
	for (i = fy_bitSizeToInt(MAX_OBJECTS); i >= 0; i--) {
		if (marks[i]) {
			for (j = fy_bitMASK; j >= 0; j--) {
				if (marks[i] & (1 << j)) {
					handle = (i << fy_bitSHIFT) + j;
					fy_arrayListAdd(context->memblocks, &from, &handle,
							exception);
					if (exception->exceptionType != exception_none) {
						fy_free(marks);
						fy_arrayListDestroy(context->memblocks, &from);
						return;
					}
				}
			}
		}
	}

	t3 = fy_portTimeMillSec(context->port);
	scanRef(context, &from, marks, exception);
	if (exception->exceptionType != exception_none) {
		fy_free(marks);
		fy_arrayListDestroy(context->memblocks, &from);
		return;
	}

	t4 = fy_portTimeMillSec(context->port);
	fy_arrayListClear(context->memblocks, &from);
	for (i = 1; i < MAX_OBJECTS; i++) {
		object = fy_heapGetObject(context,i);
		if (object->object_data != NULL && !fy_bitGet(marks, i)
				&& object->object_data->clazz->needFinalize
				&& object->object_data->finalizeStatus == not_finalized) {
			clazz = object->object_data->clazz;
//			context->logDVar(context,"ADD %d need finalize\n", i);
			fy_bitSet(marks, i);
			fy_arrayListAdd(context->memblocks, context->toFinalize, &i,
					exception);
			if (exception->exceptionType != exception_none) {
				fy_free(marks);
				fy_arrayListDestroy(context->memblocks, &from);
				return;
			}
			fy_arrayListAdd(context->memblocks, &from, &i, exception);
			if (exception->exceptionType != exception_none) {
				fy_free(marks);
				fy_arrayListDestroy(context->memblocks, &from);
				return;
			}
			object->object_data->finalizeStatus = in_finalize_array;
		}
	}

	t5 = fy_portTimeMillSec(context->port);
	scanRef(context, &from, marks, exception);
	fy_arrayListDestroy(context->memblocks, &from);
	if (exception->exceptionType != exception_none) {
		fy_free(marks);
		return;
	}

	t6 = fy_portTimeMillSec(context->port);
	block->posInEden = 0;
	block->posInYong = 0;
	block->youngId = youngId = 1 - block->youngId;
	for (i = 1; i < MAX_OBJECTS; i++) {
		object = fy_heapGetObject(context,i);
		if (object->object_data != NULL ) {
			clazz = object->object_data->clazz;
			if (fy_bitGet(marks, i)) {

				switch (object->object_data->position) {
				case eden:
				case young:
#ifdef FY_GC_DEBUG
					context->logDVarLn(context, "#GC move %d.", i);
#endif
					move(context, clazz, i, object, youngId, exception);
					break;
				case old:
#ifdef FY_GC_DEBUG
					context->logDVarLn(context, "#GC hold %d.", i);
#endif
					break;
				default: {
					fy_fault(exception, NULL, "Illegal position %d in heap",
							object->object_data->position);
					return;
				}
				}
			} else {
#ifdef FY_GC_DEBUG
				context->logDVarLn(context, "#GC release %d.", i);
#endif
				release(context, i);
			}
		}
	}
	t7 = fy_portTimeMillSec(context->port);
#ifdef FY_GC_FORCE_FULL
	compactOld(context, exception);
	FYEH();
#endif
#ifdef FY_DEBUG
	context->logDVar(context,
			"#FISCE GC AFTER %d+%d+%d total %dbytes, %d managed native bytes, %d perm bytes, %d context size, time=%"FY_PRINT64"d\n",
			block->posInEden * (fy_int) sizeof(fy_uint),
			block->posInYong * (fy_int) sizeof(fy_uint),
			block->posInOld * (fy_int) sizeof(fy_uint),
			(fy_int) ((block->posInEden + block->posInYong + block->posInOld)
					* (fy_int) sizeof(fy_uint)), context->memblocks->size,
			(OLD_ENTRIES - context->memblocks->oldTop)
					* (fy_int) sizeof(fy_uint), (fy_int) sizeof(fy_context),
			fy_portTimeMillSec(context->port) - timeStamp);
	context->logDVar(context, "%d %d %d %d %d %d %d %d\n", t1 - timeStamp,
			t2 - t1, t3 - t2, t4 - t3, t5 - t4, t6 - t5, t7 - t6,
			fy_portTimeMillSec(context->port) - t7);
#else
	context->logDVar(context,
			"#FISCE GC AFTER %d+%d+%d total %dbytes,%d perm bytes,%d context bytes, time=%"FY_PRINT64"d\n",
			block->posInEden* (fy_int)sizeof(fy_uint), block->posInYong* (fy_int)sizeof(fy_uint), block->posInOld* (fy_int)sizeof(fy_uint),
			(fy_int) ((block->posInEden + block->posInYong + block->posInOld)
					* (fy_int)sizeof(fy_uint)),
			(OLD_ENTRIES - context->memblocks->oldTop) * (fy_int)sizeof(fy_uint),
			(fy_int)sizeof(fy_context), fy_portTimeMillSec(context->port) - timeStamp);
	context->logDVar(context, "%d %d %d %d %d %d %d %d\n", t1 - timeStamp,
			t2 - t1, t3 - t2, t4 - t3, t5 - t4, t6 - t5, t7 - t6,
			fy_portTimeMillSec(context->port) - t7);
#endif
	fy_free(marks);
}

fy_uint fy_heapWrapBoolean(fy_context *context, fy_boolean value,
		fy_exception *exception) {
	fy_class *clazz;
	fy_field *field;
	fy_uint ret;
	clazz = fy_vmLookupClass(context, context->sBoolean, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueBoolean);
	FYEH()0;
	ret = fy_heapAllocate(context, clazz, exception);
	FYEH()0;
	fy_heapPutFieldBoolean(context, ret, field, value, exception);
	return ret;
}

fy_boolean fy_heapUnwrapBoolean(fy_context *context, fy_uint handle,
		fy_exception *exception) {
	fy_field *field;
	fy_class *clazz = fy_vmLookupClass(context, context->sClassBoolean,
			exception);
	FYEH()0;
	fy_vmLookupClass(context, context->sBoolean, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueBoolean);
	FYEH()0;
	if (fy_heapGetObject(context,handle)->object_data->clazz != clazz) {
		fy_fault(exception, FY_EXCEPTION_ARGU,
				"Can't unwarp handle=%"FY_PRINT32"d as boolean", handle);
		return 0;
	}
	return fy_heapGetFieldBoolean(context, handle, field, exception);
}

fy_uint fy_heapWrapByte(fy_context *context, fy_byte value,
		fy_exception *exception) {
	fy_class *clazz;
	fy_field *field;
	fy_uint ret;
	clazz = fy_vmLookupClass(context, context->sByte, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueByte);
	FYEH()0;
	ret = fy_heapAllocate(context, clazz, exception);
	FYEH()0;
	fy_heapPutFieldByte(context, ret, field, value, exception);
	return ret;
}

fy_byte fy_heapUnwrapByte(fy_context *context, fy_uint handle,
		fy_exception *exception) {
	fy_field *field;
	fy_class *clazz = fy_vmLookupClass(context, context->sClassByte, exception);
	FYEH()0;
	fy_vmLookupClass(context, context->sByte, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueByte);
	FYEH()0;
	if (fy_heapGetObject(context,handle)->object_data->clazz != clazz) {
		fy_fault(exception, FY_EXCEPTION_ARGU,
				"Can't unwarp handle=%"FY_PRINT32"d as byte", handle);
		return 0;
	}
	return fy_heapGetFieldByte(context, handle, field, exception);
}

fy_uint fy_heapWrapShort(fy_context *context, fy_short value,
		fy_exception *exception) {
	fy_class *clazz;
	fy_field *field;
	fy_uint ret;
	clazz = fy_vmLookupClass(context, context->sShort, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueShort);
	FYEH()0;
	ret = fy_heapAllocate(context, clazz, exception);
	FYEH()0;
	fy_heapPutFieldShort(context, ret, field, value, exception);
	return ret;
}

fy_short fy_heapUnwrapShort(fy_context *context, fy_uint handle,
		fy_exception *exception) {
	fy_field *field;
	fy_class *clazz = fy_vmLookupClass(context, context->sClassShort,
			exception);
	FYEH()0;
	fy_vmLookupClass(context, context->sShort, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueShort);
	FYEH()0;
	if (fy_heapGetObject(context,handle)->object_data->clazz != clazz) {
		fy_fault(exception, FY_EXCEPTION_ARGU,
				"Can't unwarp handle=%"FY_PRINT32"d as short", handle);
		return 0;
	}
	return fy_heapGetFieldShort(context, handle, field, exception);
}

fy_uint fy_heapWrapChar(fy_context *context, fy_char value,
		fy_exception *exception) {
	fy_class *clazz;
	fy_field *field;
	fy_uint ret;
	clazz = fy_vmLookupClass(context, context->sChar, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueChar);
	FYEH()0;
	ret = fy_heapAllocate(context, clazz, exception);
	FYEH()0;
	fy_heapPutFieldChar(context, ret, field, value, exception);
	return ret;
}

fy_char fy_heapUnwrapChar(fy_context *context, fy_uint handle,
		fy_exception *exception) {
	fy_field *field;
	fy_class *clazz = fy_vmLookupClass(context, context->sClassChar, exception);
	FYEH()0;
	fy_vmLookupClass(context, context->sChar, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueChar);
	FYEH()0;
	if (fy_heapGetObject(context,handle)->object_data->clazz != clazz) {
		fy_fault(exception, FY_EXCEPTION_ARGU,
				"Can't unwarp handle=%"FY_PRINT32"d as char", handle);
		return 0;
	}
	return fy_heapGetFieldChar(context, handle, field, exception);
}

fy_uint fy_heapWrapInt(fy_context *context, fy_int value,
		fy_exception *exception) {
	fy_class *clazz;
	fy_field *field;
	fy_uint ret;
	clazz = fy_vmLookupClass(context, context->sInt, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueInt);
	FYEH()0;
	ret = fy_heapAllocate(context, clazz, exception);
	FYEH()0;
	fy_heapPutFieldInt(context, ret, field, value, exception);
	return ret;
}

fy_int fy_heapUnwrapInt(fy_context *context, fy_uint handle,
		fy_exception *exception) {
	fy_field *field;
	fy_class * clazz = fy_vmLookupClass(context, context->sClassInt, exception);
	FYEH()0;
	fy_vmLookupClass(context, context->sInt, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueInt);
	FYEH()0;
	if (fy_heapGetObject(context,handle)->object_data->clazz != clazz) {
		fy_fault(exception, FY_EXCEPTION_ARGU,
				"Can't unwarp handle=%"FY_PRINT32"d as int", handle);
		return 0;
	}
	return fy_heapGetFieldInt(context, handle, field, exception);
}

fy_uint fy_heapWrapFloat(fy_context *context, fy_float value,
		fy_exception *exception) {
	fy_class *clazz;
	fy_field *field;
	fy_uint ret;
	clazz = fy_vmLookupClass(context, context->sFloat, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueFloat);
	FYEH()0;
	ret = fy_heapAllocate(context, clazz, exception);
	FYEH()0;
	fy_heapPutFieldFloat(context, ret, field, value, exception);
	return ret;
}

fy_float fy_heapUnwrapFloat(fy_context *context, fy_uint handle,
		fy_exception *exception) {
	fy_field *field;
	fy_class *clazz = fy_vmLookupClass(context, context->sClassFloat,
			exception);
	FYEH()0;
	fy_vmLookupClass(context, context->sFloat, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueFloat);
	FYEH()0;
	if (fy_heapGetObject(context,handle)->object_data->clazz != clazz) {
		fy_fault(exception, FY_EXCEPTION_ARGU,
				"Can't unwarp handle=%"FY_PRINT32"d as float", handle);
		return 0;
	}
	return fy_heapGetFieldFloat(context, handle, field, exception);
}

fy_uint fy_heapWrapLong(fy_context *context, fy_long value,
		fy_exception *exception) {
	fy_class *clazz;
	fy_field *field;
	fy_uint ret;
	clazz = fy_vmLookupClass(context, context->sLong, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueLong);
	FYEH()0;
	ret = fy_heapAllocate(context, clazz, exception);
	FYEH()0;
	fy_heapPutFieldLong(context, ret, field, value, exception);
	return ret;
}

fy_long fy_heapUnwrapLong(fy_context *context, fy_uint handle,
		fy_exception *exception) {
	fy_field *field;
	fy_class *clazz = fy_vmLookupClass(context, context->sClassLong, exception);
	FYEH()0;
	fy_vmLookupClass(context, context->sLong, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueLong);
	FYEH()0;
	if (fy_heapGetObject(context,handle)->object_data->clazz != clazz) {
		fy_fault(exception, FY_EXCEPTION_ARGU,
				"Can't unwarp handle=%"FY_PRINT32"d as long", handle);
		return 0;
	}
	return fy_heapGetFieldLong(context, handle, field, exception);
}

fy_uint fy_heapWrapDouble(fy_context *context, fy_double value,
		fy_exception *exception) {
	fy_class *clazz;
	fy_field *field;
	fy_uint ret;
	clazz = fy_vmLookupClass(context, context->sDouble, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueDouble);
	FYEH()0;
	ret = fy_heapAllocate(context, clazz, exception);
	FYEH()0;
	fy_heapPutFieldDouble(context, ret, field, value, exception);
	return ret;
}

fy_double fy_heapUnwrapDouble(fy_context *context, fy_uint handle,
		fy_exception *exception) {
	fy_field *field;
	fy_class *clazz = fy_vmLookupClass(context, context->sClassDouble,
			exception);
	FYEH()0;
	fy_vmLookupClass(context, context->sDouble, exception);
	FYEH()0;
	field = fy_vmGetField(context, context->sValueDouble);
	FYEH()0;
	if (fy_heapGetObject(context,handle)->object_data->clazz != clazz) {
		fy_fault(exception, FY_EXCEPTION_ARGU,
				"Can't unwarp handle=%"FY_PRINT32"d as double", handle);
		return 0;
	}
	return fy_heapGetFieldDouble(context, handle, field, exception);
}

fy_uint fy_heapLookupStringFromConstant(fy_context *context,
		ConstantStringInfo *constantStringInfo, fy_exception *exception) {
	fy_uint hvalue;
	if (!constantStringInfo->derefed) {
		fy_heapBeginProtect(context);
		hvalue = fy_heapLiteral(context, constantStringInfo->ci.string,
				exception);
		if (exception->exceptionType != exception_none) {
			return 0;
		}
		constantStringInfo->derefed = TRUE;
		constantStringInfo->ci.handle = hvalue;
	} else {
		hvalue = constantStringInfo->ci.handle;
	}
	return hvalue;
}

fy_int fy_heapMultiArray(fy_context *context, fy_class *clazz, fy_int layers,
		fy_int *elementsForEachLayer, fy_exception *exception) {
	int i;
	fy_int handle;
	int size = elementsForEachLayer[0];
	int ret = fy_heapAllocateArray(context, clazz, size, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	if (layers > 1) {
		for (i = 0; i < size; i++) {
			handle = fy_heapMultiArray(context, clazz->ci.arr.contentClass,
					layers - 1, elementsForEachLayer + 1, exception);
			if (exception->exceptionType != exception_none) {
				return 0;
			}
			fy_heapPutArrayHandle(context, ret, i, handle, exception);
			if (exception->exceptionType != exception_none) {
				return 0;
			}
		}
	}
	return ret;
}

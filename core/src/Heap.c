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
				DLOG("Call GC due to handle full\n");
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
	if (size + context->posInEden >= EDEN_ENTRIES) {
		/*OOM, gc first*/
		if (gced) {
			fy_fault(exception, NULL,
					"Out of memoryE! Memory overflow size=%d EDEN:%d/%d COPY:%d/%d OLD:%d/%d\n",
					size, context->posInEden, EDEN_ENTRIES, context->posInYong,
					COPY_ENTRIES, context->posInOld, context->oldTop);
			return NULL;
		} else {
			DLOG("Call GC due to eden full");
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
	if (size + context->posInOld >= context->oldTop) {
		/*OOM, gc first*/
		if (gced) {
			fy_fault(exception, NULL,
					"Out of memoryO! Memory overflow size=%d EDEN:%d/%d COPY:%d/%d OLD:%d/%d\n",
					size, context->posInEden, EDEN_ENTRIES, context->posInYong,
					COPY_ENTRIES, context->posInOld, context->oldTop);
			return NULL;
		} else {
			DLOG("Call GC due to old full");
			fy_heapGC(context, exception);
			return allocateInOld(context, handle, size, TRUE, exception);
		}
	}
	ret = context->old + context->posInOld;
	context->posInOld += size;
	return ret;
}

static void *allocateDirectInEden(fy_context *context, fy_int size,
		fy_exception *exception) {
	void *ret;
	if (size + context->posInEden >= EDEN_ENTRIES) {
		/*OOM, gc first*/
		fy_fault(exception, NULL,
				"Out of memoryDE! Memory overflow size=%d EDEN:%d/%d COPY:%d/%d OLD:%d/%d\n",
				size, context->posInEden, EDEN_ENTRIES, context->posInYong,
				COPY_ENTRIES, context->posInOld, context->oldTop);
		return NULL;
	}
	ret = context->eden + context->posInEden;
	context->posInEden += size;
	return ret;
}

static void *allocateDirectInCopy(fy_context *context, fy_int size,
		fy_exception *exception) {
	void *ret;
	if (size + context->posInYong >= COPY_ENTRIES) {
		/*OOM, gc first*/
		fy_fault(exception, NULL,
				"Out of memoryDC! Memory overflow size=%d EDEN:%d/%d COPY:%d/%d OLD:%d/%d\n",
				size, context->posInEden, EDEN_ENTRIES, context->posInYong,
				COPY_ENTRIES, context->posInOld, context->oldTop);
		return NULL;
	}
	ret = context->young + context->posInYong;
	context->posInYong += size;
	return ret;
}

static void *allocateDirectInOld(fy_context *context, fy_int size,
		fy_exception *exception) {
	void *ret;
	if (size + context->posInOld >= context->oldTop) {
		/*OOM, gc first*/
		fy_fault(exception, NULL,
				"Out of memoryDO! Memory overflow size=%d EDEN:%d/%d COPY:%d/%d OLD:%d/%d\n",
				size, context->posInEden, EDEN_ENTRIES, context->posInYong,
				COPY_ENTRIES, context->posInOld, context->oldTop);
		return NULL;
	}
	ret = context->old + context->posInOld;
	context->posInOld += size;
	return ret;
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
	obj = fy_heapGetObject(context,handle);

	switch (pos) {
	case automatic:
		if (size > (COPY_ENTRIES >> 1)) {
			obj->position = old;
			obj->data = allocateInOld(context, handle, size, FALSE, exception);
		} else {
			obj->position = eden;
			obj->data = allocateInEden(context, handle, size, FALSE, exception);
		}
		break;
	case eden:
		obj->data = allocateDirectInEden(context, size, exception);
		break;
	case young:
		obj->data = allocateDirectInCopy(context, size, exception);
		break;
	case old:
		obj->data = allocateDirectInOld(context, size, exception);
		break;
	default:
		fy_fault(exception, NULL, "Illegal pos in heap %d.", pos);
		return 0;
	}
	FYEH()0;
	memset(obj->data, 0, size * sizeof(fy_uint));
	obj->length = length;
	obj->clazz = clazz;
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
	if (obj->clazz != NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
				"Handle %d already allocated.", toHandle);
	}
	return allocate(context, size, clazz, length, toHandle, pos, exception);
}

fy_uint fy_heapAllocate(fy_context *context, fy_class *clazz,
		fy_exception *exception) {
	fy_int length = clazz->sizeAbs;
	fy_int size = length;

	if (clazz->type != obj) {
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

	if (clazz->type != arr) {
		fy_fault(exception, NULL, "Cannot instance Array with object class");
		return 0;
	}

	size = fy_heapGetArraySizeFromLength(clazz, length);

	return allocate(context, size, clazz, length, 0, automatic, exception);
}

fy_class* fy_heapGetClassOfObject(fy_context *context, fy_int handle) {
	return fy_heapGetObject(context, handle)->clazz;
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
	if (pInt == NULL) {
		pInt = fy_mmAllocate(block, sizeof(fy_int), exception);
		FYEH() 0;
		*pInt = fy_heapMakeString(context, str, exception);
		if (exception->exceptionType != exception_none) {
			fy_mmFree(block, pInt);
			return 0;
		}
		fy_hashMapPut(block, context->literals, str, pInt, exception);
		FYEH() 0;
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
	if (srcPos < 0 || destPos < 0 || len < 0 || (srcPos + len > sObject->length)
			|| (destPos + len > dObject->length)) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "0");
		return;
	}
	sClass = sObject->clazz;
	dClass = dObject->clazz;
	if (sClass->type != arr || dClass->type != arr
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
		FYEH()0;
		dobj = fy_heapGetObject(context,ret);
		memcpy(dobj->data, sobj->data, clazz->sizeAbs << 2);
	} else if (clazz->type == arr) {
		ret = fy_heapAllocateArray(context, clazz, sobj->length, exception);
		FYEH()0;
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
strcpy_s(exception->exceptionName,sizeof(exception->exceptionName), FY_EXCEPTION_NPT); \
exception->exceptionDesc[0] = 0; \
return X; \
}else ASSERT(obj->clazz!=NULL);

#define CHECK_IOOB(X) if (index < 0 || index >= obj->length) {\
exception->exceptionType = exception_normal;\
strcpy_s(exception->exceptionName,sizeof(exception->exceptionName), FY_EXCEPTION_IOOB);\
sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),\
		"%"FY_PRINT32"d / %"FY_PRINT32"d", index, obj->length);\
return X;\
}

#define CHECK_STATIC(X) if((field->access_flags & FY_ACC_STATIC)==0){\
		exception->exceptionType=exception_normal;\
		strcpy_s(exception->exceptionName,sizeof(exception->exceptionName),FY_EXCEPTION_INCOMPAT_CHANGE);\
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

	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_byte*) obj->data)[index];

}
fy_int fy_heapGetArrayHandle(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);

	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_int*) obj->data)[index];
}
fy_byte fy_heapGetArrayByte(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);

	CHECK_NPT(0)
	CHECK_IOOB(0)

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
	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_short*) obj->data)[index];
}
fy_char fy_heapGetArrayChar(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_char*) obj->data)[index];
}
fy_int fy_heapGetArrayInt(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_int*) obj->data)[index];
}
fy_long fy_heapGetArrayLong(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_long*) obj->data)[index];
}
fy_float fy_heapGetArrayFloat(fy_context *context, fy_int handle, fy_int index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_float*) obj->data)[index];
}
fy_double fy_heapGetArrayDouble(fy_context *context, fy_int handle,
		fy_int index, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	CHECK_IOOB(0)

	return ((fy_double*) obj->data)[index];
}

void fy_heapPutArrayBoolean(fy_context *context, fy_int handle, fy_int index,
		fy_boolean value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_byte*) obj->data)[index] = value;
}
void fy_heapPutArrayHandle(fy_context *context, fy_int handle, fy_int index,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_int*) obj->data)[index] = value;
}
void fy_heapPutArrayByte(fy_context *context, fy_int handle, fy_int index,
		fy_byte value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_byte*) obj->data)[index] = value;
}
void fy_heapPutArrayShort(fy_context *context, fy_int handle, fy_int index,
		fy_short value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_short*) obj->data)[index] = value;
}
void fy_heapPutArrayChar(fy_context *context, fy_int handle, fy_int index,
		fy_char value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()
	((fy_char*) obj->data)[index] = value;
}
void fy_heapPutArrayInt(fy_context *context, fy_int handle, fy_int index,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_int*) obj->data)[index] = value;
}
void fy_heapPutArrayLong(fy_context *context, fy_int handle, fy_int index,
		fy_long value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_long*) obj->data)[index] = value;
}
void fy_heapPutArrayFloat(fy_context *context, fy_int handle, fy_int index,
		fy_float value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_int*) obj->data)[index] = fy_floatToInt(value);
}
void fy_heapPutArrayDouble(fy_context *context, fy_int handle, fy_int index,
		fy_double value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	CHECK_IOOB()

	((fy_long*) obj->data)[index] = fy_doubleToLong(value);
}

fy_boolean fy_heapGetFieldBoolean(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

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
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_byte) (((fy_int*) obj->data)[field->posAbs]);
}
fy_short fy_heapGetFieldShort(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_short) (((fy_int*) obj->data)[field->posAbs]);
}
fy_char fy_heapGetFieldChar(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_char) (((fy_int*) obj->data)[field->posAbs]);
}
fy_int fy_heapGetFieldInt(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));

	return (fy_int) (((fy_int*) obj->data)[field->posAbs]);
}
fy_long fy_heapGetFieldLong(fy_context *context, fy_int handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT(0)
	ASSERT(validate(context,handle,field));
#ifdef FY_VERBOSE
	printf("high=%ud\nlow=%ud\n", ((fy_int*) obj->data)[field->posAbs],
			((fy_int*) obj->data)[field->posAbs + 1]);
#endif
	return fy_I2TOL((((fy_int*)obj->data)[field->posAbs]),(((fy_int*)obj->data)[field->posAbs+1]));
}
fy_float fy_heapGetFieldFloat(fy_context *context, fy_int handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
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
			fy_I2TOL(((fy_int*)obj->data)[field->posAbs],((fy_int*)obj->data)[field->posAbs+1]));
}

void fy_heapPutFieldBoolean(fy_context *context, fy_int handle, fy_field *field,
		fy_boolean value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = value;
}
void fy_heapPutFieldHandle(fy_context *context, fy_int handle, fy_field *field,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = value;
}
void fy_heapPutFieldByte(fy_context *context, fy_int handle, fy_field *field,
		fy_byte value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = value;
}
void fy_heapPutFieldShort(fy_context *context, fy_int handle, fy_field *field,
		fy_short value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = value;
}
void fy_heapPutFieldChar(fy_context *context, fy_int handle, fy_field *field,
		fy_char value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = value;
}
void fy_heapPutFieldInt(fy_context *context, fy_int handle, fy_field *field,
		fy_int value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = value;
}
void fy_heapPutFieldLong(fy_context *context, fy_int handle, fy_field *field,
		fy_long value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));
#ifdef FY_VERBOSE
	printf("value=%"FY_PRINT64"d\nhigh=%ud\nlow=%ud\n", value, fy_HOFL(value),
			fy_LOFL(value));
#endif
	((fy_int*) obj->data)[field->posAbs] = fy_HOFL(value);
	((fy_int*) obj->data)[field->posAbs + 1] = fy_LOFL(value);
}
void fy_heapPutFieldFloat(fy_context *context, fy_int handle, fy_field *field,
		fy_float value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	((fy_int*) obj->data)[field->posAbs] = fy_floatToInt(value);
}
void fy_heapPutFieldDouble(fy_context *context, fy_int handle, fy_field *field,
		fy_double value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	fy_long longValue;
	CHECK_NPT()
	ASSERT(validate(context,handle,field));

	longValue = fy_doubleToLong(value);
	((fy_int*) obj->data)[field->posAbs] = fy_HOFL(longValue);
	((fy_int*) obj->data)[field->posAbs + 1] = fy_LOFL(longValue);
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
			field->owner->staticArea[field->posAbs + 1]);
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

void* fy_heapPermAllocate(fy_context *context, size_t size,
		fy_exception *exception) {
	fy_int blocks = (size + 3) >> 2;
	if (context->posInOld >= context->oldTop - blocks) {
		fy_heapGC(context, exception);
		FYEH()NULL;
	}
	if (context->posInOld >= context->oldTop - blocks) {
		fy_fault(exception, NULL, "Out of memory: perm");
		FYEH()NULL;
	}
	return context->old + (context->oldTop -= blocks);
}

fy_int fy_heapPermSize(fy_context *context) {
	return OLD_ENTRIES - context->oldTop;
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

	/*Classes*/
	imax = context->classesCount;
	for (i = 1; i <= imax; i++) {
		/*Class object*/
		clazz = context->classes[i];
		fy_bitSet(marks, fy_vmGetClassObjHandle(context, clazz, exception));
		FYEH();
		/*Class static area*/
		jmax = clazz->staticSize;
		for (j = 0; j < jmax; j++) {
			field = clazz->fieldStatic[j];
			if (field != NULL
					&& (field->descriptor->content[0] == FY_TYPE_HANDLE
							|| field->descriptor->content[0] == FY_TYPE_ARRAY)) {
				fy_bitSet(marks, clazz->staticArea[j]);
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
			fy_bitSet(marks, thread->handle);
			fy_bitSet(marks, thread->waitForLockId);
			fy_bitSet(marks, thread->waitForNotifyId);
			fy_bitSet(marks, thread->currentThrowable);
			fy_threadScanRef(context, thread, marks);
		}
	}

	imax = context->toFinalize->length;
	for (i = 0; i < imax; i++) {
		fy_bitSet(marks,
				*(fy_int*) fy_arrayListGet(context->memblocks,
						context->toFinalize, i, NULL));
	}

	imax = context->protected->length;
	for (i = 0; i < imax; i++) {
		fy_bitSet(marks,
				*(fy_int*) fy_arrayListGet(context->memblocks,
						context->protected, i, NULL));
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
		clazz = object->clazz;
		ASSERT(clazz!=NULL);
		switch (clazz->type) {
		case arr:
			if (clazz->ci.arr.contentClass->type != prm) {
				for (i = object->length - 1; i >= 0; i--) {
					handle2 = fy_heapGetArrayHandle(context, handle, i,
							exception);
					FYEH();
					if (handle2 == 0) {
						continue;
					}/**/
					ASSERT(
							handle2 > 0 && handle2 < MAX_OBJECTS && fy_heapGetObject(context,handle2)->clazz != NULL);
					if (!fy_bitGet(marks, handle2)) {
						fy_bitSet(marks, handle2);
						fy_arrayListAdd(context->memblocks, from, &handle2,
								exception);
						FYEH();
					}
				}
			}
			break;
		case obj:
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
							handle2 > 0 && handle2 < MAX_OBJECTS && fy_heapGetObject(context,handle2)->clazz != NULL);
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
	fy_class *clazz = object->clazz;
	switch (clazz->type) {
	case arr:
		return fy_heapGetArraySizeFromLength(clazz, object->length);
		break;
	case obj:
		return clazz->sizeAbs;
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
	imax = context->posInOld;
	for (i = 0; i < imax; i++) {
		handle = context->old[i];
		if (handle > 0 && handle < MAX_OBJECTS) {
			object = context->objects + handle;
			if (object->clazz != NULL && object->data == context->old + i + 1) {
				/*It's a real object*/
				size = getSizeFromObject(context, object) + 1;
				if (newPos != i) {
					memmove(context->old + newPos, context->old + i,
							size * sizeof(fy_uint));
					object->data = context->old + newPos + 1;
				}
				newPos += size;
				i += size - 1;
			}
		}
	}
	context->posInOld = newPos;
}

static void moveToOld(fy_context *context, fy_class *clazz, fy_uint handle,
		fy_object *object, fy_int size, fy_exception *exception) {
	fy_int pos = context->posInOld;
	if (pos + size + 1 >= context->oldTop) {
		compactOld(context, exception);
		pos = context->posInOld;
		if (pos + size + 1 >= context->oldTop) {
			/*Really OOM*/
			fy_fault(exception, NULL, "Old area full");
		}
	}
	context->old[pos] = handle;
	memcpy(context->old + pos + 1, object->data, size * sizeof(fy_uint));
	context->posInOld = pos + size + 1;
	object->position = old;
	object->data = context->old + pos + 1;
}
static void moveToYoung(fy_context *context, fy_class *clazz, fy_uint handle,
		fy_object *object, fy_int size, fy_int youngId, fy_exception *exception) {
	fy_int pos = context->posInYong;
	if (pos + size >= COPY_ENTRIES) {
		/*move to old*/
		moveToOld(context, clazz, handle, object, size, exception);
	} else {
		/*move to young*/
		memcpy(context->young + youngId * COPY_ENTRIES + pos, object->data,
				size * sizeof(fy_uint));
		context->posInYong = pos + size;
		object->position = young;
		object->data = context->young + youngId * COPY_ENTRIES + pos;
		object->gen++;
	}
}

static void move(fy_context *context, fy_class *clazz, fy_uint handle,
		fy_object *object, fy_int youngId, fy_exception *exception) {
	fy_int size;

	size = getSizeFromObject(context, object);

	if (object->gen > MAX_GEN) {
		moveToOld(context, clazz, handle, object, size, exception);
	} else {
		moveToYoung(context, clazz, handle, object, size, youngId, exception);
	}
}

static void release(fy_context *context, fy_uint handle) {
	fy_object *object = fy_heapGetObject(context,handle);
	if (object->position == old) {
		context->oldReleasedSize += getSizeFromObject(context, object) + 1;
	}
	memset(object, 0, sizeof(fy_object));
}

void fy_heapGC(fy_context *context, fy_exception *exception) {
	/*Init scan for all handles*/
	fy_uint *marks;
	fy_object *object;
	fy_class *clazz;
	fy_arrayList from;
	fy_uint handle;
	fy_int i, j;
	fy_int youngId = context->youngId;
	fy_long timeStamp;
#ifdef _DEBUG
	printf("#FISCE GC BEFORE %d+%d+%d total %dbytes, %d managed native bytes\n",
			context->posInEden, context->posInYong, context->posInOld,
			(fy_int) ((context->posInEden + context->posInYong
					+ context->posInOld) * sizeof(fy_uint)),
			context->memblocks->size);
#else
	printf(
			"#FISCE GC BEFORE %d+%d+%d total %dbytes\n",
			context->posInEden,
			context->posInYong,
			context->posInOld,
			(fy_int) ((context->posInEden + context->posInYong
							+ context->posInOld) * sizeof(fy_uint)));
#endif

	printf("#FISCE GC BEFORE %d+%d+%d total %dbytes\n", context->posInEden,
			context->posInYong, context->posInOld,
			(fy_int) ((context->posInEden + context->posInYong
					+ context->posInOld) * sizeof(fy_uint)));

	timeStamp = fy_portTimeMillSec(context->port);
	marks = /*TEMP*/fy_allocate(fy_bitSizeToInt(MAX_OBJECTS) << fy_bitSHIFT,
			exception);
	FYEH();

	fillInitialHandles(context, marks, exception);
	if (exception->exceptionType != exception_none) {
		fy_free(marks);
		return;
	}

	fy_arrayListInit(context->memblocks, &from, sizeof(fy_uint), 128,
			exception);
	if (exception->exceptionType != exception_none) {
		fy_free(marks);
		return;
	}

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

	scanRef(context, &from, marks, exception);
	if (exception->exceptionType != exception_none) {
		fy_free(marks);
		fy_arrayListDestroy(context->memblocks, &from);
		return;
	}

	fy_arrayListClear(context->memblocks, &from);
	for (i = 1; i < MAX_OBJECTS; i++) {
		object = fy_heapGetObject(context,i);
		clazz = object->clazz;
		if (clazz != NULL && !fy_bitGet(marks, i) && clazz->needFinalize
				&& object->finalizeStatus == not_finalized) {
//			printf("ADD %d need finalize\n", i);
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
			object->finalizeStatus = in_finalize_array;
		}
	}

	scanRef(context, &from, marks, exception);
	if (exception->exceptionType != exception_none) {
		fy_free(marks);
		fy_arrayListDestroy(context->memblocks, &from);
		return;
	}

	context->posInEden = 0;
	context->posInYong = 0;
	context->youngId = youngId = 1 - context->youngId;
	for (i = 1; i < MAX_OBJECTS; i++) {
		object = fy_heapGetObject(context,i);
		clazz = object->clazz;
		if (clazz != NULL) {
			if (fy_bitGet(marks, i)) {
				switch (object->position) {
				case eden:
				case young:
					move(context, clazz, i, object, youngId, exception);
					break;
				case old:
					break;
				default: {
					fy_fault(exception, NULL, "Illegal position %d in heap",
							object->position);
					return;
				}
				}
			} else {
				release(context, i);
			}
		}
	}
#ifndef FY_GC_FORCE_FULL
	if ((context->posInOld - context->oldReleasedSize) * 4
			< context->posInOld) {
#endif
		compactOld(context, exception);
		FYEH();
#ifndef FY_GC_FORCE_FULL
	}
#endif
	printf("#FISCE GC AFTER %d+%d+%d total %dbytes time=%"FY_PRINT64"d\n",
			context->posInEden, context->posInYong, context->posInOld,
			(fy_int) ((context->posInEden + context->posInYong
					+ context->posInOld) * sizeof(fy_uint)),
			fy_portTimeMillSec(context->port) - timeStamp);

	fy_free(marks);
}

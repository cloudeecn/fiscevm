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

void fy_heapRelease(fy_VMContext *context, juint handle) {
	fy_object *objects = context->objects;
	ASSERT(handle > 0 && handle <= MAX_OBJECTS);
	if (objects[handle].clazz != NULL) {
		vm_die("Illegal handle");
	}
	switch (objects[handle].sizeShift) {
	case fy_SIZE_SHIFT_BYTE:
		vm_free(objects[handle].data.bdata);
		break;
	case fy_SIZE_SHIFT_LONG:
		vm_free(objects[handle].data.ldata);
		break;
	default:
		vm_free(objects[handle].data.idata);
		break;
	}
}

static int allocate(fy_VMContext *context, int size, fy_exception *exception) {
	int handle = context->nextHandle;
	while (1) {
		if (context->objects[handle].clazz == NULL) {
			break;
		}
		handle++;
		if (handle >= MAX_OBJECTS) {
			handle = 1;
		}
		if (handle == context->nextHandle) {
			/*TODO OOM*/
			vm_die("Out of memory! Handle overflow");
			exception->exceptionType = exception_normal;
			strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
					"java/lang/OutOfMemoryError");
			strcpy_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
					"Handle overflow");
			return 0;
		}
	}
	return handle;
}

int fy_heapAllocate(fy_VMContext *context, fy_class *clazz,
		fy_exception *exception) {
	int length = clazz->sizeAbs;
	int size = length << fy_SIZE_SHIFT_INT;
	jint *idata;
	int handle;

	if (clazz->type != obj) {
		vm_die("Cannot instance Array without size");
	}

	handle = allocate(context, size, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	idata = vm_allocate(size);
	if (idata == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/OutOfMemoryError");
		strcpy_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"Memory overflow");
		return 0;
	}
	context->objects[handle].length = length;
	context->objects[handle].clazz = clazz;
	context->objects[handle].sizeShift = fy_SIZE_SHIFT_INT;
	context->objects[handle].data.idata = idata;

	return handle;
}

int fy_heapAllocateArray(fy_VMContext *context, fy_class *clazz, int length,
		fy_exception *exception) {
	int sizeShift = clazz->ci.arr.sizeShift;
	int size = length << sizeShift;
	void *data;
	int handle;

	if (clazz->type != arr) {
		vm_die("Cannot instance Array without size");
	}

	handle = allocate(context, size, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	data = vm_allocate(size);
	if (data == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/OutOfMemoryError");
		strcpy_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"Memory overflow");
		return 0;
	}
	context->objects[handle].length = length;
	context->objects[handle].clazz = clazz;
	context->objects[handle].sizeShift = sizeShift;
	switch (sizeShift) {
	case fy_SIZE_SHIFT_BYTE:
		context->objects[handle].data.bdata = (jbyte*) data;
		break;
	case fy_SIZE_SHIFT_LONG:
		context->objects[handle].data.ldata = (jlong*) data;
		break;
	default:
		context->objects[handle].data.idata = (jint*) data;
		break;
	}
	return handle;
}

fy_class* fy_heapGetClassOfObject(fy_VMContext *context, jint handle) {
	return context->objects[handle].clazz;
}

fy_str* fy_heapGetString(fy_VMContext *context, jint handle, fy_str *target,
		fy_exception *exception) {
	int i;
	fy_field *valueField, *offsetField, *countField;
	jint ofs, len;
	jint cah;
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
		fy_strAppendChar(context, target,
				fy_heapGetArrayChar(context, cah, ofs + i, exception));
	}

	return target;
}

jint fy_heapMakeString(fy_VMContext *context, fy_str *target,
		fy_exception *exception) {
	int i;
	fy_field *valueField, *offsetField, *countField;
	jint len;
	jint cah;
	jint ret;
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

jint fy_heapLiteral(fy_VMContext *context, fy_str *str, fy_exception *exception) {
	jint *pInt;

	pInt = fy_hashMapGet(context, context->literals, str);
	if (pInt == NULL) {
		pInt = fy_vmAllocate(context, sizeof(jint));
		*pInt = fy_heapMakeString(context, str, exception);
		if (exception->exceptionType != exception_none) {
			fy_vmFree(context, pInt);
			return 0;
		}
		fy_hashMapPut(context, context->literals, str, pInt);
	}
	return *pInt;
}

void fy_heapArrayCopy(fy_VMContext *context, jint src, jint srcPos, jint dest,
		jint destPos, jint len, fy_exception *exception) {
	fy_object *sObject, *dObject;
	if (src == 0 || dest == 0) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/ArrayStoreException");
	}
	sObject = context->objects + src;
	dObject = context->objects + dest;
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
	case fy_SIZE_SHIFT_BYTE:
		memcpy(dObject->data.bdata + destPos, sObject->data.bdata + srcPos,
				len);
		break;
	case fy_SIZE_SHIFT_INT:
		memcpy(dObject->data.idata + destPos, sObject->data.idata + srcPos,
				len << fy_SIZE_SHIFT_INT);
		break;
	case fy_SIZE_SHIFT_LONG:
		memcpy(dObject->data.ldata + destPos, sObject->data.ldata + srcPos,
				len << fy_SIZE_SHIFT_LONG);
		break;
	}
}

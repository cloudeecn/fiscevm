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
	ASSERT(handle > 0 && handle <= MAX_OBJECTS);
	if (fy_heapGetObject(context, handle)->clazz != NULL) {
		vm_die("Illegal handle");
	}
	switch (fy_heapGetObject(context, handle)->sizeShift) {
	case fy_SIZE_SHIFT_BYTE:
		vm_free(fy_heapGetObject(context, handle)->data.bdata);
		break;
	case fy_SIZE_SHIFT_LONG:
		vm_free(fy_heapGetObject(context, handle)->data.ldata);
		break;
	default:
		vm_free(fy_heapGetObject(context, handle)->data.idata);
		break;
	}
}

static int allocate(fy_VMContext *context, int size, fy_exception *exception) {
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
	fy_heapGetObject(context, handle)->length = length;
	fy_heapGetObject(context, handle)->clazz = clazz;
	fy_heapGetObject(context, handle)->sizeShift = fy_SIZE_SHIFT_INT;
	fy_heapGetObject(context, handle)->data.idata = idata;

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
	fy_heapGetObject(context, handle)->length = length;
	fy_heapGetObject(context, handle)->clazz = clazz;
	fy_heapGetObject(context, handle)->sizeShift = sizeShift;
	switch (sizeShift) {
	case fy_SIZE_SHIFT_BYTE:
		fy_heapGetObject(context, handle)->data.bdata = (jbyte*) data;
		break;
	case fy_SIZE_SHIFT_LONG:
		fy_heapGetObject(context, handle)->data.ldata = (jlong*) data;
		break;
	default:
		fy_heapGetObject(context, handle)->data.idata = (jint*) data;
		break;
	}
	return handle;
}

fy_class* fy_heapGetClassOfObject(fy_VMContext *context, jint handle) {
	return fy_heapGetObject(context, handle)->clazz;
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

#define CHECK_NPT(X) if (handle == 0) { \
exception->exceptionType = exception_normal; \
strcpy_s(exception->exceptionName,sizeof(exception->exceptionName), "java/lang/NullPointerException"); \
exception->exceptionDesc[0] = 0; \
return X; \
}

#define CHECK_IOOB(X) if (index < 0 || index >= obj->length) {\
exception->exceptionType = exception_normal;\
strcpy_s(exception->exceptionName,sizeof(exception->exceptionName), "java/lang/IndexOutOfBoundException");\
sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),\
		"%I32d / %I32d", index, obj->length);\
return X;\
}

#define CHECK_STATIC(X) if((field->access_flags & fy_ACC_STATIC)==0){\
		exception->exceptionType=exception_normal;\
		strcpy_s(exception->exceptionName,sizeof(exception->exceptionName),"java/lang/IncompatibleClassChangeError");\
		strcpy_s(exception->exceptionDesc,sizeof(exception->exceptionDesc),"get/set static field is not static!");\
		return X;\
	}

#ifdef _DEBUG
static jboolean validate(fy_VMContext *context, jint handle, fy_field *field) {
	fy_class *handleClass = fy_heapGetObject(context, handle)->clazz;
	fy_class *fieldClass = field->owner;
	return fy_classCanCastTo(context, handleClass, fieldClass);
}
#endif

jint fy_heapArrayLength(fy_VMContext *context, jint handle,
		fy_exception *exception) {
	return fy_heapGetObject(context, handle)->length;
}

jboolean fy_heapGetArrayBoolean(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {

	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return obj->data.bdata[index];

}
jint fy_heapGetArrayHandle(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return (jint) obj->data.idata[index];
}
jbyte fy_heapGetArrayByte(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return obj->data.bdata[index];
}

jbyte *fy_heapGetArrayBytes(fy_VMContext *context, jint handle,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);CHECK_NPT(0)
	return obj->data.bdata;
}

jshort fy_heapGetArrayShort(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return (jshort) obj->data.idata[index];
}
jchar fy_heapGetArrayChar(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return (jchar) obj->data.idata[index];
}
jint fy_heapGetArrayInt(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return obj->data.idata[index];
}
jlong fy_heapGetArrayLong(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return obj->data.ldata[index];
}
jfloat fy_heapGetArrayFloat(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return *((jfloat*) (obj->data.idata + index));
}
jdouble fy_heapGetArrayDouble(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return *((jdouble*) (obj->data.ldata + index));
}

void fy_heapPutArrayBoolean(fy_VMContext *context, jint handle, jint index,
		jboolean value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.bdata[index] = value;
}
void fy_heapPutArrayHandle(fy_VMContext *context, jint handle, jint index,
		jint value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = value;
}
void fy_heapPutArrayByte(fy_VMContext *context, jint handle, jint index,
		jbyte value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.bdata[index] = value;
}
void fy_heapPutArrayShort(fy_VMContext *context, jint handle, jint index,
		jshort value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = value;
}
void fy_heapPutArrayChar(fy_VMContext *context, jint handle, jint index,
		jchar value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = value;
}
void fy_heapPutArrayInt(fy_VMContext *context, jint handle, jint index,
		jint value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = value;
}
void fy_heapPutArrayLong(fy_VMContext *context, jint handle, jint index,
		jlong value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.ldata[index] = value;
}
void fy_heapPutArrayFloat(fy_VMContext *context, jint handle, jint index,
		jfloat value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = fy_floatToInt(value);
}
void fy_heapPutArrayDouble(fy_VMContext *context, jint handle, jint index,
		jdouble value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.ldata[index] = fy_doubleToLong(value);
}

jboolean fy_heapGetFieldBoolean(fy_VMContext *context, jint handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (jboolean) obj->data.idata[field->posAbs];
}
jint fy_heapGetFieldHandle(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (jint) obj->data.idata[field->posAbs];
}
jbyte fy_heapGetFieldByte(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (jbyte) obj->data.idata[field->posAbs];
}
jshort fy_heapGetFieldShort(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (jshort) obj->data.idata[field->posAbs];
}
jchar fy_heapGetFieldChar(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (jchar) obj->data.idata[field->posAbs];
}
jint fy_heapGetFieldInt(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (jint) obj->data.idata[field->posAbs];
}
jlong fy_heapGetFieldLong(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return fy_I2TOL(obj->data.idata[field->posAbs],obj->data.idata[field->posAbs+1]);
}
jfloat fy_heapGetFieldFloat(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return *((jfloat*) ((obj->data.idata) + (field->posAbs)));
}
jdouble fy_heapGetFieldDouble(fy_VMContext *context, jint handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return fy_longToDouble(
			fy_I2TOL(obj->data.idata[field->posAbs],obj->data.idata[field->posAbs+1]));
}

void fy_heapPutFieldBoolean(fy_VMContext *context, jint handle, fy_field *field,
		jboolean value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldHandle(fy_VMContext *context, jint handle, fy_field *field,
		jint value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldByte(fy_VMContext *context, jint handle, fy_field *field,
		jbyte value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldShort(fy_VMContext *context, jint handle, fy_field *field,
		jshort value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldChar(fy_VMContext *context, jint handle, fy_field *field,
		jchar value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldInt(fy_VMContext *context, jint handle, fy_field *field,
		jint value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldLong(fy_VMContext *context, jint handle, fy_field *field,
		jlong value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = (jint) (value >> 32);
	obj->data.idata[field->posAbs + 1] = (jint) value;
}
void fy_heapPutFieldFloat(fy_VMContext *context, jint handle, fy_field *field,
		jfloat value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = fy_floatToInt(value);
}
void fy_heapPutFieldDouble(fy_VMContext *context, jint handle, fy_field *field,
		jdouble value, fy_exception *exception) {
	fy_object *obj = fy_heapGetObject(context, handle);
	jlong longValue;
	ASSERT(obj->clazz!=NULL);ASSERT(validate(context,handle,field));

	CHECK_NPT()
	longValue = fy_doubleToLong(value);
	obj->data.idata[field->posAbs] = (jint) (longValue >> 32);
	obj->data.idata[field->posAbs + 1] = (jint) longValue;
}

jboolean fy_heapGetStaticBoolean(fy_VMContext *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return field->owner->staticArea[field->posAbs];
}
jint fy_heapGetStaticHandle(fy_VMContext *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return field->owner->staticArea[field->posAbs];
}
jbyte fy_heapGetStaticByte(fy_VMContext *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return field->owner->staticArea[field->posAbs];
}
jshort fy_heapGetStaticShort(fy_VMContext *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return field->owner->staticArea[field->posAbs];
}
jchar fy_heapGetStaticChar(fy_VMContext *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return field->owner->staticArea[field->posAbs];
}
jint fy_heapGetStaticInt(fy_VMContext *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return field->owner->staticArea[field->posAbs];
}
jlong fy_heapGetStaticLong(fy_VMContext *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return ((jlong) field->owner->staticArea[field->posAbs] << 32)
			| ((jlong) field->owner->staticArea[field->posAbs + 1]);
}
jfloat fy_heapGetStaticFloat(fy_VMContext *context, fy_field *field,
		fy_exception *exception) {
	CHECK_STATIC(0)
	return *(float*) (field->owner->staticArea + field->posAbs);
}
jdouble fy_heapGetStaticDouble(fy_VMContext *context, fy_field *field,
		fy_exception *exception) {
	jlong lvalue;
	CHECK_STATIC(0)
	lvalue = ((jlong) field->owner->staticArea[field->posAbs] << 32)
			| ((jlong) field->owner->staticArea[field->posAbs + 1]);
	return fy_longToDouble(lvalue);
}

void fy_heapPutStaticBoolean(fy_VMContext *context, fy_field *field,
		jboolean value, fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = value;
}
void fy_heapPutStaticHandle(fy_VMContext *context, fy_field *field, jint value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = value;
}
void fy_heapPutStaticByte(fy_VMContext *context, fy_field *field, jbyte value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = value;
}
void fy_heapPutStaticShort(fy_VMContext *context, fy_field *field, jshort value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = value;
}
void fy_heapPutStaticChar(fy_VMContext *context, fy_field *field, jchar value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = value;
}
void fy_heapPutStaticInt(fy_VMContext *context, fy_field *field, jint value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = value;
}
void fy_heapPutStaticLong(fy_VMContext *context, fy_field *field, jlong value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = (jint) (value >> 32);
	field->owner->staticArea[field->posAbs + 1] = (jint) value;
}
void fy_heapPutStaticFloat(fy_VMContext *context, fy_field *field, jfloat value,
		fy_exception *exception) {
	CHECK_STATIC()
	field->owner->staticArea[field->posAbs] = fy_floatToInt(value);
}
void fy_heapPutStaticDouble(fy_VMContext *context, fy_field *field,
		jdouble value, fy_exception *exception) {
	jlong lvalue;
	CHECK_STATIC()
	lvalue = fy_doubleToLong(value);
	field->owner->staticArea[field->posAbs] = (jint) (lvalue >> 32);
	field->owner->staticArea[field->posAbs + 1] = (jint) lvalue;
}

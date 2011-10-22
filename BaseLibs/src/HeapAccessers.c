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

#include"fyc/Heap.h"

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
	fy_class *handleClass = context->objects[handle].clazz;
	fy_class *fieldClass = field->owner;
	return fy_classCanCastTo(context, handleClass, fieldClass);
}
#endif

jint fy_heapArrayLength(fy_VMContext *context, jint handle,
		fy_exception *exception) {
	return context->objects[handle].length;
}

jboolean fy_heapGetArrayBoolean(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {

	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return obj->data.bdata[index];

}
jint fy_heapGetArrayHandle(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return (jint) obj->data.idata[index];
}
jbyte fy_heapGetArrayByte(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return obj->data.bdata[index];
}

jbyte *fy_heapGetArrayBytes(fy_VMContext *context, jint handle,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	CHECK_NPT(0)
	return obj->data.bdata;
}

jshort fy_heapGetArrayShort(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return (jshort) obj->data.idata[index];
}
jchar fy_heapGetArrayChar(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return (jchar) obj->data.idata[index];
}
jint fy_heapGetArrayInt(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return obj->data.idata[index];
}
jlong fy_heapGetArrayLong(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return obj->data.ldata[index];
}
jfloat fy_heapGetArrayFloat(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return *((jfloat*) (obj->data.idata + index));
}
jdouble fy_heapGetArrayDouble(fy_VMContext *context, jint handle, jint index,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT(0)CHECK_IOOB(0)

	return *((jdouble*) (obj->data.ldata + index));
}

void fy_heapPutArrayBoolean(fy_VMContext *context, jint handle, jint index,
		jboolean value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.bdata[index] = value;
}
void fy_heapPutArrayHandle(fy_VMContext *context, jint handle, jint index,
		jint value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = value;
}
void fy_heapPutArrayByte(fy_VMContext *context, jint handle, jint index,
		jbyte value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.bdata[index] = value;
}
void fy_heapPutArrayShort(fy_VMContext *context, jint handle, jint index,
		jshort value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = value;
}
void fy_heapPutArrayChar(fy_VMContext *context, jint handle, jint index,
		jchar value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = value;
}
void fy_heapPutArrayInt(fy_VMContext *context, jint handle, jint index,
		jint value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = value;
}
void fy_heapPutArrayLong(fy_VMContext *context, jint handle, jint index,
		jlong value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.ldata[index] = value;
}
void fy_heapPutArrayFloat(fy_VMContext *context, jint handle, jint index,
		jfloat value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.idata[index] = fy_floatToInt(value);
}
void fy_heapPutArrayDouble(fy_VMContext *context, jint handle, jint index,
		jdouble value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);

	CHECK_NPT()CHECK_IOOB()

	obj->data.ldata[index] = fy_doubleToLong(value);
}

jboolean fy_heapGetFieldBoolean(fy_VMContext *context, jint handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (jboolean) obj->data.idata[field->posAbs];
}
jint fy_heapGetFieldHandle(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (jint) obj->data.idata[field->posAbs];
}
jbyte fy_heapGetFieldByte(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (jbyte) obj->data.idata[field->posAbs];
}
jshort fy_heapGetFieldShort(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (jshort) obj->data.idata[field->posAbs];
}
jchar fy_heapGetFieldChar(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (jchar) obj->data.idata[field->posAbs];
}
jint fy_heapGetFieldInt(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return (jint) obj->data.idata[field->posAbs];
}
jlong fy_heapGetFieldLong(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return fy_I2TOL(obj->data.idata[field->posAbs],obj->data.idata[field->posAbs+1]);
}
jfloat fy_heapGetFieldFloat(fy_VMContext *context, jint handle, fy_field *field,
		fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return *((jfloat*) ((obj->data.idata) + (field->posAbs)));
}
jdouble fy_heapGetFieldDouble(fy_VMContext *context, jint handle,
		fy_field *field, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT(0)
	return fy_longToDouble(
			fy_I2TOL(obj->data.idata[field->posAbs],obj->data.idata[field->posAbs+1]));
}

void fy_heapPutFieldBoolean(fy_VMContext *context, jint handle, fy_field *field,
		jboolean value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldHandle(fy_VMContext *context, jint handle, fy_field *field,
		jint value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldByte(fy_VMContext *context, jint handle, fy_field *field,
		jbyte value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldShort(fy_VMContext *context, jint handle, fy_field *field,
		jshort value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldChar(fy_VMContext *context, jint handle, fy_field *field,
		jchar value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldInt(fy_VMContext *context, jint handle, fy_field *field,
		jint value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = value;
}
void fy_heapPutFieldLong(fy_VMContext *context, jint handle, fy_field *field,
		jlong value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = (jint) (value >> 32);
	obj->data.idata[field->posAbs + 1] = (jint) value;
}
void fy_heapPutFieldFloat(fy_VMContext *context, jint handle, fy_field *field,
		jfloat value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

	CHECK_NPT()
	obj->data.idata[field->posAbs] = fy_floatToInt(value);
}
void fy_heapPutFieldDouble(fy_VMContext *context, jint handle, fy_field *field,
		jdouble value, fy_exception *exception) {
	fy_object *obj = context->objects + handle;
	jlong longValue;
	ASSERT(obj->clazz!=NULL);
	ASSERT(validate(context,handle,field));

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

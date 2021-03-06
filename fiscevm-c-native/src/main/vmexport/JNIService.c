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

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "com_cirnoworks_libfisce_shell_FisceService.h"
#include "fisce.h"
#include "fyc/Constants.h"
#include "fyc/ClassStruct.h"
#include "fyc/Heap.h"
#include "fyc/InputStream.h"
#include "fyc/Thread.h"
#include "fyc/ThreadManager.h"
#include "fyc/ExportSupport.h"

static jclass messageClass = NULL;

static jfieldID messageTypeField;
static jfieldID threadIdField;
static jfieldID nativeUniqueNameField;
static jfieldID exceptionNameField;
static jfieldID exceptionDescField;
static jfieldID sleepTimeField;
static jclass fisceService;
static jmethodID logD, logI, logW, logE, getInputStream;
static jclass inputStream;
static jmethodID read0, read1, closeIs;

typedef struct fy_contextData {
	JNIEnv *env;
	jobject buf;
	jarray bytes;
	fy_hashMap nativeCache[1];
} fy_contextData;

#define INIT_HEADER \
MAYBE_UNUSED fisce_exception exception[1]; \
fy_context *context = (*env)->GetDirectBufferAddress(env, buf); \
fy_contextData *cdata; \
cdata = context->additionalData; \
exception->exceptionType=exception_none; \

#define GENERIC_HEADER \
	INIT_HEADER \
	cdata->env=env; \
	cdata->buf=buf;

static void fillException(JNIEnv *env, fisce_exception *exception) {
	jclass vmException;
	jmethodID vmExceptionInit;
	if (exception->exceptionName[0] == 0) {
		vmException = (*env)->FindClass(env,
				"com/cirnoworks/fisce/intf/VMCriticalException");
		(*env)->ThrowNew(env, vmException, exception->exceptionDesc);
	} else {
		vmException = (*env)->FindClass(env,
				"com/cirnoworks/fisce/intf/VMException");
		vmExceptionInit = (*env)->GetMethodID(env, vmException, "<init>",
				"(Ljava/lang/String;Ljava/lang/String;)V");
		(*env)->Throw(env,
				(*env)->NewObject(env, vmException, vmExceptionInit,
						(*env)->NewStringUTF(env, exception->exceptionName),
						(*env)->NewStringUTF(env, exception->exceptionDesc)));
	}
}

#define CHECK_JNI_EXCEPTION if((*env)->ExceptionOccurred(env)) return

static void sendLog(struct fy_context *context, jmethodID method,
		const char* msg) {
	fy_contextData *cdata = context->additionalData;
	JNIEnv *env = cdata->env;
	jstring jmsg;

	(*env)->PushLocalFrame(env, 4);
	jmsg = (*env)->NewStringUTF(env, msg);
	(*env)->CallStaticVoidMethod(env, fisceService, method, jmsg);
	(*env)->PopLocalFrame(env, NULL);
}

static void dLogStr(struct fy_context *context, const fy_str *str) {
	char msg[512];

	fy_strSPrint(msg, sizeof(msg), str);
	msg[511] = 0;
	sendLog(context, logD, msg);
}

static void dLogVar(struct fy_context *context, const char *format, ...) {
	char msg[512];
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vsnprintf(msg, sizeof(msg), format, arg_ptr);
	va_end(arg_ptr);
	msg[511] = 0;
	sendLog(context, logD, msg);
}

static void dLogVarLn(struct fy_context *context, const char *format, ...) {
	char msg[512];
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vsnprintf(msg, sizeof(msg), format, arg_ptr);
	va_end(arg_ptr);
	msg[511] = 0;
	sendLog(context, logD, msg);
	sendLog(context, logD, "\n");
}

static void iLogStr(struct fy_context *context, const fy_str *str) {
	char msg[512];

	fy_strSPrint(msg, sizeof(msg), str);
	msg[511] = 0;
	sendLog(context, logI, msg);
}

static void iLogVar(struct fy_context *context, const char *format, ...) {
	char msg[512];
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vsnprintf(msg, sizeof(msg), format, arg_ptr);
	va_end(arg_ptr);
	msg[511] = 0;
	sendLog(context, logI, msg);
}

static void iLogVarLn(struct fy_context *context, const char *format, ...) {
	char msg[512];
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vsnprintf(msg, sizeof(msg), format, arg_ptr);
	va_end(arg_ptr);
	msg[511] = 0;
	sendLog(context, logI, msg);
	sendLog(context, logI, "\n");
}

static void wLogStr(struct fy_context *context, const fy_str *str) {
	char msg[512];

	fy_strSPrint(msg, sizeof(msg), str);
	msg[511] = 0;
	sendLog(context, logW, msg);
}

static void wLogVar(struct fy_context *context, const char *format, ...) {
	char msg[512];
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vsnprintf(msg, sizeof(msg), format, arg_ptr);
	va_end(arg_ptr);
	msg[511] = 0;
	sendLog(context, logW, msg);
}

static void wLogVarLn(struct fy_context *context, const char *format, ...) {
	char msg[512];
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vsnprintf(msg, sizeof(msg), format, arg_ptr);
	va_end(arg_ptr);
	msg[511] = 0;
	sendLog(context, logW, msg);
	sendLog(context, logW, "\n");
}

static void eLogStr(struct fy_context *context, const fy_str *str) {
	char msg[512];

	fy_strSPrint(msg, sizeof(msg), str);
	msg[511] = 0;
	sendLog(context, logE, msg);
}

static void eLogVar(struct fy_context *context, const char *format, ...) {
	char msg[512];
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vsnprintf(msg, sizeof(msg), format, arg_ptr);
	va_end(arg_ptr);
	msg[511] = 0;
	sendLog(context, logE, msg);
}

static void eLogVarLn(struct fy_context *context, const char *format, ...) {
	char msg[512];
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vsnprintf(msg, sizeof(msg), format, arg_ptr);
	va_end(arg_ptr);
	msg[511] = 0;
	sendLog(context, logE, msg);
	sendLog(context, logE, "\n");
}

static fisce_int isRead(fy_context *context, fy_inputStream *is,
		fisce_exception *exception) {
	fy_contextData *cdata = context->additionalData;
	JNIEnv *env = cdata->env;
	jobject jis = is->data;
	jint ret;

	ret = (*env)->CallIntMethod(env, jis, read0);
	if ((*env)->ExceptionOccurred(env)) {
		(*env)->ExceptionDescribe(env);
		return -1;
	}
	return ret;
}

static fisce_int isReadBlock(fy_context *context, fy_inputStream *is, void *target,
		fisce_int size, fisce_exception *exception) {
	fy_contextData *cdata = context->additionalData;
	JNIEnv *env = cdata->env;
	jobject jis = is->data;
	jint ret;

	if ((*env)->GetArrayLength(env, cdata->bytes) < size) {
		(*env)->DeleteGlobalRef(env, cdata->bytes);
		cdata->bytes = (*env)->NewGlobalRef(env,
				(*env)->NewByteArray(env, size));
	}

	ret = (*env)->CallIntMethod(env, jis, read1, cdata->bytes, 0, size);
	if (target != NULL && ret >= 0)
		(*env)->GetByteArrayRegion(env, cdata->bytes, 0, ret, target);
	if ((*env)->ExceptionOccurred(env)) {
		(*env)->ExceptionDescribe(env);
		return -1;
	}
	return ret;
}

static fisce_int isSkip(fy_context *context, fy_inputStream *is, fisce_int size,
		fisce_exception *exception) {
	return isReadBlock(context, is, NULL, size, exception);
#if 0
	fy_contextData *cdata = context->additionalData;
	JNIEnv *env = cdata->env;
	jobject jis = is;
	jclass clazz = (*env)->FindClass(env, "java/io/InputStream");
	jmethodID method = (*env)->GetMethodID(env, clazz, "skip", "(J)J");
	jint ret;
	if ((*env)->ExceptionOccurred(env)) {
		(*env)->ExceptionDescribe(env);
		return -1;
	}
	ret = (fisce_int) (*env)->CallLongMethod(env, jis, method, size);
	if ((*env)->ExceptionOccurred(env)) {
		(*env)->ExceptionDescribe(env);
		return -1;
	}
	return ret;
#endif
}

static void isClose(fy_context *context, fy_inputStream *is,
		fisce_exception *exception) {
	fy_contextData *cdata = context->additionalData;
	JNIEnv *env = cdata->env;
	jobject jis = is->data;
	(*env)->CallVoidMethod(env, jis, closeIs);
	(*env)->ExceptionClear(env);
	(*env)->DeleteGlobalRef(env, jis);
}

static fy_inputStream* isOpen(fy_context *context, const char *name,
		fisce_exception *exception) {
	fy_contextData *cdata = context->additionalData;
	fy_inputStream *ret;
	jobject buf = cdata->buf;
	jobject is;
	JNIEnv *env = cdata->env;
	jstring jname;
	(*env)->PushLocalFrame(env, 4);
	jname = (*env)->NewStringUTF(env, name);
	if ((*env)->ExceptionOccurred(env)) {
		(*env)->ExceptionDescribe(env);
		(*env)->PopLocalFrame(env, NULL);
		return NULL;
	}
	is = (*env)->CallStaticObjectMethod(env, fisceService, getInputStream, buf,
			jname);
	if (is == NULL) {
		(*env)->PopLocalFrame(env, NULL);
		return NULL;
	} else {
		ret = fy_mmAllocate(context->memblocks, sizeof(fy_inputStream),
				exception);
		FYEH()NULL;
		ret->data = (*env)->NewGlobalRef(env, is);
		ret->isClose = isClose;
		ret->isRead = isRead;
		ret->isReadBlock = isReadBlock;
		ret->isSkip = isSkip;
		(*env)->PopLocalFrame(env, NULL);
		return ret;
	}
}

JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_initContext(
		JNIEnv *env, jclass self, jobject buf) {
	INIT_HEADER

	setvbuf(stdout, NULL, _IONBF, 1024);
	if (messageClass == NULL) {
		messageClass = (*env)->NewGlobalRef(env,
				(*env)->FindClass(env,
						"com/cirnoworks/fisce/intf/idata/Message"));
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}

		messageTypeField = (*env)->GetFieldID(env, messageClass, "messageType",
				"I");
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}
		threadIdField = (*env)->GetFieldID(env, messageClass, "threadId", "I");
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}

		nativeUniqueNameField = (*env)->GetFieldID(env, messageClass,
				"nativeUniqueName", "Ljava/lang/String;");
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}
		exceptionNameField = (*env)->GetFieldID(env, messageClass,
				"exceptionName", "Ljava/lang/String;");
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}
		exceptionDescField = (*env)->GetFieldID(env, messageClass,
				"exceptionDesc", "Ljava/lang/String;");
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}
		sleepTimeField = (*env)->GetFieldID(env, messageClass, "sleepTime",
				"J");
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}

		fisceService = (*env)->NewGlobalRef(env,
				(*env)->FindClass(env,
						"com/cirnoworks/libfisce/shell/FisceService"));
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}
		logD = (*env)->GetStaticMethodID(env, fisceService, "logD0",
				"(Ljava/lang/String;)V");
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}
		logI = (*env)->GetStaticMethodID(env, fisceService, "logI0",
				"(Ljava/lang/String;)V");
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}
		logW = (*env)->GetStaticMethodID(env, fisceService, "logW0",
				"(Ljava/lang/String;)V");
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}
		logE = (*env)->GetStaticMethodID(env, fisceService, "logE0",
				"(Ljava/lang/String;)V");
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}

		getInputStream =
				(*env)->GetStaticMethodID(env, fisceService, "getInputStream",
						"(Ljava/nio/ByteBuffer;Ljava/lang/String;)Ljava/io/InputStream;");
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}

		inputStream = (*env)->NewGlobalRef(env,
				(*env)->FindClass(env, "java/io/InputStream"));
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}
		read0 = (*env)->GetMethodID(env, inputStream, "read", "()I");
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}
		read1 = (*env)->GetMethodID(env, inputStream, "read", "([BII)I");
		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}
		closeIs = (*env)->GetMethodID(env, inputStream, "close", "()V");

		if ((*env)->ExceptionOccurred(env)) {
			(*env)->ExceptionDescribe(env);
			return;
		}
	}

	exception->exceptionType = exception_none;
	memset(context, 0, sizeof(fy_context));
    fy_vmContextInit(context, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return;
	}
	cdata = fy_mmAllocate(context->memblocks, sizeof(fy_contextData),
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return;
	}
	fy_hashMapInitPerm(context->memblocks, cdata->nativeCache, 203, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return;
	}
	cdata->bytes = (*env)->NewGlobalRef(env, (*env)->NewByteArray(env, 4096));
	if ((*env)->ExceptionOccurred(env)) {
		(*env)->ExceptionDescribe(env);
		return;
	}
	context->additionalData = cdata;
	context->isOpen = isOpen;
	context->logDStr = dLogStr;
	context->logDVar = dLogVar;
	context->logDVarLn = dLogVarLn;
	context->logIStr = iLogStr;
	context->logIVar = iLogVar;
	context->logIVarLn = iLogVarLn;
	context->logWStr = wLogStr;
	context->logWVar = wLogVar;
	context->logWVarLn = wLogVarLn;
	context->logEStr = eLogStr;
	context->logEVar = eLogVar;
	context->logEVarLn = eLogVarLn;
}

JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_execute(
		JNIEnv *env, jclass self, jobject buf, jobject ret, jintArray params) {
	fisce_message message[1];

#ifndef FY_LATE_DECLARATION

#endif
	jobject obj;
	int len;

	GENERIC_HEADER

	message->messageType = message_none;

	fy_tmRun(context, message, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return;
	}

	(*env)->SetIntField(env, ret, messageTypeField, message->messageType);

	switch (message->messageType) {
	case message_invoke_native: {
		obj = (*env)->NewStringUTF(env,
				message->body.call.methodName);
		CHECK_JNI_EXCEPTION;
		(*env)->SetObjectField(env, ret, nativeUniqueNameField, obj);

		len = message->body.call.paramCount;
		(*env)->SetIntArrayRegion(env, params, 0, len,
				(jint*) message->body.call.params);

		(*env)->SetIntField(env, ret, threadIdField, message->threadId);
		break;
	}
	case message_sleep: {

		(*env)->SetLongField(env, ret, sleepTimeField, message->body.sleepTime);
		break;
	}
	case message_vm_dead:
		break;
	default:
		exception->exceptionType = exception_normal;
		exception->exceptionName[0] = 0;
		sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"Illegal message type %d", message->messageType);
		fillException(env, exception);
		break;
	}
}

static void destroyNativeCache(fy_str *key, void *value, void *addition) {
	JNIEnv *env = addition;
	(*env)->DeleteGlobalRef(env, value);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    destroyContext
 * Signature: (Ljava/nio/ByteBuffer;)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_destroyContext(
		JNIEnv *env, jclass self, jobject buf) {
	GENERIC_HEADER

	fy_hashMapEachValue(context->memblocks, cdata->nativeCache,
			destroyNativeCache, env);
	(*env)->DeleteGlobalRef(env, cdata->bytes);

	fy_vmContextDestroy(context);
}

JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_bootFromClass(
		JNIEnv *env, jclass self, jobject buf, jstring name) {
	const char *cname = (*env)->GetStringUTFChars(env, name, FALSE);
	GENERIC_HEADER

    fy_vmBootup(context, cname, exception);
	(*env)->ReleaseStringUTFChars(env, name, cname);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getSize(
		JNIEnv *env, jclass self) {
	return sizeof(fy_context);
}

static void jstrToFyStr(JNIEnv *env, jstring str, fy_str *fstr, fisce_exception *exception) {
	fisce_int len = (*env)->GetStringLength(env, str);
	const jchar* chars = (*env)->GetStringChars(env, str, NULL);
	fstr->length = len;
	fstr->maxLength = len;
	fstr->status &= ~FY_STR_HASHED;
	fstr->content = fy_allocate(len * sizeof(jchar), exception);
	FYEX((*env)->ReleaseStringChars(env, str, chars); return);
	memcpy(fstr->content, chars, len * sizeof(jchar));
	(*env)->ReleaseStringChars(env, str, chars);
}

static void releaseStrJstr(JNIEnv *env, jstring str, fy_str *fstr) {
	if(fstr->content){
		fy_free(fstr->content);
	}
	fstr->content = NULL;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getClassByName
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;)I
 */JNIEXPORT
jint
JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getClassByName(
		JNIEnv *env, jclass self, jobject buf, jstring name) {
	fy_str str;
	fy_class *clazz;

	GENERIC_HEADER

	jstrToFyStr(env, name, &str, exception);
	FYEX(releaseStrJstr(env, name, &str); return 0);
	clazz = fy_vmLookupClass(context, &str, exception);
	releaseStrJstr(env, name, &str);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	}
	return clazz->classId;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    validClassId
 * Signature: (Ljava/nio/ByteBuffer;I)Z
 */JNIEXPORT
jboolean JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_validClassId(
		JNIEnv *env, jclass self, jobject buf, jint classId) {
	return classId > 0 && classId < MAX_CLASSES;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getIdFromHandle
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getIdFromHandle(
		JNIEnv *env, jclass self, jobject buf, jint handle) {
	GENERIC_HEADER
	return context->objects[handle].object_data->m.classId;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldByUniqueName
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldByUniqueName(
		JNIEnv *env, jclass self, jobject buf, jstring name) {
	fy_str str;
	fy_field *field;

	GENERIC_HEADER

	jstrToFyStr(env, name, &str, exception);
	FYEX(releaseStrJstr(env, name, &str); return 0);
	field = fy_vmGetField(context, &str);
	releaseStrJstr(env, name, &str);
	if (field == NULL) {
		return -1;
	} else {
		return field->field_id;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    lookupField
 * Signature: (Ljava/nio/ByteBuffer;ILjava/lang/String;)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_lookupField(
		JNIEnv *env, jclass self, jobject buf, jint classId, jstring name) {
	fy_str str;
	fy_field *field;

	GENERIC_HEADER

	jstrToFyStr(env, name, &str, exception);
	FYEX(releaseStrJstr(env, name, &str); return 0);
	field = fy_vmLookupFieldVirtual(context, context->classes[classId],
			&str, exception);
	releaseStrJstr(env, name, &str);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	}
	if (field == NULL) {
		return -1;
	} else {
		return field->field_id;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getMethodByUniqueName
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getMethodByUniqueName(
		JNIEnv *env, jclass self, jobject buf, jstring name) {
	fy_str str;
	fy_method *method;

	GENERIC_HEADER

	jstrToFyStr(env, name, &str, exception);
	FYEX(releaseStrJstr(env, name, &str); return 0);
	method = fy_vmGetMethod(context, &str);
	releaseStrJstr(env, name, &str);
	if (method == NULL) {
		return -1;
	} else {
		return method->method_id;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    lookupMethod
 * Signature: (Ljava/nio/ByteBuffer;ILjava/lang/String;)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_lookupMethod(
		JNIEnv *env, jclass self, jobject buf, jint classId, jstring name) {
	fy_str str;
	fy_method *method;

	GENERIC_HEADER

	jstrToFyStr(env, name, &str, exception);
	FYEX(releaseStrJstr(env, name, &str); return 0);
	method = fy_vmLookupMethodVirtual(context, context->classes[classId],
			&str, exception);
	releaseStrJstr(env, name, &str);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	}
	if (method == NULL) {
		return -1;
	} else {
		return method->method_id;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getClassName
 * Signature: (Ljava/nio/ByteBuffer;I)Ljava/lang/String;
 */JNIEXPORT
jstring JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getClassName(
		JNIEnv *env, jclass self, jobject buf, jint classId) {
	fy_class *clazz;
	fy_str *clname;
	GENERIC_HEADER

	clazz = context->classes[classId];
	clname = clazz->className;
	return (*env)->NewString(env, clname->content, clname->length);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getClassSuper
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getClassSuper(
		JNIEnv *env, jclass self, jobject buf, jint classId) {
	fy_class *clazz;
	GENERIC_HEADER

	clazz = context->classes[classId]->s.super;
	if (clazz == NULL) {
		return -1;
	} else {
		return clazz->classId;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldOwner
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldOwner(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {
	GENERIC_HEADER

	return context->fields[fieldId]->owner->classId;

}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    threadReturnHandle
 * Signature: (Ljava/nio/ByteBuffer;II)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_threadReturnHandle(
		JNIEnv *env, jclass self, jobject buf, jint threadId, jint value) {
	fy_thread *thread;
	GENERIC_HEADER
	thread = context->threads[threadId];
	fy_nativeReturnHandle(context, thread, value);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    threadReturnInt
 * Signature: (Ljava/nio/ByteBuffer;II)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_threadReturnInt(
		JNIEnv *env, jclass self, jobject buf, jint threadId, jint value) {
	fy_thread *thread;
	GENERIC_HEADER
	thread = context->threads[threadId];
	fy_nativeReturnInt(context, thread, value);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    threadReturnWide
 * Signature: (Ljava/nio/ByteBuffer;IJ)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_threadReturnWide(
		JNIEnv *env, jclass self, jobject buf, jint threadId, jlong value) {
	fy_thread *thread;
	GENERIC_HEADER
	thread = context->threads[threadId];
	fy_nativeReturnLong(context, thread, value);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getThreadHandle
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getThreadHandle(
		JNIEnv *env, jclass self, jobject buf, jint threadId) {
	fy_thread *thread;
	GENERIC_HEADER
	thread = context->threads[threadId];
	return thread->handle;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    pushVMException
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;Ljava/lang/String;)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_pushVMException(
		JNIEnv *env, jclass self, jobject buf, jint threadId, jstring name,
		jstring desc) {
	fisce_exception toPrepare;
	const char *jcName;
	const char *jcDesc;
	GENERIC_HEADER

	jcName = (*env)->GetStringUTFChars(env, name, NULL);
	jcDesc = (*env)->GetStringUTFChars(env, name, NULL);
	toPrepare.exceptionType = exception_normal;
	strcpy_s(toPrepare.exceptionName, sizeof(toPrepare.exceptionName), jcName);
	strcpy_s(toPrepare.exceptionDesc, sizeof(toPrepare.exceptionDesc), jcDesc);
	(*env)->ReleaseStringUTFChars(env, name, jcName);
	(*env)->ReleaseStringUTFChars(env, desc, jcDesc);
	fy_threadPrepareThrowable(context, context->threads[threadId], &toPrepare,
			exception);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    allocateObject
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_allocateObject(
		JNIEnv *env, jclass self, jobject buf, jint classId) {
	fy_class *clazz;
	jint ret;
	GENERIC_HEADER
	clazz = context->classes[classId];
	ret = fy_heapAllocate(context, clazz, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	}
	return ret;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    allocateArray
 * Signature: (Ljava/nio/ByteBuffer;II)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_allocateArray(
		JNIEnv *env, jclass self, jobject buf, jint classId, jint len) {
	fy_class *clazz;
	jint ret;
	GENERIC_HEADER
	clazz = context->classes[classId];
	ret = fy_heapAllocateArray(context, clazz, len, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	}
	return ret;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    clone
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_clone(JNIEnv *env,
		jclass self, jobject buf, jint handle) {
	jint ret;
	GENERIC_HEADER
	ret = fy_heapClone(context, handle, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	}
	return ret;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getLiteral
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getLiteral(
		JNIEnv *env, jclass self, jobject buf, jstring content) {
	jint ret;
	fy_str str;
	GENERIC_HEADER
	jstrToFyStr(env, content, &str, exception);
	FYEX(releaseStrJstr(env, content, &str); return 0);
	ret = fy_heapLiteral(context, &str, exception);
	releaseStrJstr(env, content, &str);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	}
	return ret;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    putString
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_putString(
		JNIEnv *env, jclass self, jobject buf, jstring content) {
	jint ret;
	fy_str str;
	GENERIC_HEADER
	jstrToFyStr(env, content, &str, exception);
	FYEX(releaseStrJstr(env, content, &str); return 0);
	ret = fy_heapMakeString(context, &str, exception);
	releaseStrJstr(env, content, &str);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	}
	return ret;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getString
 * Signature: (Ljava/nio/ByteBuffer;I)Ljava/lang/String;
 */JNIEXPORT
jstring JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getString(
		JNIEnv *env, jclass self, jobject buf, jint handle) {
	fy_str str;
	jstring ret;
	GENERIC_HEADER
	str.content = NULL;
	fy_strInit(context->memblocks, &str, 32, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return NULL;
	}
	fy_heapGetString(context, handle, &str, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		fy_strDestroy(context->memblocks, &str);
		return NULL;
	}
	ret = (*env)->NewString(env, str.content, str.length);
	fy_strDestroy(context->memblocks, &str);
	return ret;
}

JNIEXPORT
jcharArray JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStringChars(
		JNIEnv *env, jclass self, jobject buf, jint handle) {
	fy_str str;
	jcharArray ret;
	GENERIC_HEADER
	str.content = NULL;
	fy_strInit(context->memblocks, &str, 32, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return NULL;
	}
	fy_heapGetString(context, handle, &str, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		fy_strDestroy(context->memblocks, &str);
		return NULL;
	}
	ret = (*env)->NewCharArray(env, str.length);
	(*env)->SetCharArrayRegion(env, ret, 0, str.length, str.content);
	fy_strDestroy(context->memblocks, &str);
	return ret;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getClassOfHandle
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getClassOfHandle(
		JNIEnv *env, jclass self, jobject buf, jint handle) {
	GENERIC_HEADER
	return context->objects[handle].object_data->clazz->classId;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayLength
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayLength(
		JNIEnv *env, jclass self, jobject buf, jint handle) {
	GENERIC_HEADER
	return context->objects[handle].object_data->m.arrayLength;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldBoolean
 * Signature: (Ljava/nio/ByteBuffer;II)Z
 */JNIEXPORT
jboolean JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldBoolean(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {
	jboolean ret;
	GENERIC_HEADER
	ret = fy_heapGetFieldBoolean(context, handle, context->fields[fieldId],
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldByte
 * Signature: (Ljava/nio/ByteBuffer;II)B
 */JNIEXPORT
jbyte JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldByte(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jbyte ret;
	GENERIC_HEADER
	ret = fy_heapGetFieldByte(context, handle, context->fields[fieldId],
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldChar
 * Signature: (Ljava/nio/ByteBuffer;II)C
 */JNIEXPORT
jchar JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldChar(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jchar ret;
	GENERIC_HEADER
	ret = fy_heapGetFieldChar(context, handle, context->fields[fieldId],
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldShort
 * Signature: (Ljava/nio/ByteBuffer;II)S
 */JNIEXPORT
jshort JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldShort(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jshort ret;
	GENERIC_HEADER
	ret = fy_heapGetFieldShort(context, handle, context->fields[fieldId],
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldHandle
 * Signature: (Ljava/nio/ByteBuffer;II)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldHandle(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jint ret;
	GENERIC_HEADER
	ret = fy_heapGetFieldHandle(context, handle, context->fields[fieldId],
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldInt
 * Signature: (Ljava/nio/ByteBuffer;II)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldInt(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jint ret;
	GENERIC_HEADER
	ret = fy_heapGetFieldInt(context, handle, context->fields[fieldId],
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldFloat
 * Signature: (Ljava/nio/ByteBuffer;II)F
 */JNIEXPORT
jfloat JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldFloat(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jfloat ret;
	GENERIC_HEADER
	ret = fy_heapGetFieldFloat(context, handle, context->fields[fieldId],
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldLong
 * Signature: (Ljava/nio/ByteBuffer;II)J
 */JNIEXPORT
jlong JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldLong(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jlong ret;
	GENERIC_HEADER
	ret = fy_heapGetFieldLong(context, handle, context->fields[fieldId],
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldDouble
 * Signature: (Ljava/nio/ByteBuffer;II)D
 */JNIEXPORT
jdouble JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldDouble(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jdouble ret;
	GENERIC_HEADER
	ret = fy_heapGetFieldDouble(context, handle, context->fields[fieldId],
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldBoolean
 * Signature: (Ljava/nio/ByteBuffer;IIZ)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldBoolean(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jboolean value) {/* */
	GENERIC_HEADER
	fy_heapPutFieldBoolean(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldByte
 * Signature: (Ljava/nio/ByteBuffer;IIB)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldByte(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jbyte value) {/* */
	GENERIC_HEADER
	fy_heapPutFieldByte(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldChar
 * Signature: (Ljava/nio/ByteBuffer;IIC)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldChar(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jchar value) {/* */
	GENERIC_HEADER
	fy_heapPutFieldChar(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldShort
 * Signature: (Ljava/nio/ByteBuffer;IIS)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldShort(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jshort value) {/* */
	GENERIC_HEADER
	fy_heapPutFieldShort(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldHandle
 * Signature: (Ljava/nio/ByteBuffer;III)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldHandle(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jint value) {/* */
	GENERIC_HEADER
	fy_heapPutFieldHandle(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldInt
 * Signature: (Ljava/nio/ByteBuffer;III)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldInt(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jint value) {/* */
	GENERIC_HEADER
	fy_heapPutFieldInt(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldFloat
 * Signature: (Ljava/nio/ByteBuffer;IIF)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldFloat(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jfloat value) {/* */
	GENERIC_HEADER
	fy_heapPutFieldFloat(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldLong
 * Signature: (Ljava/nio/ByteBuffer;IIJ)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldLong(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jlong value) {/* */
	GENERIC_HEADER
	fy_heapPutFieldLong(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldDouble
 * Signature: (Ljava/nio/ByteBuffer;IID)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldDouble(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jdouble value) {/* */
	GENERIC_HEADER
	fy_heapPutFieldDouble(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayBoolean
 * Signature: (Ljava/nio/ByteBuffer;II)Z
 */JNIEXPORT
jboolean JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayBoolean(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jboolean ret;
	GENERIC_HEADER
	ret = fy_heapGetArrayBoolean(context, handle, index, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayByte
 * Signature: (Ljava/nio/ByteBuffer;II)B
 */JNIEXPORT
jbyte JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayByte(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jbyte ret;
	GENERIC_HEADER
	ret = fy_heapGetArrayByte(context, handle, index, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayChar
 * Signature: (Ljava/nio/ByteBuffer;II)C
 */JNIEXPORT
jchar JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayChar(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jchar ret;
	GENERIC_HEADER
	ret = fy_heapGetArrayChar(context, handle, index, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayShort
 * Signature: (Ljava/nio/ByteBuffer;II)S
 */JNIEXPORT
jshort JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayShort(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jshort ret;
	GENERIC_HEADER
	ret = fy_heapGetArrayShort(context, handle, index, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayHandle
 * Signature: (Ljava/nio/ByteBuffer;II)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayHandle(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jint ret;
	GENERIC_HEADER
	ret = fy_heapGetArrayHandle(context, handle, index, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayInt
 * Signature: (Ljava/nio/ByteBuffer;II)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayInt(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jint ret;
	GENERIC_HEADER
	ret = fy_heapGetArrayInt(context, handle, index, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayFloat
 * Signature: (Ljava/nio/ByteBuffer;II)F
 */JNIEXPORT
jfloat JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayFloat(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jfloat ret;
	GENERIC_HEADER
	ret = fy_heapGetArrayFloat(context, handle, index, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayLong
 * Signature: (Ljava/nio/ByteBuffer;II)J
 */JNIEXPORT
jlong JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayLong(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jlong ret;
	GENERIC_HEADER
	ret = fy_heapGetArrayLong(context, handle, index, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayDouble
 * Signature: (Ljava/nio/ByteBuffer;II)D
 */JNIEXPORT
jdouble JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayDouble(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jdouble ret;
	GENERIC_HEADER
	ret = fy_heapGetArrayDouble(context, handle, index, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayBoolean
 * Signature: (Ljava/nio/ByteBuffer;IIZ)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayBoolean(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jboolean value) {/* */
	GENERIC_HEADER
	fy_heapPutArrayBoolean(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayByte
 * Signature: (Ljava/nio/ByteBuffer;IIB)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayByte(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jbyte value) {/* */
	GENERIC_HEADER
	fy_heapPutArrayByte(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayChar
 * Signature: (Ljava/nio/ByteBuffer;IIC)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayChar(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jchar value) {/* */
	GENERIC_HEADER
	fy_heapPutArrayChar(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayShort
 * Signature: (Ljava/nio/ByteBuffer;IIS)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayShort(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jshort value) {/* */
	GENERIC_HEADER
	fy_heapPutArrayShort(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayHandle
 * Signature: (Ljava/nio/ByteBuffer;III)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayHandle(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jint value) {/* */
	GENERIC_HEADER
	fy_heapPutArrayHandle(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayInt
 * Signature: (Ljava/nio/ByteBuffer;III)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayInt(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jint value) {/* */
	GENERIC_HEADER
	fy_heapPutArrayInt(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayFloat
 * Signature: (Ljava/nio/ByteBuffer;IIF)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayFloat(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jfloat value) {/* */
	GENERIC_HEADER
	fy_heapPutArrayFloat(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayLong
 * Signature: (Ljava/nio/ByteBuffer;IIJ)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayLong(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jlong value) {/* */
	GENERIC_HEADER
	fy_heapPutArrayLong(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayDouble
 * Signature: (Ljava/nio/ByteBuffer;IID)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayDouble(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jdouble value) {/* */
	GENERIC_HEADER
	fy_heapPutArrayDouble(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticBoolean
 * Signature: (Ljava/nio/ByteBuffer;I)Z
 */JNIEXPORT
jboolean JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticBoolean(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jboolean ret;
	GENERIC_HEADER
	ret = fy_heapGetStaticBoolean(context, context->fields[fieldId],
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticByte
 * Signature: (Ljava/nio/ByteBuffer;I)B
 */JNIEXPORT
jbyte JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticByte(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jbyte ret;
	GENERIC_HEADER
	ret = fy_heapGetStaticByte(context, context->fields[fieldId], exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticChar
 * Signature: (Ljava/nio/ByteBuffer;I)C
 */JNIEXPORT
jchar JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticChar(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jchar ret;
	GENERIC_HEADER
	ret = fy_heapGetStaticChar(context, context->fields[fieldId], exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticShort
 * Signature: (Ljava/nio/ByteBuffer;I)S
 */JNIEXPORT
jshort JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticShort(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jshort ret;
	GENERIC_HEADER
	ret = fy_heapGetStaticShort(context, context->fields[fieldId], exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticHandle
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticHandle(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jint ret;
	GENERIC_HEADER
	ret = fy_heapGetStaticHandle(context, context->fields[fieldId],
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticInt
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticInt(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jint ret;
	GENERIC_HEADER
	ret = fy_heapGetStaticInt(context, context->fields[fieldId], exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticFloat
 * Signature: (Ljava/nio/ByteBuffer;I)F
 */JNIEXPORT
jfloat JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticFloat(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jfloat ret;
	GENERIC_HEADER
	ret = fy_heapGetStaticFloat(context, context->fields[fieldId], exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticLong
 * Signature: (Ljava/nio/ByteBuffer;I)J
 */JNIEXPORT
jlong JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticLong(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jlong ret;
	GENERIC_HEADER
	ret = fy_heapGetStaticLong(context, context->fields[fieldId], exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticDouble
 * Signature: (Ljava/nio/ByteBuffer;I)D
 */JNIEXPORT
jdouble JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticDouble(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jdouble ret;
	GENERIC_HEADER
	ret = fy_heapGetStaticDouble(context, context->fields[fieldId],
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return 0;
	} else {
		return ret;
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticBoolean
 * Signature: (Ljava/nio/ByteBuffer;IZ)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticBoolean(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jboolean value) {/* */
	GENERIC_HEADER
	fy_heapPutStaticBoolean(context, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticByte
 * Signature: (Ljava/nio/ByteBuffer;IB)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticByte(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jbyte value) {/* */
	GENERIC_HEADER
	fy_heapPutStaticByte(context, context->fields[fieldId], value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticChar
 * Signature: (Ljava/nio/ByteBuffer;IC)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticChar(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jchar value) {/* */
	GENERIC_HEADER
	fy_heapPutStaticChar(context, context->fields[fieldId], value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticShort
 * Signature: (Ljava/nio/ByteBuffer;IS)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticShort(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jshort value) {/* */
	GENERIC_HEADER
	fy_heapPutStaticShort(context, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticHandle
 * Signature: (Ljava/nio/ByteBuffer;II)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticHandle(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jint value) {/* */
	GENERIC_HEADER
	fy_heapPutStaticHandle(context, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticInt
 * Signature: (Ljava/nio/ByteBuffer;II)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticInt(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jint value) {/* */
	GENERIC_HEADER
	fy_heapPutStaticInt(context, context->fields[fieldId], value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticFloat
 * Signature: (Ljava/nio/ByteBuffer;IF)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticFloat(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jfloat value) {/* */
	GENERIC_HEADER
	fy_heapPutStaticFloat(context, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticLong
 * Signature: (Ljava/nio/ByteBuffer;IJ)V
 */JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticLong(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jlong value) {/* */
	GENERIC_HEADER
	fy_heapPutStaticLong(context, context->fields[fieldId], value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticDouble
 * Signature: (Ljava/nio/ByteBuffer;ID)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticDouble(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jdouble value) {/* */
	GENERIC_HEADER
	fy_heapPutStaticDouble(context, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayBoolean
 * Signature: (Ljava/nio/ByteBuffer;II[ZII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayBoolean(
		JNIEnv *env, jclass self, jobject buf, jint dstHandle, jint dstPos,
		jbooleanArray src, jint srcPos, jint len) {/* */
	fisce_ubyte *value;
	int i;
	jboolean *booleans;
	GENERIC_HEADER
	if (dstHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, dstHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();

	value = (fisce_ubyte*) fy_heapGetArrayBytes(context, dstHandle, exception);
	FYEH();
	booleans = /*TEMP*/fy_allocate(len * sizeof(jboolean), exception);
	FYEH();
	(*env)->GetBooleanArrayRegion(env, src, srcPos, len, booleans);
	for (i = 0; i < len; i++) {
		value[i + dstPos] = booleans[i];
	}
	fy_free(booleans);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayByte
 * Signature: (Ljava/nio/ByteBuffer;II[BII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayByte(
		JNIEnv *env, jclass self, jobject buf, jint dstHandle, jint dstPos,
		jbyteArray src, jint srcPos, jint len) {/* */
	fisce_byte *value;
	int i;
	jbyte *bytes;
	GENERIC_HEADER
	if (dstHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, dstHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();

	value = (fisce_byte*) fy_heapGetArrayBytes(context, dstHandle, exception);
	FYEH();
	bytes = /*TEMP*/fy_allocate(len * sizeof(jbyte), exception);
	FYEH();
	(*env)->GetByteArrayRegion(env, src, srcPos, len, bytes);
	for (i = 0; i < len; i++) {
		value[i + dstPos] = bytes[i];
	}
	fy_free(bytes);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayChar
 * Signature: (Ljava/nio/ByteBuffer;II[CII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayChar(
		JNIEnv *env, jclass self, jobject buf, jint dstHandle, jint dstPos,
		jcharArray src, jint srcPos, jint len) {/**/
	fisce_char *value;
	int i;
	jchar *chars;
	GENERIC_HEADER
	if (dstHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, dstHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();

	value = (fisce_char*) fy_heapGetArrayBytes(context, dstHandle, exception);
	FYEH();
	chars = /*TEMP*/fy_allocate(len * sizeof(jchar), exception);
	FYEH();
	(*env)->GetCharArrayRegion(env, src, srcPos, len, chars);
	for (i = 0; i < len; i++) {
		value[i + dstPos] = chars[i];
	}
	fy_free(chars);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayShort
 * Signature: (Ljava/nio/ByteBuffer;II[SII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayShort(
		JNIEnv *env, jclass self, jobject buf, jint dstHandle, jint dstPos,
		jshortArray src, jint srcPos, jint len) {/**/
	fisce_short *value;
	int i;
	jshort *shorts;
	GENERIC_HEADER
	if (dstHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, dstHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();

	value = (fisce_short*) fy_heapGetArrayBytes(context, dstHandle, exception);
	FYEH();
	shorts = /*TEMP*/fy_allocate(len * sizeof(jshort), exception);
	FYEH();
	(*env)->GetShortArrayRegion(env, src, srcPos, len, shorts);
	for (i = 0; i < len; i++) {
		value[i + dstPos] = shorts[i];
	}
	fy_free(shorts);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayInt
 * Signature: (Ljava/nio/ByteBuffer;II[III)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayInt(
		JNIEnv *env, jclass self, jobject buf, jint dstHandle, jint dstPos,
		jintArray src, jint srcPos, jint len) {/**/
	fisce_int *value;
	int i;
	jint *ints;
	GENERIC_HEADER
	if (dstHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, dstHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();

	value = (fisce_int*) fy_heapGetArrayBytes(context, dstHandle, exception);
	FYEH();
	ints = /*TEMP*/fy_allocate(len * sizeof(jint), exception);
	FYEH();
	(*env)->GetIntArrayRegion(env, src, srcPos, len, ints);
	for (i = 0; i < len; i++) {
		value[i + dstPos] = ints[i];
	}
	fy_free(ints);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayFloat
 * Signature: (Ljava/nio/ByteBuffer;II[FII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayFloat(
		JNIEnv *env, jclass self, jobject buf, jint dstHandle, jint dstPos,
		jfloatArray src, jint srcPos, jint len) {/**/
	fisce_float *value;
	int i;
	jfloat *floats;
	GENERIC_HEADER
	if (dstHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, dstHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();

	value = (fisce_float*) fy_heapGetArrayBytes(context, dstHandle, exception);
	FYEH();
	floats = /*TEMP*/fy_allocate(len * sizeof(jfloat), exception);
	FYEH();
	(*env)->GetFloatArrayRegion(env, src, srcPos, len, floats);
	for (i = 0; i < len; i++) {
		value[i + dstPos] = floats[i];
	}
	fy_free(floats);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayLong
 * Signature: (Ljava/nio/ByteBuffer;II[JII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayLong(
		JNIEnv *env, jclass self, jobject buf, jint dstHandle, jint dstPos,
		jlongArray src, jint srcPos, jint len) {/**/
	fisce_long *value;
	int i;
	jlong *longs;
	GENERIC_HEADER
	if (dstHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, dstHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();

	value = (fisce_long*) fy_heapGetArrayBytes(context, dstHandle, exception);
	FYEH();
	longs = /*TEMP*/fy_allocate(len * sizeof(jlong), exception);
	FYEH();
	(*env)->GetLongArrayRegion(env, src, srcPos, len, longs);
	for (i = 0; i < len; i++) {
		value[i + dstPos] = longs[i];
	}
	fy_free(longs);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayDouble
 * Signature: (Ljava/nio/ByteBuffer;II[DII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayDouble(
		JNIEnv *env, jclass self, jobject buf, jint dstHandle, jint dstPos,
		jdoubleArray src, jint srcPos, jint len) {/**/
	fisce_double *value;
	int i;
	jdouble *doubles;
	GENERIC_HEADER
	if (dstHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, dstHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();

	value = (fisce_double*) fy_heapGetArrayBytes(context, dstHandle, exception);
	FYEH();
	doubles = /*TEMP*/fy_allocate(len * sizeof(jdouble), exception);
	FYEH();
	(*env)->GetDoubleArrayRegion(env, src, srcPos, len, doubles);
	for (i = 0; i < len; i++) {
		value[i + dstPos] = doubles[i];
	}
	fy_free(doubles);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayBoolean
 * Signature: (Ljava/nio/ByteBuffer;[ZIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayBoolean(
		JNIEnv *env, jclass self, jobject buf, jbooleanArray dst, jint dstPos,
		jint srcHandle, jint srcPos, jint len) {/**/
	fisce_ubyte *value;
	int i;
	jboolean *booleans;
	GENERIC_HEADER
	if (srcHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, srcHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();
	value = (fisce_ubyte*) fy_heapGetArrayBytes(context, srcHandle, exception);
	FYEH();
	booleans = /*TEMP*/fy_allocate(len * sizeof(jboolean), exception);
	FYEH();
	for (i = 0; i < len; i++) {
		booleans[i] = value[i + srcPos];
	}
	(*env)->SetBooleanArrayRegion(env, dst, dstPos, len, booleans);
	fy_free(booleans);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayByte
 * Signature: (Ljava/nio/ByteBuffer;[BIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayByte(
		JNIEnv *env, jclass self, jobject buf, jbyteArray dst, jint dstPos,
		jint srcHandle, jint srcPos, jint len) {/**/
	fisce_byte *value;
	int i;
	jbyte *bytes;
	GENERIC_HEADER
	if (srcHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, srcHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();
	value = (fisce_byte*) fy_heapGetArrayBytes(context, srcHandle, exception);
	FYEH();
	bytes = /*TEMP*/fy_allocate(len * sizeof(jbyte), exception);
	FYEH();
	for (i = 0; i < len; i++) {
		bytes[i] = value[i + srcPos];
	}
	(*env)->SetByteArrayRegion(env, dst, dstPos, len, bytes);
	fy_free(bytes);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayByte
 * Signature: (Ljava/nio/ByteBuffer;Ljava/nio/ByteBuffer;III)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayByteBuffer(
		JNIEnv *env, jclass self, jobject buf, jobject dst, jint srcHandle,
		jint srcPos, jint len) {/**/
	fy_object *obj;
	void *value;
	void *pDst;
	GENERIC_HEADER
	if (srcHandle == 0 || dst == NULL) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, srcHandle, exception) < len || len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();
	obj = context->objects + srcHandle;
	switch (obj->object_data->clazz->ci.arr.arrayType) {
	case fy_at_long:
		len <<= 3;
		break;
	case fy_at_int:
		len <<= 2;
		break;
	case fy_at_byte:
		break;
	case fy_at_short:
		len <<= 1;
		break;
	default:
		fy_fault(NULL, NULL, "Illegal array type %d",
				obj->object_data->clazz->ci.arr.arrayType);
		FYEH();
		break;
	}
	value = fy_heapGetArrayBytes(context, srcHandle, exception);
	FYEH();
	pDst = (*env)->GetDirectBufferAddress(env, dst);
	memcpy(pDst, value, len);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayChar
 * Signature: (Ljava/nio/ByteBuffer;[CIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayChar(
		JNIEnv *env, jclass self, jobject buf, jcharArray dst, jint dstPos,
		jint srcHandle, jint srcPos, jint len) {/**/
	fisce_char *value;
	int i;
	jchar *chars;
	GENERIC_HEADER
	if (srcHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, srcHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();
	value = (fisce_char*) fy_heapGetArrayBytes(context, srcHandle, exception);
	FYEH();
	chars = /*TEMP*/fy_allocate(len * sizeof(jchar), exception);
	FYEH();
	for (i = 0; i < len; i++) {
		chars[i] = value[i + srcPos];
	}
	(*env)->SetCharArrayRegion(env, dst, dstPos, len, chars);
	fy_free(chars);

}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayShort
 * Signature: (Ljava/nio/ByteBuffer;[SIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayShort(
		JNIEnv *env, jclass self, jobject buf, jshortArray dst, jint dstPos,
		jint srcHandle, jint srcPos, jint len) {/**/
	fisce_short *value;
	int i;
	jshort *shorts;
	GENERIC_HEADER
	if (srcHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, srcHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();
	value = (fisce_short*) fy_heapGetArrayBytes(context, srcHandle, exception);
	FYEH();
	shorts = /*TEMP*/fy_allocate(len * sizeof(jshort), exception);
	FYEH();
	for (i = 0; i < len; i++) {
		shorts[i] = value[i + srcPos];
	}
	(*env)->SetShortArrayRegion(env, dst, dstPos, len, shorts);
	fy_free(shorts);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayInt
 * Signature: (Ljava/nio/ByteBuffer;[IIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayInt(
		JNIEnv *env, jclass self, jobject buf, jintArray dst, jint dstPos,
		jint srcHandle, jint srcPos, jint len) {/**/
	fisce_int *value;
	int i;
	jint *ints;
	GENERIC_HEADER
	if (srcHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, srcHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();
	value = (fisce_int*) fy_heapGetArrayBytes(context, srcHandle, exception);
	FYEH();
	ints = /*TEMP*/fy_allocate(len * sizeof(jint), exception);
	FYEH();
	for (i = 0; i < len; i++) {
		ints[i] = value[i + srcPos];
	}
	(*env)->SetIntArrayRegion(env, dst, dstPos, len, ints);
	fy_free(ints);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayFloat
 * Signature: (Ljava/nio/ByteBuffer;[FIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayFloat(
		JNIEnv *env, jclass self, jobject buf, jfloatArray dst, jint dstPos,
		jint srcHandle, jint srcPos, jint len) {/**/
	fisce_float *value;
	int i;
	jfloat *floats;
	GENERIC_HEADER
	if (srcHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, srcHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();
	value = (fisce_float*) fy_heapGetArrayBytes(context, srcHandle, exception);
	FYEH();
	floats = /*TEMP*/fy_allocate(len * sizeof(jfloat), exception);
	FYEH();
	for (i = 0; i < len; i++) {
		floats[i] = value[i + srcPos];
	}
	(*env)->SetFloatArrayRegion(env, dst, dstPos, len, floats);
	fy_free(floats);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayLong
 * Signature: (Ljava/nio/ByteBuffer;[JIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayLong(
		JNIEnv *env, jclass self, jobject buf, jlongArray dst, jint dstPos,
		jint srcHandle, jint srcPos, jint len) {/**/
	fisce_long *value;
	int i;
	jlong *longs;
	GENERIC_HEADER
	if (srcHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, srcHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();
	value = (fisce_long*) fy_heapGetArrayBytes(context, srcHandle, exception);
	FYEH();
	longs = /*TEMP*/fy_allocate(len * sizeof(jlong), exception);
	FYEH();
	for (i = 0; i < len; i++) {
		longs[i] = value[i + srcPos];
	}
	(*env)->SetLongArrayRegion(env, dst, dstPos, len, longs);
	fy_free(longs);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayDouble
 * Signature: (Ljava/nio/ByteBuffer;[DIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayDouble(
		JNIEnv *env, jclass self, jobject buf, jdoubleArray dst, jint dstPos,
		jint srcHandle, jint srcPos, jint len) {/**/
	fisce_double *value;
	int i;
	jdouble *doubles;
	GENERIC_HEADER
	if (srcHandle == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (fy_heapArrayLength(context, srcHandle, exception) < len + dstPos
			|| len < 0) {
		fy_fault(exception, FY_EXCEPTION_IOOB, "%d", len);
		return;
	}
	FYEH();
	value = (fisce_double*) fy_heapGetArrayBytes(context, srcHandle, exception);
	FYEH();
	doubles = /*TEMP*/fy_allocate(len * sizeof(jdouble), exception);
	FYEH();
	for (i = 0; i < len; i++) {
		doubles[i] = value[i + srcPos];
	}
	(*env)->SetDoubleArrayRegion(env, dst, dstPos, len, doubles);
	fy_free(doubles);
}

JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_unregisterNativeHandler(
		JNIEnv *env, jclass self, jobject buf, jstring name) {
	const char *cname = (*env)->GetStringUTFChars(env, name, FALSE);
	GENERIC_HEADER

	fy_vmUnRegisterNativeHandler(context, cname, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

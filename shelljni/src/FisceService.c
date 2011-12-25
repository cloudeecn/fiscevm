#include "com_cirnoworks_libfisce_shell_FisceService.h"
#include "../inc/fisce.h"
#include "../inc/fisceprt.h"
#include "../inc/fiscestu.h"
#include "../inc/fiscedev.h"

#define GENERIC_HEADER \
	fy_exception ex; \
	fy_exception *exception = &ex; \
	fy_context *context = (*env)->GetDirectBufferAddress(env, buf); \
	ex.exceptionType=exception_none;

static void fillException(JNIEnv *env, fy_exception *exception) {
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
		(*env)->Throw(
				env,
				(*env)->NewObject(env, vmException, vmExceptionInit,
						(*env)->NewStringUTF(env, exception->exceptionName),
						(*env)->NewStringUTF(env, exception->exceptionDesc)));
	}
}

#define CHECK_JNI_EXCEPTION if((*env)->ExceptionOccurred(env)) return

JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_execute(
		JNIEnv *env, jclass self, jobject buf, jobject ret) {
	fy_message _msg;
	fy_message *message = &_msg;
	jclass messageClass;

	jfieldID messageType;
	jfieldID threadId;
	jfieldID nativeUniqueName;
	jfieldID exceptionName;
	jfieldID exceptionDesc;
	jfieldID params;
	jfieldID sleepTime;

	jobject obj;
	int len;

	GENERIC_HEADER

	messageClass = (*env)->FindClass(env,
			"com/cirnoworks/libfisce/shell/Message");

	messageType = (*env)->GetFieldID(env, messageClass, "messageType", "I");
	threadId = (*env)->GetFieldID(env, messageClass, "threadId", "I");
	nativeUniqueName = (*env)->GetFieldID(env, messageClass, "nativeUniqueName",
			"Ljava/lang/String;");
	exceptionName = (*env)->GetFieldID(env, messageClass, "exceptionName",
			"Ljava/lang/String;");
	exceptionDesc = (*env)->GetFieldID(env, messageClass, "exceptionDesc",
			"Ljava/lang/String;");
	params = (*env)->GetFieldID(env, messageClass, "params", "[I");
	sleepTime = (*env)->GetFieldID(env, messageClass, "sleepTime", "J");
	/*
	 private int messageType;
	 private int threadId;
	 private String nativeUniqueName;
	 private String exceptionName;
	 private String exceptionDesc;
	 private long sleepTo;
	 */CHECK_JNI_EXCEPTION;
	message->messageType = message_none;
	message->body.exception.exceptionType = exception_none;
	fisceRun(context, message, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		return;
	}

	(*env)->SetIntField(env, ret, messageType, message->messageType);

	switch (message->messageType) {
	case message_invoke_native:
		obj = (*env)->NewString(env,
				message->body.call.method->uniqueName->content,
				message->body.call.method->uniqueName->length);
		CHECK_JNI_EXCEPTION;
		(*env)->SetObjectField(env, ret, nativeUniqueName, obj);
		CHECK_JNI_EXCEPTION;
		len = message->body.call.paramCount;
		obj = (*env)->NewIntArray(env, len);
		CHECK_JNI_EXCEPTION;
		(*env)->SetIntArrayRegion(env, obj, 0, len,
				(fy_int*) message->body.call.params);
		CHECK_JNI_EXCEPTION;
		(*env)->SetObjectField(env, ret, params, obj);
		CHECK_JNI_EXCEPTION;
		(*env)->SetIntField(env, ret, threadId, message->thread->threadId);
		break;
	case message_exception:
		obj = (*env)->NewStringUTF(env, message->body.exception.exceptionName);
		CHECK_JNI_EXCEPTION;
		(*env)->SetObjectField(env, ret, exceptionName, obj);
		CHECK_JNI_EXCEPTION;
		obj = (*env)->NewStringUTF(env, message->body.exception.exceptionDesc);
		CHECK_JNI_EXCEPTION;
		(*env)->SetObjectField(env, ret, exceptionDesc, obj);
		CHECK_JNI_EXCEPTION;
		(*env)->SetIntField(env, ret, threadId, message->thread->threadId);
		break;
	case message_sleep:
		(*env)->SetIntField(env, ret, sleepTime, message->body.sleepTime);
		break;
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

JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_initContext(
		JNIEnv *env, jclass self, jobject buf) {
	GENERIC_HEADER

	ex.exceptionType = exception_none;
	memset(context, 0, sizeof(fy_context));
	fisceInitContext(context, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_bootFromClass(
		JNIEnv *env, jclass self, jobject buf, jstring name) {
	const char *cname = (*env)->GetStringUTFChars(env, name, FALSE);
	GENERIC_HEADER

	fisceBootFromMain(context, cname, exception);
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

static void jstrToFyStr(JNIEnv *env, jstring str, fy_str *fstr) {
	fy_int len = (*env)->GetStringLength(env, str);
	fstr->length = len;
	fstr->maxLength = len;
	fstr->hashed = FALSE;
	fstr->content = (*env)->GetStringChars(env, str, NULL);
}

static void releaseStrJstr(JNIEnv *env, jstring str, fy_str *fstr) {
	(*env)->ReleaseStringChars(env, str, fstr->content);
	fstr->content = NULL;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getClassByName
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;)I
 */
JNIEXPORT
jint
JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getClassByName(
		JNIEnv *env, jclass self, jobject buf, jstring name) {
	fy_str str;
	fy_class *clazz;

	GENERIC_HEADER

	jstrToFyStr(env, name, &str);
	clazz = fy_nativeLookupClass(context, &str, exception);
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
 */
JNIEXPORT
jboolean JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_validClassId(
		JNIEnv *env, jclass self, jobject buf, jint classId) {
	return classId > 0 && classId < MAX_CLASSES;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldByUniqueName
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;)I
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldByUniqueName(
		JNIEnv *env, jclass self, jobject buf, jstring name) {
	fy_str str;
	fy_field *field;

	GENERIC_HEADER

	jstrToFyStr(env, name, &str);
	printf("Lookup in native ");
	fy_strPrint(&str);
	printf("\n");
	field = fy_nativeGetField(context, &str);
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_lookupField(
		JNIEnv *env, jclass self, jobject buf, jint classId, jstring name) {
	fy_str str;
	fy_field *field;

	GENERIC_HEADER

	jstrToFyStr(env, name, &str);
	field = fy_nativeLookupFieldVirtual(context, context->classes[classId],
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getMethodByUniqueName(
		JNIEnv *env, jclass self, jobject buf, jstring name) {
	fy_str str;
	fy_method *method;

	GENERIC_HEADER

	jstrToFyStr(env, name, &str);
	method = fy_nativeGetMethod(context, &str);
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_lookupMethod(
		JNIEnv *env, jclass self, jobject buf, jint classId, jstring name) {
	fy_str str;
	fy_method *method;

	GENERIC_HEADER

	jstrToFyStr(env, name, &str);
	method = fy_nativeLookupMethodVirtual(context, context->classes[classId],
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
 */
JNIEXPORT
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getClassSuper(
		JNIEnv *env, jclass self, jobject buf, jint classId) {
	fy_class *clazz;
	GENERIC_HEADER

	clazz = context->classes[classId]->super;
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldOwner(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {
	GENERIC_HEADER

	return context->fields[fieldId]->owner->classId;

}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    threadReturnHandle
 * Signature: (Ljava/nio/ByteBuffer;II)V
 */
JNIEXPORT
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
 */
JNIEXPORT
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
 */
JNIEXPORT
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
 */
JNIEXPORT
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
 */
JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_pushVMException(
		JNIEnv *env, jclass self, jobject buf, jint threadId, jstring name,
		jstring desc) {
	fy_exception toPrepare;
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
	fy_nativePrepareThrowable(context, context->threads[threadId], &toPrepare,
			exception);
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    allocateObject
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_allocateObject(
		JNIEnv *env, jclass self, jobject buf, jint classId) {
	fy_class *clazz;
	jint ret;
	GENERIC_HEADER
	clazz = context->classes[classId];
	ret = fy_nativeAllocate(context, clazz, exception);
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_allocateArray(
		JNIEnv *env, jclass self, jobject buf, jint classId, jint len) {
	fy_class *clazz;
	jint ret;
	GENERIC_HEADER
	clazz = context->classes[classId];
	ret = fy_nativeAllocateArray(context, clazz, len, exception);
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_clone(JNIEnv *env,
		jclass self, jobject buf, jint handle) {
	jint ret;
	GENERIC_HEADER
	ret = fy_nativeClone(context, handle, exception);
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getLiteral(
		JNIEnv *env, jclass self, jobject buf, jstring content) {
	jint ret;
	fy_str str;
	GENERIC_HEADER
	jstrToFyStr(env, content, &str);
	ret = fy_nativeLiteral(context, &str, exception);
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_putString(
		JNIEnv *env, jclass self, jobject buf, jstring content) {
	jint ret;
	fy_str str;
	GENERIC_HEADER
	jstrToFyStr(env, content, &str);
	ret = fy_nativeMakeString(context, &str, exception);
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
 */
JNIEXPORT
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
	fy_nativeGetString(context, handle, &str, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
		fy_strDestroy(context->memblocks, &str);
		return NULL;
	}
	ret = (*env)->NewString(env, str.content, str.length);
	fy_strDestroy(context->memblocks, &str);
	return ret;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getClassOfHandle
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getClassOfHandle(
		JNIEnv *env, jclass self, jobject buf, jint handle) {
	GENERIC_HEADER
	return context->objects[handle].clazz->classId;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayLength
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayLength(
		JNIEnv *env, jclass self, jobject buf, jint handle) {
	GENERIC_HEADER
	return context->objects[handle].length;
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldBoolean
 * Signature: (Ljava/nio/ByteBuffer;II)Z
 */
JNIEXPORT
jboolean JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldBoolean(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {
	jboolean ret;
	GENERIC_HEADER
	ret = fy_nativeGetFieldBoolean(context, handle, context->fields[fieldId],
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
 */
JNIEXPORT
jbyte JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldByte(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jbyte ret;
	GENERIC_HEADER
	ret = fy_nativeGetFieldByte(context, handle, context->fields[fieldId],
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
 */
JNIEXPORT
jchar JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldChar(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jchar ret;
	GENERIC_HEADER
	ret = fy_nativeGetFieldChar(context, handle, context->fields[fieldId],
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
 */
JNIEXPORT
jshort JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldShort(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jshort ret;
	GENERIC_HEADER
	ret = fy_nativeGetFieldShort(context, handle, context->fields[fieldId],
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldHandle(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jint ret;
	GENERIC_HEADER
	ret = fy_nativeGetFieldHandle(context, handle, context->fields[fieldId],
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldInt(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jint ret;
	GENERIC_HEADER
	ret = fy_nativeGetFieldInt(context, handle, context->fields[fieldId],
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
 */
JNIEXPORT
jfloat JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldFloat(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jfloat ret;
	GENERIC_HEADER
	ret = fy_nativeGetFieldFloat(context, handle, context->fields[fieldId],
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
 */
JNIEXPORT
jlong JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldLong(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jlong ret;
	GENERIC_HEADER
	ret = fy_nativeGetFieldLong(context, handle, context->fields[fieldId],
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
 */
JNIEXPORT
jdouble JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldDouble(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId) {/* */
	jdouble ret;
	GENERIC_HEADER
	ret = fy_nativeGetFieldDouble(context, handle, context->fields[fieldId],
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
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldBoolean(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jboolean value) {/* */
	GENERIC_HEADER
	fy_nativePutFieldBoolean(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldByte
 * Signature: (Ljava/nio/ByteBuffer;IIB)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldByte(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jbyte value) {/* */
	GENERIC_HEADER
	fy_nativePutFieldByte(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldChar
 * Signature: (Ljava/nio/ByteBuffer;IIC)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldChar(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jchar value) {/* */
	GENERIC_HEADER
	fy_nativePutFieldChar(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldShort
 * Signature: (Ljava/nio/ByteBuffer;IIS)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldShort(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jshort value) {/* */
	GENERIC_HEADER
	fy_nativePutFieldShort(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldHandle
 * Signature: (Ljava/nio/ByteBuffer;III)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldHandle(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jint value) {/* */
	GENERIC_HEADER
	fy_nativePutFieldHandle(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldInt
 * Signature: (Ljava/nio/ByteBuffer;III)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldInt(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jint value) {/* */
	GENERIC_HEADER
	fy_nativePutFieldInt(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldFloat
 * Signature: (Ljava/nio/ByteBuffer;IIF)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldFloat(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jfloat value) {/* */
	GENERIC_HEADER
	fy_nativePutFieldFloat(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldLong
 * Signature: (Ljava/nio/ByteBuffer;IIJ)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldLong(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jlong value) {/* */
	GENERIC_HEADER
	fy_nativePutFieldLong(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldDouble
 * Signature: (Ljava/nio/ByteBuffer;IID)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldDouble(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint fieldId,
		jdouble value) {/* */
	GENERIC_HEADER
	fy_nativePutFieldDouble(context, handle, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayBoolean
 * Signature: (Ljava/nio/ByteBuffer;II)Z
 */
JNIEXPORT
jboolean JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayBoolean(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jboolean ret;
	GENERIC_HEADER
	ret = fy_nativeGetArrayBoolean(context, handle, index, exception);
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
 */
JNIEXPORT
jbyte JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayByte(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jbyte ret;
	GENERIC_HEADER
	ret = fy_nativeGetArrayByte(context, handle, index, exception);
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
 */
JNIEXPORT
jchar JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayChar(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jchar ret;
	GENERIC_HEADER
	ret = fy_nativeGetArrayChar(context, handle, index, exception);
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
 */
JNIEXPORT
jshort JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayShort(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jshort ret;
	GENERIC_HEADER
	ret = fy_nativeGetArrayShort(context, handle, index, exception);
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayHandle(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jint ret;
	GENERIC_HEADER
	ret = fy_nativeGetArrayHandle(context, handle, index, exception);
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayInt(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jint ret;
	GENERIC_HEADER
	ret = fy_nativeGetArrayInt(context, handle, index, exception);
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
 */
JNIEXPORT
jfloat JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayFloat(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jfloat ret;
	GENERIC_HEADER
	ret = fy_nativeGetArrayFloat(context, handle, index, exception);
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
 */
JNIEXPORT
jlong JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayLong(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jlong ret;
	GENERIC_HEADER
	ret = fy_nativeGetArrayLong(context, handle, index, exception);
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
 */
JNIEXPORT
jdouble JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayDouble(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index) {/* */
	jdouble ret;
	GENERIC_HEADER
	ret = fy_nativeGetArrayDouble(context, handle, index, exception);
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
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayBoolean(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jboolean value) {/* */
	GENERIC_HEADER
	fy_nativePutArrayBoolean(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayByte
 * Signature: (Ljava/nio/ByteBuffer;IIB)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayByte(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jbyte value) {/* */
	GENERIC_HEADER
	fy_nativePutArrayByte(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayChar
 * Signature: (Ljava/nio/ByteBuffer;IIC)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayChar(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jchar value) {/* */
	GENERIC_HEADER
	fy_nativePutArrayChar(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayShort
 * Signature: (Ljava/nio/ByteBuffer;IIS)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayShort(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jshort value) {/* */
	GENERIC_HEADER
	fy_nativePutArrayShort(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayHandle
 * Signature: (Ljava/nio/ByteBuffer;III)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayHandle(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jint value) {/* */
	GENERIC_HEADER
	fy_nativePutArrayHandle(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayInt
 * Signature: (Ljava/nio/ByteBuffer;III)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayInt(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jint value) {/* */
	GENERIC_HEADER
	fy_nativePutArrayInt(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayFloat
 * Signature: (Ljava/nio/ByteBuffer;IIF)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayFloat(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jfloat value) {/* */
	GENERIC_HEADER
	fy_nativePutArrayFloat(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayLong
 * Signature: (Ljava/nio/ByteBuffer;IIJ)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayLong(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jlong value) {/* */
	GENERIC_HEADER
	fy_nativePutArrayLong(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayDouble
 * Signature: (Ljava/nio/ByteBuffer;IID)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayDouble(
		JNIEnv *env, jclass self, jobject buf, jint handle, jint index,
		jdouble value) {/* */
	GENERIC_HEADER
	fy_nativePutArrayDouble(context, handle, index, value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticBoolean
 * Signature: (Ljava/nio/ByteBuffer;I)Z
 */
JNIEXPORT
jboolean JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticBoolean(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jboolean ret;
	GENERIC_HEADER
	ret = fy_nativeGetStaticBoolean(context, context->fields[fieldId],
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
 */
JNIEXPORT
jbyte JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticByte(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jbyte ret;
	GENERIC_HEADER
	ret = fy_nativeGetStaticByte(context, context->fields[fieldId], exception);
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
 */
JNIEXPORT
jchar JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticChar(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jchar ret;
	GENERIC_HEADER
	ret = fy_nativeGetStaticChar(context, context->fields[fieldId], exception);
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
 */
JNIEXPORT
jshort JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticShort(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jshort ret;
	GENERIC_HEADER
	ret = fy_nativeGetStaticShort(context, context->fields[fieldId], exception);
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticHandle(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jint ret;
	GENERIC_HEADER
	ret = fy_nativeGetStaticHandle(context, context->fields[fieldId],
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
 */
JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticInt(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jint ret;
	GENERIC_HEADER
	ret = fy_nativeGetStaticInt(context, context->fields[fieldId], exception);
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
 */
JNIEXPORT
jfloat JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticFloat(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jfloat ret;
	GENERIC_HEADER
	ret = fy_nativeGetStaticFloat(context, context->fields[fieldId], exception);
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
 */
JNIEXPORT
jlong JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticLong(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jlong ret;
	GENERIC_HEADER
	ret = fy_nativeGetStaticLong(context, context->fields[fieldId], exception);
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
 */
JNIEXPORT
jdouble JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticDouble(
		JNIEnv *env, jclass self, jobject buf, jint fieldId) {/* */
	jdouble ret;
	GENERIC_HEADER
	ret = fy_nativeGetStaticDouble(context, context->fields[fieldId],
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
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticBoolean(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jboolean value) {/* */
	GENERIC_HEADER
	fy_nativePutStaticBoolean(context, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticByte
 * Signature: (Ljava/nio/ByteBuffer;IB)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticByte(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jbyte value) {/* */
	GENERIC_HEADER
	fy_nativePutStaticByte(context, context->fields[fieldId], value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticChar
 * Signature: (Ljava/nio/ByteBuffer;IC)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticChar(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jchar value) {/* */
	GENERIC_HEADER
	fy_nativePutStaticChar(context, context->fields[fieldId], value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticShort
 * Signature: (Ljava/nio/ByteBuffer;IS)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticShort(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jshort value) {/* */
	GENERIC_HEADER
	fy_nativePutStaticShort(context, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticHandle
 * Signature: (Ljava/nio/ByteBuffer;II)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticHandle(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jint value) {/* */
	GENERIC_HEADER
	fy_nativePutStaticHandle(context, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticInt
 * Signature: (Ljava/nio/ByteBuffer;II)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticInt(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jint value) {/* */
	GENERIC_HEADER
	fy_nativePutStaticInt(context, context->fields[fieldId], value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticFloat
 * Signature: (Ljava/nio/ByteBuffer;IF)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticFloat(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jfloat value) {/* */
	GENERIC_HEADER
	fy_nativePutStaticFloat(context, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticLong
 * Signature: (Ljava/nio/ByteBuffer;IJ)V
 */
JNIEXPORT
void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticLong(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jlong value) {/* */
	GENERIC_HEADER
	fy_nativePutStaticLong(context, context->fields[fieldId], value, exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticDouble
 * Signature: (Ljava/nio/ByteBuffer;ID)V
 */
JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticDouble(
		JNIEnv *env, jclass self, jobject buf, jint fieldId, jdouble value) {/* */
	GENERIC_HEADER
	fy_nativePutStaticDouble(context, context->fields[fieldId], value,
			exception);
	if (exception->exceptionType != exception_none) {
		fillException(env, exception);
	}
}

#include "com_cirnoworks_libfisce_shell_FisceService.h"
#include "../inc/fisce.h"
#include "../inc/fisceprt.h"
#include "../inc/fiscestu.h"
#include "../inc/fiscedev.h"

#define GENERIC_HEADER \
	fy_exception ex; \
	fy_exception *exception = &ex; \
	fy_context *context = (*env)->GetDirectBufferAddress(env, buf); \
	jclass runtimeException = (*env)->FindClass(env, \
			"java/lang/RuntimeException"); \
	char msg[256]; \
	ex.exceptionType=exception_none;

#define FILL_EXCEPTION(env,exception) 	{ \
		sprintf_s(msg, sizeof(msg), "Fisce exception %s %s", \
		exception->exceptionName, exception->exceptionDesc); \
		(*env)->ThrowNew(env, runtimeException, msg); \
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
	fisceRun(context, message, exception);
	if (exception->exceptionType != exception_none) {
		FILL_EXCEPTION(env, exception);
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
		(*env)->ThrowNew(env, runtimeException, "Illegal message type");
		break;
	}
}

JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_initContext(
		JNIEnv *env, jclass self, jobject buf) {
	GENERIC_HEADER

	ex.exceptionType = exception_none;
	memset(context, 0, sizeof(fy_context));
	fisceInitContext(context, exception);
	if (exception->exceptionType != exception_none) {
		FILL_EXCEPTION(env, exception);
	}
}

JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_bootFromClass(
		JNIEnv *env, jclass self, jobject buf, jstring name) {
	const char *cname = (*env)->GetStringUTFChars(env, name, FALSE);
	GENERIC_HEADER

	fisceBootFromMain(context, cname, exception);
	(*env)->ReleaseStringUTFChars(env, name, cname);
	if (exception->exceptionType != exception_none) {
		FILL_EXCEPTION(env, exception);
	}
}

JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getSize(
		JNIEnv *env, jclass self) {
	return sizeof(fy_context);
}

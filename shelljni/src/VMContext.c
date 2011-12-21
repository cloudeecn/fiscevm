#include "com_cirnoworks_libfisce_shell_VMContext.h"
#include "../inc/fisce.h"
#include "../inc/fisceprt.h"
#include "../inc/fiscestu.h"
#include "../inc/fiscedev.h"
/*
 * Class:     com_cirnoworks_libfisce_shell_VMContext
 * Method:    execute0
 * Signature: ()Lcom/cirnoworks/libfisce/shell/Message;
 */
JNIEXPORT jobject JNICALL Java_com_cirnoworks_libfisce_shell_VMContext_execute0(
		JNIEnv *env, jobject self, jobject buf) {

}

/*
 * Class:     com_cirnoworks_libfisce_shell_VMContext
 * Method:    initContext
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_VMContext_initContext0(
		JNIEnv *env, jobject self, jobject buf) {
	fy_exception ex;
	fy_exception *exception = &ex;
	fy_context *context = (*env)->GetDirectBufferAddress(env, buf);
	jclass runtimeException = (*env)->FindClass(env,
			"java/lang/RuntimeException");
	char msg[256];

	ex.exceptionType = exception_none;
	memset(context, 0, sizeof(fy_context));
	fisceInitContext(context, exception);
	if (exception->exceptionType != exception_none) {
		sprintf_s(msg, sizeof(msg), "Fisce exception %s %s",
				exception->exceptionName, exception->exceptionDesc);
		(*env)->ThrowNew(env, runtimeException, msg);
	}
}

/*
 * Class:     com_cirnoworks_libfisce_shell_VMContext
 * Method:    bootFromClass0
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_VMContext_bootFromClass0(
		JNIEnv *env, jobject self, jobject buf, jstring name) {

}

/*
 * Class:     com_cirnoworks_libfisce_shell_VMContext
 * Method:    getSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_VMContext_getSize(
		JNIEnv *env, jobject self) {
	return sizeof(fy_context);
}

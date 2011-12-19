#include "com_cirnoworks_libfisce_shell_VMContext.h"
#include "../inc/fisce.h"
#include "../inc/fiscestu.h"
#include "../inc/fiscedev.h"
/*
 * Class:     com_cirnoworks_libfisce_shell_VMContext
 * Method:    execute0
 * Signature: ()Lcom/cirnoworks/libfisce/shell/Message;
 */
JNIEXPORT jobject JNICALL Java_com_cirnoworks_libfisce_shell_VMContext_execute0(
		JNIEnv *env, jobject self) {

}

/*
 * Class:     com_cirnoworks_libfisce_shell_VMContext
 * Method:    initContext
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_VMContext_initContext
(JNIEnv *env, jobject self) {

}

/*
 * Class:     com_cirnoworks_libfisce_shell_VMContext
 * Method:    bootFromClass0
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_VMContext_bootFromClass0
(JNIEnv *env, jobject self, jstring name) {

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

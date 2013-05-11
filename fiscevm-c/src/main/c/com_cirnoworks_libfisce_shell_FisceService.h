/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of libfisce.
 *
 * libfisce is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * libfisce is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libfisce.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef __ANDROID_API__
#include "jni_andr.h"
#else
#include "jni.h"
#endif
/* Header for class com_cirnoworks_libfisce_shell_FisceService */

#ifndef _Included_com_cirnoworks_libfisce_shell_FisceService
#define _Included_com_cirnoworks_libfisce_shell_FisceService
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    execute
 * Signature: (Ljava/nio/ByteBuffer;Lcom/cirnoworks/libfisce/shell/Message;)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_execute(
		JNIEnv *, jclass, jobject, jobject, jintArray);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    initContext
 * Signature: (Ljava/nio/ByteBuffer;)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_initContext(
		JNIEnv *, jclass, jobject);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    bootFromClass
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_bootFromClass(
		JNIEnv *, jclass, jobject, jstring);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getSize
 * Signature: ()I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getSize(
		JNIEnv *, jclass);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getClassByName
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getClassByName(
		JNIEnv *, jclass, jobject, jstring);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    validClassId
 * Signature: (Ljava/nio/ByteBuffer;I)Z
 */JNIEXPORT jboolean JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_validClassId(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getIdFromHandle
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT
jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getIdFromHandle(
		JNIEnv *env, jclass self, jobject buf, jint handle);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldByUniqueName
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldByUniqueName(
		JNIEnv *, jclass, jobject, jstring);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    lookupField
 * Signature: (Ljava/nio/ByteBuffer;ILjava/lang/String;)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_lookupField(
		JNIEnv *, jclass, jobject, jint, jstring);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getMethodByUniqueName
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getMethodByUniqueName(
		JNIEnv *, jclass, jobject, jstring);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    lookupMethod
 * Signature: (Ljava/nio/ByteBuffer;ILjava/lang/String;)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_lookupMethod(
		JNIEnv *, jclass, jobject, jint, jstring);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getClassName
 * Signature: (Ljava/nio/ByteBuffer;I)Ljava/lang/String;
 */JNIEXPORT jstring JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getClassName(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getClassSuper
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getClassSuper(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldOwner
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldOwner(
		JNIEnv *, jclass, jobject, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    threadReturnHandle
 * Signature: (Ljava/nio/ByteBuffer;II)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_threadReturnHandle(
		JNIEnv *, jclass, jobject, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    threadReturnInt
 * Signature: (Ljava/nio/ByteBuffer;II)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_threadReturnInt(
		JNIEnv *, jclass, jobject, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    threadReturnWide
 * Signature: (Ljava/nio/ByteBuffer;IJ)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_threadReturnWide(
		JNIEnv *, jclass, jobject, jint, jlong);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getThreadHandle
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getThreadHandle(
		JNIEnv *, jclass, jobject, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    pushVMException
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;Ljava/lang/String;)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_pushVMException(
		JNIEnv *, jclass, jobject, jint, jstring, jstring);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    allocateObject
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_allocateObject(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    allocateArray
 * Signature: (Ljava/nio/ByteBuffer;II)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_allocateArray(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    clone
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_clone(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getLiteral
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getLiteral(
		JNIEnv *, jclass, jobject, jstring);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    putString
 * Signature: (Ljava/nio/ByteBuffer;Ljava/lang/String;)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_putString(
		JNIEnv *, jclass, jobject, jstring);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getString
 * Signature: (Ljava/nio/ByteBuffer;I)Ljava/lang/String;
 */JNIEXPORT jstring JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getString(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getClassOfHandle
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getClassOfHandle(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayLength
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayLength(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldBoolean
 * Signature: (Ljava/nio/ByteBuffer;II)Z
 */JNIEXPORT jboolean JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldBoolean(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldByte
 * Signature: (Ljava/nio/ByteBuffer;II)B
 */JNIEXPORT jbyte JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldByte(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldChar
 * Signature: (Ljava/nio/ByteBuffer;II)C
 */JNIEXPORT jchar JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldChar(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldShort
 * Signature: (Ljava/nio/ByteBuffer;II)S
 */JNIEXPORT jshort JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldShort(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldHandle
 * Signature: (Ljava/nio/ByteBuffer;II)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldHandle(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldInt
 * Signature: (Ljava/nio/ByteBuffer;II)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldInt(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldFloat
 * Signature: (Ljava/nio/ByteBuffer;II)F
 */JNIEXPORT jfloat JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldFloat(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldLong
 * Signature: (Ljava/nio/ByteBuffer;II)J
 */JNIEXPORT jlong JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldLong(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getFieldDouble
 * Signature: (Ljava/nio/ByteBuffer;II)D
 */JNIEXPORT jdouble JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getFieldDouble(
		JNIEnv *, jclass, jobject, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldBoolean
 * Signature: (Ljava/nio/ByteBuffer;IIZ)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldBoolean(
		JNIEnv *, jclass, jobject, jint, jint, jboolean);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldByte
 * Signature: (Ljava/nio/ByteBuffer;IIB)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldByte(
		JNIEnv *, jclass, jobject, jint, jint, jbyte);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldChar
 * Signature: (Ljava/nio/ByteBuffer;IIC)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldChar(
		JNIEnv *, jclass, jobject, jint, jint, jchar);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldShort
 * Signature: (Ljava/nio/ByteBuffer;IIS)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldShort(
		JNIEnv *, jclass, jobject, jint, jint, jshort);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldHandle
 * Signature: (Ljava/nio/ByteBuffer;III)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldHandle(
		JNIEnv *, jclass, jobject, jint, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldInt
 * Signature: (Ljava/nio/ByteBuffer;III)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldInt(
		JNIEnv *, jclass, jobject, jint, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldFloat
 * Signature: (Ljava/nio/ByteBuffer;IIF)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldFloat(
		JNIEnv *, jclass, jobject, jint, jint, jfloat);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldLong
 * Signature: (Ljava/nio/ByteBuffer;IIJ)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldLong(
		JNIEnv *, jclass, jobject, jint, jint, jlong);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setFieldDouble
 * Signature: (Ljava/nio/ByteBuffer;IID)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setFieldDouble(
		JNIEnv *, jclass, jobject, jint, jint, jdouble);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayBoolean
 * Signature: (Ljava/nio/ByteBuffer;II)Z
 */JNIEXPORT jboolean JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayBoolean(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayByte
 * Signature: (Ljava/nio/ByteBuffer;II)B
 */JNIEXPORT jbyte JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayByte(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayChar
 * Signature: (Ljava/nio/ByteBuffer;II)C
 */JNIEXPORT jchar JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayChar(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayShort
 * Signature: (Ljava/nio/ByteBuffer;II)S
 */JNIEXPORT jshort JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayShort(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayHandle
 * Signature: (Ljava/nio/ByteBuffer;II)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayHandle(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayInt
 * Signature: (Ljava/nio/ByteBuffer;II)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayInt(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayFloat
 * Signature: (Ljava/nio/ByteBuffer;II)F
 */JNIEXPORT jfloat JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayFloat(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayLong
 * Signature: (Ljava/nio/ByteBuffer;II)J
 */JNIEXPORT jlong JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayLong(
		JNIEnv *, jclass, jobject, jint, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getArrayDouble
 * Signature: (Ljava/nio/ByteBuffer;II)D
 */JNIEXPORT jdouble JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getArrayDouble(
		JNIEnv *, jclass, jobject, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayBoolean
 * Signature: (Ljava/nio/ByteBuffer;IIZ)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayBoolean(
		JNIEnv *, jclass, jobject, jint, jint, jboolean);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayByte
 * Signature: (Ljava/nio/ByteBuffer;IIB)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayByte(
		JNIEnv *, jclass, jobject, jint, jint, jbyte);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayChar
 * Signature: (Ljava/nio/ByteBuffer;IIC)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayChar(
		JNIEnv *, jclass, jobject, jint, jint, jchar);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayShort
 * Signature: (Ljava/nio/ByteBuffer;IIS)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayShort(
		JNIEnv *, jclass, jobject, jint, jint, jshort);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayHandle
 * Signature: (Ljava/nio/ByteBuffer;III)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayHandle(
		JNIEnv *, jclass, jobject, jint, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayInt
 * Signature: (Ljava/nio/ByteBuffer;III)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayInt(
		JNIEnv *, jclass, jobject, jint, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayFloat
 * Signature: (Ljava/nio/ByteBuffer;IIF)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayFloat(
		JNIEnv *, jclass, jobject, jint, jint, jfloat);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayLong
 * Signature: (Ljava/nio/ByteBuffer;IIJ)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayLong(
		JNIEnv *, jclass, jobject, jint, jint, jlong);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setArrayDouble
 * Signature: (Ljava/nio/ByteBuffer;IID)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setArrayDouble(
		JNIEnv *, jclass, jobject, jint, jint, jdouble);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticBoolean
 * Signature: (Ljava/nio/ByteBuffer;I)Z
 */JNIEXPORT jboolean JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticBoolean(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticByte
 * Signature: (Ljava/nio/ByteBuffer;I)B
 */JNIEXPORT jbyte JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticByte(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticChar
 * Signature: (Ljava/nio/ByteBuffer;I)C
 */JNIEXPORT jchar JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticChar(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticShort
 * Signature: (Ljava/nio/ByteBuffer;I)S
 */JNIEXPORT jshort JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticShort(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticHandle
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticHandle(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticInt
 * Signature: (Ljava/nio/ByteBuffer;I)I
 */JNIEXPORT jint JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticInt(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticFloat
 * Signature: (Ljava/nio/ByteBuffer;I)F
 */JNIEXPORT jfloat JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticFloat(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticLong
 * Signature: (Ljava/nio/ByteBuffer;I)J
 */JNIEXPORT jlong JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticLong(
		JNIEnv *, jclass, jobject, jint);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    getStaticDouble
 * Signature: (Ljava/nio/ByteBuffer;I)D
 */JNIEXPORT jdouble JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_getStaticDouble(
		JNIEnv *, jclass, jobject, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticBoolean
 * Signature: (Ljava/nio/ByteBuffer;IZ)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticBoolean(
		JNIEnv *, jclass, jobject, jint, jboolean);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticByte
 * Signature: (Ljava/nio/ByteBuffer;IB)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticByte(
		JNIEnv *, jclass, jobject, jint, jbyte);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticChar
 * Signature: (Ljava/nio/ByteBuffer;IC)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticChar(
		JNIEnv *, jclass, jobject, jint, jchar);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticShort
 * Signature: (Ljava/nio/ByteBuffer;IS)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticShort(
		JNIEnv *, jclass, jobject, jint, jshort);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticHandle
 * Signature: (Ljava/nio/ByteBuffer;II)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticHandle(
		JNIEnv *, jclass, jobject, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticInt
 * Signature: (Ljava/nio/ByteBuffer;II)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticInt(
		JNIEnv *, jclass, jobject, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticFloat
 * Signature: (Ljava/nio/ByteBuffer;IF)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticFloat(
		JNIEnv *, jclass, jobject, jint, jfloat);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticLong
 * Signature: (Ljava/nio/ByteBuffer;IJ)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticLong(
		JNIEnv *, jclass, jobject, jint, jlong);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    setStaticDouble
 * Signature: (Ljava/nio/ByteBuffer;ID)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_setStaticDouble(
		JNIEnv *, jclass, jobject, jint, jdouble);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayBoolean
 * Signature: (Ljava/nio/ByteBuffer;II[ZII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayBoolean(
		JNIEnv *, jclass, jobject, jint, jint, jbooleanArray, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayByte
 * Signature: (Ljava/nio/ByteBuffer;II[BII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayByte(
		JNIEnv *, jclass, jobject, jint, jint, jbyteArray, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayChar
 * Signature: (Ljava/nio/ByteBuffer;II[CII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayChar(
		JNIEnv *, jclass, jobject, jint, jint, jcharArray, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayShort
 * Signature: (Ljava/nio/ByteBuffer;II[SII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayShort(
		JNIEnv *, jclass, jobject, jint, jint, jshortArray, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayInt
 * Signature: (Ljava/nio/ByteBuffer;II[III)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayInt(
		JNIEnv *, jclass, jobject, jint, jint, jintArray, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayFloat
 * Signature: (Ljava/nio/ByteBuffer;II[FII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayFloat(
		JNIEnv *, jclass, jobject, jint, jint, jfloatArray, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayLong
 * Signature: (Ljava/nio/ByteBuffer;II[JII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayLong(
		JNIEnv *, jclass, jobject, jint, jint, jlongArray, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fillArrayDouble
 * Signature: (Ljava/nio/ByteBuffer;II[DII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fillArrayDouble(
		JNIEnv *, jclass, jobject, jint, jint, jdoubleArray, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayBoolean
 * Signature: (Ljava/nio/ByteBuffer;[ZIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayBoolean(
		JNIEnv *, jclass, jobject, jbooleanArray, jint, jint, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayByte
 * Signature: (Ljava/nio/ByteBuffer;[BIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayByte(
		JNIEnv *, jclass, jobject, jbyteArray, jint, jint, jint, jint);

JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayByteBuffer(
		JNIEnv *env, jclass self, jobject buf, jobject dst, jint srcHandle,
		jint srcPos, jint len);
/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayChar
 * Signature: (Ljava/nio/ByteBuffer;[CIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayChar(
		JNIEnv *, jclass, jobject, jcharArray, jint, jint, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayShort
 * Signature: (Ljava/nio/ByteBuffer;[SIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayShort(
		JNIEnv *, jclass, jobject, jshortArray, jint, jint, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayInt
 * Signature: (Ljava/nio/ByteBuffer;[IIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayInt(
		JNIEnv *, jclass, jobject, jintArray, jint, jint, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayFloat
 * Signature: (Ljava/nio/ByteBuffer;[FIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayFloat(
		JNIEnv *, jclass, jobject, jfloatArray, jint, jint, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayLong
 * Signature: (Ljava/nio/ByteBuffer;[JIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayLong(
		JNIEnv *, jclass, jobject, jlongArray, jint, jint, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    fetchArrayDouble
 * Signature: (Ljava/nio/ByteBuffer;[DIIII)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_fetchArrayDouble(
		JNIEnv *, jclass, jobject, jdoubleArray, jint, jint, jint, jint);

/*
 * Class:     com_cirnoworks_libfisce_shell_FisceService
 * Method:    destroyContext
 * Signature: (Ljava/nio/ByteBuffer;)V
 */JNIEXPORT void JNICALL Java_com_cirnoworks_libfisce_shell_FisceService_destroyContext(
		JNIEnv *, jclass, jobject);
#ifdef __cplusplus
}
#endif
#endif

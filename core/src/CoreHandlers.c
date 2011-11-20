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

#include "fyc/CoreHandlers.h"

static void SystemArrayCopy(struct fy_VMContext *context,
		struct fy_thread *thread, void *data, juint *args, jint argsCount,
		fy_exception *exception) {
	jint srcHandle = args[0];
	jint srcPos = args[1];
	jint dstHandle = args[2];
	jint dstPos = args[3];
	jint len = args[4];
	fy_heapArrayCopy(context, srcHandle, srcPos, dstHandle, dstPos, len,
			exception);
}

static void SystemTimeMS(struct fy_VMContext *context, struct fy_thread *thread,
		void *data, juint *args, jint argsCount, fy_exception *exception) {
	fy_nativeReturnLong(context, thread, fy_portTimeMillSec(context));
}

static void SystemTimeNS(struct fy_VMContext *context, struct fy_thread *thread,
		void *data, juint *args, jint argsCount, fy_exception *exception) {
	fy_nativeReturnLong(context, thread, fy_portTimeNanoSec(context));
}

static void ObjectGetClass(struct fy_VMContext *context,
		struct fy_thread *thread, void *data, juint *args, jint argsCount,
		fy_exception *exception) {
	fy_class *clazz = fy_heapGetClassOfObject(context, args[0]);
	fy_nativeReturnHandle(context, thread, clazz->classObjId);
}

static void ClassGetComponentType(struct fy_VMContext *context,
		struct fy_thread *thread, void *data, juint *args, jint argsCount,
		fy_exception *exception) {
	fy_class *clazz = fy_heapGetClassOfObject(context, args[0]);
	if (clazz->type == arr) {
		fy_nativeReturnHandle(context, thread,
				clazz->ci.arr.contentClass->classObjId);
	} else {
		fy_nativeReturnHandle(context, thread, 0);
	}
}

static void ThreadCurrentThread(struct fy_VMContext *context,
		struct fy_thread *thread, void *data, juint *args, jint argsCount,
		fy_exception *exception) {
	fy_nativeReturnHandle(context, thread, thread->handle);
}

static void ThreadSetPriority(struct fy_VMContext *context,
		struct fy_thread *thread, void *data, juint *args, jint argsCount,
		fy_exception *exception) {
	/*TODO implement thread manager first*/
}

static void ThreadIsAlive(struct fy_VMContext *context,
		struct fy_thread *thread, void *data, juint *args, jint argsCount,
		fy_exception *exception) {
	/*TODO implement thread manager first*/
	fy_nativeReturnInt(context, thread, 0);
}

static void ThreadStart(struct fy_VMContext *context, struct fy_thread *thread,
		void *data, juint *args, jint argsCount, fy_exception *exception) {
	/*TODO implement thread manager first*/
}

static void VMDebugOut(struct fy_VMContext *context, struct fy_thread *thread,
		void *data, juint *args, jint argsCount, fy_exception *exception) {
	fy_str *str;
	fy_vmLookupClass(context, context->sString, exception);
	if (exception->exceptionType != exception_none) {
		return;
	}
	str = fy_strAllocate(context);
	fy_heapGetString(context, args[0], str, exception);
	if (exception->exceptionType != exception_none) {
		return;
	}
	printf("VMDebugOut: ");
	fy_strPrint(str);
	printf("\n");
	fy_strRelease(context, str);
}

static void VMDebugOutI(struct fy_VMContext *context, struct fy_thread *thread,
		void *data, juint *args, jint argsCount, fy_exception *exception) {
	printf("VMDebugOutI: %d\n", args[0]);
}

static void VMDebugOutJ(struct fy_VMContext *context, struct fy_thread *thread,
		void *data, juint *args, jint argsCount, fy_exception *exception) {
	printf("VMDebugOutI: %"PRINT64"d\n", fy_I2TOL(args[0],args[1]));
}

static void VMDebugOutF(struct fy_VMContext *context, struct fy_thread *thread,
		void *data, juint *args, jint argsCount, fy_exception *exception) {
	printf("VMDebugOutI: %f\n", fy_intToFloat(args[0]));
}

static void VMDebugOutD(struct fy_VMContext *context, struct fy_thread *thread,
		void *data, juint *args, jint argsCount, fy_exception *exception) {
	printf("VMDebugOutI: %f\n", fy_longToDouble(fy_I2TOL(args[0],args[1])));
}

static void VMThrowOut(struct fy_VMContext *context, struct fy_thread *thread,
		void *data, juint *args, jint argsCount, fy_exception *exception) {
	/*TODO */
}

static void VMExit(struct fy_VMContext *context, struct fy_thread *thread,
		void *data, juint *args, jint argsCount, fy_exception *exception) {
	/*TODO */
}

static void SOSWrite(struct fy_VMContext *context, struct fy_thread *thread,
		void *data, juint *args, jint argsCount, fy_exception *exception) {
	putchar(args[1]);
}

static void VMDecode(struct fy_VMContext *context, struct fy_thread *thread,
		void *data, juint *args, jint argsCount, fy_exception *exception) {
	jint handleSrc = args[1];
	jint ofs = args[2];
	jint len = args[3];
	int i, maxi;
	jbyte *array;
	jbyte *newArray;
	fy_str *str;
	jint handleRet;
	fy_class *charArray;
	charArray = fy_vmLookupClass(context, context->sArrayChar, exception);
	if (exception->exceptionType != exception_none) {
		fy_nativeReturnHandle(context, thread, 0);
		return;
	}
	array = fy_heapGetArrayBytes(context, handleSrc, exception);
	if (exception->exceptionType != exception_none) {
		fy_nativeReturnHandle(context, thread, 0);
		return;
	}

	newArray = vm_allocate(len + 1);
	memcpy(newArray, array + ofs, len);
	newArray[len] = 0;
	str = fy_strAllocateFromUTF8(context, (char*) newArray);
	vm_free(newArray);

	handleRet = fy_heapAllocateArray(context, charArray, maxi = str->length,
			exception);
	if (exception->exceptionType != exception_none) {
		fy_nativeReturnHandle(context, thread, 0);
		fy_strDestroy(context, str);
		vm_free(str);
		return;
	}
	for (i = 0; i < maxi; i++) {
		fy_heapPutArrayChar(context, handleRet, i, str->content[i], exception);
		if (exception->exceptionType != exception_none) {
			fy_nativeReturnHandle(context, thread, 0);
			fy_strDestroy(context, str);
			vm_free(str);
			return;
		}
	}
	fy_strDestroy(context, str);
	vm_free(str);
	fy_nativeReturnHandle(context, thread, handleRet);
}

static void VMEncode(struct fy_VMContext *context, struct fy_thread *thread,
		void *data, juint *args, jint argsCount, fy_exception *exception) {
	jint handleSrc = args[1];
	jint ofs = args[2];
	jint len = args[3];
	fy_class *byteClass;
	jint handleRet;
	jbyte *out;
	jbyte *outTmp;
	jint olen;
	jint left;
	jint read;
	jint i;
	jchar ch;
	byteClass = fy_vmLookupClass(context, context->sArrayByte, exception);
	if (exception->exceptionType != exception_none) {
		fy_nativeReturnHandle(context, thread, 0);
		return;
	}
	left = len * 3 + 1;
	olen = 0;
	out = outTmp = vm_allocate(left);
	for (i = 0; i < len; i++) {
		ch = fy_heapGetArrayChar(context, handleSrc, i + ofs, exception);
		if (exception->exceptionType != exception_none) {
			fy_nativeReturnHandle(context, thread, 0);
			vm_free(out);
			return;
		}
		read = fy_utf8Write(ch, (char**) &outTmp, &left);
		if (read <= 0) {
			break;
		}
		olen += read;
	}
	handleRet = fy_heapAllocateArray(context, byteClass, olen, exception);
	if (exception->exceptionType != exception_none) {
		fy_nativeReturnHandle(context, thread, 0);
		vm_free(out);
		return;
	}

	outTmp = fy_heapGetArrayBytes(context, handleRet, exception);
	if (exception->exceptionType != exception_none) {
		fy_nativeReturnHandle(context, thread, 0);
		vm_free(out);
		return;
	}

	memcpy(outTmp, out, olen);
	vm_free(out);
	fy_nativeReturnHandle(context, thread, handleRet);
}

static void VMGetDoubleRaw(struct fy_VMContext *context,
		struct fy_thread *thread, void *data, juint *args, jint argsCount,
		fy_exception *exception) {
	fy_nativeReturnLong(context, thread,
			((jlong) args[0] << 32) | ((juint) args[1]));
}

static void VMGetFloatRaw(struct fy_VMContext *context,
		struct fy_thread *thread, void *data, juint *args, jint argsCount,
		fy_exception *exception) {
	fy_nativeReturnInt(context, thread, args[0]);
}

static void VMStringToDouble(struct fy_VMContext *context,
		struct fy_thread *thread, void *data, juint *args, jint argsCount,
		fy_exception *exception) {
	fy_str *str = fy_strAllocate(context);
	char ch[64];
	jdouble value;
	str = fy_heapGetString(context, args[0], str, exception);
	fy_strSPrint(ch, sizeof(ch), str);
	value = atof(ch);
	fy_nativeReturnLong(context, thread, fy_doubleToLong(value));
}

static void VMDoubleToString(struct fy_VMContext *context,
		struct fy_thread *thread, void *data, juint *args, jint argsCount,
		fy_exception *exception) {
	jlong lvalue = ((julong) args[0] << 32) | ((juint) args[1]);
	jdouble dvalue = fy_longToDouble(lvalue);
	jint handleRet;
	fy_str *str;
	char ch[64];
	sprintf_s(ch, sizeof(ch), "%f", dvalue);
	str = fy_strAllocateFromUTF8(context, ch);
	handleRet = fy_heapMakeString(context, str, exception);
	fy_strRelease(context, str);
	if (exception->exceptionType != exception_none) {
		return;
	}
	fy_nativeReturnHandle(context, thread, handleRet);
}

static void VMStringToFloat(struct fy_VMContext *context,
		struct fy_thread *thread, void *data, juint *args, jint argsCount,
		fy_exception *exception) {
	fy_str *str = fy_strAllocate(context);
	char ch[64];
	jdouble value;
	str = fy_heapGetString(context, args[0], str, exception);
	fy_strSPrint(ch, sizeof(ch), str);
	value = atof(ch);
	fy_nativeReturnInt(context, thread, fy_floatToInt((jfloat) value));
}

static void VMFloatToString(struct fy_VMContext *context,
		struct fy_thread *thread, void *data, juint *args, jint argsCount,
		fy_exception *exception) {
	jint value = args[0];
	jfloat fvalue = fy_intToFloat(value);
	jint handleRet;
	fy_str *str;
	char ch[64];
	sprintf_s(ch, sizeof(ch), "%f", fvalue);
	str = fy_strAllocateFromUTF8(context, ch);
	handleRet = fy_heapMakeString(context, str, exception);
	fy_strRelease(context, str);
	if (exception->exceptionType != exception_none) {
		return;
	}
	fy_nativeReturnHandle(context, thread, handleRet);
}

void fy_coreRegisterCoreHandlers(fy_VMContext *context) {
	fy_vmRegisterNativeHandler(
			context,
			"java/lang/System.arraycopy.(L"FY_BASE_OBJECT";IL"FY_BASE_OBJECT";II)V",
			NULL, SystemArrayCopy);
	fy_vmRegisterNativeHandler(context,
			"java/lang/System.currentTimeMillis.()J", NULL, SystemTimeMS);
	fy_vmRegisterNativeHandler(context, "java/lang/System.nanoTime.()J", NULL,
			SystemTimeNS);
	fy_vmRegisterNativeHandler(context,
			""FY_BASE_OBJECT".getClass.()L"FY_BASE_CLASS";", NULL,
			ObjectGetClass);
	fy_vmRegisterNativeHandler(context,
			""FY_BASE_CLASS".getComponentType.()L"FY_BASE_CLASS";", NULL,
			ClassGetComponentType);
	fy_vmRegisterNativeHandler(context,
			""FY_BASE_THREAD".currentThread.()L"FY_BASE_THREAD";", NULL,
			ThreadCurrentThread);
	fy_vmRegisterNativeHandler(context, ""FY_BASE_THREAD".setPriority0.(I)V",
			NULL, ThreadSetPriority);
	fy_vmRegisterNativeHandler(context, ""FY_BASE_THREAD".isAlive.()Z", NULL,
			ThreadIsAlive);
	fy_vmRegisterNativeHandler(context, ""FY_BASE_THREAD".start0.()V", NULL,
			ThreadStart);
	fy_vmRegisterNativeHandler(
			context,
			"com/cirnoworks/fisce/privat/FiScEVM.debugOut.(L"FY_BASE_STRING";)V",
			NULL, VMDebugOut);
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.debugOut.(I)V", NULL,
			VMDebugOutI);
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.debugOut.(J)V", NULL,
			VMDebugOutJ);
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.debugOut.(F)V", NULL,
			VMDebugOutF);
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.debugOut.(D)V", NULL,
			VMDebugOutD);
	fy_vmRegisterNativeHandler(
			context,
			"com/cirnoworks/fisce/privat/FiScEVM.throwOut.(L"FY_BASE_THROWABLE";L"FY_BASE_STRING";)V",
			NULL, VMThrowOut);
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.exit.(I)V", NULL, VMExit);
	fy_vmRegisterNativeHandler(
			context,
			"com/cirnoworks/fisce/privat/FiScEVM.decode.(L"FY_BASE_STRING";[BII)[C",
			NULL, VMDecode);
	fy_vmRegisterNativeHandler(
			context,
			"com/cirnoworks/fisce/privat/FiScEVM.encode.(L"FY_BASE_STRING";[CII)[B",
			NULL, VMEncode);
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.getDoubleRaw.(D)J", NULL,
			VMGetDoubleRaw);
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.getFloatRaw.(F)I", NULL,
			VMGetFloatRaw);
	fy_vmRegisterNativeHandler(
			context,
			"com/cirnoworks/fisce/privat/FiScEVM.stringToDouble.(L"FY_BASE_STRING";)D",
			NULL, VMStringToDouble);
	fy_vmRegisterNativeHandler(
			context,
			"com/cirnoworks/fisce/privat/FiScEVM.doubleToString.(D)L"FY_BASE_STRING";",
			NULL, VMDoubleToString);
	fy_vmRegisterNativeHandler(
			context,
			"com/cirnoworks/fisce/privat/FiScEVM.stringToFloat.(L"FY_BASE_STRING";)F",
			NULL, VMStringToFloat);
	fy_vmRegisterNativeHandler(
			context,
			"com/cirnoworks/fisce/privat/FiScEVM.floatToString.(F)L"FY_BASE_STRING";",
			NULL, VMFloatToString);
	fy_vmRegisterNativeHandler(
			context,
			"com/cirnoworks/fisce/privat/SystemOutputStream.write0.(IL"FY_BASE_STRING";)V",
			NULL, SOSWrite);
}

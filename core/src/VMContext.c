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

#include "fyc/VMContext.h"
#include "fyc/FileInputStream.h"

/***********private***********/
static void initConstantStrings(fy_context *context, fy_exception *exception) {
	fy_memblock *block = context->memblocks;

	fy_strInitWithUTF8(block, context->sBoolean, "<boolean>", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sByte, "<byte>", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sShort, "<short>", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sChar, "<char>", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sInt, "<int>", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sFloat, "<float>", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sLong, "<long>", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sDouble, "<double>", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sTopClass, FY_BASE_OBJECT, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sClassClass, FY_BASE_CLASS, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sClassThrowable, FY_BASE_THROWABLE,
			exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sString, FY_BASE_STRING, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sThread, FY_BASE_THREAD, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sStringArray, "[L"FY_BASE_THROWABLE";",
			exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sThrowablePrintStacktrace,
			FY_BASE_THROWABLE".printStackTrace.()V", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sThrowableDetailMessage,
			FY_BASE_THROWABLE".detailMessage.L"FY_BASE_STRING";", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sInit, FY_METHOD_INIT, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sClinit, FY_METHOD_CLINIT, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sFMain, FY_METHODF_MAIN, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sFRun, FY_METHODF_RUN, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sFName, FY_FIELDF_NAME, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sFPriority, FY_FIELDF_PRIORITY,
			exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sFDaemon, FY_FIELDF_DAEMON, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sStringCount, FY_BASE_STRING".count.I",
			exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sStringValue, FY_BASE_STRING".value.[C",
			exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sStringOffset, FY_BASE_STRING".offset.I",
			exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sStackTraceElement,
			FY_BASE_STACKTHREADELEMENT, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sStackTraceElementArray,
			"[L"FY_BASE_STACKTHREADELEMENT";", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sStackTraceElementDeclaringClass,
			FY_BASE_STACKTHREADELEMENT".declaringClass.L"FY_BASE_STRING";",
			exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sStackTraceElementMethodName,
			FY_BASE_STACKTHREADELEMENT".methodName.L"FY_BASE_STRING";",
			exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sStackTraceElementFileName,
			FY_BASE_STACKTHREADELEMENT".fileName.L"FY_BASE_STRING";",
			exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sStackTraceElementLineNumber,
			FY_BASE_STACKTHREADELEMENT".lineNumber.I", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sThrowableStackTrace,
			FY_BASE_THROWABLE".stackTrace.[L"FY_BASE_STACKTHREADELEMENT";",
			exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sArrayBoolean, "[Z", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sArrayChar, "[C", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sArrayFloat, "[F", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sArrayDouble, "[D", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sArrayByte, "[B", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sArrayShort, "[S", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sArrayInteger, "[I", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sArrayLong, "[J", exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInitWithUTF8(block, context->sArrayObject, "[L"FY_BASE_OBJECT";",
			exception);
	fy_exceptionCheckAndReturn(exception);
}

static void initConstantPrimitives(fy_context *context, fy_exception *exception) {
	fy_char *cc;
	fy_memblock *block = context->memblocks;

	context->primitives[FY_TYPE_BOOLEAN] = context->sBoolean;
	context->primitives[FY_TYPE_BYTE] = context->sByte;
	context->primitives[FY_TYPE_SHORT] = context->sShort;
	context->primitives[FY_TYPE_CHAR] = context->sChar;
	context->primitives[FY_TYPE_INT] = context->sInt;
	context->primitives[FY_TYPE_FLOAT] = context->sFloat;
	context->primitives[FY_TYPE_LONG] = context->sLong;
	context->primitives[FY_TYPE_DOUBLE] = context->sDouble;

	fy_hashMapInit(block, context->mapPrimitivesRev, 13, 12, exception);
	fy_exceptionCheckAndReturn(exception);

	cc = fy_mmAllocate(block, sizeof(fy_char), exception);
	fy_exceptionCheckAndReturn(exception);
	*cc = FY_TYPE_BOOLEAN;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sBoolean, cc,
			exception);
	fy_exceptionCheckAndReturn(exception);

	cc = fy_mmAllocate(block, sizeof(fy_char), exception);
	fy_exceptionCheckAndReturn(exception);
	*cc = FY_TYPE_BYTE;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sByte, cc,
			exception);
	fy_exceptionCheckAndReturn(exception);

	cc = fy_mmAllocate(block, sizeof(fy_char), exception);
	fy_exceptionCheckAndReturn(exception);
	*cc = FY_TYPE_SHORT;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sShort, cc,
			exception);
	fy_exceptionCheckAndReturn(exception);

	cc = fy_mmAllocate(block, sizeof(fy_char), exception);
	fy_exceptionCheckAndReturn(exception);
	*cc = FY_TYPE_CHAR;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sChar, cc,
			exception);
	fy_exceptionCheckAndReturn(exception);

	cc = fy_mmAllocate(block, sizeof(fy_char), exception);
	fy_exceptionCheckAndReturn(exception);
	*cc = FY_TYPE_INT;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sInt, cc,
			exception);
	fy_exceptionCheckAndReturn(exception);

	cc = fy_mmAllocate(block, sizeof(fy_char), exception);
	fy_exceptionCheckAndReturn(exception);
	*cc = FY_TYPE_FLOAT;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sFloat, cc,
			exception);
	fy_exceptionCheckAndReturn(exception);

	cc = fy_mmAllocate(block, sizeof(fy_char), exception);
	fy_exceptionCheckAndReturn(exception);
	*cc = FY_TYPE_LONG;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sLong, cc,
			exception);
	fy_exceptionCheckAndReturn(exception);

	cc = fy_mmAllocate(block, sizeof(fy_char), exception);
	fy_exceptionCheckAndReturn(exception);
	*cc = FY_TYPE_DOUBLE;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sDouble, cc,
			exception);
	fy_exceptionCheckAndReturn(exception);
}

static void initStructClassloader(fy_context *context, fy_exception *exception) {
	fy_memblock *block = context->memblocks;
	fy_hashMapInit(block, context->mapClassNameToId, 1024, 12, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_hashMapInit(block, context->mapFieldNameToId, 1024, 12, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_hashMapInit(block, context->mapMethodNameToId, 1024, 12, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_hashMapInit(block, context->mapMUNameToNH, 1024, 12, exception);
	fy_exceptionCheckAndReturn(exception);
}

static void initThreadManager(fy_context *context, fy_exception *exception) {
	context->nextThreadId = 1;

	fy_arrayListInit(context->memblocks, context->runningThreads,
			sizeof(fy_thread*), 16, exception);
	fy_exceptionCheckAndReturn(exception);

	context->state = FY_TM_STATE_NEW;

	context->pricmds[0] = 1;
	context->pricmds[1] = 125;
	context->pricmds[2] = 250;
	context->pricmds[3] = 500;
	context->pricmds[4] = 1000;
	context->pricmds[5] = 2000;
	context->pricmds[6] = 4000;
	context->pricmds[7] = 8000;
	context->pricmds[8] = 16000;
	context->pricmds[9] = 32000;
	context->pricmds[10] = 64000;
}

static void initHeap(fy_context *context, fy_exception *exception) {
	fy_memblock *block = context->memblocks;
	context->nextHandle = 1;

	fy_hashMapInitSimple(block, context->literals, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_arrayListInit(block, context->toFinalize, sizeof(fy_uint), 32,
			exception);
	fy_exceptionCheckAndReturn(exception);

	fy_arrayListInit(block, context->protected, sizeof(fy_uint), 128,
			exception);
	fy_exceptionCheckAndReturn(exception);
}
/************public***************/

void fy_vmContextInit(fy_context *context, fy_exception *exception) {
	/*
	 fy_str *sBoolean;
	 fy_str *sByte;
	 fy_str *sShort;
	 fy_str *sChar;
	 fy_str *sInt;
	 fy_str *sFloat;
	 fy_str *sLong;
	 fy_str *sDouble;

	 fy_str *primitives[128];
	 fy_hashMap *mapPrimitivesRev;
	 */

	fy_fisInitInputStream(&(context->inputStream));
	fy_mmInit(context->memblocks, exception);
	fy_exceptionCheckAndReturn(exception);
	fy_portInit(context->port);

	initThreadManager(context, exception);
	fy_exceptionCheckAndReturn(exception);

	initConstantStrings(context, exception);
	fy_exceptionCheckAndReturn(exception);

	initConstantPrimitives(context, exception);
	fy_exceptionCheckAndReturn(exception);

	initStructClassloader(context, exception);
	fy_exceptionCheckAndReturn(exception);

	initHeap(context, exception);
	fy_exceptionCheckAndReturn(exception);

	fy_coreRegisterCoreHandlers(context, exception);
	fy_coreRegisterMathHandlers(context, exception);
	fy_exceptionCheckAndReturn(exception);
}

static fy_class* getClass(fy_context *context, fy_str *name) {
	int *pFid;
	fy_class *ret;
	pFid = fy_hashMapGet(context->memblocks, context->mapClassNameToId, name);
	if (pFid == NULL) {
		return NULL;
	}
	if (pFid == NULL || (ret = context->classes[*pFid]) == NULL) {
		return NULL;
	}
	return ret;
}

void fy_vmContextDestroy(fy_context *context) {
	fy_portDestroy(context->port);
	fy_mmDestroy(context->memblocks);
}

void fy_vmRegisterField(fy_context *context, fy_field *field,
		fy_exception *exception) {
	int *pFid;
	fy_memblock *block = context->memblocks;
	pFid = fy_hashMapGet(block, context->mapFieldNameToId, field->uniqueName);
	if (pFid == NULL) {
		pFid = fy_mmAllocate(block, sizeof(int), exception);
		fy_exceptionCheckAndReturn(exception);
		field->field_id = *pFid = context->fieldsCount++;
		fy_hashMapPut(block, context->mapFieldNameToId, field->uniqueName, pFid,
				exception);
		fy_exceptionCheckAndReturn(exception);
	}
	context->fields[*pFid] = field;
}

fy_field *fy_vmGetField(fy_context *context, fy_str *uniqueName) {
	int *pMid = fy_hashMapGet(context->memblocks, context->mapFieldNameToId,
			uniqueName);
	if (pMid == NULL) {
		return NULL;
	}
	return context->fields[*pMid];
}

fy_field *fy_vmLookupFieldVirtual(fy_context *context, fy_class *clazz,
		fy_str *fieldName, fy_exception *exception) {
	/*TODO Maybe wrong!!! need to check*/
	int *pFid;
	fy_field *field = NULL;
	fy_str *uniqueName;
	fy_str *uniqueNameTmp;
	fy_memblock *block = context->memblocks;

	uniqueName = fy_mmAllocate(block, sizeof(fy_str), exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	uniqueNameTmp = fy_mmAllocate(block, sizeof(fy_str), exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fy_strInit(block, uniqueName,
			clazz->className->length + fieldName->length + 1, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fy_strInit(block, uniqueNameTmp,
			clazz->className->length + fieldName->length + 1, exception);
	fy_exceptionCheckAndReturn(exception)NULL;

	fy_strAppend(block, uniqueName, clazz->className, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fy_strAppend(block, uniqueName, fieldName, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	while (clazz != NULL) {
		fy_strClear(uniqueNameTmp);
		fy_strAppend(block, uniqueNameTmp, clazz->className, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		fy_strAppend(block, uniqueNameTmp, fieldName, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		pFid = fy_hashMapGet(block, context->mapFieldNameToId, uniqueNameTmp);
		if (pFid != NULL) {
			fy_hashMapPut(block, context->mapFieldNameToId, uniqueName, pFid,
					exception);
			fy_exceptionCheckAndReturn(exception)NULL;
			field = context->fields[*pFid];
			break;
		}
		clazz = clazz->super;
	}
	fy_strDestroy(block, uniqueNameTmp);
	fy_mmFree(block, uniqueNameTmp);
	fy_strDestroy(block, uniqueName);
	fy_mmFree(block, uniqueName);
	return field;
}

int fy_vmGetMethodId(fy_context *context, fy_str *uniqueName,
		fy_exception *exception) {
	int *pMid;
	pMid = fy_hashMapGet(context->memblocks, context->mapMethodNameToId,
			uniqueName);
	if (pMid == NULL) {
		fy_fault(exception, NULL, "Can't find method!");
	}
	return *pMid;
}

void fy_vmRegisterMethod(fy_context *context, fy_method *method,
		fy_exception *exception) {
	int *pMid;
	fy_memblock *block = context->memblocks;
	pMid = fy_hashMapGet(context->memblocks, context->mapMethodNameToId,
			method->uniqueName);
	if (pMid == NULL) {
		pMid = fy_mmAllocate(block, sizeof(int), exception);
		fy_exceptionCheckAndReturn(exception);
		method->method_id = *pMid = context->methodsCount++;
		fy_hashMapPut(block, context->mapMethodNameToId, method->uniqueName,
				pMid, exception);
		fy_exceptionCheckAndReturn(exception);
	}
	context->methods[*pMid] = method;
}

fy_method *fy_vmGetMethod(fy_context *context, fy_str *uniqueName) {
	int *pMid = fy_hashMapGet(context->memblocks, context->mapMethodNameToId,
			uniqueName);
	if (pMid == NULL) {
		return NULL;
	}
	return context->methods[*pMid];
}

fy_method *fy_vmLookupMethodVirtual(fy_context *context, fy_class *clazz,
		fy_str *methodName, fy_exception *exception) {
	int *pMid;
	fy_method *method = NULL;
	fy_memblock *block = context->memblocks;
	fy_str *uniqueName = fy_mmAllocate(block, sizeof(fy_str), exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fy_str *uniqueNameTmp = fy_mmAllocate(block, sizeof(fy_str), exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fy_strInit(block, uniqueName,
			clazz->className->length + methodName->length + 1, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fy_strInit(block, uniqueNameTmp,
			clazz->className->length + methodName->length + 1, exception);
	fy_exceptionCheckAndReturn(exception)NULL;

	fy_strAppend(block, uniqueName, clazz->className, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fy_strAppend(block, uniqueName, methodName, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	while (clazz != NULL) {
		fy_strClear(uniqueNameTmp);
		fy_strAppend(block, uniqueNameTmp, clazz->className, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		fy_strAppend(block, uniqueNameTmp, methodName, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		pMid = fy_hashMapGet(block, context->mapMethodNameToId, uniqueNameTmp);
		if (pMid != NULL) {
			fy_hashMapPut(block, context->mapMethodNameToId, uniqueName, pMid,
					exception);
			fy_exceptionCheckAndReturn(exception)NULL;
			method = context->methods[*pMid];
			break;
		}
		clazz = clazz->super;
	}
	fy_strDestroy(block, uniqueNameTmp);
	fy_mmFree(block, uniqueNameTmp);
	fy_strDestroy(block, uniqueName);
	fy_mmFree(block, uniqueName);
	return method;
}

void fy_vmRegisterClass(fy_context *context, fy_class *clazz,
		fy_exception *exception) {
	int *pCid;
	fy_memblock *block = context->memblocks;
	pCid = fy_hashMapGet(block, context->mapClassNameToId, clazz->className);
	if (pCid == NULL) {
		pCid = fy_mmAllocate(block, sizeof(int), exception);
		fy_exceptionCheckAndReturn(exception);
		*pCid = (context->classesCount++) + 1;
		if (*pCid >= MAX_CLASSES) {
			fy_fault(exception, NULL, "Too many classes!");
			return;
		}
		fy_hashMapPut(block, context->mapClassNameToId, clazz->className, pCid,
				exception);
		fy_exceptionCheckAndReturn(exception);
		/*
		 clazz->staticArea = fy_mmAllocate(block, clazz->staticSize);*/
	}
	context->classes[*pCid] = clazz;
	clazz->classId = *pCid;
}

/*Likes com.cirnoworks.fisce.vm.VMContext.getClass(String name)*/
fy_class *fy_vmLookupClass(fy_context *context, fy_str *name,
		fy_exception *exception) {
	fy_class *clazz;
	fy_class *clazz2;
	clazz = getClass(context, name);
	if (clazz == NULL) {
		clazz = fy_clLoadclass(context, name, exception);
		if (exception->exceptionType != exception_none) {
			return NULL;
		}
		fy_vmRegisterClass(context, clazz, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		fy_clPhase2(context, clazz, exception);
		if (exception->exceptionType != exception_none) {
			return NULL;
		}
		clazz2 = fy_vmLookupClass(context, context->sClassClass, exception);
		if (exception->exceptionType != exception_none) {
			return NULL;
		}
		clazz->classObjId = fy_heapAllocate(context, clazz2, exception);
		if (exception->exceptionType != exception_none) {
			return NULL;
		}
		context->objects[clazz->classObjId].attachedId = clazz->classId;
	}
	return clazz;
}

fy_class *fy_vmGetClassFromClassObject(fy_context *context, fy_uint handle,
		fy_exception *exception) {
	fy_class *classClass;
	fy_class *inputClass;
	fy_int classId;
	inputClass = fy_heapGetClassOfObject(context, handle);
	classClass = fy_vmLookupClass(context, context->sClassClass, exception);
	if (exception->exceptionType != exception_none) {
		return NULL;
	}
	if (inputClass != classClass) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/VirtualMachineError");
		strcpy_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"Get class ID for non-class object");
		return NULL;
	}
	classId = context->objects[handle].attachedId;
	return context->classes[classId];
}

fy_class *fy_vmLookupClassFromConstant(fy_context *context,
		ConstantClass *classInfo, fy_exception *exception) {
	if (classInfo->derefed == 0) {
		classInfo->ci.clazz = fy_vmLookupClass(context, classInfo->ci.className,
				exception);
		if (exception->exceptionType != exception_none) {
			return NULL;
		}
		classInfo->derefed = 1;
	}
	return classInfo->ci.clazz;
}

fy_field *fy_vmLookupFieldFromConstant(fy_context *context,
		ConstantFieldRef *fieldInfo, fy_exception *exception) {
	fy_field *field;
	if (fieldInfo->derefed == 0) {
		fieldInfo->clazz = fy_vmLookupClassFromConstant(context,
				fieldInfo->constantClass, exception);
		if (exception->exceptionType != exception_none) {
			return NULL;
		}
		field = fy_vmLookupFieldVirtual(context, fieldInfo->clazz,
				fieldInfo->nameType, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		if (field == NULL) {
			fy_fault(exception, NULL, "Field not found");
			return NULL;
		}
		fieldInfo->derefed = 1;
		fieldInfo->field = field;
	} else {
		field = fieldInfo->field;
	}
	return field;
}

fy_method *fy_vmLookupMethodFromConstant(fy_context *context,
		ConstantMethodRef *methodInfo, fy_exception *exception) {
	fy_method *method;
	if (methodInfo->derefed == 0) {
		methodInfo->clazz = fy_vmLookupClassFromConstant(context,
				methodInfo->constantClass, exception);
		if (exception->exceptionType != exception_none) {
			return NULL;
		}
		method = fy_vmLookupMethodVirtual(context, methodInfo->clazz,
				methodInfo->nameType, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		if (method == NULL) {
			fy_fault(exception, NULL, "Method not found");
			return NULL;
		}
		methodInfo->derefed = 1;
		methodInfo->method = method;
	} else {
		method = methodInfo->method;
	}
	return method;
}

void fy_vmRegisterNativeHandler(fy_context *context, const char *name,
		void *data, fy_nhFunction handler, fy_exception *exception) {
	fy_nh* nh;
	fy_str* str;
	fy_memblock *block = context->memblocks;
	str = fy_strCreateFromUTF8(block, name, exception);
	fy_exceptionCheckAndReturn(exception);
	if (fy_hashMapGet(block, context->mapMUNameToNH, str) != NULL) {
		fy_fault(exception, NULL, "Native handler conflict %s", name);
	}
	nh = fy_mmAllocate(block, sizeof(fy_nh), exception);
	fy_exceptionCheckAndReturn(exception);
	nh->data = data;
	nh->handler = handler;
	fy_hashMapPut(block, context->mapMUNameToNH, str, nh, exception);
	fy_exceptionCheckAndReturn(exception);
	fy_strRelease(block, str);
}

fy_class *fy_vmLookupClassFromExceptionHandler(fy_context *context,
		fy_exceptionHandler *exceptionHandler, fy_exception *exception) {
	fy_class *clazz;
	if (exceptionHandler->catchTypeDerefed == 0) {
		clazz = fy_vmLookupClassFromConstant(context,
				exceptionHandler->ci.constantClass, exception);
		if (exception->exceptionType != exception_none) {
			return NULL;
		}
	} else {
		clazz = exceptionHandler->ci.clazz;
	}
	return clazz;
}

void fy_vmBootup(fy_context *context, const char* bootStrapClass,
		fy_exception *exception) {
	fy_str name;
	fy_class *clazz;
	context->TOP_CLASS = fy_vmLookupClass(context, context->sTopClass,
			exception);
	fy_exceptionCheckAndReturn(exception);

	context->TOP_THROWABLE = fy_vmLookupClass(context, context->sClassThrowable,
			exception);
	fy_exceptionCheckAndReturn(exception);
	name.content = NULL;
	fy_strInitWithUTF8(context->memblocks, &name, bootStrapClass, exception);
	fy_exceptionCheckAndReturn(exception);
	clazz = fy_vmLookupClass(context, &name, exception);
	fy_strDestroy(context->memblocks, &name);
	fy_exceptionCheckAndReturn(exception);
	fy_tmBootFromMain(context, clazz, exception);
	fy_exceptionCheckAndReturn(exception);
}


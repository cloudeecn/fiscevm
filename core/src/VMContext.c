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

/***********private***********/

/************public***************/

void *fy_vmAllocate(fy_VMContext *context, int size) {
	void *ret;
#ifdef _DEBUG
	fy_linkedListNode* node;
#endif
#if 0
	/*This will cause a bug...*/
	if (size == 0) {
		return NULL;
	}
#endif
	ret = vm_allocate(size + sizeof(fy_linkedListNode*));

	if (ret == NULL) {
		vm_die("OUT OF MEMORY");
	}
#ifdef _DEBUG
	node = fy_linkedListAppend(context->managedMemory, ret);
	*((fy_linkedListNode**) ret) = node;
	/*	printf("Allocate managed:%p at node %p\n", ret, node);*/
#else
	*((fy_linkedListNode**) ret) = fy_linkedListAppend(context->managedMemory,
			ret);
#endif

	return (jbyte*) ret + sizeof(fy_linkedListNode*);
}

void fy_vmFree(fy_VMContext *context, void *address) {
	void *base = (jbyte*) address - sizeof(fy_linkedListNode*);
	fy_linkedListNode* node = *((fy_linkedListNode**) base);
#ifdef _DEBUG
	/*	printf("Unallocate managed:%p at node %p\n", base, node);*/
#endif
	if (node->info != base) {
		vm_die("Error freeing address %p", address);
	}
	node->info = NULL;
	fy_linkedListRemoveNode(context->managedMemory, node);
	vm_free(base);
}

void fy_vmContextInit(fy_VMContext *context, fy_exception *exception) {
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
	if (fy_gInited != 0x1234567890abcdefll) {
		vm_die("Init global first!");
	}

	context->nextHandle = 1;

	context->managedMemory = vm_allocate(sizeof(fy_linkedList));
	fy_linkedListInit(context->managedMemory);

	context->sBoolean = fy_strAllocateFromUTF8(context, "boolean");
	context->sByte = fy_strAllocateFromUTF8(context, "byte");
	context->sShort = fy_strAllocateFromUTF8(context, "short");
	context->sChar = fy_strAllocateFromUTF8(context, "char");
	context->sInt = fy_strAllocateFromUTF8(context, "int");
	context->sFloat = fy_strAllocateFromUTF8(context, "float");
	context->sLong = fy_strAllocateFromUTF8(context, "long");
	context->sDouble = fy_strAllocateFromUTF8(context, "double");
	context->sTopClass = fy_strAllocateFromUTF8(context, FY_BASE_OBJECT);
	context->sClassClass = fy_strAllocateFromUTF8(context, FY_BASE_CLASS);
	context->sClassClassId = fy_strAllocateFromUTF8(context,
			FY_BASE_CLASS".classId.I");
	context->sClassThrowable = fy_strAllocateFromUTF8(context,
			FY_BASE_THROWABLE);
	context->sString = fy_strAllocateFromUTF8(context, FY_BASE_STRING);
	context->sStringArray = fy_strAllocateFromUTF8(context,
			"[L"FY_BASE_THROWABLE";");
	context->sMainPostfix = fy_strAllocateFromUTF8(context,
			".main.([L"FY_BASE_STRING";)V");
	context->sThrowablePrintStacktrace = fy_strAllocateFromUTF8(context,
			FY_BASE_THROWABLE".printStackTrace.()V");
	context->sThrowableDetailMessage = fy_strAllocateFromUTF8(context,
			FY_BASE_THROWABLE".detailMessage.L"FY_BASE_STRING";");
	context->sInit = fy_strAllocateFromUTF8(context, FY_INIT);
	context->sClinit = fy_strAllocateFromUTF8(context, FY_CLINIT);
	context->sStringCount = fy_strAllocateFromUTF8(context,
			FY_BASE_STRING".count.I");
	context->sStringValue = fy_strAllocateFromUTF8(context,
			FY_BASE_STRING".value.[C");
	context->sStringOffset = fy_strAllocateFromUTF8(context,
			FY_BASE_STRING".offset.I");

	context->sStackTraceElement = fy_strAllocateFromUTF8(context,
			FY_BASE_STACKTHREADELEMENT);
	context->sStackTraceElementArray = fy_strAllocateFromUTF8(context,
			"[L"FY_BASE_STACKTHREADELEMENT";");
	context->sStackTraceElementDeclaringClass = fy_strAllocateFromUTF8(context,
			FY_BASE_STACKTHREADELEMENT".declaringClass.L"FY_BASE_STRING";");
	context->sStackTraceElementMethodName = fy_strAllocateFromUTF8(context,
			FY_BASE_STACKTHREADELEMENT".methodName.L"FY_BASE_STRING";");
	context->sStackTraceElementFileName = fy_strAllocateFromUTF8(context,
			FY_BASE_STACKTHREADELEMENT".fileName.L"FY_BASE_STRING";");
	context->sStackTraceElementLineNumber = fy_strAllocateFromUTF8(context,
			FY_BASE_STACKTHREADELEMENT".lineNumber.I");
	context->sThrowableStackTrace = fy_strAllocateFromUTF8(context,
			FY_BASE_THROWABLE".stackTrace.[L"FY_BASE_STACKTHREADELEMENT";");

	context->sArrayBoolean = fy_strAllocateFromUTF8(context, "[Z");
	context->sArrayChar = fy_strAllocateFromUTF8(context, "[C");
	context->sArrayFloat = fy_strAllocateFromUTF8(context, "[F");
	context->sArrayDouble = fy_strAllocateFromUTF8(context, "[D");
	context->sArrayByte = fy_strAllocateFromUTF8(context, "[B");
	context->sArrayShort = fy_strAllocateFromUTF8(context, "[S");
	context->sArrayInteger = fy_strAllocateFromUTF8(context, "[I");
	context->sArrayLong = fy_strAllocateFromUTF8(context, "[J");

	context->primitives[TYPE_BOOLEAN] = context->sBoolean;
	context->primitives[TYPE_BYTE] = context->sByte;
	context->primitives[TYPE_SHORT] = context->sShort;
	context->primitives[TYPE_CHAR] = context->sChar;
	context->primitives[TYPE_INT] = context->sInt;
	context->primitives[TYPE_FLOAT] = context->sFloat;
	context->primitives[TYPE_LONG] = context->sLong;
	context->primitives[TYPE_DOUBLE] = context->sDouble;

	context->mapPrimitivesRev = fy_vmAllocate(context, sizeof(fy_hashMap));
	fy_hashMapInit(context, context->mapPrimitivesRev, 13, 12);
	fy_hashMapPut(context, context->mapPrimitivesRev, context->sBoolean,
			fy_gCBoolean);
	fy_hashMapPut(context, context->mapPrimitivesRev, context->sByte,
			fy_gCByte);
	fy_hashMapPut(context, context->mapPrimitivesRev, context->sShort,
			fy_gCShort);
	fy_hashMapPut(context, context->mapPrimitivesRev, context->sChar,
			fy_gCChar);
	fy_hashMapPut(context, context->mapPrimitivesRev, context->sInt, fy_gCInt);
	fy_hashMapPut(context, context->mapPrimitivesRev, context->sFloat,
			fy_gCFloat);
	fy_hashMapPut(context, context->mapPrimitivesRev, context->sLong,
			fy_gCLong);
	fy_hashMapPut(context, context->mapPrimitivesRev, context->sDouble,
			fy_gCDouble);

	context->mapClassNameToId = fy_vmAllocate(context, sizeof(fy_hashMap));
	fy_hashMapInit(context, context->mapClassNameToId, 1024, 12);
	context->mapFieldNameToId = fy_vmAllocate(context, sizeof(fy_hashMap));
	fy_hashMapInit(context, context->mapFieldNameToId, 1024, 12);
	context->mapMethodNameToId = fy_vmAllocate(context, sizeof(fy_hashMap));
	fy_hashMapInit(context, context->mapMethodNameToId, 1024, 12);
	context->mapMUNameToNH = fy_vmAllocate(context, sizeof(fy_hashMap));
	fy_hashMapInit(context, context->mapMUNameToNH, 1024, 12);

	context->TOP_CLASS = fy_vmLookupClass(context, context->sTopClass,
			exception);
	if (exception->exceptionType != exception_none) {
		return;
	}
	context->TOP_THROWABLE = fy_vmLookupClass(context, context->sClassThrowable,
			exception);
	if (exception->exceptionType != exception_none) {
		return;
	}

	context->literals = fy_vmAllocate(context, sizeof(fy_hashMap));
	fy_hashMapInitSimple(context, context->literals);

	fy_coreRegisterCoreHandlers(context);
	fy_portInit(context);
}

static void fy_memReleaser(fy_linkedListNode *node) {
	vm_free(node->info);
	/*	printf("%ld\n",vm_getAllocated());*/
	node->info = 0;
}

static fy_class* getClass(fy_VMContext *context, fy_str *name) {
	int *pFid;
	fy_class *ret;
	pFid = fy_hashMapGet(context, context->mapClassNameToId, name);
	if (pFid == NULL) {
		return NULL;
	}
	if (*pFid < 0 || *pFid >= MAX_CLASSES) {
		vm_die("Invalid class id=%d", *pFid);
	}
	if (pFid == NULL || (ret = context->classes[*pFid]) == NULL) {
		return NULL;
	}
	return ret;
}

void fy_vmContextDestroy(fy_VMContext *context) {
	int i;
	fy_portDestroy(context);
	for (i = 0; i < MAX_OBJECTS; i++) {
		if (context->objects[i].clazz != NULL) {
			switch (context->objects[i].sizeShift) {
			case fy_SIZE_SHIFT_BYTE:
				vm_free(context->objects[i].data.bdata);
				break;
			case fy_SIZE_SHIFT_LONG:
				vm_free(context->objects[i].data.ldata);
				break;
			default:
				vm_free(context->objects[i].data.idata);
				break;
			}
		}
	}
#ifdef _DEBUG
	printf("Releasing %d managed memory blocks\n",
			context->managedMemory->count);
#endif
	fy_linkedListTraverse(context->managedMemory, fy_memReleaser);
	fy_linkedListDestroy(context->managedMemory);
	vm_free(context->managedMemory);
}

void fy_vmRegisterField(fy_VMContext *context, fy_field *field) {
	int *pFid;
	pFid = fy_hashMapGet(context, context->mapFieldNameToId, field->uniqueName);
	if (pFid == NULL) {
		pFid = fy_vmAllocate(context, sizeof(int));
		*pFid = context->fieldsCount++;
		fy_hashMapPut(context, context->mapFieldNameToId, field->uniqueName,
				pFid);
	}
	context->fields[*pFid] = field;
}

fy_field *fy_vmGetField(fy_VMContext *context, fy_str *uniqueName) {
	int *pMid = fy_hashMapGet(context, context->mapFieldNameToId, uniqueName);
	if (pMid == NULL) {
		return NULL;
	}
	return context->fields[*pMid];
}

fy_field *fy_vmLookupFieldVirtual(fy_VMContext *context, fy_class *clazz,
		fy_str *fieldName) {
	/*TODO Maybe wrong!!! need to check*/
	int *pFid;
	fy_field *field = NULL;
	fy_str *uniqueName;
	fy_str *uniqueNameTmp;

	uniqueName = fy_vmAllocate(context, sizeof(fy_str));
	uniqueNameTmp = fy_vmAllocate(context, sizeof(fy_str));
	fy_strInit(context, uniqueName,
			clazz->className->length + fieldName->length + 1);
	fy_strInit(context, uniqueNameTmp,
			clazz->className->length + fieldName->length + 1);

	fy_strAppend(context, uniqueName, clazz->className);
	fy_strAppend(context, uniqueName, fieldName);
	while (clazz != NULL) {
		fy_strClear(uniqueNameTmp);
		fy_strAppend(context, uniqueNameTmp, clazz->className);
		fy_strAppend(context, uniqueNameTmp, fieldName);
		pFid = fy_hashMapGet(context, context->mapFieldNameToId, uniqueNameTmp);
		if (pFid != NULL) {
			fy_hashMapPut(context, context->mapFieldNameToId, uniqueName, pFid);
			field = context->fields[*pFid];
			break;
		}
		clazz = clazz->super;
	}
	fy_strDestroy(context, uniqueNameTmp);
	fy_vmFree(context, uniqueNameTmp);
	fy_strDestroy(context, uniqueName);
	fy_vmFree(context, uniqueName);
	return field;
}

fy_field *fy_vmLookupFieldStatic(fy_VMContext *context, fy_class *clazz,
		fy_str *fieldName) {
	/*TODO Maybe wrong!!! need to check*/
	int *pFid;
	fy_field *field = NULL;
	fy_str *uniqueName;

	uniqueName = fy_vmAllocate(context, sizeof(fy_str));
	fy_strInit(context, uniqueName,
			clazz->className->length + fieldName->length + 1);

	fy_strAppend(context, uniqueName, clazz->className);
	fy_strAppend(context, uniqueName, fieldName);
	pFid = fy_hashMapGet(context, context->mapFieldNameToId, uniqueName);
	if (pFid != NULL) {
		field = context->fields[*pFid];
		if (field->owner != clazz) {
			field = NULL;
		}
	}
	fy_strDestroy(context, uniqueName);
	fy_vmFree(context, uniqueName);
	return field;
}

int fy_vmGetMethodId(fy_VMContext *context, fy_str *uniqueName) {
	int *pMid;
	pMid = fy_hashMapGet(context, context->mapMethodNameToId, uniqueName);
	if (pMid == NULL) {
		vm_die("Can't find method!");
	}
	return *pMid;
}

void fy_vmRegisterMethod(fy_VMContext *context, fy_method *method) {
	int *pMid;
	pMid = fy_hashMapGet(context, context->mapMethodNameToId,
			method->uniqueName);
	if (pMid == NULL) {
		pMid = fy_vmAllocate(context, sizeof(int));
		method->method_id = *pMid = context->methodsCount++;
		fy_hashMapPut(context, context->mapMethodNameToId, method->uniqueName,
				pMid);
	}
	context->methods[*pMid] = method;
}

fy_method *fy_vmGetMethod(fy_VMContext *context, fy_str *uniqueName) {
	int *pMid = fy_hashMapGet(context, context->mapMethodNameToId, uniqueName);
	if (pMid == NULL) {
		return NULL;
	}
	return context->methods[*pMid];
}

fy_method *fy_vmLookupMethodVirtual(fy_VMContext *context, fy_class *clazz,
		fy_str *methodName) {
	int *pMid;
	fy_method *method = NULL;
	fy_str *uniqueName = fy_vmAllocate(context, sizeof(fy_str));
	fy_str *uniqueNameTmp = fy_vmAllocate(context, sizeof(fy_str));
	fy_strInit(context, uniqueName,
			clazz->className->length + methodName->length + 1);
	fy_strInit(context, uniqueNameTmp,
			clazz->className->length + methodName->length + 1);

	fy_strAppend(context, uniqueName, clazz->className);
	fy_strAppend(context, uniqueName, methodName);
	while (clazz != NULL) {
		fy_strClear(uniqueNameTmp);
		fy_strAppend(context, uniqueNameTmp, clazz->className);
		fy_strAppend(context, uniqueNameTmp, methodName);
		pMid = fy_hashMapGet(context, context->mapMethodNameToId,
				uniqueNameTmp);
		if (pMid != NULL) {
			fy_hashMapPut(context, context->mapMethodNameToId, uniqueName,
					pMid);
			method = context->methods[*pMid];
			break;
		}
		clazz = clazz->super;
	}
	fy_strDestroy(context, uniqueNameTmp);
	fy_vmFree(context, uniqueNameTmp);
	fy_strDestroy(context, uniqueName);
	fy_vmFree(context, uniqueName);
	return method;
}

fy_method *fy_vmLookupMethodStatic(fy_VMContext *context, fy_class *clazz,
		fy_str *methodName) {
	int *pMid;
	fy_method *method = NULL;
	fy_str *uniqueName = fy_vmAllocate(context, sizeof(fy_str));
	fy_strInit(context, uniqueName,
			clazz->className->length + methodName->length + 1);

	fy_strAppend(context, uniqueName, clazz->className);
	fy_strAppend(context, uniqueName, methodName);
	pMid = fy_hashMapGet(context, context->mapMethodNameToId, uniqueName);
	if (pMid != NULL) {
		method = context->methods[*pMid];
		if (method->owner != clazz) {
			method = NULL;
		}
	}
	fy_strDestroy(context, uniqueName);
	fy_vmFree(context, uniqueName);
	return method;
}

void fy_vmRegisterClass(fy_VMContext *context, fy_class *clazz) {
	int *pCid;
	pCid = fy_hashMapGet(context, context->mapClassNameToId, clazz->className);
	if (pCid == NULL) {
		pCid = fy_vmAllocate(context, sizeof(int));
		*pCid = (context->classesCount++) + 1;
		if (*pCid >= MAX_CLASSES) {
			vm_die("Too many classes!");
		}
		fy_hashMapPut(context, context->mapClassNameToId, clazz->className,
				pCid);
		/*
		 clazz->staticArea = fy_vmAllocate(context, clazz->staticSize);*/
	}
	context->classes[*pCid] = clazz;
	clazz->classId = *pCid;
}

/*Likes com.cirnoworks.fisce.vm.VMContext.getClass(String name)*/
fy_class *fy_vmLookupClass(fy_VMContext *context, fy_str *name,
		fy_exception *exception) {
	fy_class *clazz;
	fy_class *clazz2;
	fy_field *classIdField;
	clazz = getClass(context, name);
	if (clazz == NULL) {
		clazz = fy_clLoadclass(context, name, exception);
		if (exception->exceptionType != exception_none) {
			return NULL;
		}
		fy_vmRegisterClass(context, clazz);
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
		classIdField = fy_vmGetField(context, context->sClassClassId);
		if (classIdField == NULL) {
			exception->exceptionType = exception_normal;
			strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
					"java/lang/VirtualMachineError");
			strcpy_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
					FY_BASE_CLASS" has no integer field classId");
			return NULL;
		}
		fy_heapPutFieldInt(context, clazz->classObjId, classIdField,
				clazz->classId, exception);
		if (exception->exceptionType != exception_none) {
			return NULL;
		}
	}
	return clazz;
}

fy_class *fy_vmGetClassFromClassObject(fy_VMContext *context, juint handle,
		fy_exception *exception) {
	fy_class *classClass;
	fy_class *inputClass;
	jint classId;
	fy_field *classIdField;
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
	classIdField = fy_vmGetField(context, context->sClassClassId);
	if (classIdField == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/VirtualMachineError");
		strcpy_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"Can't find field for classId in "FY_BASE_CLASS);
		return NULL;
	}
	classId = fy_heapGetFieldInt(context, handle, classIdField, exception);
	if (exception->exceptionType != exception_none) {
		return NULL;
	}
	return context->classes[classId];

}

fy_class *fy_vmLookupClassFromConstant(fy_VMContext *context,
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

fy_field *fy_vmLookupFieldFromConstant(fy_VMContext *context,
		ConstantFieldRef *fieldInfo, fy_exception *exception) {
	fy_field *field;
	if (fieldInfo->derefed == 0) {
		fieldInfo->clazz = fy_vmLookupClassFromConstant(context,
				fieldInfo->constantClass, exception);
		if (exception->exceptionType != exception_none) {
			return NULL;
		}
		field = fy_vmLookupFieldStatic(context, fieldInfo->clazz,
				fieldInfo->nameType);
		if (field == NULL) {
			vm_die("Field not found");
		}
		fieldInfo->derefed = 1;
		fieldInfo->field = field;
	} else {
		field = fieldInfo->field;
	}
	return field;
}

fy_method *fy_vmLookupMethodFromConstant(fy_VMContext *context,
		ConstantMethodRef *methodInfo, fy_exception *exception) {
	fy_method *method;
	if (methodInfo->derefed == 0) {
		methodInfo->clazz = fy_vmLookupClassFromConstant(context,
				methodInfo->constantClass, exception);
		if (exception->exceptionType != exception_none) {
			return NULL;
		}
		method = fy_vmLookupMethodVirtual(context, methodInfo->clazz,
				methodInfo->nameType);
		if (method == NULL) {
			fy_strPrint(methodInfo->clazz->className);
			fy_strPrint(methodInfo->nameType);
			vm_die("Method not found");
		}
		methodInfo->derefed = 1;
		methodInfo->method = method;
	} else {
		method = methodInfo->method;
	}
	return method;
}

void fy_vmRegisterNativeHandler(fy_VMContext *context, const char *name,
		void *data, fy_nhFunction handler) {
	fy_nh* nh;
	fy_str* str;
	str = fy_strAllocateFromUTF8(context, name);
	if (fy_hashMapGet(context, context->mapMUNameToNH, str) != NULL) {
		vm_die("Native handler conflict %s", name);
	}
	nh = fy_vmAllocate(context, sizeof(fy_nh));
	nh->data = data;
	nh->handler = handler;
	fy_hashMapPut(context, context->mapMUNameToNH, str, nh);
	fy_strRelease(context, str);
}

fy_class *fy_vmLookupClassFromExceptionHandler(fy_VMContext *context,
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

void fy_vmBootup(fy_VMContext *context, jchar* bootStrapClass) {

}


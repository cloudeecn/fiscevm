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
#include "fyc/Instructions.h"
#include "fyc/FileInputStream.h"
#include "fyc/NConfig.h"
#include "fyc/BinarySaver.h"
#include "fyc/BAIS.h"

/***********private***********/
static void initConstantStrings(fy_context *context, fy_exception *exception) {
	fy_memblock *block = context->memblocks;

	context->sAttCode = fy_strCreatePermFromUTF8(block, FY_ATT_CODE, 0,
			exception);
	FYEH();

	context->sAttLineNum = fy_strCreatePermFromUTF8(block, FY_ATT_LINENUM, 0,
			exception);
	FYEH();

	context->sAttSynth = fy_strCreatePermFromUTF8(block, FY_ATT_SYNTH, 0,
			exception);
	FYEH();

	context->sAttSourceFile = fy_strCreatePermFromUTF8(block,
			FY_ATT_SOURCE_FILE, 0, exception);
	FYEH();
	context->sAttConstantValue = fy_strCreatePermFromUTF8(block,
			FY_ATT_CONSTANT_VALIE, 0, exception);
	FYEH();
	context->sBoolean = fy_strCreatePermFromUTF8(block, "<boolean>", 0,
			exception);
	FYEH();

	context->sByte = fy_strCreatePermFromUTF8(block, "<byte>", 0, exception);
	FYEH();

	context->sShort = fy_strCreatePermFromUTF8(block, "<short>", 0, exception);
	FYEH();

	context->sChar = fy_strCreatePermFromUTF8(block, "<char>", 0, exception);
	FYEH();

	context->sInt = fy_strCreatePermFromUTF8(block, "<int>", 0, exception);
	FYEH();

	context->sFloat = fy_strCreatePermFromUTF8(block, "<float>", 0, exception);
	FYEH();

	context->sLong = fy_strCreatePermFromUTF8(block, "<long>", 0, exception);
	FYEH();

	context->sDouble = fy_strCreatePermFromUTF8(block, "<double>", 0,
			exception);
	FYEH();

	context->sVoid = fy_strCreatePermFromUTF8(block, "<void>", 0, exception);
	FYEH();

	context->sEnum = fy_strCreatePermFromUTF8(block, FY_BASE_ENUM, 0,
			exception);
	FYEH();

	context->sAnnotation = fy_strCreatePermFromUTF8(block, FY_BASE_ANNOTATION,
			0, exception);
	FYEH();

	context->sTopClass = fy_strCreatePermFromUTF8(block, FY_BASE_OBJECT, 0,
			exception);
	FYEH();

	context->sClassClass = fy_strCreatePermFromUTF8(block, FY_BASE_CLASS, 0,
			exception);
	FYEH();

	context->sClassField = fy_strCreatePermFromUTF8(block, FY_REFLECT_FIELD, 0,
			exception);
	FYEH();

	context->sClassMethod = fy_strCreatePermFromUTF8(block, FY_REFLECT_METHOD,
			0, exception);
	FYEH();

	context->sClassConstructor = fy_strCreatePermFromUTF8(block,
			FY_REFLECT_CONSTRUCTOR, 0, exception);
	FYEH();

	context->sClassThrowable = fy_strCreatePermFromUTF8(block,
			FY_BASE_THROWABLE, 0, exception);
	FYEH();

	context->sString = fy_strCreatePermFromUTF8(block, FY_BASE_STRING, 0,
			exception);
	FYEH();

	context->sThread = fy_strCreatePermFromUTF8(block, FY_BASE_THREAD, 0,
			exception);
	FYEH();

	context->sStringArray = fy_strCreatePermFromUTF8(block,
			"[L"FY_BASE_THROWABLE";", 0, exception);
	FYEH();

	context->sThrowablePrintStacktrace = fy_strCreatePermFromUTF8(block,
			FY_BASE_THROWABLE".printStackTrace.()V", 0, exception);
	FYEH();

	context->sThrowableDetailMessage = fy_strCreatePermFromUTF8(block,
			FY_BASE_THROWABLE".detailMessage.L"FY_BASE_STRING";", 0, exception);
	FYEH();

	context->sInit = fy_strCreatePermFromUTF8(block, FY_METHOD_INIT, 0,
			exception);
	FYEH();

	context->sClinit = fy_strCreatePermFromUTF8(block, FY_METHOD_CLINIT, 0,
			exception);
	FYEH();

	context->sFMain = fy_strCreatePermFromUTF8(block, FY_METHODF_MAIN, 0,
			exception);
	FYEH();

	context->sFRun = fy_strCreatePermFromUTF8(block, FY_METHODF_RUN, 0,
			exception);
	FYEH();

	context->sFName = fy_strCreatePermFromUTF8(block, FY_FIELDF_NAME, 0,
			exception);
	FYEH();

	context->sMFinalize = fy_strCreatePermFromUTF8(block, FY_METHODF_FINALIZE,
			0, exception);
	FYEH();

	context->sFPriority = fy_strCreatePermFromUTF8(block, FY_FIELDF_PRIORITY, 0,
			exception);
	FYEH();

	context->sFDaemon = fy_strCreatePermFromUTF8(block, FY_FIELDF_DAEMON, 0,
			exception);
	FYEH();

	context->sStringCount = fy_strCreatePermFromUTF8(block,
			FY_BASE_STRING".count.I", 0, exception);
	FYEH();

	context->sStringValue = fy_strCreatePermFromUTF8(block,
			FY_BASE_STRING".value.[C", 0, exception);
	FYEH();

	context->sStringOffset = fy_strCreatePermFromUTF8(block,
			FY_BASE_STRING".offset.I", 0, exception);
	FYEH();

	context->sStackTraceElement = fy_strCreatePermFromUTF8(block,
			FY_BASE_STACKTHREADELEMENT, 0, exception);
	FYEH();

	context->sStackTraceElementArray = fy_strCreatePermFromUTF8(block,
			"[L"FY_BASE_STACKTHREADELEMENT";", 0, exception);
	FYEH();

	context->sStackTraceElementDeclaringClass = fy_strCreatePermFromUTF8(block,
			FY_BASE_STACKTHREADELEMENT".declaringClass.L"FY_BASE_STRING";", 0,
			exception);
	FYEH();

	context->sStackTraceElementMethodName = fy_strCreatePermFromUTF8(block,
			FY_BASE_STACKTHREADELEMENT".methodName.L"FY_BASE_STRING";", 0,
			exception);
	FYEH();

	context->sStackTraceElementFileName = fy_strCreatePermFromUTF8(block,
			FY_BASE_STACKTHREADELEMENT".fileName.L"FY_BASE_STRING";", 0,
			exception);
	FYEH();

	context->sStackTraceElementLineNumber = fy_strCreatePermFromUTF8(block,
			FY_BASE_STACKTHREADELEMENT".lineNumber.I", 0, exception);
	FYEH();

	context->sThrowableStackTrace = fy_strCreatePermFromUTF8(block,
			FY_BASE_THROWABLE".stackTrace.[L"FY_BASE_STACKTHREADELEMENT";", 0,
			exception);
	FYEH();

	context->sArrayBoolean = fy_strCreatePermFromUTF8(block, "[Z", 0,
			exception);
	FYEH();

	context->sArrayChar = fy_strCreatePermFromUTF8(block, "[C", 0, exception);
	FYEH();

	context->sArrayFloat = fy_strCreatePermFromUTF8(block, "[F", 0, exception);
	FYEH();

	context->sArrayDouble = fy_strCreatePermFromUTF8(block, "[D", 0, exception);
	FYEH();

	context->sArrayByte = fy_strCreatePermFromUTF8(block, "[B", 0, exception);
	FYEH();

	context->sArrayShort = fy_strCreatePermFromUTF8(block, "[S", 0, exception);
	FYEH();

	context->sArrayInteger = fy_strCreatePermFromUTF8(block, "[I", 0,
			exception);
	FYEH();

	context->sArrayLong = fy_strCreatePermFromUTF8(block, "[J", 0, exception);
	FYEH();

	context->sArrayObject = fy_strCreatePermFromUTF8(block,
			"[L"FY_BASE_OBJECT";", 0, exception);
	FYEH();

	context->sArrayClass = fy_strCreatePermFromUTF8(block, "[L"FY_BASE_CLASS";",
			0, exception);
	FYEH();

	context->sValueBoolean = fy_strCreatePermFromUTF8(block, FY_VALUE_BOOLEAN,
			0, exception);
	FYEH();

	context->sValueByte = fy_strCreatePermFromUTF8(block, FY_VALUE_BYTE, 0,
			exception);
	FYEH();

	context->sValueChar = fy_strCreatePermFromUTF8(block, FY_VALUE_CHARACTER, 0,
			exception);
	FYEH();

	context->sValueShort = fy_strCreatePermFromUTF8(block, FY_VALUE_SHORT, 0,
			exception);
	FYEH();

	context->sValueInt = fy_strCreatePermFromUTF8(block, FY_VALUE_INTEGER, 0,
			exception);
	FYEH();

	context->sValueFloat = fy_strCreatePermFromUTF8(block, FY_VALUE_FLOAT, 0,
			exception);
	FYEH();

	context->sValueLong = fy_strCreatePermFromUTF8(block, FY_VALUE_LONG, 0,
			exception);
	FYEH();

	context->sValueDouble = fy_strCreatePermFromUTF8(block, FY_VALUE_DOUBLE, 0,
			exception);
	FYEH();

	context->sClassBoolean = fy_strCreatePermFromUTF8(block, FY_BASE_BOOLEAN, 0,
			exception);
	FYEH();

	context->sClassByte = fy_strCreatePermFromUTF8(block, FY_BASE_BYTE, 0,
			exception);
	FYEH();

	context->sClassChar = fy_strCreatePermFromUTF8(block, FY_BASE_CHAR, 0,
			exception);
	FYEH();

	context->sClassShort = fy_strCreatePermFromUTF8(block, FY_BASE_SHORT, 0,
			exception);
	FYEH();

	context->sClassInt = fy_strCreatePermFromUTF8(block, FY_BASE_INT, 0,
			exception);
	FYEH();

	context->sClassFloat = fy_strCreatePermFromUTF8(block, FY_BASE_FLOAT, 0,
			exception);
	FYEH();

	context->sClassLong = fy_strCreatePermFromUTF8(block, FY_BASE_LONG, 0,
			exception);
	FYEH();

	context->sClassDouble = fy_strCreatePermFromUTF8(block, FY_BASE_DOUBLE, 0,
			exception);
	FYEH();

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
	context->primitives[FY_TYPE_VOID] = context->sVoid;

	fy_hashMapInitPerm(block, context->mapPrimitivesRev, 19, exception);
	FYEH();

	cc = fy_mmAllocatePerm(block, sizeof(fy_char), exception);
	FYEH();
	*cc = FY_TYPE_BOOLEAN;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sBoolean, cc,
			exception);
	FYEH();

	cc = fy_mmAllocatePerm(block, sizeof(fy_char), exception);
	FYEH();
	*cc = FY_TYPE_BYTE;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sByte, cc,
			exception);
	FYEH();

	cc = fy_mmAllocatePerm(block, sizeof(fy_char), exception);
	FYEH();
	*cc = FY_TYPE_SHORT;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sShort, cc,
			exception);
	FYEH();

	cc = fy_mmAllocatePerm(block, sizeof(fy_char), exception);
	FYEH();
	*cc = FY_TYPE_CHAR;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sChar, cc,
			exception);
	FYEH();

	cc = fy_mmAllocatePerm(block, sizeof(fy_char), exception);
	FYEH();
	*cc = FY_TYPE_INT;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sInt, cc,
			exception);
	FYEH();

	cc = fy_mmAllocatePerm(block, sizeof(fy_char), exception);
	FYEH();
	*cc = FY_TYPE_FLOAT;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sFloat, cc,
			exception);
	FYEH();

	cc = fy_mmAllocatePerm(block, sizeof(fy_char), exception);
	FYEH();
	*cc = FY_TYPE_LONG;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sLong, cc,
			exception);
	FYEH();

	cc = fy_mmAllocatePerm(block, sizeof(fy_char), exception);
	FYEH();
	*cc = FY_TYPE_DOUBLE;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sDouble, cc,
			exception);
	FYEH();

	cc = fy_mmAllocatePerm(block, sizeof(fy_char), exception);
	FYEH();
	*cc = FY_TYPE_VOID;
	fy_hashMapPut(block, context->mapPrimitivesRev, context->sVoid, cc,
			exception);
	FYEH();
}

static void initStructClassloader(fy_context *context, fy_exception *exception) {
	fy_memblock *block = context->memblocks;
	fy_hashMapInitPerm(block, context->mapClassNameToId, 1024, exception);
	FYEH();

	fy_hashMapInitPerm(block, context->mapFieldNameToId, 4096, exception);
	FYEH();

	fy_hashMapInitPerm(block, context->mapMethodNameToId, 4096, exception);
	FYEH();

	fy_hashMapInitPerm(block, context->mapMUNameToNH, 1024, exception);
	FYEH();

	fy_hashMapIInitPerm(block, context->classObjIds, 7, -1, exception);
	FYEH();

	fy_hashMapIInitPerm(block, context->methodObjIds, 7, -1, exception);
	FYEH();

	fy_hashMapIInitPerm(block, context->fieldObjIds, 7, -1, exception);
	FYEH();

	fy_hashMapIInitPerm(block, context->constructorObjIds, 7, -1, exception);
	FYEH();

	fy_hashMapInitPerm(block, context->customClassData, 128, exception);
	FYEH();
}

static void initThreadManager(fy_context *context, fy_exception *exception) {
	context->nextThreadId = 1;

	context->runningThreads = fy_arrayListCreatePerm(context->memblocks,
			sizeof(fy_thread*), 128, exception);
	FYEH();

	context->state = FY_TM_STATE_NEW;

	context->pricmds[0] = 10;
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
	block->gcContext = context;
	block->gcProvider = fy_heapGC;

	fy_hashMapInitPerm(block, context->literals, 4096, exception);
	FYEH();

	fy_arrayListInit(block, context->toFinalize, sizeof(fy_uint), 256,
			exception);
	FYEH();

	fy_arrayListInit(block, context->protected, sizeof(fy_uint), 128,
			exception);
	FYEH();
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
	fy_debugInit(context);
	ILOG(
			context,
			"Initialing vm, context size=%d bytes including heap size=%d bytes,including object meta=%d bytes\n",
			(fy_int) sizeof(fy_context), (fy_int) sizeof(fy_memblock),
			MAX_OBJECTS * (fy_int) sizeof(fy_object))
;
	fy_fisInitInputStream(context);
	fy_bsRegisterBinarySaver(context);
	fy_mmInit(context->memblocks, exception);
	FYEH();
	fy_portInit(context->port);

	initHeap(context, exception);
	FYEH();

	initThreadManager(context, exception);
	FYEH();

	initConstantStrings(context, exception);
	FYEH();

	initConstantPrimitives(context, exception);
	FYEH();

	initStructClassloader(context, exception);
	FYEH();

	fy_arrayListInit(context->memblocks, context->switchTargets,
			sizeof(fy_switch_lookup*), 64, exception);

	fy_coreRegisterCoreHandlers(context, exception);
	fy_coreRegisterMathHandlers(context, exception);
	FYEH();
}

static fy_class* getClass(fy_context *context, fy_str *name) {
	int *pFid;
	fy_class *ret;
	pFid = fy_hashMapGet(context->memblocks, context->mapClassNameToId, name);
	if (pFid == NULL ) {
		return NULL ;
	}
	if (pFid == NULL || (ret = context->classes[*pFid]) == NULL ) {
		return NULL ;
	}
	return ret;
}
#ifdef FY_PROFILE
static void sortUsage(fy_profile_data *data, int length) {
	fy_boolean changed = TRUE;
	fy_profile_data tmp;
	fy_int i;
	while (changed) {
		changed = FALSE;
		for (i = 0; i < length - 1; i++) {
			if (data[i].count < data[i + 1].count) {
				changed = TRUE;
				tmp = data[i];
				data[i] = data[i + 1];
				data[i + 1] = tmp;
			}
		}
	}
}
#endif
void fy_vmContextDestroy(fy_context *context) {
#ifdef FY_PROFILE
	fy_uint i, imax;
	const char *c, *c2;
	imax = 256;
#endif
	context->logDVar(context, "Destroying vm\n");
#ifdef FY_PROFILE
	context->logDVar(context,"Op usage top 10:\n");
	for (i = 0; i < imax; i++) {
		context->opUsage[i].op1 = i;
	}
	sortUsage(context->opUsage, 256);
	for (i = 0; i < 10; i++) {
		if (context->opUsage[i].count) {
			c = FY_OP_NAME[context->opUsage[i].op1];
			if (c) {
				context->logDVar(context,"%s(%d) - %d\n", c, context->opUsage[i].op1,
						context->opUsage[i].count);
			} else {
				context->logDVar(context,"UNKNOWN(%d) - %d\n", context->opUsage[i].op1,
						context->opUsage[i].count);
			}
		}
	}
	context->logDVar(context,"Op combian usage top 16:\n");
	imax = 65536;
	for (i = 0; i < imax; i++) {
		context->opCombine[i].op1 = i >> 8;
		context->opCombine[i].op2 = i & 0xff;
	}
	sortUsage(context->opCombine, 65536);
	for (i = 0; i < 16; i++) {
		if (context->opUsage[i].count) {
			c = FY_OP_NAME[context->opCombine[i].op1];
			c2 = FY_OP_NAME[context->opCombine[i].op2];
			context->logDVar(context,"%s(%d)+%s(%d) - %d\n", c, context->opCombine[i].op1, c2,
					context->opCombine[i].op2, context->opCombine[i].count);
		}
	}
#endif
	fy_portDestroy(context->port);
	fy_mmDestroy(context->memblocks);
}

void fy_vmRegisterField(fy_context *context, fy_field *field,
		fy_exception *exception) {
	int *pFid;
	fy_memblock *block = context->memblocks;
	pFid = fy_hashMapGet(block, context->mapFieldNameToId, field->uniqueName);
	if (pFid == NULL ) {
		pFid = fy_mmAllocatePerm(block, sizeof(int), exception);
		FYEH();
		field->field_id = *pFid = context->fieldsCount++;
		fy_hashMapPut(block, context->mapFieldNameToId, field->uniqueName, pFid,
				exception);
		FYEH();
	}
	context->fields[*pFid] = field;
}

fy_field *fy_vmGetField(fy_context *context, fy_str *uniqueName) {
	int *pMid = fy_hashMapGet(context->memblocks, context->mapFieldNameToId,
			uniqueName);
	if (pMid == NULL ) {
		return NULL ;
	}
	return context->fields[*pMid];
}

fy_field *fy_vmLookupFieldVirtual(fy_context *context, fy_class *clazz,
		fy_str *fieldName, fy_exception *exception) {
	/*TODO Maybe wrong!!! need to check*/
	int *pFid, i, imax;
	fy_field *field = NULL;
	fy_str *uniqueName;
	fy_str *uniqueNameTmp;
	fy_memblock *block = context->memblocks;
	fy_class *intf;

	uniqueName = fy_mmAllocate(block, sizeof(fy_str), exception);
	FYEH()NULL;
	uniqueNameTmp = fy_mmAllocate(block, sizeof(fy_str), exception);
	FYEH()NULL;
	fy_strInit(block, uniqueName,
			clazz->className->length + fieldName->length + 1, exception);
	FYEH()NULL;
	fy_strInit(block, uniqueNameTmp,
			clazz->className->length + fieldName->length + 1, exception);
	FYEH()NULL;

	fy_strAppend(block, uniqueName, clazz->className, exception);
	FYEH()NULL;
	fy_strAppend(block, uniqueName, fieldName, exception);
	FYEH()NULL;
#ifdef FY_VERBOSE
	context->logDVar(context, "Looking up field ");
	context->logDStr(context, uniqueName);
	context->logDVarLn(context, "");
#endif

	imax = clazz->interfacesCount;
	for (i = 0; i < imax; i++) {
		intf = clazz->interfaces[i];
		fy_strClear(uniqueNameTmp);
		fy_strAppend(block, uniqueNameTmp, intf->className, exception);
		FYEH()NULL;
		fy_strAppend(block, uniqueNameTmp, fieldName, exception);
		FYEH()NULL;
#ifdef FY_VERBOSE
		context->logDVar(context, "++Trying ");
		context->logDStr(context, uniqueNameTmp);
#endif
		pFid = fy_hashMapGet(block, context->mapFieldNameToId, uniqueNameTmp);
		if (pFid != NULL ) {
#ifdef FY_VERBOSE
			context->logDVarLn(context, "...Succeed!");
#endif
			fy_hashMapPut(block, context->mapFieldNameToId, uniqueName, pFid,
					exception);
			FYEH()NULL;
			field = context->fields[*pFid];
			clazz = NULL;
			break;
		} else {
#ifdef FY_VERBOSE
			context->logDVarLn(context, "...");
#endif
		}
	}

	while (clazz != NULL ) {
		fy_strClear(uniqueNameTmp);
		fy_strAppend(block, uniqueNameTmp, clazz->className, exception);
		FYEH()NULL;
		fy_strAppend(block, uniqueNameTmp, fieldName, exception);
		FYEH()NULL;
#ifdef FY_VERBOSE
		context->logDVar(context, "++Trying ");
		context->logDStr(context, uniqueNameTmp);
#endif
		pFid = fy_hashMapGet(block, context->mapFieldNameToId, uniqueNameTmp);
		if (pFid != NULL ) {
#ifdef FY_VERBOSE
			context->logDVarLn(context, "...Succeed!");
#endif
			fy_hashMapPut(block, context->mapFieldNameToId, uniqueName, pFid,
					exception);
			FYEH()NULL;
			field = context->fields[*pFid];
			break;
		} else {
#ifdef FY_VERBOSE
			context->logDVarLn(context, "...");
#endif
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
	if (pMid == NULL ) {
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
	if (pMid == NULL ) {
		pMid = fy_mmAllocatePerm(block, sizeof(int), exception);
		FYEH();
		method->method_id = *pMid = context->methodsCount++;
		fy_hashMapPut(block, context->mapMethodNameToId, method->uniqueName,
				pMid, exception);
		FYEH();
	}
	context->methods[*pMid] = method;
}

fy_method *fy_vmGetMethod(fy_context *context, fy_str *uniqueName) {
	int *pMid = fy_hashMapGet(context->memblocks, context->mapMethodNameToId,
			uniqueName);
	if (pMid == NULL ) {
		return NULL ;
	}
	return context->methods[*pMid];
}

fy_method *fy_vmLookupMethodVirtualByMethod(fy_context *context,
		fy_class *clazz, fy_method *method, fy_exception *exception) {
	fy_method *actureMethod;
	fy_uint actureMethodId;
	char msg[256];
	actureMethodId = fy_hashMapIGet(context->memblocks, clazz->virtualTable,
			method->method_id);
	if (actureMethodId == -1) {
		actureMethod = fy_vmLookupMethodVirtual(context, clazz,
				method->fullName, exception);
		FYEH()NULL;

		if (actureMethod == NULL ) {
			fy_strSPrint(msg, 256, method->uniqueName);
			fy_fault(exception, FY_EXCEPTION_ABSTRACT, "%s", msg);
			return NULL ;
		}
		if (actureMethod->access_flags & FY_ACC_STATIC) {
			fy_strSPrint(msg, 256, method->uniqueName);
			fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "%s", msg);
			return NULL ;
		}
		if ((actureMethod->access_flags & FY_ACC_ABSTRACT)) {
			fy_strSPrint(msg, 256, method->uniqueName);
			fy_fault(exception, FY_EXCEPTION_ABSTRACT, "%s", msg);
			return NULL ;
		}
		fy_hashMapIPut(context->memblocks, clazz->virtualTable,
				method->method_id, actureMethod->method_id, exception);
		FYEH()NULL;
	} else {
		actureMethod = context->methods[actureMethodId];
	}
	return actureMethod;
}

fy_method *fy_vmLookupMethodVirtual(fy_context *context, fy_class *clazz,
		fy_str *methodName, fy_exception *exception) {
	int *pMid;
	fy_method *method = NULL;
	fy_memblock *block = context->memblocks;
	fy_str *uniqueName = fy_mmAllocate(block, sizeof(fy_str), exception);
	FYEH()NULL;
	fy_str *uniqueNameTmp = fy_mmAllocate(block, sizeof(fy_str), exception);
	FYEH()NULL;
	fy_strInit(block, uniqueName,
			clazz->className->length + methodName->length + 1, exception);
	FYEH()NULL;
	fy_strInit(block, uniqueNameTmp,
			clazz->className->length + methodName->length + 1, exception);
	FYEH()NULL;

	fy_strAppend(block, uniqueName, clazz->className, exception);
	FYEH()NULL;
	fy_strAppend(block, uniqueName, methodName, exception);
	FYEH()NULL;
	while (clazz != NULL ) {
		fy_strClear(uniqueNameTmp);
		fy_strAppend(block, uniqueNameTmp, clazz->className, exception);
		FYEH()NULL;
		fy_strAppend(block, uniqueNameTmp, methodName, exception);
		FYEH()NULL;
		pMid = fy_hashMapGet(block, context->mapMethodNameToId, uniqueNameTmp);
		if (pMid != NULL ) {
			fy_hashMapPut(block, context->mapMethodNameToId, uniqueName, pMid,
					exception);
			FYEH()NULL;
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
	if (pCid == NULL ) {
		pCid = fy_mmAllocatePerm(block, sizeof(int), exception);
		FYEH();
		*pCid = (context->classesCount++) + 1;
		if (*pCid >= MAX_CLASSES) {
			fy_fault(exception, NULL, "Too many classes!");
			return;
		}
		fy_hashMapPut(block, context->mapClassNameToId, clazz->className, pCid,
				exception);
		FYEH();
	}
	context->classes[*pCid] = clazz;
	clazz->classId = *pCid;
}

fy_class *fy_vmLoadClass(fy_context *context, fy_str *name,
		fy_exception *exception) {
	fy_class *clazz;
	clazz = getClass(context, name);
	if (clazz == NULL ) {
		clazz = fy_clLoadclass(context, name, exception);
		if (exception->exceptionType != exception_none) {
			return NULL ;
		}
		fy_vmRegisterClass(context, clazz, exception);
		FYEH()NULL;
		fy_clPhase2(context, clazz, exception);
		if (exception->exceptionType != exception_none) {
			return NULL ;
		}
		fy_vmLookupClass(context, context->sClassClass, exception);
		if (exception->exceptionType != exception_none) {
			return NULL ;
		}
	}
	return clazz;
}

void fy_vmDefineClass(fy_context *context, fy_str *name, fy_byte *data,
		fy_int dataLen, fy_exception *exception) {
	fy_classDefine *cd;
	cd = fy_hashMapGet(context->memblocks, context->customClassData, name);
	if (cd == NULL ) {
		cd = fy_mmAllocatePerm(context->memblocks, sizeof(fy_int) + dataLen,
				exception);
		FYEH();
		cd->size = dataLen;
		memcpy(cd->data, data, dataLen);
		fy_hashMapPut(context->memblocks, context->customClassData, name, cd,
				exception);
		FYEH();
	} else {
		fy_fault(exception,FY_EXCEPTION_INCOMPAT_CHANGE,"Class define dup.");
	}
}

/*Likes com.cirnoworks.fisce.vm.VMContext.getClass(String name)*/
fy_class *fy_vmLookupClass(fy_context *context, fy_str *name,
		fy_exception *exception) {
	return fy_vmLoadClass(context, name, exception);
}

fy_class *fy_vmGetClassFromClassObject(fy_context *context, fy_uint handle,
		fy_exception *exception) {
	fy_class *classClass;
	fy_class *inputClass;
	fy_int classId;
	inputClass = fy_heapGetClassOfObject(context, handle, exception);
	FYEH()NULL;
	classClass = fy_vmLookupClass(context, context->sClassClass, exception);
	if (exception->exceptionType != exception_none) {
		return NULL ;
	}
	if (inputClass != classClass) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				FY_EXCEPTION_VM);
		strcpy_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"Get class ID for non-class object");
		return NULL ;
	}
	classId = context->objects[handle].object_data->attachedId;
	return context->classes[classId];
}

fy_class *fy_vmLookupClassFromConstant(fy_context *context,
		ConstantClass *classInfo, fy_exception *exception) {
	if (classInfo->derefed == 0) {
		classInfo->ci.clazz = fy_vmLookupClass(context, classInfo->ci.className,
				exception);
		if (exception->exceptionType != exception_none) {
			return NULL ;
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
			return NULL ;
		}
		field = fy_vmLookupFieldVirtual(context, fieldInfo->clazz,
				fieldInfo->nameType, exception);
		FYEH()NULL;
		if (field == NULL ) {
			fy_fault(exception, NULL, "Field not found");
			return NULL ;
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
			return NULL ;
		}
		method = fy_vmLookupMethodVirtual(context, methodInfo->clazz,
				methodInfo->nameType, exception);
		FYEH()NULL;
		if (method == NULL ) {
			context->logEVar(context,"Method [");
			context->logEStr(context,methodInfo->clazz->className);
			context->logEStr(context,methodInfo->nameType);
			context->logEVarLn(context,"] not found!");
			fy_fault(exception, NULL, "Method not found");
			return NULL ;
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
	fy_str *str;
	fy_memblock *block = context->memblocks;
	str = fy_strCreatePermFromUTF8(block, name, 0, exception);
	FYEH();
	if (fy_hashMapGet(block, context->mapMUNameToNH, str) != NULL ) {
		fy_fault(exception, NULL, "Native handler conflict %s", name);
	}
	nh = fy_mmAllocatePerm(block, sizeof(fy_nh), exception);
	FYEH();
	nh->data = data;
	nh->handler = handler;
	fy_hashMapPut(block, context->mapMUNameToNH, str, nh, exception);
	FYEH();
}

void fy_vmUnRegisterNativeHandler(fy_context *context, const char *name,
		fy_exception *exception) {
	fy_nh* nh;
	fy_str str[1];
	fy_memblock *block = context->memblocks;
	str->content = NULL;
	fy_strInitWithUTF8(block, str, name, exception);
	FYEH();
	if ((nh = fy_hashMapGet(block, context->mapMUNameToNH, str)) != NULL ) {
		fy_hashMapPut(block, context->mapMUNameToNH, str, NULL, exception);
		/*fy_mmFree(block, nh);*/
	}
	fy_strDestroy(block, str);
}

fy_class *fy_vmLookupClassFromExceptionHandler(fy_context *context,
		fy_exceptionHandler *exceptionHandler, fy_exception *exception) {
	fy_class *clazz;
	if (exceptionHandler->catchTypeDerefed == 0) {
		clazz = fy_vmLookupClassFromConstant(context,
				exceptionHandler->ci.constantClass, exception);
		if (exception->exceptionType != exception_none) {
			return NULL ;
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
	FYEH();

	context->TOP_THROWABLE = fy_vmLookupClass(context, context->sClassThrowable,
			exception);
	FYEH();
	name.content = NULL;
	fy_strInitWithUTF8(context->memblocks, &name, bootStrapClass, exception);
	FYEH();
	clazz = fy_vmLookupClass(context, &name, exception);
	fy_strDestroy(context->memblocks, &name);
	FYEH();
	fy_tmBootFromMain(context, clazz, exception);
	FYEH();
}

static void fillValues(fy_str *key, void *value, void *addition) {
	**(int**) addition = *(int*) value;
	(*(int**) addition)++;
}

void fy_vmSave(fy_context *context, fy_exception *exception) {
	fy_uint i, imax, count, j, jmax, tmp;
	fy_class *clazz;
	fy_method *method;
	fy_field *field;
	fy_object *object;
	fy_uint *handles;
	fy_uint *temp;
	fy_thread *thread;
	fy_frame *frame;
	void *saver;
	/*Header*/
	saver = context->saveBegin(context, exception);
	FYEH();
	/*Class*/
	context->savePrepareClass(context, saver, imax = context->classesCount,
			exception);
	FYEH();
	imax = context->classesCount;
	for (i = 1; i <= imax; i++) {
		clazz = context->classes[i];
		tmp = fy_hashMapIGet(context->memblocks, context->classObjIds,
				clazz->classId);
		FYEH();
		context->saveClass(context, saver, i, tmp, clazz->clinitThreadId,
				clazz->className, clazz->staticSize, clazz->staticArea,
				exception);
		FYEH();
	}/**/
	context->saveEndClass(context, saver, exception);
	FYEH();
	/*Method*/
	context->savePrepareMethod(context, saver, imax = context->methodsCount,
			exception);
	FYEH();
	for (i = 0; i < imax; i++) {
		method = context->methods[i];
		context->saveMethod(context, saver, i, 0/*TODO*/, method->uniqueName,
				exception);
		FYEH();
	}/**/
	context->saveEndMethod(context, saver, exception);
	FYEH();
	/*Field*/
	context->savePrepareField(context, saver, imax = context->fieldsCount,
			exception);
	FYEH();
	for (i = 0; i < imax; i++) {
		field = context->fields[i];
		context->saveField(context, saver, i, 0/*TODO*/, field->uniqueName,
				exception);
		FYEH();
	}/**/
	context->saveEndField(context, saver, exception);
	FYEH();
	/*Objects*/
	count = 0;
	for (i = 1; i < MAX_OBJECTS; i++) {
		if (fy_heapGetObject(context,i)->object_data != NULL ) {
			count++;
		}
	}
	context->savePrepareObjects(context, saver, context->nextHandle, count,
			exception);
	FYEH();
	for (i = 1; i < MAX_OBJECTS; i++) {
		if ((object = fy_heapGetObject(context,i))->object_data != NULL) {
			context->saveObject(context, saver, i,
					object->object_data->clazz->classId,
					object->object_data->position, object->object_data->gen,
					object->object_data->finalizeStatus,
					object->object_data->monitorOwnerId,
					object->object_data->monitorOwnerTimes,
					object->object_data->attachedId,
					object->object_data->length,
					object->object_data->clazz->type == array_class ?
							fy_heapGetArraySizeFromLength(
									object->object_data->clazz,
									object->object_data->length) :
							object->object_data->clazz->sizeAbs,
					(void*) object->object_data->data, exception);
			count++;
		}
	}/**/
	context->saveEndObject(context, saver, exception);
	FYEH();
	/*Literals*/
	handles = /*TEMP*/fy_allocate(context->literals->size * sizeof(fy_uint),
			exception);
	FYEH();
	temp = handles;
	fy_hashMapEachValue(context->memblocks, context->literals, fillValues,
			&temp);
	context->saveLiterals(context, saver, context->literals->size, handles,
			exception);
	fy_free(handles);
	FYEH();
	context->saveFinalizes(context, saver, context->toFinalize->length,
			context->toFinalize->data, exception);
	FYEH();
	context->savePrepareThreads(context, saver,
			imax = context->runningThreads->length, exception);
	FYEH();
	for (i = 0; i < imax; i++) {
		fy_arrayListGet(context->memblocks, context->runningThreads, i,
				&thread);
		jmax = thread->frameCount;
		context->saveThread(context, saver, thread->threadId, thread->handle,
				thread->priority, thread->daemon, thread->destroyPending,
				thread->interrupted, thread->nextWakeTime,
				thread->pendingLockCount, thread->waitForLockId,
				thread->waitForNotifyId, FY_GET_FRAME(thread,jmax-1) ->sp,
				thread->stack, thread->typeStack, exception);
		FYEH();
		context->savePrepareFrame(context, saver, jmax, exception);
		FYEH();
		for (j = 0; j < jmax; j++) {
			frame = FY_GET_FRAME(thread,j);
			context->saveFrame(context, saver, frame->methodId, frame->sb,
					frame->sp, frame->pc, frame->lpc, exception);
			FYEH();
		}
		context->saveEndFrame(context, saver, exception);
	}
	context->saveEndThread(context, saver, exception);
	context->saveEnd(context, saver, exception);
}

void fy_vmBootFromData(fy_context *context, fy_exception *exception) {

	context->loadData(context, exception);
	context->TOP_CLASS = fy_vmLookupClass(context, context->sTopClass,
			exception);
	FYEH();

	context->TOP_THROWABLE = fy_vmLookupClass(context, context->sClassThrowable,
			exception);
	FYEH();
	context->state = FY_TM_STATE_RUN_PENDING;
	context->nextGCTime = fy_portTimeMillSec(context->port) + FY_GC_IDV;
	context->nextForceGCTime = context->nextGCTime + FY_GC_FORCE_IDV;
}

fy_int fy_vmGetClassObjHandle(fy_context *context, fy_class *clazz,
		fy_exception *exception) {
	fy_class *clcl = fy_vmLookupClass(context, context->sClassClass, exception);
	fy_int handle = fy_hashMapIGet(context->memblocks, context->classObjIds,
			clazz->classId);
	FYEH()-1;
	if (handle == -1) {
		fy_heapBeginProtect(context);
		handle = fy_heapAllocate(context, clcl, exception);
		fy_heapGetObject(context,handle)->object_data->attachedId =
				clazz->classId;
		fy_hashMapIPut(context->memblocks, context->classObjIds, clazz->classId,
				handle, exception);
		//FYEH is unnessessery
	}
	return handle;
}

fy_int fy_vmGetMethodObjHandle(fy_context *context, fy_method *method,
		fy_exception *exception) {
	fy_class *mecl = fy_vmLookupClass(context, context->sClassMethod,
			exception);
	fy_class *cocl = fy_vmLookupClass(context, context->sClassConstructor,
			exception);
	fy_int handle = fy_hashMapIGet(context->memblocks, context->methodObjIds,
			method->method_id);
	FYEH()-1;
	if (handle == -1) {
		fy_heapBeginProtect(context);
		if (method->access_flags & FY_ACC_CONSTRUCTOR) {
			handle = fy_heapAllocate(context, cocl, exception);
		} else {
			handle = fy_heapAllocate(context, mecl, exception);
		}
		FYEH()-1;
		fy_heapGetObject(context,handle)->object_data->attachedId =
				method->method_id;
		fy_hashMapIPut(context->memblocks, context->methodObjIds,
				method->method_id, handle, exception);
	}
	return handle;
}

fy_int fy_vmGetFieldObjHandle(fy_context *context, fy_field *field,
		fy_exception *exception) {
	fy_class *ficl = fy_vmLookupClass(context, context->sClassField, exception);
	fy_int handle = fy_hashMapIGet(context->memblocks, context->fieldObjIds,
			field->field_id);
	FYEH()-1;
	if (handle == -1) {
		fy_heapBeginProtect(context);
		handle = fy_heapAllocate(context, ficl, exception);
		fy_heapGetObject(context,handle)->object_data->attachedId =
				field->field_id;
		fy_hashMapIPut(context->memblocks, context->fieldObjIds,
				field->field_id, handle, exception);
	}
	return handle;
}


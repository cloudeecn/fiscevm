/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
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

#ifndef FY_VMCONTEXT_H_
#define FY_VMCONTEXT_H_

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fy_util/LnkList.h"
#include "fy_util/MemMan.h"
#include "fy_util/HashMap.h"
#include "fyc/typedefs.h"
#include "fyc/ClassStruct.h"
#include "fyc/typedefs.h"

#ifdef	__cplusplus
extern "C" {
#endif

struct fy_context {
	/*Service Function Table*/
	/*INPUTSTREAM*/
	fy_inputStream* (*isOpen)(struct fy_context *context, const char *name,
			fy_exception *exception);
	const void *isParam;

	/*ResourceInputStream*/
	fy_inputStream* aliveStreams[MAX_STREAMS];

	/*Status Saver*/
	const void *saveloadParam;
	void* (*saveBegin)(struct fy_context *context, fy_exception *exception);
	void (*savePrepareClass)(struct fy_context *context, void *saver,
			fy_uint classCount, fy_exception *exception);
	void (*saveClass)(struct fy_context *context, void *saver, fy_uint classId,
			fy_uint handle, fy_int clinited, fy_str *name, fy_uint staticSize,
			fy_int *staticArea, fy_exception *exception);
	void (*saveEndClass)(struct fy_context *context, void *saver,
			fy_exception *exception);
	void (*savePrepareMethod)(struct fy_context *context, void *saver,
			fy_uint methodCount, fy_exception *exception);
	void (*saveMethod)(struct fy_context *context, void *saver,
			fy_uint methodId, fy_uint handle, fy_str *uniqueName,
			fy_exception *exception);
	void (*saveEndMethod)(struct fy_context *context, void *saver,
			fy_exception *exception);
	void (*savePrepareField)(struct fy_context *context, void *saver,
			fy_uint fieldCount, fy_exception *exception);
	void (*saveField)(struct fy_context *context, void *saver, fy_uint fieldId,
			fy_uint handle, fy_str *uniqueName, fy_exception *exception);
	void (*saveEndField)(struct fy_context *context, void *saver,
			fy_exception *exception);
	void (*savePrepareObjects)(struct fy_context *context, void *saver,
			fy_uint nextHandle, fy_uint objectCount, fy_exception *exception);
	void (*saveObject)(struct fy_context *context, void *saver, fy_uint handle,
			fy_uint classId, fy_int posInHeap, fy_int gen,
			fy_int finalizeStatus, fy_uint monitorOwner, fy_uint monitorCount,
			fy_uint multiUsageData, fy_uint dataLength, fy_uint *data,
			fy_exception *exception);
	void (*saveEndObject)(struct fy_context *context, void *saver,
			fy_exception *exception);
	void (*saveLiterals)(struct fy_context *context, void *saver, fy_uint count,
			fy_uint *handles, fy_exception *exception);
	void (*saveFinalizes)(struct fy_context *context, void *saver,
			fy_uint count, fy_uint *handles, fy_exception *exception);
	void (*savePrepareThreads)(struct fy_context *context, void *saver,
			fy_uint threadsCount, fy_exception *exception);
	void (*saveThread)(struct fy_context *context, void *saver,
			fy_uint threadId, fy_uint handle, fy_int priority, fy_uint daemon,
			fy_uint destroyPending, fy_uint interrupted, fy_long nextWakeupTime,
			fy_uint pendingLockCount, fy_uint waitForLockId,
			fy_uint waitForNotifyId, fy_uint stackSize, fy_stack_item *stack, fy_exception *exception);
	void (*savePrepareFrame)(struct fy_context *context, void *saver,
			fy_uint count, fy_exception *exception);
	void (*saveFrame)(struct fy_context *context, void *saver, fy_uint methodId,
			fy_uint sb, fy_uint lpc, fy_int pcofs,
			fy_exception *exception);
	void (*saveEndFrame)(struct fy_context *context, void *saver,
			fy_exception *exception);
	void (*saveEndThread)(struct fy_context *context, void *saver,
			fy_exception *exception);
	void (*saveEnd)(struct fy_context *context, void *saver,
			fy_exception *exception);
	void (*loadData)(struct fy_context *context, fy_exception *exception);

	/*Logging*/

	void (*logEStr)(struct fy_context *context, const fy_str *str);
	void (*logEVar)(struct fy_context *context, const char *format, ...);
	void (*logEVarLn)(struct fy_context *context, const char *format, ...);

	void (*logWStr)(struct fy_context *context, const fy_str *str);
	void (*logWVar)(struct fy_context *context, const char *format, ...);
	void (*logWVarLn)(struct fy_context *context, const char *format, ...);

	void (*logIStr)(struct fy_context *context, const fy_str *str);
	void (*logIVar)(struct fy_context *context, const char *format, ...);
	void (*logIVarLn)(struct fy_context *context, const char *format, ...);

	void (*logDStr)(struct fy_context *context, const fy_str *str);
	void (*logDVar)(struct fy_context *context, const char *format, ...);
	void (*logDVarLn)(struct fy_context *context, const char *format, ...);

	/*Finish function pointers*/

	void *additionalData;
	fy_boolean loading;

	fy_str *sAttCode;
	fy_str *sAttLineNum;
	fy_str *sAttStackMapTable;
	fy_str *sAttSynth;
	fy_str *sAttSourceFile;
	fy_str *sAttConstantValue;

	fy_str *sTopClass;
	fy_str *sClassClass;
	fy_str *sClassField;
	fy_str *sClassMethod;
	fy_str *sClassConstructor;
	fy_str *sClassThrowable;
	fy_str *sClassBoolean;
	fy_str *sClassByte;
	fy_str *sClassShort;
	fy_str *sClassChar;
	fy_str *sClassInt;
	fy_str *sClassFloat;
	fy_str *sClassLong;
	fy_str *sClassDouble;
	fy_str *sBoolean;
	fy_str *sByte;
	fy_str *sShort;
	fy_str *sChar;
	fy_str *sInt;
	fy_str *sFloat;
	fy_str *sLong;
	fy_str *sDouble;
	fy_str *sVoid;
	fy_str *sString;
	fy_str *sThread;
	fy_str *sStringArray;
	fy_str *sThrowablePrintStacktrace;
	fy_str *sInit;
	fy_str *sClinit;
	fy_str *sFMain;
	fy_str *sFRun;
	fy_str *sFPriority;
	fy_str *sFName;
	fy_str *sFDaemon;
	fy_str *sMFinalize;
	fy_str *sStringValue;
	fy_str *sStringOffset;
	fy_str *sStringCount;
	fy_str *sEnum;
	fy_str *sAnnotation;
	fy_str *sPhantomReference;
	fy_str *sSoftReference;
	fy_str *sWeakReference;

	fy_str *sArrayBoolean;
	fy_str *sArrayChar;
	fy_str *sArrayFloat;
	fy_str *sArrayDouble;
	fy_str *sArrayByte;
	fy_str *sArrayShort;
	fy_str *sArrayInteger;
	fy_str *sArrayLong;
	fy_str *sArrayObject;
	fy_str *sArrayClass;

	fy_str *sThrowableStackTrace;
	fy_str *sThrowableDetailMessage;
	fy_str *sStackTraceElement;
	fy_str *sStackTraceElementArray;
	fy_str *sStackTraceElementDeclaringClass;
	fy_str *sStackTraceElementMethodName;
	fy_str *sStackTraceElementFileName;
	fy_str *sStackTraceElementLineNumber;

	fy_str *sValueBoolean;
	fy_str *sValueByte;
	fy_str *sValueShort;
	fy_str *sValueChar;
	fy_str *sValueInt;
	fy_str *sValueFloat;
	fy_str *sValueLong;
	fy_str *sValueDouble;

	fy_class *TOP_THROWABLE;
	fy_class *TOP_CLASS;
	fy_class *TOP_ENUM;
	fy_class *TOP_ANNOTATION;
	fy_class *TOP_WEAK_REF;
	fy_class *TOP_SOFT_REF;
	fy_class *TOP_PHANTOM_REF;

	fy_str *primitives[128];
	fy_hashMap mapPrimitivesRev[1];

	fy_memblock memblocks[1];
	fy_port port[1];

	fy_int classesCount;
	fy_class *classes[MAX_CLASSES];

	fy_hashMap mapClassNameToId[1];

	fy_int methodsCount;
	fy_method *methods[MAX_METHODS];
	fy_hashMap mapMethodNameToId[1];
	fy_hashMap stackPool[1];

	fy_int fieldsCount;
	fy_field *fields[MAX_FIELDS];
	fy_hashMap mapFieldNameToId[1];

	fy_hashMap mapMUNameToNH[1];

	fy_hashMapI classObjIds[1];
	fy_hashMapI methodObjIds[1];
	fy_hashMapI fieldObjIds[1];
	fy_hashMapI constructorObjIds[1];

	void *gcCustomData;
	void (*beforeGC)(void *data);
	void (*getExtraGCKeep)(void *data, fy_int *count, fy_int **content);
	void (*afterGC)(void *data);

	fy_hashMap customClassData[1];

	/* #BEGIN GLOBAL*/
#ifdef FY_DEBUG
	fy_int stringPoolTimes;
#endif
	fy_arrayList switchTargets[1];
	fy_hashMap stringPool[1];
	/* #BEGIN THREAD MANAGER*/
	fy_int pricmds[11];
	fy_thread *threads[MAX_THREADS];
	fy_thread *currentThread;
	fy_arrayList *runningThreads;
	fy_int runningThreadPos;
	fy_int run;
	fy_int state;
	fy_long nextWakeUpTimeTotal;
	fy_int nextThreadId;
	fy_exception exitException;
	fy_int exitCode;
	fy_long nextGCTime;
	fy_long nextForceGCTime;
	fy_int engineCount;
	fy_engine *engines;
	/* #END THREAD MANAGER*/

	/* #BEGIN HEAP*/
	fy_arrayList toFinalize[1];
	fy_boolean protectMode;
	fy_arrayList protected[1];
	fy_hashMap literals[1];
	fy_hashMapI references[1];
	fy_arrayList toEnqueue[1];
	fy_uint nextHandle;
	fy_uint totalObjects;
	fy_object objects[MAX_OBJECTS];
	fy_int END_MARK[1];
	/* #END HEAP*/
};

void fy_vmContextInit(fy_context *context, fy_exception *exception);
void fy_vmContextDestroy(fy_context *context);

fy_boolean fy_vmBootup(fy_context *context, const char* bootStrapClass,
		fy_exception *exception);

void fy_vmSave(fy_context *context, fy_exception *exception);
void fy_vmBootFromData(fy_context *context, fy_exception *exception);

void fy_vmRegisterField(fy_context *context, fy_field *field,
		fy_exception *exception);
fy_field *fy_vmLookupFieldVirtual(fy_context *context, fy_class *clazz,
		fy_str *name, fy_exception *exception);
fy_field *fy_vmLookupFieldFromConstant(fy_context *context,
		ConstantFieldRef *fieldInfo, fy_exception *exception);

fy_field *fy_vmGetField(fy_context *context, fy_str *uniqueName);

void fy_vmRegisterMethod(fy_context *context, fy_method *method,
		fy_exception *exception);
fy_method *fy_vmLookupMethodVirtual(fy_context *context, fy_class *clazz,
		fy_str *name, fy_exception *exception);
fy_method *fy_vmLookupMethodVirtualByMethod(fy_context *context,
		fy_class *clazz, fy_method *method, fy_exception *exception);
fy_method *fy_vmLookupMethodFromConstant(fy_context *context,
		ConstantMethodRef *methodInfo, fy_exception *exception);

fy_method *fy_vmGetMethod(fy_context *context, fy_str *uniqueName);

void fy_vmRegisterClass(fy_context *context, fy_class *clazz,
		fy_exception *exception);
fy_class *fy_vmLoadClass(fy_context *context, fy_str *name,
		fy_exception *exception);
fy_class *fy_vmLookupClass(fy_context *context, fy_str *name,
		fy_exception *exception);
fy_class *fy_vmLookupClassFromConstant(fy_context *context,
		ConstantClass *classInfo, fy_exception *exception);

void fy_vmRegisterNativeHandler(fy_context *context, const char *name,
		void *data, fy_nhFunction handler, fy_exception *exception);
void fy_vmUnRegisterNativeHandler(fy_context *context, const char *name,
		fy_exception *exception);
fy_class *fy_vmGetClassFromClassObject(fy_context *context, fy_uint handle,
		fy_exception *exception);
fy_class *fy_vmLookupClassFromExceptionHandler(fy_context *context,
		fy_exceptionHandler *exceptionHandler, fy_exception *exception);
int fy_vmGetClassObjHandle(fy_context *context, fy_class *clazz,
		fy_exception *exception);
fy_int fy_vmGetMethodObjHandle(fy_context *context, fy_method *method,
		fy_exception *exception);
fy_int fy_vmGetFieldObjHandle(fy_context *context, fy_field *field,
		fy_exception *exception);

/*String pool*/
fy_str *fy_vmCreateStringByPool(fy_context *context, fy_str *tmp,
		fy_exception *exception);
fy_str *fy_vmCreateStringByPoolV(fy_context *context, fy_exception *exception,
		const char *pattern, ...);
fy_str *fy_vmCreateStringByPoolVA(fy_context *context, fy_strVA *va,
		fy_exception *exception);
#ifdef	__cplusplus
}
#endif
#endif /* FY_VMCONTEXT_H_ */

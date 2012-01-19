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
#include "fisceprt.h"
#include "fisceclz.h"
#include "fy_util/LnkList.h"
#include "fy_util/MemMan.h"
#include "fy_util/String.h"
#include "fy_util/HashMap.h"
#include "fy_util/ArrList.h"
#ifndef FISCESTU_H_
#define FISCESTU_H_

#define MAX_CLASSES 4096
#define MAX_METHODS 65536
#define MAX_FIELDS 65536
#define MAX_OBJECTS 131072
#define MAX_THREADS 256
#define EDEN_SIZE 131072
#define COPY_SIZE 32768
#define OLD_ENTRIES 1048576
#define STACK_SIZE 16384
#define MAX_FRAMES 256

/*Bellow are used by context*/
#define FY_TYPE_BYTE  'B'
#define FY_TYPE_CHAR  'C'
#define FY_TYPE_DOUBLE  'D'
#define FY_TYPE_FLOAT  'F'
#define FY_TYPE_LONG  'J'
#define FY_TYPE_SHORT  'S'
#define FY_TYPE_BOOLEAN  'Z'
#define FY_TYPE_ARRAY  '['
/*Below are shared by thread and context*/
#define FY_TYPE_INT  'I'
#define FY_TYPE_HANDLE  'L'
/*Below are used only by thread*/
#define FY_TYPE_WIDE  'W'
#define FY_TYPE_RETURN  'R'
#define FY_TYPE_WIDE2  '_'
#define FY_TYPE_UNKNOWN  'X'

#define FY_METHOD_INIT "<init>"
#define FY_METHOD_CLINIT "<clinit>"
#define FY_METHODF_MAIN ".main.([L"FY_BASE_STRING";)V"
#define FY_METHODF_RUN ".run.()V"
#define FY_FIELDF_PRIORITY ".priority.I"
#define FY_FIELDF_NAME ".name.[C"
#define FY_FIELDF_DAEMON ".daemon.Z"

#define FY_ACC_ABSTRACT 1024
#define FY_ACC_FINAL 16
#define FY_ACC_INTERFACE 512
#define FY_ACC_NATIVE 256
#define FY_ACC_PRIVATE 2
#define FY_ACC_PROTECTED 4
#define FY_ACC_PUBLIC 1
#define FY_ACC_STATIC 8
#define FY_ACC_STRICT 2048
#define FY_ACC_SUPER 32
#define FY_ACC_SYNCHRONIZED 32
#define FY_ACC_TRANSIENT 128
#define FY_ACC_VOLATILE 64

#define FY_TM_STATE_NEW  0
#define FY_TM_STATE_BOOT_PENDING  1
#define FY_TM_STATE_STOP  2
#define FY_TM_STATE_RUN_PENDING  3
#define FY_TM_STATE_RUNNING  4
#define FY_TM_STATE_STOP_PENDING  5
#define FY_TM_STATE_DEAD_PENDING  6
#define FY_TM_STATE_DEAD  7

#ifdef	__cplusplus
extern "C" {
#endif

typedef union stringInfo {
	fy_char string_index;
	fy_str* string;
	fy_int handle;
} stringInfo;

typedef union classInfo {
	fy_char name_index;
	fy_str* className;
	struct ConstantClass *constantClass;
	struct fy_class *clazz;
} classInfo;

typedef struct ConstantClass {

	fy_ubyte derefed;

	classInfo ci;
} ConstantClass;

typedef struct ConstantFieldRef {

	fy_ubyte derefed;

	fy_char class_index;
	ConstantClass *constantClass;
	struct fy_class *clazz;

	fy_char name_type_index;
	struct ConstantNameAndTypeInfo *constantNameType;
	fy_str* nameType;

	struct fy_field *field;
} ConstantFieldRef;

typedef struct ConstantMethodRef {

	fy_ubyte derefed;

	fy_char class_index;
	ConstantClass *constantClass;
	struct fy_class *clazz;

	fy_char name_type_index;
	struct ConstantNameAndTypeInfo *constantNameType;
	fy_str* nameType;

	/*The orignal method, not overridden.*/
	struct fy_method *method;
} ConstantMethodRef;

typedef struct ConstantStringInfo {

	fy_ubyte derefed;

	stringInfo ci;
} ConstantStringInfo;

typedef struct ConstantIntegerFloatInfo {
	fy_int value;
} ConstantIntegerFloatInfo;

typedef struct ConstantLongDoubleInfo {
	fy_long value;
} ConstantLongDoubleInfo;

typedef struct ConstantNameAndTypeInfo {

	fy_char name_index;
	fy_str* name;

	fy_char descriptor_index;
	fy_str* descriptor;
} ConstantNameAndTypeInfo;

typedef struct ConstantUtf8Info {

	fy_str* string;
} ConstantUtf8Info;

typedef struct fy_field {
	fy_uint field_id;
	fy_char access_flags;

	fy_char name_index;
	fy_str* name;

	fy_char descriptor_index;
	fy_str* descriptor;

	fy_str* fullName;
	fy_str* uniqueName;

	struct fy_class* owner;

	fy_uint posRel;
	fy_uint posAbs;

	fy_uint initHigh;
	fy_uint initLow;

} fy_field;

typedef struct fy_lineNumber {
	fy_char start_pc;
	fy_char line_number;
} fy_lineNumber;

typedef struct fy_exceptionHandler {
	fy_char start_pc;
	fy_char end_pc;
	fy_char handler_pc;
	fy_ubyte catchTypeDerefed;

	classInfo ci;
} fy_exceptionHandler;
struct fy_nh;
typedef struct fy_method {
	fy_int method_id;
	fy_char access_flags;

	fy_str* name;

	fy_str* descriptor;

	fy_str* fullName;
	fy_str* uniqueName;

	struct fy_class* owner;

	fy_ubyte synthetic;

	fy_char max_stack;
	fy_char max_locals;

	fy_uint codeLength;
	union {
		fy_ubyte *code;
		struct fy_nh *nh;
	};
	fy_char exception_table_length;
	struct fy_exceptionHandler *exception_table;

	fy_char line_number_table_length;
	struct fy_lineNumber* line_number_table;

	fy_int paramCount;
	fy_byte *paramTypes;
	fy_byte returnType;

	fy_boolean clinit;
} fy_method;

typedef enum fy_arrayType {
	fy_at_byte, fy_at_short, fy_at_int, fy_at_long
} fy_arrayType;

typedef struct fy_class {
	/*        _u4 magic;
	 //        _u2 minorVersion;
	 //        _u2 majorVersion;
	 //fields from file
	 *
	 */
	enum type {
		obj, prm, arr
	} type;
	fy_ubyte phase;
	fy_ubyte needFinalize;
	fy_char constantPoolCount;
	fy_ubyte *constantTypes;
	void** constantPools;

	fy_char accessFlags;
	ConstantClass* thisClass;
	fy_str* className;
	ConstantClass* superClass;
	struct fy_class* super;
	fy_char interfacesCount;
	struct fy_class** interfaces;
	fy_char fieldCount;
	fy_field** fields;
	/*BEGIN GC Only*/
	fy_field **fieldStatic;
	fy_field **fieldAbs;
	/* END  GC Only*/
	fy_char methodCount;
	fy_method** methods;
	fy_method* clinit;
	fy_str* sourceFile;
	int classId;

	/*fields filled by classloader*/
	fy_uint sizeRel;
	fy_uint sizeAbs;
	fy_uint ofsInHeap;

	fy_uint staticSize;
	fy_int *staticArea;

	union {
		struct {
			fy_arrayType arrayType;
			struct fy_class *contentClass;
		} arr;
		struct {
			fy_char pType;
		} prm;
	} ci;

	/*Need persist*/
	fy_uint classObjId;
	fy_int clinitThreadId;
} fy_class;

typedef struct fy_object {
	fy_class *clazz;
	fy_int length;
	enum {
		eden = 0, young, old
	} position :2;
	enum {
		not_finalized, in_finalize_array, finalized
	} finalizeStatus :2;
	fy_int gen :8;
	fy_uint monitorOwnerId;
	fy_int monitorOwnerTimes;
	fy_uint attachedId;
	void *data;
/*
 union fy_object_data {
 fy_ulong *ldata;
 fy_uint *idata;
 fy_ubyte *bdata;
 fy_char *cdata;
 } data;
 */

} fy_object;

typedef struct fy_frame {
	fy_method *method;
	fy_ubyte *code;
	fy_uint sb;
#ifdef FY_STRICT_CHECK
	fy_uint size;
	fy_uint localCount;
	fy_uint codeSize;
#endif
	fy_uint methodId;
	fy_uint sp;
	fy_uint pc;
	fy_uint lpc;
} fy_frame;

typedef struct fy_thread {
	fy_boolean yield;

	fy_uint handle;
	fy_uint currentThrowable;
	fy_int status;
	fy_int priority;
	fy_int threadId;

	fy_uint frameCount;
	fy_frame frames[MAX_FRAMES];
	fy_uint stack[STACK_SIZE];
	fy_uint typeStack[STACK_SIZE];

	/*Used by thread manager*/
	fy_int waitForLockId;
	fy_int waitForNotifyId;
	fy_int pendingLockCount;
	fy_long nextWakeTime;
	fy_boolean interrupted;
	fy_boolean daemon;
	fy_boolean destroyPending;

} fy_thread;

typedef struct fy_nativeCall {
	fy_method *method;
	fy_uint paramCount;
	fy_uint *params;
} fy_nativeCall;

typedef enum fy_messageType {
	message_continue = 0, /*In thread*/
	message_none = 1, /*Thread Only*/
	message_thread_dead = 2, /*Thread Only*/
	message_invoke_native = 3,/*Thread And TM pass thread*/
	message_exception = 4, /*Thread And TM pass thread*/
	message_sleep = 5, /*TM Only*/
	message_vm_dead = 6
/*TM Only*/
} fy_messageType;

typedef struct fy_message {
	fy_messageType messageType;
	fy_thread *thread;
	union {
		fy_nativeCall call;
		fy_exception exception;
		fy_long sleepTime;
	} body;

} fy_message;
struct fy_context;

typedef struct fy_context {
	void *additionalData;

	fy_str sTopClass[1];
	fy_str sClassClass[1];
	fy_str sClassThrowable[1];
	fy_str sBoolean[1];
	fy_str sByte[1];
	fy_str sShort[1];
	fy_str sChar[1];
	fy_str sInt[1];
	fy_str sFloat[1];
	fy_str sLong[1];
	fy_str sDouble[1];
	fy_str sString[1];
	fy_str sThread[1];
	fy_str sStringArray[1];
	fy_str sThrowablePrintStacktrace[1];
	fy_str sInit[1];
	fy_str sClinit[1];
	fy_str sFMain[1];
	fy_str sFRun[1];
	fy_str sFPriority[1];
	fy_str sFName[1];
	fy_str sFDaemon[1];
	fy_str sStringValue[1];
	fy_str sStringOffset[1];
	fy_str sStringCount[1];

	fy_str sArrayBoolean[1];
	fy_str sArrayChar[1];
	fy_str sArrayFloat[1];
	fy_str sArrayDouble[1];
	fy_str sArrayByte[1];
	fy_str sArrayShort[1];
	fy_str sArrayInteger[1];
	fy_str sArrayLong[1];
	fy_str sArrayObject[1];

	fy_str sThrowableStackTrace[1];
	fy_str sThrowableDetailMessage[1];
	fy_str sStackTraceElement[1];
	fy_str sStackTraceElementArray[1];
	fy_str sStackTraceElementDeclaringClass[1];
	fy_str sStackTraceElementMethodName[1];
	fy_str sStackTraceElementFileName[1];
	fy_str sStackTraceElementLineNumber[1];

	struct fy_class *TOP_THROWABLE;
	struct fy_class *TOP_CLASS;

	struct fy_str *primitives[128];
	struct fy_hashMap mapPrimitivesRev[1];

	fy_memblock memblocks[1];
	fy_port port[1];

	int classesCount;
	struct fy_class *classes[MAX_CLASSES];

	struct fy_hashMap mapClassNameToId[1];

	int methodsCount;
	struct fy_method *methods[MAX_METHODS];
	struct fy_hashMap mapMethodNameToId[1];

	int fieldsCount;
	struct fy_field *fields[MAX_FIELDS];
	struct fy_hashMap mapFieldNameToId[1];

	struct fy_hashMap mapMUNameToNH[1];

	/* #BEGIN HEAP*/
	fy_hashMap literals[1];
	fy_uint nextHandle;
	fy_object objects[MAX_OBJECTS];
	fy_int posInEden;
	fy_uint eden[EDEN_SIZE];
	fy_int posInYong;
	fy_uint youngId;
	fy_uint young[COPY_SIZE * 2];
	fy_int posInOld;
	fy_int oldReleasedSize;
	fy_uint old[OLD_ENTRIES];
	fy_arrayList toFinalize[1];
	fy_boolean protectMode;
	fy_arrayList protected[1];
	/* #END HEAP*/

	/* #BEGIN THREAD MANAGER*/
	int pricmds[11];
	fy_thread *threads[MAX_THREADS];
	fy_arrayList runningThreads[1];
	int runningThreadPos;
	int run;
	int state;
	fy_linkedList pendingThreads;
	fy_long nextWakeUpTimeTotal;
	fy_int nextThreadId;
	fy_exception exitException;
	fy_int exitCode;
	fy_long nextGCTime;
	fy_long nextForceGCTime;
	/* #END THREAD MANAGER*/

	/*Service Function Table*/
	/*INPUTSTREAM*/
	void* (*isOpen)(struct fy_context *context, const char *name,
			fy_exception *exception);
	fy_int (*isRead)(struct fy_context *context, void *is,
			fy_exception *exception);
	fy_int (*isReadBlock)(struct fy_context *context, void *is, void *target,
			fy_int size, fy_exception *exception);
	fy_int (*isSkip)(struct fy_context *context, void *is, fy_int size,
			fy_exception *exception);
	void (*isClose)(struct fy_context *context, void *is);
	/*Status Saver*/
	void (*callForSave)(struct fy_context *context, fy_exception *exception);
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
			fy_uint attachedId, fy_uint length, fy_uint dataLength,
			fy_uint *data, fy_exception *exception);
	void (*saveEndObject)(struct fy_context *context, void *saver,
			fy_exception *exception);
	void (*saveLiterals)(struct fy_context *context, void *saver, fy_uint count,
			fy_uint *handles, fy_exception *exception);
	void (*saveFinalizes)(struct fy_context *context, void *saver,
			fy_uint count, fy_uint *handles, fy_exception *exception);
	void (*savePrepareThreads)(struct fy_context *context, void *saver,
			fy_uint threadsCount, fy_exception *exception);
	void (*saveThread)(struct fy_context *context, void *saver,
			fy_uint threadId, fy_uint daemon, fy_uint destroyPending,
			fy_uint interrupted, fy_long nextWakeupTime,
			fy_uint pendingLockCount, fy_uint waitForLockId,
			fy_uint waitForNotifyId, fy_uint stackSize, fy_uint *stack,
			fy_uint *typeStack, fy_exception *exception);
	void (*savePrepareFrame)(struct fy_context *context, void *saver,
			fy_uint count, fy_exception *exception);
	void (*saveFrame)(struct fy_context *context, void *saver, fy_uint methodId,
			fy_uint sb, fy_uint sp, fy_uint pc, fy_uint lpc,
			fy_exception *exception);
	void (*saveEndFrame)(struct fy_context *context, void *saver,
			fy_exception *exception);
	void (*saveEndThread)(struct fy_context *context, void *saver,
			fy_exception *exception);
	void (*saveEnd)(struct fy_context *context, void *saver,
			fy_exception *exception);
} fy_context;

typedef void (*fy_nhFunction)(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception);

typedef struct fy_nh {
	void *data;
	fy_nhFunction handler;
} fy_nh;

#ifdef	__cplusplus
}
#endif

#endif /* FISCESTU_H_ */

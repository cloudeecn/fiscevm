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
#include "fisceprt.h"
#include "fisceclz.h"
#include "fy_util/LnkList.h"
#include "fy_util/MemMan.h"
#include "fy_util/String.h"
#include "fy_util/HashMap.h"
#include "fy_util/HashMapI.h"
#include "fy_util/ArrList.h"
#include "fyc/ClassStruct.h"
#ifndef FISCESTU_H_
#define FISCESTU_H_

#define MAX_CLASSES 4096
#define MAX_METHODS 32768
#define MAX_FIELDS 32768
#define MAX_OBJECTS 65536
#define MAX_THREADS 32
#define MAX_STREAMS 16
#define MAX_INSTRUCTIONS 512

#define STACK_SIZE 16384

/*Access flags*/
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
#define FY_ACC_VARARGS 128
#define FY_ACC_BRIDGE 64
/*Extended access flags*/
#define FY_ACC_SYNTHETIC	0x00001000
#define FY_ACC_ANNOTATION	0x00002000
#define FY_ACC_ENUM			0x00004000
#define FY_ACC_SOFT_REF		0x00008000
#define FY_ACC_WEAK_REF		0x00010000
#define FY_ACC_PHANTOM_REF	0x00020000
#define FY_ACC_CONSTRUCTOR	0x00100000
#define FY_ACC_CLINIT		0x00200000
#define FY_ACC_VERIFIED		0x80000000

#define FY_TM_STATE_NEW  0
#define FY_TM_STATE_BOOT_PENDING  1
#define FY_TM_STATE_STOP  2
#define FY_TM_STATE_RUN_PENDING  3
#define FY_TM_STATE_RUNNING  4
#define FY_TM_STATE_STOP_PENDING  5
#define FY_TM_STATE_DEAD_PENDING  6
#define FY_TM_STATE_DEAD  7

#define FY_CLINIT_DONE 0
#define FY_CLINIT_WAIT -1
#define FY_CLINIT_INVOKE 1

#ifdef	__cplusplus
extern "C" {
#endif

	typedef struct fy_instruction fy_instruction;
	typedef struct fy_instruction_extra fy_instruction_extra;



	struct fy_nh;
	struct fy_class;
	struct fy_context;
	struct fy_thread;
	struct fy_message;
	struct fy_frame;

#ifdef __GNUC__
	typedef void *fy_e2_label;
#else
	typedef fy_int fy_e2_label;
#endif

	typedef struct fy_e2_label_holder {
		fy_e2_label label;
		fy_int op;
	}fy_e2_label_holder;

#ifdef FY_INSTRUCTION_COUNT
	typedef struct fy_instruction_count{
		fy_int op;
		fy_int count;
		fy_double entropy;
		fy_double entropyRev;
	} fy_instruction_count;

	typedef struct fy_instruction_pair_count{
		fy_int op1;
		fy_int op2;
		fy_int count;
	} fy_instruction_pair_count;
#endif

	typedef struct fy_repl_data{
		fy_int op;
		fy_int repl_count;
		fy_int *ops;
	} fy_repl_data;

	typedef struct fy_engine_repl_data{
		fy_repl_data *repl_data[MAX_INSTRUCTIONS];
		fy_hashMapI repl_count[1];
		fy_hashMapI repl_result[1];
#ifdef FY_INSTRUCTION_COUNT
		fy_instruction_count instructionCount[MAX_INSTRUCTIONS];
		fy_instruction_pair_count instructionPairCount[MAX_INSTRUCTIONS * MAX_INSTRUCTIONS];
		fy_int last_op;
#endif
	} fy_engine_repl_data;

	typedef fy_int (*fy_engine_runner)(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			struct fy_exception *exception,
            fy_e2_label_holder **out_labels);

	typedef struct fy_engine{
		fy_engine_runner runner;
		fy_engine_repl_data replData;
	} fy_engine;

	enum fy_heapPos {
		automatic = 0, eden = 1, young = 2, old = 3
	};

	typedef struct fy_object_data {
		fy_class *clazz;
		enum fy_heapPos position :2;
		enum {
			not_finalized, in_finalize_array, finalized
		}finalizeStatus :2;
		fy_int gen :8;
		/*Union data for different class types (array/normal object/references)*/
		union {
			fy_int multiUsageData;
			/*Length of the array object*/
			fy_int arrayLength;
			fy_int threadId;
			fy_int methodId;
			fy_int fieldId;
			fy_int classId;
			fy_int streamId;
		};
		fy_uint monitorOwnerId;
		fy_int monitorOwnerTimes;
		FY_VLS(fy_byte,data);
	}fy_object_data;

	typedef struct fy_object {
		fy_object_data *object_data;
	}fy_object;

	typedef union fy_stack_item {
		fy_uint uvalue;
		fy_int ivalue;
		fy_float fvalue;
	}fy_stack_item;

	typedef struct fy_frame {
		fy_method *method;
		fy_stack_item *baseSpp;
#ifdef FY_STRICT_CHECK
		fy_uint size;
		fy_uint localCount;
		fy_uint codeSize;
#endif
		fy_uint lpc;
		fy_uint pcofs;
	}fy_frame;

#define FY_FRAME_ENTRIES ((sizeof(struct fy_frame)+3)/4)
#define FY_GET_FRAME(THREAD,FRAMEID) ((fy_frame*)((THREAD)->stack+STACK_SIZE)-((FRAMEID)+1))

	typedef union fy_stack_wide_item {
		struct {
			fy_stack_item lower_item;
			fy_stack_item higher_item;
		}fy_stack_items;
		fy_double dvalue;
		fy_long lvalue;
	}fy_stack_wide_item;

	typedef struct fy_nativeCall {
		fy_method *method;
		fy_uint paramCount;
		fy_stack_item *params;
	}fy_nativeCall;

	typedef struct fy_thread {

		fy_uint handle;
		fy_uint currentThrowable;
		fy_int status;
		fy_int priority;
		fy_int threadId;

		fy_uint frameCount;
		fy_stack_item stack[STACK_SIZE];

		/*Used by thread manager*/
		fy_int waitForLockId;
		fy_int waitForNotifyId;
		fy_int pendingLockCount;
		fy_long nextWakeTime;
		fy_boolean interrupted;
		fy_boolean daemon;
		fy_boolean destroyPending;

		fy_nativeCall pendingNative;
	}fy_thread;

	typedef enum fy_messageType {
		/*message_continue = 0, In thread*/
		message_none = 1, /*Thread Only*/
		message_thread_dead = 2, /*Thread Only*/
		message_invoke_native = 3,/*Thread And TM pass thread*/
		/*message_exception = 4, Thread And TM pass thread*/
		message_sleep = 5, /*TM Only*/
		message_vm_dead = 6
		/*TM Only*/
	}fy_messageType;

	typedef struct fy_message {
		fy_messageType messageType;
		fy_thread *thread;
		/*We care more about stability than some hundreds bytes of memory*/
		struct {
			fy_nativeCall call;
			fy_long sleepTime;
		}body;

	}fy_message;

	struct fy_context;

	typedef struct fy_inputStream {
		void* data;
		fy_int (*isRead)(struct fy_context *context, struct fy_inputStream *is,
				fy_exception *exception);
		fy_int (*isReadBlock)(struct fy_context *context, struct fy_inputStream *is,
				void *target, fy_int size, fy_exception *exception);
		fy_int (*isSkip)(struct fy_context *context, struct fy_inputStream *is,
				fy_int size, fy_exception *exception);
		void (*isClose)(struct fy_context *context, struct fy_inputStream *is,
				fy_exception *exception);
	}fy_inputStream;

	typedef struct fy_context {
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
#ifdef FY_PROFILE
		fy_uint lastOp;
		fy_profile_data opUsage[256];
		fy_profile_data opCombine[65536];
#endif
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
	}fy_context;

	typedef fy_int (*fy_nhFunction)(
			struct fy_context *context,
			struct fy_thread *thread,
			void *data,
			fy_stack_item *args,
			fy_int argsCount,
			fy_int ops,
			fy_exception *exception
	);

	typedef struct fy_nh {
		void *data;
		fy_nhFunction handler;
	}fy_nh;

	struct fy_switch_target {
		fy_int value;
		fy_int target;
	};

	typedef struct fy_switch_lookup {
		fy_int nextPC, defaultJump, count;
		FY_VLS(struct fy_switch_target,targets);
	}fy_switch_lookup;

	typedef struct fy_switch_table {
		fy_int nextPC, defaultJump, lowest, highest;
		FY_VLS(fy_int,targets);
	}fy_switch_table;

	struct fy_instruction_extra {
		fy_int sp;
#ifdef FY_STRICT_CHECK
		fy_uint localSize;
#endif
		union {
			fy_ulong stackTypeContent;
			fy_ulong *stackTypeContents;
		}s;
	};

	struct fy_instruction {
		fy_e2_label inst;
#if defined(FY_STRICT_CHECK) || defined(FY_INSTRUCTION_COUNT)
		fy_int op;
#endif
		union {
			struct {
				fy_int param1;
				fy_int param2;
			}int_params;
			fy_class *clazz;
			fy_method *method;
			fy_field *field;
			fy_switch_lookup *swlookup;
			fy_switch_table *swtable;
			fy_int *isfield;
			struct {
				fy_int derefed;
				fy_uint value;
			}ldc;
		}params;
	};

#ifdef	__cplusplus
}
#endif

#endif /* FISCESTU_H_ */

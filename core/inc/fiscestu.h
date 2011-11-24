/*
 * fiscestu.h
 *
 *  Created on: Nov 22, 2011
 *      Author: cloudee
 */
#include "fisceprt.h"
#include "fy_util/LinkedList.h"
#include "fy_util/MemMan.h"
#include "fy_util/String.h"
#include "fy_util/HashMap.h"
#ifndef FISCESTU_H_
#define FISCESTU_H_

#define MAX_CLASSES 4096
#define MAX_METHODS 65536
#define MAX_FIELDS 65536
#define MAX_OBJECTS 131072
#define MAX_THREADS 16
#define EDEN_SIZE 65536
#define COPY_SIZE 1048576
#define OLD_ENTRIES 16384
#define STACK_SIZE 16384
#define MAX_FRAMES 256

/*Bellow are used by context*/
#define FY_TYPE_BYTE  'B'
#define FY_TYPE_CHAR  'C'
#define FY_TYPE_DOUBLE  'D'
#define FY_TYPE_FLOAT  'F'
#define FY_TYPE_INT  'I'
#define FY_TYPE_LONG  'J'
#define FY_TYPE_HANDLE  'L'
#define FY_TYPE_SHORT  'S'
#define FY_TYPE_BOOLEAN  'Z'
#define FY_TYPE_ARRAY  '['
/*Below and FY_TYPE_INT/FY_TYPE_HANDLE are used by thread*/
#define FY_TYPE_WIDE  'W'
#define FY_TYPE_RETURN  'R'
#define FY_TYPE_WIDE2  '_'
#define FY_TYPE_UNKNOWN  'X'

#define FY_BASE_STRING "java/lang/String"
#define FY_BASE_OBJECT "java/lang/Object"
#define FY_BASE_CLASS "java/lang/Class"
#define FY_BASE_THROWABLE "java/lang/Throwable"
#define FY_BASE_THREAD "java/lang/Thread"
#define FY_BASE_STACKTHREADELEMENT "java/lang/StackTraceElement"
#define FY_INIT "<init>"
#define FY_CLINIT "<clinit>"

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

typedef struct fy_method {
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
	fy_ubyte *code;

	fy_char exception_table_length;
	struct fy_exceptionHandler *exception_table;

	fy_char line_number_table_length;
	struct fy_lineNumber* line_number_table;

	fy_int paramCount;
	fy_byte *paramTypes;
	fy_byte returnType;

	fy_int method_id;
	fy_boolean clinit;
} fy_method;

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
			fy_char sizeShift;
			struct fy_class *contentClass;
		} arr;
		struct {
			fy_char pType;
		} prm;
	} ci;

	fy_uint classNameHandle;

	/*Need persist*/
	fy_uint classObjId;
	fy_int clinitThreadId;
} fy_class;

typedef struct fy_data {
	int size;
	char *data;
} fy_data;

typedef struct fy_object {
	fy_class *clazz;
	fy_int length;
	int sizeShift;
	fy_uint monitorOwnerId;
	fy_int monitorOwnerTimes;
	fy_uint attachedId;
	union fy_object_data {
		fy_long *ldata;
		fy_int *idata;
		fy_byte *bdata;
	} data;

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
	fy_boolean inUse;
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

typedef struct fy_exception {
	enum exceptionType {
		exception_none = 0, exception_normal
	/*, exception_critical // use fy_fault instead!*/
	} exceptionType;
	char exceptionName[64];
	char exceptionDesc[64];
} fy_exception;

typedef enum fy_messageType {
	message_continue = 0,
	/*message_frameChange,*/
	message_none, message_thread_dead, message_invoke_native, message_exception
} fy_messageType;

typedef struct fy_message {
	fy_messageType messageType;
	union {
		fy_method *nativeMethod;
		fy_exception exception;
	} body;

} fy_message;

typedef struct fy_context {

	int status;

	fy_str *sTopClass;
	fy_str *sClassClass;
	fy_str *sClassThrowable;
	fy_str *sBoolean;
	fy_str *sByte;
	fy_str *sShort;
	fy_str *sChar;
	fy_str *sInt;
	fy_str *sFloat;
	fy_str *sLong;
	fy_str *sDouble;
	fy_str *sString;
	fy_str *sStringArray;
	fy_str *sMainPostfix;
	fy_str *sThrowablePrintStacktrace;
	fy_str *sInit;
	fy_str *sClinit;
	fy_str *sStringValue;
	fy_str *sStringOffset;
	fy_str *sStringCount;

	fy_str *sArrayBoolean;
	fy_str *sArrayChar;
	fy_str *sArrayFloat;
	fy_str *sArrayDouble;
	fy_str *sArrayByte;
	fy_str *sArrayShort;
	fy_str *sArrayInteger;
	fy_str *sArrayLong;

	fy_str *sThrowableStackTrace;
	fy_str *sThrowableDetailMessage;
	fy_str *sStackTraceElement;
	fy_str *sStackTraceElementArray;
	fy_str *sStackTraceElementDeclaringClass;
	fy_str *sStackTraceElementMethodName;
	fy_str *sStackTraceElementFileName;
	fy_str *sStackTraceElementLineNumber;

	struct fy_class *TOP_THROWABLE;
	struct fy_class *TOP_CLASS;

	struct fy_str *primitives[128];
	struct fy_hashMap *mapPrimitivesRev;

	fy_memblock *memblocks;

	int classesCount;
	struct fy_class *classes[MAX_CLASSES];

	struct fy_hashMap *mapClassNameToId;

	int methodsCount;
	struct fy_method *methods[MAX_METHODS];
	struct fy_hashMap *mapMethodNameToId;

	int fieldsCount;
	struct fy_field *fields[MAX_FIELDS];
	struct fy_hashMap *mapFieldNameToId;

	struct fy_hashMap *mapMUNameToNH;

	/* #BEGIN HEAP*/
	fy_hashMap *literals;
	int nextHandle;
	fy_object objects[MAX_OBJECTS];
	/* #END HEAP*/

	/* #BEGIN THREAD MANAGER*/
	int pricmds[10];
	fy_linkedList threads;
	int state;
	struct fy_thread *workingThread;
	fy_linkedList pendingThreads;
	fy_long nextWakeUpTimeTotal;
	fy_int nextThreadId;
	fy_exception exitException;
	fy_int exitCode;
	/* #END THREAD MANAGER*/

	/* #BEGIN PORTABLE*/
	void *portableData;
/* #END PORTABLE*/
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

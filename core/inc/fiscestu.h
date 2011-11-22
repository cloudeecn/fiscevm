/*
 * fiscestu.h
 *
 *  Created on: Nov 22, 2011
 *      Author: cloudee
 */
#include "fisceprt.h"
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

typedef struct fy_hashMapEntry {
	struct fy_str *key;
	juint keyHash;
	void *value;
	struct fy_hashMapEntry *next;
} fy_hashMapEntry;

typedef struct fy_hashMap {
	jchar loadFactor;
	juint bucketsCount;
	fy_hashMapEntry **buckets;
	juint size;
} fy_hashMap;

typedef struct fy_linkedListNode {
	void* info;
	struct fy_linkedListNode *next; /* Point to next node */
	struct fy_linkedListNode *prev; /* Point to prev node */
} fy_linkedListNode;

typedef struct fy_linkedList {
	struct fy_linkedListNode *head; /* Header node(persist, just for convenient, doesn't store data) */
	struct fy_linkedListNode *last; /* Point to the last node of the link */
	int count; /* count of the node except the head node */
} fy_linkedList;

typedef struct fy_str {
	int length;
	int maxLength;
	int hashed;
	int hashCode;
	jchar* content;
} fy_str;

typedef union stringInfo {
	jchar string_index;
	fy_str* string;
	jint handle;
} stringInfo;

typedef union classInfo {
	jchar name_index;
	fy_str* className;
	struct ConstantClass *constantClass;
	struct fy_class *clazz;
} classInfo;

typedef struct ConstantClass {

	jubyte derefed;

	classInfo ci;
} ConstantClass;

typedef struct ConstantFieldRef {

	jubyte derefed;

	jchar class_index;
	ConstantClass *constantClass;
	struct fy_class *clazz;

	jchar name_type_index;
	struct ConstantNameAndTypeInfo *constantNameType;
	fy_str* nameType;

	struct fy_field *field;
} ConstantFieldRef;

typedef struct ConstantMethodRef {

	jubyte derefed;

	jchar class_index;
	ConstantClass *constantClass;
	struct fy_class *clazz;

	jchar name_type_index;
	struct ConstantNameAndTypeInfo *constantNameType;
	fy_str* nameType;

	/*The orignal method, not overridden.*/
	struct fy_method *method;
} ConstantMethodRef;

typedef struct ConstantStringInfo {

	jubyte derefed;

	stringInfo ci;
} ConstantStringInfo;

typedef struct ConstantIntegerFloatInfo {
	jint value;
} ConstantIntegerFloatInfo;

typedef struct ConstantLongDoubleInfo {
	jlong value;
} ConstantLongDoubleInfo;

typedef struct ConstantNameAndTypeInfo {

	jchar name_index;
	fy_str* name;

	jchar descriptor_index;
	fy_str* descriptor;
} ConstantNameAndTypeInfo;

typedef struct ConstantUtf8Info {

	fy_str* string;
} ConstantUtf8Info;

typedef struct fy_field {
	jchar access_flags;

	jchar name_index;
	fy_str* name;

	jchar descriptor_index;
	fy_str* descriptor;

	fy_str* fullName;
	fy_str* uniqueName;

	struct fy_class* owner;

	juint posRel;
	juint posAbs;

	juint initHigh;
	juint initLow;

} fy_field;

typedef struct fy_lineNumber {
	jchar start_pc;
	jchar line_number;
} fy_lineNumber;

typedef struct fy_exceptionHandler {
	jchar start_pc;
	jchar end_pc;
	jchar handler_pc;
	jubyte catchTypeDerefed;

	classInfo ci;
} fy_exceptionHandler;

typedef struct fy_method {
	jchar access_flags;

	fy_str* name;

	fy_str* descriptor;

	fy_str* fullName;
	fy_str* uniqueName;

	struct fy_class* owner;

	jubyte synthetic;

	jchar max_stack;
	jchar max_locals;

	juint codeLength;
	jubyte *code;

	jchar exception_table_length;
	struct fy_exceptionHandler *exception_table;

	jchar line_number_table_length;
	struct fy_lineNumber* line_number_table;

	jint paramCount;
	jbyte *paramTypes;
	jbyte returnType;

	jint method_id;
	jboolean clinit;
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
	jubyte phase;
	jubyte needFinalize;
	jchar constantPoolCount;
	jubyte *constantTypes;
	void** constantPools;

	jchar accessFlags;
	ConstantClass* thisClass;
	fy_str* className;
	ConstantClass* superClass;
	struct fy_class* super;
	jchar interfacesCount;
	struct fy_class** interfaces;
	jchar fieldCount;
	fy_field** fields;
	jchar methodCount;
	fy_method** methods;
	fy_method* clinit;
	fy_str* sourceFile;
	int classId;

	/*fields filled by classloader*/
	juint sizeRel;
	juint sizeAbs;
	juint ofsInHeap;

	juint staticSize;
	jint *staticArea;

	union {
		struct {
			jchar sizeShift;
			struct fy_class *contentClass;
		} arr;
		struct {
			jchar pType;
		} prm;
	} ci;

	juint classNameHandle;

	/*Need persist*/
	juint classObjId;
	jint clinitThreadId;
} fy_class;

typedef struct fy_data {
	int size;
	char *data;
} fy_data;

typedef struct fy_object {
	fy_class *clazz;
	jint length;
	int sizeShift;
	jint monitorOwnerId;
	jint monitorOwnerTimes;
	union fy_object_data {
		jlong *ldata;
		jint *idata;
		jbyte *bdata;
	} data;

} fy_object;

typedef struct fy_frame {
	fy_method *method;
	jubyte *code;
	juint sb;
#ifdef FY_STRICT_CHECK
	juint size;
	juint localCount;
	juint codeSize;
#endif
	juint methodId;
	juint sp;
	juint pc;
	juint lpc;
} fy_frame;

typedef struct fy_thread {
	jboolean inUse;
	jboolean yield;

	jint handle;
	jint currentThrowable;
	jint status;
	jint priority;
	jint threadId;

	juint frameCount;
	fy_frame frames[MAX_FRAMES];
	juint stack[STACK_SIZE];
	juint typeStack[STACK_SIZE];

	/*Used by thread manager*/
	jint waitForLockId;
	jint waitForNotifyId;
	jint pendingLockCount;
	jlong nextWakeTime;
	jboolean interrupted;
	jboolean daemon;

} fy_thread;

typedef struct fy_exception {
	enum exceptionType {
		exception_none = 0, exception_normal
	/*, exception_critical // use vm_die instead!*/
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
	fy_str *sClassClassId;
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

	struct fy_linkedList* managedMemory;
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
	jlong nextWakeUpTimeTotal;
	jint nextThreadId;
	fy_exception exitException;
	jint exitCode;
	/* #END THREAD MANAGER*/

	/* #BEGIN PORTABLE*/
	void *portableData;
/* #END PORTABLE*/
} fy_context;

typedef void (*fy_nhFunction)(struct fy_context *context,
		struct fy_thread *thread, void *data, juint *args, jint argsCount,
		fy_exception *exception);

typedef struct fy_nh {
	void *data;
	fy_nhFunction handler;
} fy_nh;

#ifdef	__cplusplus
}
#endif

#endif /* FISCESTU_H_ */

/*
 * Test.c
 *
 *  Created on: Jun 27, 2011
 *      Author: cloudee
 */

#include "fisceprt.h"
#include "fy_util/HashMap.h"
#include "fy_util/LnkList.h"
#include "fy_util/String.h"
#include "fy_util/Utf8.h"
#include "fy_util/Debug.h"
#include "fiscestu.h"
#include "fyc/VMContext.h"
#include "fyc/Class.h"
#include "fyc/Data.h"
#include "fyc/ClassLoader.h"
#include "fyc/Thread.h"

typedef struct FY_TEST_FUN {
	char *name;
	void (*fun)();
} FY_TEST_FUN;
#define FY_ASSERT(value) \
  { if(!(value)){failCount++;sprintf_s(fy_unit_msg,sizeof(fy_unit_msg),"%"FY_PRINT64"d %s(%d): ASSERTION FAILED ["#value"]\n",failCount,__FILE__,__LINE__);fprintf(stderr,"%s",fy_unit_msg); fprintf(fails,"%s",fy_unit_msg);}}
#define FY_ASSERT_FATAL(value) \
  { if(!(value)){failCount++;sprintf_s(fy_unit_msg,sizeof(fy_unit_msg),"%"FY_PRINT64"d %s(%d): ASSERTION FAILED ["#value"]\n",failCount,__FILE__,__LINE__);fprintf(stderr,"%s",fy_unit_msg); fprintf(fails,"%s",fy_unit_msg);return;}}

static FILE *fails;
static fy_ulong failCount;
static char fy_unit_msg[1024];

static fy_context *context;
static fy_memblock *block;
static fy_exception *exception;

static char msg[256];
static FILE *fp;
static void fy_log(const char *format, ...) {
	char lmsg[1024];
	va_list va_ptr;
	va_start(va_ptr, format);
	vsnprintf(lmsg, sizeof(lmsg), format, va_ptr);
	va_end(va_ptr);
	puts(lmsg);
	fputs(lmsg, fp);
}

#define TEST_EXCEPTION(EXCEPTION) if((EXCEPTION)->exceptionType!=exception_none) { \
	fy_log("Exception %s: %s occored!\n",(EXCEPTION)->exceptionName,(EXCEPTION)->exceptionDesc); \
	FY_ASSERT("Critical exception in global"==NULL); \
	return; \
}

static void testAllocate1() {
	exception = fy_allocate(sizeof(fy_exception), NULL);
	context = fy_allocate(sizeof(fy_context), NULL);
	fy_vmContextInit(context, exception);
	TEST_EXCEPTION(exception);

	fy_vmBootup(context, "EXCLUDE/fisce/test/Tester", exception);
	TEST_EXCEPTION(exception);
	block = context->memblocks;
}

static void testCleanup1() {
	fy_log("Release resources\n");
	fy_vmContextDestroy(context);
	fy_free(context);
	fy_free(exception);
}

static void testClassLoader() {

	fy_str *str;
	exception->exceptionType = exception_none;

	str = fy_mmAllocate(block, sizeof(fy_str), exception);
	TEST_EXCEPTION(exception);

	fy_strInit(block, str, 128, exception);
	TEST_EXCEPTION(exception);

	fy_strAppendUTF8(block, str, ""FY_BASE_STRING"", -1, exception);
	TEST_EXCEPTION(exception);

	/*fy_class *classString = fy_clLoadclass(context, str);*/
	fy_strDestroy(block, str);
	fy_mmFree(block, str);
	fy_log("Test class loader finished.\n");
}
static fy_class *lookup(fy_context *context, const char *name,
		fy_exception *exception) {
	fy_str sName[1];
	sName->content = NULL;
	fy_strInitWithUTF8(context->memblocks, sName, name, exception);
	fy_class *clazz;
	FYEH()NULL;
	clazz = fy_vmLookupClass(context, sName, exception);
	fy_strDestroy(context->memblocks, sName);
	return clazz;
}
void testClassLoaderFull() {
	char *names[] = { ""FY_BASE_STRING"", "[[[L"FY_BASE_STRING";", "[[[I",
			"<int>", "<double>", FY_BASE_DOUBLE, FY_BASE_MATH, NULL };
	int i = 0;
	char *nm;
	fy_str snm[1];
	fy_class *clazz;
	fy_class *clStr;
	fy_class *clObj;
	exception->exceptionType = exception_none;
	while ((nm = names[i++]) != NULL) {
		DLOG("###Full loading class %s\n", nm);
		snm->content = NULL;
		fy_strInitWithUTF8(block, snm, nm, exception);
		TEST_EXCEPTION(exception);
		clazz = fy_vmLookupClass(context, snm, exception);
		TEST_EXCEPTION(exception);
		FY_ASSERT(clazz!= NULL);
		fy_strDestroy(block, snm);
	}
	clStr = lookup(context, FY_BASE_STRING, exception);
	clObj = lookup(context, FY_BASE_OBJECT, exception);
	clazz = lookup(context, FY_BASE_INT, exception);
	FY_ASSERT(fy_classCanCastTo(context, clStr, clObj));
	FY_ASSERT(!fy_classCanCastTo(context, clStr, clazz));
}

void testClassMethod() {
	fy_class *t, *class0, *class1, *class2, *class3, *class4, *class5, *class6;
	fy_method **methods;
	fy_method *target = NULL;
	fy_method *method;
	fy_str sComplex[1];
	int i;
	exception->exceptionType = exception_none;
	sComplex->content=NULL;
	fy_strInitWithUTF8(block,sComplex,"complex",exception);
	TEST_EXCEPTION(exception);
	lookup(context, "com/cirnoworks/fisce/privat/StringUtils", exception);

	lookup(context, "EXCLUDE/fisce/test/ITester", exception);
	t = lookup(context, "EXCLUDE/fisce/test/Tester", exception);
	lookup(context, "EXCLUDE/fisce/test/TesterChild", exception);

	methods = t->methods;
	for (i = 0; i < t->methodCount; i++) {
		method = methods[i];
		if (fy_strCmp(method->name, sComplex) == 0) {
			target = method;
			break;
		}
	}
	fy_strDestroy(block,sComplex);
	FY_ASSERT(target!= NULL);
	FY_ASSERT(target->paramCount == 8);
	class0 = lookup(context, ""FY_BASE_OBJECT"", exception);
	class1 = lookup(context, "[[B", exception);
	class2 = lookup(context, "[[C", exception);
	class3 = lookup(context, "[[[B", exception);
	class4 = lookup(context, "[[L"FY_BASE_OBJECT";", exception);
	class5 = lookup(context, "[[LEXCLUDE/fisce/test/ITester;", exception);
	class6 = lookup(context, "[[LEXCLUDE/fisce/test/Tester;", exception);
	lookup(context, "[[LEXCLUDE/fisce/test/TesterChild;", exception);
	FY_ASSERT(!fy_classCanCastTo(context, class1, class2));
	FY_ASSERT(!fy_classCanCastTo(context, class1, class3));
	FY_ASSERT(fy_classCanCastTo(context, class1, class0));
	FY_ASSERT(fy_classCanCastTo(context, class3, class4));
	FY_ASSERT(fy_classCanCastTo(context, class6, class5));
	FY_ASSERT(fy_classCanCastTo(context, class6, class4));
	FY_ASSERT(!fy_classCanCastTo(context, class6, class3));
	FY_ASSERT(!fy_classCanCastTo(context, class6, class2));
	FY_ASSERT(class3->ci.arr.contentClass == class1);
}

void testHeap() {
	fy_str str[1];
	fy_str compare[1];
	fy_int sHandle;
	exception->exceptionType = exception_none;
	str->content = NULL;
	fy_strInitWithUTF8(block, str, "咩哈哈哈ABCabc,|/", exception);
	TEST_EXCEPTION(exception);
	compare->content = NULL;
	fy_strInit(block, compare, 128, exception);
	TEST_EXCEPTION(exception);

	sHandle = fy_heapLiteral(context, str, exception);
	TEST_EXCEPTION(exception);
	ASSERT(sHandle != 0);
	fy_heapGetString(context, sHandle, compare, exception);
	TEST_EXCEPTION(exception);
	FY_ASSERT(fy_strCmp(str, compare) == 0);
	FY_ASSERT(sHandle == fy_heapLiteral(context, compare, NULL));
	fy_strDestroy(block, str);
	fy_strDestroy(block, compare);
}

static void testFail(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount,fy_message *message, fy_exception *exception) {
	fy_str str;
	char msg[256];
	memset(&str, 0, sizeof(str));
	fy_strInit(context->memblocks, &str, 256, exception);
	TEST_EXCEPTION(exception);
	fy_heapGetString(context, args[0], &str, exception);
	TEST_EXCEPTION(exception);
	fy_strSPrint(msg, sizeof(msg), &str);
	fy_strDestroy(context->memblocks, &str);
	fy_log("FAIL: %s\n", msg);
	FY_ASSERT("Test fail in high level test."==NULL);

}
static void hltest(char *name) {
//	fy_class *clazz;
	fy_message message;
	fy_boolean dead;
	fy_exception ex;
	fy_context *context;
	fy_exception *exception = &ex;
	if (name == NULL) {
		fy_log("+++Executing test case Load+++\n", name);
	} else {
		fy_log("+++Executing test case %s+++\n", name);
	}
	exception->exceptionType = exception_none;
	context = fy_allocate(sizeof(fy_context), exception);
	TEST_EXCEPTION(exception);
	dead = FALSE;
	fy_vmContextInit(context, exception);
	TEST_EXCEPTION(exception);
	fy_vmRegisterNativeHandler(context,
			"EXCLUDE/fisce/test/TestService.fail.(L"FY_BASE_STRING";)V", NULL,
			testFail, exception);
	TEST_EXCEPTION(exception);

	if (name == NULL) {
		fy_vmBootFromData(context, exception);
		TEST_EXCEPTION(exception);
	} else {
		fy_vmBootup(context, name, exception);
		TEST_EXCEPTION(exception);
	}

	while (!dead) {
		fy_tmRun(context, &message, exception);
		TEST_EXCEPTION(exception);

		switch (message.messageType) {
		case message_invoke_native:
			fy_strSPrint(msg, sizeof(msg),
					message.body.call.method->uniqueName);
			fy_log("Stopped at invoke native for thread %d: %s\n",
					message.thread->threadId, msg);
			FY_ASSERT("Core native functions not implemented"==NULL);
			dead = 1;
			break;
		case message_exception:
			fy_log("Thread %d Stopped at exception %s : %s\n",
					message.thread->threadId,
					message.body.exception.exceptionName,
					message.body.exception.exceptionDesc);
			FY_ASSERT("Critical exception in thread"==NULL);
			dead = 1;
			break;
		case message_sleep:
//			printf("sleep %"FY_PRINT64"dms", message.body.sleepTime);
			break;
		case message_vm_dead:
			fy_log("VM dead\n");
			dead = 1;
			break;
		case message_none:
		case message_continue:
		case message_thread_dead:
		default:
			fy_log("Invalid message type %d\n", message.messageType);
			break;
		}
	}
	FY_ASSERT(message.messageType == message_vm_dead);
	fy_vmContextDestroy(context);
	fy_free(context);
	fy_log("--------------------------------------------------------\n");
}

void testUnicode() {
	hltest("EXCLUDE/fisce/test/UnicodeTest");
}

void testThread() {
	hltest("EXCLUDE/fisce/test/Tester");
}

void testArray() {
	hltest("EXCLUDE/fisce/test/ArrayTest");
}

void testArch() {
	hltest("EXCLUDE/fisce/test/ArchitectureTest");
}

void testDebugPrintStream() {
	hltest("EXCLUDE/fisce/test/DebugPrintStreamTest");
}

void testAutoBoxing() {
	hltest("EXCLUDE/fisce/test/AutoBoxingTest");
}

void testThread2() {
	hltest("EXCLUDE/fisce/test/RunnerTester");
}

void testEnum() {
	hltest("EXCLUDE/fisce/test/EnumTester");
}

void testException() {
	hltest("EXCLUDE/fisce/test/ExceptionTester");
}

void testForEach() {
	hltest("EXCLUDE/fisce/test/ForEachTest");
}

void testHashMap() {
	hltest("EXCLUDE/fisce/test/HashMapTest");
}

void testStatic() {
	hltest("EXCLUDE/fisce/test/StaticTest");
}

void testProfile() {
	hltest("com/cirnoworks/fisce/privat/Profile");
}

void testGC() {
	hltest("EXCLUDE/fisce/test/GCTest");
}

void testComplex() {
	hltest("EXCLUDE/fisce/test/ComplexClassLayout");
}

void testTableSwitch() {
	hltest("EXCLUDE/fisce/test/SwitchTest");
}

void testLookupSwitch() {
	hltest("EXCLUDE/fisce/test/SwitchTest2");
}

void testSave() {
	hltest("EXCLUDE/fisce/test/SaveTest");
}

void testStore() {
	hltest("EXCLUDE/fisce/test/ParamStoreTest");
}

void testLoad() {
	hltest(NULL);
}

void testNative() {
	char *classes[] = { "com/cirnoworks/fisce/privat/FiScEVM",
			"com/cirnoworks/fisce/privat/ResourceInputStream",
			"com/cirnoworks/fisce/privat/SystemInputStream",
			"com/cirnoworks/fisce/privat/SystemOutputStream", FY_BASE_CLASS,
			FY_BASE_OBJECT, FY_BASE_STACKTHREADELEMENT, FY_BASE_STRING,
			FY_BASE_SYSTEM, FY_BASE_THREAD, FY_BASE_THROWABLE, FY_BASE_MATH,
			FY_BASE_RUNTIME, FY_REFLECT_ARRAY, FY_REFLECT_CONSTRUCTOR,
			FY_REFLECT_FIELD, FY_REFLECT_METHOD, NULL };
	int i = 0, j, jmax;
	char *className;
	char msg[512];
	fy_class *clazz;
	fy_method *method;
	fy_context *context;
	fy_exception ex;
	fy_exception *exception = &ex;
	fy_log("+++Scanning native methods+++\n");

	exception->exceptionType = exception_none;
	context = fy_allocate(sizeof(fy_context), exception);
	TEST_EXCEPTION(exception);
	fy_vmContextInit(context, exception);
	TEST_EXCEPTION(exception);

	while ((className = classes[i++]) != NULL) {
		clazz = lookup(context, className, exception);
		TEST_EXCEPTION(exception);

		jmax = clazz->methodCount;
		for (j = 0; j < jmax; j++) {
			method = clazz->methods[j];
			if (method->access_flags & FY_ACC_NATIVE) {
				if (!fy_hashMapGet(context->memblocks, context->mapMUNameToNH,
						method->uniqueName)) {
					fy_strSPrint(msg, sizeof(msg), method->uniqueName);
					fy_log("Handler not found: %s\n", msg);
					FY_ASSERT(FALSE);
				}
			}
		}
	}

	fy_vmContextDestroy(context);
	fy_free(context);
	fy_log("--------------------------------------------------------\n");
}

void testCustom(char *customTest) {
	if (strcmp(customTest, "NATIVE") == 0) {
		testNative();
	} else if (strcmp(customTest, "LOAD") == 0) {
		testLoad();
	} else {
		hltest(customTest);
	}
}

FY_TEST_FUN testcases[] = { //
		{ "allocate1", testAllocate1 }, //
				{ "classloader", testClassLoader }, //
				{ "classLoaderFull", testClassLoaderFull }, //
				{ "classMethod", testClassMethod }, //
				{ "heap", testHeap }, //
				{ "cleanup1", testCleanup1 }, //
				{ "Arch", testArch }, //
				{ "Unicode", testUnicode }, //
				{ "DebugPrintStream", testDebugPrintStream }, //
				{ "Static", testStatic }, //
				{ "Smoke", testThread }, //
				{ "Array", testArray }, //
				{ "AutoBoxing", testAutoBoxing }, //
				{ "Lock", testThread2 }, //
				{ "Enum", testEnum }, //
				{ "Exception", testException }, //
				{ "ForEach", testForEach }, //
				{ "HashMap", testHashMap }, //
				{ "Profile", testProfile }, //
				{ "GC", testGC }, //
				{ "ComplexClassStructor", testComplex }, //
				{ "TableSwitch", testTableSwitch }, //
				{ "LookupSwitch", testLookupSwitch }, //
				{ "Save", testSave }, //
				{ "Load", testLoad }, //
				{ "StoreParamsToArray", testStore }, //
				/*{ "Native", testNative },*/ //
				{ NULL, NULL } };
int main(int argc, char *argv[]) {
	int i = 0;
	FY_TEST_FUN *tf;
	char *name;
	void (*fun)();
	char *customTest;
	char *fn;
	setvbuf(stdout, NULL, _IONBF, 1024);
	if (argc > 1) {
		printf("Testing %s:", argv[1]);
		customTest = argv[1];
		fn = "test.custom.fail.log";
	} else {
		customTest = NULL;
		fn = "test.fail.log";
	}
	fails = fopen(fn, "w");
	if (fails == NULL) {
		fprintf(stderr, "Can't open %s for write!\n", fn);
		return 1;
	}
	failCount = 0;
	fp = fopen(customTest == NULL ? "Test.log" : "Test-custom.log", "w");
	if (fp == NULL) {
		printf("Open log file failed");
		return 1;
	} else {
		fy_log("Test begin\n");
	}
	if (customTest) {
		printf("Testing %s\n", customTest);
		testCustom(customTest);
	} else {
		while (1) {
			tf = testcases + i;
			name = tf->name;
			fun = tf->fun;
			if (name == NULL)
				break;
			printf("Testing %s\n", name);
			fun();
			i++;
		}
	}
	fy_log("ALL TEST DONE!!!MemLeak=%ld\n", fy_getAllocated());
	fclose(fp);
	fclose(fails);
	if (failCount) {
		printf("Test FAILED! %"FY_PRINT64"d\n", failCount);
	}
	return failCount > 0;
}

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

#include "CUnit/CUnit.h"
#include "CUnit/Automated.h"

static fy_context *context;
static fy_memblock *block;
static fy_exception *exception;

static char msg[256];
static FILE *fp;
static char *customTest;
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
	CU_FAIL("Critical exception in global"); \
	return; \
}

int test_init(void) {
	fp = fopen(customTest==NULL?"Test.log":"Test-custom.log", "w");
	if (fp == NULL) {
		printf("Open log file failed");
		return 1;
	} else {
		fy_log("Test begin\n");
		return 0;
	}
}

int test_clean(void) {
	fy_log("ALL TEST DONE!!!MemLeak=%ld\n", fy_getAllocated());
	fclose(fp);
	return 0;
}

void testAllocate1() {
	exception = fy_allocate(sizeof(fy_exception), NULL);
	context = fy_allocate(sizeof(fy_context), NULL);
	fy_vmContextInit(context, exception);
	TEST_EXCEPTION(exception);
	block = context->memblocks;
}

void testCleanup1() {
	fy_log("Release resources\n");
	fy_vmContextDestroy(context);
	fy_free(context);
	fy_free(exception);
}

void testPortable() {
	CU_ASSERT_EQUAL(sizeof(fy_ubyte), 1);
	CU_ASSERT_EQUAL(sizeof(fy_char), 2);
	CU_ASSERT_EQUAL(sizeof(fy_uint), 4);
	CU_ASSERT_EQUAL(sizeof(fy_ulong), 8);
	CU_ASSERT_EQUAL(sizeof(fy_byte), 1);
	CU_ASSERT_EQUAL(sizeof(fy_short), 2);
	CU_ASSERT_EQUAL(sizeof(fy_int), 4);
	CU_ASSERT_EQUAL(sizeof(fy_long), 8);
	CU_ASSERT_EQUAL(sizeof(fy_float), 4);
	CU_ASSERT_EQUAL(sizeof(fy_double), 8);
	CU_ASSERT_EQUAL(fy_I2TOL(0x12345678,0x9ABCDEF0),
			(fy_long)0x123456789ABCDEF0LL);
	CU_ASSERT_EQUAL(fy_I2TOL(0x9ABCDEF0,0x12345678),
			(fy_long)0x9ABCDEF012345678LL);
	CU_ASSERT_EQUAL(fy_I2TOL(0x12345678,0x22345678),
			(fy_long)0x1234567822345678LL);
	CU_ASSERT_EQUAL(fy_I2TOL(0x92345678,0x9ABCDEF0),
			(fy_long)0x923456789ABCDEF0LL);
	CU_ASSERT_EQUAL(fy_I2TOUL(0x12345678,0x9ABCDEF0),
			(fy_ulong)0x123456789ABCDEF0LL);
	CU_ASSERT_EQUAL(fy_I2TOUL(0x9ABCDEF0,0x12345678),
			(fy_ulong)0x9ABCDEF012345678LL);
	CU_ASSERT_EQUAL(fy_I2TOUL(0x12345678,0x22345678),
			(fy_ulong)0x1234567822345678LL);
	CU_ASSERT_EQUAL(fy_I2TOUL(0x92345678,0x9ABCDEF0),
			(fy_ulong)0x923456789ABCDEF0LL);
	CU_ASSERT_EQUAL(fy_B2TOUI(0x12,0xAB), 0x12AB);
	CU_ASSERT_EQUAL(fy_B2TOUI(0xAB,0x12), 0xAB12);
	CU_ASSERT_EQUAL(fy_B2TOUI(0x12,0x12), 0x1212);
	CU_ASSERT_EQUAL(fy_B2TOUI(0xAB,0xAB), 0xABAB);
	CU_ASSERT_EQUAL(fy_B2TOI(0x12,0xAB), (fy_short)0x12AB);
	CU_ASSERT_EQUAL(fy_B2TOI(0xAB,0x12), (fy_short)0xAB12);
	CU_ASSERT_EQUAL(fy_B2TOI(0x12,0x12), (fy_short)0x1212);
	CU_ASSERT_EQUAL(fy_B2TOI(0xAB,0xAB), (fy_short)0xABAB);
	CU_ASSERT_EQUAL(fy_B4TOI(0x12,0xAB,0x34,0xCD), 0x12AB34CD);
	CU_ASSERT_EQUAL(fy_B4TOI(0xAB,0x12,0xCD,0x34), 0xAB12CD34);
	CU_ASSERT_EQUAL(fy_B4TOI(0xAB,0xCD,0x89,0xEF), 0xABCD89EF);
	CU_ASSERT_EQUAL(fy_B4TOI(0x12,0x34,0x56,0x78), 0x12345678);
}

void testClassLoader() {

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
	fy_str *sName = fy_strCreateFromUTF8(context->memblocks, name, exception);
	fy_class *clazz;
	fy_exceptionCheckAndReturn(exception)NULL;
	clazz = fy_vmLookupClass(context, sName, exception);
	fy_strRelease(context->memblocks, sName);
	return clazz;
}
void testClassLoaderFull() {
	char *names[] = { ""FY_BASE_STRING"", "[[[L"FY_BASE_STRING";", "[[[I",
			"int", "double", "java/lang/Double", "java/lang/Math", NULL };
	int i = 0;
	char *nm;
	fy_str *snm;
	fy_class *clazz;
	fy_class *clStr;
	fy_class *clObj;
	exception->exceptionType = exception_none;
	while ((nm = names[i++]) != NULL) {
		DLOG("###Full loading class %s\n", nm);
		snm = fy_strCreateFromUTF8(block, nm, exception);
		TEST_EXCEPTION(exception);
		clazz = fy_vmLookupClass(context, snm, exception);
		TEST_EXCEPTION(exception);
		CU_ASSERT_NOT_EQUAL(clazz, NULL);
		fy_strRelease(block, snm);
	}
	clStr = lookup(context, FY_BASE_STRING, exception);
	clObj = lookup(context, FY_BASE_OBJECT, exception);
	clazz = lookup(context, "java/lang/Integer", exception);
	CU_ASSERT(fy_classCanCastTo(context,clStr,clObj));
	CU_ASSERT_FALSE(fy_classCanCastTo(context,clStr,clazz));
}

void testClassMethod() {
	fy_class *it, *t, *tc, *class0, *class1, *class2, *class3, *class4, *class5,
			*class6, *class7;
	fy_method **methods;
	fy_method *target = NULL;
	fy_method *method;
	fy_str *sComplex;
	int i;
	exception->exceptionType = exception_none;
	sComplex = fy_strCreateFromUTF8(block, "complex", exception);
	TEST_EXCEPTION(exception);
	lookup(context, "com/cirnoworks/fisce/privat/StringUtils", exception);

	it = lookup(context, "EXCLUDE/fisce/test/ITester", exception);
	t = lookup(context, "EXCLUDE/fisce/test/Tester", exception);
	tc = lookup(context, "EXCLUDE/fisce/test/TesterChild", exception);

	methods = t->methods;
	for (i = 0; i < t->methodCount; i++) {
		method = methods[i];
		if (fy_strCmp(method->name, sComplex) == 0) {
			target = method;
			break;
		}
	}CU_ASSERT_NOT_EQUAL(target, NULL);
	CU_ASSERT_EQUAL(target->paramCount, 8);
	class0 = lookup(context, ""FY_BASE_OBJECT"", exception);
	class1 = lookup(context, "[[B", exception);
	class2 = lookup(context, "[[C", exception);
	class3 = lookup(context, "[[[B", exception);
	class4 = lookup(context, "[[L"FY_BASE_OBJECT";", exception);
	class5 = lookup(context, "[[LEXCLUDE/fisce/test/ITester;", exception);
	class6 = lookup(context, "[[LEXCLUDE/fisce/test/Tester;", exception);
	class7 = lookup(context, "[[LEXCLUDE/fisce/test/TesterChild;", exception);
	CU_ASSERT(!fy_classCanCastTo(context,class1,class2));
	CU_ASSERT(!fy_classCanCastTo(context,class1,class3));
	CU_ASSERT(fy_classCanCastTo(context,class1,class0));
	CU_ASSERT(fy_classCanCastTo(context,class3,class4));
	CU_ASSERT(fy_classCanCastTo(context,class6,class5));
	CU_ASSERT(fy_classCanCastTo(context,class6,class4));
	CU_ASSERT(!fy_classCanCastTo(context,class6,class3));
	CU_ASSERT(!fy_classCanCastTo(context,class6,class2));
}

void testHeap() {
	fy_str *str;
	fy_str *compare;
	fy_int sHandle;
	exception->exceptionType = exception_none;
	str = fy_strCreateFromUTF8(block, "咩哈哈哈ABCabc,|/", exception);
	TEST_EXCEPTION(exception);
	compare = fy_strCreate(block, exception);
	TEST_EXCEPTION(exception);

	sHandle = fy_heapLiteral(context, str, exception);
	TEST_EXCEPTION(exception);
	ASSERT(sHandle != 0);
	fy_heapGetString(context, sHandle, compare, exception);
	TEST_EXCEPTION(exception);
	CU_ASSERT( fy_strCmp(str,compare)==0);
	CU_ASSERT(sHandle == fy_heapLiteral(context, compare, NULL));
	fy_strRelease(block, str);
	fy_strRelease(block, compare);
}

static void testFail(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
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
	CU_FAIL("Test fail in high level test.");

}
static void hltest(char *name) {
	fy_class *clazz;
	fy_message message;
	fy_boolean dead;
	fy_exception ex;
	fy_context *context;
	fy_exception *exception = &ex;
	fy_log("+++Executing test case %s+++\n", name);

	exception->exceptionType = exception_none;
	context = fy_allocate(sizeof(fy_context), exception);
	TEST_EXCEPTION(exception);
	dead = FALSE;
	fy_vmContextInit(context, exception);
	TEST_EXCEPTION(exception);
	fy_vmRegisterNativeHandler(context,
			"EXCLUDE/fisce/test/TestService.fail.(Ljava/lang/String;)V", NULL,
			testFail, exception);
	TEST_EXCEPTION(exception);

	clazz = lookup(context, name, exception);
	TEST_EXCEPTION(exception);
	fy_tmBootFromMain(context, clazz, exception);
	TEST_EXCEPTION(exception);
	while (!dead) {
		fy_tmRun(context, &message, exception);
		TEST_EXCEPTION(exception);

		switch (message.messageType) {
		case message_invoke_native:
			fy_strSPrint(msg, sizeof(msg),
					message.body.nativeMethod->uniqueName);
			fy_log("Stopped at invoke native for thread %d: %s\n",
					message.thread->threadId, msg);
			CU_FAIL("Core native functions not implemented");
			dead = 1;
			break;
		case message_exception:
			fy_log("Thread %d Stopped at exception %s : %s\n",
					message.thread->threadId,
					message.body.exception.exceptionName,
					message.body.exception.exceptionDesc);
			CU_FAIL("Critical exception in thread");
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
	}CU_ASSERT_EQUAL(message.messageType, message_vm_dead);
	fy_vmContextDestroy(context);
	fy_free(context);
	fy_log("--------------------------------------------------------\n");
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

void testCustom() {
	hltest(customTest);
}

CU_TestInfo testcases[] = { { "allocate1", testAllocate1 }, //
		{ "platform related", testPortable }, //
//		{ "classloader", testClassLoader }, //
		{ "classLoaderFull", testClassLoaderFull }, //
		{ "classMethod", testClassMethod }, //
		{ "heap", testHeap }, //
		{ "cleanup1", testCleanup1 }, //
		{ "Arch", testArch }, //
		{ "Static", testStatic }, //
		{ "Smoke", testThread }, //
		{ "Array", testArray }, //
		{ "AutoBoxing", testAutoBoxing }, //
		{ "Lock", testThread2 }, //
		{ "Enum", testEnum }, //
		{ "Exception", testException }, //
		{ "ForEach", testForEach }, //
		{ "HashMap", testHashMap }, //
		CU_TEST_INFO_NULL };

CU_TestInfo customCase[] = { { "custom", testCustom }, //
		CU_TEST_INFO_NULL };

CU_SuiteInfo suites[] = {
		{ "Testing parts:", test_init, test_clean, testcases }, //
		CU_SUITE_INFO_NULL };

CU_SuiteInfo customSuits[] = { { "Testing parts:", test_init, test_clean,
		customCase }, //
		CU_SUITE_INFO_NULL };

void AddTests(void) {
	ASSERT(NULL != CU_get_registry());
	ASSERT(!CU_is_test_running());
	/* shortcut regitry */

	if (CUE_SUCCESS
			!= CU_register_suites(customTest == NULL ? suites : customSuits)) {
		fprintf(stderr, "Register suites failed - %s ", CU_get_error_msg());
		exit(-1);
	}
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		printf("Testing %s:", argv[1]);
		customTest = argv[1];
	} else {
		customTest = NULL;
	}
	if (CU_initialize_registry()) {
		fprintf(stderr, " Initialization of Test Registry failed. ");
		exit(-1);
	} else {
		AddTests();
		CU_set_output_filename(customTest == NULL ? "Test" : "Test-custom");
		CU_list_tests_to_file();
		CU_automated_run_tests();
		CU_cleanup_registry();
	}
	return 0;
}

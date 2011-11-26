/*
 * Test.c
 *
 *  Created on: Jun 27, 2011
 *      Author: cloudee
 */

#include <stdio.h>
#include <time.h>
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
#include <assert.h>
#include <time.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include "CUnit/CUnit.h"
#include "CUnit/Automated.h"

static fy_context *context;
static fy_memblock *block;
static fy_exception *exception;

#define TEST_EXCEPTION(EXCEPTION) if((EXCEPTION)->exceptionType!=exception_none) { \
	printf("Exception %s: %s occored!",(EXCEPTION)->exceptionName,(EXCEPTION)->exceptionDesc); \
	CU_FAIL("Exception occored!"); \
	return; \
}

int test_init(void) {
	exception = fy_allocate(sizeof(fy_exception), NULL);
	context = fy_allocate(sizeof(fy_context), NULL);
	fy_vmContextInit(context, exception);
	if ((exception)->exceptionType != exception_none) {
		printf("Exception %s: %s occored!", (exception)->exceptionName,
				(exception)->exceptionDesc);
		CU_FAIL("Exception occored!");
		return 1;
	}
	block = context->memblocks;
	return 0;
}

int test_clean(void) {
	printf("Release resources\n");
	fy_vmContextDestroy(context);
	fy_free(context);
	fy_free(exception);
	printf("ALL TEST DONE!!!MemLeak=%ld\n", fy_getAllocated());
	return 0;
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
	CU_ASSERT_EQUAL(fy_B2TOI(0x12,0xAB), 0x12AB);
	CU_ASSERT_EQUAL(fy_B2TOI(0xAB,0x12), 0xAB12);
	CU_ASSERT_EQUAL(fy_B2TOI(0x12,0x12), 0x1212);
	CU_ASSERT_EQUAL(fy_B2TOI(0xAB,0xAB), 0xABAB);
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
	printf("Test class loader finished.\n");
}
static fy_class *lookup(const char *name) {
	fy_str *sName = fy_strCreateFromUTF8(block, name, exception);

	fy_class *clazz;
	clazz = fy_vmLookupClass(context, sName, exception);

	fy_strRelease(block, sName);
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
	clStr = lookup(""FY_BASE_STRING"");
	clObj = lookup(""FY_BASE_OBJECT"");
	clazz = lookup("java/lang/Integer");
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
	lookup("com/cirnoworks/fisce/privat/StringUtils");

	it = lookup("EXCLUDE/fisce/test/ITester");
	t = lookup("EXCLUDE/fisce/test/Tester");
	tc = lookup("EXCLUDE/fisce/test/TesterChild");

	methods = t->methods;
	for (i = 0; i < t->methodCount; i++) {
		method = methods[i];
		if (fy_strCmp(method->name, sComplex) == 0) {
			target = method;
			break;
		}
	}CU_ASSERT_NOT_EQUAL(target, NULL);
	CU_ASSERT_EQUAL(target->paramCount, 8);
	class0 = lookup(""FY_BASE_OBJECT"");
	class1 = lookup("[[B");
	class2 = lookup("[[C");
	class3 = lookup("[[[B");
	class4 = lookup("[[L"FY_BASE_OBJECT";");
	class5 = lookup("[[LEXCLUDE/fisce/test/ITester;");
	class6 = lookup("[[LEXCLUDE/fisce/test/Tester;");
	class7 = lookup("[[LEXCLUDE/fisce/test/TesterChild;");
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

void testThread() {
	fy_str *name;
	fy_thread *thread;
	fy_method *method;
	fy_class *clazz;
	fy_class *clazzThread;
	fy_field *fieldThreadPriority;
	fy_int threadHandle;
	fy_message message;
	int i, count;

	exception->exceptionType = exception_none;
	clazz = lookup("EXCLUDE/fisce/test/Tester");
	clazzThread = lookup(FY_BASE_THREAD);
	fieldThreadPriority = fy_vmLookupFieldStatic(context, clazzThread,
			fy_strCreateFromUTF8(block, ".priority.I", exception), exception);
	TEST_EXCEPTION(exception);

	ASSERT(clazz != NULL);
	ASSERT(clazzThread != NULL);
	ASSERT(fieldThreadPriority != NULL);
	count = clazz->methodCount;
#ifdef _DEBUG
	for (i = 0; i < count; i++) {
		method = clazz->methods[i];
		fy_strPrint(method->uniqueName);
		printf("\n");
	}
#endif
	method = NULL;
	name = fy_strCreateFromUTF8(block,
			"EXCLUDE/fisce/test/Tester.main.([L"FY_BASE_STRING";)V", exception);
	TEST_EXCEPTION(exception);

	thread = fy_mmAllocate(block, sizeof(fy_thread), exception);
	TEST_EXCEPTION(exception);
	method = fy_vmGetMethod(context, name);
	ASSERT(method != NULL);
	fy_threadInit(context, thread);
	thread->threadId = 1;
	thread->priority = 5;
	threadHandle = fy_heapAllocate(context, clazzThread, exception);
	TEST_EXCEPTION(exception);
	fy_heapPutFieldInt(context, threadHandle, fieldThreadPriority, 5,
			exception);
	TEST_EXCEPTION(exception);
	fy_threadCreateWithMethod(context, thread, threadHandle, method, exception);
	TEST_EXCEPTION(exception);
	fy_threadRun(context, thread, &message, 2147483647);

	switch (message.messageType) {
	case message_none:
		printf("Stopped as yield or reached inst. limit");
		break;
	case message_invoke_native:
		printf("Stopped at invoke native: ");
		fy_strPrint(message.body.nativeMethod->uniqueName);
		printf("\n");
		break;
	case message_thread_dead:
		printf("Stopped as thread dead\n");
		break;
	case message_exception:
		printf("Stopped at exception %s : %s\n",
				message.body.exception.exceptionName,
				message.body.exception.exceptionDesc);
		break;
	default:
	case message_continue:
		printf("Invalid message type %d\n", message.messageType);
		break;
	}CU_ASSERT_EQUAL(message.messageType, message_thread_dead);
}

CU_TestInfo testcases[] = { { "platform related", testPortable }, //
//		{ "classloader", testClassLoader }, //
		{ "classLoaderFull", testClassLoaderFull }, //
		{ "classMethod", testClassMethod }, //
		{ "heap", testHeap }, //
		{ "thread", testThread }, //
		CU_TEST_INFO_NULL };

CU_SuiteInfo suites[] = {
		{ "Testing parts:", test_init, test_clean, testcases }, //
		CU_SUITE_INFO_NULL };

void AddTests(void) {
	assert(NULL != CU_get_registry());
	assert(!CU_is_test_running());
	/* shortcut regitry */

	if (CUE_SUCCESS != CU_register_suites(suites)) {
		fprintf(stderr, "Register suites failed - %s ", CU_get_error_msg());
		exit(-1);
	}
}

int main(int argc, char *argv[]) {
	if (CU_initialize_registry()) {
		fprintf(stderr, " Initialization of Test Registry failed. ");
		exit(-1);
	} else {
		AddTests();
		CU_set_output_filename("Test.log");
		CU_list_tests_to_file();
		CU_automated_run_tests();
		CU_cleanup_registry();
	}
	return 0;
}

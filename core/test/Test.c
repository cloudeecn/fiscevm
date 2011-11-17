/*
 * Test.c
 *
 *  Created on: Jun 27, 2011
 *      Author: cloudee
 */

#include <stdio.h>
#include <time.h>
#include "fyc/Structs.h"
#include "fyc/VMContext.h"
#include "fyc/Class.h"
#include "fyc/Portable.h"
#include "fyc/LinkedList.h"
#include "fyc/Data.h"
#include "fyc/String.h"
#include "fyc/ClassLoader.h"
#include "fyc/Debug.h"
#include "fyc/Global.h"
#include "fyc/HashMap.h"
#include "fyc/Thread.h"
#include <assert.h>
#include <time.h>

#include "CUnit/CUnit.h"
#include "CUnit/Automated.h"

static fy_VMContext *context;

int test_init(void) {
	fy_exception exception;
	fy_gInit();
	exception.exceptionType = exception_none;
	context = vm_allocate(sizeof(fy_VMContext));
	fy_vmContextInit(context, &exception);
	if (exception.exceptionType != exception_none) {
		vm_die("Exception in initialize: %s  || %s", exception.exceptionName,
				exception.exceptionDesc);
	}
	return 0;
}

int test_clean(void) {
	printf("Release resources\n");
	fy_vmContextDestroy(context);
	vm_free(context);
	fy_gDestroy();
	printf("ALL TEST DONE!!!MemLeak=%ld\n", vm_getAllocated());
	return 0;
}

void testPortable() {
	CU_ASSERT_EQUAL(sizeof(jubyte), 1);
	CU_ASSERT_EQUAL(sizeof(jchar), 2);
	CU_ASSERT_EQUAL(sizeof(juint), 4);
	CU_ASSERT_EQUAL(sizeof(julong), 8);
	CU_ASSERT_EQUAL(sizeof(jbyte), 1);
	CU_ASSERT_EQUAL(sizeof(jshort), 2);
	CU_ASSERT_EQUAL(sizeof(jint), 4);
	CU_ASSERT_EQUAL(sizeof(jlong), 8);
	CU_ASSERT_EQUAL(sizeof(jfloat), 4);
	CU_ASSERT_EQUAL(sizeof(jdouble), 8);
	CU_ASSERT_EQUAL(fy_I2TOL(0x12345678,0x9ABCDEF0),
			(jlong)0x123456789ABCDEF0LL);
	CU_ASSERT_EQUAL(fy_I2TOL(0x9ABCDEF0,0x12345678),
			(jlong)0x9ABCDEF012345678LL);
	CU_ASSERT_EQUAL(fy_I2TOL(0x12345678,0x22345678),
			(jlong)0x1234567822345678LL);
	CU_ASSERT_EQUAL(fy_I2TOL(0x92345678,0x9ABCDEF0),
			(jlong)0x923456789ABCDEF0LL);
	CU_ASSERT_EQUAL(fy_I2TOUL(0x12345678,0x9ABCDEF0),
			(julong)0x123456789ABCDEF0LL);
	CU_ASSERT_EQUAL(fy_I2TOUL(0x9ABCDEF0,0x12345678),
			(julong)0x9ABCDEF012345678LL);
	CU_ASSERT_EQUAL(fy_I2TOUL(0x12345678,0x22345678),
			(julong)0x1234567822345678LL);
	CU_ASSERT_EQUAL(fy_I2TOUL(0x92345678,0x9ABCDEF0),
			(julong)0x923456789ABCDEF0LL);
	CU_ASSERT_EQUAL(fy_B2TOI(0x12,0xAB), 0x12AB);
	CU_ASSERT_EQUAL(fy_B2TOI(0xAB,0x12), 0xAB12);
	CU_ASSERT_EQUAL(fy_B2TOI(0x12,0x12), 0x1212);
	CU_ASSERT_EQUAL(fy_B2TOI(0xAB,0xAB), 0xABAB);
	CU_ASSERT_EQUAL(fy_B4TOI(0x12,0xAB,0x34,0xCD), 0x12AB34CD);
	CU_ASSERT_EQUAL(fy_B4TOI(0xAB,0x12,0xCD,0x34), 0xAB12CD34);
	CU_ASSERT_EQUAL(fy_B4TOI(0xAB,0xCD,0x89,0xEF), 0xABCD89EF);
	CU_ASSERT_EQUAL(fy_B4TOI(0x12,0x34,0x56,0x78), 0x12345678);
}

static void *g_address;

static void assertNoNode(fy_linkedListNode *node) {
	CU_ASSERT_NOT_EQUAL(node->info, g_address);
}

void testMemManage() {
	int i;
	void *address[17];

	for (i = 0; i < 17; i++) {
		address[i] = fy_vmAllocate(context, 4096);
		/*printf("%p\n", address[i]);*/
	}

	fy_vmFree(context, address[6]);
	fy_vmFree(context, address[15]);
	g_address = address[6] - sizeof(fy_linkedListNode*);
	fy_linkedListTraverse(context->managedMemory, assertNoNode);
	g_address = address[15] - sizeof(fy_linkedListNode*);
	fy_linkedListTraverse(context->managedMemory, assertNoNode);
}

void testString() {
	int i;
	const char *cc = "ABC中文DEF\n";
	const char *cc1 = "ABC中文DEF";
	const char *cc2 = "ABC中文DEG";
	printf("%s\n%d", cc, (int) strlen(cc));
	fy_str *js = fy_vmAllocate(context, sizeof(fy_str));
	fy_str *js1 = fy_vmAllocate(context, sizeof(fy_str));
	fy_str *js2 = fy_vmAllocate(context, sizeof(fy_str));
	fy_strInit(context, js, 1);
	fy_strInit(context, js1, 1);
	fy_strInit(context, js2, 1);
	fy_strAppendUTF8(context, js, cc, 999);
	fy_strAppendUTF8(context, js1, cc1, 999);
	fy_strAppendUTF8(context, js2, cc2, 999);
	for (i = 0; i < js->length; i++) {
		printf("%d ", (juint) js->content[i]);
	}
	printf("\n");
	fy_strPrint(js);
	CU_ASSERT(fy_strCmp(js,js)==0);
	CU_ASSERT(fy_strCmp(js1,js)<0);
	CU_ASSERT(fy_strCmp(js,js1)>0);
	CU_ASSERT(fy_strCmp(js2,js)>0);
	CU_ASSERT(fy_strCmp(js,js2)<0);
	//	fy_strDestroy(context, js);
	//	fy_vmFree(context,js);
}

void testClassLoader() {
	fy_str *str = fy_vmAllocate(context, sizeof(fy_str));
	fy_strInit(context, str, 128);
	fy_strAppendUTF8(context, str, ""FY_BASE_STRING"", -1);
	/*fy_class *classString = fy_clLoadclass(context, str);*/
	fy_strDestroy(context, str);
	fy_vmFree(context, str);
	printf("Test class loader finished.\n");
}
static fy_class *lookup(const char *name) {
	fy_str *sName = fy_strAllocateFromUTF8(context, name);
	fy_exception exception;
	fy_class *clazz;
	exception.exceptionType = exception_none;
	exception.exceptionName[0] = 0;
	exception.exceptionDesc[0] = 0;
	clazz = fy_vmLookupClass(context, sName, &exception);
	fy_strRelease(context, sName);
	if (exception.exceptionType != exception_none) {
		vm_die("Exception %s caught: %s", exception.exceptionName,
				exception.exceptionDesc);
	}
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
	fy_exception exception;
	exception.exceptionType = exception_none;
	while ((nm = names[i++]) != NULL) {
		DLOG("###Full loading class %s\n", nm);
		snm = fy_strAllocateFromUTF8(context, nm);
		clazz = fy_vmLookupClass(context, snm, &exception);
		if (exception.exceptionType != exception_none) {
			vm_die("Exception in initialize: %s  || %s",
					exception.exceptionName, exception.exceptionDesc);
		}CU_ASSERT_NOT_EQUAL(clazz, NULL);
		fy_strRelease(context, snm);
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
	sComplex = fy_strAllocateFromUTF8(context, "complex");
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
	jint sHandle;
	fy_exception exception;
	exception.exceptionType = exception_none;
	str = fy_strAllocateFromUTF8(context, "咩哈哈哈ABCabc,|/");
	compare = fy_strAllocate(context);

	sHandle = fy_heapLiteral(context, str, &exception);
	ASSERT(sHandle != 0);ASSERT(exception.exceptionType == exception_none);
	fy_heapGetString(context, sHandle, compare, &exception);
	ASSERT(exception.exceptionType == exception_none);CU_ASSERT(
			fy_strCmp(str,compare)==0);
	CU_ASSERT(sHandle == fy_heapLiteral(context, compare, &exception));
	fy_strRelease(context, str);
	fy_strRelease(context, compare);
}

void testThread() {
	fy_str *name;
	fy_thread *thread;
	fy_method *method;
	fy_exception exception;
	fy_class *clazz = lookup("EXCLUDE/fisce/test/Tester");
	fy_class *clazzThread = lookup(FY_BASE_THREAD);
	fy_field *fieldThreadPriority = fy_vmLookupFieldStatic(context, clazzThread,
			fy_strAllocateFromUTF8(context, ".priority.I"));
	jint threadHandle;
	fy_message message;
	int i, count;

	exception.exceptionType = exception_none;
	ASSERT(clazz != NULL);ASSERT(clazzThread != NULL);ASSERT(fieldThreadPriority != NULL);
	count = clazz->methodCount;
#ifdef _DEBUG
	for (i = 0; i < count; i++) {
		method = clazz->methods[i];
		fy_strPrint(method->uniqueName);
		printf("\n");
	}
#endif
	method = NULL;
	name = fy_strAllocateFromUTF8(context,
			"EXCLUDE/fisce/test/Tester.main.([L"FY_BASE_STRING";)V");

	thread = fy_vmAllocate(context, sizeof(fy_thread));
	method = fy_vmGetMethod(context, name);
	ASSERT(method != NULL);
	fy_threadInit(context, thread);
	thread->threadId = 1;
	thread->priority = 5;
	threadHandle = fy_heapAllocate(context, clazzThread, &exception);
	ASSERT(exception.exceptionType == exception_none);
	fy_heapPutFieldInt(context, threadHandle, fieldThreadPriority, 5,
			&exception);
	fy_threadCreateWithMethod(context, thread, threadHandle, method,
			&exception);
	ASSERT(exception.exceptionType == exception_none);
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

void testHashMap() {
	int blocks;
	int i;
	char buf[256];
	//	char buf1[256];
	juint values[10000];
	juint *value;
	fy_str *tmp;
	fy_hashMap *hashMap = fy_vmAllocate(context, sizeof(fy_hashMap));
	memset(buf, 0, 256);
	blocks = vm_getAllocated();
	fy_hashMapInit(context, hashMap, 16, 12);

	clock_t t1, t2, t3, t4;

	t1 = clock();
	for (i = 0; i < 10000; i++) {
		sprintf_s(buf, 10, "%d", i);
		tmp = fy_strNew(context, buf);
		fy_hashMapPut(context, hashMap, tmp, values + i);
		fy_strDestroy(context, tmp);
		fy_vmFree(context, tmp);
		values[i] = i * 3;
	}

	t2 = clock();

	CU_ASSERT_EQUAL(hashMap->size, 10000);
	for (i = 0; i < 15000; i++) {
		sprintf_s(buf, 10, "%d", i);
		tmp = fy_strNew(context, buf);
		value = fy_hashMapGet(context, hashMap, tmp);
		if (i < 10000) {
			CU_ASSERT_EQUAL(*value, values[i])
		} else {
			CU_ASSERT_EQUAL(value, NULL);
		}
		fy_strDestroy(context, tmp);
		fy_vmFree(context, tmp);
	}
	t3 = clock();
	fy_hashMapDestroy(context, hashMap);
	t4 = clock();
	printf("HashMap time %f %f %f\n",
			(t2 - t1) * (double) 1000 / (double) CLOCKS_PER_SEC,
			(t3 - t2) * (double) 1000 / (double) CLOCKS_PER_SEC,
			(t4 - t3) * (double) 1000 / (double) CLOCKS_PER_SEC);
	CU_ASSERT_EQUAL(blocks, vm_getAllocated());
	fy_vmFree(context, hashMap);
}

CU_TestInfo testcases[] = { { "platform related", testPortable }, //
		{ "memory management", testMemManage }, //
		{ "string", testString }, //
//		{ "classloader", testClassLoader }, //
		{ "classLoaderFull", testClassLoaderFull }, //
		{ "classMethod", testClassMethod }, //
		{ "heap", testHeap }, //
		{ "thread", testThread }, //
		{ "hashMap", testHashMap }, //
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

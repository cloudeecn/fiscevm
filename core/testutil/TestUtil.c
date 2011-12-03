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
#include <assert.h>
#include <time.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include "CUnit/CUnit.h"
#include "CUnit/Automated.h"

static fy_memblock *block;
static fy_port *port;
static void dumpException(fy_exception *exception) {
	if (exception == NULL || exception->exceptionType != exception_none) {
		fy_fault(NULL, NULL, "!!!!!");
	}
	fy_fault(NULL, NULL, "Exception in initialize: %s  || %s",
			exception->exceptionName, exception->exceptionDesc);
}

#define CHECK_EXCEPTION(EXCEPTION) if((EXCEPTION)->exceptionType != exception_none) dumpException(EXCEPTION)
int test_init(void) {
	fy_exception exception;
	exception.exceptionType = exception_none;
	block = fy_allocate(sizeof(fy_memblock), &exception);
	CHECK_EXCEPTION(&exception);
	fy_mmInit(block, &exception);
	CHECK_EXCEPTION(&exception);
	port = fy_mmAllocate(block, sizeof(fy_port), &exception);
	CHECK_EXCEPTION(&exception);
	fy_portInit(port);
	return 0;
}

int test_clean(void) {
	printf("Release resources\n");
	fy_mmDestroy(block);
	fy_free(block);
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

void testMemManage() {
	int i;
	void *address[17];

	fy_exception ex;
	fy_exception *exception = &ex;
	ex.exceptionType = exception_none;
	for (i = 0; i < 17; i++) {
		address[i] = fy_mmAllocate(block, 4096, exception);
		CHECK_EXCEPTION(exception);
		/*printf("%p\n", address[i]);*/
	}

	fy_mmFree(block, address[6]);
	fy_mmFree(block, address[15]);
}

void testString() {
	int i;
	const char *cc = "ABC中文DEF\n";
	const char *cc1 = "ABC中文DEF";
	const char *cc2 = "ABC中文DEG";
	fy_exception ex;
	fy_exception *exception = &ex;
	ex.exceptionType = exception_none;
	printf("%s\n%d", cc, (int) strlen(cc));
	fy_str *js = fy_mmAllocate(block, sizeof(fy_str), exception);
	fy_str *js1 = fy_mmAllocate(block, sizeof(fy_str), exception);
	fy_str *js2 = fy_mmAllocate(block, sizeof(fy_str), exception);
	CHECK_EXCEPTION(exception);
	fy_strInit(block, js, 1, exception);
	CHECK_EXCEPTION(exception);
	fy_strInit(block, js1, 1, exception);
	CHECK_EXCEPTION(exception);
	fy_strInit(block, js2, 1, exception);
	CHECK_EXCEPTION(exception);
	fy_strAppendUTF8(block, js, cc, 999, exception);
	CHECK_EXCEPTION(exception);
	fy_strAppendUTF8(block, js1, cc1, 999, exception);
	CHECK_EXCEPTION(exception);
	fy_strAppendUTF8(block, js2, cc2, 999, exception);
	CHECK_EXCEPTION(exception);
	for (i = 0; i < js->length; i++) {
		printf("%d ", (fy_uint) js->content[i]);
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

void testHashMap() {
	int blocks;
	int i;
	char buf[256];
	//	char buf1[256];
	fy_uint values[10000];
	fy_uint *value;
	fy_str *tmp;
	fy_exception ex;
	fy_exception *exception = &ex;
	ex.exceptionType = exception_none;
	fy_hashMap *hashMap = fy_mmAllocate(block, sizeof(fy_hashMap), exception);
	CHECK_EXCEPTION(exception);
	memset(buf, 0, 256);
	blocks = fy_getAllocated();
	fy_hashMapInit(block, hashMap, 16, 12, exception);
	CHECK_EXCEPTION(exception);

	fy_long t1, t2, t3, t4;

	t1 = fy_portTimeMillSec(port);
	for (i = 0; i < 10000; i++) {
		sprintf_s(buf, 10, "%d", i);
		tmp = fy_strCreateFromUTF8(block, buf, exception);
		CHECK_EXCEPTION(exception);
		fy_hashMapPut(block, hashMap, tmp, values + i, exception);
		CHECK_EXCEPTION(exception);
		fy_strDestroy(block, tmp);
		fy_mmFree(block, tmp);
		values[i] = i * 3;
	}

	t2 = fy_portTimeMillSec(port);

	CU_ASSERT_EQUAL(hashMap->size, 10000);
	for (i = 0; i < 15000; i++) {
		sprintf_s(buf, 10, "%d", i);
		tmp = fy_strCreateFromUTF8(block, buf, exception);
		CHECK_EXCEPTION(exception);
		value = fy_hashMapGet(block, hashMap, tmp);
		if (i < 10000) {
			CU_ASSERT_EQUAL(*value, values[i])
		} else {
			CU_ASSERT_EQUAL(value, NULL);
		}
		fy_strDestroy(block, tmp);
		fy_mmFree(block, tmp);
	}
	t3 = fy_portTimeMillSec(port);
	fy_hashMapDestroy(block, hashMap);
	t4 = fy_portTimeMillSec(port);
	printf("HashMap time %"FY_PRINT64"d %"FY_PRINT64"d %"FY_PRINT64"d\n",
			(t2 - t1), (t3 - t2), (t4 - t3));
	CU_ASSERT_EQUAL(blocks, fy_getAllocated());
	fy_mmFree(block, hashMap);
}

CU_TestInfo testcases[] = { { "platform related", testPortable }, //
		{ "memory management", testMemManage }, //
		{ "string", testString }, //
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
		CU_set_output_filename("TestUtil");
		CU_list_tests_to_file();
		CU_automated_run_tests();
		CU_cleanup_registry();
	}
	return 0;
}

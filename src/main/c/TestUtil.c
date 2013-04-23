/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of libfisce.
 *
 * libfisce is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * libfisce is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libfisce.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <time.h>
#include "fisceprt.h"
#include "fy_util/HashMap.h"
#include "fy_util/HashMapI.h"
#include "fy_util/LnkList.h"
#include "fy_util/String.h"
#include "fy_util/Utf8.h"
#include "fy_util/ArrList.h"
#include "fy_util/BitSet.h"
#include <assert.h>
#include <time.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

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

static fy_memblock *block;
static fy_port *port;
static void dumpException(fy_exception *exception) {
	if (exception == NULL || exception->exceptionType == exception_none) {
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
	FY_ASSERT(sizeof(fy_ubyte)== 1);
	FY_ASSERT(sizeof(fy_char)== 2);
	FY_ASSERT(sizeof(fy_uint)== 4);
	FY_ASSERT(sizeof(fy_ulong)== 8);
	FY_ASSERT(sizeof(fy_byte)== 1);
	FY_ASSERT(sizeof(fy_short)== 2);
	FY_ASSERT(sizeof(fy_int)== 4);
	FY_ASSERT(sizeof(fy_long)== 8);
	FY_ASSERT(sizeof(fy_float)== 4);
	FY_ASSERT(sizeof(fy_double)== 8);
	FY_ASSERT(fy_I2TOL(0x12345678,0x9ABCDEF0)== (fy_long) 0x123456789ABCDEF0LL);
	FY_ASSERT(fy_I2TOL(0x9ABCDEF0,0x12345678)== (fy_long) 0x9ABCDEF012345678LL);
	FY_ASSERT(fy_I2TOL(0x12345678,0x22345678)== (fy_long) 0x1234567822345678LL);
	FY_ASSERT(fy_I2TOL(0x92345678,0x9ABCDEF0)== (fy_long) 0x923456789ABCDEF0LL);
	FY_ASSERT(
			fy_I2TOUL(0x12345678,0x9ABCDEF0)== (fy_ulong) 0x123456789ABCDEF0LL);
	FY_ASSERT(
			fy_I2TOUL(0x9ABCDEF0,0x12345678)== (fy_ulong) 0x9ABCDEF012345678LL);
	FY_ASSERT(
			fy_I2TOUL(0x12345678,0x22345678)== (fy_ulong) 0x1234567822345678LL);
	FY_ASSERT(
			fy_I2TOUL(0x92345678,0x9ABCDEF0)== (fy_ulong) 0x923456789ABCDEF0LL);
	FY_ASSERT(fy_HOFL(0x123456789ABCDEF0LL)== 0x12345678);
	FY_ASSERT(fy_LOFL(0x123456789ABCDEF0LL)== 0x9ABCDEF0);
	FY_ASSERT(fy_HOFL(0x9ABCDEF012345678LL)== 0x9ABCDEF0);
	FY_ASSERT(fy_LOFL(0x9ABCDEF012345678LL)== 0x12345678);
	FY_ASSERT(fy_HOFL(0x1234567822345678LL)== 0x12345678);
	FY_ASSERT(fy_LOFL(0x1234567822345678LL)== 0x22345678);
	FY_ASSERT(fy_HOFL(0x9ABCDEF08ABCDEF0LL)== 0x9ABCDEF0);
	FY_ASSERT(fy_LOFL(0x9ABCDEF08ABCDEF0LL)== 0x8ABCDEF0);
	FY_ASSERT(fy_B2TOUI(0x12,0xAB)== 0x12AB);
	FY_ASSERT(fy_B2TOUI(0xAB,0x12)== 0xAB12);
	FY_ASSERT(fy_B2TOUI(0x12,0x12)== 0x1212);
	FY_ASSERT(fy_B2TOUI(0xAB,0xAB)== 0xABAB);
	FY_ASSERT(fy_B2TOI(0x12,0xAB)== (fy_short) 0x12AB);
	FY_ASSERT(fy_B2TOI(0xAB,0x12)== (fy_short) 0xAB12);
	FY_ASSERT(fy_B2TOI(0x12,0x12)== (fy_short) 0x1212);
	FY_ASSERT(fy_B2TOI(0xAB,0xAB)== (fy_short) 0xABAB);
	FY_ASSERT(fy_B4TOI(0x12,0xAB,0x34,0xCD)== 0x12AB34CD);
	FY_ASSERT(fy_B4TOI(0xAB,0x12,0xCD,0x34)== 0xAB12CD34);
	FY_ASSERT(fy_B4TOI(0xAB,0xCD,0x89,0xEF)== 0xABCD89EF);
	FY_ASSERT(fy_B4TOI(0x12,0x34,0x56,0x78)== 0x12345678);
}

void testMemManage() {
	int i;
	void *address[17];

	fy_exception ex;
	fy_exception *exception = &ex;

	printf("TestMemManage\n");
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
	printf("TestMemString\n");
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
		printf("%d ", (fy_uint) fy_strGet(js,i));
	}
	printf("\n");
	fy_strPrint(js);
	FY_ASSERT(fy_strCmp(js,js)==0);
	FY_ASSERT(fy_strCmp(js1,js)<0);
	FY_ASSERT(fy_strCmp(js,js1)>0);
	FY_ASSERT(fy_strCmp(js2,js)>0);
	FY_ASSERT(fy_strCmp(js,js2)<0);
}

typedef struct fy_test_map {
	fy_uint count;
	fy_uint *values;
} fy_test_map;

static void checkMapValue(fy_str *key, void *value, void *addition) {
	fy_test_map *test = (fy_test_map*) addition;
	fy_uint val = *(fy_uint*) value;
	test->count++;
	FY_ASSERT(val>=0 && val <30000);
}

void testHashMap() {
	/*int blocks;*/
	int i;
	char buf[256];
	//	char buf1[256];
	fy_uint values[10000];
	fy_uint *value;
	fy_str tmp[1];
	fy_exception ex;
	fy_exception *exception = &ex;
	ex.exceptionType = exception_none;
	fy_test_map add;
	fy_hashMap *hashMap = fy_mmAllocate(block, sizeof(fy_hashMap), exception);
	printf("TestHashMap\n");
	CHECK_EXCEPTION(exception);
	memset(buf, 0, 256);
	/*blocks = fy_getAllocated();*/
	fy_hashMapInit(block, hashMap, 16, 12, exception);
	CHECK_EXCEPTION(exception);

	fy_long t1, t2, t3, t4, t5;

	memset(&add, 0, sizeof(add));
	add.values = values;

	t1 = fy_portTimeMillSec(port);
	for (i = 0; i < 10000; i++) {
		sprintf_s(buf, 10, "%d", i);
		tmp->content = NULL;
		fy_strInitWithUTF8(block, tmp, buf, exception);
		CHECK_EXCEPTION(exception);
		fy_hashMapPut(block, hashMap, tmp, values + i, exception);
		CHECK_EXCEPTION(exception);
		fy_strDestroy(block, tmp);
		values[i] = i * 3;
	}

	t2 = fy_portTimeMillSec(port);
	fy_hashMapEachValue(block, hashMap, checkMapValue, &add);
	FY_ASSERT_FATAL(add.count == 10000);
	t3 = fy_portTimeMillSec(port);
	FY_ASSERT(hashMap->size == 10000);
	for (i = 0; i < 15000; i++) {
		sprintf_s(buf, 10, "%d", i);
		tmp->content = NULL;
		fy_strInitWithUTF8(block, tmp, buf, exception);
		CHECK_EXCEPTION(exception);
		value = fy_hashMapGet(block, hashMap, tmp);
		if (i < 10000) {
			FY_ASSERT(*value == values[i])
		} else {
			FY_ASSERT(value == NULL);
		}
		fy_strDestroy(block, tmp);
	}
	t4 = fy_portTimeMillSec(port);
	/*fy_hashMapDestroy(block, hashMap);*/
	t5 = fy_portTimeMillSec(port);
	printf(
			"HashMap time %"FY_PRINT64"d %"FY_PRINT64"d %"FY_PRINT64"d %"FY_PRINT64"d\n",
			(t2 - t1), (t3 - t2), (t4 - t3), (t5 - t4));
	/*FY_ASSERT(blocks == fy_getAllocated());*/
	fy_mmFree(block, hashMap);
}

static void checkMapValueI(fy_int key, fy_int value, fy_int nullValue,
		void *addition) {
	fy_test_map *test = (fy_test_map*) addition;
	test->count++;
	FY_ASSERT(value>=0 && value <30000);
}

void testHashMapI() {
	int blocks;
	int i;
	fy_int value;
	//	char buf1[256];
	fy_uint values[10000];
	fy_exception ex;
	fy_exception *exception = &ex;
	ex.exceptionType = exception_none;
	fy_test_map add;
	fy_hashMapI *hashMapI = fy_mmAllocate(block, sizeof(fy_hashMapI),
			exception);
	printf("TestHashMap\n");
	CHECK_EXCEPTION(exception);
	blocks = fy_getAllocated();
	fy_hashMapIInit(block, hashMapI, 16, 12, -1, exception);
	CHECK_EXCEPTION(exception);

	fy_long t1, t2, t3, t4, t5;

	memset(&add, 0, sizeof(add));
	add.values = values;

	t1 = fy_portTimeMillSec(port);
	for (i = 0; i < 10000; i++) {
		fy_hashMapIPut(block, hashMapI, i, i * 3, exception);
		CHECK_EXCEPTION(exception);
		values[i] = i * 3;
	}

	t2 = fy_portTimeMillSec(port);
	fy_hashMapIEachValue(block, hashMapI, checkMapValueI, &add);
	FY_ASSERT_FATAL(add.count == 10000);
	t3 = fy_portTimeMillSec(port);
	FY_ASSERT(hashMapI->size == 10000);
	for (i = 0; i < 15000; i++) {
		value = fy_hashMapIGet(block, hashMapI, i);
		if (i < 10000) {
			FY_ASSERT(value == values[i])
		} else {
			FY_ASSERT(value == -1);
		}
	}
	t4 = fy_portTimeMillSec(port);
	fy_hashMapIDestroy(block, hashMapI);
	t5 = fy_portTimeMillSec(port);
	printf(
			"HashMapI time %"FY_PRINT64"d %"FY_PRINT64"d %"FY_PRINT64"d %"FY_PRINT64"d\n",
			(t2 - t1), (t3 - t2), (t4 - t3), (t5 - t4));
	FY_ASSERT(blocks == fy_getAllocated());
	fy_mmFree(block, hashMapI);
}

void testArrayList() {
	struct stackData {
		int ivalue;
		float fvalue;
	};

	struct stackData data, *data1;
	int i;
	float fs[10];
	fy_arrayList stack;
	fy_exception ex;
	fy_exception *exception = &ex;
	memset(exception, 0, sizeof(ex));
	printf("TestArrayList\n");
	fy_arrayListInit(block, &stack, sizeof(struct stackData), 1, exception);
	CHECK_EXCEPTION(exception);
	for (i = 0; i < 10; i++) {
		data.ivalue = i;
		data.fvalue = fs[i] = i * 1.1f;
		fy_arrayListAdd(block, &stack, &data, exception);
		CHECK_EXCEPTION(exception);
	}

	for (i = 0; i < 10; i++) {
		fy_arrayListGet(block, &stack, i, &data);
		CHECK_EXCEPTION(exception);
		FY_ASSERT(data.ivalue == i);
		FY_ASSERT(data.fvalue == fs[i]);
	}
	for (i = 9; i >= 0; i--) {
		data1 = fy_arrayListPop(block, &stack, NULL);
		FY_ASSERT(data1->ivalue == i);
	}
	for (i = 0; i < 10; i++) {
		data.ivalue = i;
		data.fvalue = fs[i] = i * 1.1f;
		fy_arrayListAdd(block, &stack, &data, exception);
		CHECK_EXCEPTION(exception);
	}
	for (i = 9; i >= 0; i--) {
		data1 = fy_arrayListPop(block, &stack, &data);
		FY_ASSERT(data.ivalue == i);
		FY_ASSERT(data.fvalue == fs[i]);
	}
	data1 = fy_arrayListPop(block, &stack, &data);
	FY_ASSERT(data1 == NULL);
	fy_arrayListDestroy(block, &stack);
}

FY_TEST_FUN testcases[] = { { "platform related", testPortable }, //
		{ "memory management", testMemManage }, //
		{ "string", testString }, //
		{ "hashMap", testHashMap }, //
		{ "hashMapI", testHashMapI }, //
		{ "arrayList", testArrayList }, //
		{ NULL, NULL } };

int main(int argc, char *argv[]) {
	int i = 0;
	FY_TEST_FUN *tf;
	char *name;
	void (*fun)();
	setvbuf(stdout, NULL, _IONBF, 1024);
	fails = fopen("util.fail.log", "w");
	if (fails == NULL) {
		fprintf(stderr, "Can't open util.fail.log for write!\n");
		return 1;
	}
	failCount = 0;
	test_init();
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
	test_clean();
	fclose(fails);
	if (failCount) {
		printf("Test FAILED! %"FY_PRINT64"d\n", failCount);
	}
	return failCount > 0;
}

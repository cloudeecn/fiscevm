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
#include "fyc/Instructions.h"

static const char** dirs;

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

static fy_memblock *block;
static fy_port *port;

#define TEST_EXCEPTION(EXCEPTION) if((EXCEPTION)->exceptionType!=exception_none) { \
	fy_log("Exception %s: %s occored!\n",(EXCEPTION)->exceptionName,(EXCEPTION)->exceptionDesc); \
	FY_ASSERT("Critical exception in global"==NULL); \
	return; \
}

#define TEST_EXCEPTION_RETURN(EXCEPTION, RETURN) if((EXCEPTION)->exceptionType!=exception_none) { \
	fy_log("Exception %s: %s occored!\n",(EXCEPTION)->exceptionName,(EXCEPTION)->exceptionDesc); \
	FY_ASSERT("Critical exception in global"==NULL); \
	return RETURN; \
}

void test_init(void) {
	fy_exception exception;
	exception.exceptionType = exception_none;
	block = fy_allocate(sizeof(fy_memblock), &exception);
	TEST_EXCEPTION(&exception);
	fy_mmInit(block, &exception);
	TEST_EXCEPTION(&exception);
	port = fy_mmAllocate(block, sizeof(fy_port), &exception);
	TEST_EXCEPTION(&exception);
	fy_portInit(port);
}

void test_clean(void) {
	printf("Release resources\n");
	fy_mmDestroy(block);
	fy_free(block);
	printf("ALL TEST DONE!!!MemLeak=%ld\n", fy_getAllocated());
}

void testPortable() {
	FY_ASSERT(sizeof(fy_ubyte) == 1);
	FY_ASSERT(sizeof(fy_char) == 2);
	FY_ASSERT(sizeof(fy_uint) == 4);
	FY_ASSERT(sizeof(fy_ulong) == 8);
	FY_ASSERT(sizeof(fy_byte) == 1);
	FY_ASSERT(sizeof(fy_short) == 2);
	FY_ASSERT(sizeof(fy_int) == 4);
	FY_ASSERT(sizeof(fy_long) == 8);
	FY_ASSERT(sizeof(fy_float) == 4);
	FY_ASSERT(sizeof(fy_double) == 8);
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
		TEST_EXCEPTION(exception);
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
	TEST_EXCEPTION(exception);
	fy_strInit(block, js, 1, exception);
	TEST_EXCEPTION(exception);
	fy_strInit(block, js1, 1, exception);
	TEST_EXCEPTION(exception);
	fy_strInit(block, js2, 1, exception);
	TEST_EXCEPTION(exception);
	fy_strAppendUTF8(block, js, cc, 999, exception);
	TEST_EXCEPTION(exception);
	fy_strAppendUTF8(block, js1, cc1, 999, exception);
	TEST_EXCEPTION(exception);
	fy_strAppendUTF8(block, js2, cc2, 999, exception);
	TEST_EXCEPTION(exception);
	for (i = 0; i < js->length; i++) {
		printf("%d ", (fy_uint) fy_strGet(js, i));
	}
	printf("\n");
	fy_strPrint(js);
	FY_ASSERT(fy_strCmp(js, js) == 0);
	FY_ASSERT(fy_strCmp(js1, js) < 0);
	FY_ASSERT(fy_strCmp(js, js1) > 0);
	FY_ASSERT(fy_strCmp(js2, js) > 0);
	FY_ASSERT(fy_strCmp(js, js2) < 0);
}

typedef struct fy_test_map {
	fy_uint count;
	fy_uint *values;
} fy_test_map;

static void checkMapValue(fy_str *key, void *value, void *addition) {
	fy_test_map *test = (fy_test_map*) addition;
	fy_uint val = *(fy_uint*) value;
	test->count++;
	FY_ASSERT(val >= 0 && val < 30000);
}

void testNativeHashMap() {
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
	TEST_EXCEPTION(exception);
	memset(buf, 0, 256);
	/*blocks = fy_getAllocated();*/
	fy_hashMapInit(block, hashMap, 16, 12, exception);
	TEST_EXCEPTION(exception);

	fy_long t1, t2, t3, t4, t5;

	memset(&add, 0, sizeof(add));
	add.values = values;

	t1 = fy_portTimeMillSec(port);
	for (i = 0; i < 10000; i++) {
		sprintf_s(buf, 10, "%d", i);
		tmp->content = NULL;
		fy_strInitWithUTF8(block, tmp, buf, exception);
		TEST_EXCEPTION(exception);
		fy_hashMapPut(block, hashMap, tmp, values + i, exception);
		TEST_EXCEPTION(exception);
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
		TEST_EXCEPTION(exception);
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
	FY_ASSERT(value >= 0 && value < 30000);
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
	TEST_EXCEPTION(exception);
	blocks = fy_getAllocated();
	fy_hashMapIInit(block, hashMapI, 16, 12, -1, exception);
	TEST_EXCEPTION(exception);

	fy_long t1, t2, t3, t4, t5;

	memset(&add, 0, sizeof(add));
	add.values = values;

	t1 = fy_portTimeMillSec(port);
	for (i = 0; i < 10000; i++) {
		fy_hashMapIPut(block, hashMapI, i, i * 3, exception);
		TEST_EXCEPTION(exception);
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
	TEST_EXCEPTION(exception);
	for (i = 0; i < 10; i++) {
		data.ivalue = i;
		data.fvalue = fs[i] = i * 1.1f;
		fy_arrayListAdd(block, &stack, &data, exception);
		TEST_EXCEPTION(exception);
	}

	for (i = 0; i < 10; i++) {
		fy_arrayListGet(block, &stack, i, &data);
		TEST_EXCEPTION(exception);
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
		TEST_EXCEPTION(exception);
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

static void testAllocate1() {
	exception = fy_allocate(sizeof(fy_exception), NULL);
	context = fy_allocate(sizeof(fy_context), NULL);
	fy_vmContextInit(context, exception);
	TEST_EXCEPTION(exception);
	context->isParam = dirs;

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
			"int", "double", FY_BASE_DOUBLE, FY_BASE_MATH, NULL };
	int i = 0;
	char *nm;
	fy_str snm[1];
	fy_class *clazz;
	fy_class *clStr;
	fy_class *clObj;
	exception->exceptionType = exception_none;
	while ((nm = names[i++]) != NULL) {
		DLOG(context, "###Full loading class %s\n", nm);
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
	FY_ASSERT(fy_classCanCastTo(context, clStr, clObj, TRUE));
	FY_ASSERT(!fy_classCanCastTo(context, clStr, clazz, TRUE));
}

void testClassMethod() {
	fy_class *t, *class0, *class1, *class2, *class3, *class4, *class5, *class6;
	fy_method **methods;
	fy_method *target = NULL;
	fy_method *method;
	fy_str sComplex[1];
	int i;
	exception->exceptionType = exception_none;
	sComplex->content = NULL;
	fy_strInitWithUTF8(block, sComplex, "complex", exception);
	TEST_EXCEPTION(exception);

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
	fy_strDestroy(block, sComplex);
	FY_ASSERT(target!= NULL);
	FY_ASSERT(target->paramStackUsage == 8);
	class0 = lookup(context, ""FY_BASE_OBJECT"", exception);
	class1 = lookup(context, "[[B", exception);
	class2 = lookup(context, "[[C", exception);
	class3 = lookup(context, "[[[B", exception);
	class4 = lookup(context, "[[L"FY_BASE_OBJECT";", exception);
	class5 = lookup(context, "[[LEXCLUDE/fisce/test/ITester;", exception);
	class6 = lookup(context, "[[LEXCLUDE/fisce/test/Tester;", exception);
	lookup(context, "[[LEXCLUDE/fisce/test/TesterChild;", exception);
	FY_ASSERT(!fy_classCanCastTo(context, class1, class2, TRUE));
	FY_ASSERT(!fy_classCanCastTo(context, class1, class3, TRUE));
	FY_ASSERT(fy_classCanCastTo(context, class1, class0, TRUE));
	FY_ASSERT(fy_classCanCastTo(context, class3, class4, TRUE));
	FY_ASSERT(fy_classCanCastTo(context, class6, class5, TRUE));
	FY_ASSERT(fy_classCanCastTo(context, class6, class4, TRUE));
	FY_ASSERT(!fy_classCanCastTo(context, class6, class3, TRUE));
	FY_ASSERT(!fy_classCanCastTo(context, class6, class2, TRUE));
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
	TEST_EXCEPTION(exception);ASSERT(sHandle != 0);
	fy_heapGetString(context, sHandle, compare, exception);
	TEST_EXCEPTION(exception);
	FY_ASSERT(fy_strCmp(str, compare) == 0);
	FY_ASSERT(sHandle == fy_heapLiteral(context, compare, NULL));
	fy_strDestroy(block, str);
	fy_strDestroy(block, compare);
}

void testPreverifier() {
	const char *testClass = "java/util/Properties";
	const char *testMethod = "java/util/Properties.loadImpl.(Ljava/io/Reader;)V";
	fy_str className[1], methodName[1];
	fy_method *method;

	exception->exceptionType = exception_none;
	className->content = NULL;
	methodName->content = NULL;
	fy_strInitWithUTF8(block, className, testClass, exception);
	TEST_EXCEPTION(exception);
	fy_strInitWithUTF8(block, methodName, testMethod, exception);
	TEST_EXCEPTION(exception);
	fy_clLoadclass(context, className, exception);
	TEST_EXCEPTION(exception);
	method = fy_vmGetMethod(context, methodName);
	ASSERT(method != NULL);
	fy_preverify(context, method, exception);
	TEST_EXCEPTION(exception);
}

static fy_int testFail(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_str str;
	char msg[256];
	memset(&str, 0, sizeof(str));
	fy_strInit(context->memblocks, &str, 256, exception);
	TEST_EXCEPTION_RETURN(exception, 0);
	fy_heapGetString(context, args[0].uvalue, &str, exception);
	TEST_EXCEPTION_RETURN(exception, 0);
	fy_strSPrint(msg, sizeof(msg), &str);
	fy_strDestroy(context->memblocks, &str);
	fy_log("FAIL: %s\n", msg);
	FY_ASSERT("Test fail in high level test."==NULL);
	return ops - 1;

}

#ifdef FY_INSTRUCTION_COUNT
static int compareOpCount(const void *left, const void *right) {
	const fy_instruction_count *l = (fy_instruction_count*) left;
	const fy_instruction_count *r = (fy_instruction_count*) right;
	if (l->count > r->count) {
		return 1;
	} else if (l->count == r->count) {
		return 0;
	} else {
		return -1;
	}
}
static int compairOpPairCount(const void *left, const void *right) {
	const fy_instruction_pair_count *l = (fy_instruction_pair_count*) left;
	const fy_instruction_pair_count *r = (fy_instruction_pair_count*) right;
	if (l->count > r->count) {
		return 1;
	} else if (l->count == r->count) {
		return 0;
	} else {
		return -1;
	}
}
#endif

static void hltest(char *name) {
//	fy_class *clazz;
	fy_message message;
	fy_boolean dead;
	fy_exception ex;
	fy_context *context;
	fy_exception *exception = &ex;
#ifdef FY_INSTRUCTION_COUNT
	fy_engine_repl_data *repl;
	fy_int engineNum;
#endif
	int i = 0;
	fy_long total = 0;
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

	context->isParam = dirs;
	fy_vmRegisterNativeHandler(context,
			"EXCLUDE/fisce/test/TestService.fail0.(L"FY_BASE_STRING";)V", NULL,
			testFail, exception);
	TEST_EXCEPTION(exception);

	if (name == NULL) {
		context->saveloadParam = "save.dat";
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
#if 0
			fy_strSPrint(msg, sizeof(msg),
					message.body.call.method->uniqueName);
			fy_log("Stopped at invoke native for thread %d: %s\n",
					message.thread->threadId, msg);
			FY_ASSERT("Core native functions not implemented"==NULL)
			;
			dead = 1;
#endif
			break;
		case message_sleep:
//			printf("sleep %"FY_PRINT64"dms", message.body.sleepTime);
			break;
		case message_vm_dead:
			fy_log("VM dead\n");
#ifdef FY_INSTRUCTION_COUNT
			for(engineNum = 0; engineNum < FY_ENGINE_COUNT; engineNum++){
				repl = context->engineReplData + engineNum;
				total = 0;
				printf("\n\n\nInstructions pair usage %d:\n", engineNum);
				qsort(repl->instructionPairCount,
				MAX_INSTRUCTIONS * MAX_INSTRUCTIONS,
						sizeof(fy_instruction_pair_count), compairOpPairCount);
				total = 0;
				for (i = 0; i < MAX_INSTRUCTIONS * MAX_INSTRUCTIONS; i++) {
					total += repl->instructionPairCount[i].count;
				}
				printf("Total: %"FY_PRINT64"d\n", total);
				for (i = 0; i < MAX_INSTRUCTIONS * MAX_INSTRUCTIONS; i++) {
					if (repl->instructionPairCount[i].count > 0) {
						repl->instructionCount[repl->instructionPairCount[i].op1].entropy -=
							(double)repl->instructionPairCount[i].count *
							log(
								(double)repl->instructionPairCount[i].count /
								repl->instructionCount[repl->instructionPairCount[i].op1].count
							)/log(2) /
							repl->instructionCount[repl->instructionPairCount[i].op1].count;
						repl->instructionCount[repl->instructionPairCount[i].op2].entropyRev -=
							(double)repl->instructionPairCount[i].count *
							log(
								(double)repl->instructionPairCount[i].count /
								repl->instructionCount[repl->instructionPairCount[i].op2].count
							)/log(2) /
							repl->instructionCount[repl->instructionPairCount[i].op2].count;
						printf("%s(%d) -> %s(%d): %d (%f%%)\n",
								FY_OP_NAME[repl->instructionPairCount[i].op1],
								repl->instructionPairCount[i].op1,
								FY_OP_NAME[repl->instructionPairCount[i].op2],
								repl->instructionPairCount[i].op2,
								repl->instructionPairCount[i].count,
								repl->instructionPairCount[i].count * 100.0
										/ total);
					}
				}

				printf("\n\n\n\nInstructions usage %d:\n", engineNum);
				qsort(repl->instructionCount, MAX_INSTRUCTIONS,
						sizeof(fy_instruction_count), compareOpCount);
				for (i = 0; i < MAX_INSTRUCTIONS; i++) {
					total += repl->instructionCount[i].count;
				}
				printf("Total: %"FY_PRINT64"d\n", total);
				for (i = 0; i < MAX_INSTRUCTIONS; i++) {
					if (repl->instructionCount[i].count > 0) {
						printf("%s(%d): %d (%f%%) entropy: %f%% / %f%% - %f%% / %f%% = %f%% / %f%%\n",
								FY_OP_NAME[repl->instructionCount[i].op],
								repl->instructionCount[i].op,
								repl->instructionCount[i].count,
								repl->instructionCount[i].count * 100.0 / total,
								repl->instructionCount[i].entropy * 100.0,
								repl->instructionCount[i].entropy * 100.0 * repl->instructionCount[i].count / total,
								repl->instructionCount[i].entropyRev * 100.0,
								repl->instructionCount[i].entropyRev * 100.0 * repl->instructionCount[i].count / total,
								repl->instructionCount[i].entropy * 100.0 - repl->instructionCount[i].entropyRev * 100.0,
								(repl->instructionCount[i].entropy * 100.0 - repl->instructionCount[i].entropyRev * 100.0) *
									repl->instructionCount[i].count / total
								);
					}
				}
			}
#endif
			dead = 1;
			break;
		case message_none:
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

static void hltest2(char *name) {
//	fy_class *clazz;
	fy_message message;
	fy_boolean dead;
	fy_exception ex;
	fy_context *context;
	fy_exception *exception = &ex;
	fy_boolean loadMode = FALSE;
	if (name == NULL) {
		fy_log("+++Executing test case Load2+++\n", name);
	} else {
		fy_log("+++Executing test case %s+++\n", name);
	}
	exception->exceptionType = exception_none;
	context = fy_allocate(sizeof(fy_context), exception);
	TEST_EXCEPTION(exception);
	dead = FALSE;
	fy_vmContextInit(context, exception);
	TEST_EXCEPTION(exception);
	context->isParam = dirs;
	fy_vmRegisterNativeHandler(context,
			"EXCLUDE/fisce/test/TestService.fail0.(L"FY_BASE_STRING";)V", NULL,
			testFail, exception);
	TEST_EXCEPTION(exception);

	if (name == NULL) {
		context->saveloadParam = "save2.dat";
		fy_vmBootFromData(context, exception);
		TEST_EXCEPTION(exception);
		loadMode = TRUE;
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
			FY_ASSERT("Core native functions not implemented"==NULL)
			;
			dead = 1;
			break;
		case message_sleep:
//			printf("sleep %"FY_PRINT64"dms", message.body.sleepTime);
			if (!loadMode) {
				message.messageType = message_vm_dead;
				dead = 1;
				context->saveloadParam = "save2.dat";
				fy_vmSave(context, exception);
				TEST_EXCEPTION(exception);
			}
			break;
		case message_vm_dead:
			fy_log("VM dead\n");
			dead = 1;
			break;
		case message_none:
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

void testHierarchy() {
	hltest("EXCLUDE/fisce/test/HierarchyTest");
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

void testSaveLoad() {
	hltest("EXCLUDE/fisce/test/SaveTest");
	hltest(NULL);
}

void testSaveLoad2() {
	hltest2("com/cirnoworks/fisce/privat/Linpack");
	hltest2(NULL);
}

void testRIS() {
	hltest("EXCLUDE/fisce/test/RISTest");
	hltest(NULL);
}

void testStore() {
	hltest("EXCLUDE/fisce/test/ParamStoreTest");
}

void testReflection() {
	hltest("EXCLUDE/fisce/test/ReflectionTest");
}

void testBasicRegex() {
	hltest("EXCLUDE/fisce/test/BasicRegexTest");
}

void testProxy() {
	hltest("EXCLUDE/fisce/test/ProxyTest");
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
			FY_REFLECT_FIELD, FY_REFLECT_METHOD, FY_BASE_FLOAT, FY_BASE_DOUBLE,
			NULL };
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
	context->isParam = dirs;
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
	} else if (strcmp(customTest, "SAVE") == 0) {
		testSaveLoad();
	} else if (strcmp(customTest, "SAVE2") == 0) {
		testSaveLoad2();
	} else if (strcmp(customTest, "RIS") == 0) {
		testRIS();
	} else if (strcmp(customTest, "PERVERIFIER") == 0) {
		testAllocate1();
		testPreverifier();
		testCleanup1();
	} else {
		hltest(customTest);
	}
}

FY_TEST_FUN testcases[] = { //
		{ "init utils test", test_init }, //
				{ "platform related", testPortable }, //
				{ "memory management", testMemManage }, //
				{ "string", testString }, //
				{ "hashMap", testNativeHashMap }, //
				{ "hashMapI", testHashMapI }, //
				{ "arrayList", testArrayList }, //
				{ "cleanup utils test", test_clean }, //
				{ "allocate1", testAllocate1 }, //
				{ "classloader", testClassLoader }, //
				{ "classLoaderFull", testClassLoaderFull }, //
				{ "classMethod", testClassMethod }, //
				{ "preverifier", testPreverifier }, //
				{ "heap", testHeap }, //
				{ "cleanup1", testCleanup1 }, //
				{ "Arch", testArch }, //
				{ "Hierarchy", testHierarchy }, //
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
				{ "StoreParamsToArray", testStore }, //
				{ "Reflection", testReflection }, //
				// { "BasicRegex", testBasicReLoadgex }, //
				{ "Proxy", testProxy }, //
				{ "SaveLoad", testSaveLoad }, //
				{ "SaveLoad2", testSaveLoad2 }, //
				{ "RIS", testRIS }, //
				{ "Native", testNative }, //
				{ NULL, NULL } };
int main(int argc, char *argv[]) {
	int i = 0;
	FY_TEST_FUN *tf;
	char *name;
	void (*fun)();
	char *customTest;
	char *fn;
	setvbuf(stdout, NULL, _IONBF, 1024);

	for (i = argc - 1; i > 0; i--) {
		if (strcmp("--", argv[i]) == 0) {
			argc = i;
			break;
		}
	}

	if (argc == 1) {
		dirs = malloc(sizeof(char*) * 2);
		dirs[0] = "runtime";
		dirs[1] = NULL;
		customTest = NULL;
		fn = "test.fail.log";
	} else if (argc == 2) {
		dirs = malloc(sizeof(char*) * 2);
		dirs[0] = argv[1];
		dirs[1] = NULL;
		customTest = NULL;
		fn = "test.fail.log";
	} else {
		dirs = malloc(sizeof(char*) * argc);
		for (i = 1; i < argc - 1; i++) {
			dirs[i - 1] = argv[i];
		}
		dirs[i - 1] = NULL;
		printf("Testing %s:", argv[argc - 1]);
		customTest = argv[argc - 1];
		fn = "test.custom.fail.log";
	}
	printf("Classpaths:\n");
	i = 0;
	while (1) {
		if (dirs[i] == NULL) {
			break;
		}
		printf("%s\n", dirs[i]);
		i++;
	}
	fails = fopen(fn, "w");
	if (fails == NULL) {
		fprintf(stderr, "Can't open %s for write!\n", fn);
		return 1;
	}
	failCount = 0;
	fp = fopen(customTest == NULL ? "Test.log" : "Test-custom.log", "w");
	if (fp == NULL) {
		printf("Open log file failed\n");
		return 1;
	} else {
		fy_log("Test begin\n");
	}
	if (customTest) {
		printf("Testing %s\n", customTest);
		testCustom(customTest);
	} else {
		i = 0;
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
	free(dirs);
	return failCount > 0;
}

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
#ifndef FY_CLASS_STRUCT_H
#define	FY_CLASS_STRUCT_H

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fisce.h"
#include "fy_util/String.h"
#include "fy_util/HashMapI.h"
#include "fy_util/ArrList.h"
#include "fyc/typedefs.h"

struct fy_nh {
	void *data;
	fy_nhFunction handler;
	fisce_int stack_count;
};

union stringInfo {
	fisce_char string_index;
	fy_str* string;
	fisce_int handle;
};

union classInfo {
	fisce_char name_index;
	fy_str* className;
	ConstantClass *constantClass;
	fy_class *clazz;
};

struct ConstantClass {

	fisce_ubyte derefed;

	classInfo ci;
};

struct ConstantFieldRef {

	fisce_ubyte derefed;

	union {
		fisce_char class_index;
		ConstantClass *constantClass;
		struct fy_class *clazz;
	} c;
	union {
		fisce_char name_type_index;
		ConstantNameAndTypeInfo *constantNameType;
		fy_str* nameType;
	} nt;
	struct fy_field *field;
};

struct ConstantMethodRef {

	fisce_ubyte derefed;

	fisce_char class_index;
	ConstantClass *constantClass;
	fy_class *clazz;

	fisce_char name_type_index;
	ConstantNameAndTypeInfo *constantNameType;
	fy_str* nameType;

	/*The orignal method, not overridden.*/
	fy_method *method;
};

struct ConstantStringInfo {

	fisce_ubyte derefed;

	stringInfo ci;
};

struct ConstantIntegerFloatInfo {
	fisce_int value;
};

struct ConstantLongDoubleInfo {
	fisce_long value;
};

struct ConstantNameAndTypeInfo {

	union {
		fisce_char name_index;
		fy_str* name;
	} n;

	union {
		fisce_char descriptor_index;
		fy_str* descriptor;
	} d;
};

struct ConstantUtf8Info {
	fy_str* string;
};

struct fy_field {
	fisce_uint field_id;
	fisce_char access_flags;

	/*fisce_char name_index;*/
	char* utf8Name;
	fy_str* name;

	/*fisce_char descriptor_index;*/
	fy_str* descriptor;

	fisce_char constant_value_index;

	fy_str* fullName;
	fy_str* uniqueName;

	fy_class* owner;
	fy_class *type;

	fisce_uint posRel;
	fisce_uint posAbs;

};

struct fy_lineNumber {
	fisce_char start_pc;
	fisce_char line_number;
};

struct fy_exceptionHandler {
	fisce_char start_pc;
	fisce_char end_pc;
	fisce_char handler_pc;
	fisce_ubyte catchTypeDerefed;

	classInfo ci;
};

struct fy_stack_map_table {
	fisce_uint length;
	fisce_int count;
	FY_VLS(fisce_ubyte, entries);
};

struct fy_method {
	fisce_int method_id;
	fisce_uint access_flags;

	char* utf8Name;
	fy_str* name;

	fy_str* descriptor;

	fy_str* fullName;
	fy_str* uniqueName;

	fy_class* owner;

	fisce_char max_stack;
	fisce_char max_locals;

	fisce_uint codeLength;
	union {
		fisce_ubyte *code;
		struct{
			fy_instruction *instructions;
			fy_instruction_extra *instruction_extras;
			fisce_short *instruction_ops;
		} i;
		fy_nh *nh;
	} c;
	fy_stack_map_table *stackMapTable;
	fisce_char exception_table_length;
	fy_exceptionHandler *exception_table;

	fisce_char line_number_table_length;
	fy_lineNumber* line_number_table;

	/*The count of the parameters (long/double will be counted as 2 / including this for non-static methods)*/
	fisce_int paramStackUsage;
	fisce_byte *paramTypes;
	fisce_byte returnType;

	/*Used by reflection, contents refrences of class*/
	/*Will not be saved in save-status, as it will be re-initialized when the class is reloaded */
	/*real count parameters (long/double will be counted as 1)*/
	fisce_uint parameterCount;
	fy_arrayList* parameterTypes;
	fy_class *returnTypeClass;

	fy_invoke *invoke;
	fy_engine *engine;
};

typedef enum fy_arrayType {
	fy_at_byte, fy_at_short, fy_at_int, fy_at_long
} fy_arrayType;

struct fy_class {
	/*        _u4 magic;
	 //        _u2 minorVersion;
	 //        _u2 majorVersion;
	 //fields from file
	 *
	 */
	enum type {
		object_class, primitive_class, array_class
	}type;
	fisce_ubyte phase;
	fisce_ubyte needFinalize;
	fisce_char constantPoolCount;
	fisce_ubyte *constantTypes;
	void** constantPools;

	fisce_uint accessFlags;
	ConstantClass* thisClass;
	char* utf8Name;
	fy_str* className;
	union {
		ConstantClass* superClass;
		struct fy_class* super;
	} s;
	fisce_char interfacesCount;
	ConstantClass** interfaceClasses;
	fy_class** interfaces;
	fisce_char fieldCount;
	fy_field** fields;
	/*BEGIN GC Only*/
	fy_field **fieldStatic;
	fy_field **fieldAbs;
	/* END  GC Only*/
	fisce_char methodCount;
	fy_method** methods;
	fy_method* clinit;
	fy_str* sourceFile;
	int classId;

	/*fields filled by classloader*/
	fisce_uint sizeRel;
	fisce_uint sizeAbs;
	fisce_uint ofsInHeap;

	fisce_uint staticSize;
	fisce_int *staticArea;

	union {
		struct {
			fy_arrayType arrayType;
			fy_class *contentClass;
		}arr;
		struct {
			fisce_char pType;
		}prm;
	}ci;

	fy_hashMapI virtualTable[1];

	/*Need persist*/
	fisce_int clinitThreadId;
};

struct fy_object {
	fy_object_data *object_data;
};

#endif

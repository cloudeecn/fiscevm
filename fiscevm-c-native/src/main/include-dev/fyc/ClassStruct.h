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
	fy_int stack_count;
};

union stringInfo {
	fy_char string_index;
	fy_str* string;
	fy_int handle;
};

union classInfo {
	fy_char name_index;
	fy_str* className;
	ConstantClass *constantClass;
	fy_class *clazz;
};

struct ConstantClass {

	fy_ubyte derefed;

	classInfo ci;
};

struct ConstantFieldRef {

	fy_ubyte derefed;

	union {
		fy_char class_index;
		ConstantClass *constantClass;
		struct fy_class *clazz;
	} c;
	union {
		fy_char name_type_index;
		ConstantNameAndTypeInfo *constantNameType;
		fy_str* nameType;
	} nt;
	struct fy_field *field;
};

struct ConstantMethodRef {

	fy_ubyte derefed;

	fy_char class_index;
	ConstantClass *constantClass;
	fy_class *clazz;

	fy_char name_type_index;
	ConstantNameAndTypeInfo *constantNameType;
	fy_str* nameType;

	/*The orignal method, not overridden.*/
	fy_method *method;
};

struct ConstantStringInfo {

	fy_ubyte derefed;

	stringInfo ci;
};

struct ConstantIntegerFloatInfo {
	fy_int value;
};

struct ConstantLongDoubleInfo {
	fy_long value;
};

struct ConstantNameAndTypeInfo {

	union {
		fy_char name_index;
		fy_str* name;
	} n;

	union {
		fy_char descriptor_index;
		fy_str* descriptor;
	} d;
};

struct ConstantUtf8Info {
	fy_str* string;
};

struct fy_field {
	fy_uint field_id;
	fy_char access_flags;

	/*fy_char name_index;*/
	char* utf8Name;
	fy_str* name;

	/*fy_char descriptor_index;*/
	fy_str* descriptor;

	fy_char constant_value_index;

	fy_str* fullName;
	fy_str* uniqueName;

	fy_class* owner;
	fy_class *type;

	fy_uint posRel;
	fy_uint posAbs;

};

struct fy_lineNumber {
	fy_char start_pc;
	fy_char line_number;
};

struct fy_exceptionHandler {
	fy_char start_pc;
	fy_char end_pc;
	fy_char handler_pc;
	fy_ubyte catchTypeDerefed;

	classInfo ci;
};

struct fy_stack_map_table {
	fy_uint length;
	fy_int count;
	FY_VLS(fy_ubyte, entries);
};

struct fy_method {
	fy_int method_id;
	fy_uint access_flags;

	char* utf8Name;
	fy_str* name;

	fy_str* descriptor;

	fy_str* fullName;
	fy_str* uniqueName;

	fy_class* owner;

	fy_char max_stack;
	fy_char max_locals;

	fy_uint codeLength;
	union {
		fy_ubyte *code;
		struct{
			fy_instruction *instructions;
			fy_instruction_extra *instruction_extras;
			fy_short *instruction_ops;
		} i;
		fy_nh *nh;
	} c;
	fy_stack_map_table *stackMapTable;
	fy_char exception_table_length;
	fy_exceptionHandler *exception_table;

	fy_char line_number_table_length;
	fy_lineNumber* line_number_table;

	/*The count of the parameters (long/double will be counted as 2 / including this for non-static methods)*/
	fy_int paramStackUsage;
	fy_byte *paramTypes;
	fy_byte returnType;

	/*Used by reflection, contents refrences of class*/
	/*Will not be saved in save-status, as it will be re-initialized when the class is reloaded */
	/*real count parameters (long/double will be counted as 1)*/
	fy_uint parameterCount;
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
	fy_ubyte phase;
	fy_ubyte needFinalize;
	fy_char constantPoolCount;
	fy_ubyte *constantTypes;
	void** constantPools;

	fy_uint accessFlags;
	ConstantClass* thisClass;
	char* utf8Name;
	fy_str* className;
	union {
		ConstantClass* superClass;
		struct fy_class* super;
	} s;
	fy_char interfacesCount;
	ConstantClass** interfaceClasses;
	fy_class** interfaces;
	fy_char fieldCount;
	fy_field** fields;
	/*BEGIN GC Only*/
	fy_field **fieldStatic;
	fy_field **fieldAbs;
	/* END  GC Only*/
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
			fy_arrayType arrayType;
			fy_class *contentClass;
		}arr;
		struct {
			fy_char pType;
		}prm;
	}ci;

	fy_hashMapI virtualTable[1];

	/*Need persist*/
	fy_int clinitThreadId;
};

struct fy_object {
	fy_object_data *object_data;
};

#endif

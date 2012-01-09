/**
 *  Copyright 2010-2011 Yuxuan Huang. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "fyc/ClassLoader.h"

#ifdef _DEBUG
static int checkConstantBonud(fy_class *clazz, int idx, fy_exception *exception) {
	if (idx > clazz->constantPoolCount) {
		fy_fault(exception, NULL, "Constant index out of bound %d/%d", idx,
				clazz->constantPoolCount);
	}
	return idx;
}
#else
#define checkConstantBonud(C,I,E) (I)
#endif

static fy_arrayType getSizeShiftForArray(fy_str *arrayName,
		fy_exception *exception) {
	switch (arrayName->content[1]) {
	case FY_TYPE_BOOLEAN:
	case FY_TYPE_BYTE:
		return fy_at_byte;
	case FY_TYPE_DOUBLE:
	case FY_TYPE_LONG:
		return fy_at_long;
	case FY_TYPE_CHAR:
	case FY_TYPE_SHORT:
		return fy_at_short;
	case FY_TYPE_INT:
	case FY_TYPE_FLOAT:
	case FY_TYPE_HANDLE:
	case FY_TYPE_ARRAY:
		return fy_at_int;
	default:
		fy_fault(exception, NULL, "Illegal array type: %c(%d)",
				arrayName->content[1], arrayName->content[1]);
		return 0;
	}
}

#if 0
static int getSizeFromDescriptor(fy_str *descriptor) {
	switch (descriptor->content[0]) {
		case TYPE_LONG:
		case TYPE_DOUBLE:
		return 2;
		default:
		return 1;
	}
}
#endif

static void fillConstantContent(fy_context *context, fy_class *ret, void *is,
		fy_exception *exception) {
	int i, imax, j;
	int cpSkip;
	int tag;
	void *tmp, *dataBuffer;
	fy_memblock *block = context->memblocks;
	ConstantClass* tmpConstantClass;
	ConstantFieldRef* tmpConstantFieldRef;
	ConstantMethodRef* tmpConstantMethodRef;
	ConstantStringInfo* tmpConstantStringInfo;
	ConstantIntegerFloatInfo* tmpConstantIntegerFloatInfo;
	ConstantLongDoubleInfo* tmpConstantLongDoubleInfo;
	ConstantNameAndTypeInfo* tmpConstantNameAndTypeInfo;
	ConstantUtf8Info* tmpConstantUtf8Info;
	void **constantPools;
	fy_ubyte *constantTypes;

	ret->constantPoolCount = fy_dataRead2(context, is, exception);

	/*read constantPool*/
	ret->constantTypes = constantTypes = fy_mmAllocate(block,
			sizeof(fy_ubyte) * (ret->constantPoolCount + 1), exception);
	fy_exceptionCheckAndReturn(exception);
	ret->constantPools = constantPools = fy_mmAllocate(block,
			sizeof(void*) * (ret->constantPoolCount + 1), exception);
	fy_exceptionCheckAndReturn(exception);
	/*Phase 1*/
	for (i = 1, imax = ret->constantPoolCount; i < imax; i++) {
		tag = fy_dataRead(context, is, exception);
		fy_exceptionCheckAndReturn(exception);
		constantTypes[i] = tag;
		cpSkip = 0;
		switch (tag) {
		case CONSTANT_Class:
			tmpConstantClass = fy_mmAllocate(block, sizeof(ConstantClass),
					exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantClass->ci.name_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			fy_exceptionCheckAndReturn(exception);

			tmp = tmpConstantClass;
			break;
		case CONSTANT_Fieldref:
			tmpConstantFieldRef = fy_mmAllocate(block, sizeof(ConstantFieldRef),
					exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantFieldRef->class_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantFieldRef->name_type_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			fy_exceptionCheckAndReturn(exception);
			tmp = tmpConstantFieldRef;
			break;
		case CONSTANT_Methodref:
		case CONSTANT_InterfaceMethodref:
			tmpConstantMethodRef = fy_mmAllocate(block,
					sizeof(ConstantMethodRef), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantMethodRef->class_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantMethodRef->name_type_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			fy_exceptionCheckAndReturn(exception);
			tmp = tmpConstantMethodRef;
			break;
		case CONSTANT_String:
			tmpConstantStringInfo = fy_mmAllocate(block,
					sizeof(ConstantStringInfo), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantStringInfo->ci.string_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			fy_exceptionCheckAndReturn(exception);
			tmp = tmpConstantStringInfo;
			break;
		case CONSTANT_Integer:
		case CONSTANT_Float:
			tmpConstantIntegerFloatInfo = fy_mmAllocate(block,
					sizeof(ConstantIntegerFloatInfo), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantIntegerFloatInfo->value = fy_dataRead4(context, is,
					exception);
			fy_exceptionCheckAndReturn(exception);
			tmp = tmpConstantIntegerFloatInfo;
			break;
		case CONSTANT_Double:
		case CONSTANT_Long:
			cpSkip = 1;
			tmpConstantLongDoubleInfo = fy_mmAllocate(block,
					sizeof(ConstantLongDoubleInfo), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantLongDoubleInfo->value = fy_dataRead8(context, is,
					exception);
			fy_exceptionCheckAndReturn(exception);
			tmp = tmpConstantLongDoubleInfo;
			break;
		case CONSTANT_NameAndType:
			tmpConstantNameAndTypeInfo = fy_mmAllocate(block,
					sizeof(ConstantNameAndTypeInfo), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantNameAndTypeInfo->name_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantNameAndTypeInfo->descriptor_index = checkConstantBonud(
					ret, fy_dataRead2(context, is, exception), exception);
			fy_exceptionCheckAndReturn(exception);
			tmp = tmpConstantNameAndTypeInfo;
			break;
		case CONSTANT_Utf8:
			tmpConstantUtf8Info = fy_mmAllocate(block, sizeof(ConstantUtf8Info),
					exception);
			fy_exceptionCheckAndReturn(exception);
			j = fy_dataRead2(context, is, exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantUtf8Info->string = fy_mmAllocate(block, sizeof(fy_str),
					exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strInit(block, tmpConstantUtf8Info->string, j * 3, exception);
			fy_exceptionCheckAndReturn(exception);
			dataBuffer = fy_allocate(j, exception);
			fy_exceptionCheckAndReturn(exception);
			fy_dataReadBlock(context, is, dataBuffer, j, exception);
			if (exception->exceptionType != exception_none) {
				fy_free(dataBuffer);
				return;
			}
			fy_strAppendUTF8(block, tmpConstantUtf8Info->string, dataBuffer, j,
					exception);
			fy_free(dataBuffer);
			fy_exceptionCheckAndReturn(exception);
			tmp = tmpConstantUtf8Info;
			break;
		default:
			fy_fault(exception, NULL, "Unknown constant pool type %d", tag);
			/*make compiler happy*/
			return;
		}
		constantPools[i] = tmp;
		if (cpSkip) {
			i++;
		}
	}

	/*Phase2*/
	for (i = 1, imax = ret->constantPoolCount; i < imax; i++) {
		tmp = constantPools[i];
		tag = constantTypes[i];
		switch (tag) {
		case CONSTANT_Class:
			tmpConstantClass = (ConstantClass*) tmp;
			tmpConstantClass->ci.className =
					((ConstantUtf8Info*) (constantPools[tmpConstantClass->ci.name_index]))->string;
			break;
		case CONSTANT_Fieldref:
		case CONSTANT_Methodref:
		case CONSTANT_InterfaceMethodref:
			break;
		case CONSTANT_String:
			tmpConstantStringInfo = (ConstantStringInfo*) tmp;
			tmpConstantStringInfo->ci.string =
					((ConstantUtf8Info*) (constantPools[tmpConstantStringInfo->ci.string_index]))->string;
			break;
		case CONSTANT_Integer:
		case CONSTANT_Float:
			break;
		case CONSTANT_Double:
		case CONSTANT_Long:
			i++;
			break;
		case CONSTANT_NameAndType:
			tmpConstantNameAndTypeInfo = (ConstantNameAndTypeInfo*) tmp;
			tmpConstantNameAndTypeInfo->name =
					((ConstantUtf8Info*) (constantPools[tmpConstantNameAndTypeInfo->name_index]))->string;
			tmpConstantNameAndTypeInfo->descriptor =
					((ConstantUtf8Info*) (constantPools[tmpConstantNameAndTypeInfo->descriptor_index]))->string;
			break;
		case CONSTANT_Utf8:
			break;
		default:
			fy_fault(exception, NULL, "Unknown constant pool type %d", tag);
			break;
		}
	}
	/*Phase2.1*/
	for (i = 1, imax = ret->constantPoolCount; i < imax; i++) {
		tmp = constantPools[i];
		tag = constantTypes[i];
		switch (tag) {
		case CONSTANT_Class:
			break;
		case CONSTANT_Fieldref:
			tmpConstantFieldRef = (ConstantFieldRef*) tmp;
			tmpConstantFieldRef->constantClass =
					(ConstantClass*) (constantPools[tmpConstantFieldRef->class_index]);
			tmpConstantFieldRef->constantNameType =
					(ConstantNameAndTypeInfo*) (constantPools[tmpConstantFieldRef->name_type_index]);
			tmpConstantFieldRef->nameType = fy_mmAllocate(block, sizeof(fy_str),
					exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strInit(
					block,
					tmpConstantFieldRef->nameType,
					2 + tmpConstantFieldRef->constantNameType->name->length
							+ tmpConstantFieldRef->constantNameType->descriptor->length,
					exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strAppendUTF8(block, tmpConstantFieldRef->nameType, ".", 1,
					exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strAppend(block, tmpConstantFieldRef->nameType,
					tmpConstantFieldRef->constantNameType->name, exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strAppendUTF8(block, tmpConstantFieldRef->nameType, ".", 1,
					exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strAppend(block, tmpConstantFieldRef->nameType,
					tmpConstantFieldRef->constantNameType->descriptor,
					exception);
			fy_exceptionCheckAndReturn(exception);
			break;
		case CONSTANT_Methodref:
		case CONSTANT_InterfaceMethodref:
			tmpConstantMethodRef = (ConstantMethodRef*) tmp;
			tmpConstantMethodRef->constantClass =
					(ConstantClass*) (constantPools[tmpConstantMethodRef->class_index]);
			tmpConstantMethodRef->constantNameType =
					(ConstantNameAndTypeInfo*) (constantPools[tmpConstantMethodRef->name_type_index]);
			tmpConstantMethodRef->nameType = fy_mmAllocate(block,
					sizeof(fy_str), exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strInit(
					block,
					tmpConstantMethodRef->nameType,
					2 + tmpConstantMethodRef->constantNameType->name->length
							+ tmpConstantMethodRef->constantNameType->descriptor->length,
					exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strAppendUTF8(block, tmpConstantMethodRef->nameType, ".", 1,
					exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strAppend(block, tmpConstantMethodRef->nameType,
					tmpConstantMethodRef->constantNameType->name, exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strAppendUTF8(block, tmpConstantMethodRef->nameType, ".", 1,
					exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strAppend(block, tmpConstantMethodRef->nameType,
					tmpConstantMethodRef->constantNameType->descriptor,
					exception);
			fy_exceptionCheckAndReturn(exception);
			break;
		case CONSTANT_String:
		case CONSTANT_Integer:
		case CONSTANT_Float:
			break;
		case CONSTANT_Double:
		case CONSTANT_Long:
			i++;
			break;
		case CONSTANT_NameAndType:
		case CONSTANT_Utf8:
			break;
		default:
			fy_fault(exception, NULL, "Unknown constant pool type %d", tag);
			break;
		}
	}
}

static void loadInterfaces(fy_context *context, fy_class *clazz, void *is,
		fy_exception *exception) {
	int i, count;
	fy_memblock *block = context->memblocks;
	clazz->interfacesCount = count = fy_dataRead2(context, is, exception);
	fy_exceptionCheckAndReturn(exception);
	clazz->interfaces = fy_mmAllocate(block, sizeof(fy_class*) * count,
			exception);
	fy_exceptionCheckAndReturn(exception);
	for (i = 0; i < count; i++) {
		clazz->interfaces[i] = fy_vmLookupClassFromConstant(
				context,
				(ConstantClass*) clazz->constantPools[fy_dataRead2(context, is,
						exception)], exception);
		fy_exceptionCheckAndReturn(exception);
	}
}

static void loadFields(fy_context *context, fy_class *clazz, void *is,
		fy_exception *exception) {
	int i, count, j, countj;
	int length;

	fy_str *attrName;
	fy_uint attrSize;
	fy_field *field;
	fy_uint pos = 0;
	fy_uint staticPos = 0;
	fy_memblock *block = context->memblocks;
	fy_str *strConstantValue = fy_mmAllocate(block, sizeof(fy_str), exception);
	fy_exceptionCheckAndReturn(exception);

	fy_strInit(block, strConstantValue, 13, exception);
	fy_exceptionCheckAndReturn(exception);
	fy_strAppendUTF8(block, strConstantValue, "ConstantValue", 13, exception);
	fy_exceptionCheckAndReturn(exception);

	clazz->fieldCount = count = fy_dataRead2(context, is, exception);
	fy_exceptionCheckAndReturn(exception);
	clazz->fields = fy_mmAllocate(block, sizeof(fy_field*) * count, exception);
	fy_exceptionCheckAndReturn(exception);
	for (i = 0; i < count; i++) {
		field = fy_mmAllocate(block, sizeof(fy_field), exception);
		fy_exceptionCheckAndReturn(exception);
		field->owner = clazz;
		field->access_flags = fy_dataRead2(context, is, exception);
		fy_exceptionCheckAndReturn(exception);
		field->name = ((ConstantUtf8Info*) clazz->constantPools[fy_dataRead2(
				context, is, exception)])->string;
		fy_exceptionCheckAndReturn(exception);
		field->descriptor =
				((ConstantUtf8Info*) clazz->constantPools[fy_dataRead2(context,
						is, exception)])->string;
		fy_exceptionCheckAndReturn(exception);
		field->fullName = fy_mmAllocate(block, sizeof(fy_str), exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strInit(block, field->fullName, 16, exception);
		fy_exceptionCheckAndReturn(exception);
		field->uniqueName = fy_mmAllocate(block, sizeof(fy_str), exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strInit(block, field->uniqueName, 16, exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strAppendUTF8(block, field->fullName, ".", 1, exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strAppend(block, field->fullName, field->name, exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strAppendUTF8(block, field->fullName, ".", 1, exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strAppend(block, field->fullName, field->descriptor, exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strAppend(block, field->uniqueName, clazz->className, exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strAppend(block, field->uniqueName, field->fullName, exception);
		fy_exceptionCheckAndReturn(exception);

		switch (field->descriptor->content[0]) {
		case 'D':
		case 'J':
			length = 2;
			break;
		default:
			length = 1;
			break;
		}
		if (field->access_flags & FY_ACC_STATIC) {
			field->posAbs = staticPos;
			staticPos += length;
		} else {
			field->posRel = pos;
			pos += length;
		}
		countj = fy_dataRead2(context, is, exception);
		fy_exceptionCheckAndReturn(exception);
		for (j = 0; j < countj; j++) {
			attrName = fy_clGetConstantString(context, clazz,
					fy_dataRead2(context, is, exception));
			fy_exceptionCheckAndReturn(exception);
			attrSize = fy_dataRead4(context, is, exception);
			fy_exceptionCheckAndReturn(exception);
			if (fy_strCmp(strConstantValue, attrName) == 0) {
				/*TODO*/
				fy_dataRead2(context, is, exception);
				fy_exceptionCheckAndReturn(exception);
			} else {
				fy_dataSkip(context, is, attrSize, exception);
				fy_exceptionCheckAndReturn(exception);
			}
		}
		fy_vmRegisterField(context, field, exception);
		fy_exceptionCheckAndReturn(exception);
		clazz->fields[i] = field;
	}
	clazz->sizeRel = pos;
	clazz->staticSize = staticPos;
	if (staticPos > 0) {
		clazz->staticArea = fy_mmAllocate(block, staticPos * sizeof(fy_int),
				exception);
		fy_exceptionCheckAndReturn(exception);
	} else {

	}
	fy_strDestroy(block, strConstantValue);
	fy_mmFree(block, strConstantValue);
	strConstantValue = 0;
}

static void countParams(fy_context *context, fy_str *desc, fy_method *method,
		fy_exception *exception) {
	fy_byte *temp;
	fy_byte returnType = FY_TYPE_UNKNOWN;
	int pc = 0;
	fy_char ch;
	int i, maxi;
	fy_boolean begin = FALSE;
	char msg[256];
	temp = fy_allocate(desc->length * sizeof(temp), exception);
	fy_exceptionCheckAndReturn(exception);
	maxi = desc->length;
	for (i = 0; i < maxi; i++) {
		ch = desc->content[i];
		if (!begin) {
			if (ch == '(') {
				begin = TRUE;
			}
		} else {
			if (ch == ')') {
				switch (desc->content[i + 2]) {
				case 'B':
				case 'C':
				case 'F':
				case 'I':
				case 'S':
				case 'Z':
					returnType = FY_TYPE_INT;
					break;
				case 'D':
				case 'J':
					returnType = FY_TYPE_WIDE;
					break;
				case '[':
				case 'L':
					returnType = FY_TYPE_HANDLE;
					break;
				case 'V':
				default:
					returnType = FY_TYPE_UNKNOWN;
					break;
				}
				break;
			} else {
				switch (ch) {
				case 'B':
				case 'C':
				case 'F':
				case 'I':
				case 'S':
				case 'Z':
					temp[pc++] = FY_TYPE_INT;
					break;
				case 'D':
				case 'J':
					temp[pc++] = FY_TYPE_WIDE;
					temp[pc++] = FY_TYPE_WIDE2;
					break;
				case '[':
					while ((ch = desc->content[++i]) == '[') {
					}
					if (ch == 'L') {
						while ((ch = desc->content[++i]) != ';') {
						}
					}
					temp[pc++] = FY_TYPE_HANDLE;
					break;
				case 'L':
					while ((ch = desc->content[++i]) != ';')
						;
					temp[pc++] = FY_TYPE_HANDLE;
					break;
				default:
					msg[0] = 0;
					if (method != NULL) {
						fy_strSPrint(msg, sizeof(msg), method->uniqueName);
					} else {
						fy_strSPrint(msg, sizeof(msg), desc);
					}
					fy_fault(exception, NULL,
							"Malformed description data for %s", msg);
					break;
				}
			}
		}
	}
	if (method != NULL) {
		method->paramTypes = fy_mmAllocate(context->memblocks,
				pc * sizeof(fy_byte), exception);
		fy_exceptionCheckAndReturn(exception);
		method->paramCount = pc;
		memcpy(method->paramTypes, temp, pc * sizeof(fy_byte));
		method->returnType = returnType;
	}
	fy_free(temp);
}

static void loadMethods(fy_context *context, fy_class *clazz, void *is,
		fy_exception *exception) {
	fy_memblock *block = context->memblocks;
	fy_char i, count, j, jcount, k, kcount, l, lcount;
	fy_str *ATT_CODE, *ATT_LINENUM, *ATT_SYNTH;
	fy_str *attrName;
	fy_uint attrSize;
	fy_str *attrNameCode;
	fy_uint attrSizeCode;
	fy_method *method;

	ATT_CODE = fy_strCreateFromUTF8(block, "Code", exception);
	fy_exceptionCheckAndReturn(exception);
	ATT_LINENUM = fy_strCreateFromUTF8(block, "LineNumberTable", exception);
	fy_exceptionCheckAndReturn(exception);
	ATT_SYNTH = fy_strCreateFromUTF8(block, "Synthetic", exception);
	fy_exceptionCheckAndReturn(exception);
	clazz->methodCount = count = fy_dataRead2(context, is, exception);
	fy_exceptionCheckAndReturn(exception);
	clazz->methods = fy_mmAllocate(block, sizeof(fy_method*) * count,
			exception);
	fy_exceptionCheckAndReturn(exception);
#if 0
	printf("Loading %d methods...\n", count);
#endif
	for (i = 0; i < count; i++) {
		method = fy_mmAllocate(block, sizeof(fy_method), exception);
		fy_exceptionCheckAndReturn(exception);
		method->owner = clazz;
		method->access_flags = fy_dataRead2(context, is, exception);
		fy_exceptionCheckAndReturn(exception);
		method->name = fy_clGetConstantString(context, clazz,
				fy_dataRead2(context, is, exception));
		fy_exceptionCheckAndReturn(exception);
		if (fy_strCmp(method->name, context->sClinit) == 0) {
			method->clinit = TRUE;
			clazz->clinit = method;
		}
		method->descriptor = fy_clGetConstantString(context, clazz,
				fy_dataRead2(context, is, exception));
		fy_exceptionCheckAndReturn(exception);
		method->fullName = fy_mmAllocate(block, sizeof(fy_str), exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strInit(block, method->fullName,
				method->name->length + method->descriptor->length + 2,
				exception);
		fy_exceptionCheckAndReturn(exception);
		method->uniqueName = fy_mmAllocate(block, sizeof(fy_str), exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strInit(
				block,
				method->uniqueName,
				method->name->length + method->descriptor->length
						+ clazz->className->length + 2, exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strAppendUTF8(block, method->fullName, ".", 1, exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strAppend(block, method->fullName, method->name, exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strAppendUTF8(block, method->fullName, ".", 1, exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strAppend(block, method->fullName, method->descriptor, exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strAppend(block, method->uniqueName, clazz->className, exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strAppend(block, method->uniqueName, method->fullName, exception);
		fy_exceptionCheckAndReturn(exception);
		jcount = fy_dataRead2(context, is, exception);
		fy_exceptionCheckAndReturn(exception);
		for (j = 0; j < jcount; j++) {
			attrName = fy_clGetConstantString(context, clazz,
					fy_dataRead2(context, is, exception));
			fy_exceptionCheckAndReturn(exception);
			attrSize = fy_dataRead4(context, is, exception);
			fy_exceptionCheckAndReturn(exception);
			if (fy_strCmp(ATT_CODE, attrName) == 0) {
				method->max_stack = fy_dataRead2(context, is, exception);
				fy_exceptionCheckAndReturn(exception);
				method->max_locals = fy_dataRead2(context, is, exception);
				fy_exceptionCheckAndReturn(exception);
				method->codeLength = fy_dataRead4(context, is, exception);
				fy_exceptionCheckAndReturn(exception);
				method->code = fy_mmAllocate(block, method->codeLength,
						exception);
				fy_exceptionCheckAndReturn(exception);
				fy_dataReadBlock(context, is, method->code, method->codeLength,
						exception);
				fy_exceptionCheckAndReturn(exception);

				kcount = fy_dataRead2(context, is, exception);
				fy_exceptionCheckAndReturn(exception);
				method->exception_table_length = kcount;
				method->exception_table = fy_mmAllocate(block,
						sizeof(fy_exceptionHandler) * kcount, exception);
				fy_exceptionCheckAndReturn(exception);
				for (k = 0; k < kcount; k++) {
					method->exception_table[k].start_pc = fy_dataRead2(context,
							is, exception);
					fy_exceptionCheckAndReturn(exception);
					method->exception_table[k].end_pc = fy_dataRead2(context,
							is, exception);
					fy_exceptionCheckAndReturn(exception);
					method->exception_table[k].handler_pc = fy_dataRead2(
							context, is, exception);
					fy_exceptionCheckAndReturn(exception);
					method->exception_table[k].ci.constantClass =
							(ConstantClass*) clazz->constantPools[fy_dataRead2(
									context, is, exception)];
					fy_exceptionCheckAndReturn(exception);
				}
				kcount = fy_dataRead2(context, is, exception);
				fy_exceptionCheckAndReturn(exception);
				for (k = 0; k < kcount; k++) {
					attrNameCode = fy_clGetConstantString(context, clazz,
							fy_dataRead2(context, is, exception));
					fy_exceptionCheckAndReturn(exception);
					attrSizeCode = fy_dataRead4(context, is, exception);
					fy_exceptionCheckAndReturn(exception);
					if (fy_strCmp(ATT_LINENUM, attrNameCode) == 0) {
						lcount = fy_dataRead2(context, is, exception);
						fy_exceptionCheckAndReturn(exception);
						method->line_number_table = fy_mmAllocate(block,
								sizeof(fy_lineNumber) * lcount, exception);
						fy_exceptionCheckAndReturn(exception);
						method->line_number_table_length = lcount;
						for (l = 0; l < lcount; l++) {
							method->line_number_table[l].start_pc =
									fy_dataRead2(context, is, exception);
							fy_exceptionCheckAndReturn(exception);
							method->line_number_table[l].line_number =
									fy_dataRead2(context, is, exception);
							fy_exceptionCheckAndReturn(exception);
						}
					} else {
						fy_dataSkip(context, is, attrSizeCode, exception);
						fy_exceptionCheckAndReturn(exception);
					}
				}
			} else if (fy_strCmp(ATT_SYNTH, attrName) == 0) {
				method->synthetic = 1;
			} else {
				fy_dataSkip(context, is, attrSize, exception);
				fy_exceptionCheckAndReturn(exception);
			}
		}
		countParams(context, method->descriptor, method, exception);
		fy_exceptionCheckAndReturn(exception);
		fy_vmRegisterMethod(context, method, exception);
		fy_exceptionCheckAndReturn(exception);
		clazz->methods[i] = method;
	}fy_strRelease(block, ATT_CODE);
	fy_strRelease(block, ATT_LINENUM);
	fy_strRelease(block, ATT_SYNTH);
}

/************public***************/
fy_str *fy_clGetConstantString(fy_context *context, fy_class *clazz,
		fy_char idx) {
	return ((ConstantUtf8Info*) clazz->constantPools[idx])->string;
}

void *fy_clOpenResource(fy_context *context, fy_str *name,
		fy_exception *exception) {
	int i, max;
	int size = 0;
	char *cname;
	void *ret;

	for (i = 0, max = name->length; i < max; i++) {
		size += fy_utf8Size(name->content[i]);
	}
	cname = fy_allocate(size + 1, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fy_strSPrint(cname, size + 1, name);
	ret = context->isOpen(context, cname, exception);
	fy_free(cname);
	fy_exceptionCheckAndReturn(exception)NULL;
	return ret;
}

static fy_class *fy_clLoadclassPriv(fy_context *context, void *is,
		fy_exception *exception) {
	fy_char i, icount;
	fy_str *attrName;
	fy_uint attrSize;
	fy_memblock *block = context->memblocks;
	fy_str *ATTR_SOURCE_FILE = fy_strCreateFromUTF8(block, "SourceFile",
			exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fy_class *clazz = fy_mmAllocate(block, sizeof(fy_class), exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	clazz->type = obj;
	fy_dataSkip(context, is, 8, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fillConstantContent(context, clazz, is, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	clazz->constantPools[0] = NULL;
	clazz->accessFlags = fy_dataRead2(context, is, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	clazz->thisClass = (clazz->constantPools)[fy_dataRead2(context, is,
			exception)];
	fy_exceptionCheckAndReturn(exception)NULL;
	clazz->className = clazz->thisClass->ci.className;
	clazz->superClass = clazz->constantPools[fy_dataRead2(context, is,
			exception)];
	fy_exceptionCheckAndReturn(exception)NULL;
	loadInterfaces(context, clazz, is, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	loadFields(context, clazz, is, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	loadMethods(context, clazz, is, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	icount = fy_dataRead2(context, is, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	for (i = 0; i < icount; i++) {
		attrName = fy_clGetConstantString(context, clazz,
				fy_dataRead2(context, is, exception));
		fy_exceptionCheckAndReturn(exception)NULL;
		attrSize = fy_dataRead4(context, is, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		if (fy_strCmp(ATTR_SOURCE_FILE, attrName) == 0) {
			clazz->sourceFile = fy_clGetConstantString(context, clazz,
					fy_dataRead2(context, is, exception));
			fy_exceptionCheckAndReturn(exception)NULL;
		} else {
			fy_dataSkip(context, is, attrSize, exception);
			fy_exceptionCheckAndReturn(exception)NULL;
		}
	}

	clazz->phase = 1;
	return clazz;
}
void fy_clPhase2(fy_context *context, fy_class *clazz, fy_exception *exception) {
	fy_str *name;
	fy_uint i, pos;
	fy_memblock *block = context->memblocks;
	fy_field *field;
	fy_str *FINALIZE = fy_strCreateFromUTF8(block, ".finalize.()V", exception);
	fy_exceptionCheckAndReturn(exception);
#ifdef _DEBUG
	char buf[255];
#endif
	switch (clazz->type) {
	case arr:
		switch (clazz->className->content[1]) {
		case FY_TYPE_ARRAY:
			name = fy_mmAllocate(block, sizeof(fy_str), exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strInit(block, name, clazz->className->length, exception);
			fy_exceptionCheckAndReturn(exception);
			name->length = clazz->className->length - 1;
			memcpy(name->content, clazz->className->content + 1,
					(clazz->className->length - 1) << 1);
			clazz->ci.arr.contentClass = fy_vmLookupClass(context, name,
					exception);
			fy_strDestroy(block, name);
			fy_mmFree(block, name);
			break;
		case FY_TYPE_HANDLE:
			name = fy_mmAllocate(block, sizeof(fy_str), exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strInit(block, name, clazz->className->length, exception);
			fy_exceptionCheckAndReturn(exception);
			name->length = clazz->className->length - 3;
			memcpy(name->content, clazz->className->content + 2,
					(clazz->className->length - 3) << 1);
			clazz->ci.arr.contentClass = fy_vmLookupClass(context, name,
					exception);
			fy_strDestroy(block, name);
			fy_mmFree(block, name);
			break;
		default:
			clazz->ci.arr.contentClass = fy_vmLookupClass(context,
					context->primitives[clazz->className->content[1]],
					exception);
			break;
		}
		break;
	case obj:
		name = clazz->className;
		if (clazz->superClass != NULL) {
#ifdef _DEBUG
			if (fy_strCmp(context->sTopClass, clazz->className) == 0) {
				fy_fault(exception, NULL,
						"java.lang.Object cannot have super class!", buf);
			}
#endif
			clazz->super = fy_vmLookupClassFromConstant(context,
					clazz->superClass, exception);
			if (exception->exceptionType != exception_none) {
				break;
			}
			clazz->sizeAbs = clazz->super->sizeAbs + clazz->sizeRel;
			for (i = 0; i < clazz->fieldCount; i++) {
				if (clazz->fields[i]->access_flags & FY_ACC_STATIC) {

				} else {
					clazz->fields[i]->posAbs = clazz->super->sizeAbs
							+ clazz->fields[i]->posRel;
				}
			}
			if (fy_vmLookupMethodVirtual(context, clazz, FINALIZE, exception)
					!= fy_vmLookupMethodVirtual(
							context,
							fy_vmLookupClass(context, context->sTopClass,
									exception), FINALIZE, exception)) {
				clazz->needFinalize = 1;
#ifdef _DEBUG
				fy_strSPrint(buf, 255, clazz->className);
				DLOG
				("%s needs finalize!", buf);
#endif
			}
		} else {
#ifdef _DEBUG
			if (fy_strCmp(context->sTopClass, clazz->className) != 0) {
				fy_strSPrint(buf, 255, clazz->className);
				fy_fault(exception, NULL, "%s must have super class!", buf);
			}
#endif
			clazz->sizeAbs = clazz->sizeRel;
			for (i = 0; i < clazz->fieldCount; i++) {
				clazz->fields[i]->posAbs = clazz->fields[i]->posRel;
			}
		}
		/*Add data for gc*/
		clazz->fieldStatic = fy_mmAllocate(block,
				clazz->staticSize * sizeof(fy_field*), exception);
		fy_exceptionCheckAndReturn(exception);

		clazz->fieldAbs = fy_mmAllocate(block,
				clazz->sizeAbs * sizeof(fy_field*), exception);
		fy_exceptionCheckAndReturn(exception);

		for (i = 0; i < clazz->fieldCount; i++) {
			field = clazz->fields[i];
			if (field->access_flags & FY_ACC_STATIC) {
				pos = field->posAbs;
				ASSERT(pos<clazz->staticSize);
				clazz->fieldStatic[pos] = field;
			} else {
				pos = field->posAbs;
				ASSERT(pos<clazz->sizeAbs);
				clazz->fieldAbs[pos] = field;
			}
		}
		if (clazz->super && (i = clazz->super->sizeAbs) > 0) {
			memcpy(clazz->fieldAbs, clazz->super->fieldAbs,
					i * sizeof(fy_field*));
		}
		break;
	case prm:
		break;
	}
	clazz->phase = 2;
	fy_strRelease(block, FINALIZE);
}
fy_class *fy_clLoadclass(fy_context *context, fy_str *name,
		fy_exception *exception) {

	void *is;
	fy_class *clazz;
	fy_memblock *block = context->memblocks;

#if 0
	fy_strPrint(name);
	printf("\n");
#endif

	if (name->content[0] == FY_TYPE_ARRAY) {
		clazz = fy_mmAllocate(block, sizeof(fy_class), exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		clazz->type = arr;
		clazz->super = fy_vmLookupClass(context, context->sTopClass, exception);
		clazz->className = fy_strCreateClone(block, name, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		clazz->ci.arr.arrayType = getSizeShiftForArray(name, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
	} else if (fy_hashMapGet(block, context->mapPrimitivesRev, name) != NULL) {
		clazz = fy_mmAllocate(block, sizeof(fy_class), exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		clazz->className = fy_strCreateClone(block, name, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		clazz->type = prm;
		clazz->super = fy_vmLookupClass(context, context->sTopClass, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		clazz->ci.prm.pType = *(fy_char*) fy_hashMapGet(block,
				context->mapPrimitivesRev, name);
	} else {
		is = fy_clOpenResource(context, name, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		if (is == NULL) {
			exception->exceptionType = exception_normal;
			sprintf_s(exception->exceptionName,
					sizeof(exception->exceptionName),
					FY_EXCEPTION_CLASSNOTFOUND);
			fy_strSPrint(exception->exceptionDesc,
					sizeof(exception->exceptionDesc), name);
			return NULL;
		}

		clazz = fy_clLoadclassPriv(context, is, exception);
		context->isClose(context, is);
		fy_exceptionCheckAndReturn(exception)NULL;

	}
	return clazz;
}

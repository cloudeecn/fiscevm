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

static int getSizeShiftForArray(fy_str *arrayName) {
	switch (arrayName->content[1]) {
	case FY_TYPE_BOOLEAN:
	case FY_TYPE_BYTE:
		return FY_SIZE_SHIFT_BYTE;
	case FY_TYPE_DOUBLE:
	case FY_TYPE_LONG:
		return FY_SIZE_SHIFT_LONG;
	default:
		return FY_SIZE_SHIFT_INT;
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

static void fillConstantContent(fy_memblock *block, fy_class *ret,
		fy_data *data, fy_exception *exception) {
	int i, imax, j;
	int cpSkip;
	int tag;
	void* tmp;
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

	ret->constantPoolCount = fy_dataRead2(data);
	/*read constantPool*/
	ret->constantTypes = constantTypes = fy_mmAllocate(block,
			sizeof(fy_ubyte) * (ret->constantPoolCount + 1), exception);
	fy_exceptionCheckAndReturn(exception);
	ret->constantPools = constantPools = fy_mmAllocate(block,
			sizeof(void*) * (ret->constantPoolCount + 1), exception);
	fy_exceptionCheckAndReturn(exception);
	fy_exceptionCheckAndReturn(exception);
	/*Phase 1*/
	for (i = 1, imax = ret->constantPoolCount; i < imax; i++) {
		tag = fy_dataRead(data);
		constantTypes[i] = tag;
		cpSkip = 0;
		switch (tag) {
		case CONSTANT_Class:
			tmpConstantClass = fy_mmAllocate(block, sizeof(ConstantClass),
					exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantClass->ci.name_index = checkConstantBonud(ret,
					fy_dataRead2(data), exception);
			fy_exceptionCheckAndReturn(exception);

			tmp = tmpConstantClass;
			break;
		case CONSTANT_Fieldref:
			tmpConstantFieldRef = fy_mmAllocate(block, sizeof(ConstantFieldRef),
					exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantFieldRef->class_index = checkConstantBonud(ret,
					fy_dataRead2(data), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantFieldRef->name_type_index = checkConstantBonud(ret,
					fy_dataRead2(data), exception);
			fy_exceptionCheckAndReturn(exception);
			tmp = tmpConstantFieldRef;
			break;
		case CONSTANT_Methodref:
		case CONSTANT_InterfaceMethodref:
			tmpConstantMethodRef = fy_mmAllocate(block,
					sizeof(ConstantMethodRef), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantMethodRef->class_index = checkConstantBonud(ret,
					fy_dataRead2(data), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantMethodRef->name_type_index = checkConstantBonud(ret,
					fy_dataRead2(data), exception);
			fy_exceptionCheckAndReturn(exception);
			tmp = tmpConstantMethodRef;
			break;
		case CONSTANT_String:
			tmpConstantStringInfo = fy_mmAllocate(block,
					sizeof(ConstantStringInfo), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantStringInfo->ci.string_index = checkConstantBonud(ret,
					fy_dataRead2(data), exception);
			fy_exceptionCheckAndReturn(exception);
			tmp = tmpConstantStringInfo;
			break;
		case CONSTANT_Integer:
		case CONSTANT_Float:
			tmpConstantIntegerFloatInfo = fy_mmAllocate(block,
					sizeof(ConstantIntegerFloatInfo), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantIntegerFloatInfo->value = fy_dataRead4(data);
			tmp = tmpConstantIntegerFloatInfo;
			break;
		case CONSTANT_Double:
		case CONSTANT_Long:
			cpSkip = 1;
			tmpConstantLongDoubleInfo = fy_mmAllocate(block,
					sizeof(ConstantLongDoubleInfo), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantLongDoubleInfo->value = fy_dataRead8(data);
			tmp = tmpConstantLongDoubleInfo;
			break;
		case CONSTANT_NameAndType:
			tmpConstantNameAndTypeInfo = fy_mmAllocate(block,
					sizeof(ConstantNameAndTypeInfo), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantNameAndTypeInfo->name_index = checkConstantBonud(ret,
					fy_dataRead2(data), exception);
			fy_exceptionCheckAndReturn(exception);
			tmpConstantNameAndTypeInfo->descriptor_index = checkConstantBonud(
					ret, fy_dataRead2(data), exception);
			fy_exceptionCheckAndReturn(exception);
			tmp = tmpConstantNameAndTypeInfo;
			break;
		case CONSTANT_Utf8:
			tmpConstantUtf8Info = fy_mmAllocate(block, sizeof(ConstantUtf8Info),
					exception);
			fy_exceptionCheckAndReturn(exception);
			j = fy_dataRead2(data);
			tmpConstantUtf8Info->string = fy_mmAllocate(block, sizeof(fy_str),
					exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strInit(block, tmpConstantUtf8Info->string,
					fy_utf8SizeS(data->data, j), exception);
			fy_exceptionCheckAndReturn(exception);
			fy_strAppendUTF8(block, tmpConstantUtf8Info->string, data->data, j,
					exception);
			fy_exceptionCheckAndReturn(exception);
			fy_dataSkip(data, j);
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

static void loadInterfaces(fy_context *context, fy_class *clazz, fy_data *data,
		fy_exception *exception) {
	int i, count;
	fy_memblock *block = context->memblocks;
	clazz->interfacesCount = count = fy_dataRead2(data);
	clazz->interfaces = fy_mmAllocate(block, sizeof(fy_class*) * count,
			exception);
	fy_exceptionCheckAndReturn(exception);
	for (i = 0; i < count; i++) {
		clazz->interfaces[i] = fy_vmLookupClassFromConstant(context,
				(ConstantClass*) clazz->constantPools[fy_dataRead2(data)],
				exception);
		fy_exceptionCheckAndReturn(exception);
	}
}

static void loadFields(fy_context *context, fy_class *clazz, fy_data *data,
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

	clazz->fieldCount = count = fy_dataRead2(data);
	clazz->fields = fy_mmAllocate(block, sizeof(fy_field*) * count, exception);
	fy_exceptionCheckAndReturn(exception);
	for (i = 0; i < count; i++) {
		field = fy_mmAllocate(block, sizeof(fy_field), exception);
		fy_exceptionCheckAndReturn(exception);
		field->owner = clazz;
		field->access_flags = fy_dataRead2(data);
		field->name = ((ConstantUtf8Info*) clazz->constantPools[fy_dataRead2(
				data)])->string;
		field->descriptor =
				((ConstantUtf8Info*) clazz->constantPools[fy_dataRead2(data)])->string;
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
		countj = fy_dataRead2(data);
		for (j = 0; j < countj; j++) {
			attrName = fy_clGetConstantString(context, clazz,
					fy_dataRead2(data));
			attrSize = fy_dataRead4(data);
			if (fy_strCmp(strConstantValue, attrName) == 0) {
				/*TODO*/
				fy_dataRead2(data);
			} else {
				fy_dataSkip(data, attrSize);
			}
		}
		fy_vmRegisterField(context, field, exception);
		fy_exceptionCheckAndReturn(exception);
		clazz->fields[i] = field;
	}
	clazz->sizeRel = pos;
	clazz->staticSize = staticPos;
	if (staticPos > 0) {
		clazz->staticArea = fy_mmAllocate(block, staticPos << FY_SIZE_SHIFT_INT,
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

static void loadMethods(fy_context *context, fy_class *clazz, fy_data *data,
		fy_exception *exception) {
	fy_memblock *block = context->memblocks;
	fy_char i, count, j, jcount, k, kcount, l, lcount;
	fy_str *ATT_CODE = fy_strCreateFromUTF8(block, "Code", exception);
	fy_exceptionCheckAndReturn(exception);
	fy_str *ATT_LINENUM = fy_strCreateFromUTF8(block, "LineNumberTable",
			exception);
	fy_exceptionCheckAndReturn(exception);
	fy_str *ATT_SYNTH = fy_strCreateFromUTF8(block, "Synthetic", exception);
	fy_exceptionCheckAndReturn(exception);
	fy_str *attrName;
	fy_uint attrSize;
	fy_str *attrNameCode;
	fy_uint attrSizeCode;
	fy_method *method;
	clazz->methodCount = count = fy_dataRead2(data);
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
		method->access_flags = fy_dataRead2(data);
		method->name = fy_clGetConstantString(context, clazz,
				fy_dataRead2(data));
		if (fy_strCmp(method->name, context->sClinit) == 0) {
			method->clinit = TRUE;
			clazz->clinit = method;
		}
		method->descriptor = fy_clGetConstantString(context, clazz,
				fy_dataRead2(data));
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
		jcount = fy_dataRead2(data);
		for (j = 0; j < jcount; j++) {
			attrName = fy_clGetConstantString(context, clazz,
					fy_dataRead2(data));
			attrSize = fy_dataRead4(data);
			if (fy_strCmp(ATT_CODE, attrName) == 0) {
				method->max_stack = fy_dataRead2(data);
				method->max_locals = fy_dataRead2(data);
				method->codeLength = fy_dataRead4(data);
				method->code = fy_mmAllocate(block, method->codeLength,
						exception);
				fy_exceptionCheckAndReturn(exception);
				memcpy(method->code, data->data, method->codeLength);
				fy_dataSkip(data, method->codeLength);
				kcount = fy_dataRead2(data);
				method->exception_table_length = kcount;
				method->exception_table = fy_mmAllocate(block,
						sizeof(fy_exceptionHandler) * kcount, exception);
				fy_exceptionCheckAndReturn(exception);
				for (k = 0; k < kcount; k++) {
					method->exception_table[k].start_pc = fy_dataRead2(data);
					method->exception_table[k].end_pc = fy_dataRead2(data);
					method->exception_table[k].handler_pc = fy_dataRead2(data);
					method->exception_table[k].ci.constantClass =
							(ConstantClass*) clazz->constantPools[fy_dataRead2(
									data)];
				}
				kcount = fy_dataRead2(data);
				for (k = 0; k < kcount; k++) {
					attrNameCode = fy_clGetConstantString(context, clazz,
							fy_dataRead2(data));
					attrSizeCode = fy_dataRead4(data);
					if (fy_strCmp(ATT_LINENUM, attrNameCode) == 0) {
						lcount = fy_dataRead2(data);
						method->line_number_table = fy_mmAllocate(block,
								sizeof(fy_lineNumber) * lcount, exception);
						fy_exceptionCheckAndReturn(exception);
						method->line_number_table_length = lcount;
						for (l = 0; l < lcount; l++) {
							method->line_number_table[l].start_pc =
									fy_dataRead2(data);
							method->line_number_table[l].line_number =
									fy_dataRead2(data);
						}
					} else {
						fy_dataSkip(data, attrSizeCode);
					}
				}
			} else if (fy_strCmp(ATT_SYNTH, attrName) == 0) {
				method->synthetic = 1;
			} else {
				fy_dataSkip(data, attrSize);
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

fy_data *fy_clOpenResource(fy_context *context, fy_str *name,
		fy_exception *exception) {
	int i, max;
	int size = 0;
	char *cname;
	fy_data *ret;

	for (i = 0, max = name->length; i < max; i++) {
		size += fy_utf8Size(name->content[i]);
	}
	cname = fy_allocate(size + 1, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fy_strSPrint(cname, size + 1, name);
	ret = fy_resourceAllocateData(context, cname, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fy_free(cname);
	return ret;
}

void fy_clCloseResource(fy_context *context, fy_data *data) {
	fy_resourceReleaseData(context, data);
}

static fy_class *fy_clLoadclassPriv(fy_context *context, fy_data *data,
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
	fy_dataSkip(data, 8);
	fillConstantContent(block, clazz, data, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	clazz->constantPools[0] = NULL;
	clazz->accessFlags = fy_dataRead2(data);
	clazz->thisClass = (clazz->constantPools)[fy_dataRead2(data)];
	clazz->className = clazz->thisClass->ci.className;
	clazz->superClass = clazz->constantPools[fy_dataRead2(data)];
	loadInterfaces(context, clazz, data, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	loadFields(context, clazz, data, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	loadMethods(context, clazz, data, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	icount = fy_dataRead2(data);
	for (i = 0; i < icount; i++) {
		attrName = fy_clGetConstantString(context, clazz, fy_dataRead2(data));
		attrSize = fy_dataRead4(data);
		if (fy_strCmp(ATTR_SOURCE_FILE, attrName) == 0) {
			clazz->sourceFile = fy_clGetConstantString(context, clazz,
					fy_dataRead2(data));
		} else {
			fy_dataSkip(data, attrSize);
		}
	}

#ifdef _DEBUG
	if (data->size > 0) {
		fy_strPrint(clazz->className);
		fy_fault(exception, NULL, "Still %d bytes unread!", data->size);
	}
#endif
	clazz->phase = 1;
	return clazz;
}
void fy_clPhase2(fy_context *context, fy_class *clazz, fy_exception *exception) {
	fy_str *name;
	fy_char i;
	fy_memblock *block = context->memblocks;
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
		break;
	case prm:
		break;
	}
	clazz->phase = 2;
	fy_strRelease(block, FINALIZE);
}
fy_class *fy_clLoadclass(fy_context *context, fy_str *name,
		fy_exception *exception) {

	fy_data *data;
	fy_data tmpData;
	fy_class *clazz;
	fy_memblock *block = context->memblocks;

	if (name->content[0] == FY_TYPE_ARRAY) {
		clazz = fy_mmAllocate(block, sizeof(fy_class), exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		clazz->type = arr;
		clazz->super = fy_vmLookupClass(context, context->sTopClass, exception);
		clazz->className = fy_strCreateClone(block, name, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		clazz->ci.arr.sizeShift = getSizeShiftForArray(name);
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
		fy_str *localName = fy_mmAllocate(block, sizeof(fy_str), exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		fy_strInit(block, localName, name->length + 20, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		fy_strAppendUTF8(block, localName, "../rt/bin/", -1, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		fy_strAppend(block, localName, name, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		fy_strAppendUTF8(block, localName, ".class", 6, exception);
		fy_exceptionCheckAndReturn(exception)NULL;

		data = fy_clOpenResource(context, localName, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		if (data == NULL) {
			exception->exceptionType = exception_normal;
			sprintf_s(exception->exceptionName,
					sizeof(exception->exceptionName),
					"java/lang/ClassNotFoundException");
			fy_strSPrint(exception->exceptionDesc,
					sizeof(exception->exceptionDesc), name);
			return NULL;
		}

		tmpData = *data;

		clazz = fy_clLoadclassPriv(context, &tmpData, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		fy_clCloseResource(context, data);
		fy_strDestroy(block, localName);
		fy_mmFree(block, localName);
	}
	return clazz;
}

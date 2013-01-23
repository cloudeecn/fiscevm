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

#if 0 || FY_VERBOSE
# ifndef FY_CL_DEBUG
#  define FY_CL_DEBUG
# endif
#endif

#ifdef FY_DEBUG
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

static void fillConstantContent(fy_context *context, fy_class *ret,
		fy_inputStream *is, fy_exception *exception) {
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
	ret->constantTypes = constantTypes = fy_mmAllocatePerm(block,
			sizeof(fy_ubyte) * (ret->constantPoolCount + 1), exception);
	FYEH();
	ret->constantPools = constantPools = fy_mmAllocatePerm(block,
			sizeof(void*) * (ret->constantPoolCount + 1), exception);
	FYEH();
	/*Phase 1*/
	for (i = 1, imax = ret->constantPoolCount; i < imax; i++) {
		tag = fy_dataRead(context, is, exception);
		FYEH();
		constantTypes[i] = tag;
		cpSkip = 0;
		switch (tag) {
		case CONSTANT_Class:
			tmpConstantClass = fy_mmAllocatePerm(block, sizeof(ConstantClass),
					exception);
			FYEH();
			tmpConstantClass->ci.name_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			FYEH();

			tmp = tmpConstantClass;
			break;
		case CONSTANT_Fieldref:
			tmpConstantFieldRef = fy_mmAllocatePerm(block,
					sizeof(ConstantFieldRef), exception);
			FYEH();
			tmpConstantFieldRef->class_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			FYEH();
			tmpConstantFieldRef->name_type_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			FYEH();
			tmp = tmpConstantFieldRef;
			break;
		case CONSTANT_Methodref:
		case CONSTANT_InterfaceMethodref:
			tmpConstantMethodRef = fy_mmAllocatePerm(block,
					sizeof(ConstantMethodRef), exception);
			FYEH();
			tmpConstantMethodRef->class_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			FYEH();
			tmpConstantMethodRef->name_type_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			FYEH();
			tmp = tmpConstantMethodRef;
			break;
		case CONSTANT_String:
			tmpConstantStringInfo = fy_mmAllocatePerm(block,
					sizeof(ConstantStringInfo), exception);
			FYEH();
			tmpConstantStringInfo->ci.string_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			FYEH();
			tmp = tmpConstantStringInfo;
			break;
		case CONSTANT_Integer:
		case CONSTANT_Float:
			tmpConstantIntegerFloatInfo = fy_mmAllocatePerm(block,
					sizeof(ConstantIntegerFloatInfo), exception);
			FYEH();
			tmpConstantIntegerFloatInfo->value = fy_dataRead4(context, is,
					exception);
			FYEH();
			tmp = tmpConstantIntegerFloatInfo;
			break;
		case CONSTANT_Double:
		case CONSTANT_Long:
			cpSkip = 1;
			tmpConstantLongDoubleInfo = fy_mmAllocatePerm(block,
					sizeof(ConstantLongDoubleInfo), exception);
			FYEH();
			tmpConstantLongDoubleInfo->value = fy_dataRead8(context, is,
					exception);
			FYEH();
			tmp = tmpConstantLongDoubleInfo;
			break;
		case CONSTANT_NameAndType:
			tmpConstantNameAndTypeInfo = fy_mmAllocatePerm(block,
					sizeof(ConstantNameAndTypeInfo), exception);
			FYEH();
			tmpConstantNameAndTypeInfo->name_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			FYEH();
			tmpConstantNameAndTypeInfo->descriptor_index = checkConstantBonud(
					ret, fy_dataRead2(context, is, exception), exception);
			FYEH();
			tmp = tmpConstantNameAndTypeInfo;
			break;
		case CONSTANT_Utf8:
			tmpConstantUtf8Info = fy_mmAllocatePerm(block,
					sizeof(ConstantUtf8Info), exception);
			FYEH();
			j = fy_dataRead2(context, is, exception);
			FYEH();
			dataBuffer = /*TEMP*/fy_allocate(j + 1, exception);
			FYEH();
			fy_dataReadBlock(context, is, dataBuffer, j, exception);
			if (exception->exceptionType != exception_none) {
				fy_free(dataBuffer);
				return;
			}
			tmpConstantUtf8Info->string = fy_strCreatePermFromUTF8(block,
					dataBuffer, 0, exception);
			fy_free(dataBuffer);
			FYEH();
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
			tmpConstantFieldRef->nameType =
					fy_strCreatePerm(block,
							2
									+ tmpConstantFieldRef->constantNameType->name->length
									+ tmpConstantFieldRef->constantNameType->descriptor->length,
							exception);
			FYEH();
			fy_strAppendUTF8(block, tmpConstantFieldRef->nameType, ".", 1,
					exception);
			FYEH();
			fy_strAppend(block, tmpConstantFieldRef->nameType,
					tmpConstantFieldRef->constantNameType->name, exception);
			FYEH();
			fy_strAppendUTF8(block, tmpConstantFieldRef->nameType, ".", 1,
					exception);
			FYEH();
			fy_strAppend(block, tmpConstantFieldRef->nameType,
					tmpConstantFieldRef->constantNameType->descriptor,
					exception);
			FYEH();
			break;
		case CONSTANT_Methodref:
		case CONSTANT_InterfaceMethodref:
			tmpConstantMethodRef = (ConstantMethodRef*) tmp;
			tmpConstantMethodRef->constantClass =
					(ConstantClass*) (constantPools[tmpConstantMethodRef->class_index]);
			tmpConstantMethodRef->constantNameType =
					(ConstantNameAndTypeInfo*) (constantPools[tmpConstantMethodRef->name_type_index]);
			tmpConstantMethodRef->nameType =
					fy_strCreatePerm(block,
							2
									+ tmpConstantMethodRef->constantNameType->name->length
									+ tmpConstantMethodRef->constantNameType->descriptor->length,
							exception);
			FYEH();
			fy_strAppendUTF8(block, tmpConstantMethodRef->nameType, ".", 1,
					exception);
			FYEH();
			fy_strAppend(block, tmpConstantMethodRef->nameType,
					tmpConstantMethodRef->constantNameType->name, exception);
			FYEH();
			fy_strAppendUTF8(block, tmpConstantMethodRef->nameType, ".", 1,
					exception);
			FYEH();
			fy_strAppend(block, tmpConstantMethodRef->nameType,
					tmpConstantMethodRef->constantNameType->descriptor,
					exception);
			FYEH();
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

static void loadInterfaces(fy_context *context, fy_class *clazz,
		fy_inputStream *is, fy_exception *exception) {
	int i, count;
	fy_memblock *block = context->memblocks;
	clazz->interfacesCount = count = fy_dataRead2(context, is, exception);
	FYEH();
	clazz->interfaces = fy_mmAllocatePerm(block, sizeof(fy_class*) * count,
			exception);
	FYEH();
	clazz->interfaceClasses = fy_mmAllocatePerm(block,
			sizeof(ConstantClass*) * count, exception);
	FYEH();
	for (i = 0; i < count; i++) {
		clazz->interfaceClasses[i] = clazz->constantPools[fy_dataRead2(context,
				is, exception)];
	}
}

static void loadFields(fy_context *context, fy_class *clazz, fy_inputStream *is,
		fy_exception *exception) {
	int i, count, j, countj;
	int length;

	fy_str *attrName;
	fy_uint attrSize;
	fy_field *field;
	fy_uint pos = 0;
	fy_uint staticPos = 0;
	fy_memblock *block = context->memblocks;

	clazz->fieldCount = count = fy_dataRead2(context, is, exception);
	FYEH();
	clazz->fields = fy_mmAllocatePerm(block, sizeof(fy_field*) * count,
			exception);
	FYEH();
	for (i = 0; i < count; i++) {
		field = fy_mmAllocatePerm(block, sizeof(fy_field), exception);
		FYEH();
		field->owner = clazz;
		field->access_flags = fy_dataRead2(context, is, exception);
		FYEH();
		field->name = ((ConstantUtf8Info*) clazz->constantPools[fy_dataRead2(
				context, is, exception)])->string;
		FYEH();
		field->descriptor =
				((ConstantUtf8Info*) clazz->constantPools[fy_dataRead2(context,
						is, exception)])->string;
		FYEH();
		field->fullName = fy_strCreatePerm(block,
				2 + field->name->length + field->descriptor->length, exception);
		FYEH();
		field->uniqueName = fy_strCreatePerm(block,
				clazz->className->length + 2 + field->name->length
						+ field->descriptor->length, exception);
		FYEH();
		fy_strAppendUTF8(block, field->fullName, ".", 1, exception);
		FYEH();
		fy_strAppend(block, field->fullName, field->name, exception);
		FYEH();
		fy_strAppendUTF8(block, field->fullName, ".", 1, exception);
		FYEH();
		fy_strAppend(block, field->fullName, field->descriptor, exception);
		FYEH();
		fy_strAppend(block, field->uniqueName, clazz->className, exception);
		FYEH();
		fy_strAppend(block, field->uniqueName, field->fullName, exception);
		FYEH();

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
		FYEH();
		for (j = 0; j < countj; j++) {
			attrName = fy_clGetConstantString(context, clazz,
					fy_dataRead2(context, is, exception));
			FYEH();
			attrSize = fy_dataRead4(context, is, exception);
			FYEH();
			if (fy_strCmp(context->sAttConstantValue, attrName) == 0) {
				field->constant_value_index = fy_dataRead2(context, is,
						exception);
				FYEH();
			} else {
				fy_dataSkip(context, is, attrSize, exception);
				FYEH();
			}
		}
		fy_vmRegisterField(context, field, exception);
		FYEH();
		clazz->fields[i] = field;
	}
	clazz->sizeRel = pos;
	clazz->staticSize = staticPos;
	if (staticPos > 0) {
		clazz->staticArea = fy_mmAllocatePerm(block, staticPos * sizeof(fy_int),
				exception);
		FYEH();
	} else {

	}
}

static fy_class* getClassFromName(struct fy_context *context, fy_str *desc,
		fy_int begin, fy_int end, fy_exception *exception) {
	fy_char ch;
	fy_class *clazz;
	fy_str *tmp;
	fy_str className[1];
	if (begin <= 0) {
		fy_fault(exception, NULL, "Bad descriptor");
		FYEH()0;
	}
	if (end == 0) {
		//Pri

		tmp = context->primitives[desc->content[begin]];
		className->length = tmp->length;
		className->maxLength = tmp->maxLength;
		className->hashCode = tmp->hashCode;
		className->hashed = tmp->hashed;
		className->content = tmp->content;
	} else {
		//Class or Array
		ch = desc->content[begin];
		if (ch == 'L') {
			className->maxLength = className->length = end - begin - 2;
			className->hashed = 0;
			className->content = desc->content + begin + 1;
		} else if (ch == '[') {
			className->maxLength = className->length = end - begin;
			className->hashed = 0;
			className->content = desc->content + begin;
		}
	}
#if 0
	context->logDStr(context,className);
	context->logDVar(context,"\n");
#endif
	clazz = fy_vmLookupClass(context, className, exception);
	FYEH()0;
	if (clazz == NULL) {
		fy_fault(exception, NULL, "Bad descriptor");
		FYEH()0;
	}
	return clazz;

}

static void countParams(fy_context *context, fy_str *desc, fy_method *method,
		fy_exception *exception) {
	fy_byte *temp;
	fy_byte returnType = FY_TYPE_UNKNOWN;
	fy_int beginClass, endClass;
	int pc = 0;
	fy_char ch;
	int i, maxi;
	fy_class *clazz;
	fy_boolean begin = FALSE;
	fy_arrayList tmpList[1];
	char msg[256];
	temp = /*TEMP*/fy_allocate(desc->length * sizeof(temp), exception);
	FYEH();
	fy_arrayListInit(context->memblocks, tmpList, sizeof(fy_class*), 16,
			exception);

	FYEH();
	maxi = desc->length;
	for (i = 0; i < maxi; i++) {
		ch = desc->content[i];
		if (!begin) {
			if (ch == '(') {
				begin = TRUE;
			}
		} else {
			if (ch == ')') {
				switch (desc->content[i + 1]) {
				case 'B':
				case 'C':
				case 'F':
				case 'I':
				case 'S':
				case 'Z':
					beginClass = i + 1;
					endClass = 0;
					returnType = FY_TYPE_INT;
					break;
				case 'D':
				case 'J':
					beginClass = i + 1;
					endClass = 0;
					returnType = FY_TYPE_WIDE;
					break;
				case '[':
				case 'L':
					beginClass = i + 1;
					endClass = desc->length;
					returnType = FY_TYPE_HANDLE;
					break;
				case 'V':
					returnType = FY_TYPE_UNKNOWN;
					beginClass = i + 1;
					endClass = 0;
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
					FYEH();
					break;
				}
				clazz = getClassFromName(context, desc, beginClass, endClass,
						exception);
				FYEH();
				if (method != NULL) {
					method->returnTypeClass = clazz;
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
					beginClass = i;
					endClass = 0;
					temp[pc++] = FY_TYPE_INT;
					break;
				case 'D':
				case 'J':
					beginClass = i;
					endClass = 0;
					temp[pc++] = FY_TYPE_WIDE;
					temp[pc++] = FY_TYPE_WIDE2;
					break;
				case '[':
					beginClass = i;
					while ((ch = desc->content[++i]) == '[') {
					}
					if (ch == 'L') {
						while ((ch = desc->content[++i]) != ';') {
						}
					}
					endClass = i + 1;
					temp[pc++] = FY_TYPE_HANDLE;
					break;
				case 'L':
					beginClass = i;
					while ((ch = desc->content[++i]) != ';')
						;
					endClass = i + 1;
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
					return;
				}
				clazz = getClassFromName(context, desc, beginClass, endClass,
						exception);
				FYEH();
				fy_arrayListAdd(context->memblocks, tmpList, &clazz, exception);
				FYEH();
			}
		}
	}
	if (method != NULL) {
		method->parameterTypes = fy_arrayListCreatePerm(context->memblocks,
				sizeof(fy_class*), tmpList->length, exception);
		method->parameterCount = maxi = tmpList->length;
		for (i = 0; i < maxi; i++) {
			fy_arrayListAdd(context->memblocks, method->parameterTypes,
					fy_arrayListGet(context->memblocks, tmpList, i, NULL),
					exception);
			FYEH();
		}
	}
	fy_arrayListDestroy(context->memblocks, tmpList);
	if (method != NULL) {
		method->paramTypes = fy_mmAllocatePerm(context->memblocks,
				pc * sizeof(fy_byte), exception);
		FYEH();
		method->paramCount = pc;
		memcpy(method->paramTypes, temp, pc * sizeof(fy_byte));
		method->returnType = returnType;
	}
	fy_free(temp);
}

static void loadMethods(fy_context *context, fy_class *clazz,
		fy_inputStream *is, fy_exception *exception) {
	fy_memblock *block = context->memblocks;
	fy_char i, count, j, jcount, k, kcount, l, lcount;
	fy_str *attrName;
	fy_uint attrSize;
	fy_str *attrNameCode;
	fy_uint attrSizeCode;
	fy_method *method;

	clazz->methodCount = count = fy_dataRead2(context, is, exception);
	FYEH();
	clazz->methods = fy_mmAllocatePerm(block, sizeof(fy_method*) * count,
			exception);
	FYEH();
#if 0
	context->logDVar(context,"Loading %d methods...\n", count);
#endif
	for (i = 0; i < count; i++) {
		method = fy_mmAllocatePerm(block, sizeof(fy_method), exception);
		FYEH();
		method->owner = clazz;
		method->access_flags = fy_dataRead2(context, is, exception);
		FYEH();
		if (clazz->accessFlags & FY_ACC_FINAL) {
			method->access_flags |= FY_ACC_FINAL;
		}
		method->name = fy_clGetConstantString(context, clazz,
				fy_dataRead2(context, is, exception));
		FYEH();
		if (fy_strCmp(method->name, context->sClinit) == 0) {
			method->access_flags |= FY_ACC_CLINIT;
			clazz->clinit = method;
		}
		if (fy_strCmp(method->name, context->sInit) == 0) {
			method->access_flags |= FY_ACC_CONSTRUCTOR;
		}
		method->descriptor = fy_clGetConstantString(context, clazz,
				fy_dataRead2(context, is, exception));
		FYEH();
		method->fullName = fy_strCreatePerm(block,
				method->name->length + method->descriptor->length + 2,
				exception);
		FYEH();
		method->uniqueName = fy_strCreatePerm(block,
				method->name->length + method->descriptor->length
						+ clazz->className->length + 2, exception);
		FYEH();
		fy_strAppendUTF8(block, method->fullName, ".", 1, exception);
		FYEH();
		fy_strAppend(block, method->fullName, method->name, exception);
		FYEH();
		fy_strAppendUTF8(block, method->fullName, ".", 1, exception);
		FYEH();
		fy_strAppend(block, method->fullName, method->descriptor, exception);
		FYEH();
		fy_strAppend(block, method->uniqueName, clazz->className, exception);
		FYEH();
		fy_strAppend(block, method->uniqueName, method->fullName, exception);
		FYEH();
		jcount = fy_dataRead2(context, is, exception);
		FYEH();
		for (j = 0; j < jcount; j++) {
			attrName = fy_clGetConstantString(context, clazz,
					fy_dataRead2(context, is, exception));
			FYEH();
			attrSize = fy_dataRead4(context, is, exception);
			FYEH();
			if (fy_strCmp(context->sAttCode, attrName) == 0) {
				method->max_stack = fy_dataRead2(context, is, exception);
				FYEH();
				method->max_locals = fy_dataRead2(context, is, exception);
				FYEH();
				method->codeLength = fy_dataRead4(context, is, exception);
				FYEH();
				/*临时分配到堆里，Preverify之后销毁*/
				method->code = fy_mmAllocate(block, method->codeLength,
						exception);
				FYEH();
				fy_dataReadBlock(context, is, method->code, method->codeLength,
						exception);
				FYEH();

				kcount = fy_dataRead2(context, is, exception);
				FYEH();
				method->exception_table_length = kcount;
				method->exception_table = fy_mmAllocatePerm(block,
						sizeof(fy_exceptionHandler) * kcount, exception);
				FYEH();
				for (k = 0; k < kcount; k++) {
					method->exception_table[k].start_pc = fy_dataRead2(context,
							is, exception);
					FYEH();
					method->exception_table[k].end_pc = fy_dataRead2(context,
							is, exception);
					FYEH();
					method->exception_table[k].handler_pc = fy_dataRead2(
							context, is, exception);
					FYEH();
					method->exception_table[k].ci.constantClass =
							(ConstantClass*) clazz->constantPools[fy_dataRead2(
									context, is, exception)];
					FYEH();
				}
				kcount = fy_dataRead2(context, is, exception);
				FYEH();
				for (k = 0; k < kcount; k++) {
					attrNameCode = fy_clGetConstantString(context, clazz,
							fy_dataRead2(context, is, exception));
					FYEH();
					attrSizeCode = fy_dataRead4(context, is, exception);
					FYEH();
					if (fy_strCmp(context->sAttLineNum, attrNameCode) == 0) {
						lcount = fy_dataRead2(context, is, exception);
						FYEH();
						method->line_number_table = fy_mmAllocatePerm(block,
								sizeof(fy_lineNumber) * lcount, exception);
						FYEH();
						method->line_number_table_length = lcount;
						for (l = 0; l < lcount; l++) {
							method->line_number_table[l].start_pc =
									fy_dataRead2(context, is, exception);
							FYEH();
							method->line_number_table[l].line_number =
									fy_dataRead2(context, is, exception);
							FYEH();
						}
					} else {
						fy_dataSkip(context, is, attrSizeCode, exception);
						FYEH();
					}
				}
			} else if (fy_strCmp(context->sAttSynth, attrName) == 0) {
				method->access_flags |= FY_ACC_SYNTHETIC;
			} else {
				fy_dataSkip(context, is, attrSize, exception);
				FYEH();
			}
		}

		fy_vmRegisterMethod(context, method, exception);
		FYEH();
		clazz->methods[i] = method;
	}
}

/************public***************/
fy_str *fy_clGetConstantString(fy_context *context, fy_class *clazz,
		fy_char idx) {
	return ((ConstantUtf8Info*) clazz->constantPools[idx])->string;
}

fy_inputStream *fy_clOpenResource(fy_context *context, fy_str *name,
		fy_exception *exception) {
	int i, max;
	int size = 0;
	char *cname;
	void *ret;

	for (i = 0, max = name->length; i < max; i++) {
		size += fy_utf8Size(name->content[i]);
	}
	cname = /*TEMP*/fy_allocate(size + 1, exception);
	FYEH()NULL;
	fy_strSPrint(cname, size + 1, name);
	ret = context->isOpen(context, cname, exception);
	fy_free(cname);
	FYEH()NULL;
	return ret;
}

static fy_class *fy_clLoadclassPriv(fy_context *context, fy_inputStream *is,
		fy_exception *exception) {
	fy_char i, icount;
	fy_str *attrName;
	fy_uint attrSize;
	fy_memblock *block = context->memblocks;
	fy_class *clazz = fy_mmAllocatePerm(block, sizeof(fy_class), exception);
	FYEH()NULL;

	clazz->type = object_class;
	fy_dataSkip(context, is, 8, exception);
	FYEH()NULL;
	fillConstantContent(context, clazz, is, exception);
	FYEH()NULL;
	clazz->constantPools[0] = NULL;
	clazz->accessFlags = fy_dataRead2(context, is, exception);
	FYEH()NULL;

	clazz->thisClass = (clazz->constantPools)[fy_dataRead2(context, is,
			exception)];
	FYEH()NULL;
	clazz->className = clazz->thisClass->ci.className;
#ifdef FY_CL_DEBUG
	context->logDVar(context, "#CL Phase1: ");
	context->logDStr(context, clazz->className);
	context->logDVarLn(context, "...");
#endif
	clazz->superClass = clazz->constantPools[fy_dataRead2(context, is,
			exception)];
	FYEH()NULL;
	loadInterfaces(context, clazz, is, exception);
	FYEH()NULL;
	loadFields(context, clazz, is, exception);
	FYEH()NULL;
	loadMethods(context, clazz, is, exception);
	FYEH()NULL;
	icount = fy_dataRead2(context, is, exception);
	FYEH()NULL;
	for (i = 0; i < icount; i++) {
		attrName = fy_clGetConstantString(context, clazz,
				fy_dataRead2(context, is, exception));
		FYEH()NULL;
		attrSize = fy_dataRead4(context, is, exception);
		FYEH()NULL;
		if (fy_strCmp(context->sAttSourceFile, attrName) == 0) {
			clazz->sourceFile = fy_clGetConstantString(context, clazz,
					fy_dataRead2(context, is, exception));
			FYEH()NULL;
		} else {
			fy_dataSkip(context, is, attrSize, exception);
			FYEH()NULL;
		}
	}
	clazz->phase = 1;

	if (context->TOP_CLASS == NULL
			&& fy_strCmp(clazz->className, context->sTopClass) == 0) {
#ifdef FY_CL_DEBUG
		context->logDStr(context, clazz->className);
		context->logDVarLn(context, " is TOP_CLASS");
#endif
		context->TOP_CLASS = clazz;
	} else if (context->TOP_THROWABLE == NULL
			&& fy_strCmp(clazz->className, context->sClassThrowable) == 0) {
#ifdef FY_CL_DEBUG
		context->logDStr(context, clazz->className);
		context->logDVarLn(context, " is TOP_THROWABLE");
#endif
		context->TOP_THROWABLE = clazz;
	} else if (context->TOP_ENUM == NULL
			&& fy_strCmp(clazz->className, context->sEnum) == 0) {
#ifdef FY_CL_DEBUG
		context->logDStr(context, clazz->className);
		context->logDVarLn(context, " is TOP_ENUM");
#endif
		context->TOP_ENUM = clazz;
	} else if (context->TOP_ANNOTATION == NULL
			&& fy_strCmp(clazz->className, context->sAnnotation) == 0) {
#ifdef FY_CL_DEBUG
		context->logDStr(context, clazz->className);
		context->logDVarLn(context, " is TOP_ANNOTATION");
#endif
		context->TOP_ANNOTATION = clazz;
	} else if (context->TOP_SOFT_REF == NULL
			&& fy_strCmp(clazz->className, context->sSoftReference) == 0) {
#ifdef FY_CL_DEBUG
		context->logDStr(context, clazz->className);
		context->logDVarLn(context, " is TOP_SOFT_REF");
#endif
		context->TOP_SOFT_REF = clazz;
	} else if (context->TOP_WEAK_REF == NULL
			&& fy_strCmp(clazz->className, context->sWeakReference) == 0) {
#ifdef FY_CL_DEBUG
		context->logDStr(context, clazz->className);
		context->logDVarLn(context, " is TOP_WEAK_REF");
#endif
		context->TOP_WEAK_REF = clazz;
	} else if (context->TOP_PHANTOM_REF == NULL
			&& fy_strCmp(clazz->className, context->sPhantomReference) == 0) {
#ifdef FY_CL_DEBUG
		context->logDStr(context, clazz->className);
		context->logDVarLn(context, " is TOP_PHANTOM_REF");
#endif
		context->TOP_PHANTOM_REF = clazz;
	}
#ifdef FY_CL_DEBUG
	context->logDVar(context, "#CL Phase1: ");
	context->logDStr(context, clazz->className);
	context->logDVarLn(context, "...OK");
#endif
	return clazz;
}
void fy_clPhase2(fy_context *context, fy_class *clazz, fy_exception *exception) {
	fy_uint i, pos;
	fy_memblock *block = context->memblocks;
	fy_field *field;
	fy_method *method;
	fy_str str[1];
	fy_class *tmp;
	fy_method *finalizeMethod;
#ifdef FY_DEBUG
	char buf[255];
#endif
	/*
	 fy_class *annotationClazz, *enumClazz;
	 annotationClazz = fy_vmLookupClass(context, context->sAnnotation,
	 exception);
	 FYEH();
	 enumClazz = fy_vmLookupClass(context, context->sEnum, exception);
	 FYEH();
	 */
#ifdef FY_CL_DEBUG
	context->logDVar(context, "#CL Phase2: ");
	context->logDStr(context, clazz->className);
	context->logDVarLn(context, "...");
#endif
	switch (clazz->type) {
	case array_class:

		break;
	case object_class:
		pos = clazz->methodCount;
		for (i = 0; i < pos; i++) {
			method = clazz->methods[i];
			countParams(context, method->descriptor, method, exception);
			FYEH();
		}
		pos = clazz->interfacesCount;
		for (i = 0; i < pos; i++) {
			clazz->interfaces[i] = fy_vmLookupClassFromConstant(context,
					(ConstantClass*) clazz->interfaceClasses[i], exception);
			FYEH();
		}
		/*name = clazz->className;*/
		if (clazz->superClass != NULL) {
#ifdef FY_DEBUG
			if (fy_strCmp(context->sTopClass, clazz->className) == 0) {
				fy_fault(exception, NULL,
						"java.lang.Object cannot have super class!", buf);
			}
#endif

			/*貌似加载顺序的问题会导致父类的sizeAbs不正确，我们暂时枚举所有的父类把sizeRel加起来
			 clazz->sizeAbs = clazz->super->sizeAbs + clazz->sizeRel;
			 */
			tmp = clazz;
			clazz->sizeAbs = 0;
			while (tmp != NULL) {
				clazz->sizeAbs += tmp->sizeRel;
				tmp = tmp->super;
			}
#ifdef FY_CL_DEBUG
			context->logDVar(context, "#CL#");
			context->logDStr(context, clazz->className);
			context->logDVar(context, " sizeAbs=%d sizeRel=%d\n",
					clazz->sizeAbs, clazz->sizeRel);

			context->logDVar(context, "   +");
			context->logDStr(context, clazz->super->className);
			context->logDVar(context, " sizeAbs=%d sizeRel=%d\n",
					clazz->super->sizeAbs, clazz->super->sizeRel);
#endif
			for (i = 0; i < clazz->fieldCount; i++) {
				if (clazz->fields[i]->access_flags & FY_ACC_STATIC) {

				} else {
					clazz->fields[i]->posAbs = clazz->super->sizeAbs
							+ clazz->fields[i]->posRel;
				}
			}
			finalizeMethod = fy_vmLookupMethodVirtual(context, clazz,
					context->sMFinalize, exception);
			FYEH();
			if (finalizeMethod
					!= fy_vmLookupMethodVirtual(context,
							fy_vmLookupClass(context, context->sTopClass,
									exception), context->sMFinalize, exception)
					&& /*More than just a "RETURN"*/finalizeMethod->codeLength
							> 1) {
				clazz->needFinalize = 1;
#ifdef FY_DEBUG
				fy_strSPrint(buf, 255, clazz->className);
				DLOG
				(context, "%s needs finalize! %"FY_PRINT32"d", buf,
						finalizeMethod->codeLength);
#endif
			}
		} else {
#ifdef FY_DEBUG
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
		clazz->fieldStatic = fy_mmAllocatePerm(block,
				clazz->staticSize * sizeof(fy_field*), exception);
		FYEH();

		clazz->fieldAbs = fy_mmAllocatePerm(block,
				clazz->sizeAbs * sizeof(fy_field*), exception);
		FYEH();

		if (clazz->super && (i = clazz->super->sizeAbs) > 0) {
			memcpy(clazz->fieldAbs, clazz->super->fieldAbs,
					i * sizeof(fy_field*));
		}
		if (fy_classExtendsAnnotation(context, clazz)) {
#ifdef FY_CL_DEBUG
		context->logDStr(context, clazz->className);
		context->logDVarLn(context, " is Annotation");
#endif
			clazz->accessFlags |= FY_ACC_ANNOTATION;
		} else if (fy_classExtendsEnum(context, clazz)) {
#ifdef FY_CL_DEBUG
		context->logDStr(context, clazz->className);
		context->logDVarLn(context, " is Enum");
#endif
			clazz->accessFlags |= FY_ACC_ENUM;
		} else if (fy_classExtendsPhantomRef(context, clazz)) {
#ifdef FY_CL_DEBUG
		context->logDStr(context, clazz->className);
		context->logDVarLn(context, " is PhantomReference");
#endif
			clazz->accessFlags |= FY_ACC_PHANTOM_REF;
		} else if (fy_classExtendsWeakRef(context, clazz)) {
#ifdef FY_CL_DEBUG
		context->logDStr(context, clazz->className);
		context->logDVarLn(context, " is WeakReference");
#endif
			clazz->accessFlags |= FY_ACC_WEAK_REF;
		} else if (fy_classExtendsSoftRef(context, clazz)) {
#ifdef FY_CL_DEBUG
		context->logDStr(context, clazz->className);
		context->logDVarLn(context, " is SoftReference");
#endif
			clazz->accessFlags |= FY_ACC_SOFT_REF;
		}

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
			switch (field->descriptor->content[0]) {
			case '[':
				field->type = fy_vmLookupClass(context, field->descriptor,
						exception);
				break;
			case 'L':
				str->content = NULL;
				fy_strInit(context->memblocks, str, field->descriptor->length,
						exception);
				FYEH();
				fy_strAppend(context->memblocks, str, field->descriptor,
						exception);
				FYEH();
				fy_strSubstring(context->memblocks, str, 1, str->length - 1);
				field->type = fy_vmLookupClass(context, str, exception);
				fy_strDestroy(context->memblocks, str);
				FYEH();
				break;
			case 'Z':
			case 'B':
			case 'S':
			case 'C':
			case 'I':
			case 'F':
			case 'J':
			case 'D':
				field->type = fy_vmLookupClass(context,
						context->primitives[field->descriptor->content[0]],
						exception);
				break;
			default:
				fy_fault(exception, NULL, "Illegal descriptor of field");
				break;
			}
#if 1
			if (field->constant_value_index > 0) {
				if ((field->access_flags & FY_ACC_STATIC)
						&& (field->access_flags & FY_ACC_FINAL)) {
					switch (field->descriptor->content[0]) {
					case FY_TYPE_BOOLEAN:
					case FY_TYPE_BYTE:
					case FY_TYPE_SHORT:
					case FY_TYPE_CHAR:
					case FY_TYPE_INT:
					case FY_TYPE_FLOAT: {
						clazz->staticArea[field->posAbs] =
								((ConstantIntegerFloatInfo*) clazz->constantPools[field->constant_value_index])->value;
						break;
					}
					case FY_TYPE_LONG:
					case FY_TYPE_DOUBLE: {
						clazz->staticArea[field->posAbs] =
								fy_HOFL(
										((ConstantLongDoubleInfo*) clazz->constantPools[field->constant_value_index])->value);
						clazz->staticArea[field->posAbs + 1] =
								fy_LOFL(
										((ConstantLongDoubleInfo*) clazz->constantPools[field->constant_value_index])->value);
						break;
					}
					case FY_TYPE_HANDLE: {
						/*Will be lazy loaded in Field.get()*/
						break;
					}
					}
				} else {
					/*Ignore it since it will be proceed in default <init>*/
				}
			}
#endif
		}

		break;
	case primitive_class:
		break;
	}
	clazz->phase = 2;
#ifdef FY_CL_DEBUG
	context->logDVar(context, "#CL Phase2: ");
	context->logDStr(context, clazz->className);
	context->logDVarLn(context, "...OK");
#endif
}
fy_class *fy_clLoadclass(fy_context *context, fy_str *name,
		fy_exception *exception) {
	fy_inputStream *is;
	fy_classDefine *cd;
	fy_class *clazz;
	fy_memblock *block = context->memblocks;
	fy_str str[1];
	str->content = NULL;

#if 0
	context->logDStr(context,name);
	context->logDVar(context,"\n");
#endif

	if (name->content[0] == FY_TYPE_ARRAY) {
		clazz = fy_mmAllocatePerm(block, sizeof(fy_class), exception);
		FYEH()NULL;
		clazz->type = array_class;
		clazz->super = fy_vmLookupClass(context, context->sTopClass, exception);
		clazz->className = fy_strCreatePermFromClone(block, name, 0, exception);
		FYEH()NULL;
		clazz->ci.arr.arrayType = getSizeShiftForArray(name, exception);
		FYEH()NULL;
		switch (clazz->className->content[1]) {
		case FY_TYPE_ARRAY:
			fy_strInit(block, str, clazz->className->length, exception);
			FYEH()NULL;
			str->length = clazz->className->length - 1;
			memcpy(str->content, clazz->className->content + 1,
					(clazz->className->length - 1) << 1);
			clazz->ci.arr.contentClass = fy_vmLookupClass(context, str,
					exception);
			fy_strDestroy(block, str);
			break;
		case FY_TYPE_HANDLE:
			fy_strInit(block, str, clazz->className->length, exception);
			FYEH()NULL;
			str->length = clazz->className->length - 3;
			memcpy(str->content, clazz->className->content + 2,
					(clazz->className->length - 3) << 1);
			clazz->ci.arr.contentClass = fy_vmLookupClass(context, str,
					exception);
			fy_strDestroy(block, str);
			break;
		default:
			clazz->ci.arr.contentClass = fy_vmLookupClass(context,
					context->primitives[clazz->className->content[1]],
					exception);
			break;
		}
	} else if (fy_hashMapGet(block, context->mapPrimitivesRev, name) != NULL) {
		clazz = fy_mmAllocatePerm(block, sizeof(fy_class), exception);
		FYEH()NULL;
		clazz->className = fy_strCreatePermFromClone(block, name, 0, exception);
		FYEH()NULL;
		clazz->type = primitive_class;
		clazz->super = fy_vmLookupClass(context, context->sTopClass, exception);
		FYEH()NULL;
		clazz->ci.prm.pType = *(fy_char*) fy_hashMapGet(block,
				context->mapPrimitivesRev, name);
	} else {
		cd = fy_hashMapGet(context->memblocks, context->customClassData, name);
		if (cd == NULL) {
			is = fy_clOpenResource(context, name, exception);
			FYEH()NULL;
		} else {
			is = fy_baisOpenByteArrayInputStream(context, cd->data, cd->size,
					exception);
			FYEH()NULL;
		}
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
		is->isClose(context, is);
		fy_mmFree(context->memblocks, is);
		FYEH()NULL;
		if (clazz->superClass != NULL) {
			clazz->super = fy_vmLookupClassFromConstant(context,
					clazz->superClass, exception);
			FYEH()NULL;
		}

	}
	fy_hashMapIInitPerm(block, clazz->virtualTable, 3, -1, exception);
	FYEH()NULL;
	return clazz;
}

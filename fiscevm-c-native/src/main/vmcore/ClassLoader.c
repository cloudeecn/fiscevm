/**
 *  Copyright 2010-2015 Yuxuan Huang. All rights reserved.
 *
 * This file is part offiscevm
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
 * along withfiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fyc/ClassLoader.h"

#include "fy_util/MemMan.h"
#include "fy_util/String.h"
#include "fy_util/Utf8.h"
#include "fy_util/Debug.h"
#include "fyc/Config.h"
#include "fyc/Constants.h"
#include "fyc/ClassStruct.h"
#include "fyc/Debug.h"
#include "fyc/Class.h"
#include "fyc/Data.h"
#include "fyc/BAIS.h"
#include "fyc/VMContext.h"
#include "fyc/InputStream.h"

#if 0 || FY_VERBOSE
# ifndef FY_CL_DEBUG
#  define FY_CL_DEBUG
# endif
#endif

typedef struct fy_classDefine {
	fisce_int size;
	FY_VLS(fisce_byte,data);
}fy_classDefine;

#ifdef FY_DEBUG
static int checkConstantBonud(fy_class *clazz, int idx, fisce_exception *exception) {
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
		fisce_exception *exception) {
	switch (fy_strGet(arrayName,1)) {
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
		fy_strGet(arrayName,1), fy_strGet(arrayName,1));
		return 0;
	}
}

#if 0
static int getSizeFromDescriptor(fy_str *descriptor) {
	switch (fy_strGet(descriptor,0)) {
		case TYPE_LONG:
		case TYPE_DOUBLE:
		return 2;
		default:
		return 1;
	}
}
#endif

static void fillConstantContent(fy_context *context, fy_class *ret,
		fy_inputStream *is, fisce_exception *exception) {
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
	fisce_ubyte *constantTypes;

	ret->constantPoolCount = fy_dataRead2(context, is, exception);

	/*read constantPool*/
	ret->constantTypes = constantTypes = fy_mmAllocatePerm(block,
			sizeof(fisce_ubyte) * (ret->constantPoolCount + 1), exception);
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
			tmpConstantFieldRef->c.class_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			FYEH();
			tmpConstantFieldRef->nt.name_type_index = checkConstantBonud(ret,
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
			tmpConstantNameAndTypeInfo->n.name_index = checkConstantBonud(ret,
					fy_dataRead2(context, is, exception), exception);
			FYEH();
			tmpConstantNameAndTypeInfo->d.descriptor_index = checkConstantBonud(
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
			tmpConstantUtf8Info->string = fy_vmCreateStringByPoolV(context,
					exception, "a", dataBuffer);/*fy_strCreatePermFromUTF8(block,
			 dataBuffer, 0, exception);*/
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
			tmpConstantNameAndTypeInfo->n.name =
					((ConstantUtf8Info*) (constantPools[tmpConstantNameAndTypeInfo->n.name_index]))->string;
			tmpConstantNameAndTypeInfo->d.descriptor =
					((ConstantUtf8Info*) (constantPools[tmpConstantNameAndTypeInfo->d.descriptor_index]))->string;
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
			tmpConstantFieldRef->c.constantClass =
					(ConstantClass*) (constantPools[tmpConstantFieldRef->c.class_index]);
			tmpConstantFieldRef->nt.constantNameType =
					(ConstantNameAndTypeInfo*) (constantPools[tmpConstantFieldRef->nt.name_type_index]);
			tmpConstantFieldRef->nt.nameType = fy_vmCreateStringByPoolV(context,
					exception, "cscs", '.',
					tmpConstantFieldRef->nt.constantNameType->n.name, '.',
					tmpConstantFieldRef->nt.constantNameType->d.descriptor);
			FYEH();
#if 0
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
#endif
			break;
			case CONSTANT_Methodref:
			case CONSTANT_InterfaceMethodref:
			tmpConstantMethodRef = (ConstantMethodRef*) tmp;
			tmpConstantMethodRef->constantClass =
			(ConstantClass*) (constantPools[tmpConstantMethodRef->class_index]);
			tmpConstantMethodRef->constantNameType =
			(ConstantNameAndTypeInfo*) (constantPools[tmpConstantMethodRef->name_type_index]);
			tmpConstantMethodRef->nameType = fy_vmCreateStringByPoolV(context,
					exception, "cscs", '.',
					tmpConstantMethodRef->constantNameType->n.name, '.',
					tmpConstantMethodRef->constantNameType->d.descriptor);
			FYEH();
#if 0
			nameType =
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
#endif
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
		fy_inputStream *is, fisce_exception *exception) {
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
		fisce_exception *exception) {
	int i, count, j, countj;
	int length;

	fy_str *attrName;
	fisce_uint attrSize;
	fy_field *field;
	fisce_uint pos = 0;
	fisce_uint staticPos = 0;
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
		field->uniqueName = fy_vmCreateStringByPoolV(context, exception,
				"scscs", clazz->className, '.', field->name, '.',
				field->descriptor);
		FYEH();
		field->utf8Name = fy_strToUTF8Perm(block, field->uniqueName, exception);
		FYEH();
		field->fullName = fy_strCreatePermPersistSubstring(context->memblocks,
				field->uniqueName, clazz->className->length,
				field->uniqueName->length, exception);
		FYEH();
#if 0
		fy_strCreatePerm(block,
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
#endif
		switch (fy_strGet(field->descriptor,0)) {
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
		clazz->staticArea = fy_mmAllocatePerm(block, staticPos * sizeof(fisce_int),
				exception);
		FYEH();
	} else {

	}
}

static fy_class* getClassFromName(struct fy_context *context, fy_str *desc,
		fisce_int begin, fisce_int end, fisce_exception *exception) {
	fisce_char ch;
	fy_class *clazz;
	fy_str tmp[1];
	fy_str *finalName;
	if (begin <= 0) {
		fy_fault(exception, NULL, "Bad descriptor");
		FYEH()0;
	}
	if (end == 0) {
		/*Pri*/
		finalName = context->primitives[fy_strGet(desc,begin)];
	} else {
		/*Class or Array*/
		ch = fy_strGet(desc,begin);
		if (ch == 'L') {
			tmp->maxLength = tmp->length = end - begin - 2;
			tmp->status = 0;
			tmp->content = desc->content + begin + 1;
			finalName = fy_hashMapGet(context->memblocks, context->stringPool,
					tmp);
			if (finalName == NULL) {
				/*persist and put to string pool*/
				finalName = fy_strCreatePermPersistSubstring(context->memblocks,
						desc, begin + 1, end - 1, exception);
				FYEH()NULL;
				finalName = fy_vmCreateStringByPool(context, finalName,
						exception);
			}
		} else if (ch == '[') {
			tmp->maxLength = tmp->length = end - begin;
			tmp->status = 0;
			tmp->content = desc->content + begin;
			finalName = fy_hashMapGet(context->memblocks, context->stringPool,
					tmp);
			if (finalName == NULL) {
				/*persist and put to string pool*/
				finalName = fy_strCreatePermPersistSubstring(context->memblocks,
						desc, begin, end, exception);
				FYEH()NULL;
				finalName = fy_vmCreateStringByPool(context, finalName,
						exception);
			}
		} else {
			fy_fault(NULL, NULL, "Illegal status in class loader");
			finalName = NULL;
		}
	}
#if 0
	context->logDStr(context, finalName);
	context->logDVar(context, "\n");
#endif
	clazz = fy_vmLookupClass(context, finalName, exception);
	FYEH()0;
	if (clazz == NULL) {
		fy_fault(exception, NULL, "Bad descriptor");
		FYEH()0;
	}
	return clazz;

}

static void countParams(fy_context *context, fy_str *desc, fy_method *method,
		fisce_exception *exception) {
	fisce_byte *temp;
	fisce_byte returnType = FY_TYPE_UNKNOWN;
	fisce_int beginClass = 0, endClass = 0;
	int pc = 0;
	fisce_char ch;
	int i, maxi;
	fy_class *clazz;
	fisce_boolean begin = FALSE;
	fy_arrayList tmpList[1];
	char msg[256];

#if 0
	context->logDStr(context, desc);
	context->logDVarLn(context, " counting Params");
#endif
	temp = /*TEMP*/fy_allocate(desc->length * sizeof(fisce_byte), exception);
	FYEH();
	fy_arrayListInit(context->memblocks, tmpList, sizeof(fy_class*), 16,
			exception);

	FYEH();
	maxi = desc->length;
	for (i = 0; i < maxi; i++) {
		ch = fy_strGet(desc,i);
		if (!begin) {
			if (ch == '(') {
				begin = TRUE;
			}
		} else {
			if (ch == ')') {
				switch (fy_strGet(desc,i + 1)) {
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
					while ((ch = fy_strGet(desc,++i)) == '[') {
					}
					if (ch == 'L') {
						while ((ch = fy_strGet(desc,++i)) != ';') {
						}
					}
					endClass = i + 1;
					temp[pc++] = FY_TYPE_HANDLE;
					break;
					case 'L':
					beginClass = i;
					while ((ch = fy_strGet(desc,++i)) != ';')
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
		method->paramStackUsage = pc + ((method->access_flags & FY_ACC_STATIC)? 0 : 1);
		method->paramTypes = fy_mmAllocatePerm(context->memblocks,
				method->paramStackUsage * sizeof(fisce_byte), exception);
		FYEH();
		memcpy(method->paramTypes + ((method->access_flags & FY_ACC_STATIC)? 0 : 1), temp, pc * sizeof(fisce_byte));
		if(!(method->access_flags & FY_ACC_STATIC)){
			/*this*/
			method->paramTypes[0] = FY_TYPE_HANDLE;
		}
		method->returnType = returnType;
	}
	fy_free(temp);
}

static void loadMethods(fy_context *context, fy_class *clazz,
		fy_inputStream *is, fisce_exception *exception) {
	fy_memblock *block = context->memblocks;
	fisce_char i, count, j, jcount, k, kcount, l, lcount;
	fy_str *attrName;
	fisce_uint attrSize;
	fy_str *attrNameCode;
	fisce_uint attrSizeCode;
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
		method->uniqueName = fy_vmCreateStringByPoolV(context, exception,
				"scscs", clazz->className, '.', method->name, '.',
				method->descriptor);
		FYEH();
		method->utf8Name = fy_strToUTF8Perm(block, method->uniqueName, exception);
				FYEH();
		method->fullName = fy_strCreatePermPersistSubstring(context->memblocks,
				method->uniqueName, clazz->className->length,
				method->uniqueName->length, exception);
		FYEH();
#if 0
		fy_strCreatePerm(block,
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
#endif
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
				method->c.code = fy_mmAllocate(block, method->codeLength,
						exception);
				FYEH();
				fy_dataReadBlock(context, is, method->c.code, method->codeLength,
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
					} else if(fy_strCmp(context->sAttStackMapTable, attrNameCode) == 0) {
						method->stackMapTable = fy_mmAllocate(block, sizeof(fy_stack_map_table) + attrSizeCode - 2, exception);
						FYEH();
						method->stackMapTable->length = attrSizeCode;
						method->stackMapTable->count = fy_dataRead2(context, is, exception);
						FYEH();
						fy_dataReadBlock(context, is, method->stackMapTable->entries,attrSizeCode - 2, exception);
						FYEH();
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
		fisce_char idx) {
	return ((ConstantUtf8Info*) clazz->constantPools[idx])->string;
}

fy_inputStream *fy_clOpenResource(fy_context *context, fy_str *name,
		fisce_exception *exception) {
	int i, max;
	int size = 0;
	char *cname;
	void *ret;

	for (i = 0, max = name->length; i < max; i++) {
		size += fy_utf8Size(fy_strGet(name,i));
	}
	cname = /*TEMP*/fy_allocate(size + 7, exception);
	FYEH()NULL;
	fy_strSPrint(cname, size + 1, name);
	strcat(cname, ".class");
	ret = context->isOpen(context, cname, exception);
	fy_free(cname);
	FYEH()NULL;
	return ret;
}

static fy_class *fy_clLoadclassPriv(fy_context *context, fy_inputStream *is,
		fisce_exception *exception) {
	fisce_char i, icount;
	fy_str *attrName;
	fisce_uint attrSize;
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
	clazz->s.superClass = clazz->constantPools[fy_dataRead2(context, is,
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
void fy_clPhase2(fy_context *context, fy_class *clazz, fisce_exception *exception) {
	fisce_uint i, pos;
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
		if (clazz->s.superClass != NULL) {
			clazz->s.super = fy_vmLookupClassFromConstant(context,
					clazz->s.superClass, exception);
			FYEH();
		}
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
		if (clazz->s.superClass != NULL) {
#ifdef FY_DEBUG
			if (fy_strCmp(context->sTopClass, clazz->className) == 0) {
				fy_fault(exception, NULL,
						"java.lang.Object cannot have super class!", buf);
			}
#endif

			/*���������������������������������������������sizeAbs������������������������������������������������sizeRel���������
			 clazz->sizeAbs = clazz->super->sizeAbs + clazz->sizeRel;
			 */
			tmp = clazz;
			clazz->sizeAbs = 0;
			while (tmp != NULL) {
				clazz->sizeAbs += tmp->sizeRel;
				tmp = tmp->s.super;
			}
#ifdef FY_CL_DEBUG
			context->logDVar(context, "#CL#");
			context->logDStr(context, clazz->className);
			context->logDVar(context, " sizeAbs=%d sizeRel=%d\n",
					clazz->sizeAbs, clazz->sizeRel);

			context->logDVar(context, "   +");
			context->logDStr(context, clazz->s.super->className);
			context->logDVar(context, " sizeAbs=%d sizeRel=%d\n",
					clazz->s.super->sizeAbs, clazz->s.super->sizeRel);
#endif
			for (i = 0; i < clazz->fieldCount; i++) {
				if (clazz->fields[i]->access_flags & FY_ACC_STATIC) {

				} else {
					clazz->fields[i]->posAbs = clazz->s.super->sizeAbs
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

		if (clazz->s.super && (i = clazz->s.super->sizeAbs) > 0) {
			memcpy(clazz->fieldAbs, clazz->s.super->fieldAbs,
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
			switch (fy_strGet(field->descriptor,0)) {
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
						context->primitives[fy_strGet(field->descriptor,0)],
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
					switch (fy_strGet(field->descriptor,0)) {
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
		fisce_exception *exception) {
	fy_inputStream *is;
	fy_classDefine *cd;
	fy_class *clazz;
	fy_memblock *block = context->memblocks;
	fy_str str[1];
	fisce_exception closeException[1];
	str->content = NULL;

#if 0
	context->logDStr(context,name);
	context->logDVar(context,"\n");
#endif

	if (fy_strGet(name,0) == FY_TYPE_ARRAY) {
		clazz = fy_mmAllocatePerm(block, sizeof(fy_class), exception);
		FYEH()NULL;
		clazz->type = array_class;
		clazz->s.super = fy_vmLookupClass(context, context->sTopClass, exception);
		clazz->className = fy_vmCreateStringByPool(context, name, exception);/*fy_strCreatePermFromClone(block, name, 0, exception);*/
		FYEH()NULL;
		clazz->utf8Name = fy_strToUTF8Perm(block, clazz->className, exception);
		FYEH()NULL;
#if 0
		if (clazz->className->content == NULL) {
			fy_fault(NULL, NULL, "!!!!!");
		}
#endif
		clazz->ci.arr.arrayType = getSizeShiftForArray(name, exception);
		FYEH()NULL;
		switch (fy_strGet(clazz->className,1)) {
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
					context->primitives[fy_strGet(clazz->className,1)],
					exception);
			break;
		}
	} else if (fy_hashMapGet(block, context->mapPrimitivesRev, name) != NULL) {
		clazz = fy_mmAllocatePerm(block, sizeof(fy_class), exception);
		FYEH()NULL;
		clazz->className = fy_vmCreateStringByPool(context, name, exception);/*fy_strCreatePermFromClone(block, name, 0, exception);*/
		FYEH()NULL;
		clazz->utf8Name = fy_strToUTF8Perm(block, clazz->className, exception);
		FYEH()NULL;
		clazz->type = primitive_class;
		clazz->s.super = fy_vmLookupClass(context, context->sTopClass, exception);
		FYEH()NULL;
		clazz->ci.prm.pType = *(fisce_char*) fy_hashMapGet(block,
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
		clazz->utf8Name = fy_strToUTF8Perm(block, clazz->className, exception);
		FYEH()NULL;
		closeException->exceptionType = exception_none;
		is->isClose(context, is, closeException);
		if(closeException->exceptionType != exception_none) {
			context->logWVarLn(context, "Error closing stream errno=%s",
					closeException->exceptionDesc);
		}
		fy_mmFree(context->memblocks, is);
		FYEH()NULL;
	}
	fy_hashMapIInitPerm(block, clazz->virtualTable, 3, -1, exception);
	FYEH()NULL;
	return clazz;
}

void fy_clDefineClass(fy_context *context, fy_str *name, fisce_byte *data,
		fisce_int dataLen, fisce_exception *exception) {
	fy_classDefine *cd;
	cd = fy_hashMapGet(context->memblocks, context->customClassData, name);
	if (cd == NULL) {
		cd = fy_mmAllocatePerm(context->memblocks, sizeof(fisce_int) + dataLen,
				exception);
		FYEH();
		cd->size = dataLen;
		memcpy(cd->data, data, dataLen);
		fy_hashMapPut(context->memblocks, context->customClassData, name, cd,
				exception);
		FYEH();
	} else {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Class define dup.");
	}
}

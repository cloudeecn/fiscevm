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
static int checkConstantBonud(fy_class *clazz, int idx) {
	if (idx > clazz->constantPoolCount) {
		vm_die("Constant index out of bound %d/%d", idx,
				clazz->constantPoolCount);
	}
	return idx;
}
#else
#define checkConstantBonud(C,I) (I)
#endif

static int getSizeShiftForArray(fy_str *arrayName) {
	switch (arrayName->content[1]) {
	case TYPE_BOOLEAN:
	case TYPE_BYTE:
		return fy_SIZE_SHIFT_BYTE;
	case TYPE_DOUBLE:
	case TYPE_LONG:
		return fy_SIZE_SHIFT_LONG;
	default:
		return fy_SIZE_SHIFT_INT;
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

static void fillConstantContent(fy_VMContext *context, fy_class *ret,
		fy_data *data) {
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
	jubyte *constantTypes;

	ret->constantPoolCount = fy_dataRead2(data);
	/*read constantPool*/
	ret->constantTypes = constantTypes = fy_vmAllocate(context,
			sizeof(jubyte) * (ret->constantPoolCount + 1));
	ret->constantPools = constantPools = fy_vmAllocate(context,
			sizeof(void*) * (ret->constantPoolCount + 1));
	/*Phase 1*/
	for (i = 1, imax = ret->constantPoolCount; i < imax; i++) {
		tag = fy_dataRead(data);
		constantTypes[i] = tag;
		cpSkip = 0;
		switch (tag) {
		case CONSTANT_Class:
			tmpConstantClass = fy_vmAllocate(context, sizeof(ConstantClass));
			tmpConstantClass->ci.name_index = checkConstantBonud(ret,
					fy_dataRead2(data));
			tmp = tmpConstantClass;
			break;
		case CONSTANT_Fieldref:
			tmpConstantFieldRef = fy_vmAllocate(context,
					sizeof(ConstantFieldRef));
			tmpConstantFieldRef->class_index = checkConstantBonud(ret,
					fy_dataRead2(data));
			tmpConstantFieldRef->name_type_index = checkConstantBonud(ret,
					fy_dataRead2(data));
			tmp = tmpConstantFieldRef;
			break;
		case CONSTANT_Methodref:
		case CONSTANT_InterfaceMethodref:
			tmpConstantMethodRef = fy_vmAllocate(context,
					sizeof(ConstantMethodRef));
			tmpConstantMethodRef->class_index = checkConstantBonud(ret,
					fy_dataRead2(data));
			tmpConstantMethodRef->name_type_index = checkConstantBonud(ret,
					fy_dataRead2(data));
			tmp = tmpConstantMethodRef;
			break;
		case CONSTANT_String:
			tmpConstantStringInfo = fy_vmAllocate(context,
					sizeof(ConstantStringInfo));
			tmpConstantStringInfo->ci.string_index = checkConstantBonud(ret,
					fy_dataRead2(data));
			tmp = tmpConstantStringInfo;
			break;
		case CONSTANT_Integer:
		case CONSTANT_Float:
			tmpConstantIntegerFloatInfo = fy_vmAllocate(context,
					sizeof(ConstantIntegerFloatInfo));
			tmpConstantIntegerFloatInfo->value = fy_dataRead4(data);
			tmp = tmpConstantIntegerFloatInfo;
			break;
		case CONSTANT_Double:
		case CONSTANT_Long:
			cpSkip = 1;
			tmpConstantLongDoubleInfo = fy_vmAllocate(context,
					sizeof(ConstantLongDoubleInfo));
			tmpConstantLongDoubleInfo->value = fy_dataRead8(data);
			tmp = tmpConstantLongDoubleInfo;
			break;
		case CONSTANT_NameAndType:
			tmpConstantNameAndTypeInfo = fy_vmAllocate(context,
					sizeof(ConstantNameAndTypeInfo));
			tmpConstantNameAndTypeInfo->name_index = checkConstantBonud(ret,
					fy_dataRead2(data));
			tmpConstantNameAndTypeInfo->descriptor_index = checkConstantBonud(
					ret, fy_dataRead2(data));
			tmp = tmpConstantNameAndTypeInfo;
			break;
		case CONSTANT_Utf8:
			tmpConstantUtf8Info = fy_vmAllocate(context,
					sizeof(ConstantUtf8Info));
			j = fy_dataRead2(data);
			tmpConstantUtf8Info->string = fy_vmAllocate(context,
					sizeof(fy_str));
			fy_strInit(context, tmpConstantUtf8Info->string,
					fy_utf8SizeS(data->data, j));
			fy_strAppendUTF8(context, tmpConstantUtf8Info->string, data->data,
					j);
			fy_dataSkip(data, j);
			tmp = tmpConstantUtf8Info;
			break;
		default:
			vm_die("Unknown constant pool type %d", tag);
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
			vm_die("Unknown constant pool type %d", tag);
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
			tmpConstantFieldRef->nameType = fy_vmAllocate(context,
					sizeof(fy_str));
			fy_strInit(
					context,
					tmpConstantFieldRef->nameType,
					2 + tmpConstantFieldRef->constantNameType->name->length
							+ tmpConstantFieldRef->constantNameType->descriptor->length);
			fy_strAppendUTF8(context, tmpConstantFieldRef->nameType, ".", 1);
			fy_strAppend(context, tmpConstantFieldRef->nameType,
					tmpConstantFieldRef->constantNameType->name);
			fy_strAppendUTF8(context, tmpConstantFieldRef->nameType, ".", 1);
			fy_strAppend(context, tmpConstantFieldRef->nameType,
					tmpConstantFieldRef->constantNameType->descriptor);
			break;
		case CONSTANT_Methodref:
		case CONSTANT_InterfaceMethodref:
			tmpConstantMethodRef = (ConstantMethodRef*) tmp;
			tmpConstantMethodRef->constantClass =
					(ConstantClass*) (constantPools[tmpConstantMethodRef->class_index]);
			tmpConstantMethodRef->constantNameType =
					(ConstantNameAndTypeInfo*) (constantPools[tmpConstantMethodRef->name_type_index]);
			tmpConstantMethodRef->nameType = fy_vmAllocate(context,
					sizeof(fy_str));
			fy_strInit(
					context,
					tmpConstantMethodRef->nameType,
					2 + tmpConstantMethodRef->constantNameType->name->length
							+ tmpConstantMethodRef->constantNameType->descriptor->length);
			fy_strAppendUTF8(context, tmpConstantMethodRef->nameType, ".", 1);
			fy_strAppend(context, tmpConstantMethodRef->nameType,
					tmpConstantMethodRef->constantNameType->name);
			fy_strAppendUTF8(context, tmpConstantMethodRef->nameType, ".", 1);
			fy_strAppend(context, tmpConstantMethodRef->nameType,
					tmpConstantMethodRef->constantNameType->descriptor);
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
			vm_die("Unknown constant pool type %d", tag);
			break;
		}
	}
}

static void loadInterfaces(fy_VMContext *context, fy_class *clazz,
		fy_data *data) {
	int i, count;
	fy_exception exception;
	exception.exceptionType = exception_none;
	exception.exceptionName[0] = 0;
	exception.exceptionDesc[0] = 0;
	clazz->interfacesCount = count = fy_dataRead2(data);
	clazz->interfaces = fy_vmAllocate(context, sizeof(fy_class*) * count);
	for (i = 0; i < count; i++) {
		clazz->interfaces[i] = fy_vmLookupClassFromConstant(context,
				(ConstantClass*) clazz->constantPools[fy_dataRead2(data)],
				&exception);
		if (exception.exceptionType != exception_none) {
			vm_die("Exception %s caught: %s", exception.exceptionName,
					exception.exceptionDesc);
		}
#ifdef _DEBUG
		fy_strPrint(clazz->interfaces[i]->className);
#endif
	}
}

static void loadFields(fy_VMContext *context, fy_class *clazz, fy_data *data) {
	int i, count, j, countj;
	int length;

	fy_str *attrName;
	juint attrSize;
	fy_field *field;
	juint pos = 0;
	juint staticPos = 0;
	fy_str *strConstantValue = fy_vmAllocate(context, sizeof(fy_str));

	fy_strInit(context, strConstantValue, 13);
	fy_strAppendUTF8(context, strConstantValue, "ConstantValue", 13);

	clazz->fieldCount = count = fy_dataRead2(data);
	clazz->fields = fy_vmAllocate(context, sizeof(fy_field*) * count);
	for (i = 0; i < count; i++) {
		field = fy_vmAllocate(context, sizeof(fy_field));
		field->owner = clazz;
		field->access_flags = fy_dataRead2(data);
		field->name = ((ConstantUtf8Info*) clazz->constantPools[fy_dataRead2(
				data)])->string;
		field->descriptor =
				((ConstantUtf8Info*) clazz->constantPools[fy_dataRead2(data)])->string;
		field->fullName = fy_vmAllocate(context, sizeof(fy_str));
		fy_strInit(context, field->fullName, 16);
		field->uniqueName = fy_vmAllocate(context, sizeof(fy_str));
		fy_strInit(context, field->uniqueName, 16);
		fy_strAppendUTF8(context, field->fullName, ".", 1);
		fy_strAppend(context, field->fullName, field->name);
		fy_strAppendUTF8(context, field->fullName, ".", 1);
		fy_strAppend(context, field->fullName, field->descriptor);
		fy_strAppend(context, field->uniqueName, clazz->className);
		fy_strAppend(context, field->uniqueName, field->fullName);

		switch (field->descriptor->content[0]) {
		case 'D':
		case 'J':
			length = 2;
			break;
		default:
			length = 1;
			break;
		}
		if (field->access_flags & fy_ACC_STATIC) {
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
		fy_vmRegisterField(context, field);
		clazz->fields[i] = field;
	}
	clazz->sizeRel = pos;
	clazz->staticSize = staticPos;
	if (staticPos > 0) {
		clazz->staticArea = fy_vmAllocate(context,
				staticPos << fy_SIZE_SHIFT_INT);
	} else {

	}
	fy_strDestroy(context, strConstantValue);
	fy_vmFree(context, strConstantValue);
	strConstantValue = 0;
}

static void countParams(fy_VMContext *context, fy_str *desc, fy_method *method) {
	jbyte *temp;
	jbyte returnType=TH_TYPE_UNKNOWN;
	int pc = 0;
	jchar ch;
	int i, maxi;
	jboolean begin = FALSE;
	char msg[256];
	temp = vm_allocate(desc->length * sizeof(temp));
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
					returnType = TH_TYPE_INT;
					break;
				case 'D':
				case 'J':
					returnType = TH_TYPE_WIDE;
					break;
				case '[':
				case 'L':
					returnType = TH_TYPE_HANDLE;
					break;
				case 'V':
				default:
					returnType = TH_TYPE_UNKNOWN;
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
					temp[pc++] = TH_TYPE_INT;
					break;
				case 'D':
				case 'J':
					temp[pc++] = TH_TYPE_WIDE;
					temp[pc++] = TH_TYPE_WIDE2;
					break;
				case '[':
					while ((ch = desc->content[++i]) == '[') {
					}
					if (ch == 'L') {
						while ((ch = desc->content[++i]) != ';') {
						}
					}
					temp[pc++] = TH_TYPE_HANDLE;
					break;
				case 'L':
					while ((ch = desc->content[++i]) != ';')
						;
					temp[pc++] = TH_TYPE_HANDLE;
					break;
				default:
					msg[0] = 0;
					if (method != NULL) {
						fy_strSPrint(msg, sizeof(msg), method->uniqueName);
					} else {
						fy_strSPrint(msg, sizeof(msg), desc);
					}
					vm_die("Malformed description data for %s", msg);
					break;
				}
			}
		}
	}
	if (method != NULL) {
		method->paramTypes = fy_vmAllocate(context, pc * sizeof(jbyte));
		method->paramCount = pc;
		memcpy(method->paramTypes, temp, pc * sizeof(jbyte));
		method->returnType = returnType;
	}
	vm_free(temp);
}

static void loadMethods(fy_VMContext *context, fy_class *clazz, fy_data *data) {
	jchar i, count, j, jcount, k, kcount, l, lcount;
	fy_str *ATT_CODE = fy_strAllocateFromUTF8(context, "Code");
	fy_str *ATT_LINENUM = fy_strAllocateFromUTF8(context, "fy_lineNumber");
	fy_str *ATT_SYNTH = fy_strAllocateFromUTF8(context, "Synthetic");
	fy_str *attrName;
	juint attrSize;
	fy_str *attrNameCode;
	juint attrSizeCode;
	fy_method *method;
	clazz->methodCount = count = fy_dataRead2(data);
	clazz->methods = fy_vmAllocate(context, sizeof(fy_method*) * count);
#if 0
	printf("Loading %d methods...\n", count);
#endif
	for (i = 0; i < count; i++) {
		method = fy_vmAllocate(context, sizeof(fy_method));
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
		method->fullName = fy_vmAllocate(context, sizeof(fy_str));
		fy_strInit(context, method->fullName,
				method->name->length + method->descriptor->length + 2);
		method->uniqueName = fy_vmAllocate(context, sizeof(fy_str));
		fy_strInit(
				context,
				method->uniqueName,
				method->name->length + method->descriptor->length
						+ clazz->className->length + 2);
		fy_strAppendUTF8(context, method->fullName, ".", 1);
		fy_strAppend(context, method->fullName, method->name);
		fy_strAppendUTF8(context, method->fullName, ".", 1);
		fy_strAppend(context, method->fullName, method->descriptor);
		fy_strAppend(context, method->uniqueName, clazz->className);
		fy_strAppend(context, method->uniqueName, method->fullName);
		jcount = fy_dataRead2(data);
		for (j = 0; j < jcount; j++) {
			attrName = fy_clGetConstantString(context, clazz,
					fy_dataRead2(data));
			attrSize = fy_dataRead4(data);
			if (fy_strCmp(ATT_CODE, attrName) == 0) {
				method->max_stack = fy_dataRead2(data);
				method->max_locals = fy_dataRead2(data);
				method->codeLength = fy_dataRead4(data);
				method->code = fy_vmAllocate(context, method->codeLength);
				memcpy(method->code, data->data, method->codeLength);
				fy_dataSkip(data, method->codeLength);
				kcount = fy_dataRead2(data);
				method->exception_table_length = kcount;
				method->exception_table = fy_vmAllocate(context,
						sizeof(fy_exceptionHandler) * kcount);
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
						method->line_number_table = fy_vmAllocate(context,
								sizeof(fy_lineNumber) * lcount);
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
		countParams(context, method->descriptor, method);
		fy_vmRegisterMethod(context, method);
		clazz->methods[i] = method;
	}fy_strRelease(context, ATT_CODE);
	fy_strRelease(context, ATT_LINENUM);
	fy_strRelease(context, ATT_SYNTH);
}

/************public***************/
fy_str *fy_clGetConstantString(fy_VMContext *context, fy_class *clazz,
		jchar idx) {
	return ((ConstantUtf8Info*) clazz->constantPools[idx])->string;
}

fy_data *fy_clOpenResource(fy_VMContext *context, fy_str *name) {
	int i, max;
	int size = 0;
	char *cname;
	fy_data *ret;

	for (i = 0, max = name->length; i < max; i++) {
		size += fy_utf8Size(name->content[i]);
	}
	cname = vm_allocate(size + 1);
	fy_strSPrint(cname, size + 1, name);
	ret = fy_resourceAllocateData(context, cname);
	vm_free(cname);
	return ret;
}

void fy_clCloseResource(fy_VMContext *context, fy_data *data) {
	fy_resourceReleaseData(context, data);
}

static fy_class *fy_clLoadclassPriv(fy_VMContext *context, fy_data *data) {
	jchar i, icount;
	fy_str *attrName;
	juint attrSize;
	fy_str *ATTR_SOURCE_FILE = fy_strAllocateFromUTF8(context, "SourceFile");
	fy_class *clazz = fy_vmAllocate(context, sizeof(fy_class));
	clazz->type = obj;
	fy_dataSkip(data, 8);
	fillConstantContent(context, clazz, data);
	clazz->constantPools[0] = NULL;
	clazz->accessFlags = fy_dataRead2(data);
	clazz->thisClass = (clazz->constantPools)[fy_dataRead2(data)];
	clazz->className = clazz->thisClass->ci.className;
	clazz->superClass = clazz->constantPools[fy_dataRead2(data)];
	loadInterfaces(context, clazz, data);
	loadFields(context, clazz, data);
	loadMethods(context, clazz, data);
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
		vm_die("Still %d bytes unread!", data->size);
	}
#endif
	clazz->phase = 1;
	return clazz;
}
void fy_clPhase2(fy_VMContext *context, fy_class *clazz,
		fy_exception *exception) {
	fy_str *name;
	jchar i;
	fy_str *FINALIZE = fy_strAllocateFromUTF8(context, ".finalize.()V");
#ifdef _DEBUG
	char buf[255];
#endif
	switch (clazz->type) {
	case arr:
		switch (clazz->className->content[1]) {
		case TYPE_ARRAY:
			name = fy_vmAllocate(context, sizeof(fy_str));
			fy_strInit(context, name, clazz->className->length);
			name->length = clazz->className->length - 1;
			memcpy(name->content, clazz->className->content + 1,
					(clazz->className->length - 1) << 1);
			clazz->ci.arr.contentClass = fy_vmLookupClass(context, name,
					exception);
			fy_strDestroy(context, name);
			fy_vmFree(context, name);
			break;
		case TYPE_HANDLE:
			name = fy_vmAllocate(context, sizeof(fy_str));
			fy_strInit(context, name, clazz->className->length);
			name->length = clazz->className->length - 3;
			memcpy(name->content, clazz->className->content + 2,
					(clazz->className->length - 3) << 1);
			clazz->ci.arr.contentClass = fy_vmLookupClass(context, name,
					exception);
			fy_strDestroy(context, name);
			fy_vmFree(context, name);
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
				vm_die("java.lang.Object cannot have super class!", buf);
			}
#endif
			clazz->super = fy_vmLookupClassFromConstant(context,
					clazz->superClass, exception);
			if (exception->exceptionType != exception_none) {
				break;
			}
			clazz->sizeAbs = clazz->super->sizeAbs + clazz->sizeRel;
			for (i = 0; i < clazz->fieldCount; i++) {
				if (clazz->fields[i]->access_flags & fy_ACC_STATIC) {

				} else {
					clazz->fields[i]->posAbs = clazz->super->sizeAbs
							+ clazz->fields[i]->posRel;
				}
			}
			if (fy_vmLookupMethodVirtual(context, clazz, FINALIZE)
					!= fy_vmLookupMethodVirtual(
							context,
							fy_vmLookupClass(context, context->sTopClass,
									exception), FINALIZE)) {
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
				vm_die("%s must have super class!", buf);
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
	fy_strRelease(context, FINALIZE);
}
fy_class *fy_clLoadclass(fy_VMContext *context, fy_str *name,
		fy_exception *exception) {

	fy_data *data;
	fy_data tmpData;
	fy_class *clazz;

	if (name->content[0] == TYPE_ARRAY) {
		clazz = fy_vmAllocate(context, sizeof(fy_class));
		clazz->type = arr;
		clazz->super = fy_vmLookupClass(context, context->sTopClass, exception);
		clazz->className = fy_strAllocateClone(context, name);
		clazz->ci.arr.sizeShift = getSizeShiftForArray(name);
	} else if (fy_hashMapGet(context, context->mapPrimitivesRev, name)
			!= NULL) {
		clazz = fy_vmAllocate(context, sizeof(fy_class));
		clazz->className = fy_strAllocateClone(context, name);
		clazz->type = prm;
		clazz->super = fy_vmLookupClass(context, context->sTopClass, exception);
		clazz->ci.prm.pType = *(jchar*) fy_hashMapGet(context,
				context->mapPrimitivesRev, name);
	} else {
		fy_str *localName = fy_vmAllocate(context, sizeof(fy_str));
		fy_strInit(context, localName, name->length + 20);
		fy_strAppendUTF8(context, localName, "../rt/bin/", -1);
		fy_strAppend(context, localName, name);
		fy_strAppendUTF8(context, localName, ".class", 6);

		data = fy_clOpenResource(context, localName);
		if (data == NULL) {
			fy_strPrint(localName);
			printf("\n");
			vm_die("Class not found Exception!");
		}

		tmpData = *data;

		clazz = fy_clLoadclassPriv(context, &tmpData);
		fy_clCloseResource(context, data);
		fy_strDestroy(context, localName);
		fy_vmFree(context, localName);
	}
	return clazz;
}

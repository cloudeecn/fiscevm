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

#ifndef FY_VMCONTEXT_H_
#define FY_VMCONTEXT_H_

#include "../fisceprt.h"
#include "../fiscestu.h"
#include "../fy_util/LnkList.h"
#include "../fy_util/MemMan.h"
#include "../fy_util/HashMap.h"
#include "Class.h"
#include "ClassLoader.h"
#include "Heap.h"
#include "CoreHandlers.h"
#ifdef	__cplusplus
extern "C" {
#endif
void fy_vmContextInit(fy_context *context, fy_exception *exception);
void fy_vmContextDestroy(fy_context *context);

fy_boolean fy_vmBootup(fy_context *context, const char* bootStrapClass,
		fy_exception *exception);

void fy_vmSave(fy_context *context, fy_exception *exception);
void fy_vmBootFromData(fy_context *context, fy_exception *exception);

void fy_vmRegisterField(fy_context *context, fy_field *field,
		fy_exception *exception);
fy_field *fy_vmLookupFieldVirtual(fy_context *context, fy_class *clazz,
		fy_str *name, fy_exception *exception);
fy_field *fy_vmLookupFieldFromConstant(fy_context *context,
		ConstantFieldRef *fieldInfo, fy_exception *exception);

fy_field *fy_vmGetField(fy_context *context, fy_str *uniqueName);

void fy_vmRegisterMethod(fy_context *context, fy_method *method,
		fy_exception *exception);
fy_method *fy_vmLookupMethodVirtual(fy_context *context, fy_class *clazz,
		fy_str *name, fy_exception *exception);
fy_method *fy_vmLookupMethodVirtualByMethod(fy_context *context,
		fy_class *clazz, fy_method *method, fy_exception *exception);
fy_method *fy_vmLookupMethodFromConstant(fy_context *context,
		ConstantMethodRef *methodInfo, fy_exception *exception);

fy_method *fy_vmGetMethod(fy_context *context, fy_str *uniqueName);

void fy_vmRegisterClass(fy_context *context, fy_class *clazz,
		fy_exception *exception);
fy_class *fy_vmLoadClass(fy_context *context, fy_str *name,
		fy_exception *exception);
fy_class *fy_vmLookupClass(fy_context *context, fy_str *name,
		fy_exception *exception);
fy_class *fy_vmLookupClassFromConstant(fy_context *context,
		ConstantClass *classInfo, fy_exception *exception);

void fy_vmRegisterNativeHandler(fy_context *context, const char *name,
		void *data, fy_nhFunction handler, fy_exception *exception);
void fy_vmUnRegisterNativeHandler(fy_context *context, const char *name,
		fy_exception *exception);
fy_class *fy_vmGetClassFromClassObject(fy_context *context, fy_uint handle,
		fy_exception *exception);
fy_class *fy_vmLookupClassFromExceptionHandler(fy_context *context,
		fy_exceptionHandler *exceptionHandler, fy_exception *exception);
int fy_vmGetClassObjHandle(fy_context *context, fy_class *clazz,
		fy_exception *exception);
fy_int fy_vmGetMethodObjHandle(fy_context *context, fy_method *method,
		fy_exception *exception);
fy_int fy_vmGetFieldObjHandle(fy_context *context, fy_field *field,
		fy_exception *exception);
void fy_vmDefineClass(fy_context *context, fy_str *name, fy_byte *data,
		fy_int dataLen, fy_exception *exception);

/*String pool*/
fy_str *fy_vmCreateStringByPool(fy_context *context, fy_str *tmp,
		fy_exception *exception);
fy_str *fy_vmCreateStringByPoolV(fy_context *context, fy_exception *exception,
		const char *pattern, ...);
fy_str *fy_vmCreateStringByPoolVA(fy_context *context, fy_strVA *va,
		fy_exception *exception);
#ifdef	__cplusplus
}
#endif
#endif /* FY_VMCONTEXT_H_ */

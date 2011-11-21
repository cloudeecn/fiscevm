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

#ifndef FY_VMCONTEXT_H_
#define FY_VMCONTEXT_H_

#include "Portable.h"
#include "Global.h"
#include "Structs.h"
#include "LinkedList.h"
#include "Class.h"
#include "HashMap.h"
#include "ClassLoader.h"
#include "Heap.h"
#include "CoreHandlers.h"
#include "VMPortable.h"

#ifdef	__cplusplus
extern "C" {
#endif

void fy_vmContextInit(fy_VMContext *context, fy_exception *exception);
void fy_vmContextDestroy(fy_VMContext *context);

void fy_vmBootup(fy_VMContext *context, jchar* bootStrapClass);
void* fy_vmAllocate(fy_VMContext *context, int size);
void fy_vmFree(fy_VMContext *context, void *address);

void fy_vmRegisterField(fy_VMContext *context, fy_field *field);
fy_field *fy_vmLookupFieldVirtual(fy_VMContext *context, fy_class *clazz,
		fy_str *name);
fy_field *fy_vmLookupFieldStatic(fy_VMContext *context, fy_class *clazz,
		fy_str *fieldName);
fy_field *fy_vmLookupFieldFromConstant(fy_VMContext *context,
		ConstantFieldRef *fieldInfo, fy_exception *exception);

fy_field *fy_vmGetField(fy_VMContext *context, fy_str *uniqueName);

void fy_vmRegisterMethod(fy_VMContext *context, fy_method *method);
fy_method *fy_vmLookupMethodVirtual(fy_VMContext *context, fy_class *clazz,
		fy_str *name);
fy_method *fy_vmLookupMethodStatic(fy_VMContext *context, fy_class *clazz,
		fy_str *methodName);
fy_method *fy_vmLookupMethodFromConstant(fy_VMContext *context,
		ConstantMethodRef *methodInfo, fy_exception *exception);

fy_method *fy_vmGetMethod(fy_VMContext *context, fy_str *uniqueName);

void fy_vmRegisterClass(fy_VMContext *context, fy_class *clazz);
fy_class *fy_vmLookupClass(fy_VMContext *context, fy_str *name,
		fy_exception *exception);
fy_class *fy_vmLookupClassFromConstant(fy_VMContext *context,
		ConstantClass *classInfo, fy_exception *exception);

void fy_vmRegisterNativeHandler(fy_VMContext *context, const char *name,
		void *data, fy_nhFunction handler);

fy_class *fy_vmLookupClassFromExceptionHandler(fy_VMContext *context,
		fy_exceptionHandler *exceptionHandler, fy_exception *exception);
#ifdef	__cplusplus
}
#endif
#endif /* FY_VMCONTEXT_H_ */

/**
 *  Copyright 2010-2015 Yuxuan Huang. All rights reserved.
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

#include "fisce.h"

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fy_util/HashMap.h"
#include "fy_util/Utf8.h"
#include "fy_util/String.h"

#include "fyc/Constants.h"
#include "fyc/ClassStruct.h"
#include "fyc/VMContext.h"
#include "fyc/Class.h"
#include "fyc/Thread.h"
#include "fyc/Heap.h"
#include "fyc/ThreadManager.h"
#include "fyc/ExportSupport.h"
#include "fyc/VMContext.h"


static fisce_uint contextSeq = 0;

struct nativeData{
    fisce_context *ctx;
    fisce_nhf fun;
    void *data;
};

struct fisce_context{
    fisce_uint contextId;
    fy_context context[1];
};

struct fisce_class{
    fisce_uint contextId;
    char *utf8Name;
    fy_class *clazz;
};

struct fisce_method{
    fisce_uint contextId;
    char *className;
    char *utf8Name;
    fy_method *method;
};

struct fisce_field{
    fisce_uint contextId;
    char *className;
    char *utf8Name;
    fy_field *field;
};

size_t sizeofContext(){
    return sizeof(fisce_context);
}

static fisce_int nativeDelegator(
                           fy_context *context,
                           fy_thread *thread,
                           void *data,
                           fisce_stack_item *args,
                           fisce_int argsCount,
                           fisce_int ops,
                           fisce_exception *exception
                                 ){
    struct nativeData *ndata = (struct nativeData *)data;
    (*(ndata->fun))(ndata->ctx, thread->threadId, ndata->data, args, argsCount, ops, exception);
    if(exception->exceptionType == exception_none){
        return ops - 1;
    } else {
        return ops;
    }
}

void fisceInitContext(fisce_context *context, fisce_exception *exception){
    context->contextId = ++contextSeq;
    fy_vmContextInit(context->context, exception);
    FYEH();
}

void fisceDestroyContext(fisce_context *context){
    fy_vmContextDestroy(context->context);
}

void fisceBootFromMain(fisce_context *context, const char *name,
                       fisce_exception *exception){
    fy_vmBootup(context->context, name, exception);
}

void fisceBootFromDataFile(fisce_context *context, fisce_exception *exception);
void fisceSaveToFile(fisce_context *context, fisce_exception *exception);
void fisceRun(fisce_context *context, fisce_message *message,
              fisce_exception *exception);
void fisceSetGCExtra(fisce_context *context, void *data, void (*beforeHandler)(void* data), void (*getExtraHandler)(void* data, fisce_int* size, fisce_int **phandles), void (*afterHandler)(void* data)){
    context->context->gcCustomData = data;
    context->context->beforeGC = beforeHandler;
    context->context->afterGC = afterHandler;
    context->context->getExtraGCKeep = getExtraHandler;
}


void fisceReturnInt(fisce_context *context, fisce_int threadId, fisce_int value){
    return fy_nativeReturnInt(context->context, context->context->threads[threadId], value);
}
void fisceReturnHandle(fisce_context *context, fisce_int threadId, fisce_int value){
    return fy_nativeReturnHandle(context->context, context->context->threads[threadId], value);
}
void fisceReturnLong(fisce_context *context, fisce_int threadId, fisce_long value){
    return fy_nativeReturnLong(context->context, context->context->threads[threadId], value);
}
void fisceReturnFloat(fisce_context *context, fisce_int threadId, fisce_long value){
    return fy_nativeReturnFloat(context->context, context->context->threads[threadId], value);
}
void fisceReturnDouble(fisce_context *context, fisce_int threadId, fisce_long value){
    return fy_nativeReturnDouble(context->context, context->context->threads[threadId], value);
}

fisce_class *fisceLookupClass(fisce_context *context, const char *name,
                              fisce_exception *exception){
    fy_str str[1];
    fy_class *clazz;
    fisce_class *ret;
    fisce_int len = (fisce_int)strlen(name);
    str->content = NULL;
    fy_strInitWithUTF8(context->context->memblocks, str, name, exception);
    FYEH()NULL;
    clazz = fy_vmLookupClass(context->context, str, exception);
    fy_strDestroy(context->context->memblocks, str);
    FYEH()NULL;
    ret = fy_allocate(sizeof(fisce_class), exception);
    FYEH()NULL;
    ret->utf8Name = fy_allocate(len + 1, exception);
    FYEX(fy_free(ret); return NULL);
    strncpy(ret->utf8Name, name, len);
    ret->contextId = context->contextId;
    ret->clazz = clazz;
    return ret;
}

void fisceReleaseClass(fisce_class *clazz){
    fy_free(clazz->utf8Name);
    fy_free(clazz);
}

static fy_class *getFyClass(fisce_context *context, fisce_class *fisceClass, fisce_exception *exception){
    fy_str *strName;
    if(fisceClass->contextId != context->contextId){
        strName = fy_allocate(sizeof(fy_str), exception);
        FYEH()NULL;
        fy_strInitWithUTF8(context->context->memblocks, strName, fisceClass->utf8Name, exception);
        FYEX(fy_free(strName);return NULL);
        fisceClass->contextId = context->contextId;
        fisceClass->clazz = fy_vmLookupClass(context->context, strName, exception);
        fy_strDestroy(context->context->memblocks, strName);
        fy_free(strName);
        FYEH()NULL;
    }
    return fisceClass->clazz;
}

const char *fisceClassName(fisce_context *context, fisce_class *clazz){
    return clazz->utf8Name;
}

fisce_boolean fisceClassCanCastTo(fisce_context *context, fisce_class *from,
                                  fisce_class *to, fisce_exception *exception){
    fy_class *fromClass, *toClass;
    fromClass = getFyClass(context, from, exception);
    FYEH()0;
    toClass = getFyClass(context, to, exception);
    FYEH()0;
    return fy_classCanCastTo(context->context, fromClass, toClass, 1);
}

fisce_field *fisceGetField(fisce_context *context, const char *name, fisce_exception *exception){
    fy_str str[1];
    fy_field *field;
    fisce_field *ret;
    fisce_int len = (fisce_int)strlen(name);
    str->content = NULL;
    fy_strInitWithUTF8(context->context->memblocks, str, name, exception);
    FYEH()NULL;
    field = fy_vmGetField(context->context, str);
    if(field == NULL){
        fy_fault(exception, FY_EXCEPTION_ABSTRACT, "Can't find field %s", name);
    }
    fy_strDestroy(context->context->memblocks, str);
    FYEH()NULL;
    
    ret = fy_allocate(sizeof(fisce_field), exception);
    FYEH()NULL;
    
    ret->utf8Name = fy_allocate(len + 1, exception);
    FYEX(fy_free(ret); return NULL);
    strncpy(ret->utf8Name, name, len);
    
    len = (fisce_int)strlen(field->owner->utf8Name);
    ret->className = fy_allocate(len + 1, exception);
    FYEX(fy_free(ret->utf8Name); fy_free(ret); return NULL);
    strncpy(ret->className, field->owner->utf8Name, len);
    
    ret->contextId = context->contextId;
    ret->field = field;
    return ret;
}

fisce_field *fisceLookupFieldVirtual(fisce_context *context, fisce_class *clazz,
                                     const char *name, fisce_exception *exception){
    fy_str str[1];
    fy_class *cl = getFyClass(context, clazz, exception);
    fy_field *fld;
    FYEH()NULL;
    str->content = NULL;
    fy_strInitWithUTF8(context->context->memblocks, str, name, exception);
    FYEH()NULL;
    fld = fy_vmLookupFieldVirtual(context->context, cl, str, exception);
    fy_strDestroy(context->context->memblocks, str);
    return fisceGetField(context, fld->utf8Name, exception);
}

const char *fisceFieldName(fisce_context *context, fisce_field *field){
    return field->utf8Name;
}

void fisceReleaseField(fisce_field *field){
    fy_free(field->className);
    fy_free(field->utf8Name);
    fy_free(field);
}

static fy_field *getFyField(fisce_context *context, fisce_field *fisceField, fisce_exception *exception){
    fy_str *strName;
    if(fisceField->contextId != context->contextId){
        strName = fy_allocate(sizeof(fy_str), exception);
        FYEH()NULL;
        fy_strInitWithUTF8(context->context->memblocks, strName, fisceField->className, exception);
        FYEX(fy_free(strName); return NULL;);
        fy_vmLookupClass(context->context, strName, exception);
        fy_strDestroy(context->context->memblocks, strName);
        FYEX(fy_free(strName); return NULL;);
        
        fy_strInitWithUTF8(context->context->memblocks, strName, fisceField->className, exception);
        FYEX(fy_free(strName); return NULL;);
        fisceField->field = fy_vmGetField(context->context, strName);
        if(fisceField->field == NULL){
            fy_fault(exception, FY_EXCEPTION_ABSTRACT, "Can't find field %s", fisceField->utf8Name);
        }
        fy_strDestroy(context->context->memblocks, strName);
        fy_free(strName);
        FYEH()NULL;
    }
    return fisceField->field;
}

fisce_method *fisceGetMethod(fisce_context *context, const char *name, fisce_exception *exception){
    fy_str str[1];
    fy_method *method;
    fisce_method *ret;
    fisce_int len = (fisce_int)strlen(name);
    str->content = NULL;
    fy_strInitWithUTF8(context->context->memblocks, str, name, exception);
    FYEH()NULL;
    method = fy_vmGetMethod(context->context, str);
    if(method == NULL){
        fy_fault(exception, FY_EXCEPTION_ABSTRACT, "Can't find method %s", name);
    }
    fy_strDestroy(context->context->memblocks, str);
    FYEH()NULL;
    
    ret = fy_allocate(sizeof(fisce_method), exception);
    FYEH()NULL;
    
    ret->utf8Name = fy_allocate(len + 1, exception);
    FYEX(fy_free(ret); return NULL);
    strncpy(ret->utf8Name, name, len);
    
    len = (fisce_int)strlen(method->owner->utf8Name);
    ret->className = fy_allocate(len + 1, exception);
    FYEX(fy_free(ret->utf8Name); fy_free(ret); return NULL);
    strncpy(ret->className, method->owner->utf8Name, len);
    
    ret->contextId = context->contextId;
    ret->method = method;
    return ret;
}

fisce_method *fisceLookupMethodVirtual(fisce_context *context, fisce_class *clazz,
                                     const char *name, fisce_exception *exception){
    fy_str str[1];
    fy_class *cl = getFyClass(context, clazz, exception);
    fy_method *mtd;
    FYEH()NULL;
    str->content = NULL;
    fy_strInitWithUTF8(context->context->memblocks, str, name, exception);
    FYEH()NULL;
    mtd = fy_vmLookupMethodVirtual(context->context, cl, str, exception);
    fy_strDestroy(context->context->memblocks, str);
    return fisceGetMethod(context, mtd->utf8Name, exception);
}

const char *fisceMethodName(fisce_context *context, fisce_method *method){
    return method->utf8Name;
}

void fisceReleaseMethod(fisce_field *method){
    fy_free(method->className);
    fy_free(method->utf8Name);
    fy_free(method);
}

MAYBE_UNUSED static fy_method *getFyMethod(fisce_context *context, fisce_method *fisceMethod, fisce_exception *exception){
    fy_str *strName;
    if(fisceMethod->contextId != context->contextId){
        strName = fy_allocate(sizeof(fy_str), exception);
        FYEH()NULL;
        fy_strInitWithUTF8(context->context->memblocks, strName, fisceMethod->className, exception);
        FYEX(fy_free(strName); return NULL;);
        fy_vmLookupClass(context->context, strName, exception);
        fy_strDestroy(context->context->memblocks, strName);
        FYEX(fy_free(strName); return NULL;);
        
        fy_strInitWithUTF8(context->context->memblocks, strName, fisceMethod->className, exception);
        FYEX(fy_free(strName); return NULL;);
        fisceMethod->method = fy_vmGetMethod(context->context, strName);
        if(fisceMethod->method == NULL){
            fy_fault(exception, FY_EXCEPTION_ABSTRACT, "Can't find field %s", fisceMethod->utf8Name);
        }
        fy_strDestroy(context->context->memblocks, strName);
        fy_free(strName);
        FYEH()NULL;
    }
    return fisceMethod->method;
}


fisce_int fiscePrepareThrowable(fisce_context *context, fisce_int threadId,
                                fisce_exception* toPrepare,
                                fisce_exception* exception){
    return fy_threadPrepareThrowable(context->context, context->context->threads[threadId], toPrepare, exception);
}

int fisceAllocate(fisce_context *context, fisce_class *clazz,
                  fisce_exception *exception){
    fy_class *clz = getFyClass(context, clazz, exception);
    FYEH()0;
    return fy_heapAllocate(context->context, clz, exception);
}

fisce_class* fisceGetClassOfObject(fisce_context *context, fisce_int handle,
                                   fisce_exception *exception){
    fy_class *clz;
    clz = fy_heapGetClassOfObject(context->context, handle, exception);
    FYEH()NULL;
    return fisceLookupClass(context, clz->utf8Name, exception);
}

int fisceAllocateArray(fisce_context *context, fisce_class *clazz, int length,
                       fisce_exception *exception){
    fy_class *clz = getFyClass(context, clazz, exception);
    FYEH()0;
    return fy_heapAllocateArray(context->context, clz, length, exception);
}

size_t fisceArrayLength(fisce_context *context, fisce_int handle,
                        fisce_exception *exception){
    return fy_heapArrayLength(context->context, handle, exception);
}

void fisceArrayCopy(fisce_context *context, fisce_int src, fisce_int srcPos, fisce_int dest,
                    fisce_int destPos, fisce_int len, fisce_exception *exception){
    return fy_heapArrayCopy(context->context, src, srcPos, dest, destPos, len, exception);
}

fisce_int fisceClone(fisce_context *context, fisce_int src, fisce_exception *exception){
    return fy_heapClone(context->context, src, exception);
}

size_t fisceStringLen(fisce_context *context, fisce_int handle, fisce_exception *exception){
    fy_str target[1];
    fisce_int i, max;
    size_t len = 0;
    target->content = NULL;
    fy_strInit(context->context->memblocks, target, 128, exception);
    FYEH()0;
    fy_heapGetString(context->context, handle, target, exception);
    FYEX(fy_strDestroy(context->context->memblocks, target); return 0);
    max = target->length;
    for(i = 0; i < max; i++){
        len += fy_utf8Size(fy_strGet(target, i));
    }
    fy_strDestroy(context->context->memblocks, target);
    return len;
}

char *fisceGetString(fisce_context *context, fisce_int handle, char *target,
                     size_t len, fisce_exception *exception){
    fy_str targetStr[1];
    
    fy_strInit(context->context->memblocks, targetStr, (fisce_int)len, exception);
    FYEH()NULL;
    fy_heapGetString(context->context, handle, targetStr, exception);
    FYEX(fy_strDestroy(context->context->memblocks, targetStr); return 0);
    fy_strSPrint(target, len, targetStr);
    fy_strDestroy(context->context->memblocks, targetStr);
    return target;
}

fisce_int fisceMakeString(fisce_context *context, const char *str,
                          fisce_exception *exception){
    fy_str targetStr[1];
    size_t len;
    fisce_int ret;
    len = strlen(str);
    targetStr->content = NULL;
    fy_strInit(context->context->memblocks, targetStr, (fisce_int)len, exception);
    ret = fy_heapMakeString(context->context, targetStr, exception);
    fy_strDestroy(context->context->memblocks, targetStr);
    return ret;
}

fisce_int fisceLiteral(fisce_context *context, const char *str,
                       fisce_exception *exception){
    fy_str targetStr[1];
    size_t len;
    fisce_int ret;
    len = strlen(str);
    targetStr->content = NULL;
    fy_strInit(context->context->memblocks, targetStr, (fisce_int)len, exception);
    ret = fy_heapLiteral(context->context, targetStr, exception);
    fy_strDestroy(context->context->memblocks, targetStr);
    return ret;
}

void fisceBeginProtect(fisce_context *context){
    fy_heapBeginProtect(context->context);
}

void fisceEndProtect(fisce_context *context){
    fy_heapEndProtect(context->context);
}

fisce_boolean fisceGetArrayBoolean(fisce_context *context, fisce_int handle,
                                   fisce_int index, fisce_exception *exception){
    return fy_heapGetArrayBoolean(context->context, handle, index, exception);
}
fisce_int fisceGetArrayHandle(fisce_context *context, fisce_int handle, fisce_int index,
                              fisce_exception *exception){
    return fy_heapGetArrayHandle(context->context, handle, index, exception);
}
fisce_byte fisceGetArrayByte(fisce_context *context, fisce_int handle, fisce_int index,
                             fisce_exception *exception){
    return fy_heapGetArrayByte(context->context, handle, index, exception);
}
fisce_byte *fisceGetArrayBytes(fisce_context *context, fisce_int handle,
                               fisce_exception *exception){
    return fy_heapGetArrayBytes(context->context, handle, exception);
}
fisce_short fisceGetArrayShort(fisce_context *context, fisce_int handle, fisce_int index,
                               fisce_exception *exception){
    return fy_heapGetArrayShort(context->context, handle, index, exception);
}
fisce_char fisceGetArrayChar(fisce_context *context, fisce_int handle, fisce_int index,
                             fisce_exception *exception){
    return fy_heapGetArrayChar(context->context, handle, index, exception);
}
fisce_int fisceGetArrayInt(fisce_context *context, fisce_int handle, fisce_int index,
                           fisce_exception *exception){
    return fy_heapGetArrayInt(context->context, handle, index, exception);
}
fisce_long fisceGetArrayLong(fisce_context *context, fisce_int handle, fisce_int index,
                             fisce_exception *exception){
    return fy_heapGetArrayLong(context->context, handle, index, exception);
}
fisce_float fisceGetArrayFloat(fisce_context *context, fisce_int handle, fisce_int index,
                               fisce_exception *exception){
    return fy_heapGetArrayFloat(context->context, handle, index, exception);
}
fisce_double fisceGetArrayDouble(fisce_context *context, fisce_int handle, fisce_int index,
                                 fisce_exception *exception){
    return fy_heapGetArrayDouble(context->context, handle, index, exception);
}


void fiscePutArrayBoolean(fisce_context *context, fisce_int handle, fisce_int index,
                          fisce_boolean value, fisce_exception *exception){
    fy_heapPutArrayBoolean(context->context, handle, index, value, exception);
}
void fiscePutArrayHandle(fisce_context *context, fisce_int handle, fisce_int index,
                         fisce_int value, fisce_exception *exception){
    fy_heapPutArrayHandle(context->context, handle, index, value, exception);
}
void fiscePutArrayByte(fisce_context *context, fisce_int handle, fisce_int index,
                       fisce_byte value, fisce_exception *exception){
    fy_heapPutArrayByte(context->context, handle, index, value, exception);
}
void fiscePutArrayShort(fisce_context *context, fisce_int handle, fisce_int index,
                        fisce_short value, fisce_exception *exception){
    fy_heapPutArrayShort(context->context, handle, index, value, exception);
}
void fiscePutArrayChar(fisce_context *context, fisce_int handle, fisce_int index,
                       fisce_char value, fisce_exception *exception){
    fy_heapPutArrayChar(context->context, handle, index, value, exception);
}
void fiscePutArrayInt(fisce_context *context, fisce_int handle, fisce_int index,
                      fisce_int value, fisce_exception *exception){
    fy_heapPutArrayInt(context->context, handle, index, value, exception);
}
void fiscePutArrayLong(fisce_context *context, fisce_int handle, fisce_int index,
                       fisce_long value, fisce_exception *exception){
    fy_heapPutArrayLong(context->context, handle, index, value, exception);
}
void fiscePutArrayFloat(fisce_context *context, fisce_int handle, fisce_int index,
                        fisce_float value, fisce_exception *exception){
    fy_heapPutArrayFloat(context->context, handle, index, value, exception);
}
void fiscePutArrayDouble(fisce_context *context, fisce_int handle, fisce_int index,
                         fisce_double value, fisce_exception *exception){
    fy_heapPutArrayDouble(context->context, handle, index, value, exception);
}

fisce_boolean fisceGetFieldBoolean(fisce_context *context, fisce_int handle,
                                   fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetFieldBoolean(context->context, handle, fld, exception);
}
fisce_int fisceGetFieldHandle(fisce_context *context, fisce_int handle,
                              fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetFieldHandle(context->context, handle, fld, exception);
}
fisce_byte fisceGetFieldByte(fisce_context *context, fisce_int handle,
                             fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetFieldByte(context->context, handle, fld, exception);
}
fisce_short fisceGetFieldShort(fisce_context *context, fisce_int handle,
                               fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetFieldShort(context->context, handle, fld, exception);
}
fisce_char fisceGetFieldChar(fisce_context *context, fisce_int handle,
                             fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetFieldChar(context->context, handle, fld, exception);
}
fisce_int fisceGetFieldInt(fisce_context *context, fisce_int handle, fisce_field *field,
                           fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetFieldInt(context->context, handle, fld, exception);
}
fisce_long fisceGetFieldLong(fisce_context *context, fisce_int handle,
                             fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetFieldLong(context->context, handle, fld, exception);
}
fisce_float fisceGetFieldFloat(fisce_context *context, fisce_int handle,
                               fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetFieldFloat(context->context, handle, fld, exception);
}
fisce_double fisceGetFieldDouble(fisce_context *context, fisce_int handle,
                                 fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetFieldDouble(context->context, handle, fld, exception);
}

void fiscePutFieldBoolean(fisce_context *context, fisce_int handle,
                          fisce_field *field, fisce_boolean value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutFieldBoolean(context->context, handle, fld, value, exception);
}
void fiscePutFieldHandle(fisce_context *context, fisce_int handle,
                         fisce_field *field, fisce_int value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutFieldHandle(context->context, handle, fld, value, exception);
}
void fiscePutFieldByte(fisce_context *context, fisce_int handle, fisce_field *field,
                       fisce_byte value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutFieldByte(context->context, handle, fld, value, exception);
}
void fiscePutFieldShort(fisce_context *context, fisce_int handle,
                        fisce_field *field, fisce_short value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutFieldShort(context->context, handle, fld, value, exception);
}
void fiscePutFieldChar(fisce_context *context, fisce_int handle, fisce_field *field,
                       fisce_char value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutFieldChar(context->context, handle, fld, value, exception);
}
void fiscePutFieldInt(fisce_context *context, fisce_int handle, fisce_field *field,
                      fisce_int value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutFieldInt(context->context, handle, fld, value, exception);
}
void fiscePutFieldLong(fisce_context *context, fisce_int handle, fisce_field *field,
                       fisce_long value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutFieldLong(context->context, handle, fld, value, exception);
}
void fiscePutFieldFloat(fisce_context *context, fisce_int handle,
                        fisce_field *field, fisce_float value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutFieldFloat(context->context, handle, fld, value, exception);
}
void fiscePutFieldDouble(fisce_context *context, fisce_int handle,
                         fisce_field *field, fisce_double value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutFieldDouble(context->context, handle, fld, value, exception);
}

fisce_boolean fisceGetStaticBoolean(fisce_context *context, 
                                    fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetStaticBoolean(context->context, fld, exception);
}
fisce_int fisceGetStaticHandle(fisce_context *context, 
                               fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetStaticHandle(context->context, fld, exception);
}
fisce_byte fisceGetStaticByte(fisce_context *context, 
                              fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetStaticByte(context->context, fld, exception);
}
fisce_short fisceGetStaticShort(fisce_context *context, 
                                fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetStaticShort(context->context, fld, exception);
}
fisce_char fisceGetStaticChar(fisce_context *context, 
                              fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetStaticChar(context->context, fld, exception);
}
fisce_int fisceGetStaticInt(fisce_context *context,  fisce_field *field,
                            fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetStaticInt(context->context, fld, exception);
}
fisce_long fisceGetStaticLong(fisce_context *context, 
                              fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetStaticLong(context->context, fld, exception);
}
fisce_float fisceGetStaticFloat(fisce_context *context, 
                                fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetStaticFloat(context->context, fld, exception);
}
fisce_double fisceGetStaticDouble(fisce_context *context, 
                                  fisce_field *field, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH()0;
    return fy_heapGetStaticDouble(context->context, fld, exception);
}

void fiscePutStaticBoolean(fisce_context *context, 
                           fisce_field *field, fisce_boolean value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutStaticBoolean(context->context, fld, value, exception);
}
void fiscePutStaticHandle(fisce_context *context, 
                          fisce_field *field, fisce_int value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutStaticHandle(context->context, fld, value, exception);
}
void fiscePutStaticByte(fisce_context *context,  fisce_field *field,
                        fisce_byte value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutStaticByte(context->context, fld, value, exception);
}
void fiscePutStaticShort(fisce_context *context, 
                         fisce_field *field, fisce_short value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutStaticShort(context->context, fld, value, exception);
}
void fiscePutStaticChar(fisce_context *context,  fisce_field *field,
                        fisce_char value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutStaticChar(context->context, fld, value, exception);
}
void fiscePutStaticInt(fisce_context *context,  fisce_field *field,
                       fisce_int value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutStaticInt(context->context, fld, value, exception);
}
void fiscePutStaticLong(fisce_context *context,  fisce_field *field,
                        fisce_long value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutStaticLong(context->context, fld, value, exception);
}
void fiscePutStaticFloat(fisce_context *context, 
                         fisce_field *field, fisce_float value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutStaticFloat(context->context, fld, value, exception);
}
void fiscePutStaticDouble(fisce_context *context, 
                          fisce_field *field, fisce_double value, fisce_exception *exception){
    fy_field *fld = getFyField(context, field, exception);
    FYEH();
    fy_heapPutStaticDouble(context->context, fld, value, exception);
}

void fisceRegisterNativeHandler(fisce_context *context, const char *name,
                                void *data, fisce_nhf handler, fisce_exception *exception){
    struct nativeData *ndata = fy_mmAllocate(context->context->memblocks, sizeof(struct nativeData), exception);
    ndata->ctx = context;
    ndata->fun = handler;
    ndata->data = data;
    
    fy_vmRegisterNativeHandler(context->context, name, ndata, nativeDelegator, exception);
}
void fisceUnRegisterNativeHandler(fisce_context *context, const char *name,
                                  fisce_exception *exception){
    fy_vmUnRegisterNativeHandler(context->context, name, exception);
}

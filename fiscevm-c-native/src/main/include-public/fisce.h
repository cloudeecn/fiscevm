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

#ifndef MAIN_INCLUDE_PUBLIC_FISCE_H_
#define MAIN_INCLUDE_PUBLIC_FISCE_H_

#include<stdio.h>
#include "fiscepub.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#if defined(FISCE_DLL_EXPORT)
# define FISCE_ATTR_EXPORT __declspec(dllexport)
#elif defined(FISCE_DLL_IMPORT)
# define FISCE_ATTR_EXPORT __declspec(dllimport)
#else
# define FISCE_ATTR_EXPORT
#endif
    
    typedef struct fisce_context fisce_context;
    typedef struct fisce_class fisce_class;
    typedef struct fisce_method fisce_method;
    typedef struct fisce_field fisce_field;
    
    typedef void (*fisce_nhf)(fisce_context *context, fisce_int threadId, void *data,
                                   fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
                                   fisce_exception *exception);
    
    size_t sizeofContext();
    
    void fisceInitContext(fisce_context *context, fisce_exception *exception);
    void fisceDestroyContext(fisce_context *context);
    void fisceBootFromMain(fisce_context *context, const char *name,
                           fisce_exception *exception);
    void fisceBootFromDataFile(fisce_context *context, fisce_exception *exception);
    void fisceSaveToFile(fisce_context *context, fisce_exception *exception);
    void fisceRun(fisce_context *context, fisce_message *message,
                  fisce_exception *exception);
    void fisceSetGCExtra(fisce_context *context, void *data, void (*beforeHandler)(void* data),
                         void (*getExtraHandler)(void* data, fisce_int* size, fisce_int **phandles), void (*afterHandler)(void* data));
    
    
    void fisceReturnInt(fisce_context *context, fisce_int threadId, fisce_int value);
    void fisceReturnHandle(fisce_context *context, fisce_int threadId, fisce_int value);
    void fisceReturnLong(fisce_context *context, fisce_int threadId, fisce_long value);
    void fisceReturnFloat(fisce_context *context, fisce_int threadId, fisce_long value);
    void fisceReturnDouble(fisce_context *context, fisce_int threadId, fisce_long value);
    
    fisce_class *fisceLookupClass(fisce_context *context, const char *name,
                                  fisce_exception *exception);
    const char *fisceClassName(fisce_context *context, fisce_class *clazz);
    fisce_boolean fisceClassCanCastTo(fisce_context *context, fisce_class *from,
                                      fisce_class *to, fisce_exception *exception);
    void fisceReleaseClass(fisce_class *clazz);
    
    fisce_field *fisceGetField(fisce_context *context, const char *name, fisce_exception *exception);
    fisce_field *fisceLookupFieldVirtual(fisce_context *context, fisce_class *clazz,
                                         const char *name, fisce_exception *exception);
    const char *fisceFieldName(fisce_context *context, fisce_field *field);
    void fisceReleaseField(fisce_field *field);
    
    fisce_method *fisceGetMethod(fisce_context *context, const char *name, fisce_exception *exception);
    fisce_method *fisceLookupMethodVirtual(fisce_context *context,
                                           fisce_class *clazz, const char *name, fisce_exception *exception);
    const char *fisceMethodName(fisce_context *context, fisce_method *method);
    void fisceReleaseMethod(fisce_field *field);
    
    fisce_int fiscePrepareThrowable(fisce_context *context, fisce_int threadId,
                                    fisce_exception* toPrepare,
                                    fisce_exception* exception);
    void fisceThrow(fisce_context *context, fisce_int threadId, fisce_int handle);
    
    int fisceAllocate(fisce_context *context, fisce_class *clazz,
                      fisce_exception *exception);
    fisce_class* fisceGetClassOfObject(fisce_context *context, fisce_int handle,
                                       fisce_exception *exception);
    
    int fisceAllocateArray(fisce_context *context, fisce_class *clazz, int length,
                           fisce_exception *exception);
    size_t fisceArrayLength(fisce_context *context, fisce_int handle,
                            fisce_exception *exception);
    
    void fisceArrayCopy(fisce_context *context, fisce_int src, fisce_int srcPos, fisce_int dest,
                        fisce_int destPos, fisce_int len, fisce_exception *exception);
    fisce_int fisceClone(fisce_context *context, fisce_int src, fisce_exception *exception);
    size_t fisceStringLen(fisce_context *context, fisce_int handle, fisce_exception *exception);
    char *fisceGetString(fisce_context *context, fisce_int handle, char *target,
                         size_t len, fisce_exception *exception);
    fisce_int fisceMakeString(fisce_context *context, const char *str,
                              fisce_exception *exception);
    fisce_int fisceLiteral(fisce_context *context, const char *str,
                           fisce_exception *exception);
    
    void fisceBeginProtect(fisce_context *context);
    void fisceEndProtect(fisce_context *context);
    
    fisce_boolean fisceGetArrayBoolean(fisce_context *context, fisce_int handle,
                                       fisce_int index, fisce_exception *exception);
    fisce_int fisceGetArrayHandle(fisce_context *context, fisce_int handle, fisce_int index,
                                  fisce_exception *exception);
    fisce_byte fisceGetArrayByte(fisce_context *context, fisce_int handle, fisce_int index,
                                 fisce_exception *exception);
    fisce_byte *fisceGetArrayBytes(fisce_context *context, fisce_int handle,
                                   fisce_exception *exception);
    fisce_short fisceGetArrayShort(fisce_context *context, fisce_int handle, fisce_int index,
                                   fisce_exception *exception);
    fisce_char fisceGetArrayChar(fisce_context *context, fisce_int handle, fisce_int index,
                                 fisce_exception *exception);
    fisce_int fisceGetArrayInt(fisce_context *context, fisce_int handle, fisce_int index,
                               fisce_exception *exception);
    fisce_long fisceGetArrayLong(fisce_context *context, fisce_int handle, fisce_int index,
                                 fisce_exception *exception);
    fisce_float fisceGetArrayFloat(fisce_context *context, fisce_int handle, fisce_int index,
                                   fisce_exception *exception);
    fisce_double fisceGetArrayDouble(fisce_context *context, fisce_int handle, fisce_int index,
                                     fisce_exception *exception);
    
    void fiscePutArrayBoolean(fisce_context *context, fisce_int handle, fisce_int index,
                              fisce_boolean value, fisce_exception *exception);
    void fiscePutArrayHandle(fisce_context *context, fisce_int handle, fisce_int index,
                             fisce_int value, fisce_exception *exception);
    void fiscePutArrayByte(fisce_context *context, fisce_int handle, fisce_int index,
                           fisce_byte value, fisce_exception *exception);
    void fiscePutArrayShort(fisce_context *context, fisce_int handle, fisce_int index,
                            fisce_short value, fisce_exception *exception);
    void fiscePutArrayChar(fisce_context *context, fisce_int handle, fisce_int index,
                           fisce_char value, fisce_exception *exception);
    void fiscePutArrayInt(fisce_context *context, fisce_int handle, fisce_int index,
                          fisce_int value, fisce_exception *exception);
    void fiscePutArrayLong(fisce_context *context, fisce_int handle, fisce_int index,
                           fisce_long value, fisce_exception *exception);
    void fiscePutArrayFloat(fisce_context *context, fisce_int handle, fisce_int index,
                            fisce_float value, fisce_exception *exception);
    void fiscePutArrayDouble(fisce_context *context, fisce_int handle, fisce_int index,
                             fisce_double value, fisce_exception *exception);
    
    fisce_boolean fisceGetFieldBoolean(fisce_context *context, fisce_int handle,
                                       fisce_field *field, fisce_exception *exception);
    fisce_int fisceGetFieldHandle(fisce_context *context, fisce_int handle,
                                  fisce_field *field, fisce_exception *exception);
    fisce_byte fisceGetFieldByte(fisce_context *context, fisce_int handle,
                                 fisce_field *field, fisce_exception *exception);
    fisce_short fisceGetFieldShort(fisce_context *context, fisce_int handle,
                                   fisce_field *field, fisce_exception *exception);
    fisce_char fisceGetFieldChar(fisce_context *context, fisce_int handle,
                                 fisce_field *field, fisce_exception *exception);
    fisce_int fisceGetFieldInt(fisce_context *context, fisce_int handle, fisce_field *field,
                               fisce_exception *exception);
    fisce_long fisceGetFieldLong(fisce_context *context, fisce_int handle,
                                 fisce_field *field, fisce_exception *exception);
    fisce_float fisceGetFieldFloat(fisce_context *context, fisce_int handle,
                                   fisce_field *field, fisce_exception *exception);
    fisce_double fisceGetFieldDouble(fisce_context *context, fisce_int handle,
                                     fisce_field *field, fisce_exception *exception);
    
    void fiscePutFieldBoolean(fisce_context *context, fisce_int handle,
                              fisce_field *field, fisce_boolean value, fisce_exception *exception);
    void fiscePutFieldHandle(fisce_context *context, fisce_int handle,
                             fisce_field *field, fisce_int value, fisce_exception *exception);
    void fiscePutFieldByte(fisce_context *context, fisce_int handle, fisce_field *field,
                           fisce_byte value, fisce_exception *exception);
    void fiscePutFieldShort(fisce_context *context, fisce_int handle,
                            fisce_field *field, fisce_short value, fisce_exception *exception);
    void fiscePutFieldChar(fisce_context *context, fisce_int handle, fisce_field *field,
                           fisce_char value, fisce_exception *exception);
    void fiscePutFieldInt(fisce_context *context, fisce_int handle, fisce_field *field,
                          fisce_int value, fisce_exception *exception);
    void fiscePutFieldLong(fisce_context *context, fisce_int handle, fisce_field *field,
                           fisce_long value, fisce_exception *exception);
    void fiscePutFieldFloat(fisce_context *context, fisce_int handle,
                            fisce_field *field, fisce_float value, fisce_exception *exception);
    void fiscePutFieldDouble(fisce_context *context, fisce_int handle,
                             fisce_field *field, fisce_double value, fisce_exception *exception);
    
    fisce_boolean fisceGetStaticBoolean(fisce_context *context, fisce_field *field,
                                        fisce_exception *exception);
    fisce_int fisceGetStaticHandle(fisce_context *context, fisce_field *field,
                                   fisce_exception *exception);
    fisce_byte fisceGetStaticByte(fisce_context *context, fisce_field *field,
                                  fisce_exception *exception);
    fisce_short fisceGetStaticShort(fisce_context *context, fisce_field *field,
                                    fisce_exception *exception);
    fisce_char fisceGetStaticChar(fisce_context *context, fisce_field *field,
                                  fisce_exception *exception);
    fisce_int fisceGetStaticInt(fisce_context *context, fisce_field *field,
                                fisce_exception *exception);
    fisce_long fisceGetStaticLong(fisce_context *context, fisce_field *field,
                                  fisce_exception *exception);
    fisce_float fisceGetStaticFloat(fisce_context *context, fisce_field *field,
                                    fisce_exception *exception);
    fisce_double fisceGetStaticDouble(fisce_context *context, fisce_field *field,
                                      fisce_exception *exception);
    
    void fiscePutStaticBoolean(fisce_context *context, fisce_field *field,
                               fisce_boolean value, fisce_exception *exception);
    void fiscePutStaticHandle(fisce_context *context, fisce_field *field,
                              fisce_int value, fisce_exception *exception);
    void fiscePutStaticByte(fisce_context *context, fisce_field *field,
                            fisce_byte value, fisce_exception *exception);
    void fiscePutStaticShort(fisce_context *context, fisce_field *field,
                             fisce_short value, fisce_exception *exception);
    void fiscePutStaticChar(fisce_context *context, fisce_field *field,
                            fisce_char value, fisce_exception *exception);
    void fiscePutStaticInt(fisce_context *context, fisce_field *field, fisce_int value,
                           fisce_exception *exception);
    void fiscePutStaticLong(fisce_context *context, fisce_field *field,
                            fisce_long value, fisce_exception *exception);
    void fiscePutStaticFloat(fisce_context *context, fisce_field *field,
                             fisce_float value, fisce_exception *exception);
    void fiscePutStaticDouble(fisce_context *context, fisce_field *field,
                              fisce_double value, fisce_exception *exception);
    
    void fisceRegisterNativeHandler(fisce_context *context, const char *name,
                                    void *data, fisce_nhf handler, fisce_exception *exception);
    void fisceUnRegisterNativeHandler(fisce_context *context, const char *name,
                                      fisce_exception *exception);
    
#ifdef __cplusplus
}
#endif

#endif /* MAIN_INCLUDE_PUBLIC_FISCE_H_ */

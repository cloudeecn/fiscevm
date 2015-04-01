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

#ifndef fisce_fiscepub_h
#define fisce_fiscepub_h

typedef float fisce_float;
typedef double fisce_double;

#if __STDC_VERSION__ >= 199901L
# include<inttypes.h>
typedef int8_t fisce_byte;
typedef uint8_t fisce_ubyte;
typedef int16_t fisce_short;
typedef uint16_t fisce_char;
typedef int32_t fisce_int;
typedef uint32_t fisce_uint;
typedef int64_t fisce_long;
typedef uint64_t fisce_ulong;
#else
typedef signed char fisce_byte;
typedef unsigned char fisce_ubyte;
typedef signed short fisce_short;
typedef unsigned short fisce_char;
typedef signed int fisce_int;
typedef unsigned int fisce_uint;
# ifdef _MSC_VER
typedef unsigned __int64 fisce_ulong;
typedef __int64 fisce_long;
# elif defined(CUSTOM_LONG_LONG)
typedef unsigned CUSTOM_LONG_LONG fisce_ulong;
typedef CUSTOM_LONG_LONG fisce_long;
# else
typedef unsigned long long fisce_ulong;
typedef long long fisce_long;
# endif
#endif
typedef fisce_int fisce_boolean;


typedef union fisce_stack_item {
    fisce_uint uvalue;
    fisce_int ivalue;
    fisce_float fvalue;
} fisce_stack_item;


typedef struct fisce_nativeCall {
    char *methodName;
    fisce_stack_item *params;
    fisce_uint paramCount;
} fisce_nativeCall;

typedef struct fisce_exception {
    enum exceptionType {
        exception_none = 0, exception_normal
        /*, exception_critical // use futl_fault instead!*/
    } exceptionType;
    char exceptionName[256];
    char exceptionDesc[256];
} fisce_exception;

typedef enum fisce_messageType {
    /*message_continue = 0, In thread*/
    message_none = 1, /*Thread Only*/
    message_thread_dead = 2, /*Thread Only*/
    message_invoke_native = 3,/*Thread And TM pass thread*/
    /*message_exception = 4, Thread And TM pass thread*/
    message_sleep = 5, /*TM Only*/
    message_vm_dead = 6
    /*TM Only*/
}fisce_messageType;

typedef struct fisce_message {
    fisce_messageType messageType;
    fisce_int threadId;
    /*We care more about stability than some hundreds bytes of memory*/
    struct {
        fisce_nativeCall call;
        fisce_long sleepTime;
    }body;
    
} fisce_message;

#endif

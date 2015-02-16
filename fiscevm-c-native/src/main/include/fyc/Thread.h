/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of fiscevm
 *
 *fiscevmis free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 *fiscevmis distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with fiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FY_THREAD_H_
#define FY_THREAD_H_

#include "fy_util/Portable.h"
#include "fyc/ClassStruct.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct fy_nh {
	void *data;
	fy_nhFunction handler;
}fy_nh;

typedef union fy_stack_item {
	fy_uint uvalue;
	fy_int ivalue;
	fy_float fvalue;
}fy_stack_item;

typedef struct fy_frame {
	fy_method *method;
	fy_stack_item *baseSpp;
#ifdef FY_STRICT_CHECK
	fy_uint size;
	fy_uint localCount;
	fy_uint codeSize;
#endif
	fy_uint lpc;
	fy_uint pcofs;
}fy_frame;

#define FY_FRAME_ENTRIES ((sizeof(struct fy_frame)+3)/4)
#define FY_GET_FRAME(THREAD,FRAMEID) ((fy_frame*)((THREAD)->stack+STACK_SIZE)-((FRAMEID)+1))

typedef union fy_stack_wide_item {
	struct {
		fy_stack_item lower_item;
		fy_stack_item higher_item;
	}fy_stack_items;
	fy_double dvalue;
	fy_long lvalue;
}fy_stack_wide_item;

typedef struct fy_nativeCall {
	fy_method *method;
	fy_uint paramCount;
	fy_stack_item *params;
}fy_nativeCall;

typedef enum fy_messageType {
	/*message_continue = 0, In thread*/
	message_none = 1, /*Thread Only*/
	message_thread_dead = 2, /*Thread Only*/
	message_invoke_native = 3,/*Thread And TM pass thread*/
	/*message_exception = 4, Thread And TM pass thread*/
	message_sleep = 5, /*TM Only*/
	message_vm_dead = 6
	/*TM Only*/
}fy_messageType;

typedef struct fy_message {
	fy_messageType messageType;
	fy_thread *thread;
	/*We care more about stability than some hundreds bytes of memory*/
	struct {
		fy_nativeCall call;
		fy_long sleepTime;
	}body;

}fy_message;

typedef struct fy_thread {

	fy_uint handle;
	fy_uint currentThrowable;
	fy_int status;
	fy_int priority;
	fy_int threadId;

	fy_uint frameCount;
	fy_stack_item stack[STACK_SIZE];

	/*Used by thread manager*/
	fy_int waitForLockId;
	fy_int waitForNotifyId;
	fy_int pendingLockCount;
	fy_long nextWakeTime;
	fy_boolean interrupted;
	fy_boolean daemon;
	fy_boolean destroyPending;

	fy_nativeCall pendingNative;
}fy_thread;

void fy_threadSetCurrentThrowable(fy_context *context, fy_thread *thread,
		fy_int handle, fy_exception *exception);

fy_int fy_threadMonitorEnter(fy_context *context, fy_thread *thread,
		fy_int handle, fy_int ops);

fy_int fy_threadMonitorExit(fy_context *context, fy_thread *thread,
		fy_int handle, fy_int ops, fy_exception *exception);

void fy_threadDestroy(fy_context *context, fy_thread *thread);

fy_method *fy_threadGetCurrentMethod(fy_context *context, fy_thread *thread);

void fy_threadInitWithRun(fy_context *context, fy_thread *thread, int handle,
		fy_exception *exception);

void fy_threadInitWithMethod(fy_context *context, fy_thread *thread,
		int threadHandle, fy_method *method, fy_exception *exception);

fy_frame *fy_threadCurrentFrame(fy_context *context, fy_thread *thread);

fy_int fy_threadPushMethod(fy_context *context, fy_thread *thread,
		fy_method *invoke, fy_stack_item *spp, fy_int ops, fy_exception *exception);

fy_int fy_threadInvoke(fy_context *context, fy_thread *thread,
		fy_method *method, fy_stack_item *spp, fy_int ops, fy_exception *exception);

fy_int fy_threadClinit(fy_context *context, fy_thread *thread, fy_class *clazz,
		fy_stack_item *spp, fy_int ops, fy_exception *exception);

void fy_threadRun(fy_context *context, fy_thread *thread, fy_message *message,
		fy_int ops, fy_exception *exception);

void fy_threadFillException(fy_context *context, fy_thread *thread,
		fy_uint handle, fy_exception *exception);

fy_uint fy_threadPrepareThrowable(fy_context *context, fy_thread *thread,
		FY_ATTR_RESTRICT fy_exception *toPrepare,
		FY_ATTR_RESTRICT fy_exception *exception);


void fy_threadReturnInt(fy_stack_item *spp, fy_int value);

void fy_threadReturnLong(fy_stack_item *spp, fy_long value);

void fy_threadScanRef(fy_context *context, fy_thread *thread,
		fy_arrayList *from, fy_exception *exception);

#define fy_threadReturnFloat(SPP,V) fy_threadReturnInt(SPP,fy_floatToInt(V))

#define fy_threadReturnDouble(SPP,V) fy_threadReturnLong(SPP,fy_doubleToLong(V))

fy_frame *fy_threadPushFrame(fy_context *context, fy_thread *thread,
		fy_method *invoke, fy_stack_item *spp, fy_exception *exception);
fy_frame *fy_threadPopFrame(fy_context *context, fy_thread *thread);

fy_int fy_threadInvokeSpecial(fy_context *context, fy_thread *thread,
		fy_frame *frame, fy_method *method, fy_stack_item *spp, fy_int ops,
		fy_exception *exception);
fy_int fy_threadInvokeVirtual(fy_context *context, fy_thread *thread,
		fy_frame *frame, fy_method *method, fy_stack_item *spp, fy_int ops,
		fy_exception *exception);
fy_int fy_threadInvokeStatic(fy_context *context, fy_thread *thread,
		fy_frame *frame, fy_method *method, fy_stack_item *spp, fy_int ops,
		fy_exception *exception);

#ifdef	__cplusplus
}
#endif
#endif /* FY_HEAP_H_ */

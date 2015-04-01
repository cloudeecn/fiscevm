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

#ifndef FY_THREAD_H_
#define FY_THREAD_H_

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fyc/ClassStruct.h"
#include "fyc/VMContext.h"
#include "fyc/typedefs.h"

#ifdef	__cplusplus
extern "C" {
#endif

struct fy_frame {
	fy_method *method;
	fisce_stack_item *baseSpp;
#ifdef FY_STRICT_CHECK
	fisce_uint size;
	fisce_uint localCount;
	fisce_uint codeSize;
#endif
	fisce_uint lpc;
	fisce_int pcofs;
};

#define FY_FRAME_ENTRIES ((sizeof(struct fy_frame)+3)/4)
#define FY_GET_FRAME(THREAD,FRAMEID) ((fy_frame*)((THREAD)->stack+STACK_SIZE)-((FRAMEID)+1))

struct fy_thread {

	fisce_uint handle;
	fisce_uint currentThrowable;
	fisce_int status;
	fisce_int priority;
	fisce_int threadId;

	fisce_uint frameCount;
	fisce_stack_item stack[STACK_SIZE];

	/*Used by thread manager*/
	fisce_int waitForLockId;
	fisce_int waitForNotifyId;
	fisce_int pendingLockCount;
	fisce_long nextWakeTime;
	fisce_boolean interrupted;
	fisce_boolean daemon;
	fisce_boolean destroyPending;

	fisce_nativeCall pendingNative;
};

void fy_threadSetCurrentThrowable(fy_context *context, fy_thread *thread,
		fisce_int handle, fisce_exception *exception);

fisce_int fy_threadMonitorEnter(fy_context *context, fy_thread *thread,
		fisce_int handle, fisce_int ops);

void fy_threadMonitorExit(fy_context *context, fy_thread *thread,
		fisce_int handle, fisce_exception *exception);

void fy_threadDestroy(fy_context *context, fy_thread *thread);

fy_method *fy_threadGetCurrentMethod(fy_context *context, fy_thread *thread);

void fy_threadInitWithRun(fy_context *context, fy_thread *thread, int handle,
		fisce_exception *exception);

void fy_threadInitWithMethod(fy_context *context, fy_thread *thread,
		int threadHandle, fy_method *method, fisce_exception *exception);

fy_frame *fy_threadCurrentFrame(fy_context *context, fy_thread *thread);

fy_class *fy_threadCheckClinit(fy_context *context, fy_thread *thread, fy_class *clazz);

fisce_int fy_threadPushMethod(fy_context *context, fy_thread *thread,
		fy_method *invoke, fisce_stack_item *spp, fisce_int ops, fisce_exception *exception);

fisce_int fy_threadInvoke(fy_context *context, fy_thread *thread,
		fy_method *method, fisce_stack_item *spp, fisce_int ops, fisce_exception *exception);

fisce_int fy_threadClinit(fy_context *context, fy_thread *thread, fy_class *clazz,
		fisce_stack_item *spp, fisce_int ops, fisce_exception *exception);

void fy_threadRun(fy_context *context, fy_thread *thread, fisce_message *message,
		fisce_int ops, fisce_exception *exception);

void fy_threadFillException(fy_context *context, fy_thread *thread,
		fisce_uint handle, fisce_exception *exception);

fisce_uint fy_threadPrepareThrowable(fy_context *context, fy_thread *thread,
        fisce_exception* toPrepare,
        fisce_exception* exception);


void fy_threadReturnInt(fisce_stack_item *spp, fisce_int value);

void fy_threadReturnLong(fisce_stack_item *spp, fisce_long value);

void fy_threadScanRef(fy_context *context, fy_thread *thread,
		fy_arrayList *from, fisce_exception *exception);

#define fy_threadReturnFloat(SPP,V) fy_threadReturnInt(SPP,fisce_floatToInt(V))

#define fy_threadReturnDouble(SPP,V) fy_threadReturnLong(SPP,fisce_doubleToLong(V))

fy_frame *fy_threadPushFrame(fy_context *context, fy_thread *thread,
		fy_method *invoke, fisce_stack_item *spp, fisce_exception *exception);
fy_frame *fy_threadPopFrame(fy_context *context, fy_thread *thread);

fisce_int fy_threadInvokeSpecial(fy_context *context, fy_thread *thread,
		fy_frame *frame, fy_method *method, fisce_stack_item *spp, fisce_int ops,
		fisce_exception *exception);
fisce_int fy_threadInvokeVirtual(fy_context *context, fy_thread *thread,
		fy_frame *frame, fy_method *method, fisce_stack_item *spp, fisce_int ops,
		fisce_exception *exception);
fisce_int fy_threadInvokeStatic(fy_context *context, fy_thread *thread,
		fy_frame *frame, fy_method *method, fisce_stack_item *spp, fisce_int ops,
		fisce_exception *exception);

#ifdef	__cplusplus
}
#endif
#endif /* FY_HEAP_H_ */

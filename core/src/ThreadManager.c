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

#include "fyc/ThreadManager.h"

static fy_thread *getThreadByHandle(fy_context *context, fy_uint targetHandle,
		fy_exception *exception) {
	//TODO
	fy_object *obj = context->objects + targetHandle;
	fy_uint threadId = obj->attachedId;
	ASSERT(threadId>0 && threadId<MAX_THREADS);
	return context->threads + obj->attachedId;
}

static fy_int monitorEnter(fy_context *context, fy_thread *thread,
		fy_uint monitorId, fy_int times) {
	fy_object *monitor = context->objects + monitorId;
	fy_uint owner = monitor->monitorOwnerId;
	fy_uint threadId = thread->threadId;
	if (owner == threadId) {
		monitor->monitorOwnerTimes += times;
	} else if (owner <= 0) {
		monitor->monitorOwnerId = threadId;
		monitor->monitorOwnerTimes = times;
	} else {
		ASSERT(thread->waitForLockId==0);
		thread->waitForLockId = monitorId;
		thread->pendingLockCount = 1;
		thread->yield = TRUE;
	}
	return times;
}

static fy_int monitorExit(fy_context *context, fy_thread *thread,
		fy_uint monitorId, fy_int times, fy_exception *exception) {
	fy_uint threadId = thread->threadId;
	fy_object *monitor = context->objects + monitorId;
	fy_uint owner = monitor->monitorOwnerId;
	if (owner != threadId) {
		fy_fault(exception, FY_EXCEPTION_MONITOR, "");
		return 0;
	}
	monitor->monitorOwnerTimes -= times;
	if (monitor->monitorOwnerTimes == 0) {
		monitor->monitorOwnerId = 0;
		thread->yield = TRUE;
	} else if (monitor->monitorOwnerTimes < 0) {
		fy_fault(exception, NULL, "Too many monitors released!");
		return 0;
	}
	return times;
}

static fy_int releaseMonitor(fy_context *context, fy_thread *thread,
		fy_uint monitorId, fy_exception *exception) {
	fy_uint threadId = thread->threadId;
	fy_object *monitor = context->objects + monitorId;
	fy_uint owner = monitor->monitorOwnerId;
	if (owner != threadId) {
		fy_fault(exception, FY_EXCEPTION_MONITOR, "");
		return 0;
	}
	return monitorExit(context, thread, monitorId, exception);
}

static fy_uint fetchNextThreadId(fy_context *context, fy_exception *exception) {
	fy_int h = context->nextThreadId;
	fy_thread *target;
	while ((target = context->threads + h)->inUse) {
		h++;
		if (h == MAX_THREADS) {
			h = 1;
		}
		if (h == context->nextThreadId) {
			fy_fault(exception, NULL, "Threads used up!");
		}
	}
	context->nextThreadId = (h % (MAX_THREADS - 1)) + 1;
	return h;
}

static void cleanupThread(fy_context *context, fy_thread *thread) {
	fy_uint handle;
	fy_object *obj;
	thread->inUse = FALSE;
	thread->waitForLockId = 0;
	thread->waitForNotifyId = 0;
	thread->nextWakeTime = 0;
	thread->pendingLockCount = 0;
	thread->destroyPending = FALSE;
	for (handle = 1; handle < MAX_OBJECTS; handle++) {
		obj = context->objects + handle;
		if (obj->monitorOwnerId == thread->threadId) {
			obj->monitorOwnerId = 0;
			obj->monitorOwnerTimes = 0;
		}
	}
}

void fy_tmSleep(fy_context *context, fy_thread *thread, fy_long time) {
	thread->nextWakeTime = fy_portTimeMillSec(context->port) + time;
}

void fy_tmInterrupt(fy_context *context, fy_uint targetHandle,
		fy_exception *exception) {
	fy_thread *target;
	fy_uint exceptionHandle;
	fy_exception targetException;
	targetException.exceptionType = exception_none;
	sprintf_s(targetException.exceptionName,
			sizeof(targetException.exceptionName),
			"java/lang/InterruptedException");
	targetException.exceptionDesc[0] = 0;

	target = getThreadByHandle(targetHandle, exception);
	if (exception->exceptionType != exception_none) {
		return;
	}
	if (target == NULL) {
		return;
	}
	if (target->nextWakeTime > 0) {
		ASSERT(target->currentThrowable == 0);
		exceptionHandle = fy_threadPrepareThrowable(context, target,
				&targetException, exception);
		if (exception->exceptionType != exception_none) {
			return;;
		}
		target->currentThrowable = exceptionHandle;
		target->nextWakeTime = 0;
		target->interrupted = TRUE;
	}
}

fy_boolean fy_tmIsInterrupted(fy_context *context, fy_uint targetHandle,
		fy_boolean clear, fy_exception *exception) {
	fy_thread *target;
	fy_boolean ret;

	target = getThreadByHandle(targetHandle, exception);
	if (exception->exceptionType != exception_none) {
		return FALSE;
	}
	ret = target->interrupted;
	if (clear) {
		target->interrupted = FALSE;
	}
	return ret;
}

void fy_tmWait(fy_context *context, fy_thread *thread, fy_int monitorId,
		fy_long time, fy_exception *exception) {
	fy_object *monitor;

	ASSERT( thread->waitForNotifyId == 0);
	monitor = context->objects + monitorId;
	ASSERT(monitor->clazz!=NULL);
	if (monitor->monitorOwnerId != thread->threadId) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/IllegalMonitorStateException");
		exception->exceptionDesc[0] = 0;
		return;
	}
	thread->waitForNotifyId = monitorId;
	thread->pendingLockCount = releaseMonitor(context, thread, monitorId,
			exception);
	if (exception->exceptionType != exception_none) {
		return;
	}
	if (time <= 0) {
		thread->nextWakeTime = 0x7FFFFFFFFFFFFFFFLL;
	} else {
		thread->nextWakeTime = fy_portTimeMillSec(context->port) + time;
	}
	thread->yield = TRUE;
}

void fy_tmNotify(fy_context *context, fy_thread *thread, fy_int monitorId,
		fy_boolean all, fy_exception *exception) {
	fy_object *monitor;
	fy_thread * target;
	int i;

	ASSERT(thread->waitForNotifyId == 0);
	monitor = context->objects + monitorId;
	ASSERT(monitor->clazz!=NULL);
	if (monitor->monitorOwnerId != thread->threadId) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/IllegalMonitorStateException");
		exception->exceptionDesc[0] = 0;
		return;
	}
	for (i = 1; i < MAX_THREADS; i++) {
		target = context->threads + i;
		if (target->waitForNotifyId == monitorId) {
			target->waitForNotifyId = 0;
			ASSERT(target->waitForLockId==0);
			target->waitForLockId = monitorId;
			target->nextWakeTime = 0;
			if (!all) {
				break;
			}
		}
	}
}

fy_boolean fy_tmIsAlive(fy_context *context, fy_uint threadHandle,
		fy_exception *exception) {
	fy_thread *target;

	target = getThreadByHandle(threadHandle, exception);
	return target != NULL;
}

void fy_tmDestroyThread(fy_thread *thread) {
	thread->destroyPending = TRUE;
}

void fy_tmBootFromMain(fy_context *context, fy_class *clazz,
		fy_exception *exception) {
	fy_method *method;
	fy_uint threadId;
	fy_thread *thread;
	fy_class *threadClass;
	fy_uint threadHandle;
	fy_field *threadNameField;
	fy_field *threadPriorityField;


	if (context->status != FY_TM_STATE_NEW) {
		fy_fault(exception, NULL, "Status is not new while booting! %d",
				context->status);
		return;
	}
	method = fy_vmLookupMethodStatic(context, clazz, context->sFMain,
			exception);
	fy_exceptionCheckAndReturn(exception);

	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_NO_METHOD, "Main method not found!");
		return;
	}

	threadId = fetchNextThreadId(context, exception);
	fy_exceptionCheckAndReturn(exception);

	thread=context->threads+threadId;
	threadClass=fy_vmLookupClass(context,context->sThread,exception);
	fy_exceptionCheckAndReturn(exception);

	threadHandle=fy_heapAllocate(context,threadClass,exception);
	fy_exceptionCheckAndReturn(exception);

	threadNameField=fy_vm
}

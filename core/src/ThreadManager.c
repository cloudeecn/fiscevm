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

static fy_thread *getThreadByHandle(fy_uint targetHandle,
		fy_exception *exception) {
	//TODO
	return NULL;
}

static fy_int releaseMonitor(fy_context *context, fy_thread *thread,
		fy_uint monitorId, fy_exception *exception) {
	//TODO;
	return 0;
}

void fy_tmSleep(fy_context *context, fy_thread *thread, fy_long time) {
	thread->nextWakeTime = fy_portTimeMillSec(context) + time;
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
		thread->nextWakeTime = fy_portTimeMillSec() + time;
	}
	thread->yield = TRUE;
}

void fy_tmNotify(fy_context *context, fy_thread *thread, fy_int monitorId,
		fy_boolean all, fy_exception *exception) {
	fy_object *monitor;
	fy_int i;
	fy_thread * target;
	fy_linkedListNode *node;

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
	node = context->threads.head;
	while ((node = node->next) != NULL) {
		target = node->info;
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

fy_boolean fy_tmIsAlive(fy_context *context,fy_uint threadHandle){

}

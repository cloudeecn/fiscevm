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
#include "fyc/NConfig.h"

static fy_thread *getThreadByHandle(fy_context *context, fy_uint targetHandle,
		fy_exception *exception) {
	//TODO
	fy_object *obj = context->objects + targetHandle;
	fy_uint threadId = obj->attachedId;
	if (threadId == 0) {
		return NULL;
	} //
	ASSERT(threadId>0 && threadId<MAX_THREADS);
	return context->threads[threadId];
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
	return monitorExit(context, thread, monitorId, monitor->monitorOwnerTimes,
			exception);
}

static fy_uint fetchNextThreadId(fy_context *context, fy_exception *exception) {
	fy_int h = context->nextThreadId;
	fy_thread *target;
	while ((target = context->threads[h]) != 0) {
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

void fy_tmMonitorEnter(fy_context *context, fy_thread *thread,
		fy_uint monitorId) {
	monitorEnter(context, thread, monitorId, 1);
}

void fy_tmMonitorExit(fy_context *context, fy_thread *thread, fy_uint monitorId,
		fy_exception *exception) {
	monitorExit(context, thread, monitorId, 1, exception);
}

void fy_tmSleep(fy_context *context, fy_thread *thread, fy_long time) {
	thread->nextWakeTime = fy_portTimeMillSec(context->port) + time;
	thread->yield = TRUE;
}

void fy_tmInterrupt(fy_context *context, fy_uint targetHandle,
		fy_exception *exception) {
	fy_thread *target;
	fy_uint exceptionHandle;
	fy_exception targetException;
	targetException.exceptionType = exception_none;
	sprintf_s(targetException.exceptionName,
			sizeof(targetException.exceptionName), FY_EXCEPTION_INTR);
	targetException.exceptionDesc[0] = 0;

	target = getThreadByHandle(context, targetHandle, exception);
	if (exception->exceptionType != exception_none) {
		return;
	}
	if (target == NULL) {
		return;
	}
	if (target->nextWakeTime > 0) {
		ASSERT(target->currentThrowable == 0);
		fy_heapBeginProtect(context);
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

	target = getThreadByHandle(context, targetHandle, exception);
	if (target == NULL) {
		return 0;
	}
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
				FY_EXCEPTION_IMSE);
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
				FY_EXCEPTION_IMSE);
		exception->exceptionDesc[0] = 0;
		return;
	}
	for (i = context->runningThreads->length - 1; i >= 0; i--) {
		fy_arrayListGet(context->memblocks, context->runningThreads, i,
				&target);
		if (target != NULL && target->waitForNotifyId == monitorId) {
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

	target = getThreadByHandle(context, threadHandle, exception);
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
	fy_class *charArrayClass;
	fy_uint threadHandle;
	fy_uint threadNameHandle;
	fy_field *threadNameField;
	fy_field *threadPriorityField;
	int i;

	if (context->state != FY_TM_STATE_NEW) {
		fy_fault(exception, NULL, "Status is not new while booting! %d",
				context->state);
		return;
	}

	charArrayClass = fy_vmLookupClass(context, context->sArrayChar, exception);
	FYEH();

	threadNameHandle = fy_heapAllocateArray(context, charArrayClass,
			clazz->className->length + 5, exception);
	FYEH();
	/*No exception will happen if the allocate is finished*/
	fy_heapPutArrayChar(context, threadNameHandle, 0, 'M', exception);
	fy_heapPutArrayChar(context, threadNameHandle, 1, 'a', exception);
	fy_heapPutArrayChar(context, threadNameHandle, 2, 'i', exception);
	fy_heapPutArrayChar(context, threadNameHandle, 3, 'n', exception);
	fy_heapPutArrayChar(context, threadNameHandle, 4, '.', exception);
	for (i = 0; i < clazz->className->length; i++) {
		fy_heapPutArrayChar(context, threadNameHandle, i + 5,
				clazz->className->content[i], exception);
	}FYEH();

	threadClass = fy_vmLookupClass(context, context->sThread, exception);
	FYEH();

	threadNameField = fy_vmLookupFieldVirtual(context, threadClass,
			context->sFName, exception);
	FYEH();

	threadPriorityField = fy_vmLookupFieldVirtual(context, threadClass,
			context->sFPriority, exception);
	FYEH();

	method = fy_vmLookupMethodVirtual(context, clazz, context->sFMain,
			exception);
	FYEH();

	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_NO_METHOD, "Main method not found!");
		return;
	}

	threadId = fetchNextThreadId(context, exception);
	FYEH();

	thread = fy_mmAllocate(context->memblocks, sizeof(fy_thread), exception);
	FYEH();

	threadHandle = fy_heapAllocate(context, threadClass, exception);
	FYEH();

	fy_heapPutFieldHandle(context, threadHandle, threadNameField,
			threadNameHandle, exception);
	FYEH();

	fy_heapPutFieldInt(context, threadHandle, threadPriorityField, 5,
			exception);
	FYEH();

	thread->threadId = threadId;
	context->threads[threadId] = thread;
	thread->priority = 5;
	fy_threadInitWithMethod(context, thread, threadHandle, method, exception);
	FYEH();

	fy_arrayListAdd(context->memblocks, context->runningThreads, &thread,
			exception);
	FYEH();
	context->state = FY_TM_STATE_RUN_PENDING;
	context->nextGCTime = fy_portTimeMillSec(context->port) + FY_GC_IDV;
	context->nextForceGCTime = context->nextGCTime + FY_GC_FORCE_IDV;
}

void fy_tmPushThread(fy_context *context, fy_uint threadHandle,
		fy_exception *exception) {
	fy_thread *thread;
	fy_uint threadId;
	fy_int priority;
	fy_boolean daemon;
	fy_class *threadClass;
	fy_field *threadDaemonField;
	fy_field *threadPriorityField;

	if (context->state != FY_TM_STATE_RUNNING
			&& context->state != FY_TM_STATE_STOP_PENDING) {
		fy_fault(exception, NULL,
				"Status is not RUNNING or STOP_PENDING while adding thread! %d",
				context->state);
		return;
	}

	threadClass = fy_vmLookupClass(context, context->sThread, exception);
	FYEH();
	threadDaemonField = fy_vmLookupFieldVirtual(context, threadClass,
			context->sFDaemon, exception);
	FYEH();
	threadPriorityField = fy_vmLookupFieldVirtual(context, threadClass,
			context->sFPriority, exception);
	FYEH();

	priority = fy_heapGetFieldInt(context, threadHandle, threadPriorityField,
			exception);
	FYEH();
	if (priority == 0) {
		priority = 5;
	}

	daemon = fy_heapGetFieldBoolean(context, threadHandle, threadDaemonField,
			exception);
	FYEH();

	threadId = fetchNextThreadId(context, exception);
	FYEH();

	thread = fy_mmAllocate(context->memblocks, sizeof(fy_thread), exception);
	FYEH();
	thread->threadId = threadId;
	context->threads[threadId] = thread;
	thread->priority = priority;
	thread->daemon = daemon;
	fy_threadInitWithRun(context, thread, threadHandle, exception);
	FYEH();

	fy_arrayListAdd(context->memblocks, context->runningThreads, &thread,
			exception);
	FYEH();
}

void fy_tmRun(fy_context *context, fy_message *message, fy_exception *exception) {
	/*exception means exception in thread manager
	 * message means exception in thread*/
//	fy_long nextGC = fy_portTimeMillSec(context->port) + 500;
//	fy_long nextGCForce = nextGC + 1000;
	fy_boolean stateLocal;
	fy_arrayList *running = context->runningThreads;

#ifndef FY_LATE_DECLARATION
	fy_thread *thread;
	fy_long nextWakeUpTime,now,sleepTime;
	fy_uint lockId;
	fy_object *lock;
#endif

	if (context->state != FY_TM_STATE_RUN_PENDING
			&& context->state != FY_TM_STATE_RUNNING
			&& context->state != FY_TM_STATE_STOP_PENDING) {
		fy_fault(exception, NULL, "Illegal VM status %d $tmRun1",
				context->state);
		return;
	}
	context->state = FY_TM_STATE_RUNNING;
	while (1) {
#ifdef FY_LATE_DECLARATION
		fy_thread *thread;
#endif
		message->messageType = message_none;
		stateLocal = context->state;
		switch (stateLocal) {
		case FY_TM_STATE_RUNNING: {
#ifdef FY_LATE_DECLARATION
			fy_long nextWakeUpTime;
			fy_uint lockId;
			fy_object *lock;
#endif
			if (running->length > 0) {
				if (context->runningThreadPos < running->length) {
					fy_arrayListGet(context->memblocks, running,
							context->runningThreadPos, &thread);
					if (thread->destroyPending) {
						fy_threadDestroy(context, thread);
						fy_arrayListRemove(context->memblocks, running,
								context->runningThreadPos, exception);
						FYEH();

						context->threads[thread->threadId] = NULL;
						fy_mmFree(context->memblocks, thread);
						break;
					}
					context->runningThreadPos++;
					if (!thread->daemon) {
						context->run = TRUE;
					}
					nextWakeUpTime = thread->nextWakeTime;
					if (nextWakeUpTime > fy_portTimeMillSec(context->port)) {
						if (context->nextWakeUpTimeTotal > nextWakeUpTime) {
							context->nextWakeUpTimeTotal = nextWakeUpTime;
						}
						break;
					}
					thread->nextWakeTime = 0;
					context->nextWakeUpTimeTotal = 0;
					lockId = thread->waitForLockId;
					if (lockId > 0) {
						lock = context->objects + lockId;
						if (lock->monitorOwnerId <= 0) {
							lock->monitorOwnerId = thread->threadId;
							lock->monitorOwnerTimes = thread->pendingLockCount;
							thread->waitForLockId = 0;
						} else {
							break;
						}
					}
					fy_threadRun(context, thread, message,
							context->pricmds[thread->priority]);
					fy_heapEndProtect(context);
					switch (message->messageType) {
					case message_continue:
					case message_vm_dead:
					case message_sleep:
						/*Illegal!*/
						fy_fault(exception, NULL, "Illegal message type %d",
								message->messageType);
						return;
					case message_none:
						break;
					case message_exception:
						/*critical exception happened*/
						message->thread = thread;
						return;
					case message_invoke_native:
						/*send invoke native message*/
						message->thread = thread;
						fy_heapBeginProtect(context);
						return;
					case message_thread_dead:
						thread->destroyPending = TRUE;
						break;
					default:
						/*Illegal*/
						fy_fault(exception, NULL, "Illegal message type %d",
								message->messageType);
						return;
					}

				} else {
					if (!context->run) {
						context->state = FY_TM_STATE_DEAD;
					} else {

#ifdef FY_LATE_DECLARATION
						fy_long now, sleepTime;
#endif
						now = fy_portTimeMillSec(context->port);
						sleepTime = context->nextWakeUpTimeTotal - now;
						if ((sleepTime > 10 && now > context->nextGCTime)
								|| now > context->nextForceGCTime) {
							context->nextGCTime = now + FY_GC_IDV;
							context->nextForceGCTime = context->nextGCTime
									+ FY_GC_FORCE_IDV;
							DLOG("Call GC due to time out");
							fy_heapGC(context, exception);
							FYEH();
							now = fy_portTimeMillSec(context->port);
							sleepTime = context->nextWakeUpTimeTotal - now;
						}
						context->nextWakeUpTimeTotal = 0x7fffffffffffffffLL;
						context->runningThreadPos = 0;
						context->run = FALSE;
						if (sleepTime > 0) {
							message->messageType = message_sleep;
							message->body.sleepTime = sleepTime;
							return;
						}
					}
				}
			} else {
				context->state = FY_TM_STATE_DEAD;
			}
			break;
		}
		case FY_TM_STATE_STOP_PENDING: {
			context->runningThreadPos = 0;
			context->state = FY_TM_STATE_STOP;
			break;
		}
		case FY_TM_STATE_DEAD_PENDING:
			context->state = FY_TM_STATE_DEAD;
			break;
		case FY_TM_STATE_DEAD:
			message->messageType = message_vm_dead;
			return;
		default:
			fy_fault(exception, NULL, "Illegal vm state %d $tmRun2",
					context->state);
			return;
		}
	}
}


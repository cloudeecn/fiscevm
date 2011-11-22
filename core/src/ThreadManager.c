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

static fy_thread *getThreadByHandle(juint targetHandle, fy_exception *exception) {
	//TODO
	return NULL;
}

void fy_tmSleep(fy_context *context, fy_thread *thread, jlong time) {
	thread->nextWakeTime = fy_portTimeMillSec(context) + time;
}

void fy_tmInterrupt(fy_context *context, juint targetHandle,
		fy_exception *exception) {
	fy_thread *target;
	juint exceptionHandle;
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

jboolean fy_tmIsInterrupted(fy_context *context, juint targetHandle,
		jboolean clear, fy_exception *exception) {
	fy_thread *target;
	jboolean ret;

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


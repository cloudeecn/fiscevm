/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of libfisce.
 *
 * libfisce is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * libfisce is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libfisce.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fyc/RIS.h"
#include "fy_util/String.h"
#include "fyc/Heap.h"
#include "fyc/VMContext.h"

static void RISBind0(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_message *message,
		fy_exception *exception) {
	/**
	 * args[0] - this
	 * args[1] - name
	 * args[2] - pos
	 */
	fy_inputStream *is;
	char cName[512];
	fy_int i;
	fy_uint thisHandle = args[0];
	fy_uint nameHandle = args[1];
	fy_int pos = (fy_int) args[2];
	fy_str name[1];
	fy_object *stream = fy_heapGetObject(context,thisHandle);
	fy_int streamId = stream->object_data->streamId;

	if (streamId == 0 || context->aliveStreams[streamId] == NULL) {
		name->content = NULL;
		fy_strInit(context->memblocks, name, 256, exception);
		FYEH();

		fy_heapGetString(context, nameHandle, name, exception);
		if (exception->exceptionType != exception_none) {
			fy_strDestroy(context->memblocks, name);
			return;
		}

		fy_strSPrint(cName, sizeof(cName), name);
		fy_strDestroy(context->memblocks, name);

		is = context->isOpen(context, cName, exception);
		FYEH();
		if (is == NULL) {
			fy_fault(exception, FY_EXCEPTION_FNF, cName);
			FYEH();
		}

		if (streamId == 0) {
			for (i = 1; i < MAX_STREAMS; i++) {
				if (context->aliveStreams[i] == NULL) {
					context->aliveStreams[i] = is;
					streamId = i;
					break;
				}
			}
			if (streamId == 0) {
				fy_fault(exception, FY_EXCEPTION_IO, "Too many file opened");
				FYEH();
			}
			stream->object_data->streamId = streamId;
		} else {
			if (context->aliveStreams[streamId] == NULL) {
				context->aliveStreams[streamId] = is;
			} else {
				fy_fault(exception, FY_EXCEPTION_IO, "Slot already allocated");
				FYEH();
			}
		}

		if (pos > 0) {
			is->isSkip(context, is, pos, exception);
			FYEH();
		}
	}
}

static void RISRead0(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_message *message,
		fy_exception *exception) {
	fy_object *stream = fy_heapGetObject(context,args[0]);
	fy_inputStream *is = context->aliveStreams[stream->object_data->streamId];
	fy_nativeReturnInt(context, thread, is->isRead(context, is, exception));
}

static void RISRead0BII(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_message *message,
		fy_exception *exception) {
	fy_byte buf[4096];
	fy_byte *target;
	fy_object *stream = fy_heapGetObject(context,args[0]);
	fy_inputStream *is = context->aliveStreams[stream->object_data->streamId];
	fy_uint bufHandle = args[1];
	fy_int pos = (fy_int) args[2];
	fy_int len = (fy_int) args[3];

	if (len > sizeof(buf)) {
		len = sizeof(buf);
	}

	len = is->isReadBlock(context, is, buf, len, exception);
	FYEH();

	target = fy_heapGetArrayBytes(context, bufHandle, exception);
	FYEH();

	if(len>=0){
		memcpy(target + pos, buf, len);
	}
	fy_nativeReturnInt(context, thread, len);
}

static void RISClose0(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_message *message,
		fy_exception *exception) {
	fy_object *stream = fy_heapGetObject(context,args[0]);
	fy_inputStream *is = context->aliveStreams[stream->object_data->streamId];
	is->isClose(context, is, exception);
	fy_mmFree(context->memblocks, is);
	context->aliveStreams[stream->object_data->streamId] = NULL;
	stream->object_data->streamId = 0;
}

void fy_risInit(fy_context *context, fy_exception *exception) {
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/ResourceInputStream.bind0.(L"FY_BASE_STRING";I)V",
			NULL, RISBind0, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/ResourceInputStream.read0.()I", NULL,
			RISRead0, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/ResourceInputStream.read0.([BII)I",
			NULL, RISRead0BII, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/ResourceInputStream.close0.()V", NULL,
			RISClose0, exception);
	FYEH();

}

void fy_risDestroy(fy_context *context) {
	fy_int i;
	fy_inputStream *is;
	fy_exception exception[1];
	for (i = 0; i < MAX_STREAMS; i++) {
		is = context->aliveStreams[i];
		if (is != NULL) {
#ifdef FY_DEBUG
			context->logDVarLn(context,
					"Releasing opened stream at slot #%"FY_PRINT32"d", i);
#endif
			is->isClose(context, is, exception);
			if (exception->exceptionType != exception_none) {
				exception->exceptionType = exception_none;
				context->logWVarLn(context,
						"Exception in closing stream #%"FY_PRINT32"d %s", i,
						exception->exceptionDesc);
			}
		}
	}
}

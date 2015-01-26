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
#include "fiscedev.h"

#include "fyc/Thread.h"
FY_ATTR_EXPORT void fy_nativeReturnInt(fy_context *context, fy_thread *thread,
		fy_int value) {
	fy_frame *frame;
	fy_int sp;
	fy_stack_item *spp;
	frame = fy_threadCurrentFrame(context, thread);
	sp = frame->method->instruction_extras[frame->lpc + frame->pcofs].sp - 1;
	if (sp < frame->method->max_stack
			|| sp >= frame->method->max_stack + frame->method->max_locals) {
		fy_fault(NULL, NULL, "Illegal sp %"FY_PRINT32"d", sp);
	}
	spp = frame->baseSpp + sp;
	fy_threadReturnInt(spp, value);
}

FY_ATTR_EXPORT void fy_nativeReturnHandle(fy_context *context,
		fy_thread *thread, fy_int value) {
	fy_frame *frame;
	fy_int sp;
	fy_stack_item *spp;
	frame = fy_threadCurrentFrame(context, thread);
	sp = frame->method->instruction_extras[frame->lpc + frame->pcofs].sp - 1;
	if (sp < frame->method->max_stack
			|| sp >= frame->method->max_stack + frame->method->max_locals) {
		fy_fault(NULL, NULL, "Illegal sp %"FY_PRINT32"d", sp);
	}
	spp = frame->baseSpp + sp;
	return fy_threadReturnInt(spp, value);
}

FY_ATTR_EXPORT void fy_nativeReturnLong(fy_context *context, fy_thread *thread,
		fy_long value) {
	fy_frame *frame;
	fy_int sp;
	fy_stack_item *spp;
	frame = fy_threadCurrentFrame(context, thread);
	sp = frame->method->instruction_extras[frame->lpc + frame->pcofs].sp - 1;
	if (sp < frame->method->max_stack
			|| sp >= frame->method->max_stack + frame->method->max_locals) {
		fy_fault(NULL, NULL, "Illegal sp %"FY_PRINT32"d", sp);
	}
	spp = frame->baseSpp + sp;
	return fy_threadReturnLong(spp, value);
}

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

#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fyc/Thread.h"
#include "fyc/Instructions.h"

void fy_nativeReturnInt(fy_context *context, fy_thread *thread,
		fisce_int value) {
	fy_frame *frame;
	fisce_int sp;
	fisce_stack_item *spp;
	frame = fy_threadCurrentFrame(context, thread);
	sp = frame->method->c.i.instruction_extras[frame->lpc + frame->pcofs].sp - 1;
#ifdef FY_STRICT
	if (sp < frame->method->max_stack
			|| sp >= frame->method->max_stack + frame->method->max_locals) {
		futl_fault(NULL, NULL, "Illegal sp %"FUTL_PRINT32"d", sp);
	}
#endif
	spp = frame->baseSpp + sp;
	fy_threadReturnInt(spp, value);
}

void fy_nativeReturnHandle(fy_context *context,
		fy_thread *thread, fisce_int value) {
	fy_frame *frame;
	fisce_int sp;
	fisce_stack_item *spp;
	frame = fy_threadCurrentFrame(context, thread);
	sp = frame->method->c.i.instruction_extras[frame->lpc + frame->pcofs].sp - 1;
#ifdef FY_STRICT
	if (sp < frame->method->max_stack
			|| sp >= frame->method->max_stack + frame->method->max_locals) {
		futl_fault(NULL, NULL, "Illegal sp %"FUTL_PRINT32"d", sp);
	}
#endif
	spp = frame->baseSpp + sp;
	fy_threadReturnInt(spp, value);
}

void fy_nativeReturnLong(fy_context *context, fy_thread *thread,
		fisce_long value) {
	fy_frame *frame;
	fisce_int sp;
	fisce_stack_item *spp;
	frame = fy_threadCurrentFrame(context, thread);
	sp = frame->method->c.i.instruction_extras[frame->lpc + frame->pcofs].sp - 1;
#ifdef FY_STRICT
	if (sp < frame->method->max_stack
			|| sp >= frame->method->max_stack + frame->method->max_locals) {
		futl_fault(NULL, NULL, "Illegal sp %"FUTL_PRINT32"d", sp);
	}
#endif
	spp = frame->baseSpp + sp;
	fy_threadReturnLong(spp, value);
}

void fy_nativeReturnFloat(fy_context *context, fy_thread *thread,
		fisce_float value) {
    fy_nativeReturnInt(context, thread, fisce_floatToInt(value));
}

void fy_nativeReturnDouble(fy_context *context, fy_thread *thread,
		fisce_double value) {
    fy_nativeReturnLong(context, thread, fisce_doubleToLong(value));
}


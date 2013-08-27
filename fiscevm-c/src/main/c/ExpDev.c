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
#include "fiscedev.h"

#include "fyc/Thread.h"
FY_ATTR_EXPORT void fy_nativeReturnInt(fy_context *context, fy_thread *thread,
		fy_int value) {
	return fy_threadReturnInt(context, thread, value);
}

FY_ATTR_EXPORT void fy_nativeReturnHandle(fy_context *context,
		fy_thread *thread, fy_int value) {
	return fy_threadReturnHandle(context, thread, value);
}

FY_ATTR_EXPORT void fy_nativeReturnLong(fy_context *context, fy_thread *thread,
		fy_long value) {
	return fy_threadReturnLong(context, thread, value);
}

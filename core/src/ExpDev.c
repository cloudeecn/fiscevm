/**
 *  Copyright 2010 Yuxuan Huang. All rights reserved.
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
#include "fiscedev.h"

#include "fyc/Thread.h"
_FY_EXPORT void fy_nativeReturnInt(fy_context *context, fy_thread *thread,
		fy_int value) {
	return fy_threadReturnInt(context, thread, value);
}

_FY_EXPORT void fy_nativeReturnHandle(fy_context *context, fy_thread *thread,
		fy_int value) {
	return fy_threadReturnHandle(context, thread, value);
}

_FY_EXPORT void fy_nativeReturnLong(fy_context *context, fy_thread *thread,
		fy_long value) {
	return fy_threadReturnLong(context, thread, value);
}

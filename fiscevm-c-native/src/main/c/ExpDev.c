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
FY_ATTR_EXPORT void fy_nativeReturnInt(fy_stack_item *spp,
		fy_int value) {
	return fy_threadReturnInt(spp, value);
}

FY_ATTR_EXPORT void fy_nativeReturnHandle(fy_stack_item *spp, fy_int value) {
	return fy_threadReturnHandle(spp, value);
}

FY_ATTR_EXPORT void fy_nativeReturnLong(fy_stack_item *spp,
		fy_long value) {
	return fy_threadReturnLong(spp, value);
}

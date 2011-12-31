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
#ifndef _FY_FISCE_H
#define _FY_FISCE_H

#include "fiscecon.h"
#include "fisceprt.h"
#include "fiscestu.h"
#include "fiscedev.h"

#ifdef	__cplusplus
extern "C" {
#endif

FY_ATTR_EXPORT void fisceInitContext(fy_context *context, fy_exception *exception);

FY_ATTR_EXPORT void fisceDestroyContext(fy_context *context);

FY_ATTR_EXPORT void fisceBootFromMain(fy_context *context, const char *name,
		fy_exception *exception);

FY_ATTR_EXPORT void fisceRun(fy_context *context, fy_message *message,
		fy_exception *exception);

#ifdef	__cplusplus
}
#endif

#endif

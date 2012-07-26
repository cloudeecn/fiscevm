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

#ifndef FY_CLASSLOADER_H
#define	FY_CLASSLOADER_H

#include "../fisceprt.h"
#include "../fy_util/MemMan.h"
#include "../fy_util/String.h"
#include "../fy_util/Debug.h"
#include "../fiscestu.h"
#include "VMContext.h"
#include "Class.h"
#include "Data.h"
#ifdef	__cplusplus
extern "C" {
#endif
fy_str *fy_clGetConstantString(fy_context *context, fy_class *clazz,
		fy_char idx);
fy_class *fy_clLoadclass(fy_context *context, fy_str *name,fy_inputStream *is,
		fy_exception *exception);
void fy_clPhase2(fy_context *context, fy_class *clazz, fy_exception *exception);
#ifdef	__cplusplus
}
#endif
#endif	/* FY_CLASSLOADER_H */


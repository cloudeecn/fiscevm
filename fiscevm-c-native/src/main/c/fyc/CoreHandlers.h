/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
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
#ifndef FY_COREHANDLERS_H_
#define FY_COREHANDLERS_H_

#include <float.h>
#include "../fiscestu.h"
#include "../fiscedev.h"
#include "VMContext.h"
#include "Thread.h"
#include "Heap.h"
#ifdef	__cplusplus
extern "C" {
#endif
void fy_coreRegisterCoreHandlers(fy_context *context, fy_exception *exception);

void fy_coreRegisterMathHandlers(fy_context *context, fy_exception *exception);
#ifdef	__cplusplus
}
#endif

#endif /* FY_COREHANDLERS_H_ */

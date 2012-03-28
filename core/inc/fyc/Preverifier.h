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
#ifndef PREVERIFER_H_
#define PREVERIFER_H_

#include "../fisceprt.h"
#include "../fiscestu.h"
#include "../fiscedev.h"
#include "../fy_util/Debug.h"
#include "VMContext.h"

void fy_preverify(fy_context *context, fy_method *method, fy_exception *exception);

#endif /* PREVERIFER_H_ */

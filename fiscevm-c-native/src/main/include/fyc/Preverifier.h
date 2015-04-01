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
#ifndef PREVERIFER_H_
#define PREVERIFER_H_

#include "fy_util/Portable.h"
#include "fyc/typedefs.h"

#ifdef	__cplusplus
extern "C" {
#endif
void fy_preverify(fy_context *context, fy_method *method,
		fisce_exception *exception);
#ifdef	__cplusplus
}
#endif

#endif /* PREVERIFER_H_ */

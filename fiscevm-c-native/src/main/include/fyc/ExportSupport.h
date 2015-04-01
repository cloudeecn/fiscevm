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

#ifndef FISCEVM_EXPORTSUPPORT_H_
#define FISCEVM_EXPORTSUPPORT_H_

void fy_nativeReturnInt(fy_context *context, fy_thread *thread,
        fisce_int value);

void fy_nativeReturnHandle(fy_context *context,
        fy_thread *thread, fisce_int value);

void fy_nativeReturnLong(fy_context *context, fy_thread *thread,
        fisce_long value);

void fy_nativeReturnFloat(fy_context *context, fy_thread *thread,
        fisce_float value);

void fy_nativeReturnDouble(fy_context *context, fy_thread *thread,
        fisce_double value);


#endif /* MAIN_INCLUDE_FISCEVM_EXPORTSUPPORT_H_ */

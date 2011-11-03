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



#ifndef COREHANDLERS_H_
#define COREHANDLERS_H_

#include <float.h>
#include "Structs.h"
#include "VMContext.h"
#include "Thread.h"
#include "Heap.h"


#ifdef	__cplusplus
extern "C" {
#endif

void fy_coreRegisterCoreHandlers(fy_VMContext *context);

#ifdef	__cplusplus
}
#endif

#endif /* COREHANDLERS_H_ */

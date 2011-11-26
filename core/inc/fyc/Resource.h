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

#ifndef FY_RESOURCE_H_
#define FY_RESOURCE_H_

#include "../fisceprt.h"
#include "Data.h"
#include "VMContext.h"
#ifdef	__cplusplus
extern "C" {
#endif

fy_data *fy_resourceAllocateData(fy_context *context, const char *name,
		fy_exception *exception);

void fy_resourceReleaseData(fy_context *context, fy_data *data);

#ifdef	__cplusplus
}
#endif
#endif /* FY_RESOURCE_H_ */

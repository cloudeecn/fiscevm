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

#ifndef FY_DEBUG_H_
#define FY_DEBUG_H_

#include "fy_util/Portable.h"
#include "fy_util/String.h"

#ifdef	__cplusplus
extern "C" {
#endif

FY_ATTR_EXPORT void fy_strFPrint(FILE *fp, const fy_str *str);

FY_ATTR_EXPORT void fy_strPrint(const fy_str *str);

#ifdef FY_DEBUG
# define ASSERT(TEST) {if(!(TEST)) fy_fault(NULL,NULL,"ASSERTION ERROR IN %s:%d", __FILE__,__LINE__);}
#else
#define ASSERT(TEST)
# endif

#ifdef	__cplusplus
}
#endif

#endif /* FY_DEBUG_H_ */

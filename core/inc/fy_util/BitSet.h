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

#ifndef BITSET_H_
#define BITSET_H_

#include "../fisceprt.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define fy_bitSHIFT 5
#define fy_bitMASK ((1<<fy_bitSHIFT)-1)
#define fy_bitSizeToInt(X) (((X)+fy_bitMASK)>>fy_bitSHIFT)

void fy_bitSet(fy_uint *marks, fy_uint pos);

void fy_bitClear(fy_uint *marks, fy_uint pos);

fy_boolean fy_bitGet(fy_uint *marks, fy_uint pos);

#ifdef	__cplusplus
}
#endif

#endif /* BITSET_H_ */

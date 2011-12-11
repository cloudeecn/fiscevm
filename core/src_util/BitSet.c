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

#include "fy_util/BitSet.h"

void fy_bitSet(fy_uint *marks, fy_uint pos) {
	marks[pos >> fy_bitSHIFT] |= 1 << (pos & fy_bitMASK);
}

void fy_bitClear(fy_uint *marks, fy_uint pos) {
	marks[pos >> fy_bitSHIFT] &= ~(fy_uint) (1 << (pos & fy_bitMASK));
}

fy_boolean fy_bitGet(fy_uint *marks, fy_uint pos) {
	return (marks[pos >> fy_bitSHIFT] >> (pos & fy_bitMASK)) & 1;
}


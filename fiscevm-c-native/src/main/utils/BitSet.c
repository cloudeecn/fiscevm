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

#include "fy_util/BitSet.h"

fisce_uint fy_bits[32] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100,
		0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000,
		0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000, 0x1000000,
		0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000,
		0x80000000, };
fisce_uint fy_nbits[32] =
		{ 0xffffffff - 0x1, 0xffffffff - 0x2, 0xffffffff - 0x4, 0xffffffff
				- 0x8, 0xffffffff - 0x10, 0xffffffff - 0x20, 0xffffffff - 0x40,
				0xffffffff - 0x80, 0xffffffff - 0x100, 0xffffffff - 0x200,
				0xffffffff - 0x400, 0xffffffff - 0x800, 0xffffffff - 0x1000,
				0xffffffff - 0x2000, 0xffffffff - 0x4000, 0xffffffff - 0x8000,
				0xffffffff - 0x10000, 0xffffffff - 0x20000, 0xffffffff
						- 0x40000, 0xffffffff - 0x80000, 0xffffffff - 0x100000,
				0xffffffff - 0x200000, 0xffffffff - 0x400000, 0xffffffff
						- 0x800000, 0xffffffff - 0x1000000, 0xffffffff
						- 0x2000000, 0xffffffff - 0x4000000, 0xffffffff
						- 0x8000000, 0xffffffff - 0x10000000, 0xffffffff
						- 0x20000000, 0xffffffff - 0x40000000, 0xffffffff
						- 0x80000000, };

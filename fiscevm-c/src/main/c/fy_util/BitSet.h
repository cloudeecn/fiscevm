/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part offiscevm
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
 * along withfiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BITSET_H_
#define BITSET_H_

#include "../fisceprt.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define fy_bitSHIFT 5
#define fy_bitMASK 31/*((1<<fy_bitSHIFT)-1)*/
#define fy_bitSizeToInt(X) (((X)+fy_bitMASK)>>fy_bitSHIFT)
#define fy_bitSet(BS,POS)	(BS)[(POS) >> fy_bitSHIFT] |= fy_bits[(POS) & fy_bitMASK]
#define fy_bitClear(BS,POS) (BS)[(POS) >> fy_bitSHIFT] &= fy_nbits[(POS) & fy_bitMASK]
#define fy_bitGet(BS,POS) (((BS)[(POS) >> fy_bitSHIFT] >> ((POS) & fy_bitMASK)) & 1)

extern fy_uint fy_bits[32];
extern fy_uint fy_nbits[32];

#ifdef	__cplusplus
}
#endif

#endif /* BITSET_H_ */

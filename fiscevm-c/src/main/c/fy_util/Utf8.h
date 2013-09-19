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
#ifndef FY_UTF8_H_
#define FY_UTF8_H_
#include "../fisceprt.h"
#ifdef	__cplusplus
extern "C" {
#endif

fy_uint fy_utf8Size(fy_char unicode);

fy_uint fy_utf8SizeU(signed char firstByte);

fy_uint fy_utf8SizeS(const char *str, fy_int length);

fy_char fy_utf8Read(const char **from, fy_int *left);

int fy_utf8Write(fy_char unicode, char **to, fy_int *left);

#ifdef	__cplusplus
}
#endif

#endif /* FY_UTF8_H_ */

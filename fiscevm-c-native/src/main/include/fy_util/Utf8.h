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
#ifndef FY_UTF8_H_
#define FY_UTF8_H_
#include "fy_util/Portable.h"

#ifdef	__cplusplus
extern "C" {
#endif

fisce_uint fy_utf8Size(fisce_char unicode);

fisce_uint fy_utf8SizeU(signed char firstByte);

fisce_uint fy_utf8SizeS(const char *str, fisce_int length);

fisce_char fy_utf8Read(const char **from, fisce_int *left);

int fy_utf8Write(fisce_char unicode, char **to, fisce_int *left);

#ifdef	__cplusplus
}
#endif

#endif /* FY_UTF8_H_ */

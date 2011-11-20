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
#ifndef FY_UTF8_H_
#define FY_UTF8_H_
#include "Portable.h"
#ifdef	__cplusplus
extern "C" {
#endif

juint fy_utf8Size(jchar unicode);

juint fy_utf8SizeU(char firstByte);

juint fy_utf8SizeS(const char *str, jint length);

jchar fy_utf8Read(const char **from, jint *left);

int fy_utf8Write(jchar unicode, char **to, jint *left);

#ifdef	__cplusplus
}
#endif

#endif /* FY_UTF8_H_ */

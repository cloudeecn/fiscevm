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

#ifndef FY_STRING_H_
#define FY_STRING_H_

#include "../fisceprt.h"
#include "../fiscestu.h"
#include "Utf8.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct fy_str {
	int length;
	int maxLength;
	int hashed;
	int hashCode;
	fy_char* content;
} fy_str;

#define fy_strCreate(BLOCK) fy_strInit((BLOCK),fy_vmAllocate((BLOCK),sizeof(fy_str)),16)
#define fy_strRelease(BLOCK,STR) fy_strDestroy((BLOCK),(STR));fy_vmFree((BLOCK),(STR));

fy_str *fy_strCreateFromUTF8(fy_context *context, const char *utf8);

fy_str *fy_strInit(struct fy_context *context, fy_str *string, fy_int size);

fy_str *fy_strNew(struct fy_context *context, const char *utf8);

void fy_strDestroy(struct fy_context *context, fy_str *string);

fy_str *fy_strAppend(struct fy_context *context, fy_str *this,
		const fy_str *string);
fy_str *fy_strAppendChar(fy_context *context, fy_str *this, fy_char ch);
fy_str *fy_strAppendUTF8(struct fy_context *context, fy_str *this,
		const char* utf8, fy_int size);
fy_str *fy_strSubstring(struct fy_context *context, fy_str *this, fy_int begin,
		fy_int end);
fy_uint fy_strUtf8Count(fy_str *str);

fy_uint fy_strHash(fy_str *str);

int fy_strCmp(fy_str *left, fy_str *right);
fy_boolean fy_strEndsWith(fy_str *this, fy_str *comp);

void fy_strClear(fy_str *this);

char *fy_strSPrint(char *target, size_t targetSize, fy_str *str);

fy_str *fy_strReplaceOne(fy_str *str, fy_char from,fy_char to);

fy_str *fy_strCreateClone(fy_context *context, fy_str *from);

#ifdef	__cplusplus
}
#endif

#endif /* FY_STRING_H_ */

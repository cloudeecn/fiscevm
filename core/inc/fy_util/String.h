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
#include "MemMan.h"
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
#define fy_strCreate(BLOCK,EXCEPTION) fy_strInit((BLOCK),fy_mmAllocate((BLOCK),sizeof(fy_str),(EXCEPTION)),16,(EXCEPTION))
#define fy_strRelease(BLOCK,STR) fy_strDestroy((BLOCK),(STR));fy_mmFree((BLOCK),(STR));
FY_ATTR_EXPORT void fy_strInitWithUTF8(fy_memblock *block, fy_str *str,
		const char *utf8, fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strCreateFromUTF8(fy_memblock *mem, const char *utf8,
		fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strInit(fy_memblock *block, fy_str *str, fy_int size,
		fy_exception *exception);
FY_ATTR_EXPORT void fy_strDestroy(fy_memblock *mem, fy_str *string);
FY_ATTR_EXPORT fy_str *fy_strEnsureSize(fy_memblock *block, fy_str *_this,
		fy_int size, fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strAppend(fy_memblock *mem, fy_str *this,
		const fy_str *string, fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strAppendChar(fy_memblock *mem, fy_str *this, fy_char ch,
		fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strAppendUTF8(fy_memblock *mem, fy_str *this,
		const char* utf8, fy_int size, fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strSubstring(fy_memblock *mem, fy_str *this, fy_int begin,
		fy_int end);
FY_ATTR_EXPORT fy_uint fy_strUtf8Count(fy_str *str);
FY_ATTR_EXPORT fy_uint fy_strHash(fy_str *str);

FY_ATTR_EXPORT int fy_strCmp(fy_str *left, fy_str *right);
FY_ATTR_EXPORT fy_boolean fy_strEndsWith(fy_str *this, fy_str *comp);

FY_ATTR_EXPORT void fy_strClear(fy_str *this);

FY_ATTR_EXPORT char *fy_strSPrint(char *target, size_t targetSize, fy_str *str);
FY_ATTR_EXPORT fy_str *fy_strReplaceOne(fy_str *str, fy_char from, fy_char to);
FY_ATTR_EXPORT fy_str *fy_strCreateClone(fy_memblock *mem, fy_str *from,
		fy_exception *exception);

#ifdef	__cplusplus
}
#endif

#endif /* FY_STRING_H_ */

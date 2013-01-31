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

#ifndef FY_STR_MAX_VA
# define FY_STR_MAX_VA 16
#endif

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * 请用api来修改content中的内容，否则后果自负……
 */
typedef struct fy_str {
	fy_int length;
	fy_int maxLength;
	fy_uint status;
	fy_int hashCode;
	fy_char* content;
	FY_VLS(fy_char,staticContent);
} fy_str;

typedef union fy_strVarStorage {
	char c;
	char* a;
	fy_str *s;
} fy_strVarStorage;

typedef struct fy_strVA {
	fy_int size;
	char pattern[FY_STR_MAX_VA + 1];
	fy_int patternLength;
	fy_strVarStorage vars[FY_STR_MAX_VA];
} fy_strVA;
/**
 * 字符串可以append，不能改已经放进去的内容
 **/
#define FY_STR_PERSIST 1
/**
 * 字符串放到Perm存储区中，不能被释放，append不能超过maxLength
 **/
#define FY_STR_PERM 2
/**
 * 字符串的Hash值没有变化，可以直接从hashCode中取
 **/
#define FY_STR_HASHED 4

/*Set to 1 for eclipse to search all direct usage of string->content*/
#if 1
FY_ATTR_EXPORT fy_char fy_strGet0(fy_str *str, fy_int pos);
# define fy_strGet(STR,POS) fy_strGet0((STR),(POS))
#else
# define fy_strGet(STR,POS) (STR)->content[(POS)]
#endif
FY_ATTR_EXPORT void fy_strParseV(fy_strVA *output, const char *pattern, ...);
FY_ATTR_EXPORT void fy_strParseVA(fy_strVA *output, const char *pattern,
		va_list arg_ptr);

FY_ATTR_EXPORT void fy_strInitWithUTF8(fy_memblock *block, fy_str *str,
		const char *utf8, fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strCreatePerm(fy_memblock *mem, fy_int size,
		fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strCreatePermFromClone(fy_memblock *mem,
		fy_str *other, fy_int additionalSize, fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strCreatePermFromSubstring(fy_memblock *mem,
		fy_str *other, fy_int begin, fy_int end, fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strCreatePermFromUTF8(fy_memblock *mem,
		const char *utf8, fy_int additionalSize, fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strInit(fy_memblock *block, fy_str *str, fy_int size,
		fy_exception *exception);
FY_ATTR_EXPORT void fy_strDestroy(fy_memblock *mem, fy_str *string);
FY_ATTR_EXPORT fy_str *fy_strEnsureSize(fy_memblock *block, fy_str *_this,
		fy_int size, fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strAppendVA(fy_memblock *block, fy_str *_this,
		fy_strVA *va, fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strAppend(fy_memblock *mem, fy_str *this,
		const fy_str *string, fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strAppendChar(fy_memblock *mem, fy_str *this,
		fy_char ch, fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strAppendUTF8(fy_memblock *mem, fy_str *this,
		const char* utf8, fy_int size, fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strSubstring(fy_memblock *mem, fy_str *this,
		fy_int begin, fy_int end);
FY_ATTR_EXPORT fy_uint fy_strUtf8Count(fy_str *str);
FY_ATTR_EXPORT fy_uint fy_strHash(fy_str *str);

FY_ATTR_EXPORT fy_int fy_strCmp(fy_str *left, fy_str *right);
FY_ATTR_EXPORT fy_boolean fy_strEndsWith(fy_str *this, fy_str *comp);

FY_ATTR_EXPORT void fy_strClear(fy_str *this);

FY_ATTR_EXPORT char *fy_strSPrint(char *target, size_t targetSize, fy_str *str);
FY_ATTR_EXPORT fy_str *fy_strReplaceOne(fy_str *str, fy_char from, fy_char to);
FY_ATTR_EXPORT fy_str *fy_strCreateClone(fy_memblock *mem, fy_str *from,
		fy_exception *exception);

FY_ATTR_EXPORT fy_str *fy_strCreatePermPersistVA(fy_memblock *mem, fy_strVA va,
		fy_exception *exception);
FY_ATTR_EXPORT fy_str *fy_strCreatePermPersist(fy_memblock *mem,
		fy_exception *exception, const char *pattern, ...);
FY_ATTR_EXPORT fy_str *fy_strCreatePermPersistSubstring(fy_memblock *mem,
		fy_str *from, int begin, int end, fy_exception *exception);

FY_ATTR_EXPORT fy_uint fy_strHashVA(fy_strVA *va);

#ifdef	__cplusplus
}
#endif

#endif /* FY_STRING_H_ */

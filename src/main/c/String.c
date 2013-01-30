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

#include "fy_util/String.h"
#include <stdarg.h>
FY_ATTR_EXPORT fy_str *fy_strCreatePerm(fy_memblock *mem, fy_int size,
		fy_exception *exception) {
	fy_str *str = fy_mmAllocatePerm(mem,
			sizeof(fy_str) + size * sizeof(fy_char), exception);
	FYEH()NULL;
	str->length = 0;
	str->maxLength = size;
	str->status = FY_STR_PERM;
	str->content = str->staticContent;
	return str;
}

FY_ATTR_EXPORT fy_str *fy_strCreatePermFromClone(fy_memblock *mem,
		fy_str *other, fy_int additionalSize, fy_exception *exception) {
	fy_str *str;
	fy_int size = other->length + additionalSize;
	str = fy_strCreatePerm(mem, size, exception);
	FYEH()NULL;
	fy_strAppend(mem, str, other, exception);
	FYEH()NULL;
	return str;
}
FY_ATTR_EXPORT fy_str *fy_strCreatePermFromSubstring(fy_memblock *mem,
		fy_str *other, fy_int begin, fy_int end, fy_exception *exception) {
	fy_str *ret;

	if (begin < 0 || begin >= other->length || end < begin
			|| end > other->length) {
		fy_fault(exception, NULL,
				"Illegal arguments length=%"FY_PRINT32"d begin=%"FY_PRINT32"d end=%"FY_PRINT32"d",
				other->length, begin, end);
		FYEH()NULL;
	}
	ret = fy_mmAllocatePerm(mem, sizeof(fy_str), exception);
	FYEH()NULL;
	ret->length = end - begin;
	ret->status &= ~FY_STR_PERM;

	return ret;
}
FY_ATTR_EXPORT fy_str *fy_strCreatePermFromUTF8(fy_memblock *mem,
		const char *utf8, fy_int additionalSize, fy_exception *exception) {
	fy_str *str;
	fy_int size = (fy_utf8SizeS(utf8, -1) + 1) / 2 + additionalSize;
	str = fy_strCreatePerm(mem, size, exception);
	FYEH()NULL;
	fy_strAppendUTF8(mem, str, utf8, -1, exception);
	FYEH()NULL;
	return str;
}

FY_ATTR_EXPORT fy_str *fy_strInit(fy_memblock *block, fy_str *str, fy_int size,
		fy_exception *exception) {
	if (str->content != NULL) {
		fy_fault(exception, NULL, "duplicated initJavaString");
		return NULL;
	}
	str->length = 0;
	str->maxLength = size;
	str->content = fy_mmAllocate(block, size << 1, exception);
	str->status = 0;
	return str;
}

FY_ATTR_EXPORT void fy_strDestroy(fy_memblock *block, fy_str *string) {
	if (string->status & FY_STR_PERM) {
		fy_fault(NULL, NULL, "Try to destroy a static string");
	} else {
		fy_mmFree(block, string->content);
		string->length = 0;
		string->maxLength = 0;
		string->content = NULL;
	}
}

FY_ATTR_EXPORT void fy_strInitWithUTF8(fy_memblock *block, fy_str *str,
		const char *utf8, fy_exception *exception) {
	size_t size;
	size = (fy_utf8SizeS(utf8, -1) + 1) / 2;
	fy_strInit(block, str, size, exception);
	FYEH();
	fy_strAppendUTF8(block, str, utf8, -1, exception);
	FYEH();
}

static fy_str *ensureSize(fy_memblock *block, fy_str *_this, fy_int size,
		fy_exception *exception) {
	int len;
	fy_char *newContent;

	if (_this->maxLength < size) {
		if (_this->status & FY_STR_PERM) {
			fy_fault(exception, NULL, "Perm string overflow!");
			FYEH()NULL;
		}
		len = _this->maxLength;
		while (len < size) {
			len <<= 1;
		}
		newContent = fy_mmAllocate(block, len * sizeof(fy_char), exception);
		FYEH()NULL;
		memcpy(newContent, _this->content, _this->length << 1);
		fy_mmFree(block, _this->content);
		_this->content = newContent;
		_this->maxLength = len;
	}
	return _this;
}

static fy_str *fy_strAppendPriv(fy_memblock *block, fy_str *_this,
		fy_char *from, fy_int length, fy_exception *exception) {

	if (_this == NULL || from == NULL) {
		fy_fault(exception, NULL, "NPT");
		return NULL;
	}
	ensureSize(block, _this, _this->length + length, exception);
	FYEH()NULL;
	memcpy(_this->content + _this->length, from, length << 1);
	_this->length += length;
	_this->status &= ~FY_STR_HASHED;
	return _this;
}

FY_ATTR_EXPORT fy_str *fy_strEnsureSize(fy_memblock *block, fy_str *_this,
		fy_int size, fy_exception *exception) {
	if (_this->status & FY_STR_PERSIST) {
		fy_fault(NULL, NULL, "Logic error: try to clear a persisted string");
		return NULL;
	}
	return ensureSize(block, _this, size, exception);
}

FY_ATTR_EXPORT fy_str *fy_strAppendChar(fy_memblock *block, fy_str *_this,
		fy_char ch, fy_exception *exception) {
	if (_this->status & FY_STR_PERSIST) {
		fy_fault(NULL, NULL, "Logic error: try to clear a persisted string");
		return NULL;
	}
	return fy_strAppendPriv(block, _this, &ch, 1, exception);
}

FY_ATTR_EXPORT fy_str *fy_strAppend(fy_memblock *block, fy_str *_this,
		const fy_str *string, fy_exception *exception) {
	if (_this->status & FY_STR_PERSIST) {
		fy_fault(NULL, NULL, "Logic error: try to clear a persisted string");
		return NULL;
	}
	return fy_strAppendPriv(block, _this, string->content, string->length,
			exception);
}

FY_ATTR_EXPORT fy_str *fy_strAppendUTF8(fy_memblock *block, fy_str *_this,
		const char *utf8, fy_int size, fy_exception *exception) {
	const char *inbuf = utf8;
	fy_char outbuf;
	fy_int sl = strlen(utf8);
	if (_this->status & FY_STR_PERSIST) {
		fy_fault(NULL, NULL, "Logic error: try to clear a persisted string");
		return NULL;
	}
	if (size > sl || size < 0) {
		size = sl;
	}
	while (size > 0) {
		outbuf = fy_utf8Read(&inbuf, &size);
		fy_strAppendPriv(block, _this, &outbuf, 1, exception);
		FYEH()NULL;
	}
	return _this;
}

FY_ATTR_EXPORT fy_str *fy_strSubstring(fy_memblock *block, fy_str *_this,
		fy_int begin, fy_int end) {
	int size = end - begin;
	int i;
	if (_this == NULL) {
		fy_fault(NULL, NULL, "Null pointer exception.");
		return NULL;
	}
	if (begin < 0 || end < 0 || end >= _this->length || begin > end) {
		fy_fault(NULL, NULL, "Index out of bound exception");
		return NULL;
	}
	if (_this->status & FY_STR_PERSIST) {
		fy_fault(NULL, NULL, "Logic error: try to clear a persisted string");
		return NULL;
	}
	_this->length = size;
	for (i = 0; i < size; i++) {
		_this->content[i] = _this->content[i + begin];
	}
	_this->status &= ~FY_STR_HASHED;
	return _this;
}

FY_ATTR_EXPORT fy_uint fy_strUtf8Count(fy_str *str) {
	fy_uint size = 0;
	int i, count;
	count = str->length;
	for (i = 0; i < count; i++) {
		size += fy_utf8Size(str->content[i]);
	}
	return size;
}

FY_ATTR_EXPORT int fy_strCmp(fy_str *left, fy_str *right) {
	int resultWhenEqual =
			left->length == right->length ?
					0 : (left->length > right->length ? 1 : -1);
	int len = left->length > right->length ? right->length : left->length;
	int i;
	fy_char *lc = left->content;
	fy_char *rc = right->content;
	int ret;
	for (i = 0; i < len; i++) {
		ret = lc[i] - rc[i];
		if (ret != 0) {
			return ret;
		}
	}
	return resultWhenEqual;
}

FY_ATTR_EXPORT fy_boolean fy_strEndsWith(fy_str *_this, fy_str *right) {
	int delta;
	int i;
	if (_this->length < right->length) {
		return FALSE;
	}
	delta = _this->length - right->length;
	i = 0;
	for (i = right->length - 1; i >= 0; i--) {
		if (right->content[i] != _this->content[i + delta]) {
			return FALSE;
		}
	}
	return TRUE;
}
FY_ATTR_EXPORT void fy_strClear(fy_str *_this) {
	if (_this->status & FY_STR_PERSIST) {
		fy_fault(NULL, NULL, "Logic error: try to clear a persisted string");
		return;
	}
	_this->length = 0;
}

FY_ATTR_EXPORT fy_str *fy_strReplaceOne(fy_str *str, fy_char from, fy_char to) {
	int i;
	if (str->status & FY_STR_PERSIST) {
		fy_fault(NULL, NULL, "Logic error: try to clear a persisted string");
		return NULL;
	}
	for (i = str->length - 1; i >= 0; i--) {
		if (str->content[i] == from) {
			str->content[i] = to;
		}
	}
	str->status &= ~FY_STR_HASHED;
	return str;
}

FY_ATTR_EXPORT char *fy_strSPrint(char *target, size_t targetSize, fy_str *str) {
	int i, count;
	fy_char unicode;
	char *tmp;
	fy_int left;
	tmp = target;
	left = targetSize;
	count = str->length;
	for (i = 0; i < count; i++) {
		unicode = str->content[i];
		if (fy_utf8Write(unicode, &tmp, &left) <= 0) {
			break;
		}
	}
	if (left > 0) {
		*tmp = 0;
	} else {
		target[targetSize - 1] = 0;
	}
	return target;
}

FY_ATTR_EXPORT fy_str *fy_strCreateClone(fy_memblock *block, fy_str *from,
		fy_exception *exception) {
	fy_str *_this = fy_mmAllocate(block, sizeof(fy_str), exception);
	FYEH()NULL;
	fy_strInit(block, _this, from->length, exception);
	FYEH()NULL;
	fy_strAppend(block, _this, from, exception);
	FYEH()NULL;
	_this->status |= FY_STR_HASHED;
	_this->hashCode = from->hashCode;
	return _this;
}

static fy_uint hash(fy_str *key) {
	int i, imax;
	fy_uint ret = 0;
	for (i = 0, imax = key->length; i < imax; i++) {
		ret = (ret << 5) + (ret << 2) + ret + key->content[i];
	}
	return ret;
}

FY_ATTR_EXPORT fy_uint fy_strHash(fy_str *str) {
	if (str->status & FY_STR_HASHED) {
		return str->hashCode;
	} else {
		str->status |= FY_STR_HASHED;
		return str->hashCode = hash(str);
	}
}

FY_ATTR_EXPORT fy_char fy_strGet0(fy_str *str, fy_int pos) {
	return str->content[pos];
}

union varStorage {
	char c;
	char* a;
	fy_str *s;
};

FY_ATTR_EXPORT fy_str *fy_strCreatePermPersist(fy_memblock *mem,
		fy_exception *exception, const char *pattern, ...) {

	va_list arg_ptr;
	union varStorage vs[16];
	char c;
	fy_str *str;
	fy_int i = 0, patternLen, size = 0;

	if (strlen(pattern) > 16) {
		fy_fault(exception, NULL, "Pattern %s too long", pattern);
		return NULL;
	}

	va_start(arg_ptr, pattern);
	while ((c = pattern[i]) != 0) {
		switch (c) {
		case 'c':
			/*(one char)*/
			vs[i].c = va_arg(arg_ptr,int);
			size += 1;
			break;
		case 'a':
			/*(char*)*/
			vs[i].a = va_arg(arg_ptr,char*);
			size += strlen(vs[i].a);
			break;
		case 's':
			/*(fy_str*)*/
			vs[i].s = va_arg(arg_ptr,fy_str*);
			size += vs[i].s->length;
			break;
		default:
			fy_fault(exception, NULL, "Illegal character %c in pattern %s", c,
					pattern);
			return NULL;
		}
		i++;
	}
	va_end(arg_ptr);
	str = fy_strCreatePerm(mem, size, exception);
	FYEH()NULL;
	patternLen = i;
	for (i = 0; i < patternLen; i++) {
		switch (c) {
		case 'c':
			/*(one char)*/
			fy_strAppendChar(mem, str, vs[i].c, exception);
			FYEH()NULL;
			break;
		case 'a':
			/*(char*)*/
			fy_strAppendUTF8(mem, str, vs[i].a, -1, exception);
			FYEH()NULL;
			break;
		case 's':
			/*(fy_str*)*/
			fy_strAppend(mem, str, vs[i].s, exception);
			FYEH()NULL;
			break;
		default:
			fy_fault(exception, NULL, "Illegal character %c in pattern %s", c,
					pattern);
			return NULL;
		}
	}
	str->status |= FY_STR_PERSIST;
	return str;
}

FY_ATTR_EXPORT fy_str *fy_strCreatePermPersistSubstring(fy_memblock *mem,
		fy_str *from, int begin, int end, fy_exception *exception) {
	fy_str *str;
	if (begin < 0) {
		begin = 0;
	}
	if (end < 0) {
		end = from->length;
	}
	if (end < begin || end > from->length) {
		fy_fault(exception, NULL, "Index out of bound exception");
		return NULL;
	}
	if (from->status & FY_STR_PERSIST) {
		str = fy_strCreatePerm(mem, 0, exception);
		FYEH()NULL;
		str->content = from->content + begin;
		str->length = end - begin;
		str->maxLength = end - begin;
		str->status = FY_STR_PERM & FY_STR_PERSIST;
	} else {
		fy_fault(exception, NULL,
				"Can't make persist substring from non-persisted string.");
		return NULL;
	}
	return str;
}

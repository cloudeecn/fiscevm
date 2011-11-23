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

#include "fyc/String.h"

fy_str *fy_strInit(fy_context *context, fy_str *_this, fy_int size) {
	if (_this->content != NULL) {
		fy_fault("duplicated initJavaString");
	}
	_this->length = 0;
	_this->maxLength = size;
	_this->content = fy_vmAllocate(context, size << 1);
	_this->hashed = FALSE;
	return _this;
}

fy_str *fy_strNew(fy_context *context, const char *utf8) {
	fy_str *ret = fy_vmAllocate(context, sizeof(fy_str));
	fy_strInit(context, ret, fy_utf8SizeS(utf8, -1));
	fy_strAppendUTF8(context, ret, utf8, -1);
	return ret;
}

void fy_strDestroy(fy_context *context, fy_str *string) {
	fy_vmFree(context, string->content);
	string->length = 0;
	string->maxLength = 0;

}

fy_str *fy_strAllocateFromUTF8(fy_context *context, const char *utf8) {
	fy_str *ret;
	size_t size;
	size = fy_utf8SizeS(utf8, -1);
	ret = fy_vmAllocate(context, sizeof(fy_str));
	fy_strInit(context, ret, size);
	fy_strAppendUTF8(context, ret, utf8, -1);
	return ret;
}

static fy_str *fy_strEnsureSize(fy_context *context, fy_str *_this, fy_int size) {
	int len;
	fy_char *newContent;
	if (_this->maxLength < size) {

		len = _this->maxLength;
		while (len < size) {
			len <<= 1;
		}
		newContent = fy_vmAllocate(context, len * sizeof(fy_char));
		memcpy(newContent, _this->content, _this->length << 1);
		fy_vmFree(context, _this->content);
		_this->content = newContent;
		_this->maxLength = len;
	}
	return _this;
}

static fy_str *fy_strAppendPriv(fy_context *context, fy_str *_this,
		fy_char *from, fy_int length) {

	if (_this == NULL || from == NULL) {
		fy_fault("NPT");
	}
	fy_strEnsureSize(context, _this, _this->length + length);
	memcpy(_this->content + _this->length, from, length << 1);
	_this->length += length;
	_this->hashed = FALSE;
	return _this;
}

fy_str *fy_strAppendChar(fy_context *context, fy_str *_this, fy_char ch) {
	return fy_strAppendPriv(context, _this, &ch, 1);
}

fy_str *fy_strAppend(fy_context *context, fy_str *_this, const fy_str *string) {
	return fy_strAppendPriv(context, _this, string->content, string->length);
}

fy_str *fy_strAppendUTF8(fy_context *context, fy_str *_this, const char *utf8,
		fy_int size) {
	const char *inbuf = utf8;
	fy_char outbuf;
	fy_int sl = strlen(utf8);
	if (size > sl || size < 0) {
		size = sl;
	}
	while (size > 0) {
		outbuf = fy_utf8Read(&inbuf, &size);
		fy_strAppendPriv(context, _this, &outbuf, 1);
	}
	return _this;
}

fy_str *fy_strSubstring(fy_context *context, fy_str *_this, fy_int begin,
		fy_int end) {
	int size = end - begin;
	int i;
	if (_this == NULL) {
		fy_fault("Null pointer exception.");
	}
	if (begin < 0 || end < 0 || end >= _this->length || begin > end) {
		fy_fault("Index out of bound exception");
	}
	_this->length = size;
	for (i = 0; i < size; i++) {
		_this->content[i] = _this->content[i + begin];
	}
	_this->hashed = FALSE;
	return _this;
}

fy_uint fy_strUtf8Count(fy_str *str) {
	fy_uint size = 0;
	int i, count;
	count = str->length;
	for (i = 0; i < count; i++) {
		size += fy_utf8Size(str->content[i]);
	}
	return size;
}

int fy_strCmp(fy_str *left, fy_str *right) {
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

fy_boolean fy_strEndsWith(fy_str *_this, fy_str *right) {
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
void fy_strClear(fy_str *_this) {
	_this->length = 0;
}

fy_str *fy_strReplaceOne(fy_str *str, fy_char from, fy_char to) {
	int i;
	for (i = str->length - 1; i >= 0; i--) {
		if (str->content[i] == from) {
			str->content[i] = to;
		}
	}
	str->hashed = FALSE;
	return str;
}

char *fy_strSPrint(char *target, size_t targetSize, fy_str *str) {
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

fy_str *fy_strAllocateClone(fy_context *context, fy_str *from) {
	fy_str *_this = fy_vmAllocate(context, sizeof(fy_str));
	fy_strInit(context, _this, from->length);
	fy_strAppend(context, _this, from);
	_this->hashed = TRUE;
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

fy_uint fy_strHash(fy_str *str) {
	if (str->hashed) {
		return str->hashCode;
	} else {
		str->hashed = TRUE;
		return str->hashCode = hash(str);
	}
}

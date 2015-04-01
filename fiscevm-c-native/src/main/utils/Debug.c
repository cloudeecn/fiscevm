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
 * along with fiscevm. If not, see <http://www.gnu.org/licenses/>.
 */

#include "fy_util/Debug.h"
#include "fy_util/Utf8.h"
#include <stdarg.h>

static void fy_strPrint0(FILE *fp, const fy_str *str) {
	int i, count;
	fisce_char unicode;
	char buf[4];
	char *tmp;
	fisce_int left;
	count = str->length;
	for (i = 0; i < count; i++) {
		unicode = fy_strGet(str,i);
		tmp = buf;
		left = 3;
		fy_utf8Write(unicode, &tmp, &left);
		*tmp = 0;
		fprintf(fp, "%s", buf);
	}
}

FY_ATTR_EXPORT void fy_strFPrint(FILE *fp, const fy_str *str) {
	fy_strPrint0(fp, str);
}

FY_ATTR_EXPORT void fy_strPrint(const fy_str *str) {
	fy_strPrint0(stdout, str);
}

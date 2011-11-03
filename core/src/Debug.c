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

#include "fyc/Debug.h"

void fy_strPrint(fy_str *str) {
	int i, count;
	jchar unicode;
	char buf[4];
	char *tmp;
	jint left;
	count = str->length;
	for (i = 0; i < count; i++) {
		unicode = str->content[i];
		tmp = buf;
		left = 3;
		fy_utf8Write(unicode, &tmp, &left);
		*tmp = 0;
		printf("%s", buf);
	}
}

void varprintf(const char *format,...) {
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vfprintf(stdout,format,arg_ptr);
	va_end(arg_ptr);
	printf("\n");
	/*printf("%s:%d: %s\n",__FILE__ ,__LINE__,buf);*/
}

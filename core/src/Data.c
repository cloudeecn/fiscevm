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

#include "fyc/Data.h"

/*****************public*********************/
jubyte fy_dataRead(fy_data *data) {
	if (data->size < 1) {
		vm_die("Buffer overflow!");
	}
	data->size--;
	return *(data->data++);
}

jchar fy_dataRead2(fy_data *data) {
	int i;
	jchar ret = 0;
	if (data->size < 2) {
		vm_die("Buffer overflow!");
	}
	data->size -= 2;
	for (i = 0; i < 2; i++) {
		ret = (ret << 8) + (unsigned char) *(data->data++);
	}
	return ret;
}

juint fy_dataRead4(fy_data *data) {
	int i;
	juint ret = 0;
	if (data->size < 4) {
		vm_die("Buffer overflow!");
	}
	data->size -= 4;
	for (i = 0; i < 4; i++) {
		ret = (ret << 8) + (unsigned char) *(data->data++);
	}
	return ret;
}

julong fy_dataRead8(fy_data *data) {
	int i;
	julong ret = 0;
	if (data->size < 8) {
		vm_die("Buffer overflow!");
	}
	data->size -= 8;
	for (i = 0; i < 8; i++) {
		ret = (ret << 8) + (unsigned char) *(data->data++);
	}
	return ret;
}

void fy_dataSkip(fy_data *data, int size) {
	if (data->size < size) {
		vm_die("Buffer overflow!");
	}
	data->size -= size;
	data->data += size;
}


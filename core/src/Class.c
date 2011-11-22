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
#include "fyc/Class.h"

jboolean fy_classIsSuperClassOf(fy_context *context, fy_class *this,
		fy_class *other) {
	if (this == other) {
		return FALSE;
	}
	return fy_classCanCastTo(context, other, this);
}

jboolean fy_classCanCastTo(fy_context *context, fy_class *this,
		fy_class *other) {
	fy_class **interfaces;
	int i, max;
	if (this == other) {
		return TRUE;
	} else {
		switch (this->type) {
		case obj: {
			interfaces = this->interfaces;
			for (i = 0, max = this->interfacesCount; i < max; i++) {
				if (other == interfaces[i]) {
					return TRUE;
				}
			}

			if (this->super != NULL
					&& fy_classCanCastTo(context, this->super, other)) {
				return TRUE;
			}
		}
			break;
		case arr: {
			if (other == context->TOP_CLASS) {
				return 1;
			} else if (arr == other->type) {
				fy_class *thisContent = this->ci.arr.contentClass;
				fy_class *otherContent = other->ci.arr.contentClass;
				if (thisContent != NULL && otherContent != NULL) {
					return fy_classCanCastTo(context, thisContent, otherContent);
				} else {
					return FALSE;
				}
			} else {
				return FALSE;
			}
		}
			break;
		case prm:
			return FALSE;
		}
		return FALSE;
	}
}

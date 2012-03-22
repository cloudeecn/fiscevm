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

fy_boolean fy_classIsSuperClassOf(fy_context *context, fy_class *this,
		fy_class *other) {
	if (this == other) {
		return FALSE;
	}
	return fy_classCanCastTo(context, other, this);
}

fy_boolean fy_classCanCastTo(fy_context *context, fy_class *this,
		fy_class *other) {
	fy_class **interfaces;
	int i, max;
#ifdef FY_VERBOSE
	printf("##CAST ");
	fy_strPrint(this->className);
	printf("  -->  ");
	fy_strPrint(other->className);
#endif
	if (this == other) {
#ifdef FY_VERBOSE
		printf("=TRUE\n");
#endif
		return TRUE;
	} else {
#ifdef FY_VERBOSE
		if (fy_strCmp(this->className, other->className) == 0) {
			printf("\n");
			fy_strPrint(this->className);
			printf("\n");
			fy_fault(NULL, NULL, "Same class name refers different class!");
		}
#endif
		switch (this->type) {
		case obj: {
			interfaces = this->interfaces;
			for (i = 0, max = this->interfacesCount; i < max; i++) {
				if (other == interfaces[i]) {
#ifdef FY_VERBOSE
					printf("=TRUE\n");
#endif
					return TRUE;
				}
			}

			if (this->super != NULL
					&& fy_classCanCastTo(context, this->super, other)) {
#ifdef FY_VERBOSE
				printf("=TRUE\n");
#endif
				return TRUE;
			}
		}
			break;
		case arr: {
			if (other == context->TOP_CLASS) {
#ifdef FY_VERBOSE
				printf("=TRUE\n");
#endif
				return TRUE;
			} else if (arr == other->type) {
				fy_class *thisContent = this->ci.arr.contentClass;
				fy_class *otherContent = other->ci.arr.contentClass;
				if (thisContent != NULL && otherContent != NULL) {
#ifdef FY_VERBOSE
					printf("PENDING\n|--");
#endif
					return fy_classCanCastTo(context, thisContent, otherContent);
				} else {
#ifdef FY_VERBOSE
					printf("=FALSE\n");
#endif
					return FALSE;
				}
			} else {
#ifdef FY_VERBOSE
				printf("=FALSE\n");
#endif
				return FALSE;
			}
		}
			break;
		case prm:
#ifdef FY_VERBOSE
			printf("=FALSE\n");
#endif
			return FALSE;
		}
#ifdef FY_VERBOSE
		printf("=FALSE\n");
#endif
		return FALSE;
	}
}

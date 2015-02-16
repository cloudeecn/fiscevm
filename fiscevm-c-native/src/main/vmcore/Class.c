/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of fiscevm
 *
 *fiscevmis free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 *fiscevmis distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with fiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fyc/Class.h"
#include "fy_util/String.h"
#include "fy_util/Debug.h"

#if 0
# ifndef FY_CLASS_VERBOSE
#  define FY_CLASS_VERBOSE
# endif
#endif

fy_boolean fy_classIsSuperClassOf(fy_context *context, fy_class *this,
		fy_class *other) {
	if (this == other) {
		return FALSE;
	}
	return fy_classCanCastTo(context, other, this, TRUE);
}

fy_boolean fy_classCanCastTo(fy_context *context, fy_class *this,
		fy_class *other, fy_boolean processInterface) {
	fy_class **interfaces;
	fy_class *intf;
	int i, max;
#ifdef FY_CLASS_VERBOSE
	context->logDVar(context,"##CAST ");
	context->logDStr(context,this->className);
	context->logDVar(context,"  -->  ");
	context->logDStr(context,other->className);
#endif
	if (this == other) {
#ifdef FY_CLASS_VERBOSE
		context->logDVar(context,"=TRUE\n");
#endif
		return TRUE;
	} else {
#ifdef FY_CLASS_VERBOSE
		if (fy_strCmp(this->className, other->className) == 0) {
			context->logDVar(context,"\n");
			context->logDStr(context,this->className);
			context->logDVar(context,"\n");
			fy_fault(NULL, NULL, "Same class name refers different class!");
		}
#endif
		switch (this->type) {
		case object_class: {
			if (processInterface) {
				interfaces = this->interfaces;
				for (i = 0, max = this->interfacesCount; i < max; i++) {
					intf = interfaces[i];
					if (fy_classCanCastTo(context, intf, other,
							processInterface)) {
#ifdef FY_CLASS_VERBOSE
						context->logDVar(context,"=TRUE\n");
#endif
						return TRUE;
					}
				}
			}

			if (this->super != NULL
					&& fy_classCanCastTo(context, this->super, other,
							processInterface)) {
#ifdef FY_CLASS_VERBOSE
				context->logDVar(context,"=TRUE\n");
#endif
				return TRUE;
			}
		}
			break;
		case array_class: {
			if (other == context->TOP_CLASS) {
#ifdef FY_CLASS_VERBOSE
				context->logDVar(context,"=TRUE\n");
#endif
				return TRUE;
			} else if (array_class == other->type) {
				fy_class *thisContent = this->ci.arr.contentClass;
				fy_class *otherContent = other->ci.arr.contentClass;
				if (thisContent != NULL && otherContent != NULL) {
#ifdef FY_CLASS_VERBOSE
					context->logDVar(context,"PENDING\n|--");
#endif
					return fy_classCanCastTo(context, thisContent, otherContent,
							processInterface);
				} else {
#ifdef FY_CLASS_VERBOSE
					context->logDVar(context,"=FALSE\n");
#endif
					return FALSE;
				}
			} else {
#ifdef FY_CLASS_VERBOSE
				context->logDVar(context,"=FALSE\n");
#endif
				return FALSE;
			}
		}
			break;
		case primitive_class:
#ifdef FY_CLASS_VERBOSE
			context->logDVar(context,"=FALSE\n");
#endif
			return FALSE;
		}
#ifdef FY_CLASS_VERBOSE
		context->logDVar(context,"=FALSE\n");
#endif
		return FALSE;
	}
}

static fy_boolean fy_classCanCastToWithNull(fy_context *context,
		fy_class *clazz, fy_class *other) {
	return other == NULL ?
	FALSE :
							fy_classCanCastTo(context, clazz, other, FALSE);
}

fy_boolean fy_classExtendsThrowable(fy_context *context, fy_class *clazz) {
	return fy_classCanCastToWithNull(context, clazz, context->TOP_THROWABLE);
}

fy_boolean fy_classExtendsAnnotation(fy_context *context, fy_class *clazz) {
	return fy_classCanCastToWithNull(context, clazz, context->TOP_ANNOTATION);
}

fy_boolean fy_classExtendsEnum(fy_context *context, fy_class *clazz) {
	return fy_classCanCastToWithNull(context, clazz, context->TOP_ENUM);
}

fy_boolean fy_classExtendsSoftRef(fy_context *context, fy_class *clazz) {
	return fy_classCanCastToWithNull(context, clazz, context->TOP_SOFT_REF);
}

fy_boolean fy_classExtendsWeakRef(fy_context *context, fy_class *clazz) {
	return fy_classCanCastToWithNull(context, clazz, context->TOP_WEAK_REF);
}

fy_boolean fy_classExtendsPhantomRef(fy_context *context, fy_class *clazz) {
	return fy_classCanCastToWithNull(context, clazz, context->TOP_PHANTOM_REF);
}

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

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fy_util/Debug.h"
#include "fyc/Debug.h"
#include "fyc/VMContext.h"
#include <stdarg.h>

static void stdoutLogStr(fy_context *context, const fy_str *str) {
	fy_strFPrint(stdout, str);
}

static void stdoutLogVar(fy_context *context, const char *format, ...) {
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vfprintf(stdout, format, arg_ptr);
	va_end(arg_ptr);
}

static void stdoutLogVarLn(fy_context *context, const char *format, ...) {
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vfprintf(stdout, format, arg_ptr);
	va_end(arg_ptr);
	fprintf(stdout, "\n");
}

static void stderrLogStr(fy_context *context, const fy_str *str) {
	fy_strFPrint(stderr, str);
}

static void stderrLogVar(fy_context *context, const char *format, ...) {
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vfprintf(stderr, format, arg_ptr);
	va_end(arg_ptr);
}

static void stderrLogVarLn(fy_context *context, const char *format, ...) {
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vfprintf(stderr, format, arg_ptr);
	va_end(arg_ptr);
	fprintf(stderr, "\n");
}
#ifndef FY_DEBUG
static void nullLogStr(fy_context *context, const fy_str *str) {
}

static void nullLogVar(fy_context *context, const char *format, ...) {
}

static void nullLogVarLn(fy_context *context, const char *format, ...) {
}
#endif

FY_ATTR_EXPORT void fy_debugInit(fy_context *context) {
	context->logEStr = stderrLogStr;
	context->logEVar = stderrLogVar;
	context->logEVarLn = stderrLogVarLn;

	context->logWStr = stderrLogStr;
	context->logWVar = stderrLogVar;
	context->logWVarLn = stderrLogVarLn;

	context->logIStr = stdoutLogStr;
	context->logIVar = stdoutLogVar;
	context->logIVarLn = stdoutLogVarLn;

#if defined(FY_DEBUG) || defined(FY_LOG_DEBUG)
	context->logDStr = stdoutLogStr;
	context->logDVar = stdoutLogVar;
	context->logDVarLn = stdoutLogVarLn;
#else
	context->logDStr = nullLogStr;
	context->logDVar = nullLogVar;
	context->logDVarLn = nullLogVarLn;
#endif
}

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
 * along with fiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fyc/CoreHandlers.h"
#include "fyc/Constants.h"
#include "fyc/Thread.h"

#include <math.h>
#include <float.h>

static fisce_int MathACos(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, acos(param));
	return ops;
}

static fisce_int MathASin(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, asin(param));
	return ops;
}

static fisce_int MathATan(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, atan(param));
	return ops;
}

static fisce_int MathATan2(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	double param2 = fisce_longToDouble(fy_I2TOL(args[2].uvalue,args[3].uvalue));
	fy_threadReturnDouble(args, atan2(param, param2));
	return ops;
}

static fisce_int MathCbrt(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, cbrt(param));
	return ops;
}

static fisce_int MathCeil(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, ceil(param));
	return ops;
}

static fisce_int MathCos(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, cos(param));
	return ops;
}

static fisce_int MathCosh(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, cosh(param));
	return ops;
}

static fisce_int MathExp(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, exp(param));
	return ops;
}

static fisce_int MathExpM1(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, expm1(param));
	return ops;
}

static fisce_int MathFloor(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, floor(param));
	return ops;
}

static fisce_int MathHypot(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	double param2 = fisce_longToDouble(fy_I2TOL(args[2].uvalue,args[3].uvalue));
	fy_threadReturnDouble(args, hypot(param, param2));
	return ops;
}

static fisce_int MathIEEERemainder(struct fy_context *context,
		struct fy_thread *thread, void *data, fisce_stack_item *args, fisce_int argsCount,
		fisce_int ops, fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	double param2 = fisce_longToDouble(fy_I2TOL(args[2].uvalue,args[3].uvalue));
	if (param2 == 0) {
		fy_threadReturnDouble(args, 0.0 / param2);
	} else {
		fy_threadReturnDouble(args,
				param - floor(param / param2) * param2);
	}
	return ops;
}

static fisce_int MathLog(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue, args[1].uvalue));
	fy_threadReturnDouble(args, log(param));
	return ops;
}

static fisce_int MathLog10(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, log10(param));
	return ops;
}

static fisce_int MathLog1p(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, log1p(param));
	return ops;
}

static fisce_int MathPow(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	double param2 = fisce_longToDouble(fy_I2TOL(args[2].uvalue,args[3].uvalue));
	fy_threadReturnDouble(args, pow(param, param2));
	return ops;
}

static fisce_int MathRint(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, rint(param));
	return ops;
}

static fisce_int MathSignum(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	if (param != param) {
		fy_threadReturnDouble(args, 0.0 / 0.0);
	} else {
		fy_threadReturnDouble(args,
				param > 0 ? 1.0 : (param < 0 ? -1.0 : 0.0));
	}
	return ops;
}

static fisce_int MathSignumf(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	float param = fisce_intToFloat(args[0].ivalue);
	if (param != param) {
		fy_threadReturnFloat(args, 0.0f / 0.0f);
	} else {
		fy_threadReturnFloat(args,
				param > 0 ? 1.0f : (param < 0 ? -1.0f : 0.0f));
	}
	return ops;
}

static fisce_int MathSin(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, sin(param));
	return ops;
}

static fisce_int MathSinh(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, sinh(param));
	return ops;
}

static fisce_int MathSqrt(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, sqrt(param));
	return ops;
}

static fisce_int MathTan(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, tan(param));
	return ops;
}

static fisce_int MathTanh(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	double param = fisce_longToDouble(fy_I2TOL(args[0].uvalue,args[1].uvalue));
	fy_threadReturnDouble(args, tanh(param));
	return ops;
}

static fisce_int MathUlp(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	/*TODO*/
	fy_threadReturnDouble(args, 0);
	return ops;
}

static fisce_int MathUlpf(struct fy_context *context, struct fy_thread *thread,
		void *data, fisce_stack_item *args, fisce_int argsCount, fisce_int ops,
		fisce_exception *exception) {
	/*TODO*/
	fy_threadReturnFloat(args, 0);
	return ops;
}

void fy_coreRegisterMathHandlers(fy_context *context, fisce_exception *exception) {
	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".acos.(D)D", NULL,
			MathACos, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".asin.(D)D", NULL,
			MathASin, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".atan.(D)D", NULL,
			MathATan, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".atan2.(DD)D", NULL,
			MathATan2, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".cbrt.(D)D", NULL,
			MathCbrt, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".ceil.(D)D", NULL,
			MathCeil, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".cos.(D)D", NULL, MathCos,
			exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".cosh.(D)D", NULL,
			MathCosh, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".exp.(D)D", NULL, MathExp,
			exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".expm1.(D)D", NULL,
			MathExpM1, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".floor.(D)D", NULL,
			MathFloor, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".hypot.(DD)D", NULL,
			MathHypot, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".IEEEremainder.(DD)D",
	NULL, MathIEEERemainder, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".log.(D)D", NULL, MathLog,
			exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".log10.(D)D", NULL,
			MathLog10, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".log1p.(D)D", NULL,
			MathLog1p, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".pow.(DD)D", NULL, MathPow,
			exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".rint.(D)D", NULL,
			MathRint, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".signum.(D)D", NULL,
			MathSignum, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".signum.(F)F", NULL,
			MathSignumf, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".sin.(D)D", NULL, MathSin,
			exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".sinh.(D)D", NULL,
			MathSinh, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".sqrt.(D)D", NULL,
			MathSqrt, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".tan.(D)D", NULL, MathTan,
			exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".tanh.(D)D", NULL,
			MathTanh, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".ulp.(D)D", NULL, MathUlp,
			exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_MATH".ulp.(F)F", NULL, MathUlpf,
			exception);
	FYEH();

}

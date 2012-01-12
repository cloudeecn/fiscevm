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

#include "fyc/CoreHandlers.h"
#include "fiscedev.h"
#include <math.h>
#include <float.h>

static void MathACos(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, acos(param));
}

static void MathASin(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, asin(param));
}

static void MathATan(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, atan(param));
}

static void MathATan2(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	double param2 = fy_longToDouble(fy_I2TOL(args[2],args[3]));
	fy_nativeReturnDouble(context, thread, atan2(param,param2));
}

static void MathCbrt(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, cbrt(param));
}

static void MathCeil(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, ceil(param));
}

static void MathCos(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, cos(param));
}

static void MathCosh(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, cosh(param));
}

static void MathExp(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, exp(param));
}

static void MathExpM1(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, expm1(param));
}

static void MathFloor(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, floor(param));
}

static void MathHypot(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	double param2 = fy_longToDouble(fy_I2TOL(args[2],args[3]));
	fy_nativeReturnDouble(context, thread, hypot(param,param2));
}

static void MathIEEERemainder(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	double param2 = fy_longToDouble(fy_I2TOL(args[2],args[3]));
	if (param2 == 0) {
		fy_nativeReturnDouble(context, thread, 0.0 / param2);
	} else {
		fy_nativeReturnDouble(context, thread,
				param-floor(param/param2)*param2);
	}
}

static void MathLog(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0], args[1]));
	fy_nativeReturnDouble(context, thread, log(param));
}

static void MathLog10(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, log10(param));
}

static void MathLog1p(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, log1p(param));
}

static void MathPow(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	double param2 = fy_longToDouble(fy_I2TOL(args[2],args[3]));
	fy_nativeReturnDouble(context, thread, pow(param,param2));
}

static void MathRint(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, rint(param));
}

static void MathSignum(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	if (param != param) {
		fy_nativeReturnDouble(context, thread, 0.0/0.0);
	} else {
		fy_nativeReturnDouble(context, thread, param>0?1.0:(param<0?-1.0:0.0));
	}
}

static void MathSignumf(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	float param = fy_intToFloat(args[0]);
	if (param != param) {
		fy_nativeReturnFloat(context, thread, 0.0f/0.0f);
	} else {
		fy_nativeReturnFloat(context, thread,
				param>0?1.0f:(param<0?-1.0f:0.0f));
	}
}

static void MathSin(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, sin(param));
}

static void MathSinh(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, sinh(param));
}

static void MathSqrt(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, sqrt(param));
}

static void MathTan(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, tan(param));
}

static void MathTanh(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	double param = fy_longToDouble(fy_I2TOL(args[0],args[1]));
	fy_nativeReturnDouble(context, thread, tanh(param));
}

static void MathUlp(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	/*TODO*/
	fy_nativeReturnDouble(context, thread, 0);
}

static void MathUlpf(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	/*TODO*/
	fy_nativeReturnFloat(context, thread, 0);
}

void fy_coreRegisterMathHandlers(fy_context *context, fy_exception *exception) {
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

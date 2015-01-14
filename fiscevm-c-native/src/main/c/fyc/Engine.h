/*
 * Engine.h
 *
 *  Created on: Dec 18, 2014
 *      Author: cloudee
 */

#ifndef FYC_ENGINE_H_
#define FYC_ENGINE_H_

#include "../fiscestu.h"

#ifdef	__cplusplus
extern "C" {
#endif

fy_engine_result fy_thread_runner_01(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception);

#if FY_ENGINE_COUNT >= 2
fy_engine_result fy_thread_runner_02(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception);
#endif

#if FY_ENGINE_COUNT >= 3
fy_engine_result fy_thread_runner_03(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception);
#endif

#if FY_ENGINE_COUNT >= 4
fy_engine_result fy_thread_runner_04(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception);
#endif

#if FY_ENGINE_COUNT >= 5
fy_engine_result fy_thread_runner_05(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception);
#endif

#if FY_ENGINE_COUNT >= 6
fy_engine_result fy_thread_runner_06(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception);
#endif

#if FY_ENGINE_COUNT >= 7
fy_engine_result fy_thread_runner_07(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception);
#endif

#if FY_ENGINE_COUNT >= 8
fy_engine_result fy_thread_runner_08(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception);
#endif
#ifdef	__cplusplus
}
#endif
#endif /* FYC_ENGINE_H_ */

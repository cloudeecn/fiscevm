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
#ifdef	__cplusplus
}
#endif
#endif /* FYC_ENGINE_H_ */

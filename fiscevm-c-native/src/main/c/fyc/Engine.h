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

fy_int fy_thread_runner_00(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception,
            fy_e2_label_holder **out_labels
);

#if FY_ENGINE_COUNT > 1
fy_int fy_thread_runner_01(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception,
            fy_e2_label_holder **out_labels
);
#endif

#if FY_ENGINE_COUNT > 2
fy_int fy_thread_runner_02(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception,
            fy_e2_label_holder **out_labels
);
#endif

#if FY_ENGINE_COUNT > 3
fy_int fy_thread_runner_03(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception,
            fy_e2_label_holder **out_labels
);
#endif

#if FY_ENGINE_COUNT > 4
fy_int fy_thread_runner_04(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception,
            fy_e2_label_holder **out_labels
);
#endif

#if FY_ENGINE_COUNT > 5
fy_int fy_thread_runner_05(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception,
            fy_e2_label_holder **out_labels
);
#endif

#if FY_ENGINE_COUNT > 6
fy_int fy_thread_runner_06(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception,
            fy_e2_label_holder **out_labels
);
#endif

#if FY_ENGINE_COUNT > 7
fy_int fy_thread_runner_07(struct fy_context *context,
			struct fy_thread *thread,
			struct fy_frame *frame,
			fy_int ops,
			fy_exception *exception,
            fy_e2_label_holder **out_labels
);
#endif
#ifdef	__cplusplus
}
#endif
#endif /* FYC_ENGINE_H_ */

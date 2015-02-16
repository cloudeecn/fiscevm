/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
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

#ifndef FYC_ENGINE_H_
#define FYC_ENGINE_H_

#include "fisce.h"
#include "fyc/Config.h"
#include "fyc/ClassStruct.h"
#include "fyc/VMContext.h"
#include "fyc/Thread.h"

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef FY_SUPPORT_THREADING
typedef void *fy_e2_label;
#else
typedef fy_int fy_e2_label;
#endif

#ifdef FY_INSTRUCTION_COUNT
typedef struct fy_instruction_count{
	fy_int op;
	fy_int count;
	fy_double entropy;
	fy_double entropyRev;
} fy_instruction_count;

typedef struct fy_instruction_pair_count{
	fy_int op1;
	fy_int op2;
	fy_int count;
} fy_instruction_pair_count;
#endif

typedef struct fy_repl_data{
	fy_int op;
	fy_int repl_count;
	fy_int *ops;
} fy_repl_data;

typedef struct fy_engine_repl_data{
	fy_repl_data *repl_data[MAX_INSTRUCTIONS];
	fy_hashMapI repl_count[1];
	fy_hashMapI repl_result[1];
#ifdef FY_INSTRUCTION_COUNT
	fy_instruction_count instructionCount[MAX_INSTRUCTIONS];
	fy_instruction_pair_count instructionPairCount[MAX_INSTRUCTIONS * MAX_INSTRUCTIONS];
	fy_int last_op;
#endif
} fy_engine_repl_data;

typedef struct fy_e2_label_holder {
	fy_e2_label label;
	fy_int op;
}fy_e2_label_holder;

typedef fy_int (*fy_engine_runner)(struct fy_context *context,
		struct fy_thread *thread,
		struct fy_frame *frame,
		fy_int ops,
		struct fy_exception *exception,
		fy_e2_label_holder **out_labels);

typedef struct fy_engine{
	fy_engine_runner runner;
	fy_engine_repl_data replData;
} fy_engine;

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

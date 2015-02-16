/**
 *  Copyright 2010-2014 Yuxuan Huang. All rights reserved.
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

#include "fyc/Engine.h"

#include "fisceprt.h"
#include "fiscestu.h"
#include "fyc/Class.h"
#include "fyc/Heap.h"
#include "fyc/VMContext.h"
#include "fyc/Instructions.h"

#ifdef FY_VERBOSE
#define VM_DEBUG
#endif

#if 0
# define ASM_CHECK
#endif

#if 0
# define ASM_SHOW
#endif

#ifndef REPL_MIN
# define REPL_MIN 5000
#endif

#ifdef FY_INSTRUCTION_COUNT
# define DEBUG_REPL 1
#else
# define DEBUG_REPL 0
#endif

#define vm_fy_stack_item2i(_cell,_x)	((_x)=(_cell).uvalue)
#define vm_i2fy_stack_item(_x,_cell)	((_cell).uvalue=(_x))
#define vm_fy_stack_item2f(_cell,_x)	((_x)=(_cell).fvalue)
#define vm_f2fy_stack_item(_x,_cell)	((_cell).fvalue=(_x))
#define vm_twofy_stack_item2l(_cell1, _cell2, _x) ((_x)=fy_I2TOL((_cell1).uvalue, (_cell2).uvalue));
#define vm_twofy_stack_item2d(_cell1, _cell2, _x) ((_x)=fy_longToDouble(fy_I2TOL((_cell1).uvalue, (_cell2).uvalue)));
#define vm_l2twofy_stack_item(_x, _cell1, _cell2) (_cell1).uvalue = (fy_uint)(((fy_ulong)(_x))>>32); (_cell2).uvalue = (fy_uint)(_x);
#define vm_d2twofy_stack_item(_x, _cell1, _cell2) vm_l2twofy_stack_item(fy_doubleToLong(_x), (_cell1), (_cell2));

/* for future extensions */
#define IMM_ARG(access,value)		(access)

/* mini type-specific support functions */
#define printarg_i(_x) fprintf(vm_out, "%"FY_PRINT32"d", _x)
#define printarg_f(_x) fprintf(vm_out, "%f", _x)
#define printarg_l(_x) fprintf(vm_out, "%"FY_PRINT64"d", _x)
#define printarg_d(_x) fprintf(vm_out, "%f", _x)

/* #define USE_spTOS 1 */

#ifdef FY_USE_TOS
#define IF_sppTOS(x) x
#else
#define IF_sppTOS(x)
#define sppTOS spp[-1]
#endif

#ifdef ASM_SHOW
# define NAME(_x) puts(_x);
#else
# ifdef VM_DEBUG
#  define NAME(_x) if (vm_debug) {fprintf(vm_out, "T%d: %5d %p: %-13s, ", thread->threadId, FY_PDIFF(fy_instruction, PCURR_INST, instructions), PCURR_INST, _x); fprintf(vm_out,"spp=%p, sp=%5d ", spp, FY_PDIFF(fy_stack_item, spp, thread->stack));}
# else
#  define NAME(_x)
# endif
#endif

#ifdef ASM_CHECK
# define TRAP ({if(likely(exception != NULL)){*((volatile char*)0) = 1;}})
#else
# define TRAP
#endif

#ifdef FY_INSTRUCTION_COUNT
# define INST_COUNT(ipp) ({ \
	context->engines[FY_ENGINE_NUM].replData.instructionPairCount[context->engines[FY_ENGINE_NUM].replData.last_op * MAX_INSTRUCTIONS + ipp->op].op1 = context->engines[FY_ENGINE_NUM].replData.last_op; \
	context->engines[FY_ENGINE_NUM].replData.instructionPairCount[context->engines[FY_ENGINE_NUM].replData.last_op * MAX_INSTRUCTIONS + ipp->op].op2 = ipp->op; \
	context->engines[FY_ENGINE_NUM].replData.instructionPairCount[context->engines[FY_ENGINE_NUM].replData.last_op * MAX_INSTRUCTIONS + ipp->op].count++; \
	context->engines[FY_ENGINE_NUM].replData.last_op = \
	context->engines[FY_ENGINE_NUM].replData.instructionCount[ipp->op].op = ipp->op; \
	context->engines[FY_ENGINE_NUM].replData.instructionCount[ipp->op].count++; \
})
#else
# define INST_COUNT(ipp)
#endif

#if defined(FY_STRICT_CHECK) || defined(FY_INSTRUCTION_COUNT)
# define MODIFY_CURR_INST(OP) ({CURR_INST.inst=INST_OF(OP);method->instruction_ops[FY_PDIFF(fy_instruction, PCURR_INST, instructions)]=CODE_OF(OP);CURR_INST.op=CODE_OF(OP);})
#else
# define MODIFY_CURR_INST(OP) ({CURR_INST.inst=INST_OF(OP);method->instruction_ops[FY_PDIFF(fy_instruction, PCURR_INST, instructions)]=CODE_OF(OP);})
#endif
/* different threading schemes for different architectures; the sparse
 numbering is there for historical reasons */

/* here you select the threading scheme; I have only set this up for
 386 and generic, because I don't know what preprocessor macros to
 test for (Gforth uses config.guess instead).  Anyway, it's probably
 best to build them all and select the fastest instead of hardwiring
 a specific scheme for an architecture.  E.g., scheme 8 is fastest
 for Gforth "make bench" on a 486, whereas scheme 5 is fastest for
 "mini fib.mini" on an Athlon */
#ifndef THREADING_SCHEME
#define THREADING_SCHEME 8
#endif /* defined(THREADING_SCHEME) */

#ifdef FY_SUPPORT_THREADING
#if THREADING_SCHEME==1
/* direct threading scheme 1: autoinc, long latency (HPPA, Sharc) */
#  define USE_CFA 1
#  define ENGINE_ENTER ({SET_IP(frame->lpc += frame->pcofs); frame->pcofs = 0; NEXT_P1; spp = frame->baseSpp + method->instruction_extras[frame->lpc += frame->pcofs].sp; TRAP; NEXT_P2;})
#  define NEXT_P0	({cfa=(ipp++)->inst; INST_COUNT(PCURR_INST);})
#  define IP		(ipp-1)
#  define PCURR_INST		(ipp-2)
#  define CURR_INST		(*(ipp-2))
#  define SET_IP(p)	({ipp=instructions + (p); NEXT_P0;})
#  define NEXT_INST	(*(ipp-1))
#  define DEF_CA
#  define NEXT_P1	({})
#  define NEXT_P2	({goto *cfa;})
#endif

#if THREADING_SCHEME==3
/* direct threading scheme 3: autoinc, low latency (68K) */
#  define USE_CFA 1
#  define ENGINE_ENTER ({SET_IP(frame->lpc += frame->pcofs); frame->pcofs = 0; NEXT_P1; spp = frame->baseSpp + method->instruction_extras[frame->lpc += frame->pcofs].sp; TRAP; NEXT_P2;})
#  define NEXT_P0   ({INST_COUNT(PCURR_INST);})
#  define IP		(ipp)
#  define PCURR_INST		(ipp - 1)
#  define CURR_INST		(*(ipp - 1))
#  define SET_IP(p)	({ipp=instructions + (p); NEXT_P0;})
#  define NEXT_INST	(*ipp)
#  define DEF_CA
#  define NEXT_P1	({cfa=(ipp++)->inst;})
#  define NEXT_P2	({goto *cfa;})
#endif

#if THREADING_SCHEME==5
/* direct threading scheme 5: early fetching (Alpha, MIPS) */
#  define USE_CFA 1
#  define ENGINE_ENTER ({SET_IP(frame->lpc += frame->pcofs); frame->pcofs = 0; NEXT_P1; spp = frame->baseSpp + method->instruction_extras[frame->lpc += frame->pcofs].sp; TRAP; NEXT_P2;})
#  define CFA_NEXT
#  define NEXT_P0	({cfa=ipp->inst; INST_COUNT(PCURR_INST);})
#  define IP		(ipp)
#  define PCURR_INST		(ipp-1)
#  define CURR_INST		(*(ipp-1))
#  define SET_IP(p)	({ipp=instructions + (p); NEXT_P0; /*fprintf(vm_out, "\njumping to %"FY_PRINT32"d [%p]\n", cfa.op, cfa.inst); if(cfa.inst == NULL){fy_breakpoint();}*/})
#  define NEXT_INST	(*IP)
#  define DEF_CA
#  define NEXT_P1	({ipp++;})
#  define NEXT_P2	({goto *cfa;})
#endif

#if THREADING_SCHEME==8
/* direct threading scheme 8: i386 hack */
#  define ENGINE_ENTER ({SET_IP(frame->lpc += frame->pcofs); frame->pcofs = 0; NEXT_P1; spp = frame->baseSpp + method->instruction_extras[frame->lpc += frame->pcofs].sp; TRAP; NEXT_P2;})
#  define NEXT_P0 ({/*FY_PREFETCH(ipp);*/INST_COUNT(PCURR_INST);})
#  define IP		(ipp)
#  define PCURR_INST		(ipp-1)
#  define CURR_INST		(*(ipp-1))
#  define SET_IP(p)	({ipp=instructions + (p); NEXT_P0;})
#  define NEXT_INST	(*IP)
#  define DEF_CA
#  define NEXT_P1
#  define NEXT_P2	({goto *((ipp++)->inst);})
#endif

#if THREADING_SCHEME==9
/* direct threading scheme 9: prefetching (for PowerPC) */
/* note that the "cfa=next_cfa;" occurs only in NEXT_P1, because this
 works out better with the capabilities of gcc to introduce and
 schedule the mtctr instruction. */
#  define USE_CFA 1
#  define ENGINE_ENTER ({SET_IP(frame->lpc += frame->pcofs); frame->pcofs = 0; NEXT_P1; spp = frame->baseSpp + method->instruction_extras[frame->lpc += frame->pcofs].sp; TRAP; NEXT_P2;})
#  define NEXT_P0 ({INST_COUNT(PCURR_INST);})
#  define IP		ipp
#  define SET_IP(p)	({ipp=instructions + (p); next_cfa=ipp->inst; NEXT_P0;})
#  define PCURR_INST		(ipp-1)
#  define CURR_INST		(*(ipp-1))
#  define NEXT_INST	(*ipp)
#  define INC_IP(const_inc)	({next_cfa=IP[const_inc]; ip+=(const_inc);})
#  define DEF_CA
#  define NEXT_P1	({cfa=next_cfa; ipp++; next_cfa=ipp->inst;})
#  define NEXT_P2	({goto *cfa;})
#  define MORE_VARS	fy_e2_label next_cfa;
#endif

#if THREADING_SCHEME==10
/* direct threading scheme 10: plain (no attempt at scheduling) */
#  define USE_CFA
#  define REGISTER_CFA
#  define ENGINE_ENTER ({NEXT_P0; SET_IP(frame->lpc += frame->pcofs); frame->pcofs = 0; NEXT_P1; spp = frame->baseSpp + method->instruction_extras[frame->lpc += frame->pcofs].sp; TRAP; NEXT_P2;})
#  define NEXT_P0   ({})
#  define IP		(ipp)
#  define PCURR_INST		(ipp - 1)
#  define CURR_INST (*(ipp - 1))
#  define SET_IP(p)	({ipp=instructions + (p); NEXT_P0;})
#  define NEXT_INST	(*ipp)
#  define DEF_CA
#  define NEXT_P1	({INST_COUNT(ipp); cfa = (ipp++)->inst;})
#  define NEXT_P2	({goto *cfa;})
#endif

#define NEXT ({DEF_CA NEXT_P1; NEXT_P2;})
#define IPTOS ((NEXT_INST))

#define INST_ADDR(name) {&&I_##name , FY_OP_##name}
#define LABEL(name) I_##name:
#define INST_OF(name) (&&I_##name)
#define CODE_OF(name) (FY_OP_##name)

#else /* !defined(FY_SUPPORT_THREADING) */
/* use switch dispatch */
#define DEF_CA
#define NEXT_P0
#define NEXT_P1
#define NEXT_P2 goto next_inst;
#define SET_IP(p)	(ip=(p))
#define IP              ip
#define NEXT_INST	(*ip)
#define INC_IP(const_inc)	(ip+=(const_inc))
#define IPTOS NEXT_INST
#define INST_ADDR(name) I_##name
#define LABEL(name) case I_##name:

#endif /* !defined(FY_SUPPORT_THREADING) */

#define LABEL2(x)

#ifdef VM_PROFILING
#define SUPER_END  vm_count_block(IP)
#else
#define SUPER_END
#endif

#define SUPER_CONTINUE

#ifndef __GNUC__
enum {
#include "fisce-labels.i"
};
#endif

/**
 ####### #     #  #####    ###   #     # #######  #####
 #       ##    # #     #    #    ##    # #       #     #
 #       # #   # #          #    # #   # #             #
 #####   #  #  # #  ####    #    #  #  # #####    #####
 #       #   # # #     #    #    #   # # #       #
 #       #    ## #     #    #    #    ## #       #
 ####### #     #  #####    ###   #     # ####### #######
 */

#ifdef FY_SUPPORT_THREADING
# define FY_FALLOUT_INVOKE {goto I_dropout;}
# define FY_FALLOUT_NOINVOKE {fy_localToFrame(context, frame); goto I_dropout;}
#else
# define FY_FALLOUT_INVOKE {SET_IP(FY_IP_dropout);NEXT_P1;NEXT_P2;}
# define FY_FALLOUT_NOINVOKE {fy_localToFrame(context, frame); SET_IP(FY_IP_dropout);NEXT_P1;NEXT_P2;}
#endif

# define FY_OP_GOTO ({SET_IP(CURR_INST.params.int_params.param1);})
# define FY_CHECK_OPS(OPS) if(unlikely(OPS <= 0)){FY_FALLOUT_NOINVOKE}
# define FY_CHECK_OPS_INVOKE(OPS) if(unlikely(OPS <= 0)){FY_FALLOUT_INVOKE}
# define FY_CHECK_OPS_AND_GOTO(OPS) \
if(unlikely(OPS <= 0)){ \
	fy_localToFrame(context, frame); \
	frame->pcofs = CURR_INST.params.int_params.param1 - frame->lpc; \
	FY_FALLOUT_INVOKE; \
} else { \
	SET_IP(CURR_INST.params.int_params.param1); \
}

#ifdef FY_STRICT_CHECK

# define fy_checkPutLocal(P,SIZE) { \
	if((P)<0 || (P)+(SIZE)>=method->max_locals) {\
		fy_fault(exception,NULL,"Local var out of range %d/%d",(P),method->max_locals);\
		FY_THEH() \
	} \
}

# define fy_checkGetLocal(P,SIZE) { \
	if((P)<0 || (P)+(SIZE)>=method->max_locals) {\
		fy_fault(exception,NULL,"Local var out of range %d/%d",(P),method->max_locals);\
		FY_THEH() \
	} \
}

# define fy_checkCall(COUNT) \
	if (FY_PDIFF(fy_stack_item, spp, sbase) - (COUNT) < method->max_locals) { \
		fy_fault(exception,NULL,"Buffer underflow! sb=%d local=%d sp=%d count=%d", sb, localCount, sp, COUNT); \
		FY_THEH() \
	}

#else
# define fy_checkPutLocal(P,SIZE)
# define fy_checkGetLocal(P,SIZE)
# define fy_checkCall(COUNT)
#endif

#define fy_threadPutLocalInt(P,V) { \
	fy_checkPutLocal(P,0) \
	sbase[(P)].uvalue=(V); \
}

#define fy_threadPutLocalHandle(P,V) { \
	fy_checkPutLocal(P,0) \
	sbase[(P)].uvalue=(V); \
}

#define fy_threadPutLocalLong(P,W) { \
	fy_checkPutLocal(P,1) \
	sbase[(P)].uvalue=fy_HOFL(W); \
	sbase[(P)+1].uvalue=fy_LOFL(W); \
}

#define fy_threadGetLocalInt(P,O) { \
	fy_checkGetLocal(P,0) \
	O=sbase[(P)].uvalue; \
}

#define fy_threadGetLocalHandle(P,O) { \
	fy_checkGetLocal(P,0) \
	O=sbase[(P)].uvalue; \
}

#define fy_threadGetLocalLong(P,O) { \
	fy_checkGetLocal(P,1) \
	O=sbase[(P)].uvalue; \
	O=((fy_ulong)O<<32)+((fy_ulong)sbase[(P)+1].uvalue); \
}

#ifdef FY_VERBOSE
#define fy_localToFrame(context,ptrFrame) { \
	ptrFrame->lpc = FY_PDIFF(fy_instruction, PCURR_INST, instructions); \
	ptrFrame->pcofs = 1; \
	context->logDVar(context, " #frame[%p]->lpc=%"FY_PRINT32"d nextpc=%"FY_PRINT32"d\n", ptrFrame, ptrFrame->lpc, ptrFrame->lpc + ptrFrame->pcofs); \
}
#else
#define fy_localToFrame(context,ptrFrame) { \
	ptrFrame->lpc = FY_PDIFF(fy_instruction, PCURR_INST, instructions); \
	ptrFrame->pcofs = 1; \
}
#endif

#define FY_UPDATE_SP(context, ptrFrame) { \
	spp = ptrFrame->baseSpp + method->instruction_extras[ptrFrame->lpc + ptrFrame->pcofs].sp; \
}

#define FY_THEH(FINALLY) \
if(unlikely(exception->exceptionType != exception_none)){ \
	ops = 0; \
	FINALLY; \
	FY_FALLOUT_NOINVOKE; \
}

#define FY_ENGINE_CLINIT(CLASS, SPSS) { \
	ops = fy_threadClinit(context, thread, (CLASS), spp + (SPSS), ops, exception); \
	FY_THEH(); \
	FY_CHECK_OPS_INVOKE(ops); \
}

static fy_e2_label lookup_label(fy_e2_label_holder *labels, fy_int op) {
	fy_e2_label_holder *holder = labels;
	while (holder->op >= 0) {
		if (holder->op == op) {
			return holder->label;
		}
		holder++;
	}
	return (fy_e2_label) -1;
}

static void calc_repl(fy_context *context,
		fy_method *method, fy_int op, fy_instruction *currInst,
		fy_int engineNum, fy_e2_label_holder *labels,
		fy_exception *exception) {
#define PAIR_OF(OP, NEXT_OP) ((OP) * 1031 + (NEXT_OP))
    fy_engine_repl_data *engine_repl_data = &context->engines[engineNum].replData;
	fy_repl_data **repl_data = engine_repl_data->repl_data;
	fy_hashMapI *repl_count = engine_repl_data->repl_count;
	fy_hashMapI *repl_result = engine_repl_data->repl_result;
	fy_int nextOp;
	fy_int count;
	fy_int toOp;
	if (repl_data[op] != NULL) {
		nextOp = method->instruction_ops[FY_PDIFF(fy_instruction, currInst, method->instructions) + 1];
		if (nextOp < 0) {
            WLOG(context, "Can't find op from label %p in engine %d method %s ip %d", currInst[1].inst, engineNum, method->utf8Name, FY_PDIFF(fy_instruction, currInst + 1, method->instructions));
            nextOp = -1;
		} else if (repl_data[nextOp] == NULL) {
			toOp = fy_hashMapIGet(context->memblocks, repl_result,
					PAIR_OF(op, nextOp));
			if (toOp >= 0) {
				currInst->inst = lookup_label(labels, toOp);
#if DEBUG_REPL
				ILOG(
						context,
						"OP %s(%d / %p) (-> %s(%d)) replicated to %s(%d / %p) in method %s inst %p from cache",
						FY_OP_NAME[op], op, currInst->inst, FY_OP_NAME[nextOp], nextOp, FY_OP_NAME[toOp], toOp, currInst->inst, method->utf8Name, currInst)
				;					//
#endif
				method->instruction_ops[FY_PDIFF(fy_instruction, currInst, method->instructions)] = toOp;
#if defined(FY_STRICT_CHECK) || defined(FY_INSTRUCTION_COUNT)
				currInst->op = toOp;
#endif
				if (currInst->inst == (fy_e2_label) -1) {
					fy_fault(exception, NULL,
							"Can't find label for op %s(%d)",
							FY_OP_NAME[toOp], toOp);
					FYEH();
				}
			} else {
				count = fy_hashMapIInc(context->memblocks, repl_count,
						PAIR_OF(op, nextOp), 1, exception);
				FYEH();
				if (count > REPL_MIN) {
					toOp = repl_data[op]->ops[0];
#if DEBUG_REPL
					ILOG(
							context,
							"OP %s(%d / %p) (-> %s(%d)) replicated to %s(%d / %p) in method %s inst %p",
							FY_OP_NAME[op], op, currInst->inst, FY_OP_NAME[nextOp], nextOp, FY_OP_NAME[toOp], toOp, currInst->inst, method->utf8Name, currInst)
					;				//
#endif
					fy_hashMapIPut(context->memblocks, repl_result, PAIR_OF(op, nextOp), toOp, exception);
					FYEH();
					currInst->inst = lookup_label(labels, toOp);
					method->instruction_ops[FY_PDIFF(fy_instruction, currInst, method->instructions)] = toOp;
#if defined(FY_STRICT_CHECK) || defined(FY_INSTRUCTION_COUNT)
					currInst->op = toOp;
#endif
					if (currInst->inst == (fy_e2_label) -1) {
						fy_fault(exception, NULL,
								"Can't find label for op %s(%d)",
								FY_OP_NAME[toOp], toOp);
						FYEH();
					}
					if (repl_data[op]->repl_count > 1) {
						repl_data[op]->repl_count--;
						memmove(repl_data[op]->ops, repl_data[op]->ops + 1,
								sizeof(int) * repl_data[op]->repl_count);
					} else {
#if DEBUG_REPL
						ILOG(
								context, "WARN: REPL_OP %s overloaded", FY_OP_NAME[toOp])
						;						//
#endif
					}
				}
			}
		}
	}
}

#if REPL_MIN > 0
#define RCAL(OP) ({calc_repl(context, method, OP, PCURR_INST, FY_ENGINE_NUM, labels, exception);})
#else
#define RCAL(OP)
#endif

static fy_int opLDC(fy_context *context,
		fy_class *owner, fy_char index, fy_exception *exception) {
	ConstantStringInfo *constantStringInfo;
	ConstantClass *constantClass;
	fy_class *clazz;
	fy_int hvalue;
	switch (owner->constantTypes[index]) {
	case CONSTANT_Integer:
	case CONSTANT_Float:
		return ((ConstantIntegerFloatInfo*) (owner->constantPools[index]))->value;
	case CONSTANT_String:
		constantStringInfo =
				((ConstantStringInfo*) (owner->constantPools[index]));
		hvalue = fy_heapLookupStringFromConstant(context, constantStringInfo,
				exception);
		return hvalue;
	case CONSTANT_Class:
		constantClass = (ConstantClass*) (owner->constantPools[index]);
		clazz = fy_vmLookupClassFromConstant(context, constantClass, exception);
		if (exception->exceptionType != exception_none) {
			return 0;
		}
		return fy_vmGetClassObjHandle(context, clazz, exception);
	default:
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				FY_EXCEPTION_VM);
		sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"LDC type wrong! %"FY_PRINT32"d,%d", index,
				owner->constantTypes[index]);
		return 0;
	}
}

static fy_long opLDC2(fy_context *context,
		fy_class *owner, fy_char index, fy_exception *exception) {
	switch (owner->constantTypes[index]) {
	case CONSTANT_Double:
	case CONSTANT_Long:
		return ((ConstantLongDoubleInfo*) (owner->constantPools[index]))->value;
	default:
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				FY_EXCEPTION_VM);
		sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"LDC2 type wrong! %"FY_PRINT32"d,%d", index,
				owner->constantTypes[index]);
		return 0;
	}
}

#define FY_ENGINE_HEADER

#define FY_ENGINE_NUM 00
#include "fisce-vm.i"
#undef FY_ENGINE_NUM

#if FY_ENGINE_COUNT > 1
#define FY_ENGINE_NUM 01
#include "fisce-vm.i"
#undef FY_ENGINE_NUM
#endif

#if FY_ENGINE_COUNT > 2
#define FY_ENGINE_NUM 02
#include "fisce-vm.i"
#undef FY_ENGINE_NUM
#endif

#if FY_ENGINE_COUNT > 3
#define FY_ENGINE_NUM 03
#include "fisce-vm.i"
#undef FY_ENGINE_NUM
#endif

#if FY_ENGINE_COUNT > 4
#define FY_ENGINE_NUM 04
#include "fisce-vm.i"
#undef FY_ENGINE_NUM
#endif

#if FY_ENGINE_COUNT > 5
#define FY_ENGINE_NUM 05
#include "fisce-vm.i"
#undef FY_ENGINE_NUM
#endif

#if FY_ENGINE_COUNT > 6
#define FY_ENGINE_NUM 06
#include "fisce-vm.i"
#undef FY_ENGINE_NUM
#endif

#if FY_ENGINE_COUNT > 7
#define FY_ENGINE_NUM 07
#include "fisce-vm.i"
#undef FY_ENGINE_NUM
#endif

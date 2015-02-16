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

#include "fyc/Preverifier.h"
#include "fyc/Config.h"
#include "fyc/Constants.h"
#include "fyc/Instructions.h"

static fy_int code_length[255] = {
/* 0x00 OP_NOP */1,

/* 0x01 OP_ACONST_NULL */1,
/* 0x02 OP_ICONST_M1 */1,
/* 0x03 OP_ICONST_0 */1,
/* 0x04 OP_ICONST_1 */1,
/* 0x05 OP_ICONST_2 */1,
/* 0x06 OP_ICONST_3 */1,
/* 0x07 OP_ICONST_4 */1,
/* 0x08 OP_ICONST_5 */1,
/* 0x09 OP_LCONST_0 */1,
/* 0x0a OP_LCONST_1 */1,
/* 0x0b OP_FCONST_0 */1,
/* 0x0c OP_FCONST_1 */1,
/* 0x0d OP_FCONST_2 */1,
/* 0x0e OP_DCONST_0 */1,
/* 0x0f OP_DCONST_1 */1,

/* 0x10 OP_BIPUSH */2,
/* 0x11 OP_SIPUSH */3,

/* 0x12 OP_LDC */2,
/* 0x13 OP_LDC_W */3,
/* 0x14 OP_LDC2_W */3,

/* 0x15 OP_ILOAD */2,
/* 0x16 OP_LLOAD */2,
/* 0x17 OP_FLOAD */2,
/* 0x18 OP_DLOAD */2,
/* 0x19 OP_ALOAD */2,

/* 0x1a OP_ILOAD_0 */1,
/* 0x1b OP_ILOAD_1 */1,
/* 0x1c OP_ILOAD_2 */1,
/* 0x1d OP_ILOAD_3 */1,
/* 0x1e OP_LLOAD_0 */1,
/* 0x1f OP_LLOAD_1 */1,
/* 0x20 OP_LLOAD_2 */1,
/* 0x21 OP_LLOAD_3 */1,
/* 0x22 OP_FLOAD_0 */1,
/* 0x23 OP_FLOAD_1 */1,
/* 0x24 OP_FLOAD_2 */1,
/* 0x25 OP_FLOAD_3 */1,
/* 0x26 OP_DLOAD_0 */1,
/* 0x27 OP_DLOAD_1 */1,
/* 0x28 OP_DLOAD_2 */1,
/* 0x29 OP_DLOAD_3 */1,
/* 0x2a OP_ALOAD_0 */1,
/* 0x2b OP_ALOAD_1 */1,
/* 0x2c OP_ALOAD_2 */1,
/* 0x2d OP_ALOAD_3 */1,

/* 0x2e OP_IALOAD */1,
/* 0x2f OP_LALOAD */1,
/* 0x30 OP_FALOAD */1,
/* 0x31 OP_DALOAD */1,
/* 0x32 OP_AALOAD */1,
/* 0x33 OP_BALOAD */1,
/* 0x34 OP_CALOAD */1,
/* 0x35 OP_SALOAD */1,

/* 0x36 OP_ISTORE */2,
/* 0x37 OP_LSTORE */2,
/* 0x38 OP_FSTORE */2,
/* 0x39 OP_DSTORE */2,
/* 0x3a OP_ASTORE */2,

/* 0x3b OP_ISTORE_0 */1,
/* 0x3c OP_ISTORE_1 */1,
/* 0x3d OP_ISTORE_2 */1,
/* 0x3e OP_ISTORE_3 */1,
/* 0x3f OP_LSTORE_0 */1,
/* 0x40 OP_LSTORE_1 */1,
/* 0x41 OP_LSTORE_2 */1,
/* 0x42 OP_LSTORE_3 */1,
/* 0x43 OP_FSTORE_0 */1,
/* 0x44 OP_FSTORE_1 */1,
/* 0x45 OP_FSTORE_2 */1,
/* 0x46 OP_FSTORE_3 */1,
/* 0x47 OP_DSTORE_0 */1,
/* 0x48 OP_DSTORE_1 */1,
/* 0x49 OP_DSTORE_2 */1,
/* 0x4a OP_DSTORE_3 */1,
/* 0x4b OP_ASTORE_0 */1,
/* 0x4c OP_ASTORE_1 */1,
/* 0x4d OP_ASTORE_2 */1,
/* 0x4e OP_ASTORE_3 */1,

/* 0x4f OP_IASTORE */1,
/* 0x50 OP_LASTORE */1,
/* 0x51 OP_FASTORE */1,
/* 0x52 OP_DASTORE */1,
/* 0x53 OP_AASTORE */1,
/* 0x54 OP_BASTORE */1,
/* 0x55 OP_CASTORE */1,
/* 0x56 OP_SASTORE */1,

/* 0x57 OP_POP */1,
/* 0x58 OP_POP2 */1,

/* 0x59 OP_DUP */1,
/* 0x5a OP_DUP_X1 */1,
/* 0x5b OP_DUP_X2 */1,
/* 0x5c OP_DUP2 */1,
/* 0x5d OP_DUP2_X1 */1,
/* 0x5e OP_DUP2_X2 */1,

/* 0x5f OP_SWAP */1,

/* 0x60 OP_IADD */1,
/* 0x61 OP_LADD */1,
/* 0x62 OP_FADD */1,
/* 0x63 OP_DADD */1,
/* 0x64 OP_ISUB */1,
/* 0x65 OP_LSUB */1,
/* 0x66 OP_FSUB */1,
/* 0x67 OP_DSUB */1,
/* 0x68 OP_IMUL */1,
/* 0x69 OP_LMUL */1,
/* 0x6a OP_FMUL */1,
/* 0x6b OP_DMUL */1,
/* 0x6c OP_IDIV */1,
/* 0x6d OP_LDIV */1,
/* 0x6e OP_FDIV */1,
/* 0x6f OP_DDIV */1,
/* 0x70 OP_IREM */1,
/* 0x71 OP_LREM */1,
/* 0x72 OP_FREM */1,
/* 0x73 OP_DREM */1,
/* 0x74 OP_INEG */1,
/* 0x75 OP_LNEG */1,
/* 0x76 OP_FNEG */1,
/* 0x77 OP_DNEG */1,
/* 0x78 OP_ISHL */1,
/* 0x79 OP_LSHL */1,
/* 0x7a OP_ISHR */1,
/* 0x7b OP_LSHR */1,
/* 0x7c OP_IUSHR */1,
/* 0x7d OP_LUSHR */1,
/* 0x7e OP_IAND */1,
/* 0x7f OP_LAND */1,
/* 0x80 OP_IOR */1,
/* 0x81 OP_LOR */1,
/* 0x82 OP_IXOR */1,
/* 0x83 OP_LXOR */1,

/* 0x84 OP_IINC */3,

/* 0x85 OP_I2L */1,
/* 0x86 OP_I2F */1,
/* 0x87 OP_I2D */1,
/* 0x88 OP_L2I */1,
/* 0x89 OP_L2F */1,
/* 0x8a OP_L2D */1,
/* 0x8b OP_F2I */1,
/* 0x8c OP_F2L */1,
/* 0x8d OP_F2D */1,
/* 0x8e OP_D2I */1,
/* 0x8f OP_D2L */1,
/* 0x90 OP_D2F */1,
/* 0x91 OP_I2B */1,
/* 0x92 OP_I2C */1,
/* 0x93 OP_I2S */1,

/* 0x94 OP_LCMP */1,
/* 0x95 OP_FCMPL */1,
/* 0x96 OP_FCMPG */1,
/* 0x97 OP_DCMPL */1,
/* 0x98 OP_DCMPG */1,

/* 0x99 OP_IFEQ */3,
/* 0x9a OP_IFNE */3,
/* 0x9b OP_IFLT */3,
/* 0x9c OP_IFGE */3,
/* 0x9d OP_IFGT */3,
/* 0x9e OP_IFLE */3,
/* 0x9f OP_IF_ICMPEQ */3,
/* 0xa0 OP_IF_ICMPNE */3,
/* 0xa1 OP_IF_ICMPLT */3,
/* 0xa2 OP_IF_ICMPGE */3,
/* 0xa3 OP_IF_ICMPGT */3,
/* 0xa4 OP_IF_ICMPLE */3,
/* 0xa5 OP_IF_ACMPEQ */3,
/* 0xa6 OP_IF_ACMPNE */3,

/* 0xa7 OP_GOTO */3,

/* 0xa8 OP_JSR */3,
/* 0xa9 OP_RET */2,

/* 0xaa OP_TABLESWITCH */-1,
/* 0xab OP_LOOKUPSWITCH */-1,

/* 0xac OP_IRETURN */1,
/* 0xad OP_LRETURN */1,
/* 0xae OP_FRETURN */1,
/* 0xaf OP_DRETURN */1,
/* 0xb0 OP_ARETURN */1,
/* 0xb1 OP_RETURN */1,

/* 0xb2 OP_GETSTATIC */3,
/* 0xb3 OP_PUTSTATIC */3,
/* 0xb4 OP_GETFIELD */3,
/* 0xb5 OP_PUTFIELD */3,

/* 0xb6 OP_INVOKEVIRTUAL */3,
/* 0xb7 OP_INVOKESPECIAL */3,
/* 0xb8 OP_INVOKESTATIC */3,

/* 0xb9 OP_INVOKEINTERFACE */5,

/* oxba XXX_UNUSED_XXX */1,

/* 0xbb OP_NEW */3,
/* 0xbc OP_NEWARRAY */2,
/* 0xbd OP_ANEWARRAY */3,

/* 0xbe OP_ARRAYLENGTH */1,

/* 0xbf OP_ATHROW */1,

/* 0xc0 OP_CHECKCAST */3,
/* 0xc1 OP_INSTANCEOF */3,

/* 0xc2 OP_MONITORENTER */1,
/* 0xc3 OP_MONITOREXIT */1,

/* 0xc4 OP_WIDE */-1,

/* 0xc5 OP_MULTIANEWARRAY */4,

/* 0xc6 OP_IFNULL */3,
/* 0xc7 OP_IFNONNULL */3,

/* 0xc8 OP_GOTO_W */5,
/* 0xc9 OP_JSR_W */5,
/* 0xca BREADPOINT */1,

};

#ifdef FY_VERBOSE_XX
# define FY_VERBOSE_PREVERIFIER
#endif

static fy_int getIcFromPc(fy_context *context, fy_int pc,
		fy_hashMapI *tmpPcIcMap, fy_exception *exception) {
	fy_int ic = fy_hashMapIGet(context->memblocks, tmpPcIcMap, pc);
	if (ic < 0) {
		fy_fault(exception, NULL, "Can't find ic for pc=%d", pc);
		FYEH()-1;
	}
	return ic;
}
static void preverifyMethodExceptionTable(fy_context *context,
		fy_method *method, fy_hashMapI *tmpPcIcMap, fy_exception *exception) {
	fy_exceptionHandler *eh;
	fy_int i, imax;
	if ((imax = method->exception_table_length) == 0) {
		return;
	}
	for (i = 0; i < imax; i++) {
		eh = method->exception_table + i;
		eh->start_pc = getIcFromPc(context, eh->start_pc, tmpPcIcMap,
				exception);
		eh->end_pc = getIcFromPc(context, eh->end_pc, tmpPcIcMap, exception);
		eh->handler_pc = getIcFromPc(context, eh->handler_pc, tmpPcIcMap,
				exception);
		FYEH();
	}
}
static void preverifyMethodLineNumberTable(fy_context *context,
		fy_method *method, fy_hashMapI *tmpPcIcMap, fy_exception *exception) {
	fy_lineNumber *ln;
	fy_int i, imax;
	if ((imax = method->line_number_table_length) == 0) {
		return;
	}
	for (i = 0; i < imax; i++) {
		ln = method->line_number_table + i;
		ln->start_pc = getIcFromPc(context, ln->start_pc, tmpPcIcMap,
				exception);
		FYEH();
	}
}

struct read_stack_status {
	fy_uint frameNum;
	fy_uint sc;
	fy_uint pc;
	fy_uint type;
	fy_uint localSize;
	fy_instruction_extra tmp[1];
	fy_instruction_extra *lastFrame;
};

static fy_boolean readStackTypeOfs(fy_context *context, fy_stack_map_table *smt,
		struct read_stack_status *status, fy_method *method, fy_int pc,
		fy_exception *exception) {
	fy_uint type;
	if (status->sc >= smt->length) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
				"Can't verify %s, pc=%"FY_PRINT32"d: unexpected end of stack frame table",
				method->utf8Name, pc);
		return FALSE;
	} else {
		type = status->type = smt->entries[status->sc++];
		if (type < 64) { /*0-63 SAME*/
			status->pc += 1 + type;
#ifdef FY_VERBOSE_PREVERIFIER
			context->logDVarLn(context, "NEXT FRAME: SAME, pc=%d", status->pc);
#endif
		} else if (type < 128) { /*64-127 SAME_LOCALS_1_STACK_ITEM*/
			status->pc += 1 + type - 64;
#ifdef FY_VERBOSE_PREVERIFIER
			context->logDVarLn(context,
					"NEXT FRAME: SAME_LOCALS_1_STACK_ITEM, pc=%d", status->pc);
#endif
		} else if (type < 247) { /*ILLEGAL*/
			fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
					"Can't verify %s, pc=%"FY_PRINT32"d: illegal type %"FY_PRINT32"d",
					method->utf8Name, pc, type);
			return FALSE;
		} else if (type < 248) { /*247 SAME_LOCALS_1_STACK_ITEM_EXTENDED*/
			status->pc += 1
					+ (fy_B2TOUI(smt->entries[status->sc],
							smt->entries[status->sc + 1]));
			status->sc += 2;
#ifdef FY_VERBOSE_PREVERIFIER
			context->logDVarLn(context,
					"NEXT FRAME: SAME_LOCALS_1_STACK_ITEM_EXTENDED, pc=%d",
					status->pc);
#endif
		} else if (type < 251) { /*248-250 CHOP*/
			status->pc += 1
					+ (fy_B2TOUI(smt->entries[status->sc],
							smt->entries[status->sc + 1]));
			status->sc += 2;
#ifdef FY_VERBOSE_PREVERIFIER
			context->logDVarLn(context, "NEXT FRAME: CHOP, pc=%d", status->pc);
#endif
		} else if (type < 252) { /*251 SAME_FRAME_EXTENDED*/
			status->pc += 1
					+ (fy_B2TOUI(smt->entries[status->sc],
							smt->entries[status->sc + 1]));
			status->sc += 2;
#ifdef FY_VERBOSE_PREVERIFIER
			context->logDVarLn(context,
					"NEXT FRAME: SAME_FRAME_EXTENDED, pc=%d", status->pc);
#endif
		} else if (type < 255) { /*252-254 APPEND*/
			status->pc += 1
					+ (fy_B2TOUI(smt->entries[status->sc],
							smt->entries[status->sc + 1]));
			status->sc += 2;
#ifdef FY_VERBOSE_PREVERIFIER
			context->logDVarLn(context, "NEXT FRAME: APPEND, pc=%d",
					status->pc);
#endif
		} else { /*255 FULL*/
			status->pc += 1
					+ (fy_B2TOUI(smt->entries[status->sc],
							smt->entries[status->sc + 1]));
			status->sc += 2;
#ifdef FY_VERBOSE_PREVERIFIER
			context->logDVarLn(context, "NEXT FRAME: FULL, pc=%d", status->pc);
#endif
		}
		return TRUE;
	}
}

static void parseStackItemInitial(fy_context *context, fy_stack_map_table *smt,
		struct read_stack_status *status, fy_method *method, fy_int pc,
		fy_instruction_extra *instruction_extra, fy_exception *exception) {
	fy_int localSize = method->paramStackUsage;
	fy_int i, ofs = 0;
	fy_instInitStackItem(context->memblocks, instruction_extra, method->max_locals,
			exception);
	FYEH();
	if (!(method->access_flags & FY_ACC_STATIC)) {
		fy_instMarkStackItem(instruction_extra, 0, -1);
		ofs = 1;
	}
	for (i = 0; i < localSize; i++) {
		if (method->paramTypes[i] == FY_TYPE_HANDLE) {
			fy_instMarkStackItem(instruction_extra, i + ofs, -1);
		}
	}
#ifdef FY_VERBOSE_PREVERIFIER
	context->logDVarLn(context, "FRAME: INITIAL");
#endif
	status->sc = 0;
	status->localSize = localSize + ofs;
	status->lastFrame = instruction_extra;
	status->frameNum = 0;
	status->pc = -1;
	if (smt != NULL && smt->count > status->frameNum) {
		readStackTypeOfs(context, smt, status, method, pc, exception);
	} else {
		status->type = 256;
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVarLn(context, "FRAME END");
#endif
	}
	status->tmp->sp = instruction_extra->sp;
	status->tmp->s = instruction_extra->s;
#ifdef FY_STRICT_CHECK
	status->tmp->localSize = instruction_extra->localSize = status->localSize;
#endif
#ifdef FY_VERBOSE_PREVERIFIER
	context->logDVarLn(context, "FRAME localSize=%d", status->localSize);
#endif
}

static void parseStackItemFrame(fy_context *context, fy_stack_map_table *smt,
		struct read_stack_status *status, fy_method *method, fy_int pc,
		fy_instruction_extra *instruction_extra, fy_exception *exception) {
	fy_int i, max;
	fy_ubyte vti;
	fy_uint ofs = 0;
	fy_uint type = status->type;

	if (type < 64) {/*0-63 SAME*/
		instruction_extra->sp = method->max_locals;
		instruction_extra->s = status->lastFrame->s;
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVarLn(context, "FRAME: SAME");
#endif
	} else if (type < 128) {/*64-127 SAME_LOCALS_1_STACK_ITEM*/
		vti = smt->entries[status->sc++];
		switch (vti) {
		case 1:
		case 2:
			/* int float */
		{
			fy_instInitStackItem(context->memblocks, instruction_extra,
					method->max_locals + 1, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction_extra);
			fy_instMarkStackItem(instruction_extra, method->max_locals, 0);
			break;
		}
		case 3:
		case 4:
			/* wide */
		{
			fy_instInitStackItem(context->memblocks, instruction_extra,
					method->max_locals + 2, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction_extra);
			fy_instMarkStackItem(instruction_extra, method->max_locals, 0);
			fy_instMarkStackItem(instruction_extra, method->max_locals + 1, 0);
			break;
		}
		case 5:
		case 6: {
			fy_instInitStackItem(context->memblocks, instruction_extra,
					method->max_locals + 1, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction_extra);
			fy_instMarkStackItem(instruction_extra, method->max_locals, -1);
			break;
		}
		case 7:/* handle */
		case 8:/* uninit */
		{
			fy_instInitStackItem(context->memblocks, instruction_extra,
					method->max_locals + 1, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction_extra);
			fy_instMarkStackItem(instruction_extra, method->max_locals, -1);
			status->sc += 2;
			break;
		}
		default: {
			fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
					"Can't verify %s, pc=%"FY_PRINT32"d illegal stack item type %"FY_PRINT32"d at %"FY_PRINT32"d (same_locals_1_stack_item_frame)",
					method->utf8Name, pc, vti, status->sc);
			break;
		}
		}
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVarLn(context, "FRAME: SAME_LOCALS_1_STACK_ITEM");
#endif
	} else if (type < 247) { /*128-246 ILLEGAL*/
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
				"Can't verify %s, pc=%"FY_PRINT32"d: illegal type %"FY_PRINT32"d",
				method->utf8Name, pc, type);
	} else if (type < 248) { /*247 SAME_LOCALS_1_STACK_ITEM_EXTENDED*/
		vti = smt->entries[status->sc++];
		switch (vti) {
		case 0:
			/*top*/
		case 1:
		case 2:
			/* int float */
		{
			fy_instInitStackItem(context->memblocks, instruction_extra,
					method->max_locals + 1, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction_extra);
			fy_instMarkStackItem(instruction_extra, method->max_locals, 0);
			break;
		}
		case 3:
		case 4:
			/* wide */
		{
			fy_instInitStackItem(context->memblocks, instruction_extra,
					method->max_locals + 2, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction_extra);
			fy_instMarkStackItem(instruction_extra, method->max_locals, 0);
			fy_instMarkStackItem(instruction_extra, method->max_locals + 1, 0);
			break;
		}
		case 5:
		case 6: {
			fy_instInitStackItem(context->memblocks, instruction_extra,
					method->max_locals + 1, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction_extra);
			fy_instMarkStackItem(instruction_extra, method->max_locals, -1);
			break;
		}
		case 7: /* handle */
		case 8: /* uninit */
		{
			fy_instInitStackItem(context->memblocks, instruction_extra,
					method->max_locals + 1, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction_extra);
			fy_instMarkStackItem(instruction_extra, method->max_locals, -1);
			status->sc += 2;
			break;
		}
		default: {
			fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
					"Can't verify %s, pc=%"FY_PRINT32"d illegal stack item type %"FY_PRINT32"d at %"FY_PRINT32"d (same_locals_1_stack_item_frame)",
					method->utf8Name, pc, vti, status->sc);
			break;
		}
		}
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVarLn(context, "FRAME: SAME_LOCALS_1_STACK_ITEM_EXTENDED");
#endif
	} else if (type < 251) { /*CHOP*/
		vti = 251 - type;
		if (status->localSize < vti) {
			fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
					"Can't verify [%s] pc=%"FY_PRINT32"d: can't chop %"FY_PRINT32"d from %"FY_PRINT32"d (CHOP)",
					method->utf8Name, pc, vti, status->localSize);
			FYEH();
		}
		max = status->localSize;
		status->localSize -= vti;
		fy_instInitStackItem(context->memblocks, instruction_extra,
				method->max_locals, exception);
		FYEH();
		fy_instStackItemClone(status->lastFrame, instruction_extra);
		for (i = status->localSize; i < max; i++) {
			fy_instMarkStackItem(instruction_extra, i, 0);
		}
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVarLn(context, "FRAME: CHOP");
#endif
	} else if (type < 252) { /*SAME_FRAME_EXTENDED*/
		instruction_extra->sp = method->max_locals;
		instruction_extra->s = status->lastFrame->s;
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVarLn(context, "FRAME: SAME_FRAME_EXTENDED");
#endif
	} else if (type < 255) { /*APPEND*/
		fy_instInitStackItem(context->memblocks, instruction_extra,
				method->max_locals, exception);
		FYEH();
		fy_instStackItemClone(status->lastFrame, instruction_extra);
		max = status->localSize + (type - 251);
		ofs = 0;
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVarLn(context,
				"FRAME: APPEND FROM %"FY_PRINT32"d TO %"FY_PRINT32"d",
				status->localSize, max);
#endif
		for (i = status->localSize; i < max; i++) {
			vti = smt->entries[status->sc++];
			switch (vti) {
			case 0:
				/*top*/
			case 1:
			case 2:
				/* int float */
			{
#ifdef FY_VERBOSE_PREVERIFIER
				context->logDVar(context, "%d", vti);
#endif
				fy_instMarkStackItem(instruction_extra, i + ofs, 0);
				break;
			}
			case 3:
			case 4:
				/* wide */
			{
#ifdef FY_VERBOSE_PREVERIFIER
				context->logDVar(context, "%d-", vti);
#endif
				fy_instMarkStackItem(instruction_extra, i + ofs, 0);
				fy_instMarkStackItem(instruction_extra, i + ofs + 1, 0);
				ofs++;
				break;
			}
			case 5:
			case 6: {
#ifdef FY_VERBOSE_PREVERIFIER
				context->logDVar(context, "%d", vti);
#endif
				fy_instMarkStackItem(instruction_extra, i + ofs, -1);
				break;
			}
			case 7:/* handle */
			case 8:/* uninit */
			{
#ifdef FY_VERBOSE_PREVERIFIER
				context->logDVar(context, "%d", vti);
#endif
				fy_instMarkStackItem(instruction_extra, i + ofs, -1);
				status->sc += 2;
				break;
			}
			default: {
				fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
						"Can't verify %s, pc=%"FY_PRINT32"d illegal stack item type %"FY_PRINT32"d at %"FY_PRINT32"d (APPEND)",
						method->utf8Name, pc, vti, status->sc - 1);
				break;
			}
			}
		}
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVarLn(context, "");
#endif
		status->localSize = max + ofs;
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVarLn(context, "FRAME: APPEND");
#endif
	} else if (type < 256) {/*FULL*/
		fy_uint tmpsc = status->sc;
		max = (fy_B2TOUI(smt->entries[tmpsc], smt->entries[tmpsc + 1]));
		tmpsc += 2;
		for (i = 0; i < max; i++) {
			vti = smt->entries[tmpsc++];
			switch (vti) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6: {
				break;
			}
			case 7:/* handle */
			case 8:/* uninit */
			{
				tmpsc += 2;
				break;
			}
			default: {
				fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
						"Can't verify %s, pc=%"FY_PRINT32"d illegal stack item type %"FY_PRINT32"d at %"FY_PRINT32"d (FULL)",
						method->utf8Name, pc, vti, status->sc - 1);
				break;
			}
			}
		}
		max = (fy_B2TOUI(smt->entries[tmpsc], smt->entries[tmpsc + 1]));
		tmpsc += 2;
		instruction_extra->sp = method->max_locals;
		for (i = 0; i < max; i++) {
			vti = smt->entries[tmpsc++];
			switch (vti) {
			case 0:
				/*top*/
			case 1:
			case 2:
				/* int float */
			{
				instruction_extra->sp++;
				break;
			}
			case 3:
			case 4:
				/* wide */
			{
				instruction_extra->sp += 2;
				break;
			}
			case 5:
			case 6: {
				instruction_extra->sp++;
				break;
			}
			case 7:/* handle */
			case 8:/* uninit */
			{
				instruction_extra->sp++;
				tmpsc += 2;
				break;
			}
			default: {
				fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
						"Can't verify %s, pc=%"FY_PRINT32"d illegal stack item type %"FY_PRINT32"d at %"FY_PRINT32"d (APPEND)",
						method->utf8Name, pc, vti, status->sc - 1);
				break;
			}
			}
		}
		fy_instInitStackItem(context->memblocks, instruction_extra, instruction_extra->sp,
				exception);
		FYEH();
		status->localSize = 0;
		i = 0;
		max =
				(fy_B2TOUI(smt->entries[status->sc],
						smt->entries[status->sc + 1]));
		status->sc += 2;
		ofs = 0;
		for (i = 0; i < max; i++) {
			vti = smt->entries[status->sc++];
			switch (vti) {
			case 0:
				/*top*/
			case 1:
			case 2:
				/* int float */

			{
				fy_instMarkStackItem(instruction_extra, i + ofs, 0);
				break;
			}
			case 3:
			case 4:
				/* wide */
			{
				fy_instMarkStackItem(instruction_extra, i + ofs, 0);
				fy_instMarkStackItem(instruction_extra, i + ofs + 1, 0);
				ofs++;
				break;
			}
			case 5:
			case 6: {
				fy_instMarkStackItem(instruction_extra, i + ofs, -1);
				break;
			}
			case 7:/* handle */
			case 8:/* uninit */
			{
				fy_instMarkStackItem(instruction_extra, i + ofs, -1);
				status->sc += 2;
				break;
			}
			default: {
				fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
						"Can't verify %s, pc=%"FY_PRINT32"d illegal stack item type %"FY_PRINT32"d at %"FY_PRINT32"d (APPEND)",
						method->utf8Name, pc, vti, status->sc - 1);
				break;
			}
			}
		}
		status->localSize = max + ofs;
		i = 0;
		max =
				(fy_B2TOUI(smt->entries[status->sc],
						smt->entries[status->sc + 1]));
		status->sc += 2;
		ofs = 0;

		for (i = 0; i < max; i++) {
			vti = smt->entries[status->sc++];
			switch (vti) {
			case 0:
				/*top*/
			case 1:
			case 2:
				/* int float */
			{
				fy_instMarkStackItem(instruction_extra, method->max_locals + i + ofs,
						0);
				break;
			}
			case 3:
			case 4:
				/* wide */
			{
				fy_instMarkStackItem(instruction_extra, method->max_locals + i + ofs,
						0);
				fy_instMarkStackItem(instruction_extra,
						method->max_locals + i + ofs + 1, 0);
				ofs++;
				break;
			}
			case 5:
			case 6: {
				fy_instMarkStackItem(instruction_extra, method->max_locals + i + ofs,
						-1);
				break;
			}
			case 7: /* handle */
			case 8: /* uninit */
			{
				fy_instMarkStackItem(instruction_extra, method->max_locals + i + ofs,
						-1);
				status->sc += 2;
				break;
			}
			default: {
				fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
						"Can't verify %s, pc=%"FY_PRINT32"d illegal stack item type %"FY_PRINT32"d at %"FY_PRINT32"d (APPEND)",
						method->utf8Name, pc, vti, status->sc - 1);
				break;
			}
			}
		}
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVarLn(context, "FRAME: FULL");
#endif
	} else {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
				"Can't verify %s, pc=%"FY_PRINT32"d illegal stack frame type %"FY_PRINT32"d at %"FY_PRINT32"d (APPEND)",
				method->utf8Name, pc, type, status->sc - 1);
	}
#ifdef FY_VERBOSE_PREVERIFIER
	context->logDVarLn(context, "FRAME localSize=%d", status->localSize);
#endif
	status->lastFrame = instruction_extra;
	status->frameNum++;
#ifdef FY_STRICT_CHECK
	instruction_extra->localSize = status->localSize;
#endif
	if (smt != NULL && smt->count > status->frameNum) {
		readStackTypeOfs(context, smt, status, method, pc, exception);
	} else {
		status->type = 256;
		status->pc = -1;
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVarLn(context, "FRAME END");
#endif
	}
	status->tmp->sp = instruction_extra->sp;
	status->tmp->s = instruction_extra->s;
#ifdef FY_STRICT_CHECK
	status->tmp->localSize = instruction_extra->localSize;
#endif
}

static void parseStackItemInstruction(fy_context *context,
		fy_stack_map_table *smt, struct read_stack_status *status,
		fy_method *method, fy_int pc, fy_instruction_extra *instruction_extra,
		fy_exception *exception) {
	instruction_extra->sp = status->tmp->sp;
	instruction_extra->s = status->tmp->s;
#ifdef FY_STRICT_CHECK
	instruction_extra->localSize = status->tmp->localSize;
#endif
}

void fy_preverify(fy_context *context, fy_method *method,
		fy_exception *exception) {
	fy_ubyte *code = method->code;
	fy_uint codeLength = method->codeLength;
	fy_uint pc = 0, lpc = 0;
	fy_instruction *instruction;
	fy_instruction_extra *instruction_extra;
	fy_uint instCount = FY_IP_begin;
	fy_int ic = 0;
	fy_int op;
	fy_switch_table *switchTable;
	fy_switch_lookup *switchLookup;
	fy_hashMapI tmpSwitchTargets[1];
	fy_hashMapI tmpPcIcMap[1];
	fy_stack_map_table *smt;
	fy_int i = 0, imax;
	fy_int target;
	fy_exceptionHandler *exh;
	fy_int exhic;
	fy_instruction_extra *exhi;

	struct read_stack_status smtStatus;

	fy_e2_label_holder *labels;
	fy_e2_label labelsByOp[0x200];
	fy_engine *engine;

	fy_uint ivalue3, ivalue, ivalue2, ivalue4;
#ifdef FY_VERBOSE_PREVERIFIER
	context->logDVar(context, "Preverifing [");
	context->logDStr(context, method->uniqueName);
	context->logDVarLn(context, "]");
	context->logDVarLn(context,
			"max_locals = %"FY_PRINT32"d max_stack = %"FY_PRINT32"d",
			method->max_locals, method->max_stack);
#endif
	if (method->access_flags & FY_ACC_VERIFIED) {
		fy_fault(exception, NULL, "Method already preverified");
		FYEH();
	}

	engine = context->engines + (method->method_id % context->engineCount);
	memset(labelsByOp, 0, sizeof(labelsByOp));


	(*(engine->runner))(context, NULL, NULL, 0, exception, &labels);
	FYEH();
	while (labels->op >= 0) {
		labelsByOp[labels->op] = labels->label;
		labels++;
	}
	method->engine = engine;

	fy_hashMapIInit(context->memblocks, tmpSwitchTargets, 4, 12, -1, exception);
	FYEH();
	fy_hashMapIInit(context->memblocks, tmpPcIcMap, 10, 12, -1, exception);
	FYEH();

	while (pc < codeLength) {
		op = code[pc];
		fy_hashMapIPut(context->memblocks, tmpPcIcMap, pc, instCount,
				exception);
		FYEH();
#if 0
		context->logDStr(context,method->uniqueName);
		context->logDVarLn(context," - pc=%d inst=%d op=[%d,%s]", pc, instCount, op,
				FY_OP_NAME[op]);
#endif
		if (op > 0xca) {
			fy_fault(exception, NULL, "Illegal op %d", op);
			FYEH();
		}
		if (op == FY_OP_lookupswitch) {
			ivalue4 = context->switchTargets->length;
			fy_hashMapIPut(context->memblocks, tmpSwitchTargets, instCount,
					ivalue4, exception);
			FYEH();
			pc++;
			ivalue3 = (65536 - pc) % 4;
			pc += ivalue3;
			ivalue = fy_nextS4(code)
			;/*db*/
			ivalue2 = fy_nextS4(code)
			;/*np*/
			switchLookup = fy_mmAllocatePerm(context->memblocks,
					sizeof(fy_switch_lookup)
							+ sizeof(struct fy_switch_target) * ivalue2,
					exception);
			FYEH();
			fy_arrayListAdd(context->memblocks, context->switchTargets,
					&switchLookup, exception);
			FYEH();
			switchLookup->defaultJump = ivalue;
			switchLookup->count = ivalue2;
			for (i = 0; i < ivalue2; i++) {
				switchLookup->targets[i].value = fy_nextS4(code)
				;
				switchLookup->targets[i].target = fy_nextS4(code)
				;
			}
			switchLookup->nextPC = pc;
		} else if (op == FY_OP_tableswitch) {
			ivalue4 = context->switchTargets->length;
			fy_hashMapIPut(context->memblocks, tmpSwitchTargets, instCount,
					ivalue4, exception);
			FYEH();
			pc++;
			ivalue3 = (65536 - pc) % 4;
			pc += ivalue3;
			ivalue = fy_nextS4(code)
			;/*db*/
			ivalue2 = fy_nextS4(code)
			;/*lb*/
			ivalue3 = fy_nextS4(code)
			;/*hb*/
			ivalue4 = ivalue3 - ivalue2 + 1;/*count*/
			switchTable = fy_mmAllocatePerm(context->memblocks,
					sizeof(fy_switch_table) + sizeof(fy_uint) * ivalue4,
					exception);
			FYEH();
			fy_arrayListAdd(context->memblocks, context->switchTargets,
					&switchTable, exception);
			FYEH();
			switchTable->defaultJump = ivalue;
			switchTable->lowest = ivalue2;
			switchTable->highest = ivalue3;
			for (i = 0; i < ivalue4; i++) {
				switchTable->targets[i] = fy_nextS4(code)
				;
			}
			switchTable->nextPC = pc;
		} else if (op == FY_OP_wide) {
			if (code[pc + 1] == FY_OP_iinc) {
				pc += 6;
			} else {
				pc += 4;
			}

		} else {
			pc += code_length[op];
		}
		if (pc > codeLength) {
			fy_fault(exception, NULL, "code area damaged");
		}
		instCount++;
	}
	method->codeLength = instCount + 1;
	method->instructions = fy_mmAllocatePerm(context->memblocks,
			instCount * sizeof(fy_instruction), exception);
	FYEH();
	method->instruction_extras = fy_mmAllocatePerm(context->memblocks,
			instCount * sizeof(fy_instruction_extra), exception);
	FYEH();
	method->instruction_ops = fy_mmAllocatePerm(context->memblocks,
			instCount * sizeof(fy_short), exception);
	FYEH();
	pc = 0;
	ic = FY_IP_begin;

	smt = method->stackMapTable;
#ifdef FY_VERBOSE_PREVERIFIER
	if (smt != NULL) {
		context->logDVarLn(context, "Dumping stack map table:");
		for (i = 0; i < smt->length; i++) {
			context->logDVar(context, "%02x ", smt->entries[i]);
		}
		context->logDVarLn(context, "");
	} else {
		context->logDVarLn(context, "No stack map table");
	}
#endif

	while (pc < codeLength) {
		instruction = method->instructions + ic;
		instruction_extra = method->instruction_extras + ic;
		ic++;
		lpc = pc;
		if (pc == 0) {
			parseStackItemInitial(context, smt, &smtStatus, method, pc,
					instruction_extra, exception);
			FYEH();
			if (pc == smtStatus.pc) {
				parseStackItemFrame(context, smt, &smtStatus, method, pc,
						instruction_extra, exception);
			}
		} else if (pc == smtStatus.pc) {
			parseStackItemFrame(context, smt, &smtStatus, method, pc,
					instruction_extra, exception);
		} else {
			parseStackItemInstruction(context, smt, &smtStatus, method, pc,
					instruction_extra, exception);
			if (instruction_extra->sp == 0xffffffff) {
				if (smt == NULL) {
					imax = method->exception_table_length;
					for (i = 0; i < imax; i++) {
						exh = method->exception_table + i;
						if (pc == exh->handler_pc) {
							exhic = getIcFromPc(context, exh->start_pc,
									tmpPcIcMap, exception);
							FYEH();
							exhi = method->instruction_extras + exhic;
							fy_instInitStackItem(context->memblocks,
									instruction_extra, exhi->sp + 1, exception);
							FYEH();
							fy_instStackItemClone(exhi, instruction_extra);
							fy_instMarkStackItem(instruction_extra,
									instruction_extra->sp - 1, -1);
#ifdef FY_VERBOSE_PREVERIFIER
							context->logDVarLn(context,
									"Got frame data from exception handler");
#endif
							break;
						}
					}
				}
				if (instruction_extra->sp == 0xffffffff) {
					fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
							"Can't verify %s, pc=%"FY_PRINT32"d can't fetch stack data from last op",
							method->utf8Name, pc);
				}
			}
		}

		op = fy_nextU1(code);
#if 0
		context->logDStr(context,method->uniqueName);
		context->logDVarLn(context," + pc=%d inst=%d op=[%d,%s]", pc - 1, ic - 1, op,
				FY_OP_NAME[op]);
#endif
		switch (op) {
		/*NO_PARAM*/
		/*DONT CARE*/
		case FY_OP_breakpoint:
		case FY_OP_areturn:
		case FY_OP_freturn:
		case FY_OP_ireturn:
		case FY_OP_lreturn:
		case FY_OP_dreturn:
		case FY_OP_return:
		case FY_OP_athrow: {
			smtStatus.tmp->sp = 0xffffffff;
		}
			break;
			/*NO_STACK_CHANGE/NO_LOCAL_CHANGE*/
		case FY_OP_unused_ba:
		case FY_OP_nop:
		case FY_OP_i2b:
		case FY_OP_i2s:
		case FY_OP_i2c:
		case FY_OP_i2f:
		case FY_OP_ineg:
		case FY_OP_f2i:
		case FY_OP_fneg:
		case FY_OP_lneg:
		case FY_OP_l2d:
		case FY_OP_laload:
		case FY_OP_d2l:
		case FY_OP_dneg:
		case FY_OP_daload:
		case FY_OP_swap: {
// same content
			smtStatus.tmp->sp = instruction_extra->sp;
			smtStatus.tmp->s = instruction_extra->s;
#ifdef FY_STRICT_CHECK
			smtStatus.tmp->localSize = instruction_extra->localSize;
#endif
			break;
		}
		case FY_OP_arraylength: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
		}
			break;
			/*POP 1/NO_LOCAL_CHANGE*/
		case FY_OP_iadd:
		case FY_OP_isub:
		case FY_OP_imul:
		case FY_OP_idiv:
		case FY_OP_irem:
		case FY_OP_iand:
		case FY_OP_ior:
		case FY_OP_ixor:
		case FY_OP_ishl:
		case FY_OP_ishr:
		case FY_OP_iushr:
		case FY_OP_lshl:
		case FY_OP_lshr:
		case FY_OP_lushr:
		case FY_OP_fadd:
		case FY_OP_fsub:
		case FY_OP_fmul:
		case FY_OP_fdiv:
		case FY_OP_frem:
		case FY_OP_fcmpg:
		case FY_OP_fcmpl:
		case FY_OP_d2f:
		case FY_OP_d2i:
		case FY_OP_l2f:
		case FY_OP_l2i:
		case FY_OP_monitorenter:
		case FY_OP_monitorexit:
		case FY_OP_pop: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			break;
		}
			/*POP 1/WRITE_LOCAL*/
		case FY_OP_astore_0: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 0, -1);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 1)
				smtStatus.tmp->localSize = 1;
#endif
			break;
		}
		case FY_OP_astore_1: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 1, -1);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 2)
				smtStatus.tmp->localSize = 2;
#endif
			break;
		}
		case FY_OP_astore_2: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 2, -1);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 3)
				smtStatus.tmp->localSize = 3;
#endif
			break;
		}
		case FY_OP_astore_3: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 3, -1);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 4)
				smtStatus.tmp->localSize = 4;
#endif
			break;
		}
		case FY_OP_fstore_0:
		case FY_OP_istore_0: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 0, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 1)
				smtStatus.tmp->localSize = 1;
#endif
			break;
		}
		case FY_OP_fstore_1:
		case FY_OP_istore_1: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 1, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 2)
				smtStatus.tmp->localSize = 2;
#endif
			break;
		}
		case FY_OP_fstore_2:
		case FY_OP_istore_2: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 2, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 3)
				smtStatus.tmp->localSize = 3;
#endif
			break;
		}
		case FY_OP_fstore_3:
		case FY_OP_istore_3: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 3, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 4)
				smtStatus.tmp->localSize = 4;
#endif
			break;
		}
			/*POP 1, MODIFY 1/NO_LOCAL_CHANGE*/
		case FY_OP_aaload: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
			break;
		}
		case FY_OP_iaload:
		case FY_OP_faload:
		case FY_OP_baload:
		case FY_OP_caload:
		case FY_OP_saload: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
			break;
		}
			/*POP 2/NO_LOCAL_CHANGE*/
		case FY_OP_pop2:
		case FY_OP_ladd:
		case FY_OP_lsub:
		case FY_OP_lmul:
		case FY_OP_ldiv:
		case FY_OP_lrem:
		case FY_OP_land:
		case FY_OP_lor:
		case FY_OP_lxor:
		case FY_OP_dadd:
		case FY_OP_dsub:
		case FY_OP_dmul:
		case FY_OP_ddiv:
		case FY_OP_drem: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 2, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			break;
		}
			/*POP 2/WRITE_LOCAL*/
		case FY_OP_dstore_0:
		case FY_OP_lstore_0: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 2, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 0, 0);
			fy_instMarkStackItem(smtStatus.tmp, 1, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 2)
				smtStatus.tmp->localSize = 2;
#endif
			break;
		}
		case FY_OP_dstore_1:
		case FY_OP_lstore_1: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 2, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 1, 0);
			fy_instMarkStackItem(smtStatus.tmp, 2, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 3)
				smtStatus.tmp->localSize = 3;
#endif
			break;
		}
		case FY_OP_dstore_2:
		case FY_OP_lstore_2: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 2, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 2, 0);
			fy_instMarkStackItem(smtStatus.tmp, 3, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 4)
				smtStatus.tmp->localSize = 4;
#endif
			break;
		}
		case FY_OP_dstore_3:
		case FY_OP_lstore_3: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 2, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 3, 0);
			fy_instMarkStackItem(smtStatus.tmp, 4, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 5)
				smtStatus.tmp->localSize = 5;
#endif
			break;
		}
			/*POP 3/NO_LOCAL_CHANGE*/
		case FY_OP_aastore:
		case FY_OP_iastore:
		case FY_OP_fastore:
		case FY_OP_bastore:
		case FY_OP_castore:
		case FY_OP_sastore:
		case FY_OP_dcmpg:
		case FY_OP_dcmpl:
		case FY_OP_lcmp: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 3, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			break;
		}
			/*POP 4/NO_LOCAL_CHANGE*/
		case FY_OP_dastore:
		case FY_OP_lastore: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp - 4, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			break;
		}
			/*PUSH 1, NO_LOCAL_CHANGE*/
		case FY_OP_aconst_null:
		case FY_OP_aload_0:
		case FY_OP_aload_1:
		case FY_OP_aload_2:
		case FY_OP_aload_3: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp + 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
			break;
		}
		case FY_OP_iconst_0:
		case FY_OP_iconst_1:
		case FY_OP_iconst_2:
		case FY_OP_iconst_3:
		case FY_OP_iconst_4:
		case FY_OP_iconst_5:
		case FY_OP_iconst_m1:
		case FY_OP_iload_0:
		case FY_OP_iload_1:
		case FY_OP_iload_2:
		case FY_OP_iload_3:
		case FY_OP_fconst_0:
		case FY_OP_fconst_1:
		case FY_OP_fconst_2:
		case FY_OP_fload_0:
		case FY_OP_fload_1:
		case FY_OP_fload_2:
		case FY_OP_fload_3:
		case FY_OP_i2d:
		case FY_OP_i2l:
		case FY_OP_f2d:
		case FY_OP_f2l: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp + 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
			break;
		}
			/*PUSH 2, NO_LOCAL_CHANGE*/
		case FY_OP_dconst_0:
		case FY_OP_dconst_1:
		case FY_OP_dload_0:
		case FY_OP_dload_1:
		case FY_OP_dload_2:
		case FY_OP_dload_3:
		case FY_OP_lconst_0:
		case FY_OP_lconst_1:
		case FY_OP_lload_0:
		case FY_OP_lload_1:
		case FY_OP_lload_2:
		case FY_OP_lload_3: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp + 2, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
			break;
		}
			/*X-DUPS*/
		case FY_OP_dup: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp + 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1,
					fy_instGetStackItem(instruction_extra, instruction_extra->sp - 1));
			break;
		}
		case FY_OP_dup_x1: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp + 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 3,
					fy_instGetStackItem(instruction_extra, instruction_extra->sp - 1));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2,
					fy_instGetStackItem(instruction_extra, instruction_extra->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1,
					fy_instGetStackItem(instruction_extra, instruction_extra->sp - 1));
			break;
		}
		case FY_OP_dup_x2:
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction_extra->sp + 1, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-4, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 1));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-3, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 3));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-2, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-1, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 1));
			break;
			case FY_OP_dup2:
			fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 2, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-4, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-3, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 1));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-2, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-1, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 1));
			break;
			case FY_OP_dup2_x1:
			fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 2, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-5, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-4, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 1));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-3, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 3));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-2, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-1, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 1));
			break;
			case FY_OP_dup2_x2:
			fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 2, exception);
			FYEH();
			fy_instStackItemClone(instruction_extra, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-6, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-5, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 1));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-4, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 4));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-3, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 3));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-2, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-1, fy_instGetStackItem(instruction_extra, instruction_extra->sp - 1));
			break;
			/*******/

			case FY_OP_aload: /*local var id*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
				break;
			}
			case FY_OP_astore:/*local var id*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 1, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1, -1);
#ifdef FY_STRICT_CHECK
				if(smtStatus.tmp->localSize < instruction->params.int_params.param1 + 1) smtStatus.tmp->localSize = instruction->params.int_params.param1 + 1;
#endif
				break;
			}
			case FY_OP_dload:/*local var id*/
			case FY_OP_lload:/*local var id*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 2, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
				break;
			}
			case FY_OP_dstore:/*local var id*/
			case FY_OP_lstore:/*local var id*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 2, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1, 0);
				fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1 + 1, 0);
#ifdef FY_STRICT_CHECK
				if(smtStatus.tmp->localSize < instruction->params.int_params.param1 + 2) smtStatus.tmp->localSize = instruction->params.int_params.param1 + 2;
#endif
				break;
			}

			case FY_OP_fload:/*local var id*/
			case FY_OP_iload:/*local var id*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
				break;
			}
			case FY_OP_fstore:/*local var id*/
			case FY_OP_istore:/*local var id*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 1, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1, 0);
#ifdef FY_STRICT_CHECK
				if(smtStatus.tmp->localSize < instruction->params.int_params.param1 + 1) smtStatus.tmp->localSize = instruction->params.int_params.param1 + 1;
#endif
				break;
			}
			case FY_OP_ret:/*local var id*/
			{
				//TODO RET/JSR is no longer supported
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
						"Illegal op in method %s, pc %d, ip %d, JSR/RET not supported",
						method->utf8Name, lpc, ic);
				FYEH();
				break;
			}
			case FY_OP_ldc: /*constant id*/
			{
				instruction->params.ldc.derefed = FALSE;
				instruction->params.ldc.value = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				switch(method->owner->constantTypes[instruction->params.ldc.value]) {
					case CONSTANT_Integer:
					case CONSTANT_Float:
					{
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case CONSTANT_String:
					case CONSTANT_Class:
					{
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;
					}
				}
				break;
			}
			case FY_OP_ldc_w: /*constant id*/
			{
				instruction->params.ldc.derefed = FALSE;
				instruction->params.ldc.value = fy_nextU2(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				switch(method->owner->constantTypes[instruction->params.ldc.value]) {
					case CONSTANT_Integer:
					case CONSTANT_Float:
					{
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case CONSTANT_String:
					case CONSTANT_Class:
					{
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;
					}
				}
				break;
			}
			case FY_OP_ldc2_w: /*constant id*/
			{
				instruction->params.ldc.derefed = FALSE;
				instruction->params.ldc.value = fy_nextU2(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 2, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
				break;
			}

			case FY_OP_newarray: /*type*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
				break;
			}
			case FY_OP_bipush: {
				instruction->params.int_params.param1 = fy_nextS1(code); /*value*/
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
				break;
			}
			case FY_OP_sipush: {
				instruction->params.int_params.param1 = fy_nextS2(code); /*value*/
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
				break;
			}
			case FY_OP_anewarray: /*Constant id -> class constant*/
			{
				target = fy_nextU2(code);
				instruction->params.clazz = fy_vmLookupClassFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
				break;
			}
			case FY_OP_checkcast: /*Constant id -> class constant*/
			{
				target = fy_nextU2(code);
				instruction->params.clazz = fy_vmLookupClassFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				smtStatus.tmp->sp = instruction_extra->sp;
				smtStatus.tmp->s = instruction_extra->s;
#ifdef FY_STRICT_CHECK
				smtStatus.tmp->localSize = instruction_extra->localSize;
#endif
				break;
			}
			case FY_OP_instanceof: /*Constant id -> class constant*/
			{
				target = fy_nextU2(code);
				instruction->params.clazz = fy_vmLookupClassFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
				break;
			}
			case FY_OP_new: {
				target = fy_nextU2(code)
				;
				instruction->params.clazz = fy_vmLookupClassFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
				break;
			}
			case FY_OP_getfield: /*Constant id -> field constant*/
			{
				target = fy_nextU2(code);
				instruction->params.field = fy_vmLookupFieldFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch((fy_byte) fy_strGet(instruction->params.field->descriptor, 0)) {
					case 'D':
					case 'J':
					{
						op = FY_OP_getfield_x;
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case 'L':
					case '[':
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;
					}
					default:
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
				}
				break;
			}
			case FY_OP_getstatic: /*Constant id -> field constant*/
			{
				target = fy_nextU2(code);
				instruction->params.field = fy_vmLookupFieldFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch((fy_byte) fy_strGet(instruction->params.field->descriptor, 0)) {
					case 'D':
					case 'J':
					{
						op = FY_OP_getstatic_x;
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 2, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case 'L':
					case '[':
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;
					}
					default:
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
				}
				break;
			}
			case FY_OP_putfield: /*Constant id -> field constant*/
			{
				target = fy_nextU2(code);
				instruction->params.field = fy_vmLookupFieldFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch((fy_byte) fy_strGet(instruction->params.field->descriptor, 0)) {
					case 'D':
					case 'J':
					{
						op = FY_OP_putfield_x;
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 3, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						break;
					}
					default:
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 2, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						break;
					}
				}
				break;
			}
			case FY_OP_putstatic: /*Constant id -> field constant*/
			{
				target = fy_nextU2(code);
				instruction->params.field = fy_vmLookupFieldFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch((fy_byte) fy_strGet(instruction->params.field->descriptor, 0)) {
					case 'D':
					case 'J':
					{
						op=FY_OP_putstatic_x;
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 2, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						break;
					}
					default:
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						break;
					}
				}
				break;
			}
			case FY_OP_invokespecial:/*Constant id -> method constant*/
			{
				target = fy_nextU2(code);
				instruction->params.method = fy_vmLookupMethodFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch(instruction->params.method->returnType) {
					case FY_TYPE_INT: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage , exception);
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case FY_TYPE_HANDLE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage , exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;
					}
					case FY_TYPE_WIDE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case FY_TYPE_UNKNOWN: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage - 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						break;
					}
				}
				break;
			}
			case FY_OP_invokestatic:/*Constant id -> method constant*/
			{
				target = fy_nextU2(code);
				instruction->params.method = fy_vmLookupMethodFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch(instruction->params.method->returnType) {
					case FY_TYPE_INT: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage+1 , exception);
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;}
					case FY_TYPE_HANDLE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage +1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;}
					case FY_TYPE_WIDE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage + 2, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;}
					case FY_TYPE_UNKNOWN: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage , exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						break;
					}
				}
				break;
			}
			case FY_OP_invokevirtual: /*Constant id -> method constant*/
			{
				target = fy_nextU2(code);
				instruction->params.method = fy_vmLookupMethodFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch(instruction->params.method->returnType) {
					case FY_TYPE_INT: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage , exception);
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;}
					case FY_TYPE_HANDLE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage , exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;}
					case FY_TYPE_WIDE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;}
					case FY_TYPE_UNKNOWN: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage - 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						break;
					}
				}
				break;
			}
			case FY_OP_invokeinterface: {
				target = fy_nextU2(code);
				instruction->params.method = fy_vmLookupMethodFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				pc += 2;
				switch(instruction->params.method->returnType) {
					case FY_TYPE_INT: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage , exception);
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;}
					case FY_TYPE_HANDLE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage , exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;
					}
					case FY_TYPE_WIDE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case FY_TYPE_UNKNOWN: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.method->paramStackUsage - 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						break;
					}
				}
				break;
			}
			case FY_OP_goto:
			{
				target = pc - 1 + fy_nextS2(code);
				instruction->params.int_params.param1 = getIcFromPc(context, target,
						tmpPcIcMap, exception);
				FYEH();
				smtStatus.tmp->sp = 0xffffffff;
				break;
			}
			case FY_OP_if_acmpeq:
			case FY_OP_if_acmpne:
			case FY_OP_if_icmpeq:
			case FY_OP_if_icmpge:
			case FY_OP_if_icmpgt:
			case FY_OP_if_icmple:
			case FY_OP_if_icmplt:
			case FY_OP_if_icmpne: {
				target = pc - 1 + fy_nextS2(code);
				instruction->params.int_params.param1 = getIcFromPc(context, target,
						tmpPcIcMap, exception);
				FYEH();
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 2, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				break;
			}
			case FY_OP_ifeq:
			case FY_OP_ifge:
			case FY_OP_ifgt:
			case FY_OP_ifle:
			case FY_OP_iflt:
			case FY_OP_ifne:
			case FY_OP_ifnonnull:
			case FY_OP_ifnull: {
				target = pc - 1 + fy_nextS2(code)
				;
				instruction->params.int_params.param1 = getIcFromPc(context, target,
						tmpPcIcMap, exception);
				FYEH();
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 1, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				break;
			}
			case FY_OP_goto_w: {
				target = pc - 1 + fy_nextS4(code)
				; /*jump*/
				instruction->params.int_params.param1 = getIcFromPc(context, target,
						tmpPcIcMap, exception);
				FYEH();
				smtStatus.tmp->sp = 0xffffffff;
				break;
			}
			case FY_OP_jsr: {
				target = pc - 1 + fy_nextS2(code)
				; /*jump*/
				instruction->params.int_params.param1 = getIcFromPc(context, target,
						tmpPcIcMap, exception);
				FYEH();
				fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
						"Illegal op in method %s, pc %d, ip %d, JSR/RET not supported",
						method->utf8Name, lpc, ic);
				FYEH();
				break;
			}
			case FY_OP_jsr_w: {
				target = pc - 1 + fy_nextS4(code)
				; /*jump*/
				instruction->params.int_params.param1 = getIcFromPc(context, target,
						tmpPcIcMap, exception);
				FYEH();
				fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
						"Illegal op in method %s, pc %d, ip %d, JSR/RET not supported",
						method->utf8Name, lpc, ic);
				FYEH();
				break;
			}
			case FY_OP_iinc: {
				instruction->params.int_params.param1 = fy_nextU1(code); /*local var id*/
				instruction->params.int_params.param2 = fy_nextS1(code); /*value*/
				smtStatus.tmp->sp = instruction_extra->sp;
				smtStatus.tmp->s = instruction_extra->s;
#ifdef FY_STRICT_CHECK
				smtStatus.tmp->localSize = instruction_extra->localSize;
#endif
				break;
			}
			case FY_OP_multianewarray: {
				instruction->params.int_params.param1 = fy_nextU2(code)
				; /*Constant id-> class Constant*/
				instruction->params.int_params.param2 = fy_nextU1(code); /*count*/
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - instruction->params.int_params.param2 + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
				break;
			}
			case FY_OP_lookupswitch: {
				target = fy_hashMapIGet(context->memblocks, tmpSwitchTargets,
						ic - 1);
				if (target == -1) {
					fy_fault(exception, NULL, "Can't find switch target for ic=%d",
							ic - 1);
				}
				fy_arrayListGet(context->memblocks, context->switchTargets, target,
						&switchLookup);
				instruction->params.swlookup = switchLookup;
				imax = switchLookup->count;
				switchLookup->defaultJump = getIcFromPc(context,
						pc - 1 + switchLookup->defaultJump, tmpPcIcMap, exception);
				for (i = 0; i < imax; i++) {
					(switchLookup->targets + i)->target = getIcFromPc(context,
							pc - 1 + (switchLookup->targets + i)->target,
							tmpPcIcMap, exception);
				}
				pc = switchLookup->nextPC;
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 1, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				break;
			}
			case FY_OP_tableswitch: {
				target = fy_hashMapIGet(context->memblocks, tmpSwitchTargets,
						ic - 1);
				if (target == -1) {
					fy_fault(exception, NULL, "Can't find switch target for ic=%d",
							ic - 1);
				}
				fy_arrayListGet(context->memblocks, context->switchTargets, target,
						&switchTable);
				instruction->params.swtable = switchTable;
				imax = switchTable->highest - switchTable->lowest;
				switchTable->defaultJump = getIcFromPc(context,
						pc - 1 + switchTable->defaultJump, tmpPcIcMap, exception);
				for (i = 0; i <= imax; i++) {
					switchTable->targets[i] = getIcFromPc(context,
							pc - 1 + switchTable->targets[i], tmpPcIcMap,
							exception);
				}
				pc = switchTable->nextPC;
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 1, exception);
				FYEH();
				fy_instStackItemClone(instruction_extra, smtStatus.tmp);
				break;
			}
			case FY_OP_wide: {
				op = fy_nextU1(code);
				switch (op) {
					case FY_OP_aload: /*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;
					}
					case FY_OP_astore:/*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1, -1);
#ifdef FY_STRICT_CHECK
						if(smtStatus.tmp->localSize < instruction->params.int_params.param1 + 1) smtStatus.tmp->localSize = instruction->params.int_params.param1 + 1;
#endif
						break;
					}
					case FY_OP_dload:/*local var id*/
					case FY_OP_lload:/*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 2, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case FY_OP_dstore:/*local var id*/
					case FY_OP_lstore:/*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 2, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1, 0);
						fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1 + 1, 0);
#ifdef FY_STRICT_CHECK
						if(smtStatus.tmp->localSize < instruction->params.int_params.param1 + 2) smtStatus.tmp->localSize = instruction->params.int_params.param1 + 2;
#endif
						break;
					}
					case FY_OP_fload:/*local var id*/
					case FY_OP_iload:/*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case FY_OP_fstore:/*local var id*/
					case FY_OP_istore:/*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction_extra->sp - 1, exception);
						FYEH();
						fy_instStackItemClone(instruction_extra, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1, 0);
#ifdef FY_STRICT_CHECK
						if(smtStatus.tmp->localSize < instruction->params.int_params.param1 + 1) smtStatus.tmp->localSize = instruction->params.int_params.param1 + 1;
#endif
						break;
					}
					case FY_OP_ret:/*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
								"Illegal op in method %s, pc %d, ip %d, JSR/RET not supported",
								method->utf8Name, lpc, ic);
						FYEH();
						break;
					}
					case FY_OP_iinc: {
						instruction->params.int_params.param1 = fy_nextU2(code);
						instruction->params.int_params.param2 = fy_nextS2(code);
						smtStatus.tmp->sp = instruction_extra->sp;
						smtStatus.tmp->s = instruction_extra->s;
#ifdef FY_STRICT_CHECK
						smtStatus.tmp->localSize = instruction_extra->localSize;
#endif
						break;
					}
					default: {
						fy_fault(exception, NULL, "Unknown Wide OPCode %d", op);
						FYEH();
						break;
					}
				}
				break;
			}
			default: {
				fy_fault(exception, NULL, "Unknown OPCode %d", op);
				FYEH();
				break;
			}
		}
		if (instruction_extra->sp > 65535) {
			fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
					"Illegal sp value in method %s, pc %d, ip %d",
					method->utf8Name, lpc, ic);
			FYEH();
		}
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVar(context, "%04x ", ic - 1);
		for (i = 0; i < method->max_locals; i++) {
#ifdef FY_STRICT_CHECK
			context->logDVar(context, "%c",
					i < instruction_extra->localSize ?
					fy_instGetStackItem(instruction_extra, i) ? 'R' : 'I'
					: '.');
#else
			context->logDVar(context, "%c",
					fy_instGetStackItem(instruction_extra, i) ? 'R' : 'I');
#endif
		}
		context->logDVar(context, " ");
		for (i = method->max_locals; i < instruction_extra->sp; i++) {
			context->logDVar(context, "%c",
					fy_instGetStackItem(instruction_extra, i) ? 'R' : 'I');
		}
		if (instruction_extra->sp < method->max_locals) {
			fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
					"Can't verify %s, pc=%"FY_PRINT32"d stack underflow",
					method->utf8Name, lpc);
		} else if (instruction_extra->sp > method->max_locals + method->max_stack) {
			fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
					"Can't verify %s, pc=%"FY_PRINT32"d stack overflow",
					method->utf8Name, lpc);
		}
		for (i = instruction_extra->sp; i < method->max_locals + method->max_stack;
				i++) {
			context->logDVar(context, " ");
		}
		context->logDVarLn(context, " %-14s %08x %08x", FY_OP_NAME[op],
				instruction->params.int_params.param1,
				instruction->params.int_params.param2);
#endif
        switch (op) {
            case FY_OP_fload:
            case FY_OP_aload:
                op = FY_OP_iload;
                break;
            case FY_OP_iload_0:
            case FY_OP_aload_0:
            case FY_OP_fload_0:
                op = FY_OP_iload;
                instruction->params.int_params.param1 = 0;
                break;
            case FY_OP_iload_1:
            case FY_OP_aload_1:
            case FY_OP_fload_1:
                op = FY_OP_iload;
                instruction->params.int_params.param1 = 1;
                break;
            case FY_OP_iload_2:
            case FY_OP_aload_2:
            case FY_OP_fload_2:
                op = FY_OP_iload;
                instruction->params.int_params.param1 = 2;
                break;
            case FY_OP_iload_3:
            case FY_OP_aload_3:
            case FY_OP_fload_3:
                op = FY_OP_iload;
                instruction->params.int_params.param1 = 3;
                break;
            case FY_OP_fstore:
            case FY_OP_astore:
                op = FY_OP_istore;
                break;
            case FY_OP_istore_0:
            case FY_OP_astore_0:
            case FY_OP_fstore_0:
                op = FY_OP_istore;
                instruction->params.int_params.param1 = 0;
                break;
            case FY_OP_istore_1:
            case FY_OP_astore_1:
            case FY_OP_fstore_1:
                op = FY_OP_istore;
                instruction->params.int_params.param1 = 1;
                break;
            case FY_OP_istore_2:
            case FY_OP_astore_2:
            case FY_OP_fstore_2:
                op = FY_OP_istore;
                instruction->params.int_params.param1 = 2;
                break;
            case FY_OP_istore_3:
            case FY_OP_astore_3:
            case FY_OP_fstore_3:
                op = FY_OP_istore;
                instruction->params.int_params.param1 = 3;
                break;
            
            case FY_OP_dload:
                op = FY_OP_lload;
                break;
            case FY_OP_dload_0:
            case FY_OP_lload_0:
                op = FY_OP_lload;
                instruction->params.int_params.param1 = 0;
                break;
            case FY_OP_dload_1:
            case FY_OP_lload_1:
                op = FY_OP_lload;
                instruction->params.int_params.param1 = 1;
                break;
            case FY_OP_dload_2:
            case FY_OP_lload_2:
                op = FY_OP_lload;
                instruction->params.int_params.param1 = 2;
                break;
            case FY_OP_dload_3:
            case FY_OP_lload_3:
                op = FY_OP_lload;
                instruction->params.int_params.param1 = 3;
                break;
            case FY_OP_dstore:
                op = FY_OP_lstore;
                break;
            case FY_OP_dstore_0:
            case FY_OP_lstore_0:
                op = FY_OP_lstore;
                instruction->params.int_params.param1 = 0;
                break;
            case FY_OP_dstore_1:
            case FY_OP_lstore_1:
                op = FY_OP_lstore;
                instruction->params.int_params.param1 = 1;
                break;
            case FY_OP_dstore_2:
            case FY_OP_lstore_2:
                op = FY_OP_lstore;
                instruction->params.int_params.param1 = 2;
                break;
            case FY_OP_dstore_3:
            case FY_OP_lstore_3:
                op = FY_OP_lstore;
                instruction->params.int_params.param1 = 3;
                break;
                
            case FY_OP_bipush:
                op = FY_OP_sipush;
                break;
                
            case FY_OP_iconst_m1:
                op = FY_OP_sipush;
                instruction->params.int_params.param1 = -1;
                break;
                
            case FY_OP_iconst_0:
            case FY_OP_aconst_null:
                op = FY_OP_sipush;
                instruction->params.int_params.param1 = 0;
                break;
                
            case FY_OP_iconst_1:
                op = FY_OP_sipush;
                instruction->params.int_params.param1 = 1;
                break;
                
            case FY_OP_iconst_2:
                op = FY_OP_sipush;
                instruction->params.int_params.param1 = 2;
                break;
                
            case FY_OP_iconst_3:
                op = FY_OP_sipush;
                instruction->params.int_params.param1 = 3;
                break;
                
            case FY_OP_iconst_4:
                op = FY_OP_sipush;
                instruction->params.int_params.param1 = 4;
                break;
                
            case FY_OP_iconst_5:
                op = FY_OP_sipush;
                instruction->params.int_params.param1 = 5;
                break;
                
            case FY_OP_fconst_0:
                op = FY_OP_sipush;
                instruction->params.int_params.param1 = fy_floatToInt(0.0f);
                break;
                
            case FY_OP_fconst_1:
                op = FY_OP_sipush;
                instruction->params.int_params.param1 = fy_floatToInt(1.0f);
                break;
                
            case FY_OP_fconst_2:
                op = FY_OP_sipush;
                instruction->params.int_params.param1 = fy_floatToInt(2.0f);
                break;
            
            case FY_OP_dconst_0:
                op = FY_OP_slpush;
                instruction->params.int_params.param1 = fy_HOFL(fy_doubleToLong(0.0));
                instruction->params.int_params.param2 = fy_LOFL(fy_doubleToLong(0.0));
                break;
            case FY_OP_lconst_0:
                op = FY_OP_slpush;
                instruction->params.int_params.param1 = 0;
                instruction->params.int_params.param2 = 0;
                break;
            case FY_OP_dconst_1:
                op = FY_OP_slpush;
                instruction->params.int_params.param1 = fy_HOFL(fy_doubleToLong(1.0));
                instruction->params.int_params.param2 = fy_LOFL(fy_doubleToLong(1.0));
                break;
            case FY_OP_lconst_1:
                op = FY_OP_slpush;
                instruction->params.int_params.param1 = 0;
                instruction->params.int_params.param2 = 1;
                break;
                
            case FY_OP_ldc_w:
                op = FY_OP_ldc;
                break;
                
            case FY_OP_aaload:
            case FY_OP_faload:
                op = FY_OP_iaload;
                break;
                
            case FY_OP_aastore:
            case FY_OP_fastore:
                op = FY_OP_iastore;
                break;
                
            case FY_OP_dastore:
                op = FY_OP_lastore;
                break;
                
            case FY_OP_invokeinterface:
                op = FY_OP_invokevirtual;
                break;
            case FY_OP_areturn:
            case FY_OP_freturn:
                op = FY_OP_ireturn;
                break;
            case FY_OP_dreturn:
                op = FY_OP_lreturn;
                break;
            case FY_OP_goto_w:
                op = FY_OP_goto;
                break;
            case FY_OP_ifnull:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_ifnull_b;
            	}
            	break;
            case FY_OP_ifnonnull:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_ifnonnull_b;
            	}
            	break;
            case FY_OP_ifeq:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_ifeq_b;
            	}
            	break;
            case FY_OP_ifne:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_ifne_b;
            	}
            	break;
            case FY_OP_iflt:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_iflt_b;
            	}
            	break;
            case FY_OP_ifge:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_ifge_b;
            	}
            	break;
            case FY_OP_ifgt:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_ifgt_b;
            	}
            	break;
            case FY_OP_ifle:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_ifle_b;
            	}
            	break;
            case FY_OP_if_icmpeq:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_if_icmpeq_b;
            	}
            	break;
            case FY_OP_if_icmpne:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_if_icmpne_b;
            	}
            	break;
            case FY_OP_if_acmpeq:
                if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_if_icmpeq_b;
            	} else {
            		op = FY_OP_if_icmpeq;
            	}
                break;
            case FY_OP_if_acmpne:
                if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_if_icmpne_b;
            	} else {
            		op = FY_OP_if_icmpne;
            	}
                break;
            case FY_OP_if_icmplt:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_if_icmplt_b;
            	}
            	break;
            case FY_OP_if_icmpge:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_if_icmpge_b;
            	}
            	break;
            case FY_OP_if_icmpgt:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_if_icmpgt_b;
            	}
            	break;
            case FY_OP_if_icmple:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_if_icmple_b;
            	}
            	break;
            case FY_OP_goto:
            	if(instruction->params.int_params.param1 < ic){
            		op = FY_OP_goto_b;
            	}
            	break;
            default:
                break;
        }
		instruction->inst = labelsByOp[op];
		method->instruction_ops[FY_PDIFF(fy_instruction, instruction, method->instructions)] = op;
		if(instruction->inst == NULL){
			fy_breakpoint();
            fy_fault(NULL, "Illegal op", "Illegal op: %s", FY_OP_NAME[op]);
		}
#if defined(FY_STRICT_CHECK) || defined(FY_INSTRUCTION_COUNT)
		instruction->op = op;
#endif
	}
	method->instructions[0].inst = labelsByOp[FY_OP_dropout];
	preverifyMethodExceptionTable(context, method, tmpPcIcMap, exception);
	preverifyMethodLineNumberTable(context, method, tmpPcIcMap, exception);
	method->access_flags |= FY_ACC_VERIFIED;
	fy_mmFree(context->memblocks, code);
	fy_hashMapIDestroy(context->memblocks, tmpPcIcMap);
	fy_hashMapIDestroy(context->memblocks, tmpSwitchTargets);
	i++;
}

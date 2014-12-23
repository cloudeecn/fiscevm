/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part offiscevm
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
 * along withfiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fyc/Preverifier.h"
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

#ifdef FY_VERBOSE
#define FY_VERBOSE_PREVERIFIER
#endif

#define FY_VERBOSE_PREVERIFIER

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
	fy_instruction tmp[1];
	fy_instruction *lastFrame;
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
		fy_instruction *instruction, fy_exception *exception) {
	fy_int localSize = method->paramStackUsage;
	fy_int i, ofs = 0;
	fy_instInitStackItem(context->memblocks, instruction, method->max_locals,
			exception);
	FYEH();
	if (!(method->access_flags & FY_ACC_STATIC)) {
		fy_instMarkStackItem(instruction, 0, -1);
		ofs = 1;
	}
	for (i = 0; i < localSize; i++) {
		if (method->paramTypes[i] == FY_TYPE_HANDLE) {
			fy_instMarkStackItem(instruction, i + ofs, -1);
		}
	}
#ifdef FY_VERBOSE_PREVERIFIER
	context->logDVarLn(context, "FRAME: INITIAL");
#endif
	status->sc = 0;
	status->localSize = localSize + ofs;
	status->lastFrame = instruction;
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
	status->tmp->sp = instruction->sp;
	status->tmp->s = instruction->s;
#ifdef FY_STRICT_CHECK
	status->tmp->localSize = instruction->localSize = status->localSize;
#endif
#ifdef FY_VERBOSE_PREVERIFIER
	context->logDVarLn(context, "FRAME localSize=%d", status->localSize);
#endif
}

static void parseStackItemFrame(fy_context *context, fy_stack_map_table *smt,
		struct read_stack_status *status, fy_method *method, fy_int pc,
		fy_instruction *instruction, fy_exception *exception) {
	fy_int i, max;
	fy_ubyte vti;
	fy_uint ofs = 0;
	fy_uint type = status->type;

	if (type < 64) {/*0-63 SAME*/
		instruction->sp = method->max_locals;
		instruction->s = status->lastFrame->s;
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
			fy_instInitStackItem(context->memblocks, instruction,
					method->max_locals + 1, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction);
			fy_instMarkStackItem(instruction, method->max_locals, 0);
			break;
		}
		case 3:
		case 4:
			/* wide */
		{
			fy_instInitStackItem(context->memblocks, instruction,
					method->max_locals + 2, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction);
			fy_instMarkStackItem(instruction, method->max_locals, 0);
			fy_instMarkStackItem(instruction, method->max_locals + 1, 0);
			break;
		}
		case 5:
		case 6: {
			fy_instInitStackItem(context->memblocks, instruction,
					method->max_locals + 1, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction);
			fy_instMarkStackItem(instruction, method->max_locals, -1);
			break;
		}
		case 7:/* handle */
		case 8:/* uninit */
		{
			fy_instInitStackItem(context->memblocks, instruction,
					method->max_locals + 1, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction);
			fy_instMarkStackItem(instruction, method->max_locals, -1);
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
			fy_instInitStackItem(context->memblocks, instruction,
					method->max_locals + 1, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction);
			fy_instMarkStackItem(instruction, method->max_locals, 0);
			break;
		}
		case 3:
		case 4:
			/* wide */
		{
			fy_instInitStackItem(context->memblocks, instruction,
					method->max_locals + 2, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction);
			fy_instMarkStackItem(instruction, method->max_locals, 0);
			fy_instMarkStackItem(instruction, method->max_locals + 1, 0);
			break;
		}
		case 5:
		case 6: {
			fy_instInitStackItem(context->memblocks, instruction,
					method->max_locals + 1, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction);
			fy_instMarkStackItem(instruction, method->max_locals, -1);
			break;
		}
		case 7: /* handle */
		case 8: /* uninit */
		{
			fy_instInitStackItem(context->memblocks, instruction,
					method->max_locals + 1, exception);
			FYEH();
			fy_instStackItemClone(status->lastFrame, instruction);
			fy_instMarkStackItem(instruction, method->max_locals, -1);
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
		fy_instInitStackItem(context->memblocks, instruction,
				method->max_locals, exception);
		FYEH();
		fy_instStackItemClone(status->lastFrame, instruction);
		for (i = status->localSize; i < max; i++) {
			fy_instMarkStackItem(instruction, i, 0);
		}
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVarLn(context, "FRAME: CHOP");
#endif
	} else if (type < 252) { /*SAME_FRAME_EXTENDED*/
		instruction->sp = method->max_locals;
		instruction->s = status->lastFrame->s;
#ifdef FY_VERBOSE_PREVERIFIER
		context->logDVarLn(context, "FRAME: SAME_FRAME_EXTENDED");
#endif
	} else if (type < 255) { /*APPEND*/
		fy_instInitStackItem(context->memblocks, instruction,
				method->max_locals, exception);
		FYEH();
		fy_instStackItemClone(status->lastFrame, instruction);
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
				fy_instMarkStackItem(instruction, i + ofs, 0);
				break;
			}
			case 3:
			case 4:
				/* wide */
			{
#ifdef FY_VERBOSE_PREVERIFIER
				context->logDVar(context, "%d-", vti);
#endif
				fy_instMarkStackItem(instruction, i + ofs, 0);
				fy_instMarkStackItem(instruction, i + ofs + 1, 0);
				ofs++;
				break;
			}
			case 5:
			case 6: {
#ifdef FY_VERBOSE_PREVERIFIER
				context->logDVar(context, "%d", vti);
#endif
				fy_instMarkStackItem(instruction, i + ofs, -1);
				break;
			}
			case 7:/* handle */
			case 8:/* uninit */
			{
#ifdef FY_VERBOSE_PREVERIFIER
				context->logDVar(context, "%d", vti);
#endif
				fy_instMarkStackItem(instruction, i + ofs, -1);
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
		instruction->sp = method->max_locals;
		for (i = 0; i < max; i++) {
			vti = smt->entries[tmpsc++];
			switch (vti) {
			case 0:
				/*top*/
			case 1:
			case 2:
				/* int float */
			{
				instruction->sp++;
				break;
			}
			case 3:
			case 4:
				/* wide */
			{
				instruction->sp += 2;
				break;
			}
			case 5:
			case 6: {
				instruction->sp++;
				break;
			}
			case 7:/* handle */
			case 8:/* uninit */
			{
				instruction->sp++;
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
		fy_instInitStackItem(context->memblocks, instruction, instruction->sp,
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
				fy_instMarkStackItem(instruction, i + ofs, 0);
				break;
			}
			case 3:
			case 4:
				/* wide */
			{
				fy_instMarkStackItem(instruction, i + ofs, 0);
				fy_instMarkStackItem(instruction, i + ofs + 1, 0);
				ofs++;
				break;
			}
			case 5:
			case 6: {
				fy_instMarkStackItem(instruction, i + ofs, -1);
				break;
			}
			case 7:/* handle */
			case 8:/* uninit */
			{
				fy_instMarkStackItem(instruction, i + ofs, -1);
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
				fy_instMarkStackItem(instruction, method->max_locals + i + ofs,
						0);
				break;
			}
			case 3:
			case 4:
				/* wide */
			{
				fy_instMarkStackItem(instruction, method->max_locals + i + ofs,
						0);
				fy_instMarkStackItem(instruction,
						method->max_locals + i + ofs + 1, 0);
				ofs++;
				break;
			}
			case 5:
			case 6: {
				fy_instMarkStackItem(instruction, method->max_locals + i + ofs,
						-1);
				break;
			}
			case 7: /* handle */
			case 8: /* uninit */
			{
				fy_instMarkStackItem(instruction, method->max_locals + i + ofs,
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
	status->lastFrame = instruction;
	status->frameNum++;
#ifdef FY_STRICT_CHECK
	instruction->localSize = status->localSize;
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
	status->tmp->sp = instruction->sp;
	status->tmp->s = instruction->s;
#ifdef FY_STRICT_CHECK
	status->tmp->localSize = instruction->localSize;
#endif
}

static void parseStackItemInstruction(fy_context *context,
		fy_stack_map_table *smt, struct read_stack_status *status,
		fy_method *method, fy_int pc, fy_instruction *instruction,
		fy_exception *exception) {
	instruction->sp = status->tmp->sp;
	instruction->s = status->tmp->s;
#ifdef FY_STRICT_CHECK
	instruction->localSize = status->tmp->localSize;
#endif
}

void fy_preverify(fy_context *context, fy_method *method,
		fy_exception *exception) {
	fy_ubyte *code = method->code;
	fy_uint codeLength = method->codeLength;
	fy_uint pc = 0, lpc = 0;
	fy_instruction *instruction;
	fy_uint instCount = 0;
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
	fy_instruction *exhi;

	struct read_stack_status smtStatus;

	fy_uint ivalue3, ivalue, ivalue2, ivalue4;
#ifdef FY_VERBOSE_PREVERIFIER
	fy_class *owner = method->owner;
	context->logDVar(context, "Preverifing [");
	context->logDStr(context, method->uniqueName);
	context->logDVarLn(context, "]");
	context->logDVarLn(context,
			"max_locals = %"FY_PRINT32"d max_stack = %"FY_PRINT32"d",
			method->max_locals, method->max_stack);
	/*
	 imax=owner->fieldCount;
	 for(i=0;i<imax;i++){
	 context->logDStr(context,owner->fields[i]->uniqueName);
	 context->logDVarLn(context," ###");
	 }
	 */
#endif
	if (method->access_flags & FY_ACC_VERIFIED) {
		fy_fault(exception, NULL, "Method already preverified");
		FYEH();
	}

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
		if (op == LOOKUPSWITCH) {
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
		} else if (op == TABLESWITCH) {
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
		} else if (op == WIDE) {
			if (code[pc + 1] == IINC) {
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
	method->codeLength = instCount;
	method->instructions = fy_mmAllocatePerm(context->memblocks,
			instCount * sizeof(fy_instruction), exception);
	FYEH();
	pc = 0;

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
		instruction = method->instructions + (ic++);
		lpc = pc;
		if (pc == 0) {
			parseStackItemInitial(context, smt, &smtStatus, method, pc,
					instruction, exception);
			FYEH();
			if (pc == smtStatus.pc) {
				parseStackItemFrame(context, smt, &smtStatus, method, pc,
						instruction, exception);
			}
		} else if (pc == smtStatus.pc) {
			parseStackItemFrame(context, smt, &smtStatus, method, pc,
					instruction, exception);
		} else {
			parseStackItemInstruction(context, smt, &smtStatus, method, pc,
					instruction, exception);
			if (instruction->sp == 0xffffffff) {
				if (smt == NULL) {
					imax = method->exception_table_length;
					for (i = 0; i < imax; i++) {
						exh = method->exception_table + i;
						if (pc == exh->handler_pc) {
							exhic = getIcFromPc(context, exh->start_pc,
									tmpPcIcMap, exception);
							FYEH();
							exhi = method->instructions + exhic;
							fy_instInitStackItem(context->memblocks,
									instruction, exhi->sp + 1, exception);
							FYEH();
							fy_instStackItemClone(exhi, instruction);
							fy_instMarkStackItem(instruction,
									instruction->sp - 1, -1);
#ifdef FY_VERBOSE_PREVERIFIER
							context->logDVarLn(context,
									"Got frame data from exception handler");
#endif
							break;
						}
					}
				}
				if (instruction->sp == 0xffffffff) {
					fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
							"Can't verify %s, pc=%"FY_PRINT32"d can't fetch stack data from last op",
							method->utf8Name, pc);
				}
			}
		}

		op = fy_nextU1(code);
		instruction->op = op;

#if 0
		context->logDStr(context,method->uniqueName);
		context->logDVarLn(context," + pc=%d inst=%d op=[%d,%s]", pc - 1, ic - 1, op,
				FY_OP_NAME[op]);
#endif
		switch (op) {
		/*NO_PARAM*/
		/*DONT CARE*/
		case BREAKPOINT:
		case ARETURN:
		case FRETURN:
		case IRETURN:
		case LRETURN:
		case DRETURN:
		case RETURN:
		case ATHROW: {
			smtStatus.tmp->sp = 0xffffffff;
		}
			break;
			/*NO_STACK_CHANGE/NO_LOCAL_CHANGE*/
		case UNUSED_BA:
		case NOP:
		case I2B:
		case I2S:
		case I2C:
		case I2F:
		case INEG:
		case F2I:
		case FNEG:
		case LNEG:
		case L2D:
		case LALOAD:
		case D2L:
		case DNEG:
		case DALOAD:
		case SWAP: {
// same content
			smtStatus.tmp->sp = instruction->sp;
			smtStatus.tmp->s = instruction->s;
#ifdef FY_STRICT_CHECK
			smtStatus.tmp->localSize = instruction->localSize;
#endif
			break;
		}
		case ARRAYLENGTH: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
		}
			break;
			/*POP 1/NO_LOCAL_CHANGE*/
		case IADD:
		case ISUB:
		case IMUL:
		case IDIV:
		case IREM:
		case IAND:
		case IOR:
		case IXOR:
		case ISHL:
		case ISHR:
		case IUSHR:
		case LSHL:
		case LSHR:
		case LUSHR:
		case FADD:
		case FSUB:
		case FMUL:
		case FDIV:
		case FREM:
		case FCMPG:
		case FCMPL:
		case D2F:
		case D2I:
		case L2F:
		case L2I:
		case MONITORENTER:
		case MONITOREXIT:
		case POP: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			break;
		}
			/*POP 1/WRITE_LOCAL*/
		case ASTORE_0: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 0, -1);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 1)
				smtStatus.tmp->localSize = 1;
#endif
			break;
		}
		case ASTORE_1: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 1, -1);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 2)
				smtStatus.tmp->localSize = 2;
#endif
			break;
		}
		case ASTORE_2: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 2, -1);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 3)
				smtStatus.tmp->localSize = 3;
#endif
			break;
		}
		case ASTORE_3: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 3, -1);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 4)
				smtStatus.tmp->localSize = 4;
#endif
			break;
		}
		case FSTORE_0:
		case ISTORE_0: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 0, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 1)
				smtStatus.tmp->localSize = 1;
#endif
			break;
		}
		case FSTORE_1:
		case ISTORE_1: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 1, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 2)
				smtStatus.tmp->localSize = 2;
#endif
			break;
		}
		case FSTORE_2:
		case ISTORE_2: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 2, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 3)
				smtStatus.tmp->localSize = 3;
#endif
			break;
		}
		case FSTORE_3:
		case ISTORE_3: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 3, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 4)
				smtStatus.tmp->localSize = 4;
#endif
			break;
		}
			/*POP 1, MODIFY 1/NO_LOCAL_CHANGE*/
		case AALOAD: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
			break;
		}
		case IALOAD:
		case FALOAD:
		case BALOAD:
		case CALOAD:
		case SALOAD: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
			break;
		}
			/*POP 2/NO_LOCAL_CHANGE*/
		case POP2:
		case LADD:
		case LSUB:
		case LMUL:
		case LDIV:
		case LREM:
		case LAND:
		case LOR:
		case LXOR:
		case DADD:
		case DSUB:
		case DMUL:
		case DDIV:
		case DREM: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 2, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			break;
		}
			/*POP 2/WRITE_LOCAL*/
		case DSTORE_0:
		case LSTORE_0: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 2, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 0, 0);
			fy_instMarkStackItem(smtStatus.tmp, 1, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 2)
				smtStatus.tmp->localSize = 2;
#endif
			break;
		}
		case DSTORE_1:
		case LSTORE_1: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 2, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 1, 0);
			fy_instMarkStackItem(smtStatus.tmp, 2, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 3)
				smtStatus.tmp->localSize = 3;
#endif
			break;
		}
		case DSTORE_2:
		case LSTORE_2: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 2, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 2, 0);
			fy_instMarkStackItem(smtStatus.tmp, 3, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 4)
				smtStatus.tmp->localSize = 4;
#endif
			break;
		}
		case DSTORE_3:
		case LSTORE_3: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 2, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, 3, 0);
			fy_instMarkStackItem(smtStatus.tmp, 4, 0);
#ifdef FY_STRICT_CHECK
			if (smtStatus.tmp->localSize < 5)
				smtStatus.tmp->localSize = 5;
#endif
			break;
		}
			/*POP 3/NO_LOCAL_CHANGE*/
		case AASTORE:
		case IASTORE:
		case FASTORE:
		case BASTORE:
		case CASTORE:
		case SASTORE:
		case DCMPG:
		case DCMPL:
		case LCMP: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 3, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			break;
		}
			/*POP 4/NO_LOCAL_CHANGE*/
		case DASTORE:
		case LASTORE: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp - 4, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			break;
		}
			/*PUSH 1, NO_LOCAL_CHANGE*/
		case ACONST_NULL:
		case ALOAD_0:
		case ALOAD_1:
		case ALOAD_2:
		case ALOAD_3: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp + 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
			break;
		}
		case ICONST_0:
		case ICONST_1:
		case ICONST_2:
		case ICONST_3:
		case ICONST_4:
		case ICONST_5:
		case ICONST_M1:
		case ILOAD_0:
		case ILOAD_1:
		case ILOAD_2:
		case ILOAD_3:
		case FCONST_0:
		case FCONST_1:
		case FCONST_2:
		case FLOAD_0:
		case FLOAD_1:
		case FLOAD_2:
		case FLOAD_3:
		case I2D:
		case I2L:
		case F2D:
		case F2L: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp + 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
			break;
		}
			/*PUSH 2, NO_LOCAL_CHANGE*/
		case DCONST_0:
		case DCONST_1:
		case DLOAD_0:
		case DLOAD_1:
		case DLOAD_2:
		case DLOAD_3:
		case LCONST_0:
		case LCONST_1:
		case LLOAD_0:
		case LLOAD_1:
		case LLOAD_2:
		case LLOAD_3: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp + 2, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
			break;
		}
			/*X-DUPS*/
		case DUP: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp + 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1,
					fy_instGetStackItem(instruction, instruction->sp - 1));
			break;
		}
		case DUP_X1: {
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp + 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 3,
					fy_instGetStackItem(instruction, instruction->sp - 1));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2,
					fy_instGetStackItem(instruction, instruction->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1,
					fy_instGetStackItem(instruction, instruction->sp - 1));
			break;
		}
		case DUP_X2:
			fy_instInitStackItem(context->memblocks, smtStatus.tmp,
					instruction->sp + 1, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-4, fy_instGetStackItem(instruction, instruction->sp - 1));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-3, fy_instGetStackItem(instruction, instruction->sp - 3));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-2, fy_instGetStackItem(instruction, instruction->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-1, fy_instGetStackItem(instruction, instruction->sp - 1));
			break;
			case DUP2:
			fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 2, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-4, fy_instGetStackItem(instruction, instruction->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-3, fy_instGetStackItem(instruction, instruction->sp - 1));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-2, fy_instGetStackItem(instruction, instruction->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-1, fy_instGetStackItem(instruction, instruction->sp - 1));
			break;
			case DUP2_X1:
			fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 2, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-5, fy_instGetStackItem(instruction, instruction->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-4, fy_instGetStackItem(instruction, instruction->sp - 1));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-3, fy_instGetStackItem(instruction, instruction->sp - 3));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-2, fy_instGetStackItem(instruction, instruction->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-1, fy_instGetStackItem(instruction, instruction->sp - 1));
			break;
			case DUP2_X2:
			fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 2, exception);
			FYEH();
			fy_instStackItemClone(instruction, smtStatus.tmp);
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-6, fy_instGetStackItem(instruction, instruction->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-5, fy_instGetStackItem(instruction, instruction->sp - 1));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-4, fy_instGetStackItem(instruction, instruction->sp - 4));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-3, fy_instGetStackItem(instruction, instruction->sp - 3));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-2, fy_instGetStackItem(instruction, instruction->sp - 2));
			fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp-1, fy_instGetStackItem(instruction, instruction->sp - 1));
			break;
			/*******/

			case ALOAD: /*local var id*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
				break;
			}
			case ASTORE:/*local var id*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 1, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1, -1);
#ifdef FY_STRICT_CHECK
				if(smtStatus.tmp->localSize < instruction->params.int_params.param1 + 1) smtStatus.tmp->localSize = instruction->params.int_params.param1 + 1;
#endif
				break;
			}
			case DLOAD:/*local var id*/
			case LLOAD:/*local var id*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 2, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
				break;
			}
			case DSTORE:/*local var id*/
			case LSTORE:/*local var id*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 2, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1, 0);
				fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1 + 1, 0);
#ifdef FY_STRICT_CHECK
				if(smtStatus.tmp->localSize < instruction->params.int_params.param1 + 2) smtStatus.tmp->localSize = instruction->params.int_params.param1 + 2;
#endif
				break;
			}

			case FLOAD:/*local var id*/
			case ILOAD:/*local var id*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
				break;
			}
			case FSTORE:/*local var id*/
			case ISTORE:/*local var id*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 1, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1, 0);
#ifdef FY_STRICT_CHECK
				if(smtStatus.tmp->localSize < instruction->params.int_params.param1 + 1) smtStatus.tmp->localSize = instruction->params.int_params.param1 + 1;
#endif
				break;
			}
			case RET:/*local var id*/
			{
				//TODO RET/JSR is no longer supported
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
						"Illegal op in method %s, pc %d, ip %d, JSR/RET not supported",
						method->utf8Name, lpc, ic);
				FYEH();
				break;
			}
			case LDC: /*constant id*/
			{
				instruction->params.ldc.derefed = FALSE;
				instruction->params.ldc.value = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
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
			case LDC_W: /*constant id*/
			{
				instruction->params.ldc.derefed = FALSE;
				instruction->params.ldc.value = fy_nextU2(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
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
			case LDC2_W: /*constant id*/
			{
				instruction->params.ldc.derefed = FALSE;
				instruction->params.ldc.value = fy_nextU2(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 2, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
				break;
			}

			case NEWARRAY: /*type*/
			{
				instruction->params.int_params.param1 = fy_nextU1(code);
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
				break;
			}
			case BIPUSH: {
				instruction->params.int_params.param1 = fy_nextS1(code); /*value*/
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
				break;
			}
			case SIPUSH: {
				instruction->params.int_params.param1 = fy_nextS2(code); /*value*/
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
				break;
			}
			case ANEWARRAY: /*Constant id -> class constant*/
			{
				target = fy_nextU2(code);
				instruction->params.clazz = fy_vmLookupClassFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
				break;
			}
			case CHECKCAST: /*Constant id -> class constant*/
			{
				target = fy_nextU2(code);
				instruction->params.clazz = fy_vmLookupClassFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				smtStatus.tmp->sp = instruction->sp;
				smtStatus.tmp->s = instruction->s;
#ifdef FY_STRICT_CHECK
				smtStatus.tmp->localSize = instruction->localSize;
#endif
				break;
			}
			case INSTANCEOF: /*Constant id -> class constant*/
			{
				target = fy_nextU2(code);
				instruction->params.clazz = fy_vmLookupClassFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
				break;
			}
			case NEW: {
				target = fy_nextU2(code)
				;
				instruction->params.clazz = fy_vmLookupClassFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
				break;
			}
			case GETFIELD: /*Constant id -> field constant*/
			{
				target = fy_nextU2(code);
				instruction->params.field = fy_vmLookupFieldFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch((fy_byte) fy_strGet(instruction->params.field->descriptor, 0)) {
					case 'D':
					case 'J':
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case 'L':
					case '[':
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;
					}
					default:
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
				}
				break;
			}
			case GETSTATIC: /*Constant id -> field constant*/
			{
				target = fy_nextU2(code);
				instruction->params.field = fy_vmLookupFieldFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch((fy_byte) fy_strGet(instruction->params.field->descriptor, 0)) {
					case 'D':
					case 'J':
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 2, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case 'L':
					case '[':
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;
					}
					default:
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
				}
				break;
			}
			case PUTFIELD: /*Constant id -> field constant*/
			{
				target = fy_nextU2(code);
				instruction->params.field = fy_vmLookupFieldFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch((fy_byte) fy_strGet(instruction->params.field->descriptor, 0)) {
					case 'D':
					case 'J':
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 3, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						break;
					}
					default:
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 2, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						break;
					}
				}
				break;
			}
			case PUTSTATIC: /*Constant id -> field constant*/
			{
				target = fy_nextU2(code);
				instruction->params.field = fy_vmLookupFieldFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch((fy_byte) fy_strGet(instruction->params.field->descriptor, 0)) {
					case 'D':
					case 'J':
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 2, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						break;
					}
					default:
					{
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						break;
					}
				}
				break;
			}
			case INVOKESPECIAL:/*Constant id -> method constant*/
			{
				target = fy_nextU2(code);
				instruction->params.method = fy_vmLookupMethodFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch(instruction->params.method->returnType) {
					case FY_TYPE_INT: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage , exception);
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case FY_TYPE_HANDLE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage , exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;
					}
					case FY_TYPE_WIDE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case FY_TYPE_UNKNOWN: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage - 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						break;
					}
				}
				break;
			}
			case INVOKESTATIC:/*Constant id -> method constant*/
			{
				target = fy_nextU2(code);
				instruction->params.method = fy_vmLookupMethodFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch(instruction->params.method->returnType) {
					case FY_TYPE_INT: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage+1 , exception);
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;}
					case FY_TYPE_HANDLE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage +1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;}
					case FY_TYPE_WIDE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage + 2, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;}
					case FY_TYPE_UNKNOWN: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage , exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						break;
					}
				}
				break;
			}
			case INVOKEVIRTUAL: /*Constant id -> method constant*/
			{
				target = fy_nextU2(code);
				instruction->params.method = fy_vmLookupMethodFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				switch(instruction->params.method->returnType) {
					case FY_TYPE_INT: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage , exception);
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;}
					case FY_TYPE_HANDLE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage , exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;}
					case FY_TYPE_WIDE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;}
					case FY_TYPE_UNKNOWN: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage - 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						break;
					}
				}
				break;
			}
			case INVOKEINTERFACE: {
				target = fy_nextU2(code);
				instruction->params.method = fy_vmLookupMethodFromConstant(context,
						method->owner->constantPools[target], exception);
				FYEH();
				pc += 2;
				switch(instruction->params.method->returnType) {
					case FY_TYPE_INT: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage , exception);
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;}
					case FY_TYPE_HANDLE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage , exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;
					}
					case FY_TYPE_WIDE: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case FY_TYPE_UNKNOWN: {
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.method->paramStackUsage - 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						break;
					}
				}
				break;
			}
			case GOTO:
			{
				target = pc - 1 + fy_nextS2(code);
				instruction->params.int_params.param1 = getIcFromPc(context, target,
						tmpPcIcMap, exception);
				FYEH();
				smtStatus.tmp->sp = 0xffffffff;
				break;
			}
			case IF_ACMPEQ:
			case IF_ACMPNE:
			case IF_ICMPEQ:
			case IF_ICMPGE:
			case IF_ICMPGT:
			case IF_ICMPLE:
			case IF_ICMPLT:
			case IF_ICMPNE: {
				target = pc - 1 + fy_nextS2(code);
				instruction->params.int_params.param1 = getIcFromPc(context, target,
						tmpPcIcMap, exception);
				FYEH();
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 2, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				break;
			}
			case IFEQ:
			case IFGE:
			case IFGT:
			case IFLE:
			case IFLT:
			case IFNE:
			case IFNONNULL:
			case IFNULL: {
				target = pc - 1 + fy_nextS2(code)
				;
				instruction->params.int_params.param1 = getIcFromPc(context, target,
						tmpPcIcMap, exception);
				FYEH();
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 1, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				break;
			}
			case GOTO_W: {
				target = pc - 1 + fy_nextS4(code)
				; /*jump*/
				instruction->params.int_params.param1 = getIcFromPc(context, target,
						tmpPcIcMap, exception);
				FYEH();
				smtStatus.tmp->sp = 0xffffffff;
				break;
			}
			case JSR: {
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
			case JSR_W: {
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
			case IINC: {
				instruction->params.int_params.param1 = fy_nextU1(code); /*local var id*/
				instruction->params.int_params.param2 = fy_nextS1(code); /*value*/
				smtStatus.tmp->sp = instruction->sp;
				smtStatus.tmp->s = instruction->s;
#ifdef FY_STRICT_CHECK
				smtStatus.tmp->localSize = instruction->localSize;
#endif
				break;
			}
			case MULTIANEWARRAY: {
				instruction->params.int_params.param1 = fy_nextU2(code)
				; /*Constant id-> class Constant*/
				instruction->params.int_params.param2 = fy_nextU1(code); /*count*/
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - instruction->params.int_params.param2 + 1, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
				break;
			}
			case LOOKUPSWITCH: {
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
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 1, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				break;
			}
			case TABLESWITCH: {
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
				fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 1, exception);
				FYEH();
				fy_instStackItemClone(instruction, smtStatus.tmp);
				break;
			}
			case WIDE: {
				switch (instruction->op = fy_nextU1(code)) {
					case ALOAD: /*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, -1);
						break;
					}
					case ASTORE:/*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1, -1);
#ifdef FY_STRICT_CHECK
						if(smtStatus.tmp->localSize < instruction->params.int_params.param1 + 1) smtStatus.tmp->localSize = instruction->params.int_params.param1 + 1;
#endif
						break;
					}
					case DLOAD:/*local var id*/
					case LLOAD:/*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 2, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 2, 0);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case DSTORE:/*local var id*/
					case LSTORE:/*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 2, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1, 0);
						fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1 + 1, 0);
#ifdef FY_STRICT_CHECK
						if(smtStatus.tmp->localSize < instruction->params.int_params.param1 + 2) smtStatus.tmp->localSize = instruction->params.int_params.param1 + 2;
#endif
						break;
					}
					case FLOAD:/*local var id*/
					case ILOAD:/*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp + 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, smtStatus.tmp->sp - 1, 0);
						break;
					}
					case FSTORE:/*local var id*/
					case ISTORE:/*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_instInitStackItem(context->memblocks, smtStatus.tmp, instruction->sp - 1, exception);
						FYEH();
						fy_instStackItemClone(instruction, smtStatus.tmp);
						fy_instMarkStackItem(smtStatus.tmp, instruction->params.int_params.param1, 0);
#ifdef FY_STRICT_CHECK
						if(smtStatus.tmp->localSize < instruction->params.int_params.param1 + 1) smtStatus.tmp->localSize = instruction->params.int_params.param1 + 1;
#endif
						break;
					}
					case RET:/*local var id*/
					{
						instruction->params.int_params.param1 = fy_nextU2(code);
						fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
								"Illegal op in method %s, pc %d, ip %d, JSR/RET not supported",
								method->utf8Name, lpc, ic);
						FYEH();
						break;
					}
					case IINC: {
						instruction->params.int_params.param1 = fy_nextU2(code);
						instruction->params.int_params.param2 = fy_nextS2(code);
						smtStatus.tmp->sp = instruction->sp;
						smtStatus.tmp->s = instruction->s;
#ifdef FY_STRICT_CHECK
						smtStatus.tmp->localSize = instruction->localSize;
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
		if (instruction->sp > 65535) {
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
					i < instruction->localSize ?
							fy_instGetStackItem(instruction, i) ? 'R' : 'I'
							: '.');
#else
			context->logDVar(context, "%c",
					fy_instGetStackItem(instruction, i) ? 'R' : 'I');
#endif
		}
		context->logDVar(context, " ");
		for (i = method->max_locals; i < instruction->sp; i++) {
			context->logDVar(context, "%c",
					fy_instGetStackItem(instruction, i) ? 'R' : 'I');
		}
		if (instruction->sp < method->max_locals) {
			fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
					"Can't verify %s, pc=%"FY_PRINT32"d stack underflow",
					method->utf8Name, lpc);
		} else if (instruction->sp > method->max_locals + method->max_stack) {
			fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
					"Can't verify %s, pc=%"FY_PRINT32"d stack overflow",
					method->utf8Name, lpc);
		}
		for (i = instruction->sp; i < method->max_locals + method->max_stack;
				i++) {
			context->logDVar(context, " ");
		}
		context->logDVarLn(context, " %-14s %08x %08x", FY_OP_NAME[op],
				instruction->params.int_params.param1,
				instruction->params.int_params.param2);
#endif
	}
	preverifyMethodExceptionTable(context, method, tmpPcIcMap, exception);
	preverifyMethodLineNumberTable(context, method, tmpPcIcMap, exception);
	method->access_flags |= FY_ACC_VERIFIED;
	fy_mmFree(context->memblocks, code);
	fy_hashMapIDestroy(context->memblocks, tmpPcIcMap);
	fy_hashMapIDestroy(context->memblocks, tmpSwitchTargets);
	i++;
}

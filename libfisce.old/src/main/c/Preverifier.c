/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of libfisce.
 *
 * libfisce is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * libfisce is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libfisce.  If not, see <http://www.gnu.org/licenses/>.
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

void fy_preverify(fy_context *context, fy_method *method,
		fy_exception *exception) {
	fy_ubyte *code = method->code;
	fy_uint codeLength = method->codeLength;
	fy_uint pc = 0;
	fy_instruction *instruction;
	fy_uint instCount = 0;
	fy_uint ic = 0;
	fy_uint op;
	fy_switch_table *switchTable;
	fy_switch_lookup *switchLookup;
	fy_hashMapI tmpSwitchTargets[1];
	fy_hashMapI tmpPcIcMap[1];
	fy_int i, imax;
	fy_int target;

	fy_uint ivalue3, ivalue, ivalue2, ivalue4;
#ifdef FY_VERBOSE
	fy_class *owner = method->owner;
	context->logDVar(context, "Preverifing [");
	context->logDStr(context, method->uniqueName);
	context->logDVarLn(context, "]");
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
		context->logDVar(context," - pc=%d inst=%d op=[%d,%s]\n", pc, instCount, op,
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

	while (pc < codeLength) {
		op = fy_nextU1(code);
		instruction = method->instructions + (ic++);
		instruction->op = op;
#if 0
		context->logDStr(context,method->uniqueName);
		context->logDVar(context," + pc=%d inst=%d op=[%d,%s]\n", pc - 1, ic - 1, op,
				FY_OP_NAME[op]);
#endif
		switch (op) {
		case UNUSED_BA:
		case AALOAD:
		case AASTORE:
		case ACONST_NULL:
		case ALOAD_0:
		case ALOAD_1:
		case ALOAD_2:
		case ALOAD_3:
		case ARETURN:
		case ARRAYLENGTH:
		case ASTORE_0:
		case ASTORE_1:
		case ASTORE_2:
		case ASTORE_3:
		case ATHROW:
		case BALOAD:
		case BASTORE:
		case BREAKPOINT:
		case CALOAD:
		case CASTORE:
		case D2F:
		case D2I:
		case D2L:
		case DADD:
		case DALOAD:
		case DASTORE:
		case DCMPG:
		case DCMPL:
		case DCONST_0:
		case DCONST_1:
		case DDIV:
		case DLOAD_0:
		case DLOAD_1:
		case DLOAD_2:
		case DLOAD_3:
		case DMUL:
		case DNEG:
		case DREM:
		case DRETURN:
		case DSTORE_0:
		case DSTORE_1:
		case DSTORE_2:
		case DSTORE_3:
		case DSUB:
		case DUP:
		case DUP_X1:
		case DUP_X2:
		case DUP2:
		case DUP2_X1:
		case DUP2_X2:
		case F2D:
		case F2I:
		case F2L:
		case FADD:
		case FALOAD:
		case FASTORE:
		case FCMPG:
		case FCMPL:
		case FCONST_0:
		case FCONST_1:
		case FCONST_2:
		case FDIV:
		case FLOAD_0:
		case FLOAD_1:
		case FLOAD_2:
		case FLOAD_3:
		case FMUL:
		case FNEG:
		case FREM:
		case FRETURN:
		case FSTORE_0:
		case FSTORE_1:
		case FSTORE_2:
		case FSTORE_3:
		case FSUB:
		case I2B:
		case I2C:
		case I2D:
		case I2F:
		case I2L:
		case I2S:
		case IADD:
		case IALOAD:
		case IAND:
		case IASTORE:
		case ICONST_0:
		case ICONST_1:
		case ICONST_2:
		case ICONST_3:
		case ICONST_4:
		case ICONST_5:
		case ICONST_M1:
		case IDIV:
		case ILOAD_0:
		case ILOAD_1:
		case ILOAD_2:
		case ILOAD_3:
		case IMUL:
		case INEG:
		case IOR:
		case IREM:
		case IRETURN:
		case ISHL:
		case ISHR:
		case ISTORE_0:
		case ISTORE_1:
		case ISTORE_2:
		case ISTORE_3:
		case ISUB:
		case IUSHR:
		case IXOR:
		case L2D:
		case L2F:
		case L2I:
		case LADD:
		case LALOAD:
		case LAND:
		case LASTORE:
		case LCMP:
		case LCONST_0:
		case LCONST_1:
		case LDIV:
		case LLOAD_0:
		case LLOAD_1:
		case LLOAD_2:
		case LLOAD_3:
		case LMUL:
		case LNEG:
		case LOR:
		case LREM:
		case LRETURN:
		case LSHL:
		case LSHR:
		case LSTORE_0:
		case LSTORE_1:
		case LSTORE_2:
		case LSTORE_3:
		case LSUB:
		case LUSHR:
		case LXOR:
		case MONITORENTER:
		case MONITOREXIT:
		case NOP:
		case POP:
		case POP2:
		case RETURN:
		case SALOAD:
		case SASTORE:
		case SWAP: {
			break;
		}
		case ALOAD: /*local var id*/
		case ASTORE:/*local var id*/
		case DLOAD:/*local var id*/
		case DSTORE:/*local var id*/
		case FLOAD:/*local var id*/
		case FSTORE:/*local var id*/
		case ILOAD:/*local var id*/
		case ISTORE:/*local var id*/
		case LLOAD:/*local var id*/
		case LSTORE:/*local var id*/
		case RET:/*local var id*/
		{
			instruction->params.int_params.param1 = fy_nextU1(code);
			break;
		}
		case LDC: /*constant id*/
		{
			instruction->params.ldc.derefed = FALSE;
			instruction->params.ldc.value = fy_nextU1(code);
			break;
		}
		case LDC_W: /*constant id*/
		case LDC2_W: /*constant id*/
		{
			instruction->params.ldc.derefed = FALSE;
			instruction->params.ldc.value = fy_nextU2(code)
			;
			break;
		}

		case NEWARRAY: /*type*/
		{
			instruction->params.int_params.param1 = fy_nextU1(code);
			break;
		}
		case BIPUSH: {
			instruction->params.int_params.param1 = fy_nextS1(code); /*value*/
			break;
		}
		case SIPUSH: {
			instruction->params.int_params.param1 = fy_nextS2(code)
			; /*value*/
			break;
		}
		case ANEWARRAY: /*Constant id -> class constant*/
		case CHECKCAST: /*Constant id -> class constant*/
		case INSTANCEOF: /*Constant id -> class constant*/

		case NEW: {
			target = fy_nextU2(code)
			;
			instruction->params.clazz = fy_vmLookupClassFromConstant(context,
					method->owner->constantPools[target], exception);
			FYEH();
			break;
		}
		case GETFIELD: /*Constant id -> field constant*/
		case GETSTATIC: /*Constant id -> field constant*/
		case PUTFIELD: /*Constant id -> field constant*/
		case PUTSTATIC: /*Constant id -> field constant*/
		{
			target = fy_nextU2(code)
			;
			instruction->params.field = fy_vmLookupFieldFromConstant(context,
					method->owner->constantPools[target], exception);
			FYEH();
			break;
		}
		case INVOKESPECIAL:/*Constant id -> method constant*/
		case INVOKESTATIC:/*Constant id -> method constant*/
		case INVOKEVIRTUAL: /*Constant id -> method constant*/
		{
			target = fy_nextU2(code)
			;
			instruction->params.method = fy_vmLookupMethodFromConstant(context,
					method->owner->constantPools[target], exception);
			FYEH();
			break;
		}
		case INVOKEINTERFACE: {
			target = fy_nextU2(code)
			;
			instruction->params.method = fy_vmLookupMethodFromConstant(context,
					method->owner->constantPools[target], exception);
			FYEH();
			pc += 2;
			break;
		}
		case GOTO:
		case IF_ACMPEQ:
		case IF_ACMPNE:
		case IF_ICMPEQ:
		case IF_ICMPGE:
		case IF_ICMPGT:
		case IF_ICMPLE:
		case IF_ICMPLT:
		case IF_ICMPNE:
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
			break;
		}
		case GOTO_W: {
			target = pc - 1 + fy_nextS4(code)
			; /*jump*/
			instruction->params.int_params.param1 = getIcFromPc(context, target,
					tmpPcIcMap, exception);
			FYEH();
			break;
		}
		case JSR: {
			target = pc - 1 + fy_nextS2(code)
			; /*jump*/
			instruction->params.int_params.param1 = getIcFromPc(context, target,
					tmpPcIcMap, exception);
			FYEH();
			break;
		}
		case JSR_W: {
			target = pc - 1 + fy_nextS4(code)
			; /*jump*/
			instruction->params.int_params.param1 = getIcFromPc(context, target,
					tmpPcIcMap, exception);
			FYEH();
			break;
		}
		case IINC: {
			instruction->params.int_params.param1 = fy_nextU1(code); /*local var id*/
			instruction->params.int_params.param2 = fy_nextS1(code); /*value*/
			break;
		}
		case MULTIANEWARRAY: {
			instruction->params.int_params.param1 = fy_nextU2(code)
			; /*Constant id-> class Constant*/
			instruction->params.int_params.param2 = fy_nextU1(code); /*count*/
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
			break;
		}
		case WIDE: {
			switch (instruction->op = fy_nextU1(code) ) {
			case ALOAD: /*local var id*/
			case ASTORE:/*local var id*/
			case DLOAD:/*local var id*/
			case DSTORE:/*local var id*/
			case FLOAD:/*local var id*/
			case FSTORE:/*local var id*/
			case ILOAD:/*local var id*/
			case ISTORE:/*local var id*/
			case LLOAD:/*local var id*/
			case LSTORE:/*local var id*/
			case RET:/*local var id*/
			{
				instruction->params.int_params.param1 = fy_nextU2(code)
				;
				break;
			}
			case IINC: {
				instruction->params.int_params.param1 = fy_nextU2(code)
				;
				instruction->params.int_params.param2 = fy_nextS2(code)
				;
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
	}
	preverifyMethodExceptionTable(context, method, tmpPcIcMap, exception);
	preverifyMethodLineNumberTable(context, method, tmpPcIcMap, exception);
	method->access_flags |= FY_ACC_VERIFIED;
	fy_mmFree(context->memblocks, code);
	fy_hashMapIDestroy(context->memblocks, tmpPcIcMap);
	fy_hashMapIDestroy(context->memblocks, tmpSwitchTargets);
	i++;
}

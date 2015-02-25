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

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fyc/Instructions.h"
#include <math.h>

const char *FY_OP_NAME[MAX_INSTRUCTIONS] = { /**/
"NOP", /* 0x00 */
"ACONST_NULL", /* 0x01 */
"ICONST_M1", /* 0x02 */
"ICONST_0", /* 0x03 */
"ICONST_1", /* 0x04 */
"ICONST_2", /* 0x05 */
"ICONST_3", /* 0x06 */
"ICONST_4", /* 0x07 */
"ICONST_5", /* 0x08 */
"LCONST_0", /* 0x09 */
"LCONST_1", /* 0x0A */
"FCONST_0", /* 0x0B */
"FCONST_1", /* 0x0C */
"FCONST_2", /* 0x0D */
"DCONST_0", /* 0x0E */
"DCONST_1", /* 0x0F */
"BIPUSH", /* 0x10 */
"SIPUSH", /* 0x11 */
"LDC", /* 0x12 */
"LDC_W", /* 0x13 */
"LDC2_W", /* 0x14 */
"ILOAD", /* 0x15 */
"LLOAD", /* 0x16 */
"FLOAD", /* 0x17 */
"DLOAD", /* 0x18 */
"ALOAD", /* 0x19 */
"ILOAD_0", /* 0x1A */
"ILOAD_1", /* 0x1B */
"ILOAD_2", /* 0x1C */
"ILOAD_3", /* 0x1D */
"LLOAD_0", /* 0x1E */
"LLOAD_1", /* 0x1F */
"LLOAD_2", /* 0x20 */
"LLOAD_3", /* 0x21 */
"FLOAD_0", /* 0x22 */
"FLOAD_1", /* 0x23 */
"FLOAD_2", /* 0x24 */
"FLOAD_3", /* 0x25 */
"DLOAD_0", /* 0x26 */
"DLOAD_1", /* 0x27 */
"DLOAD_2", /* 0x28 */
"DLOAD_3", /* 0x29 */
"ALOAD_0", /* 0x2A */
"ALOAD_1", /* 0x2B */
"ALOAD_2", /* 0x2C */
"ALOAD_3", /* 0x2D */
"IALOAD", /* 0x2E */
"LALOAD", /* 0x2F */
"FALOAD", /* 0x30 */
"DALOAD", /* 0x31 */
"AALOAD", /* 0x32 */
"BALOAD", /* 0x33 */
"CALOAD", /* 0x34 */
"SALOAD", /* 0x35 */
"ISTORE", /* 0x36 */
"LSTORE", /* 0x37 */
"FSTORE", /* 0x38 */
"DSTORE", /* 0x39 */
"ASTORE", /* 0x3A */
"ISTORE_0", /* 0x3B */
"ISTORE_1", /* 0x3C */
"ISTORE_2", /* 0x3D */
"ISTORE_3", /* 0x3E */
"LSTORE_0", /* 0x3F */
"LSTORE_1", /* 0x40 */
"LSTORE_2", /* 0x41 */
"LSTORE_3", /* 0x42 */
"FSTORE_0", /* 0x43 */
"FSTORE_1", /* 0x44 */
"FSTORE_2", /* 0x45 */
"FSTORE_3", /* 0x46 */
"DSTORE_0", /* 0x47 */
"DSTORE_1", /* 0x48 */
"DSTORE_2", /* 0x49 */
"DSTORE_3", /* 0x4A */
"ASTORE_0", /* 0x4B */
"ASTORE_1", /* 0x4C */
"ASTORE_2", /* 0x4D */
"ASTORE_3", /* 0x4E */
"IASTORE", /* 0x4F */
"LASTORE", /* 0x50 */
"FASTORE", /* 0x51 */
"DASTORE", /* 0x52 */
"AASTORE", /* 0x53 */
"BASTORE", /* 0x54 */
"CASTORE", /* 0x55 */
"SASTORE", /* 0x56 */
"POP", /* 0x57 */
"POP2", /* 0x58 */
"DUP", /* 0x59 */
"DUP_X1", /* 0x5A */
"DUP_X2", /* 0x5B */
"DUP2", /* 0x5C */
"DUP2_X1", /* 0x5D */
"DUP2_X2", /* 0x5E */
"SWAP", /* 0x5F */
"IADD", /* 0x60 */
"LADD", /* 0x61 */
"FADD", /* 0x62 */
"DADD", /* 0x63 */
"ISUB", /* 0x64 */
"LSUB", /* 0x65 */
"FSUB", /* 0x66 */
"DSUB", /* 0x67 */
"IMUL", /* 0x68 */
"LMUL", /* 0x69 */
"FMUL", /* 0x6A */
"DMUL", /* 0x6B */
"IDIV", /* 0x6C */
"LDIV", /* 0x6D */
"FDIV", /* 0x6E */
"DDIV", /* 0x6F */
"IREM", /* 0x70 */
"LREM", /* 0x71 */
"FREM", /* 0x72 */
"DREM", /* 0x73 */
"INEG", /* 0x74 */
"LNEG", /* 0x75 */
"FNEG", /* 0x76 */
"DNEG", /* 0x77 */
"ISHL", /* 0x78 */
"LSHL", /* 0x79 */
"ISHR", /* 0x7A */
"LSHR", /* 0x7B */
"IUSHR", /* 0x7C */
"LUSHR", /* 0x7D */
"IAND", /* 0x7E */
"LAND", /* 0x7F */
"IOR", /* 0x80 */
"LOR", /* 0x81 */
"IXOR", /* 0x82 */
"LXOR", /* 0x83 */
"IINC", /* 0x84 */
"I2L", /* 0x85 */
"I2F", /* 0x86 */
"I2D", /* 0x87 */
"L2I", /* 0x88 */
"L2F", /* 0x89 */
"L2D", /* 0x8A */
"F2I", /* 0x8B */
"F2L", /* 0x8C */
"F2D", /* 0x8D */
"D2I", /* 0x8E */
"D2L", /* 0x8F */
"D2F", /* 0x90 */
"I2B", /* 0x91 */
"I2C", /* 0x92 */
"I2S", /* 0x93 */
"LCMP", /* 0x94 */
"FCMPL", /* 0x95 */
"FCMPG", /* 0x96 */
"DCMPL", /* 0x97 */
"DCMPG", /* 0x98 */
"IFEQ", /* 0x99 */
"IFNE", /* 0x9A */
"IFLT", /* 0x9B */
"IFGE", /* 0x9C */
"IFGT", /* 0x9D */
"IFLE", /* 0x9E */
"IF_ICMPEQ", /* 0x9F */
"IF_ICMPNE", /* 0xA0 */
"IF_ICMPLT", /* 0xA1 */
"IF_ICMPGE", /* 0xA2 */
"IF_ICMPGT", /* 0xA3 */
"IF_ICMPLE", /* 0xA4 */
"IF_ACMPEQ", /* 0xA5 */
"IF_ACMPNE", /* 0xA6 */
"GOTO", /* 0xA7 */
"JSR", /* 0xA8 */
"RET", /* 0xA9 */
"TABLESWITCH", /* 0xAA */
"LOOKUPSWITCH", /* 0xAB */
"IRETURN", /* 0xAC */
"LRETURN", /* 0xAD */
"FRETURN", /* 0xAE */
"DRETURN", /* 0xAF */
"ARETURN", /* 0xB0 */
"RETURN", /* 0xB1 */
"GETSTATIC", /* 0xB2 */
"PUTSTATIC", /* 0xB3 */
"GETFIELD", /* 0xB4 */
"PUTFIELD", /* 0xB5 */
"INVOKEVIRTUAL", /* 0xB6 */
"INVOKESPECIAL", /* 0xB7 */
"INVOKESTATIC", /* 0xB8 */
"INVOKEINTERFACE", /* 0xB9 */
"UNUSED_BA", /* 0xBA */
"NEW", /* 0xBB */
"NEWARRAY", /* 0xBC */
"ANEWARRAY", /* 0xBD */
"ARRAYLENGTH", /* 0xBE */
"ATHROW", /* 0xBF */
"CHECKCAST", /* 0xC0 */
"INSTANCEOF", /* 0xC1 */
"MONITORENTER", /* 0xC2 */
"MONITOREXIT", /* 0xC3 */
"WIDE", /* 0xC4 */
"MULTIANEWARRAY", /* 0xC5 */
"IFNULL", /* 0xC6 */
"IFNONNULL", /* 0xC7 */
"GOTO_W", /* 0xC8 */
"JSR_W", /* 0xC9 */
"BREAKPOINT", /* 0xCA */
"<undef>", /* 0xCB */
"<undef>", /* 0xCC */
"RETURN_NS", /* 0xCD */
"IRETURN_NS", /* 0xCE */
"LRETURN_NS", /* 0xCF */
"LPUSH", /* 0xD0 */
"<undef>", /* 0xD1 */
"GETSTATIC_N", /* 0xD2 */
"PUTSTATIC_N", /* 0xD3 */
"GETFIELD_N", /* 0xD4 */
"PUTFIELD_N", /* 0xD5 */
"GETSTATIC_NX", /* 0xD6 */
"PUTSTATIC_NX", /* 0xD7 */
"GETFIELD_NX", /* 0xD8 */
"PUTFIELD_NX", /* 0xD9 */
"NEW_N", /* 0xDA */
"NEWARRAY_N", /* 0xDB */
"<undef>", /* 0xDC */
"<undef>", /* 0xDD */
"<undef>", /* 0xDE */
"<undef>", /* 0xDF */
"<undef>", /* 0xE0 */
"<undef>", /* 0xE1 */
"GETSTATIC_X", /* 0xE2 */
"PUTSTATIC_X", /* 0xE3 */
"GETFIELD_X", /* 0xE4 */
"PUTFIELD_X", /* 0xE5 */
"IFNULL_B", /* 0xE6 */
"IFNONNULL_B", /* 0xE7 */
"IFEQ_B", /* 0xE8 */
"IFNE_B", /* 0xE9 */
"IFLT_B", /* 0xEA */
"IFGE_B", /* 0xEB */
"IFGT_B", /* 0xEC */
"IFLE_B", /* 0xED */
"IF_ICMPEQ_B", /* 0xEE */
"IF_ICMPNE_B", /* 0xEF */
"IF_ICMPLT_B", /* 0xF0 */
"IF_ICMPGE_B", /* 0xF1 */
"IF_ICMPGT_B", /* 0xF2 */
"IF_ICMPLE_B", /* 0xF3 */
"IF_ACMPEQ_B", /* 0xF4 */
"IF_ACMPNE_B", /* 0xF5 */
"GOTO_B", /* 0xF6 */
"INVOKESTATIC_ND", /* 0xF7 */
"INVOKESTATIC_NN", /* 0xF8 */
"INVOKESTATIC_NM", /* 0xF9 */
"INVOKEDIRECT_ND", /* 0xFA */
"INVOKEDIRECT_NN", /* 0xFB */
"INVOKEDIRECT_NM", /* 0xFC */
"INVOKEVIRTUAL_N", /* 0xFD */
"<undef>", /* 0xFE */
"<dropout>", /* 0xFF */

"ILOAD_R0", /* 0x100*/
"ILOAD_R1", /* 0x101*/
"ILOAD_R2", /* 0x102*/
"ILOAD_R3", /* 0x103*/
"ILOAD_R4", /* 0x104*/
"ILOAD_R5", /* 0x105*/
"ILOAD_R6", /* 0x106*/
"ILOAD_R7", /* 0x107*/
"ILOAD_R8", /* 0x108*/
"ILOAD_R9", /* 0x109*/
"ILOAD_R10", /* 0x10A*/
"ILOAD_R11", /* 0x10B*/
"ISTORE_R0", /* 0x10C*/
"ISTORE_R1", /* 0x10D*/
"ISTORE_R2", /* 0x10E*/
"ISTORE_R3", /* 0x10F*/
"SIPUSH_R0", /* 0x110*/
"SIPUSH_R1", /* 0x111*/
"SIPUSH_R2", /* 0x112*/
"SIPUSH_R3", /* 0x113*/
"SIPUSH_R4", /* 0x114*/
"SIPUSH_R5", /* 0x115*/
"SIPUSH_R6", /* 0x116*/
"SIPUSH_R7", /* 0x117*/
"<undef>", /* 0x118*/
"<undef>", /* 0x119*/
"<undef>", /* 0x11A*/
"<undef>", /* 0x11B*/
"IINC_R0", /* 0x11C*/
"IINC_R1", /* 0x11D*/
"IINC_R2", /* 0x11E*/
"IINC_R3", /* 0x11F*/
"GETFIELD_N_R0", /* 0x120*/
"GETFIELD_N_R1", /* 0x121*/
"GETFIELD_N_R2", /* 0x122*/
"GETFIELD_N_R3", /* 0x123*/
"GETFIELD_N_R4", /* 0x124*/
"GETFIELD_N_R5", /* 0x125*/
"GETFIELD_N_R6", /* 0x126*/
"GETFIELD_N_R7", /* 0x127*/
"<undef>", /* 0x128*/
"<undef>", /* 0x129*/
"<undef>", /* 0x12A*/
"<undef>", /* 0x12B*/
"PUTFIELD_N_R0", /* 0x12C*/
"PUTFIELD_N_R1", /* 0x12D*/
"PUTFIELD_N_R2", /* 0x12E*/
"PUTFIELD_N_R3", /* 0x12F*/
"GETSTATIC_N_R0", /* 0x130*/
"GETSTATIC_N_R1", /* 0x131*/
"GETSTATIC_N_R2", /* 0x132*/
"GETSTATIC_N_R3", /* 0x133*/
"GETSTATIC_N_R4", /* 0x134*/
"GETSTATIC_N_R5", /* 0x135*/
"GETSTATIC_N_R6", /* 0x136*/
"GETSTATIC_N_R7", /* 0x137*/
"<undef>", /* 0x138*/
"<undef>", /* 0x139*/
"<undef>", /* 0x13A*/
"<undef>", /* 0x13B*/
"PUTSTATIC_N_R0", /* 0x13C*/
"PUTSTATIC_N_R1", /* 0x13D*/
"PUTSTATIC_N_R2", /* 0x13E*/
"PUTSTATIC_N_R3", /* 0x13F*/
"FADD_R0", /* 0x140*/
"FADD_R1", /* 0x141*/
"FADD_R2", /* 0x142*/
"<undef>", /* 0x143*/
"<undef>", /* 0x144*/
"<undef>", /* 0x145*/
"<undef>", /* 0x146*/
"<undef>", /* 0x147*/
"IADD_R0", /* 0x148*/
"IADD_R1", /* 0x149*/
"IADD_R2", /* 0x14A*/
"<undef>", /* 0x14B*/
"<undef>", /* 0x14C*/
"<undef>", /* 0x14D*/
"<undef>", /* 0x14E*/
"<undef>", /* 0x14F*/
"IASTORE_R0", /* 0x150*/
"IASTORE_R1", /* 0x151*/
"IASTORE_R2", /* 0x152*/
"<undef>", /* 0x153*/
"<undef>", /* 0x154*/
"<undef>", /* 0x155*/
"<undef>", /* 0x156*/
"<undef>", /* 0x157*/
"IALOAD_R0", /* 0x158*/
"IALOAD_R1", /* 0x159*/
"IALOAD_R2", /* 0x15A*/
"IALOAD_R3", /* 0x15B*/
"IALOAD_R4", /* 0x15C*/
"IALOAD_R5", /* 0x15D*/
"<undef>", /* 0x15E*/
"<undef>", /* 0x15F*/
"ILOAD_R12", /* 0x160*/
"ILOAD_R13", /* 0x161*/
"ILOAD_R14", /* 0x162*/
"ILOAD_R15", /* 0x163*/
"ILOAD_R16", /* 0x164*/
"ILOAD_R17", /* 0x165*/
"ILOAD_R18", /* 0x166*/
"ILOAD_R19", /* 0x167*/
"ILOAD_R20", /* 0x168*/
"ILOAD_R21", /* 0x169*/
"ILOAD_R22", /* 0x16A*/
"ILOAD_R23", /* 0x16B*/
"<undef>", /* 0x16C*/
"<undef>", /* 0x16D*/
"<undef>", /* 0x16E*/
"<undef>", /* 0x16F*/
"<undef>", /* 0x170*/
"<undef>", /* 0x171*/
"<undef>", /* 0x172*/
"<undef>", /* 0x173*/
"<undef>", /* 0x174*/
"<undef>", /* 0x175*/
"<undef>", /* 0x176*/
"<undef>", /* 0x177*/
"<undef>", /* 0x178*/
"<undef>", /* 0x179*/
"<undef>", /* 0x17A*/
"<undef>", /* 0x17B*/
"<undef>", /* 0x17C*/
"<undef>", /* 0x17D*/
"<undef>", /* 0x17E*/
"<undef>", /* 0x17F*/
"<undef>", /* 0x180*/
"<undef>", /* 0x181*/
"<undef>", /* 0x182*/
"<undef>", /* 0x183*/
"<undef>", /* 0x184*/
"<undef>", /* 0x185*/
"<undef>", /* 0x186*/
"<undef>", /* 0x187*/
"<undef>", /* 0x188*/
"<undef>", /* 0x189*/
"<undef>", /* 0x18A*/
"<undef>", /* 0x18B*/
"<undef>", /* 0x18C*/
"<undef>", /* 0x18D*/
"<undef>", /* 0x18E*/
"<undef>", /* 0x18F*/
"<undef>", /* 0x190*/
"<undef>", /* 0x191*/
"<undef>", /* 0x192*/
"<undef>", /* 0x193*/
"<undef>", /* 0x194*/
"<undef>", /* 0x195*/
"<undef>", /* 0x196*/
"<undef>", /* 0x197*/
"<undef>", /* 0x198*/
"<undef>", /* 0x199*/
"<undef>", /* 0x19A*/
"<undef>", /* 0x19B*/
"<undef>", /* 0x19C*/
"<undef>", /* 0x19D*/
"<undef>", /* 0x19E*/
"<undef>", /* 0x19F*/
"<undef>", /* 0x1A0*/
"<undef>", /* 0x1A1*/
"<undef>", /* 0x1A2*/
"<undef>", /* 0x1A3*/
"<undef>", /* 0x1A4*/
"<undef>", /* 0x1A5*/
"<undef>", /* 0x1A6*/
"<undef>", /* 0x1A7*/
"<undef>", /* 0x1A8*/
"<undef>", /* 0x1A9*/
"<undef>", /* 0x1AA*/
"<undef>", /* 0x1AB*/
"<undef>", /* 0x1AC*/
"<undef>", /* 0x1AD*/
"<undef>", /* 0x1AE*/
"<undef>", /* 0x1AF*/
"<undef>", /* 0x1B0*/
"<undef>", /* 0x1B1*/
"<undef>", /* 0x1B2*/
"<undef>", /* 0x1B3*/
"<undef>", /* 0x1B4*/
"<undef>", /* 0x1B5*/
"<undef>", /* 0x1B6*/
"<undef>", /* 0x1B7*/
"<undef>", /* 0x1B8*/
"<undef>", /* 0x1B9*/
"<undef>", /* 0x1BA*/
"<undef>", /* 0x1BB*/
"<undef>", /* 0x1BC*/
"<undef>", /* 0x1BD*/
"<undef>", /* 0x1BE*/
"<undef>", /* 0x1BF*/
"<undef>", /* 0x1C0*/
"<undef>", /* 0x1C1*/
"<undef>", /* 0x1C2*/
"<undef>", /* 0x1C3*/
"<undef>", /* 0x1C4*/
"<undef>", /* 0x1C5*/
"<undef>", /* 0x1C6*/
"<undef>", /* 0x1C7*/
"<undef>", /* 0x1C8*/
"<undef>", /* 0x1C9*/
"<undef>", /* 0x1CA*/
"<undef>", /* 0x1CB*/
"<undef>", /* 0x1CC*/
"<undef>", /* 0x1CD*/
"<undef>", /* 0x1CE*/
"<undef>", /* 0x1CF*/
"<undef>", /* 0x1D0*/
"<undef>", /* 0x1D1*/
"<undef>", /* 0x1D2*/
"<undef>", /* 0x1D3*/
"<undef>", /* 0x1D4*/
"<undef>", /* 0x1D5*/
"<undef>", /* 0x1D6*/
"<undef>", /* 0x1D7*/
"<undef>", /* 0x1D8*/
"<undef>", /* 0x1D9*/
"<undef>", /* 0x1DA*/
"<undef>", /* 0x1DB*/
"<undef>", /* 0x1DC*/
"<undef>", /* 0x1DD*/
"<undef>", /* 0x1DE*/
"<undef>", /* 0x1DF*/
"<undef>", /* 0x1E0*/
"<undef>", /* 0x1E1*/
"<undef>", /* 0x1E2*/
"<undef>", /* 0x1E3*/
"<undef>", /* 0x1E4*/
"<undef>", /* 0x1E5*/
"<undef>", /* 0x1E6*/
"<undef>", /* 0x1E7*/
"<undef>", /* 0x1E8*/
"<undef>", /* 0x1E9*/
"<undef>", /* 0x1EA*/
"<undef>", /* 0x1EB*/
"<undef>", /* 0x1EC*/
"<undef>", /* 0x1ED*/
"<undef>", /* 0x1EE*/
"<undef>", /* 0x1EF*/
"NOP_R0", /* 0x1F0*/
"NOP_R1", /* 0x1F1*/
"NOP_R2", /* 0x1F2*/
"<undef>", /* 0x1F3*/
"<undef>", /* 0x1F4*/
"<undef>", /* 0x1F5*/
"<undef>", /* 0x1F6*/
"<undef>", /* 0x1F7*/
"<undef>", /* 0x1F8*/
"<undef>", /* 0x1F9*/
"<undef>", /* 0x1FA*/
"<undef>", /* 0x1FB*/
"<undef>", /* 0x1FC*/
"<undef>", /* 0x1FD*/
"<undef>", /* 0x1FE*/
"<none>"  /* 0x1FF*/
};

void fy_instInitStackItem(fy_memblock *block, fy_instruction_extra *instruction,
		fy_int size, fy_exception *exception) {
	instruction->sp = size;
	if (size <= 64) {
		instruction->s.stackTypeContent = 0;
	} else {
		instruction->s.stackTypeContents = fy_mmAllocatePerm(block,
				sizeof(fy_ulong) * ((size + 63) >> 6), exception);
	}
}

static FY_INLINE fy_int imin(fy_int a, fy_int b) {
	return a < b ? a : b;
}

void fy_instStackItemClone(fy_instruction_extra *from, fy_instruction_extra *to) {
	fy_int size;
	if (from->sp <= 64 && to->sp <= 64) {
		to->s.stackTypeContent = from->s.stackTypeContent;
	} else if (from->sp > 64 && to->sp <= 64) {
		to->s.stackTypeContent = from->s.stackTypeContents[0];
	} else if (from->sp <= 64 && to->sp > 64) {
		to->s.stackTypeContents[0] = from->s.stackTypeContent;
	} else {
		size = (imin(from->sp, to->sp) + 63) >> 6;
		memcpy(to->s.stackTypeContents, from->s.stackTypeContents,
				sizeof(fy_ulong) * size);
	}
#ifdef FY_STRICT_CHECK
	to->localSize = from->localSize;
#endif
}

void fy_instMarkStackItem(fy_instruction_extra *instruction, fy_int pos,
		fy_long isHandle) {
	fy_ulong mkOr;
	fy_ulong mkAnd;
	fy_int lpos;
	if (instruction->sp <= 64) {
		/*happy*/
		mkOr = (((fy_ulong)1) << pos) & isHandle;
		mkAnd = isHandle | ~(((fy_ulong)1) << pos);

		instruction->s.stackTypeContent = (instruction->s.stackTypeContent
				& mkAnd) | mkOr;
	} else {
		/*not so happy*/
		lpos = pos >> 6; /* /64 */
		pos = pos & 63;
		mkOr = (((fy_ulong)1) << pos) & isHandle;
		mkAnd = isHandle | ~(((fy_ulong)1) << pos);
		instruction->s.stackTypeContents[lpos] =
				(instruction->s.stackTypeContents[lpos] & mkAnd) | mkOr;
	}
}

fy_int fy_instGetStackItem(fy_instruction_extra *instruction, fy_int pos) {
	if (instruction->sp <= 64) {
		return -((instruction->s.stackTypeContent >> pos) & 1);
	} else {
		return -((instruction->s.stackTypeContents[pos >> 6] >> (pos & 63)) & 1);
	}
}


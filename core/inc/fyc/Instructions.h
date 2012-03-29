/*
 * Instructions.h
 *
 *  Created on: Mar 28, 2012
 *      Author: Cloudee
 */

#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#define NOP  0x00
#define ACONST_NULL  0x01
#define ICONST_M1  0x02
#define ICONST_0  0x03
#define ICONST_1  0x04
#define ICONST_2  0x05
#define ICONST_3  0x06
#define ICONST_4  0x07

#define ICONST_5  0x08
#define LCONST_0  0x09
#define LCONST_1  0x0A
#define FCONST_0  0x0B
#define FCONST_1  0x0C
#define FCONST_2  0x0D
#define DCONST_0  0x0E
#define DCONST_1  0x0F

#define BIPUSH  0x10
#define SIPUSH  0x11
#define LDC  0x12
#define LDC_W  0x13
#define LDC2_W  0x14
#define ILOAD  0x15
#define LLOAD  0x16
#define FLOAD  0x17

#define DLOAD  0x18
#define ALOAD  0x19
#define ILOAD_0  0x1A
#define ILOAD_1  0x1B
#define ILOAD_2  0x1C
#define ILOAD_3  0x1D
#define LLOAD_0  0x1E
#define LLOAD_1  0x1F

#define LLOAD_2  0x20
#define LLOAD_3  0x21
#define FLOAD_0  0x22
#define FLOAD_1  0x23
#define FLOAD_2  0x24
#define FLOAD_3  0x25
#define DLOAD_0  0x26
#define DLOAD_1  0x27

#define DLOAD_2  0x28
#define DLOAD_3  0x29
#define ALOAD_0  0x2A
#define ALOAD_1  0x2B
#define ALOAD_2  0x2C
#define ALOAD_3  0x2D
#define IALOAD  0x2E
#define LALOAD  0x2F

#define FALOAD  0x30
#define DALOAD  0x31
#define AALOAD  0x32
#define BALOAD  0x33
#define CALOAD  0x34
#define SALOAD  0x35
#define ISTORE  0x36
#define LSTORE  0x37

#define FSTORE  0x38
#define DSTORE  0x39
#define ASTORE  0x3A
#define ISTORE_0  0x3B
#define ISTORE_1  0x3C
#define ISTORE_2  0x3D
#define ISTORE_3  0x3E
#define LSTORE_0  0x3F

#define LSTORE_1  0x40
#define LSTORE_2  0x41
#define LSTORE_3  0x42
#define FSTORE_0  0x43
#define FSTORE_1  0x44
#define FSTORE_2  0x45
#define FSTORE_3  0x46
#define DSTORE_0  0x47

#define DSTORE_1  0x48
#define DSTORE_2  0x49
#define DSTORE_3  0x4A
#define ASTORE_0  0x4B
#define ASTORE_1  0x4C
#define ASTORE_2  0x4D
#define ASTORE_3  0x4E
#define IASTORE  0x4F

#define LASTORE  0x50
#define FASTORE  0x51
#define DASTORE  0x52
#define AASTORE  0x53
#define BASTORE  0x54
#define CASTORE  0x55
#define SASTORE  0x56
#define POP  0x57

#define POP2  0x58
#define DUP  0x59
#define DUP_X1  0x5A
#define DUP_X2  0x5B
#define DUP2  0x5C
#define DUP2_X1  0x5D
#define DUP2_X2  0x5E
#define SWAP  0x5F

#define IADD  0x60
#define LADD  0x61
#define FADD  0x62
#define DADD  0x63
#define ISUB  0x64
#define LSUB  0x65
#define FSUB  0x66
#define DSUB  0x67

#define IMUL  0x68
#define LMUL  0x69
#define FMUL  0x6A
#define DMUL  0x6B
#define IDIV  0x6C
#define LDIV  0x6D
#define FDIV  0x6E
#define DDIV  0x6F

#define IREM  0x70
#define LREM  0x71
#define FREM  0x72
#define DREM  0x73
#define INEG  0x74
#define LNEG  0x75
#define FNEG  0x76
#define DNEG  0x77

#define ISHL  0x78
#define LSHL  0x79
#define ISHR  0x7A
#define LSHR  0x7B
#define IUSHR  0x7C
#define LUSHR  0x7D
#define IAND  0x7E
#define LAND  0x7F

#define IOR  0x80
#define LOR  0x81
#define IXOR  0x82
#define LXOR  0x83
#define IINC  0x84
#define I2L  0x85
#define I2F  0x86
#define I2D  0x87

#define L2I  0x88
#define L2F  0x89
#define L2D  0x8A
#define F2I  0x8B
#define F2L  0x8C
#define F2D  0x8D
#define D2I  0x8E
#define D2L  0x8F

#define D2F  0x90
#define I2B  0x91
#define I2C  0x92
#define I2S  0x93
#define LCMP  0x94
#define FCMPL  0x95
#define FCMPG  0x96
#define DCMPL  0x97

#define DCMPG  0x98
#define IFEQ  0x99
#define IFNE  0x9A
#define IFLT  0x9B
#define IFGE  0x9C
#define IFGT  0x9D
#define IFLE  0x9E
#define IF_ICMPEQ  0x9F

#define IF_ICMPNE  0xA0
#define IF_ICMPLT  0xA1
#define IF_ICMPGE  0xA2
#define IF_ICMPGT  0xA3
#define IF_ICMPLE  0xA4
#define IF_ACMPEQ  0xA5
#define IF_ACMPNE  0xA6
#define GOTO  0xA7

#define JSR  0xA8
#define RET  0xA9
#define TABLESWITCH  0xAA
#define LOOKUPSWITCH  0xAB
#define IRETURN  0xAC
#define LRETURN  0xAD
#define FRETURN  0xAE
#define DRETURN  0xAF

#define ARETURN  0xB0
#define RETURN  0xB1
#define GETSTATIC  0xB2
#define PUTSTATIC  0xB3
#define GETFIELD  0xB4
#define PUTFIELD  0xB5
#define INVOKEVIRTUAL  0xB6
#define INVOKESPECIAL  0xB7

#define INVOKESTATIC  0xB8
#define INVOKEINTERFACE  0xB9
#define UNUSED_BA  0xBA
#define NEW  0xBB
#define NEWARRAY  0xBC
#define ANEWARRAY  0xBD
#define ARRAYLENGTH  0xBE
#define ATHROW  0xBF

#define CHECKCAST  0xC0
#define INSTANCEOF  0xC1
#define MONITORENTER  0xC2
#define MONITOREXIT  0xC3
#define WIDE  0xC4
#define MULTIANEWARRAY  0xC5
#define IFNULL  0xC6
#define IFNONNULL  0xC7

#define GOTO_W  0xC8
#define JSR_W  0xC9
#define BREAKPOINT  0xCA

#define fy_nextU1(CODE) (fy_uint)CODE[pc++]
#define fy_nextS1(CODE) ((fy_byte)CODE[pc++])
#define fy_nextU2(CODE) (fy_B2TOI(CODE[pc],CODE[pc+1])&0xffff);pc+=2
#define fy_nextS2(CODE) fy_B2TOI(CODE[pc],CODE[pc+1]);pc+=2
#define fy_nextS4(CODE) fy_B4TOI(CODE[pc],CODE[pc+1],CODE[pc+2],CODE[pc+3]);pc+=4

extern const char *FY_OP_NAME[256];


#endif /* INSTRUCTIONS_H_ */

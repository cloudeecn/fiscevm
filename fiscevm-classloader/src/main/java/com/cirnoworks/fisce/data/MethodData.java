/**
 *  Copyright 2010 Yuxuan Huang. All rights reserved.
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.cirnoworks.fisce.data;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;

import org.objectweb.asm.Label;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.FieldInsnNode;
import org.objectweb.asm.tree.IincInsnNode;
import org.objectweb.asm.tree.IntInsnNode;
import org.objectweb.asm.tree.JumpInsnNode;
import org.objectweb.asm.tree.LabelNode;
import org.objectweb.asm.tree.LdcInsnNode;
import org.objectweb.asm.tree.LineNumberNode;
import org.objectweb.asm.tree.LookupSwitchInsnNode;
import org.objectweb.asm.tree.MethodInsnNode;
import org.objectweb.asm.tree.MethodNode;
import org.objectweb.asm.tree.MultiANewArrayInsnNode;
import org.objectweb.asm.tree.TableSwitchInsnNode;
import org.objectweb.asm.tree.TryCatchBlockNode;
import org.objectweb.asm.tree.TypeInsnNode;
import org.objectweb.asm.tree.VarInsnNode;
import org.objectweb.asm.tree.analysis.Analyzer;
import org.objectweb.asm.tree.analysis.AnalyzerException;
import org.objectweb.asm.tree.analysis.BasicValue;
import org.objectweb.asm.tree.analysis.BasicVerifier;
import org.objectweb.asm.tree.analysis.Frame;

import com.cirnoworks.fisce.classloader.utils.DescriptorAnalyzer;
import com.cirnoworks.fisce.data.constants.ConstantClassData;
import com.cirnoworks.fisce.data.constants.ConstantData;
import com.cirnoworks.fisce.data.constants.ConstantDoubleData;
import com.cirnoworks.fisce.data.constants.ConstantFloatData;
import com.cirnoworks.fisce.data.constants.ConstantIntegerData;
import com.cirnoworks.fisce.data.constants.ConstantLongData;
import com.cirnoworks.fisce.data.constants.ConstantReferenceData;
import com.cirnoworks.fisce.data.constants.ConstantStringData;
import com.cirnoworks.fisce.vm.data.attributes.ExceptionHandler;
import com.cirnoworks.fisce.vm.data.attributes.LineNumber;

/**
 * 
 * @author yuxuanhuang
 */
public final class MethodData extends MethodNode {

	private static final Analyzer<BasicValue> analyzer = new Analyzer<BasicValue>(
			new BasicVerifier());

	public MethodData(ClassData owner, int access, String name, String desc,
			String signature, String[] exceptions) {
		super(access, name, desc, signature, exceptions);
		this.owner = owner;
	}

	private ClassData owner;
	private int[] code;

	private int paramCount;
	private byte[] paramType;
	private byte returnType;

	private int parameterCount;
	private String[] parameterTypeClassNames;
	private String returnTypeClassName;

	private Frame<BasicValue>[] rawFrames;
	private AbstractInsnNode[] rawInstructions;
	private boolean[] hintFrame;
	private boolean[] checkOps;
	private ArrayList<LookupSwitchTarget> lookupSwitchTargets = new ArrayList<LookupSwitchTarget>();
	private Map<Integer, Integer> lookupSwitchTargetMap = new TreeMap<Integer, Integer>();
	private ArrayList<TableSwitchTarget> tableSwitchTargets = new ArrayList<TableSwitchTarget>();
	private Map<Integer, Integer> tableSwitchTargetMap = new TreeMap<Integer, Integer>();
	private ExceptionHandler[] exceptionTable;
	private LineNumber[] lineNumberTable;

	private static boolean isAllocateInst(int op) {
		return op == Opcodes.NEW || op == Opcodes.NEWARRAY
				|| op == Opcodes.MULTIANEWARRAY;
	}

	public int getAccessFlags() {
		return access;
	}

	public String getName() {
		return name;
	}

	public String getDescriptor() {
		return desc;
	}

	public int getMaxStack() {
		return maxStack;
	}

	public int getMaxLocals() {
		return maxLocals;
	}

	public int[] getCode() {

		return code;
	}

	public int getParamCount() {
		return paramCount;
	}

	public byte[] getParamType() {
		return paramType;
	}

	public byte getReturnType() {
		return returnType;
	}

	public int getParameterCount() {
		return parameterCount;
	}

	public String[] getParameterTypeClassNames() {
		return parameterTypeClassNames;
	}

	public String getReturnTypeClassName() {
		return returnTypeClassName;
	}

	public void visitEnd() {
		super.visitEnd();

//		System.out.println(owner.name + "." + name + "." + desc);

		DescriptorAnalyzer da = new DescriptorAnalyzer(desc);
		returnType = da.getReturnType();
		returnTypeClassName = da.getReturnClassName();

		paramCount = da.getParamStackUsage();
		paramType = da.getParamTypes();

		parameterCount = da.getParamCount();
		parameterTypeClassNames = da.getParamClassNames();

		try {
			if ((access & Opcodes.ACC_NATIVE) == 0
					&& (access & Opcodes.ACC_ABSTRACT) == 0) {

				int codeLength = instructions.size();
				if (codeLength == 0) {
					throw new RuntimeException("Non-native/abstract method "
							+ owner.name + "." + name + "." + desc
							+ " has no code");
				}
				Frame<BasicValue>[] frames;
				synchronized (analyzer) {
					frames = analyzer.analyze(owner.name, this);
				}

				int ip = 0;
				int[] targetMap = new int[codeLength];
				rawInstructions = new AbstractInsnNode[codeLength];
				rawFrames = new Frame[codeLength];
				Map<Label, Integer> ipMapper = new HashMap<Label, Integer>();
				Label[] labelled = new Label[codeLength];
				Label currentLabel = null;

				ArrayList<Entry<Label, Integer>> tmpLineNumbers = new ArrayList<Entry<Label, Integer>>();
				for (int i = 0; i < codeLength; i++) {
					AbstractInsnNode inst = instructions.get(i);
					if (inst.getType() == AbstractInsnNode.LABEL) {
						currentLabel = ((LabelNode) inst).getLabel();
					} else if (inst.getType() == AbstractInsnNode.LINE) {
						LineNumberNode lnn = (LineNumberNode) inst;
						tmpLineNumbers
								.add(new SimpleImmutableEntry<Label, Integer>(
										lnn.start.getLabel(), lnn.line));
					} else if (inst.getOpcode() >= 0) {
						targetMap[i] = ip;
						if (currentLabel != null) {
							ipMapper.put(currentLabel, ip);
							labelled[ip] = currentLabel;
							currentLabel = null;
						}

						rawInstructions[ip] = inst;
						rawFrames[ip] = frames[i];
						ip++;
					}
				}
				int newLength = ip;
				hintFrame = new boolean[newLength];
				checkOps = new boolean[newLength];
				hintFrame[0] = true;
				for (ip = 0; ip < newLength; ip++) {
					AbstractInsnNode inst = rawInstructions[ip];
					if (ip % 50 == 49) {
						hintFrame[ip] = true;
						checkOps[ip] = true;
					}

					switch (inst.getType()) {
					case AbstractInsnNode.JUMP_INSN:
						int target = ipMapper.get(((JumpInsnNode) inst).label
								.getLabel());
						// 往回跳的检查一下剩余的指令数量
						if (target < ip) {
							hintFrame[ip] = true;
							checkOps[ip] = true;
						}
						break;
					case AbstractInsnNode.METHOD_INSN:
					case AbstractInsnNode.FIELD_INSN:
						hintFrame[ip] = true;
						break;
					case AbstractInsnNode.LOOKUPSWITCH_INSN:
						LookupSwitchInsnNode lsin = (LookupSwitchInsnNode) inst;
						LookupSwitchTarget lookupSwitchTarget = new LookupSwitchTarget();

						lookupSwitchTarget.setDefaultTarget(ipMapper
								.get(lsin.dflt.getLabel()));
						lookupSwitchTarget.setKeys(new int[lsin.keys.size()]);
						lookupSwitchTarget
								.setTargets(new int[lsin.keys.size()]);
						for (int i = 0, max = lsin.keys.size(); i < max; i++) {
							Integer key = lsin.keys.get(i);
							Integer targetIp = ipMapper.get(lsin.labels.get(i)
									.getLabel());
							lookupSwitchTarget.getKeys()[i] = key;
							lookupSwitchTarget.getTargets()[i] = targetIp;
						}
						lookupSwitchTargetMap.put(ip,
								lookupSwitchTargets.size());
						lookupSwitchTargets.add(lookupSwitchTarget);
						break;
					case AbstractInsnNode.TABLESWITCH_INSN:
						TableSwitchInsnNode tsin = (TableSwitchInsnNode) inst;
						TableSwitchTarget tableSwitchTarget = new TableSwitchTarget();
						tableSwitchTarget
								.setTargets(new int[tsin.labels.size()]);
						if (tsin.max - tsin.min + 1 != tsin.labels.size()) {
							throw new RuntimeException();
						}
						tableSwitchTarget.setDefaultTarget(ipMapper
								.get(tsin.dflt.getLabel()));
						tableSwitchTarget.setMin(tsin.min);
						tableSwitchTarget.setMax(tsin.max);
						for (int i = 0, max = tsin.labels.size(); i < max; i++) {
							tableSwitchTarget.getTargets()[i] = ipMapper
									.get(tsin.labels.get(i).getLabel());
						}
						tableSwitchTargetMap.put(ip, tableSwitchTargets.size());
						tableSwitchTargets.add(tableSwitchTarget);
						break;
					}

					if (isAllocateInst(inst.getOpcode())) {
						hintFrame[ip] = true;
					}
				}

				if (tmpLineNumbers.size() > 0) {
					lineNumberTable = new LineNumber[tmpLineNumbers.size()];
					for (int i = 0, max = lineNumberTable.length; i < max; i++) {
						LineNumber ln = lineNumberTable[i] = new LineNumber();
						Entry<Label, Integer> tmpLineNumber = tmpLineNumbers
								.get(i);
						ln.startPc = (char) ipMapper
								.get(tmpLineNumber.getKey()).intValue();
						ln.lineNumber = (char) tmpLineNumber.getValue()
								.intValue();
					}
				}

				if (tryCatchBlocks.size() > 0) {
					exceptionTable = new ExceptionHandler[tryCatchBlocks.size()];
					for (int i = 0, max = exceptionTable.length; i < max; i++) {
						ExceptionHandler eh = exceptionTable[i] = new ExceptionHandler();
						TryCatchBlockNode tcbn = tryCatchBlocks.get(i);
						eh.startPc = ipMapper.get(tcbn.start.getLabel());
						eh.endPc = ipMapper.get(tcbn.end.getLabel());
						eh.handlerPc = ipMapper.get(tcbn.handler.getLabel());
						eh.catchClassIdx = tcbn.type == null ? 0 : owner
								.getConstantIdByClassName(tcbn.type);
					}
				}

				code = new int[newLength * 3];
				for (ip = 0; ip < newLength; ip++) {
					AbstractInsnNode inst = rawInstructions[ip];
					int base = ip * 3;
					code[base] = inst.getOpcode();
					switch (inst.getType()) {
					case AbstractInsnNode.FIELD_INSN:
						FieldInsnNode fin = (FieldInsnNode) inst;
						ConstantReferenceData crdf = new ConstantReferenceData();
						crdf.setClassName(fin.owner);
						crdf.setName(fin.name);
						crdf.setDescriptior(fin.desc);
						code[base + 1] = owner.getConstantIdByReference(crdf);
						break;
					case AbstractInsnNode.IINC_INSN:
						IincInsnNode iin = (IincInsnNode) inst;
						code[base + 1] = iin.var;
						code[base + 2] = iin.incr;
						break;
					case AbstractInsnNode.INT_INSN:
						IntInsnNode inin = (IntInsnNode) inst;
						code[base + 1] = inin.operand;
						break;
					case AbstractInsnNode.INSN:
						break;
					case AbstractInsnNode.JUMP_INSN:
						JumpInsnNode jin = (JumpInsnNode) inst;
						code[base + 1] = ipMapper.get(jin.label.getLabel());
						break;
					case AbstractInsnNode.LDC_INSN:
						LdcInsnNode lin = (LdcInsnNode) inst;
						code[base + 1] = owner.getConstantId(lin.cst);
						ConstantData cd = owner.getConstantPool()[code[base + 1]];
						if (cd == null) {
							throw new IllegalStateException(
									"LDC with null constant " + code[base + 1]
											+ "/" + lin.cst.getClass() + ": "
											+ lin.cst);
						}
						if (cd instanceof ConstantIntegerData) {
							code[base + 2] = 0;
						} else if (cd instanceof ConstantFloatData) {
							code[base + 2] = 0;
						} else if (cd instanceof ConstantLongData) {
							code[base + 2] = 1;
						} else if (cd instanceof ConstantDoubleData) {
							code[base + 2] = 1;
						} else if (cd instanceof ConstantStringData) {
							code[base + 2] = 2;
						} else if (cd instanceof ConstantClassData) {
							code[base + 2] = 3;
						} else {
							throw new IllegalStateException(
									"LDC with constant "
											+ cd.getClass().getName()
											+ " is not supported.");
						}
						break;
					case AbstractInsnNode.METHOD_INSN:
						MethodInsnNode min = (MethodInsnNode) inst;
						ConstantReferenceData crdm = new ConstantReferenceData();
						crdm.setClassName(min.owner);
						crdm.setName(min.name);
						crdm.setDescriptior(min.desc);
						code[base + 1] = owner.getConstantIdByReference(crdm);
						break;
					case AbstractInsnNode.MULTIANEWARRAY_INSN:
						MultiANewArrayInsnNode manain = (MultiANewArrayInsnNode) inst;
						code[base + 1] = owner
								.getConstantIdByClassName(manain.desc);
						code[base + 2] = manain.dims;
						break;
					case AbstractInsnNode.LOOKUPSWITCH_INSN:
						code[base + 1] = lookupSwitchTargetMap.get(ip);
						break;
					case AbstractInsnNode.TABLESWITCH_INSN:
						code[base + 1] = tableSwitchTargetMap.get(ip);
						break;
					case AbstractInsnNode.TYPE_INSN:
						TypeInsnNode tin = (TypeInsnNode) inst;
						code[base + 1] = owner
								.getConstantIdByClassName(tin.desc);
						break;
					case AbstractInsnNode.VAR_INSN:
						VarInsnNode vin = (VarInsnNode) inst;
						code[base + 1] = vin.var;
						break;
					default:
						throw new RuntimeException("Unknown instruction type "
								+ inst.getType());
					}
				}
			}

		} catch (AnalyzerException e) {
			throw new RuntimeException(e);
		}
	}

	public Frame<BasicValue>[] getRawFrames() {
		return rawFrames;
	}

	public AbstractInsnNode[] getRawInstructions() {
		return rawInstructions;
	}

	public ExceptionHandler[] getExceptionTable() {
		return exceptionTable;
	}

	public LineNumber[] getLineNumberTable() {
		return lineNumberTable;
	}

	public boolean[] getHintFrame() {
		return hintFrame;
	}

	public boolean[] getCheckOps() {
		return checkOps;
	}

	public ArrayList<TableSwitchTarget> getTableSwitchTargets() {
		return tableSwitchTargets;
	}

	public ArrayList<LookupSwitchTarget> getLookupSwitchTargets() {
		return lookupSwitchTargets;
	}

}

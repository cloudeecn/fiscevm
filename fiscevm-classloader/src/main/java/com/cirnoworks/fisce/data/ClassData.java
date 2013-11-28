package com.cirnoworks.fisce.data;

import java.util.HashMap;
import java.util.Map;

import org.objectweb.asm.ClassReader;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.tree.ClassNode;
import org.objectweb.asm.tree.FieldNode;
import org.objectweb.asm.tree.MethodNode;

import com.cirnoworks.fisce.data.constants.ConstantClassData;
import com.cirnoworks.fisce.data.constants.ConstantData;
import com.cirnoworks.fisce.data.constants.ConstantDoubleData;
import com.cirnoworks.fisce.data.constants.ConstantFloatData;
import com.cirnoworks.fisce.data.constants.ConstantIntegerData;
import com.cirnoworks.fisce.data.constants.ConstantLongData;
import com.cirnoworks.fisce.data.constants.ConstantReferenceData;
import com.cirnoworks.fisce.data.constants.ConstantStringData;
import com.cirnoworks.fisce.data.constants.internal.ConstantNameTypeInfoData;

public class ClassData extends ClassNode {

	private static int getSizeFromDescriptor(String descriptor) {
		switch (descriptor.charAt(0)) {
		case 'J':
		case 'D':
			return 2;
		default:
			return 1;
		}
	}

	private ConstantData[] constantPool;
	private int superClassInfoIndex;
	private int[] interfaceInfoIndexs;

	private FieldData[] fieldDatas;
	private MethodData[] methodDatas;

	protected int sizeInHeap;
	protected int sizeInStatic;

	protected final ClassReader cr;
	private Map<Object, Integer> ldcTable = new HashMap<Object, Integer>();
	private Map<String, Integer> ldcClassTable = new HashMap<String, Integer>();
	private Map<ConstantReferenceData, Integer> referenceMap = new HashMap<ConstantReferenceData, Integer>();

	public ClassData(ClassReader cr) {
		this.cr = cr;
	}

	public ConstantData[] getConstantPool() {
		return constantPool;
	}

	public String getName() {
		return name;
	}

	public String getSourceFile() {
		return sourceFile;
	}

	public void visitEnd() {
		super.visitEnd();
		constantPool = new ConstantData[cr.getItemCount()];
		byte[] b = cr.b;
		char[] cbuf = new char[cr.getMaxStringLength()];
		for (int i = 1, max = cr.getItemCount(); i < max; i++) {
			int begin = cr.getItem(i);
			int tag = b[begin - 1];
			switch (tag) {
			case 1: // utf-8;
			{
				break;
			}
			case 3: {
				ConstantIntegerData c = new ConstantIntegerData();
				c.setData(cr.readInt(begin));
				constantPool[i] = c;
				ldcTable.put(c.getData(), i);
				break;
			}
			case 4: {
				ConstantFloatData c = new ConstantFloatData();
				c.setData(Float.intBitsToFloat(cr.readInt(begin)));
				constantPool[i] = c;
				ldcTable.put(c.getData(), i);
				break;
			}
			case 5: {
				ConstantLongData c = new ConstantLongData();
				c.setData(cr.readLong(begin));
				constantPool[i] = c;
				ldcTable.put(c.getData(), i);
				i++;
				break;
			}
			case 6: {
				ConstantDoubleData c = new ConstantDoubleData();
				c.setData(Double.longBitsToDouble(cr.readLong(begin)));
				constantPool[i] = c;
				ldcTable.put(c.getData(), i);
				i++;
				break;
			}
			case 7: {
				ConstantClassData c = new ConstantClassData();
				c.setName(cr.readUTF8(begin, cbuf));
				constantPool[i] = c;
				ldcClassTable.put(c.getName(), i);
				break;
			}
			case 8: {
				ConstantStringData c = new ConstantStringData();
				c.setStr(cr.readUTF8(begin, cbuf));
				constantPool[i] = c;
				ldcTable.put(c.getStr(), i);
				break;
			}
			case 9:
			case 10:
			case 11: {
				ConstantReferenceData c = new ConstantReferenceData();
				c.setClassName(cr.readClass(begin, cbuf));
				c.setNameAndTypeIdx(cr.readUnsignedShort(begin + 2));
				constantPool[i] = c;
				break;
			}
			case 12: {
				ConstantNameTypeInfoData c = new ConstantNameTypeInfoData();
				c.setName(cr.readUTF8(begin, cbuf));
				c.setDescriptor(cr.readUTF8(begin + 2, cbuf));
				constantPool[i] = c;
				break;
			}
			default: {
				throw new RuntimeException("Unknown tag type " + tag);
			}

			}
		}
		for (int i = 1, max = cr.getItemCount(); i < max; i++) {
			ConstantData cd = constantPool[i];
			if (cd instanceof ConstantReferenceData) {
				ConstantReferenceData crd = (ConstantReferenceData) cd;
				crd.fillConstants(constantPool);
				if (this.referenceMap.containsKey(crd)) {
					// throw new
					// RuntimeException("Duplicated reference constant "
					// + crd.getClassName() + "." + crd.getName() + "."
					// + crd.getDescriptior());
				} else {
					referenceMap.put(crd, i);
				}
			}
		}

		int superPos = cr.header + 4;
		int intfPos = cr.header + 6;
		this.superClassInfoIndex = cr.readUnsignedShort(superPos);
		this.interfaceInfoIndexs = new int[cr.readUnsignedShort(intfPos)];
		for (int i = 0, max = this.interfaceInfoIndexs.length; i < max; i++) {
			this.interfaceInfoIndexs[i] = cr.readUnsignedShort(intfPos + i * 2
					+ 2);
		}

		fieldDatas = new FieldData[super.fields.size()];

		int fieldPos = 0;
		int staticPos = 0;
		for (int i = 0, max = fieldDatas.length; i < max; i++) {
			FieldNode node = super.fields.get(i);
			FieldData field = new FieldData(node);

			int length = getSizeFromDescriptor(field.getDescriptor());
			field.setLength(length);
			if ((field.getAccessFlags() & Opcodes.ACC_STATIC) == 0) {
				field.setPosition(fieldPos);
				fieldPos += length;
			} else {
				field.setPosition(staticPos);
				staticPos += length;
			}

			if (node.value != null) {
				int cid = getConstantId(node.value);
				field.setConstantValueIndex((char) cid);
			}

			fieldDatas[i] = field;
		}

		sizeInHeap = fieldPos;
		sizeInStatic = staticPos;

		methodDatas = new MethodData[methods.size()];
		for (int i = 0, max = methods.size(); i < max; i++) {
			MethodNode mn = methods.get(i);
			MethodData md = new MethodData(this, mn.access, mn.name, mn.desc,
					mn.signature,
					mn.exceptions.toArray(new String[mn.exceptions.size()]));
			mn.accept(md);
			methodDatas[i] = md;
		}

	}

	public int getAccessFlags() {
		return access;
	}

	public int getSuperClassInfoIndex() {
		return this.superClassInfoIndex;
	}

	public int[] getInterfaceInfoIndexs() {
		return this.interfaceInfoIndexs;
	}

	public FieldData[] getFields() {
		return fieldDatas;
	}

	public int getSizeInHeap() {
		return sizeInHeap;
	}

	public int getSizeInStatic() {
		return sizeInStatic;
	}

	public MethodData[] getMethods() {
		return methodDatas;
	}

	public int getConstantIdByClassName(String className) {
		Integer ret = ldcClassTable.get(className);
		if (ret == null) {
			throw new RuntimeException("Can't find content id for class name "
					+ className);
		}
		return ret;
	}

	public int getConstantIdByReference(ConstantReferenceData crd) {
		if (!referenceMap.containsKey(crd)) {
			throw new RuntimeException("Can't find reference constant "
					+ crd.getClassName() + "." + crd.getName() + "."
					+ crd.getDescriptior());
		}
		return referenceMap.get(crd);
	}

	public int getConstantId(Object ldcValue) {
		Integer ret;
		if (ldcValue instanceof Type) {
			Type type = (Type) ldcValue;
			switch (type.getSort()) {
			case Type.ARRAY:
			case Type.OBJECT:
				return getConstantIdByClassName(((Type) ldcValue)
						.getInternalName());
			default:
				throw new RuntimeException("Illegal ldc type sort "
						+ type.toString() + " " + type.getSort());
			}
		} else if (ldcValue instanceof Number || ldcValue instanceof Boolean
				|| ldcValue instanceof String || ldcValue instanceof Character) {
			ret = ldcTable.get(ldcValue);
		} else {
			throw new RuntimeException("Unknown ldcValue "
					+ ldcValue.getClass());
		}
		if (ret == null) {
			throw new RuntimeException("Can't find content id for ldc value "
					+ ldcValue + " " + ldcValue.getClass());
		} else {
			return ret;
		}
	}
}

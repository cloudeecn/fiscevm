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
package com.cirnoworks.fisce.vm.default_impl;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;

import com.cirnoworks.fisce.vm.IClassLoader;
import com.cirnoworks.fisce.vm.INativeHandler;
import com.cirnoworks.fisce.vm.IToolkit;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.VMCriticalException;
import com.cirnoworks.fisce.vm.VMException;
import com.cirnoworks.fisce.vm.data.AbstractClass;
import com.cirnoworks.fisce.vm.data.ClassArray;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.ClassField;
import com.cirnoworks.fisce.vm.data.ClassMethod;
import com.cirnoworks.fisce.vm.data.IAttributesHolder;
import com.cirnoworks.fisce.vm.data.IReference;
import com.cirnoworks.fisce.vm.data.attributes.Attribute;
import com.cirnoworks.fisce.vm.data.attributes.AttributeCode;
import com.cirnoworks.fisce.vm.data.attributes.AttributeConstanValue;
import com.cirnoworks.fisce.vm.data.attributes.AttributeExceptions;
import com.cirnoworks.fisce.vm.data.attributes.AttributeInnerClasses;
import com.cirnoworks.fisce.vm.data.attributes.AttributeLineNumberTable;
import com.cirnoworks.fisce.vm.data.attributes.AttributeLocalVariableTable;
import com.cirnoworks.fisce.vm.data.attributes.AttributeSourceFile;
import com.cirnoworks.fisce.vm.data.attributes.AttributeSynthetic;
import com.cirnoworks.fisce.vm.data.attributes.AttributeUnknown;
import com.cirnoworks.fisce.vm.data.attributes.ExceptionHandler;
import com.cirnoworks.fisce.vm.data.attributes.InnerClass;
import com.cirnoworks.fisce.vm.data.attributes.LineNumber;
import com.cirnoworks.fisce.vm.data.attributes.LocalVariable;
import com.cirnoworks.fisce.vm.data.constants.Constant;
import com.cirnoworks.fisce.vm.data.constants.ConstantClass;
import com.cirnoworks.fisce.vm.data.constants.ConstantDouble;
import com.cirnoworks.fisce.vm.data.constants.ConstantFieldRef;
import com.cirnoworks.fisce.vm.data.constants.ConstantFloat;
import com.cirnoworks.fisce.vm.data.constants.ConstantInteger;
import com.cirnoworks.fisce.vm.data.constants.ConstantInterfaceMethodRef;
import com.cirnoworks.fisce.vm.data.constants.ConstantLong;
import com.cirnoworks.fisce.vm.data.constants.ConstantMethodRef;
import com.cirnoworks.fisce.vm.data.constants.ConstantNameTypeInfo;
import com.cirnoworks.fisce.vm.data.constants.ConstantString;
import com.cirnoworks.fisce.vm.data.constants.ConstantUTF8;

/**
 * 
 * @author cloudee
 */
public class DefaultClassLoader implements IClassLoader {
	private int getSizeShiftForArray(String arrayName) {
		char c = arrayName.charAt(1);
		switch (c) {
		case TYPE_BOOLEAN:
		case TYPE_BYTE:
			return 0;
		case TYPE_DOUBLE:
		case TYPE_LONG:
			return 3;
		default:
			return 2;
		}
	}

	private ClassBase loadClassFromStream(ClassBase cb, InputStream is,
			VMContext context) throws IOException, VMException {

		DataInputStream dis = new DataInputStream(is);
		cb.setMagic(dis.readInt());
		if (cb.getMagic() != 0xcafebabe) {
			throw new IOException("bad class id!");
		}
		cb.setMinorVersion(dis.readChar());
		cb.setMajorVersion(dis.readChar());
		int constantPoolSize = dis.readChar();
		cb.createConstantPool(constantPoolSize);
		// context.println("max=" + constantPool.length);
		for (int i = 1; i < constantPoolSize; i++) {
			int tag = dis.readByte();
			switch (tag) {
			case 1: // utf-8;
			{
				ConstantUTF8 c = new ConstantUTF8(context, cb, dis);
				cb.getConstantPool()[i] = c;
				break;
			}
			case 3: {
				ConstantInteger c = new ConstantInteger(context, cb, dis);
				cb.getConstantPool()[i] = c;
				break;
			}
			case 4: {
				ConstantFloat c = new ConstantFloat(context, cb, dis);
				cb.getConstantPool()[i] = c;
				break;
			}
			case 5: {
				ConstantLong c = new ConstantLong(context, cb, dis);
				cb.getConstantPool()[i] = c;
				i++;
				break;
			}
			case 6: {
				ConstantDouble c = new ConstantDouble(context, cb, dis);
				cb.getConstantPool()[i] = c;
				i++;
				break;
			}
			case 7: {
				ConstantClass c = new ConstantClass(context, cb, dis);
				cb.getConstantPool()[i] = c;
				break;
			}
			case 8: {
				ConstantString c = new ConstantString(context, cb, dis);
				cb.getConstantPool()[i] = c;
				break;
			}
			case 9: {
				ConstantFieldRef c = new ConstantFieldRef(context, cb, dis);
				cb.getConstantPool()[i] = c;
				break;
			}
			case 10: {
				ConstantMethodRef c = new ConstantMethodRef(context, cb, dis);
				cb.getConstantPool()[i] = c;
				break;
			}
			case 11: {
				ConstantInterfaceMethodRef c = new ConstantInterfaceMethodRef(
						context, cb, dis);
				cb.getConstantPool()[i] = c;
				break;
			}
			case 12: {
				ConstantNameTypeInfo c = new ConstantNameTypeInfo(context, cb,
						dis);
				cb.getConstantPool()[i] = c;
				break;
			}
			default: {
				throw new IOException("Unknown tag type " + tag);
			}

			}
		}
		cb.setConstantsLoaded(true);

		// Fill in the string fields
		for (int i = 1, max = constantPoolSize; i < max; i++) {
			Constant c = cb.getConstantPool()[i];
			if (c != null) {
				c.fillConstants();
			}
		}

		for (int i = 1, max = constantPoolSize; i < max; i++) {

			Constant c = cb.getConstantPool()[i];
			if (c instanceof IReference) {
				((IReference) c).fillUniqueName();
			}
		}
		cb.setConstantsFilled(true);
		cb.setAccessFlags(dis.readChar());
		cb.setThisClassInfoIndex(dis.readChar());
		cb.setSuperClassInfoIndex(dis.readChar());

		char interfaceCount = dis.readChar();
		cb.createInterfaceInfoIndexs(interfaceCount);
		for (int i = 0; i < interfaceCount; i++) {
			cb.getInterfaceInfoIndexs()[i] = dis.readChar();
		}
		cb.fillConstants();
		cb.setName(cb.getThisClassInfo().getName());

		char fieldsCount = dis.readChar();
		cb.createFields(fieldsCount);
		int fieldPos = 0;
		int staticPos = 0;
		for (int i = 0; i < fieldsCount; i++) {
			ClassField field = new ClassField(context, cb);
			field.setAccessFlags(dis.readChar());
			field.setNameIndex(dis.readChar());
			field.setDescriptorIndex(dis.readChar());
			field.fillStringValues();
			fetchAttributes(field, dis, cb);
			// ConstantNameTypeInfo nameAndType = new ConstantNameTypeInfo(
			// context, cb, field.getName(), field.getDescriptor());
			// ConstantFieldRef ref = new ConstantFieldRef(context, cb, cb.
			// getThisClassInfo(),
			// nameAndType);

			int length = getSizeFromDescriptor(field.getDescriptor());
			field.setLength(length);
			if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
				field.setPosition(fieldPos);
				fieldPos += length;

			} else {
				field.setPosition(staticPos);
				staticPos += length;

			}

			AttributeConstanValue acv = (AttributeConstanValue) Attribute
					.getAttributeByName(field, "ConstantValue");
			if (acv != null) {
				field.setConstantValue(acv.constantValue);
			}
			context.registerField(field);

			// context.putField(ref, field);
			cb.getFields()[i] = field;
		}
		cb.setSizeInHeap(fieldPos);
		cb.setSizeInStatic(staticPos);

		char methodsCount = dis.readChar();
		cb.createMethods(methodsCount);
		for (int i = 0; i < methodsCount; i++) {
			ClassMethod method = new ClassMethod(context, cb);
			method.setAccessFlags(dis.readChar());
			method.setNameIndex(dis.readChar());
			method.setDescriptorIndex(dis.readChar());
			method.fillStringValues();
			// context.println("Fetch attribute for " +
			// method.getDescriptor()
			// + " " + method.getName());
			fetchAttributes(method, dis, cb);
			if ((cb.getAccessFlags() & ClassBase.ACC_INTERFACE) > 0) {
				// Interface Method
				// ConstantNameTypeInfo nameAndType = new ConstantNameTypeInfo(
				// context, cb, method.getName(), method.getDescriptor());
				// ConstantInterfaceMethodRef ref = new
				// ConstantInterfaceMethodRef(
				// context, cb, cb.getThisClassInfo(), nameAndType);
				// context.putInterfaceMethod(ref, method);
				context.registerMethod(method);
			} else {
				// ConstantNameTypeInfo nameAndType = new ConstantNameTypeInfo(
				// context, cb, method.getName(), method.getDescriptor());
				context.registerMethod(method);
				if (method.getName().equals("<clinit>")) {
					cb.setClinit(method);
				}
			}
			/*
			 * //From AttributeCode private char maxStack; private char
			 * maxLocals; private byte[] code; private ExceptionHandler[]
			 * exceptionTable; //From AttributeCode.AttributeLineNumberTabe
			 * private LineNumber[] lineNumberTable; //From
			 * AttributeCode.AttributeLocalVariableTable private LocalVariable[]
			 * localVariableTable; //from AttributeExceptions private
			 * ConstantClass[] exceptions;
			 */
			AttributeCode code = (AttributeCode) Attribute.getAttributeByName(
					method, "Code");
			if (AbstractClass.hasFlag(method.getAccessFlags(),
					AbstractClass.ACC_NATIVE)) {
				INativeHandler inh = context.getNativeHandler(method
						.getUniqueName());
				if (inh == null) {
					throw new VMException("java/lang/UnsatisfiedLinkError",
							method.getUniqueName());
				}
				method.setNativeHandler(inh);
			} else if (code == null) {
				if (((cb.getAccessFlags() & ClassBase.ACC_INTERFACE) == 0)
						&& ((method.getAccessFlags() & ClassBase.ACC_ABSTRACT) == 0)) {
					throw new VMException("java/lang/VirtualMachineError",
							"Not abstract and native method must have code!");
				}
			} else {
				method.setMaxStack(code.maxStack);
				method.setMaxLocals(code.maxLocals);
				method.setCode(code.code);
				method.setExceptionTable(code.exceptionTable);
				AttributeLineNumberTable lnt = (AttributeLineNumberTable) Attribute
						.getAttributeByName(code, "LineNumberTable");
				if (lnt != null) {
					method.setLineNumberTable(lnt.lineNumberTable);
				}
				AttributeLocalVariableTable lvt = (AttributeLocalVariableTable) Attribute
						.getAttributeByName(code, "LocalVariableTable");
				if (lvt != null) {
					method.setLocalVariableTable(lvt.localVariableTable);
				}
			}
			AttributeExceptions ae = (AttributeExceptions) Attribute
					.getAttributeByName(method, "Exceptions");
			if (ae == null) {
				method.setExceptions(new ConstantClass[0]);
			} else {
				method.setExceptions(ae.exceptions);
			}
			cb.getMethods()[i] = method;
		}
		fetchAttributes(cb, dis, cb);

		AttributeInnerClasses ic = (AttributeInnerClasses) Attribute
				.getAttributeByName(cb, "InnerClasses");
		if (ic != null) {
			cb.setInnerClasses(ic.classes);
		}
		AttributeSourceFile sf = (AttributeSourceFile) Attribute
				.getAttributeByName(cb, "SourceFile");
		if (sf != null) {
			cb.setSourceFile(sf.sourceFile);
		}
		return cb;
	}

	// Very dirty, but works....
	private void fetchAttributes(IAttributesHolder owner, DataInputStream dis,
			ClassBase cb) throws IOException {
		char count = dis.readChar();
		owner.createAttributes(count);
		for (int i = 0; i < count; i++) {
			char attributeNameIndex = dis.readChar();
			String attributeName = cb
					.getStringFromUTF8Constant(attributeNameIndex);
			int attributeLength = dis.readInt();
			// context.println("Fetching attribute:" + attributeName);
			Attribute add;
			if (attributeName.equals("ConstantValue")) {

				AttributeConstanValue a = (AttributeConstanValue) (add = new AttributeConstanValue());

				a.constantValueIndex = dis.readChar();
				a.constantValue = cb.getConstantPool()[a.constantValueIndex];
			} else if (attributeName.equals("Code")) {
				AttributeCode a = (AttributeCode) (add = new AttributeCode());
				a.maxStack = dis.readChar();
				a.maxLocals = dis.readChar();
				a.code = new byte[dis.readInt()];
				dis.readFully(a.code);
				a.exceptionTable = new ExceptionHandler[dis.readChar()];
				for (int j = 0, max = a.exceptionTable.length; j < max; j++) {
					ExceptionHandler et = new ExceptionHandler();
					et.startPc = dis.readChar();
					et.endPc = dis.readChar();
					et.handlerPc = dis.readChar();
					et.catchType = dis.readChar();
					// et.catchClassIdx = dis.readChar();
					et.catchClass = et.catchType == 0 ? null
							: (ConstantClass) cb.getConstantPool()[et.catchType];
					a.exceptionTable[j] = et;
				}
				fetchAttributes(a, dis, cb);

			} else if (attributeName.equals("Exceptions")) {
				AttributeExceptions a = (AttributeExceptions) (add = new AttributeExceptions());

				char exceptionsCount = dis.readChar();
				a.exceptionIndexTable = new char[exceptionsCount];
				a.exceptions = new ConstantClass[exceptionsCount];

				for (int j = 0; j < exceptionsCount; j++) {
					char eid = dis.readChar();
					a.exceptionIndexTable[j] = eid;
					a.exceptions[j] = (ConstantClass) cb.getConstantPool()[eid];
				}
			} else if (attributeName.equals("InnerClasses")) {
				AttributeInnerClasses a = (AttributeInnerClasses) (add = new AttributeInnerClasses());

				int classesCount = dis.readChar();
				a.classes = new InnerClass[classesCount];
				for (int j = 0; j < classesCount; j++) {
					InnerClass ic = new InnerClass();
					ic.innerClassInfoIndex = dis.readChar();
					ic.innerClassInfo = ic.innerClassInfoIndex == 0 ? null
							: (ConstantClass) cb.getConstantPool()[ic.innerClassInfoIndex];
					ic.outerClassInfoIndex = dis.readChar();
					ic.outerClassInfo = ic.outerClassInfoIndex == 0 ? null
							: (ConstantClass) cb.getConstantPool()[ic.outerClassInfoIndex];
					ic.innerNameIndex = dis.readChar();
					ic.innerName = ic.innerNameIndex == 0 ? null
							: (ConstantUTF8) cb.getConstantPool()[ic.innerNameIndex];
					ic.innerClassAccessFlage = dis.readChar();
					a.classes[j] = ic;
				}
			} else if (attributeName.equals("Synthetic")) {
				// AttributeSynthetic a = (AttributeSynthetic) (
				add = new AttributeSynthetic();
				// );
			} else if (attributeName.equals("SourceFile")) {
				AttributeSourceFile a = (AttributeSourceFile) (add = new AttributeSourceFile());

				a.sourceFileIndex = dis.readChar();
				a.sourceFile = ((ConstantUTF8) cb.getConstantPool()[a.sourceFileIndex])
						.getString();
			} else if (attributeName.equals("LineNumberTable")) {
				AttributeLineNumberTable a = (AttributeLineNumberTable) (add = new AttributeLineNumberTable());
				char tableLength = dis.readChar();
				a.lineNumberTable = new LineNumber[tableLength];
				for (int j = 0; j < tableLength; j++) {
					LineNumber t = new LineNumber();
					t.startPc = dis.readChar();
					t.lineNumber = dis.readChar();
					a.lineNumberTable[j] = t;
				}

			} else if (attributeName.equals("LocalVariableTable")) {
				AttributeLocalVariableTable a = (AttributeLocalVariableTable) (add = new AttributeLocalVariableTable());

				char length = dis.readChar();
				a.localVariableTable = new LocalVariable[length];
				for (int j = 0; j < length; j++) {
					LocalVariable t = new LocalVariable();
					t.startPc = dis.readChar();
					t.length = dis.readChar();
					t.nameIndex = dis.readChar();
					t.name = ((ConstantUTF8) cb.getConstantPool()[t.nameIndex])
							.getString();
					t.descriptorIndex = dis.readChar();
					t.descriptor = ((ConstantUTF8) cb.getConstantPool()[t.descriptorIndex])
							.getString();
					t.index = dis.readChar();
					a.localVariableTable[j] = t;
				}

			} else if (attributeName.equals("Deprecated")) {
				// AttributeConstanValue a = (AttributeConstanValue) (
				add = new AttributeConstanValue();
				// );

			} else {
				AttributeUnknown a = (AttributeUnknown) (add = new AttributeUnknown());
				a.data = new byte[attributeLength];
				dis.readFully(a.data);
			}
			add.attributeNameIndex = attributeNameIndex;
			add.attributeName = cb
					.getStringFromUTF8Constant(attributeNameIndex);
			add.attributeLength = attributeLength;
			owner.setAttribute(i, add);
		}
	}

	public AbstractClass loadClassByName(String className, VMContext context)
			throws VMException, VMCriticalException {
		assert context.getConsole().debug("Loading class " + className);
		if (className.startsWith("[")) {
			ClassArray ca = new ClassArray(context, this);
			ca.setSuperClass((ClassBase) context.getClass("java/lang/Object"));
			ca.createInterfaces(0);
			ca.setName(className);
			ca.setSizeShift(getSizeShiftForArray(className));
			ca.setConstantsLoaded(true);
			ca.setConstantsFilled(true);
			return ca;
		} else {
			ClassBase cb = new ClassBase(context, this);
			boolean found = false;
			for (IToolkit cp : context.getToolkits()) {
				InputStream is = cp.getResourceByClassName(className);
				if (is != null) {
					try {
						cb = loadClassFromStream(cb, is, context);
						found = true;
						break;
					} catch (IOException e) {
						throw new VMException(
								"java/lang/ClassNotFoundException",
								e.toString());
					} finally {
						if (is != null) {
							try {
								is.close();
							} catch (IOException e) {
							}
						}
					}
				}
			}
			if (!found) {
				throw new VMException("java/lang/ClassNotFoundException",
						"Class " + className + " not found!");
			}
			return cb;
		}
	}

	public void verifyPass2(AbstractClass ac, VMContext context)
			throws VMException, VMCriticalException {
		if (ac instanceof ClassArray) {
			ClassArray clazz = (ClassArray) ac;
			String contentClassName = clazz.getName().substring(1);
			char type = contentClassName.charAt(0);
			clazz.setpType(type);
			assert type == '[' || Character.isUpperCase(type) : clazz.getName();
			if (type == '[') {
				clazz.setContentClass(context.getClass(contentClassName));
			} else if (type == 'L') {
				clazz.setContentClass(context.getClass(contentClassName
						.substring(1, contentClassName.length() - 1)));
			}
		} else if (ac instanceof ClassBase) {
			ClassBase clazz = (ClassBase) ac;
			// Load and verify all super classes:
			assert !clazz.isLoaded();
			String name = clazz.getName();

			ConstantClass supercli = clazz.getSuperClassInfo();
			// int cid =
			context.getClazzId(clazz);
			if (supercli != null) {
				if (name.equals("java/lang/Object")) {
					throw new VMException("java/lang/VirtualMachineError",
							"java.lang.Object cannot have super class!");
				}
				ClassBase supercl = (ClassBase) context.getClass(supercli
						.getName());
				if (!supercl.isLoaded()) {
					throw new VMException("java/lang/VirtualMachineError",
							"Class define loop!");
				}
				int ofs = supercl.getTotalSizeInHeap();
				clazz.setOffsetInHeap(ofs);
				ClassField[] fields = clazz.getFields();

				for (int i = 0, max = fields.length; i < max; i++) {
					ClassField field = fields[i];

					if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) > 0) {
						field.setAbsPos(context.getHeap().getClazzStaticPos(
								clazz)
								+ field.getPosition());
					} else {
						field.setAbsPos(field.getPosition() + ofs);
					}

				}
				clazz.setTotalSizeInHeap(clazz.getSizeInHeap()
						+ supercl.getTotalSizeInHeap());
				clazz.setSuperClass(supercl);
			} else {
				if (!name.equals("java/lang/Object")) {
					throw new VMException("java/lang/VirtualMachineError",
							"Class " + clazz.getName()
									+ " have no super class!");
				}
				clazz.setOffsetInHeap(0);
				ClassField[] fields = clazz.getFields();
				for (int i = 0, max = fields.length; i < max; i++) {
					ClassField field = fields[i];
					field.setAbsPos(field.getPosition());
				}
				clazz.setTotalSizeInHeap(clazz.getSizeInHeap());
			}

			// Load and verify all the interfaces
			clazz.createInterfaces(clazz.getInterfaceInfos().length);
			for (int i = 0, max = clazz.getInterfaceInfos().length; i < max; i++) {
				clazz.getInterfaces()[i] = (ClassBase) clazz
						.getInterfaceInfos()[i].getClazz();
			}
		} else {
			throw new VMException("java/lang/Error", "Unsupported class type");
		}
		ac.setLoaded(true);

	}

	private int getSizeFromDescriptor(String descriptor) {
		switch (descriptor.charAt(0)) {
		case 'J':
		case 'D':
			return 2;
		default:
			return 1;
		}
	}
}

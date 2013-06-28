package com.cirnoworks.fisce.classloader.utils;

import java.util.ArrayList;

public class DescriptorAnalyzer {

	public static final byte TYPE_INT = 'I';
	public static final byte TYPE_WIDE = 'W';
	public static final byte TYPE_HANDLE = 'H';
	public static final byte TYPE_RETURN = 'R';
	public static final byte TYPE_WIDE2 = '_';
	public static final byte TYPE_UNKNOWN = 0;

	public static byte getType2(String type) {
		switch (type.charAt(0)) {
		case 'B':
		case 'C':
		case 'F':
		case 'I':
		case 'S':
		case 'Z':
			return TYPE_INT;
		case 'D':
		case 'J':
			return TYPE_WIDE;
		case '[':
		case 'L':
			return TYPE_HANDLE;
		case 'V':
			return TYPE_UNKNOWN;
		default:
			return TYPE_UNKNOWN;
		}
	}

	private int paramStackUsage = 0;
	private byte[] paramTypes;
	private byte returnType;

	private int paramCount = 0;
	private String[] paramClassNames;
	private String returnClassName;

	public DescriptorAnalyzer(String descriptor) {
		int paramsBegin = descriptor.indexOf('(');
		int paramsEnd = descriptor.indexOf(')');
		if (paramsBegin < 0 || paramsEnd < 0 || paramsBegin >= paramsEnd) {
			throw new IllegalArgumentException("Illegal descriptor: "
					+ descriptor);
		}
		ArrayList<Byte> paramTypesList = new ArrayList<Byte>();
		ArrayList<String> paramClassNamesList = new ArrayList<String>();
		String params = descriptor.substring(paramsBegin + 1, paramsEnd);
		int pos = 0;
		int len = params.length();
		while (pos < len) {
			char c = params.charAt(pos++);
			switch (c) {
			case 'B':
				paramTypesList.add(TYPE_INT);
				paramClassNamesList.add("byte");
				break;
			case 'C':
				paramTypesList.add(TYPE_INT);
				paramClassNamesList.add("char");
				break;
			case 'F':
				paramTypesList.add(TYPE_INT);
				paramClassNamesList.add("float");
				break;
			case 'I':
				paramTypesList.add(TYPE_INT);
				paramClassNamesList.add("int");
				break;
			case 'S':
				paramTypesList.add(TYPE_INT);
				paramClassNamesList.add("short");
				break;
			case 'Z':
				paramTypesList.add(TYPE_INT);
				paramClassNamesList.add("boolean");
				break;

			case 'D':
				paramTypesList.add(TYPE_WIDE);
				paramTypesList.add(TYPE_WIDE2);
				paramClassNamesList.add("double");
				break;
			case 'J':
				paramTypesList.add(TYPE_WIDE);
				paramTypesList.add(TYPE_WIDE2);
				paramClassNamesList.add("long");
				break;

			case '[': {
				paramTypesList.add(TYPE_HANDLE);
				int pos0 = pos - 1;
				while (params.charAt(pos) == '[') {
					pos++;
				}
				if (params.charAt(pos) == 'L') {
					while (params.charAt(pos) != ';') {
						pos++;
					}
				}
				pos++;
				paramClassNamesList.add(params.substring(pos0, pos));
				break;
			}
			case 'L': {
				paramTypesList.add(TYPE_HANDLE);
				int pos0 = pos;
				while (params.charAt(pos) != ';') {
					pos++;
				}
				pos++;
				paramClassNamesList.add(params.substring(pos0, pos - 1));
				break;
			}
			default: {
				throw new IllegalArgumentException("Illegal descriptor: "
						+ descriptor);
			}
			}
		}

		paramStackUsage = paramTypesList.size();
		paramTypes = new byte[paramStackUsage];
		for (int i = 0, max = paramStackUsage; i < max; i++) {
			paramTypes[i] = paramTypesList.get(i);
		}

		paramCount = paramClassNamesList.size();
		paramClassNames = paramClassNamesList.toArray(new String[paramCount]);

		String ret = descriptor.substring(paramsEnd + 1);
		switch (ret.charAt(0)) {
		case 'B':
			returnType = TYPE_INT;
			returnClassName = "byte";
			break;
		case 'C':
			returnType = TYPE_INT;
			returnClassName = "char";
			break;
		case 'F':
			returnType = TYPE_INT;
			returnClassName = "float";
			break;
		case 'I':
			returnType = TYPE_INT;
			returnClassName = "int";
			break;
		case 'S':
			returnType = TYPE_INT;
			returnClassName = "short";
			break;
		case 'Z':
			returnType = TYPE_INT;
			returnClassName = "boolean";
			break;
		case 'D':
			returnType = TYPE_WIDE;
			returnClassName = "double";
			break;
		case 'J':
			returnType = TYPE_WIDE;
			returnClassName = "long";
			break;
		case '[':
			returnType = TYPE_HANDLE;
			returnClassName = ret;
			break;
		case 'L':
			returnType = TYPE_HANDLE;
			returnClassName = ret.substring(1, ret.length() - 1);
			break;
		case 'V':
			returnType = TYPE_UNKNOWN;
			returnClassName = "void";
			break;
		default: {
			throw new IllegalArgumentException("Illegal descriptor: "
					+ descriptor);
		}
		}
	}

	public int getParamStackUsage() {
		return paramStackUsage;
	}

	public byte[] getParamTypes() {
		return paramTypes;
	}

	public byte getReturnType() {
		return returnType;
	}

	public int getParamCount() {
		return paramCount;
	}

	public String[] getParamClassNames() {
		return paramClassNames;
	}

	public String getReturnClassName() {
		return returnClassName;
	}

}

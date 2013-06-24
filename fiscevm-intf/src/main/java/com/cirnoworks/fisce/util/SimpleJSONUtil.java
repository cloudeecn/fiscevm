package com.cirnoworks.fisce.util;

public final class SimpleJSONUtil {

	private final static String[] translateTable = new String[128];
	static {
		translateTable['\b'] = "\\b";
		translateTable['\n'] = "\\n";
		translateTable['\t'] = "\\t";
		translateTable['\f'] = "\\f";
		translateTable['\r'] = "\\r";

		translateTable['\''] = "\\'";
		translateTable['\"'] = "\\\"";
		translateTable['\\'] = "\\\\";
		translateTable['/'] = "\\/";
	}

	public static String escapeString(String origString) {
		return escapeString(origString, true);
	}

	public static String escapeString(String origString, boolean addQuote) {
		StringBuilder sb = new StringBuilder(origString.length() * 2
				+ (addQuote ? 2 : 0));
		if (addQuote) {
			sb.append("\"");
		}
		for (int i = 0, max = origString.length(); i < max; i++) {
			char c = origString.charAt(i);
			if (c < 32 || c > 127) {
				// unicode escape
				sb.append("\\u");
				if (c <= 0xff) {
					sb.append("000");
				} else if (c <= 0xffff) {
					sb.append("00");
				} else if (c <= 0xffffff) {
					sb.append("0");
				}
				sb.append(Integer.toString(c, 16));
			} else {
				String translated = translateTable[c];
				if (translated == null) {
					sb.append(c);
				} else {
					sb.append(translated);
				}
			}
		}
		if (addQuote) {
			sb.append("\"");
		}
		return sb.toString();
	}

	public static void addIndent(StringBuilder sb, int indent) {
		for (int i = 0; i < indent; i++) {
			sb.append('\t');
		}
	}

	public static void add(StringBuilder sb, int indent, String str,
			boolean addComma) {
		addIndent(sb, indent);
		sb.append(str);
		if (addComma) {
			sb.append(",\n");
		} else {
			sb.append('\n');
		}
	}

	public static void add(StringBuilder sb, int indent, String str) {
		add(sb, indent, str, true);
	}

	public static void add(StringBuilder sb, int indent, String key,
			Object value, boolean addComma) {
		addIndent(sb, indent);
		sb.append(key);
		sb.append(" : ");
		sb.append(value);
		if (addComma) {
			sb.append(",\n");
		} else {
			sb.append('\n');
		}
	}

	public static void add(StringBuilder sb, int indent, String key,
			Object value) {
		add(sb, indent, key, value, true);
	}
}

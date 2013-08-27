package com.cirnoworks.fisce.classloader.utils;

public final class SimpleJSONUtil {

	private final static String[] translateTable = new String[128];
	static {
		translateTable['\b'] = "\\b";
		translateTable['\n'] = "\\n";
		translateTable['\t'] = "\\t";
		translateTable['\f'] = "\\f";
		translateTable['\r'] = "\\r";

		// translateTable['\''] = "\\'";
		// translateTable['\"'] = "\\\"";
		translateTable['\\'] = "\\\\";
		translateTable['/'] = "\\/";
	}

	public static String escapeString(String origString) {
		return escapeString(origString, '"', true);
	}

	public static String escapeString(String origString, boolean addQuote) {
		return escapeString(origString, '"', addQuote);
	}

	public static String escapeString(String origString, char quote,
			boolean addQuote) {
		StringBuilder sb = new StringBuilder(origString.length() * 2
				+ (addQuote ? 2 : 0));
		if (addQuote) {
			sb.append(quote);
		}
		for (int i = 0, max = origString.length(); i < max; i++) {
			char c = origString.charAt(i);
			String translated = c < 128 ? translateTable[c] : null;
			if (translated != null) {
				sb.append(translated);
			} else if (c < 32 || c > 127) {
				// unicode escape
				sb.append("\\u");
				if (c <= 0xf) {
					sb.append("000");
				} else if (c <= 0xff) {
					sb.append("00");
				} else if (c <= 0xfff) {
					sb.append("0");
				}
				sb.append(Integer.toString(c, 16));
			} else {
				if (c == quote) {
					sb.append('\\');
					sb.append(c);
				} else {
					sb.append(c);
				}
			}
		}
		if (addQuote) {
			sb.append(quote);
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

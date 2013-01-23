/* 
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package java.text;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MessageFormat {

	private final static Pattern replacePattern = Pattern
			.compile("\\{(.*?)\\}");

	/**
	 * Formats the supplied objects using the specified message format pattern.
	 * 
	 * @param template
	 *            the pattern to use for formatting.
	 * @param objects
	 *            the array of objects to format.
	 * @return the formatted result.
	 * @throws IllegalArgumentException
	 *             if the pattern cannot be parsed.
	 */
	public static String format(String template, Object... objects) {
		if (objects != null) {
			for (int i = 0; i < objects.length; i++) {
				if (objects[i] == null) {
					objects[i] = "null";
				}
			}
		}
		StringBuilder builder = new StringBuilder();
		Matcher matcher = replacePattern.matcher(template);
		int next = 0;
		while (matcher.find()) {
			if (matcher.start() > next) {
				builder.append(template.substring(next, matcher.start()));
				String group = matcher.group(1);
				String mapped = null;
				try {
					int iGroup = Integer.parseInt(group);
					if (iGroup >= 0 && iGroup < objects.length) {
						mapped = objects[iGroup].toString();
					}
				} catch (NumberFormatException e) {

				}
				if (mapped != null) {
					builder.append(mapped);
					next = matcher.end();
				} else {
					next = matcher.start();
				}
			}
		}
		if (next < template.length() - 1) {
			builder.append(template.substring(next));
		}
		return builder.toString();
	}

}

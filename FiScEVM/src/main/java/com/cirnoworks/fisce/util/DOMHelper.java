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
package com.cirnoworks.fisce.util;

import java.util.ArrayList;
import java.util.List;

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public final class DOMHelper {
	private DOMHelper() {
		// prevent instance.
	}

	public static List<Element> elements(Element e, String tag) {
		List<Element> ret = new ArrayList<Element>();
		NodeList children = e.getChildNodes();
		for (int i = 0, max = children.getLength(); i < max; i++) {
			Node n = children.item(i);
			if (n instanceof Element) {
				Element element = (Element) n;
				if (tag.equals(element.getTagName())) {
					ret.add(element);
				}
			}
		}
		return ret;
	}
}

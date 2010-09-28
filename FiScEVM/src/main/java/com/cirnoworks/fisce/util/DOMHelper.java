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

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

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Text;

public final class DOMHelper {
	private DOMHelper() {
		// prevent instance.
	}

	public static List<Element> elements(Node e, String tag) {
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

	public static void setTextContent(Element element, String content) {
		Document document = element.getOwnerDocument();
		element.appendChild(document.createTextNode(content));
	}

	public static String getTextContent(Element element) {
		Document document = element.getOwnerDocument();
		NodeList children = element.getChildNodes();
		for (int i = 0, max = children.getLength(); i < max; i++) {
			Node node = children.item(i);
			if (node instanceof Text) {
				Text text = (Text) node;
				return text.getData();
			}
		}
		return "";
	}

	public static String getStringFromNode(Node root, int level) {

		StringBuilder result = new StringBuilder();

		if (root.getNodeType() == 3) {
			result.append(root.getNodeValue().replaceAll("&", "&amp;")
					.replaceAll("<", "&lt;").replaceAll(">", "&gt;"));
		} else {
			if (root.getNodeType() != 9) {
				StringBuffer attrs = new StringBuffer();
				NamedNodeMap attributes = root.getAttributes();
				for (int k = 0; k < attributes.getLength(); ++k) {
					Node attribute = attributes.item(k);
					attrs.append(" ").append(attribute.getNodeName())
							.append("=\"").append(attribute.getNodeValue())
							.append("\" ");
				}
				result.append("\n");
				for (int i = 0; i < level; i++) {
					result.append("\t");
				}
				result.append("<").append(root.getNodeName()).append(" ")
						.append(attrs).append(">");
			} else {
				result.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
			}

			NodeList nodes = root.getChildNodes();
			boolean hasText = false;
			for (int i = 0, j = nodes.getLength(); i < j; i++) {
				Node node = nodes.item(i);
				if (node.getNodeType() == 3) {
					hasText = true;
				}
				result.append(getStringFromNode(node,
						node.getNodeType() == 9 ? level : (level + 1)));
			}

			if (root.getNodeType() != 9) {
				if (!hasText) {
					result.append("\n");
					for (int i = 0; i < level; i++) {
						result.append("\t");
					}
				}
				result.append("</").append(root.getNodeName()).append(">");
			}
		}
		return result.toString();
	}
}

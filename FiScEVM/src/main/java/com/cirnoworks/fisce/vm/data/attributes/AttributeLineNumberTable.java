/**
 *  Copyright 2010 Yuxuan Huang
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the Lesser GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.cirnoworks.fisce.jvm13.data.attributes;

import java.io.PrintWriter;
import java.io.StringWriter;

/**
 *
 * @author cloudee
 */
public class AttributeLineNumberTable extends Attribute {

    public LineNumber[] lineNumberTable;

    public String toString() {
        StringWriter sw = new StringWriter();
        PrintWriter out = new PrintWriter(sw);

        out.println("$$$$$AttributeLineNumberTable");
        for (LineNumber i : lineNumberTable) {
            out.println(i.toString());
        }
        out.println(
                "$$$$$/AttributeLineNumberTable");

        return sw.toString();
    }

	@Override
	public void fillConstants() {
	}
}

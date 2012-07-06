/* Licensed to the Apache Software Foundation (ASF) under one or more
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
package EXCLUDE.fisce.test;

/**
 * @author Cloudee
 *
 */
public class AutoBoxingTest extends TestService {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			Boolean z = true;
			Byte b = 120;
			Short s0 = 115;
			Short s1 = 2000;
			Character c0 = 112;
			Character c1 = 1500;
			Integer i0 = -5;
			Integer i1 = -3000;
			Long l0 = -100l;
			Long l1 = -1321341235231541325l;
			Float f = 3.555f;
			Double d = 5.666;
			if (z != z.booleanValue()) {
				fail("");
			}
			if (b != b.byteValue()) {
				fail("");
			}
			if (s0 != s0.intValue()) {
				fail("");
			}
			if (s1 != s1.intValue()) {
				fail("");
			}
			if (c0 != c0.charValue()) {
				fail("");
			}
			if (c1 != c1.charValue()) {
				fail("");
			}
			if (i0 != i0.intValue()) {
				fail("");
			}
			if (i1 != i1.intValue()) {
				fail("");
			}
			if (l0 != l0.longValue()) {
				fail("");
			}
			if (l1 != l1.longValue()) {
				fail("");
			}
			if (f != f.floatValue()) {
				fail("");
			}
			if (d != d.doubleValue()) {
				fail("");
			}
		} catch (Throwable e) {
			e.printStackTrace();
			fail(e.toString());
		}
	}

}

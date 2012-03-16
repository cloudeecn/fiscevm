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

import fisce.util.SimpleList;

/**
 * @author Cloudee
 *
 */
public class ForEachTest extends TestService {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		SimpleList<Integer> sl = new SimpleList<Integer>();
		int[] input = { 1, 2, 5, 6, 0x7f7f7f7f, 5 };
		for (int in : input) {
			sl.add(in);
		}
		int i = 0;
		for (int in : sl) {
			if (input[i] != in || input[i] != sl.get(i)) {
				RuntimeException e = new RuntimeException(i + " " + in + " "
						+ input[i] + " " + sl.get(i));
				e.printStackTrace();
				fail("for each");
			}
			i++;
		}
	}
}

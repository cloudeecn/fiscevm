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
 * @author cloudee
 * 
 */
public class ArrayTest extends TestService {

	public static void main(String[] args) {
		try {
			int[][][] array = new int[5][3][4];
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 3; j++) {
					for (int k = 0; k < 4; k++) {
						array[i][j][k] = i * 100 + j * 10 + k;
					}
				}
			}
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 3; j++) {
					for (int k = 0; k < 4; k++) {
						if (array[i][j][k] != i * 100 + j * 10 + k) {
							fail("Integer array error! expected="
									+ (i * 100 + j * 10 + k) + " real="
									+ array[i][j][k]);
						}
					}
				}
			}
			String[][][] array1 = new String[5][3][4];
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 3; j++) {
					for (int k = 0; k < 4; k++) {
						array1[i][j][k] = i * 100 + j * 10 + k + "";
					}
				}
			}
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 3; j++) {
					for (int k = 0; k < 4; k++) {
						if (!array1[i][j][k]
								.equals("" + (i * 100 + j * 10 + k))) {
							fail("String array error! expected="
									+ (i * 100 + j * 10 + k) + " real="
									+ array[i][j][k]);
						}
					}
				}
			}

			int[] ia = { 1, 2, 3, 4 };
			for (int i : ia) {
				System.out.println(i);
			}

			Object[] oa = { new Object(), new Object() };
			for (Object o : oa) {
				System.out.println(o);
			}
			System.out.println("Array test ok!");
		} catch (Exception e) {
			e.printStackTrace();
			fail("Exception " + e.toString() + " occored!");
		}
	}
}

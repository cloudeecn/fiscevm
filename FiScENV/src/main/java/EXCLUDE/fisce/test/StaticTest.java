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

import com.cirnoworks.fisce.privat.FiScEVM;

/**
 * @author Cloudee
 *
 */
public class StaticTest {

	public static void main(String[] args) {
		Thread t1 = new Thread() {
			@Override
			public void run() {
				if (Layer5.z != 2) {
					Exception e = new Exception("static error!");
					e.printStackTrace();
					FiScEVM.throwOut(e);
				}

				if (Layer5.a != 3) {
					Exception e = new Exception("static error!");
					e.printStackTrace();
					FiScEVM.throwOut(e);
				}

				if (Layer5.b != 4) {
					Exception e = new Exception("static error!");
					e.printStackTrace();
					FiScEVM.throwOut(e);
				}

				if (Layera.c != 5) {
					Exception e = new Exception("static error!");
					e.printStackTrace();
					FiScEVM.throwOut(e);
				}
			}
		};

		Thread t2 = new Thread() {
			@Override
			public void run() {

				if (Layer5.b != 4) {
					Exception e = new Exception("static error!");
					e.printStackTrace();
					FiScEVM.throwOut(e);
				}

				if (Layera.c != 5) {
					Exception e = new Exception("static error!");
					e.printStackTrace();
					FiScEVM.throwOut(e);
				}

				if (Layer5.z != 2) {
					Exception e = new Exception("static error!");
					e.printStackTrace();
					FiScEVM.throwOut(e);
				}

				if (Layer5.a != 3) {
					Exception e = new Exception("static error!");
					e.printStackTrace();
					FiScEVM.throwOut(e);
				}

			}
		};

		Thread t3 = new Thread() {
			@Override
			public void run() {
				if (Layer5.b != 4) {
					Exception e = new Exception("static error!");
					e.printStackTrace();
					FiScEVM.throwOut(e);
				}
				if (Layer5.z != 2) {
					Exception e = new Exception("static error!");
					e.printStackTrace();
					FiScEVM.throwOut(e);
				}
				if (Layera.c != 5) {
					Exception e = new Exception("static error!");
					e.printStackTrace();
					FiScEVM.throwOut(e);
				}
				if (Layer5.a != 3) {
					Exception e = new Exception("static error!");
					e.printStackTrace();
					FiScEVM.throwOut(e);
				}
			}
		};

		t2.start();
		t3.start();
		t1.start();
	}
}

class Layer1 {

	static int z;
	static {
		z = 0;
		try {
			Thread.sleep(200);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		z = 2;
	}
}

class Layer2 extends Layer1 {
	static int a;
	static {
		a = z + 1;
	}

}

class Layer3 extends Layer2 {
	static int b;
	static {
		b = 0;
		try {
			Thread.sleep(500);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		b = a + 1;
	}
}

class Layer4 extends Layer3 {

}

class Layer5 extends Layer4 {

}

class Layera extends Layer2 {
	static int c;
	static {
		c = a + 2;
	}
}
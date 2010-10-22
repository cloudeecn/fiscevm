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
package EXCLUDE.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;

/**
 * @author Cloudee
 * 
 */
public class StaticTest {

	public static void main(String[] args) {
		Thread t1 = new Thread() {
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
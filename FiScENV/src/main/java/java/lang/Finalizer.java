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
package java.lang;

import com.cirnoworks.fisce.privat.FiScEVM;

/**
 * @author Cloudee
 * 
 */
public final class Finalizer extends Thread {

	Finalizer() {
		this.setDaemon(true);
	}

	public void run() {
		System.out.println("aaa");
		try {
			while (true) {
				Object[] finalizee = getFinalizee();
				for (Object o : finalizee) {
					try {
						o.finalize();
						// markFinalized(o);
					} catch (java.lang.Throwable e) {
						e.printStackTrace();
					}
				}
				try {
					Thread.sleep(100);
				} catch (java.lang.InterruptedException e) {
					e.printStackTrace();
				}
			}
		} catch (Throwable t) {
			FiScEVM.throwOut(t);
		}
	}

	private static native Object[] getFinalizee();
}
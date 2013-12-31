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
package java.lang;

import java.lang.ref.Reference;

/**
 * @author Cloudee
 * 
 */
public final class Finalizer extends Thread {

	Finalizer() {
		this.setDaemon(true);
		this.setName("FINALIZER");
	}

	@Override
	public void run() {
		try {
			while (true) {
				Reference<?>[] refs = getReferencesToEnqueue();
				if (refs != null) {
					for (int i = 0, max = refs.length; i < max; i++) {
						Reference<?> ref = refs[i];
						ref.enqueue();
						ref = null;
						refs[i] = null;
					}
				}
				Object[] finalizee = getFinalizee();
				if (finalizee != null) {
					// FiScEVM.infoOut("#FINALIZER "
					// + System.identityHashCode(Thread.currentThread())
					// + ":" + System.identityHashCode(finalizee) + ": "
					// + finalizee.length + " objects to finalize...");
					for (int i = 0, max = finalizee.length; i < max; i++) {
						Object o = finalizee[i];
						try {

							// FiScEVM.infoOut("FINALIZE "
							// + System.identityHashCode(o));

							o.finalize();
							// markFinalized(o);
						} catch (java.lang.Throwable e) {
							e.printStackTrace();
						}
						o = null;
						finalizee[i] = null;
					}
					// FiScEVM.infoOut("#FINALIZER:" + finalizee.length
					// + " objects finalized.");
					finalizee = null;
				}
				try {
					Thread.sleep(500);
				} catch (java.lang.InterruptedException e) {
					e.printStackTrace();
				}
			}
		} catch (Throwable t) {
			t.printStackTrace();
			throw new RuntimeException(t);
		}
	}

	private static native Object[] getFinalizee();

	private static native Reference<?>[] getReferencesToEnqueue();
}

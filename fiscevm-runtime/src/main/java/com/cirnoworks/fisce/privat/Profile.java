/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
package com.cirnoworks.fisce.privat;

/**
 * 
 * @author yuxuanhuang
 */
class PSuper {
	protected void prot() {

	}

	protected final void pf() {

	}
}

public class Profile extends PSuper implements Runnable {

	public static void main(String[] args) {
		new Thread(new Profile()).start();
	}

	int j;
	static int s;
	String name;

	public Profile(String name) {
		this.name = name;
	}

	public Profile() {
		this("profiler");
	}

	public final void pub() {

	}

	public final void fnl() {

	}

	private void prv() {

	}

	public void run() {
		int k = 0;
		int i;

		long ttStack = 0;
		long ttField = 0;
		long ttStatic = 0;
		long ttIRootI = 0;
		long ttIFooI = 0;
		long ttRootS = 0;
		long ttRootV = 0;
		long ttRootF = 0;
		long ttRootI = 0;
		long ttFooS = 0;
		long ttFooF = 0;
		long ttFooI = 0;
		long ttInh = 0;
		long ttPub = 0;
		long ttFnl = 0;
		long ttPrv = 0;
		long ttProt = 0;
		long ttPf = 0;

		final int max = 14;
		final int min = 4;

		for (int u = 0; u < max; u++) {
			long t = System.nanoTime();
			for (i = 0; i < 50000; i++) {
				k += i;
			}
			long t2 = System.nanoTime();
			if (u >= min) {
				ttStack += t2 - t;
			}
			t = System.nanoTime();
			for (j = 0; j < 50000; j++) {
				k += j;
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttField += t2 - t;
			}
			t = System.nanoTime();
			for (s = 0; s < 50000; s++) {
				k += s;
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttStatic += t2 - t;
			}
			Intf in = new Impl();
			Impl im = (Impl) in;

			t = System.nanoTime();
			for (i = 0; i < 50000; i++) {
				k += i;
				in.rootI();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttIRootI += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				in.fooI();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttIFooI += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				im.rootS();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttRootS += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				im.rootV();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttRootV += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				im.rootF();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttRootF += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				im.rootI();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttRootI += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				Impl.fooS();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttFooS += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				im.fooF();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttFooF += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				im.fooI();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttFooI += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				im.inh();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttInh += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				pub();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttPub += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				fnl();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttFnl += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				prv();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttPrv += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				prot();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttProt += t2 - t;
			}
			t = System.nanoTime();

			for (i = 0; i < 50000; i++) {
				k += i;
				pf();
			}
			t2 = System.nanoTime();
			if (u >= min) {
				ttPf += t2 - t;
			}

			String result = "" + u;
			debugOut(result);
			try {
				Thread.sleep(1);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		ttStack /= max - min;
		ttField /= max - min;
		ttStatic /= max - min;
		ttIRootI /= max - min;
		ttIFooI /= max - min;
		ttRootS /= max - min;
		ttRootV /= max - min;
		ttRootF /= max - min;
		ttRootI /= max - min;
		ttFooS /= max - min;
		ttFooF /= max - min;
		ttFooI /= max - min;
		ttInh /= max - min;
		ttPub /= max - min;
		ttFnl /= max - min;
		ttPrv /= max - min;
		ttProt /= max - min;
		ttPf /= max - min;
		debugOut(" ttStack=" + ttStack / 500000f);
		debugOut(" ttField=" + ttField / 500000f);
		debugOut("ttStatic=" + ttStatic / 500000f);

		debugOut("   ttPub=" + ttPub / 500000f);
		debugOut("   ttFnl=" + ttFnl / 500000f);
		debugOut("   ttPrv=" + ttPrv / 500000f);
		debugOut("  ttProt=" + ttProt / 500000f);
		debugOut("   ttPFn=" + ttPf / 500000f);

		debugOut("ttIRootI=" + ttIRootI / 500000f);
		debugOut(" ttIFooI=" + ttIFooI / 500000f);
		debugOut(" ttRootS=" + ttRootS / 500000f);
		debugOut(" ttRootV=" + ttRootV / 500000f);
		debugOut(" ttRootF=" + ttRootF / 500000f);
		debugOut(" ttRootI=" + ttRootI / 500000f);
		debugOut("  ttFooS=" + ttFooS / 500000f);
		debugOut("  ttFooF=" + ttFooF / 500000f);
		debugOut("  ttFooI=" + ttFooI / 500000f);
		debugOut("   ttInh=" + ttInh / 500000f);

	}

	private void debugOut(String v) {
		FiScEVM.infoOut(v);
		// System.out.println(v);
	}
}

interface Intf {
	void fooI();

	void rootI();
}

class Supr {
	static void rootS() {

	}

	public void rootV() {

	}

	public void inh() {

	}

	public final void rootF() {

	}

	public final void rootI() {

	}
}

class Impl extends Supr implements Intf {

	static void fooS() {

	}

	final void fooF() {

	}

	public void fooI() {

	}

	@Override
	public void inh() {

	}
}

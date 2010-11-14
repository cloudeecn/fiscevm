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
package com.cirnoworks.fisce.env.minimal;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;

import org.w3c.dom.Element;

import com.cirnoworks.fisce.env.minimal.nh.ClassForName;
import com.cirnoworks.fisce.env.minimal.nh.ClassGetComponentType;
import com.cirnoworks.fisce.env.minimal.nh.ClassGetInterfaces;
import com.cirnoworks.fisce.env.minimal.nh.ClassGetName;
import com.cirnoworks.fisce.env.minimal.nh.ClassGetSuperClass;
import com.cirnoworks.fisce.env.minimal.nh.ClassInvokeMethod;
import com.cirnoworks.fisce.env.minimal.nh.ClassIsArray;
import com.cirnoworks.fisce.env.minimal.nh.ClassIsAssignableFrom;
import com.cirnoworks.fisce.env.minimal.nh.ClassIsInstance;
import com.cirnoworks.fisce.env.minimal.nh.ClassIsInterface;
import com.cirnoworks.fisce.env.minimal.nh.ClassIsPrimitive;
import com.cirnoworks.fisce.env.minimal.nh.ClassNewInstance0;
import com.cirnoworks.fisce.env.minimal.nh.ClassNewInstance0I;
import com.cirnoworks.fisce.env.minimal.nh.DoubleDoubleToLongBits;
import com.cirnoworks.fisce.env.minimal.nh.DoubleDoubleToRawLongBits;
import com.cirnoworks.fisce.env.minimal.nh.DoubleLongBitsToDouble;
import com.cirnoworks.fisce.env.minimal.nh.FiScEVMDebugOut;
import com.cirnoworks.fisce.env.minimal.nh.FiScEVMDecode;
import com.cirnoworks.fisce.env.minimal.nh.FiScEVMDoubleToString;
import com.cirnoworks.fisce.env.minimal.nh.FiScEVMEncode;
import com.cirnoworks.fisce.env.minimal.nh.FiScEVMExit;
import com.cirnoworks.fisce.env.minimal.nh.FiScEVMFloatToString;
import com.cirnoworks.fisce.env.minimal.nh.FiScEVMGetDoubleRaw;
import com.cirnoworks.fisce.env.minimal.nh.FiScEVMGetFloatRaw;
import com.cirnoworks.fisce.env.minimal.nh.FiScEVMStringToDouble;
import com.cirnoworks.fisce.env.minimal.nh.FiScEVMStringToFloat;
import com.cirnoworks.fisce.env.minimal.nh.FiScEVMThrowOut;
import com.cirnoworks.fisce.env.minimal.nh.FinalizerGetFinalizee;
import com.cirnoworks.fisce.env.minimal.nh.FloatFloatToIntBits;
import com.cirnoworks.fisce.env.minimal.nh.FloatFloatToRawIntBits;
import com.cirnoworks.fisce.env.minimal.nh.FloatIntBitsToFloat;
import com.cirnoworks.fisce.env.minimal.nh.MathAcos;
import com.cirnoworks.fisce.env.minimal.nh.MathAsin;
import com.cirnoworks.fisce.env.minimal.nh.MathAtan;
import com.cirnoworks.fisce.env.minimal.nh.MathAtan2;
import com.cirnoworks.fisce.env.minimal.nh.MathCbrt;
import com.cirnoworks.fisce.env.minimal.nh.MathCeil;
import com.cirnoworks.fisce.env.minimal.nh.MathCos;
import com.cirnoworks.fisce.env.minimal.nh.MathCosh;
import com.cirnoworks.fisce.env.minimal.nh.MathExp;
import com.cirnoworks.fisce.env.minimal.nh.MathExpm1;
import com.cirnoworks.fisce.env.minimal.nh.MathFloor;
import com.cirnoworks.fisce.env.minimal.nh.MathHypot;
import com.cirnoworks.fisce.env.minimal.nh.MathIEEEremainder;
import com.cirnoworks.fisce.env.minimal.nh.MathLog;
import com.cirnoworks.fisce.env.minimal.nh.MathLog10;
import com.cirnoworks.fisce.env.minimal.nh.MathLog1p;
import com.cirnoworks.fisce.env.minimal.nh.MathNextAfterD;
import com.cirnoworks.fisce.env.minimal.nh.MathNextAfterF;
import com.cirnoworks.fisce.env.minimal.nh.MathPow;
import com.cirnoworks.fisce.env.minimal.nh.MathRint;
import com.cirnoworks.fisce.env.minimal.nh.MathSignumD;
import com.cirnoworks.fisce.env.minimal.nh.MathSignumF;
import com.cirnoworks.fisce.env.minimal.nh.MathSin;
import com.cirnoworks.fisce.env.minimal.nh.MathSinh;
import com.cirnoworks.fisce.env.minimal.nh.MathSqrt;
import com.cirnoworks.fisce.env.minimal.nh.MathTan;
import com.cirnoworks.fisce.env.minimal.nh.MathTanh;
import com.cirnoworks.fisce.env.minimal.nh.ObjectClone;
import com.cirnoworks.fisce.env.minimal.nh.ObjectGetClass;
import com.cirnoworks.fisce.env.minimal.nh.ObjectNotify;
import com.cirnoworks.fisce.env.minimal.nh.ObjectNotifyAll;
import com.cirnoworks.fisce.env.minimal.nh.ObjectWait;
import com.cirnoworks.fisce.env.minimal.nh.ResourceInputStreamClose0;
import com.cirnoworks.fisce.env.minimal.nh.ResourceInputStreamRead0a;
import com.cirnoworks.fisce.env.minimal.nh.ResourceInputStreamRead0b;
import com.cirnoworks.fisce.env.minimal.nh.StringIntern;
import com.cirnoworks.fisce.env.minimal.nh.SystemArrayCopy;
import com.cirnoworks.fisce.env.minimal.nh.SystemCurrentTimeMillis;
import com.cirnoworks.fisce.env.minimal.nh.SystemExit;
import com.cirnoworks.fisce.env.minimal.nh.SystemGc;
import com.cirnoworks.fisce.env.minimal.nh.SystemGetProperty;
import com.cirnoworks.fisce.env.minimal.nh.SystemIdentityHashCode;
import com.cirnoworks.fisce.env.minimal.nh.SystemInputStreamRead0;
import com.cirnoworks.fisce.env.minimal.nh.SystemNanoTime;
import com.cirnoworks.fisce.env.minimal.nh.SystemOutStreamWrite0;
import com.cirnoworks.fisce.env.minimal.nh.SystemSetErr0;
import com.cirnoworks.fisce.env.minimal.nh.SystemSetIn0;
import com.cirnoworks.fisce.env.minimal.nh.SystemSetOut0;
import com.cirnoworks.fisce.env.minimal.nh.SystemSetProperty;
import com.cirnoworks.fisce.env.minimal.nh.ThreadCurrentThread;
import com.cirnoworks.fisce.env.minimal.nh.ThreadInterrupt0;
import com.cirnoworks.fisce.env.minimal.nh.ThreadIsAlive;
import com.cirnoworks.fisce.env.minimal.nh.ThreadIsInterrupted;
import com.cirnoworks.fisce.env.minimal.nh.ThreadSetPriority0;
import com.cirnoworks.fisce.env.minimal.nh.ThreadSleep;
import com.cirnoworks.fisce.env.minimal.nh.ThreadStart0;
import com.cirnoworks.fisce.env.minimal.nh.ThreadYield;
import com.cirnoworks.fisce.env.minimal.nh.ThrowableFillInStackTrace0;
import com.cirnoworks.fisce.vm.IToolkit;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.VMCriticalException;

public class BaseToolkit implements IToolkit {

	private VMContext context;

	// persist
	private HashMap<String, String> properties = new HashMap<String, String>();

	// no persist
	// key=name+"*"+handle
	private HashMap<String, InputStream> resources = new HashMap<String, InputStream>();

	public InputStream getResourceByClassName(String arg0) {
		String path = "/fisce_scripts/" + arg0 + ".class";
		return getClass().getResourceAsStream(path);
	}

	public void loadData(Element arg0) throws VMCriticalException {

	}

	public void saveData(Element arg0) throws VMCriticalException {

	}

	public void setupContext() {
		context.setThrower(new BaseThrower(context, this));

		context.registerNativeHandler(new FinalizerGetFinalizee());

		context.registerNativeHandler(new SystemCurrentTimeMillis());
		context.registerNativeHandler(new SystemArrayCopy());
		context.registerNativeHandler(new SystemGetProperty(this));
		context.registerNativeHandler(new SystemSetProperty(this));
		context.registerNativeHandler(new SystemSetErr0());
		context.registerNativeHandler(new SystemSetIn0());
		context.registerNativeHandler(new SystemSetOut0());
		context.registerNativeHandler(new SystemIdentityHashCode());
		context.registerNativeHandler(new SystemExit());
		context.registerNativeHandler(new SystemGc());
		context.registerNativeHandler(new SystemNanoTime());

		context.registerNativeHandler(new ClassForName());
		context.registerNativeHandler(new ClassNewInstance0());
		context.registerNativeHandler(new ClassNewInstance0I());
		context.registerNativeHandler(new ClassGetName());
		context.registerNativeHandler(new ClassIsArray());
		context.registerNativeHandler(new ClassIsAssignableFrom());
		context.registerNativeHandler(new ClassIsInstance());
		context.registerNativeHandler(new ClassIsInterface());
		context.registerNativeHandler(new ClassIsPrimitive());
		context.registerNativeHandler(new ClassGetSuperClass());
		context.registerNativeHandler(new ClassGetInterfaces());
		context.registerNativeHandler(new ClassGetComponentType());
		context.registerNativeHandler(new ClassInvokeMethod());

		context.registerNativeHandler(new DoubleDoubleToLongBits());
		context.registerNativeHandler(new DoubleDoubleToRawLongBits());
		context.registerNativeHandler(new DoubleLongBitsToDouble());

		context.registerNativeHandler(new FloatFloatToIntBits());
		context.registerNativeHandler(new FloatFloatToRawIntBits());
		context.registerNativeHandler(new FloatIntBitsToFloat());

		context.registerNativeHandler(new MathAcos());
		context.registerNativeHandler(new MathAsin());
		context.registerNativeHandler(new MathAtan());
		context.registerNativeHandler(new MathAtan2());
		context.registerNativeHandler(new MathCbrt());
		context.registerNativeHandler(new MathCeil());
		context.registerNativeHandler(new MathCos());
		context.registerNativeHandler(new MathCosh());
		context.registerNativeHandler(new MathExp());
		context.registerNativeHandler(new MathExpm1());
		context.registerNativeHandler(new MathFloor());
		context.registerNativeHandler(new MathHypot());
		context.registerNativeHandler(new MathIEEEremainder());
		context.registerNativeHandler(new MathLog());
		context.registerNativeHandler(new MathLog10());
		context.registerNativeHandler(new MathLog1p());
		context.registerNativeHandler(new MathNextAfterF());
		context.registerNativeHandler(new MathNextAfterD());
		context.registerNativeHandler(new MathPow());
		context.registerNativeHandler(new MathRint());
		context.registerNativeHandler(new MathSignumD());
		context.registerNativeHandler(new MathSignumF());
		context.registerNativeHandler(new MathSin());
		context.registerNativeHandler(new MathSinh());
		context.registerNativeHandler(new MathSqrt());
		context.registerNativeHandler(new MathTan());
		context.registerNativeHandler(new MathTanh());

		context.registerNativeHandler(new ObjectGetClass());
		context.registerNativeHandler(new ObjectWait());
		context.registerNativeHandler(new ObjectNotify());
		context.registerNativeHandler(new ObjectNotifyAll());
		context.registerNativeHandler(new ObjectClone());

		context.registerNativeHandler(new ThreadCurrentThread());
		context.registerNativeHandler(new ThreadStart0());
		context.registerNativeHandler(new ThreadSetPriority0());
		context.registerNativeHandler(new ThreadYield());
		context.registerNativeHandler(new ThreadSleep());
		context.registerNativeHandler(new ThreadInterrupt0());
		context.registerNativeHandler(new ThreadIsInterrupted());
		context.registerNativeHandler(new ThreadIsAlive());
		context.registerNativeHandler(new ThreadSetPriority0());

		context.registerNativeHandler(new ThrowableFillInStackTrace0());

		context.registerNativeHandler(new FiScEVMDecode());
		context.registerNativeHandler(new FiScEVMEncode());
		context.registerNativeHandler(new FiScEVMThrowOut());
		context.registerNativeHandler(new FiScEVMExit());
		context.registerNativeHandler(new FiScEVMGetDoubleRaw());
		context.registerNativeHandler(new FiScEVMGetFloatRaw());
		context.registerNativeHandler(new FiScEVMStringToDouble());
		context.registerNativeHandler(new FiScEVMStringToFloat());
		context.registerNativeHandler(new FiScEVMDoubleToString());
		context.registerNativeHandler(new FiScEVMFloatToString());
		context.registerNativeHandler(new FiScEVMDebugOut());

		context.registerNativeHandler(new SystemOutStreamWrite0());

		context.registerNativeHandler(new SystemInputStreamRead0());

		context.registerNativeHandler(new StringIntern());

		context.registerNativeHandler(new ResourceInputStreamClose0(this));
		context.registerNativeHandler(new ResourceInputStreamRead0a(this));
		context.registerNativeHandler(new ResourceInputStreamRead0b(this));
	}

	public void setContext(VMContext context) {
		this.context = context;
	}

	public void setProperty(String key, String value) {
		properties.put(key, value);
	}

	public String getProperty(String key) {
		return properties.get(key);
	}

	public InputStream getResource(String name) {
		return resources.get(name);
	}

	public void putResource(String name, InputStream is) {
		try {
			closeResource(name);
		} catch (IOException e) {
		}
		resources.put(name, is);
	}

	public void closeResource(String name) throws IOException {
		InputStream is = getResource(name);
		if (is != null) {
			is.close();
			resources.put(name, null);
		}
	}

}

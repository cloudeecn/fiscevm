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

import com.cirnoworks.fisce.env.minimal.nh.*;
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
		
		context.registerNativeHandler(new StringUtilsDigit());
		context.registerNativeHandler(new StringUtilsFormat());
		context.registerNativeHandler(new StringUtilsGetDirectionality());
		context.registerNativeHandler(new StringUtilsGetNumbericValue());
		context.registerNativeHandler(new StringUtilsGetType());
		context.registerNativeHandler(new StringUtilsIsDefined());
		context.registerNativeHandler(new StringUtilsIsDigit());
		context.registerNativeHandler(new StringUtilsIsIdentifierIgnorable());
		context.registerNativeHandler(new StringUtilsIsLetter());
		context.registerNativeHandler(new StringUtilsIsLetterOrDigit());
		context.registerNativeHandler(new StringUtilsIsLowerCase());
		context.registerNativeHandler(new StringUtilsIsMirrored());
		context.registerNativeHandler(new StringUtilsIsSpaceChar());
		context.registerNativeHandler(new StringUtilsIsTitleCase());
		context.registerNativeHandler(new StringUtilsIsUnicodeIdentifierPart());
		context.registerNativeHandler(new StringUtilsIsUnicodeIdentifierStart());
		context.registerNativeHandler(new StringUtilsIsUpperCase());
		context.registerNativeHandler(new StringUtilsIsWhitespace());
		context.registerNativeHandler(new StringUtilsToLowerCase());
		context.registerNativeHandler(new StringUtilsToLowerCaseI());
		context.registerNativeHandler(new StringUtilsToTitleCaseI());
		context.registerNativeHandler(new StringUtilsToUpperCase());
		context.registerNativeHandler(new StringUtilsToUpperCaseI());

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

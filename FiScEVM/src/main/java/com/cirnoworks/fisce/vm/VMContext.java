/**
 *  Copyright 2010 Yuxuan Huang
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the Lesser GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.cirnoworks.fisce.jvm13;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map.Entry;

import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.SAXReader;
import org.dom4j.io.XMLWriter;

import com.cirnoworks.fisce.jvm13.data.AbstractClass;
import com.cirnoworks.fisce.jvm13.data.ClassBase;
import com.cirnoworks.fisce.jvm13.data.ClassField;
import com.cirnoworks.fisce.jvm13.data.ClassMethod;

/**
 * 
 * @author yuxuanhuang
 */
public class VMContext implements FiScEVM {

	public static final int MAX_METHODS = 65536;
	public static final int MAX_FIELDS = 65536;
	public static final int MAX_CLASSES = 65536;

	public static final int CLINIT_NONE = -1;
	public static final int CLINIT_FINISHED = -2;

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.jvm13.FiScEVM#bootFromData(java.io.InputStream)
	 */
	public void bootFromData(InputStream is) throws VMCriticalException {
		try {
			SAXReader reader = new SAXReader();
			Document document = reader.read(is);
			Element root = document.getRootElement();

			Element classes = root.element("classes");
			classCount = Integer.parseInt(classes.attributeValue("next"));
			for (Element ce : (List<Element>) classes.elements("class")) {
				int cid = Integer.parseInt(ce.attributeValue("cid"));
				int clinited = Integer.parseInt(ce.attributeValue("clinited"));
				int handle = Integer.parseInt(ce.attributeValue("handle"));
				String name = ce.getText();
				classMap.put(name, cid);
				classObjectMap.put(name, handle);
				classObjectMapRev.put(handle, name);
				this.classClinitThreadId[cid] = clinited;
			}

			Element methods = root.element("methods");
			methodCount = Integer.parseInt(methods.attributeValue("next"));
			for (Element me : (List<Element>) methods.elements("method")) {
				methodMap.put(me.getText(),
						Integer.parseInt(me.attributeValue("mid")));
			}

			Element fields = root.element("fields");
			fieldCount = Integer.parseInt(fields.attributeValue("next"));
			for (Element fe : (List<Element>) fields.elements("field")) {
				fieldMap.put(fe.getText(),
						Integer.parseInt(fe.attributeValue("fid")));
			}

			Element heapElement = root.element("heap");
			if (heap == null
					|| !heap.getClass().getCanonicalName()
							.equals(heapElement.attributeValue("class"))) {
				throw new VMCriticalException("Heap processer is wrong!");
			}

			Element tmElement = root.element("threadManager");
			if (threadManager == null
					|| !threadManager.getClass().getCanonicalName()
							.equals(tmElement.attributeValue("class"))) {
				throw new VMCriticalException("Thread manager is wrong!");
			}

			Element tks = root.element("toolkits");
			for (Element tke : (List<Element>) tks.elements("toolkit")) {
				String tkName = tke.attributeValue("class");
				boolean found = false;
				for (IToolkit toolkit : toolkits) {
					if (toolkit.getClass().getCanonicalName().equals(tkName)) {
						found = true;
						toolkit.loadData(tke);
						break;
					}
				}
				if (!found) {
					throw new VMCriticalException("Toolkit " + tkName
							+ " not found!");
				}
			}

			if (classLoader == null
					|| !classLoader
							.getClass()
							.getCanonicalName()
							.equals(root.element("classloader").attributeValue(
									"class"))) {
				throw new VMCriticalException("Class loader is wrong!");
			}

			initialize();
			heap.loadData(heapElement);
			String[] classNames = classMap.keySet().toArray(
					new String[classMap.keySet().size()]);
			for (String cn : classNames) {
				getClass(cn);
			}
			threadManager.loadData(tmElement);
		} catch (Exception e) {
			throw new VMCriticalException(e);
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.jvm13.FiScEVM#saveData(java.io.InputStream)
	 */
	public void saveData(OutputStream os) throws VMCriticalException,
			IOException {
		Document document = DocumentHelper.createDocument();
		Element context = document.addElement("vmcontext");

		Element classes = context.addElement("classes").addAttribute("next",
				String.valueOf(classCount));
		for (Entry<String, Integer> cm : classMap.entrySet()) {
			String name = cm.getKey();
			int id = cm.getValue();
			int cl = classClinitThreadId[id];
			int handle = classObjectMap.get(name);
			classes.addElement("class").addAttribute("cid", String.valueOf(id))
					.addAttribute("clinited", String.valueOf(cl))
					.addAttribute("handle", String.valueOf(handle))
					.addText(name);
		}

		Element methods = context.addElement("methods").addAttribute("next",
				String.valueOf(methodCount));
		for (Entry<String, Integer> mm : methodMap.entrySet()) {
			methods.addElement("method")
					.addAttribute("mid", mm.getValue().toString())
					.addText(mm.getKey());
		}
		Element fields = context.addElement("fields").addAttribute("next",
				String.valueOf(fieldCount));
		for (Entry<String, Integer> fm : fieldMap.entrySet()) {
			fields.addElement("field")
					.addAttribute("fid", fm.getValue().toString())
					.addText(fm.getKey());
		}
		heap.saveData(context.addElement("heap").addAttribute("class",
				heap.getClass().getCanonicalName()));
		threadManager.saveData(context.addElement("threadManager")
				.addAttribute("class",
						threadManager.getClass().getCanonicalName()));
		context.addElement("classloader").addAttribute("class",
				classLoader.getClass().getCanonicalName());
		Element tks = context.addElement("toolkits");
		for (IToolkit toolkit : toolkits) {
			toolkit.saveData(tks.addElement("toolkit").addAttribute("class",
					toolkit.getClass().getCanonicalName()));
		}

		OutputFormat format = OutputFormat.createPrettyPrint();
		XMLWriter writer = new XMLWriter(os, format);
		writer.write(document);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.jvm13.FiScEVM#bootFromClass(java.lang.String)
	 */
	public void bootFromClass(String name) throws VMException,
			VMCriticalException {
		initialize();
		ClassBase cb = (ClassBase) getClass(name);
		threadManager.bootFromMain(cb);
	}

	public void start() throws VMException {
		threadManager.start();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.jvm13.FiScEVM#requestStop()
	 */
	public void requestStop() {
		threadManager.requestStop();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.jvm13.FiScEVM#waitTillStopped(long)
	 */
	public int waitTillStopped(long waitTime) throws InterruptedException {
		return threadManager.waitTillStopped(waitTime);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.cirnoworks.fisce.jvm13.FiScEVM#setClassLoader(com.cirnoworks.fisce
	 * .jvm13.IClassLoader)
	 */
	public synchronized void setClassLoader(IClassLoader classLoader) {
		if (this.classLoader != null) {
			throw new IllegalStateException("class loader already set!");
		}
		this.classLoader = classLoader;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.cirnoworks.fisce.jvm13.FiScEVM#registerNativeHandler(com.cirnoworks
	 * .fisce.jvm13.INativeHandler)
	 */
	public void registerNativeHandler(INativeHandler inh) {
		nativeHandlers.put(inh.getUniqueName(), inh);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.cirnoworks.fisce.jvm13.FiScEVM#addStateListener(com.cirnoworks.fisce
	 * .jvm13.IStateListener)
	 */
	public void addStateListener(IStateListener isl) {
		statusListeners.add(isl);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.cirnoworks.fisce.jvm13.FiScEVM#setThreadManager(com.cirnoworks.fisce
	 * .jvm13.IThreadManager)
	 */
	public void setThreadManager(IThreadManager threadManager) {
		this.threadManager = threadManager;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.cirnoworks.fisce.jvm13.FiScEVM#setHeap(com.cirnoworks.fisce.jvm13
	 * .IHeap)
	 */
	public void setHeap(IHeap heap) {
		this.heap = heap;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.cirnoworks.fisce.jvm13.FiScEVM#setConsole(com.cirnoworks.fisce.jvm13
	 * .IDebugConsole)
	 */
	public void setConsole(IDebugConsole console) {
		this.console = console;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.cirnoworks.fisce.jvm13.FiScEVM#setThrower(com.cirnoworks.fisce.jvm13
	 * .IThrower)
	 */
	public void setThrower(IThrower thrower) {
		this.thrower = thrower;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.jvm13.FiScEVM#exit(int)
	 */
	public void exit(int code) {
		threadManager.exit(code);
	}

	/*
	 * ***********************
	 * service beans, must have NO instance fields.:
	 */
	private IClassLoader classLoader;
	private IDebugConsole console;
	private IThrower thrower;
	private List<IStateListener> statusListeners = new ArrayList<IStateListener>();
	private HashMap<String, INativeHandler> nativeHandlers = new HashMap<String, INativeHandler>();

	/*
	 * ***********************
	 * Data need to be persisted:
	 */
	private IHeap heap;
	private IThreadManager threadManager;
	private ArrayList<IToolkit> toolkits = new ArrayList<IToolkit>();

	// Method unique name --> method id
	private HashMap<String, Integer> methodMap = new HashMap<String, Integer>();
	// Field unique name --> field id
	private HashMap<String, Integer> fieldMap = new HashMap<String, Integer>();
	// Class name --> class id
	private HashMap<String, Integer> classMap = new HashMap<String, Integer>();
	// Class name -> Class Object handle
	private HashMap<String, Integer> classObjectMap = new HashMap<String, Integer>();
	private HashMap<Integer, String> classObjectMapRev = new HashMap<Integer, String>();

	private int[] classClinitThreadId = new int[MAX_CLASSES];

	private int methodCount = 0;
	private int fieldCount = 0;
	private int classCount = 0;

	// *************************************************
	// no persist data following will be restored manually when loading
	//
	// methods[method id]
	private ClassMethod[] methods = new ClassMethod[MAX_METHODS];

	// fields[field id]
	private ClassField[] fields = new ClassField[MAX_FIELDS];

	// classes[class id]
	private AbstractClass[] classes = new AbstractClass[MAX_CLASSES];

	/*
	 * ***********************
	 * Self-service methods:
	 */

	public Collection<Integer> getClassIds() {
		return Collections.unmodifiableCollection(classMap.values());
	}

	public AbstractClass[] getClasses() {
		return classes;
	}

	//
	public void initialize() {

		for (IToolkit toolkit : toolkits) {
			toolkit.setContext(this);
		}
		threadManager.setContext(this);
		heap.setContext(this);
		for (IToolkit toolkit : toolkits) {
			toolkit.setupContext();
		}

	}

	public VMContext() {
		for (int i = 0; i < MAX_CLASSES; i++) {
			classClinitThreadId[i] = CLINIT_NONE;
		}
	}

	private AbstractClass getClazz(String className) {
		Integer cid = getClazzId(className);
		if (cid == null) {
			return null;
		}
		AbstractClass ret = getClazzById(cid);
		assert ret == null || ret.getName().equals(className);
		return ret;
	}

	/*
	 * ***********************
	 * Service methods for inner VM and Native
	 */

	public List<IToolkit> getToolkits() {
		return toolkits;
	}

	public void addToolkit(IToolkit toolkit) {
		toolkits.add(toolkit);
	}

	public INativeHandler getNativeHandler(String uniqueName) {
		return nativeHandlers.get(uniqueName);
	}

	public IClassLoader getClassLoader() {
		return classLoader;
	}

	public IThreadManager getThreadManager() {
		return threadManager;
	}

	public IHeap getHeap() {
		return heap;
	}

	public IDebugConsole getConsole() {
		return console;
	}

	public synchronized void registarClazz(AbstractClass clazz)
			throws VMException {
		Integer cid = getClazzId(clazz.getName());
		if (cid == null) {
			// never loaded even in saved context.
			cid = classCount++;
			classMap.put(clazz.getName(), cid);
			classes[cid] = clazz;
			heap.initStaticAreaForClass(clazz);
		} else {
			assert classes[cid] == null;
			classes[cid] = clazz;
		}
	}

	public int getClassObjectHandleForClass(AbstractClass clazz) {
		return classObjectMap.get(clazz.getName());
	}

	public AbstractClass getClassForClassObjectHandle(int handle)
			throws VMException, VMCriticalException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		return getClass(classObjectMapRev.get(handle));
	}

	// private int fp;

	public final Integer getClazzId(String className) {
		return classMap.get(className);
	}

	public final Integer getClazzId(AbstractClass clazz) {
		return classMap.get(clazz.getName());
	}

	public final AbstractClass getClazzById(int cid) {
		return classes[cid];
	}

	public final int getClinitThreadId(ClassBase clazz) {
		Integer cid = getClazzId(clazz);
		assert cid != null;
		return classClinitThreadId[cid];
	}

	public final void setClinited(ClassBase clazz, IThread thread) {
		Integer cid = getClazzId(clazz);
		assert cid != null;
		classClinitThreadId[cid] = thread.getThreadId();
	}

	public final void finishClinited(ClassBase clazz) {
		Integer cid = getClazzId(clazz);
		assert cid != null;
		classClinitThreadId[cid] = CLINIT_FINISHED;
	}

	public synchronized AbstractClass getClass(String name) throws VMException,
			VMCriticalException {
		AbstractClass ret = getClazz(name);
		if (ret == null) {
			ret = classLoader.loadClassByName(name, this);
			registarClazz(ret);
			classLoader.verifyPass2(ret, this);
			Integer classObjHandle = classObjectMap.get(name);
			if (classObjHandle == null) {
				classObjHandle = heap
						.allocate((ClassBase) getClass("java/lang/Class"));
				classObjectMap.put(name, classObjHandle);
				assert !classObjectMapRev.containsKey(classObjHandle);
				classObjectMapRev.put(classObjHandle, name);
			}
		}
		return ret;
	}

	public synchronized AbstractClass getClass(int handle) throws VMException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		int cid = heap.getClass(handle);
		return classes[cid];
	}

	public ClassMethod getMethod(String uniqueName) {
		Integer mid = getMethodId(uniqueName);
		if (mid == null)
			return null;
		ClassMethod ret = methods[mid];
		// assert ret == null || ret.getUniqueName().equals(uniqueName) : mid
		// + " " + ret + " " + uniqueName;
		return ret;
	}

	public ClassMethod lookupMethodVirtual(ClassBase clazz, String methodName) {
		String uniqueName = clazz.getName() + "." + methodName;
		while (clazz != null) {
			Integer mid = getMethodId(clazz.getName() + "." + methodName);
			if (mid != null) {
				methodMap.put(uniqueName, mid);
				return getMethodById(mid);
			}
			clazz = clazz.getSuperClass();
		}
		return null;
	}

	public Integer getMethodId(String uniqueName) {
		return methodMap.get(uniqueName);
	}

	public Integer getMethodId(ClassMethod method) {
		return getMethodId(method.getUniqueName());
	}

	public ClassMethod getMethodById(int id) {
		return methods[id];
	}

	public synchronized void registerMethod(ClassMethod method) {
		Integer mid = methodMap.get(method.getUniqueName());
		if (mid == null) {
			mid = methodCount++;
			methodMap.put(method.getUniqueName(), mid);
			// context.println("put " + method.getUniqueName() + " " + mid);
		}
		// context.println("reg " + method.getUniqueName() + " " + mid);
		assert methods[mid] == null;
		methods[mid] = method;
	}

	public ClassField getField(String uniqueName) {
		Integer fid = fieldMap.get(uniqueName);
		if (fid == null) {
			return null;
		}
		ClassField ret = fields[fid];
		// assert ret == null || ret.getUniqueName().equals(uniqueName);
		return ret;
	}

	public ClassField lookupFieldVirtual(ClassBase clazz, String fieldName) {
		String uniqueName = clazz.getName() + "." + fieldName;
		while (clazz != null) {
			Integer fid = getFieldId(clazz.getName() + "." + fieldName);
			if (fid != null) {
				fieldMap.put(uniqueName, fid);
				return getFieldById(fid);
			}
			clazz = clazz.getSuperClass();
		}
		return null;
	}

	public Integer getFieldId(String uniqueName) {
		return fieldMap.get(uniqueName);
	}

	public ClassField getFieldById(int id) {
		return fields[id];
	}

	public synchronized void registerField(ClassField field) {
		Integer fid = fieldMap.get(field.getUniqueName());
		if (fid == null) {
			fid = fieldCount++;
			fieldMap.put(field.getUniqueName(), fid);
		}
		assert fields[fid] == null;
		fields[fid] = field;
	}

	public void onStateChange(int state) {
		for (IStateListener listener : statusListeners) {
			listener.onStateChange(state);
		}
	}

	public IThrower getThrower() {
		return thrower;
	}

}

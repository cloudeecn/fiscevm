package com.cirnoworks.libfisce.shell;

import java.nio.ByteBuffer;

import com.cirnoworks.fisce.intf.IHeap;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.intf.idata.IClassArray;
import com.cirnoworks.fisce.intf.idata.IClassBase;
import com.cirnoworks.fisce.intf.idata.IField;

public class FYHeap implements IHeap {

	private FYContext context;

	public FYHeap(FYContext context) {
		super();
		this.context = context;
	}

	public int allocate(IClassBase clazz) throws VMException,
			VMCriticalException {
		return FisceService.allocateObject(context.getContext(),
				((FYClass) clazz).getId());
	}

	public int allocate(IClassArray clazz, int length) throws VMException,
			VMCriticalException {
		return FisceService.allocateArray(context.getContext(),
				((FYClass) clazz).getId(), length);
	}

	public int clone(int handle) throws VMException, VMCriticalException {
		return FisceService.clone(context.getContext(), handle);
	}

	public int getInternString(String content) throws VMException,
			VMCriticalException {
		return FisceService.getLiteral(context.getContext(), content);
	}

	public int getClass(int handle) throws VMException {
		return FisceService.getClassOfHandle(context.getContext(), handle);
	}

	public int getArrayLength(int handle) throws VMException,
			VMCriticalException {
		return FisceService.getArrayLength(context.getContext(), handle);
	}

	public boolean getFieldBoolean(int handle, IField field) throws VMException {
		return FisceService.getFieldBoolean(context.getContext(), handle,
				((FYField) field).getId());
	}

	public byte getFieldByte(int handle, IField field) throws VMException {
		return FisceService.getFieldByte(context.getContext(), handle,
				((FYField) field).getId());
	}

	public short getFieldShort(int handle, IField field) throws VMException {
		return FisceService.getFieldShort(context.getContext(), handle,
				((FYField) field).getId());
	}

	public char getFieldChar(int handle, IField field) throws VMException {
		return FisceService.getFieldChar(context.getContext(), handle,
				((FYField) field).getId());
	}

	public int getFieldInt(int handle, IField field) throws VMException {
		return FisceService.getFieldInt(context.getContext(), handle,
				((FYField) field).getId());
	}

	public long getFieldLong(int handle, IField field) throws VMException {
		return FisceService.getFieldLong(context.getContext(), handle,
				((FYField) field).getId());
	}

	public float getFieldFloat(int handle, IField field) throws VMException {
		return FisceService.getFieldFloat(context.getContext(), handle,
				((FYField) field).getId());
	}

	public double getFieldDouble(int handle, IField field) throws VMException {
		return FisceService.getFieldDouble(context.getContext(), handle,
				((FYField) field).getId());
	}

	public int getFieldHandle(int handle, IField field) throws VMException {
		return FisceService.getFieldHandle(context.getContext(), handle,
				((FYField) field).getId());
	}

	public void putFieldBoolean(int handle, IField field, boolean value)
			throws VMException {
		FisceService.setFieldBoolean(context.getContext(), handle,
				((FYField) field).getId(), value);

	}

	public void putFieldByte(int handle, IField field, byte value)
			throws VMException {
		FisceService.setFieldByte(context.getContext(), handle,
				((FYField) field).getId(), value);
	}

	public void putFieldShort(int handle, IField field, short value)
			throws VMException {
		FisceService.setFieldShort(context.getContext(), handle,
				((FYField) field).getId(), value);
	}

	public void putFieldChar(int handle, IField field, char value)
			throws VMException {
		FisceService.setFieldChar(context.getContext(), handle,
				((FYField) field).getId(), value);
	}

	public void putFieldInt(int handle, IField field, int value)
			throws VMException {
		FisceService.setFieldInt(context.getContext(), handle,
				((FYField) field).getId(), value);
	}

	public void putFieldLong(int handle, IField field, long value)
			throws VMException {
		FisceService.setFieldLong(context.getContext(), handle,
				((FYField) field).getId(), value);
	}

	public void putFieldFloat(int handle, IField field, float value)
			throws VMException {
		FisceService.setFieldFloat(context.getContext(), handle,
				((FYField) field).getId(), value);
	}

	public void putFieldDouble(int handle, IField field, double value)
			throws VMException {
		FisceService.setFieldDouble(context.getContext(), handle,
				((FYField) field).getId(), value);
	}

	public void putFieldHandle(int handle, IField field, int value)
			throws VMException {
		FisceService.setFieldHandle(context.getContext(), handle,
				((FYField) field).getId(), value);
	}

	public boolean getArrayBoolean(int handle, int index) throws VMException,
			VMCriticalException {
		return FisceService
				.getArrayBoolean(context.getContext(), handle, index);
	}

	public byte getArrayByte(int handle, int index) throws VMException,
			VMCriticalException {
		return FisceService.getArrayByte(context.getContext(), handle, index);
	}

	public short getArrayShort(int handle, int index) throws VMException,
			VMCriticalException {
		return FisceService.getArrayShort(context.getContext(), handle, index);
	}

	public char getArrayChar(int handle, int index) throws VMException,
			VMCriticalException {
		return FisceService.getArrayChar(context.getContext(), handle, index);
	}

	public int getArrayInt(int handle, int index) throws VMException,
			VMCriticalException {
		return FisceService.getArrayInt(context.getContext(), handle, index);
	}

	public long getArrayLong(int handle, int index) throws VMException,
			VMCriticalException {
		return FisceService.getArrayLong(context.getContext(), handle, index);
	}

	public float getArrayFloat(int handle, int index) throws VMException,
			VMCriticalException {
		return FisceService.getArrayFloat(context.getContext(), handle, index);
	}

	public double getArrayDouble(int handle, int index) throws VMException,
			VMCriticalException {
		return FisceService.getArrayDouble(context.getContext(), handle, index);
	}

	public int getArrayHandle(int handle, int index) throws VMException,
			VMCriticalException {
		return FisceService.getArrayHandle(context.getContext(), handle, index);
	}

	public void putArrayBoolean(int handle, int index, boolean value)
			throws VMException, VMCriticalException {
		FisceService
				.setArrayBoolean(context.getContext(), handle, index, value);
	}

	public void putArrayByte(int handle, int index, byte value)
			throws VMException, VMCriticalException {
		FisceService.setArrayByte(context.getContext(), handle, index, value);
	}

	public void putArrayShort(int handle, int index, short value)
			throws VMException, VMCriticalException {
		FisceService.setArrayShort(context.getContext(), handle, index, value);
	}

	public void putArrayChar(int handle, int index, char value)
			throws VMException, VMCriticalException {
		FisceService.setArrayChar(context.getContext(), handle, index, value);
	}

	public void putArrayInt(int handle, int index, int value)
			throws VMException, VMCriticalException {
		FisceService.setArrayInt(context.getContext(), handle, index, value);
	}

	public void putArrayLong(int handle, int index, long value)
			throws VMException, VMCriticalException {
		FisceService.setArrayLong(context.getContext(), handle, index, value);
	}

	public void putArrayFloat(int handle, int index, float value)
			throws VMException, VMCriticalException {
		FisceService.setArrayFloat(context.getContext(), handle, index, value);
	}

	public void putArrayDouble(int handle, int index, double value)
			throws VMException, VMCriticalException {
		FisceService.setArrayDouble(context.getContext(), handle, index, value);
	}

	public void putArrayHandle(int handle, int index, int value)
			throws VMException, VMCriticalException {
		FisceService.setArrayHandle(context.getContext(), handle, index, value);
	}

	public void fillArrayBoolean(int handle, int dstPos, boolean[] src,
			int srcPos, int len) throws VMException, VMCriticalException {
		if (src.length < srcPos + len) {
			throw new IndexOutOfBoundsException(srcPos + " " + len);
		}
		FisceService.fillArrayBoolean(context.getContext(), handle, dstPos,
				src, srcPos, len);

	}

	public void fillArrayByte(int handle, int dstPos, byte[] src, int srcPos,
			int len) throws VMException, VMCriticalException {
		if (src.length < srcPos + len) {
			throw new IndexOutOfBoundsException(srcPos + " " + len);
		}
		FisceService.fillArrayByte(context.getContext(), handle, dstPos, src,
				srcPos, len);
	}

	public void fillArrayShort(int handle, int dstPos, short[] src, int srcPos,
			int len) throws VMException, VMCriticalException {
		if (src.length < srcPos + len) {
			throw new IndexOutOfBoundsException(srcPos + " " + len);
		}
		FisceService.fillArrayShort(context.getContext(), handle, dstPos, src,
				srcPos, len);
	}

	public void fillArrayChar(int handle, int dstPos, char[] src, int srcPos,
			int len) throws VMException, VMCriticalException {
		if (src.length < srcPos + len) {
			throw new IndexOutOfBoundsException(srcPos + " " + len);
		}
		FisceService.fillArrayChar(context.getContext(), handle, dstPos, src,
				srcPos, len);
	}

	public void fillArrayInt(int handle, int dstPos, int[] src, int srcPos,
			int len) throws VMException, VMCriticalException {
		if (src.length < srcPos + len) {
			throw new IndexOutOfBoundsException(srcPos + " " + len);
		}
		FisceService.fillArrayInt(context.getContext(), handle, dstPos, src,
				srcPos, len);
	}

	public void fillArrayLong(int handle, int dstPos, long[] src, int srcPos,
			int len) throws VMException, VMCriticalException {
		if (src.length < srcPos + len) {
			throw new IndexOutOfBoundsException(srcPos + " " + len);
		}
		FisceService.fillArrayLong(context.getContext(), handle, dstPos, src,
				srcPos, len);
	}

	public void fillArrayFloat(int handle, int dstPos, float[] src, int srcPos,
			int len) throws VMException, VMCriticalException {
		if (src.length < srcPos + len) {
			throw new IndexOutOfBoundsException(srcPos + " " + len);
		}
		FisceService.fillArrayFloat(context.getContext(), handle, dstPos, src,
				srcPos, len);
	}

	public void fillArrayDouble(int handle, int dstPos, double[] src,
			int srcPos, int len) throws VMException, VMCriticalException {
		if (src.length < srcPos + len) {
			throw new IndexOutOfBoundsException(srcPos + " " + len);
		}
		FisceService.fillArrayDouble(context.getContext(), handle, dstPos, src,
				srcPos, len);
	}

	public void getArrayBoolean(boolean[] dst, int dstPos, int handle,
			int srcPos, int len) throws VMException, VMCriticalException {
		if (dst.length < dstPos + len) {
			throw new IndexOutOfBoundsException(dstPos + " " + len);
		}
		FisceService.fetchArrayBoolean(context.getContext(), dst, dstPos,
				handle, srcPos, len);
	}

	public void getArrayByte(byte[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException {
		if (dst.length < dstPos + len) {
			throw new IndexOutOfBoundsException(dstPos + " " + len);
		}
		FisceService.fetchArrayByte(context.getContext(), dst, dstPos, handle,
				srcPos, len);
	}

	public void getArrayShort(short[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException {
		if (dst.length < dstPos + len) {
			throw new IndexOutOfBoundsException(dstPos + " " + len);
		}
		FisceService.fetchArrayShort(context.getContext(), dst, dstPos, handle,
				srcPos, len);
	}

	public void getArrayChar(char[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException {
		if (dst.length < dstPos + len) {
			throw new IndexOutOfBoundsException(dstPos + " " + len);
		}
		FisceService.fetchArrayChar(context.getContext(), dst, dstPos, handle,
				srcPos, len);
	}

	public void getArrayInt(int[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException {
		if (dst.length < dstPos + len) {
			throw new IndexOutOfBoundsException(dstPos + " " + len);
		}
		FisceService.fetchArrayInt(context.getContext(), dst, dstPos, handle,
				srcPos, len);
	}

	public void getArrayLong(long[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException {
		if (dst.length < dstPos + len) {
			throw new IndexOutOfBoundsException(dstPos + " " + len);
		}
		FisceService.fetchArrayLong(context.getContext(), dst, dstPos, handle,
				srcPos, len);
	}

	public void getArrayFloat(float[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException {
		if (dst.length < dstPos + len) {
			throw new IndexOutOfBoundsException(dstPos + " " + len);
		}
		FisceService.fetchArrayFloat(context.getContext(), dst, dstPos, handle,
				srcPos, len);
	}

	public void getArrayDouble(double[] dst, int dstPos, int handle,
			int srcPos, int len) throws VMException, VMCriticalException {
		if (dst.length < dstPos + len) {
			throw new IndexOutOfBoundsException(dstPos + " " + len);
		}
		FisceService.fetchArrayDouble(context.getContext(), dst, dstPos,
				handle, srcPos, len);
	}

	public int getLiteral(String str) throws VMException, VMCriticalException {
		return FisceService.getLiteral(context.getContext(), str);
	}

	public String getString(int handle) throws VMException, VMCriticalException {
		return FisceService.getString(context.getContext(), handle);
	}

	public int putString(String content) throws VMException,
			VMCriticalException {
		return FisceService.putString(context.getContext(), content);
	}

	public boolean getStaticBoolean(IField field) throws VMException {
		return FisceService.getStaticBoolean(context.getContext(),
				((FYField) field).getId());
	}

	public byte getStaticByte(IField field) throws VMException {
		return FisceService.getStaticByte(context.getContext(),
				((FYField) field).getId());
	}

	public short getStaticShort(IField field) throws VMException {
		return FisceService.getStaticShort(context.getContext(),
				((FYField) field).getId());
	}

	public char getStaticChar(IField field) throws VMException {
		return FisceService.getStaticChar(context.getContext(),
				((FYField) field).getId());
	}

	public int getStaticInt(IField field) throws VMException {
		return FisceService.getStaticInt(context.getContext(),
				((FYField) field).getId());
	}

	public long getStaticLong(IField field) throws VMException {
		return FisceService.getStaticLong(context.getContext(),
				((FYField) field).getId());
	}

	public float getStaticFloat(IField field) throws VMException {
		return FisceService.getStaticFloat(context.getContext(),
				((FYField) field).getId());
	}

	public double getStaticDouble(IField field) throws VMException {
		return FisceService.getStaticDouble(context.getContext(),
				((FYField) field).getId());
	}

	public void setStaticBoolean(IField field, boolean value)
			throws VMException {
		FisceService.setStaticBoolean(context.getContext(),
				((FYField) field).getId(), value);
	}

	public void setStaticByte(IField field, byte value) throws VMException {
		FisceService.setStaticByte(context.getContext(),
				((FYField) field).getId(), value);
	}

	public void setStaticShort(IField field, short value) throws VMException {
		FisceService.setStaticShort(context.getContext(),
				((FYField) field).getId(), value);
	}

	public void setStaticChar(IField field, char value) throws VMException {
		FisceService.setStaticChar(context.getContext(),
				((FYField) field).getId(), value);
	}

	public void setStaticInt(IField field, int value) throws VMException {
		FisceService.setStaticInt(context.getContext(),
				((FYField) field).getId(), value);
	}

	public void setStaticLong(IField field, long value) throws VMException {
		FisceService.setStaticLong(context.getContext(),
				((FYField) field).getId(), value);
	}

	public void setStaticFloat(IField field, float value) throws VMException {
		FisceService.setStaticFloat(context.getContext(),
				((FYField) field).getId(), value);
	}

	public void setStaticDouble(IField field, double value) throws VMException {
		FisceService.setStaticDouble(context.getContext(),
				((FYField) field).getId(), value);
	}

	public void getArrayByteBuffer(ByteBuffer dst, int handle, int srcPos,
			int len) {
		FisceService.fetchArrayByteBuffer(context.getContext(), dst, handle,
				srcPos, len);
	}

}

package com.cirnoworks.fisce.intf.idata;

public class Message {
	public static final int message_invoke_native = 3;// Thread And TM pass
														// thread
	public static final int message_exception = 4; // Thread And TM pass thread
	public static final int message_sleep = 5; // TM Only
	public static final int message_vm_dead = 6;

	private int messageType;
	private int threadId;
	private String nativeUniqueName;
	private String exceptionName;
	private String exceptionDesc;
	private long sleepTime;

	public int getMessageType() {
		return messageType;
	}

	public void setMessageType(int messageType) {
		this.messageType = messageType;
	}

	public int getThreadId() {
		return threadId;
	}

	public void setThreadId(int threadId) {
		this.threadId = threadId;
	}

	public String getNativeUniqueName() {
		return nativeUniqueName;
	}

	public void setNativeUniqueName(String nativeUniqueName) {
		this.nativeUniqueName = nativeUniqueName;
	}

	public String getExceptionName() {
		return exceptionName;
	}

	public void setExceptionName(String exceptionName) {
		this.exceptionName = exceptionName;
	}

	public String getExceptionDesc() {
		return exceptionDesc;
	}

	public void setExceptionDesc(String exceptionDesc) {
		this.exceptionDesc = exceptionDesc;
	}

	public long getSleepTime() {
		return sleepTime;
	}

	public void setSleepTime(long sleepTime) {
		this.sleepTime = sleepTime;
	}

}

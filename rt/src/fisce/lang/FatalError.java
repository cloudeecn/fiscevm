package fisce.lang;

public class FatalError extends Error {

	public FatalError() {
		super();
	}

	public FatalError(String message, Throwable cause) {
		super(message, cause);
	}

	public FatalError(String message) {
		super(message);
	}

	public FatalError(Throwable cause) {
		super(cause);
	}

}

package java.net;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.List;
import java.util.Map;

public abstract class URLConnection {

	protected final URL url;

	public abstract void connect();

	public URLConnection(URL url) {
		this.url = url;
	}

	public boolean getAllowUserInteraction() {
		return false;
	}

	public Object getContent() throws IOException {
		return null;
	}

	public Object getContent(Class[] types) throws IOException {
		return null;
	}

	public String getContentEncoding() {
		return null;
	}

	public int getContentLength() {
		return 0;
	}

	public String getContentType() {
		return null;
	}

	public long getDate() {
		return 0;
	}

	public boolean getDefaultUseCaches() {
		return false;
	}

	public boolean getDoInput() {
		return false;
	}

	public boolean getDoOutput() {
		return false;
	}

	public long getExpiration() {
		return 0;
	}

	public String getHeaderField(int pos) {
		return null;
	}

	public Map<String, List<String>> getHeaderFields() {
		return null;
	}

	public Map<String, List<String>> getRequestProperties() {
		return null;
	}

	public void addRequestProperty(String field, String newValue) {
	}

	public String getHeaderField(String key) {
		return null;
	}

	public long getHeaderFieldDate(String field, long defaultValue) {
		return 0;
	}

	public int getHeaderFieldInt(String field, int defaultValue) {
		return 0;
	}

	public String getHeaderFieldKey(int posn) {
		return null;
	}

	public long getIfModifiedSince() {
		return 0;
	}

	public InputStream getInputStream() throws IOException {
		return null;
	}

	public long getLastModified() {
		return 0;
	}

	public OutputStream getOutputStream() throws IOException {
		return null;
	}

	public String getRequestProperty(String field) {
		return null;
	}

	public URL getURL() {
		return url;
	}

	public boolean getUseCaches() {
		return false;
	}

	public void setAllowUserInteraction(boolean newValue) {
	}

	public void setDefaultUseCaches(boolean newValue) {
	}

	public void setDoInput(boolean newValue) {
	}

	public void setDoOutput(boolean newValue) {
	}

	public void setIfModifiedSince(long newValue) {
	}

	public void setRequestProperty(String field, String newValue) {
	}

	public void setUseCaches(boolean newValue) {
	}

	public void setConnectTimeout(int timeout) {
	}

	public int getConnectTimeout() {
		return 0;
	}

	public void setReadTimeout(int timeout) {
	}

	public int getReadTimeout() {
		return 0;
	}

}

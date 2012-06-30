import java.io.IOException;
import java.io.InputStream;

public class HelloWorld {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println("Hello world!" + (5.0 * 3.3));
		InputStream is = null;
		try {
			is = (new Object()).getClass().getResourceAsStream("/show.txt");
			int read;
			while ((read = is.read()) >= 0) {
				System.out.write(read);
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (IOException e) {
				}
			}
		}
	}

}

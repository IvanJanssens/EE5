package resource;

import java.io.InputStream;

public class ResourceLoader {
	
	public static InputStream load(String path) {
		System.out.println(path);
		InputStream input = ClassLoader.getSystemClassLoader().getResourceAsStream(path);
		if(input == null) {
			input = ClassLoader.getSystemClassLoader().getResourceAsStream("/" + path);
		}
		return input;
	}
}
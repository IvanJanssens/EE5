package resource;

import java.io.InputStream;

public class ResourceLoader {
	
	public static InputStream load(String path) {
		System.out.println(path);
//		InputStream input = ResourceLoader.class.getResourceAsStream(path);
		InputStream input = ClassLoader.getSystemClassLoader().getResourceAsStream(path);
		if(input == null) {
//			input = ResourceLoader.class.getResourceAsStream("/" + path);
			input = ClassLoader.getSystemClassLoader().getResourceAsStream(path);
		}
		return input;
	}
}

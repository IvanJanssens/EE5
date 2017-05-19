package application;

import userinterface.UI;

public class decodeProtocol {

	public static void decode(byte message) {
		byte module = (byte) (message & 0xE0); //check first 3 bits
		if(module == 0xC0)
			decodeMM(message);
		else if(module == 0x40)
			Oscilloscope.updateAttenuationA(checkGain(message));
		else if(module == 0x80)
			Oscilloscope.updateAttenuationB(checkGain(message));
		else if(module == 0xE0)
			System.out.println("update offset");
	}
	
	private static void decodeMM(byte message) {
		
	}
	
	private static int checkGain(byte message) {
		int gain = UI.ATTENUATION;
		if(checkbit(message,0))
			gain = 2;
		else if(checkbit(message,1))
			gain = 5;
		else if(checkbit(message,2))
			gain = 10;
		return gain;
	}
	
	private static boolean checkbit(byte message, int pos) {
		return ((message << pos) & 1) != 0;
	}
}

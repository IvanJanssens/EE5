package application;



import java.io.InputStream;

import communication.Connection;
import javafx.application.Platform;
import javafx.concurrent.Service;
import javafx.concurrent.Task;
import userinterface.OscilloscopeUI;
import userinterface.UI;

public class Oscilloscope extends Service<Object>{

	Connection connection;
	public Oscilloscope(Connection connection) {
		this.connection = connection;
	}
	@Override
	protected Task<Object> createTask() {
		return new Task<Object>() {
			int result;
			@Override
			protected Object call() throws Exception {
				connection.send(UI.STOP); //Send 0 to stop current datastream from pic
				System.out.println("testing2");
				connection.send(UI.OSCILLOSCOPE); //Send 9 to start the multimeter datastream
				InputStream input = connection.getInputStream();
				int len = -1;
				result = 0;
				byte[] buffer = new byte[64];
				int[] intbuffer = new int[64];
				try {
					while(! isCancelled()) {
					if( (len = input.read(buffer)) > 0 ) {
		            		System.out.println(len);
		            		for(int i = 0; i<len; i++){
		            			intbuffer[i] = buffer[i] & 0xFF;
//		            			System.out.println("buffer" + i + " : "+ buffer[i]);
//		            			System.out.println("intbuffer" + i + " : "+ intbuffer[i]);
		            		}
		            		if(len>1) {
		            			result = (intbuffer[0] << 8) |  intbuffer[1];
		            			 Platform.runLater(new Runnable() {
				                    	@Override
				                    	public void run() {
				                    		try {
//				                    			MultimeterUI.updateMeter((5.11/65536)*result);
				                    			OscilloscopeUI.addData((5.11/65536)*result);
				                    		}
				                    		catch (Exception e) {
				                    			e.printStackTrace();
				                    		}
				                    	}
				                    });
		            		}
//		                    System.out.println("string" + new String(intbuffer,0,len));
//		                    System.out.println("result" + result);
//		                    System.out.println("volt" + (5.11/65536)*result);
		                   
		            	}
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
				return null;
			}
		};
	}
}

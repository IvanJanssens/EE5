package application;

import java.io.InputStream;

import communication.Connection;
import javafx.application.Platform;
import javafx.concurrent.Service;
import javafx.concurrent.Task;
import userinterface.MultimeterUI;
import userinterface.UI;

public class Multimeter extends Service<Object> {

	Connection connection;
	public Multimeter (Connection connection) {
		this.connection = connection;
	}


	@Override
	protected Task<Object> createTask() {
		return new Task<Object>() {
			double result;
			@Override
			protected Object call() throws Exception {

				InputStream input = connection.getInputStream();
				System.out.println("testing");
				System.out.println(input.available());
				connection.send(UI.MULTIMETER); //Send 9 to start the multimeter datastream
				int len = -1;
				result = 0;
				System.out.println(input.available());
				byte[] buffer = new byte[3];
				int[] intbuffer = new int[3];
				try {
					while(! isCancelled()) {
					if( (input.available()) > 2 ) {
							len = input.read(buffer,0,3);
//		            		System.out.println(len);
		            		for(int i = 0; i<len; i++){
		            			intbuffer[i] = buffer[i] & 0xFF;
		            			System.out.println("bufferTest" + i + " : "+ buffer[i]);
		            			System.out.println("intbuffer" + i + " : "+ intbuffer[i]);
		            		}
	            			result = (double)(((intbuffer[1] << 2) |  (intbuffer[2] >> 6)) * (5.11/1024));
	            			System.out.println(result);
            				Platform.runLater(new Runnable() {
			                    	@Override
			                    	public void run() {
			                    		try {
			                    			MultimeterUI.updateMeter(result);
			                    		}
			                    		catch (Exception e) {
			                    			e.printStackTrace();
			                    		}
			                    	}
			                    });
//		                    System.out.println("string" + new String(intbuffer,0,len));
//		                    System.out.println("result" + result);
//		                    System.out.println("volt" + (5.11/65536)*result);
		                   
		            	}
					}
					if(isCancelled()) {
						connection.send(UI.STOP);
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
				return null;
			}
		};
	}
}

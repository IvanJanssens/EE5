package application;

import java.io.InputStream;

import communication.Connection;
import javafx.application.Platform;
import javafx.concurrent.Service;
import javafx.concurrent.Task;
import userinterface.MultimeterUI;
import userinterface.UI;

public class Multimeter extends Service<Object> {


	public Multimeter () {
	}


	@Override
	protected Task<Object> createTask() {
		return new Task<Object>() {
			double result;
			@Override
			protected Object call() throws Exception {

				InputStream input = Connection.getInputStream();
				Connection.send(UI.MULTIMETER); //Send UI.MULTIMETER to start the multimeter datastream
				int len = -1;
				result = 0;
				byte[] buffer = new byte[3];
				int[] intbuffer = new int[3];
				try {
					while(! isCancelled()) {
						if( (input.available()) > 2 ) {
							len = input.read(buffer,0,3);
		            		for(int i = 0; i<len; i++){
		            			intbuffer[i] = buffer[i] & 0xFF;
		            		}
	            			result = (double)(((intbuffer[1] << 2) |  (intbuffer[2] >> 6)) * (5.11/1024));
	            			
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

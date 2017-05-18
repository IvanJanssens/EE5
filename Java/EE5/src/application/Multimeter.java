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
				Connection.send(UI.MULTIMETERON); //Send UI.MULTIMETER to start the multimeter datastream
				int len = -1;
				result = 0;
				byte[] buffer = new byte[2];
				int[] intbuffer = new int[2];
				try {
					while(! isCancelled()) {
						if( (input.available()) > 1 ) {
							len = input.read(buffer,0,2);
							if((buffer[0] & 0xE0) == 0xE0) {
							
			            		for(int i = 0; i<len; i++){
			            			intbuffer[i] = buffer[i] & 0x1F;
			            		}
		            			result = (double)(((intbuffer[0] << 2) |  (intbuffer[1] >> 6)) * (5.11/1024));
		            			
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
					}
					if(isCancelled()) {
						Connection.send(UI.MULTIMETEROFF);
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
				return null;
			}
		};
	}
}

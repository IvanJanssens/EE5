package application;

import java.io.InputStream;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.TimeUnit;

import communication.Connection;
import javafx.application.Platform;
import javafx.concurrent.Service;
import javafx.concurrent.Task;
import userinterface.MultimeterUI;
import userinterface.UI;

public class Multimeter extends Service<Object> {

	private static Queue<Double> databuffer;
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
				databuffer = new LinkedList<Double>();
				Timer timer = new Timer();
				timer.scheduleAtFixedRate(new TimerTask() {
					  @Override
					  public void run() {
					    Connection.send(UI.MULTIMETERON);
					  }
					},(long)0, (long)100);
				try {
					while(! isCancelled()) {
						if( (input.available()) > 1 ) {
							len = input.read(buffer,0,2);
							if((buffer[0] & 0xE0) == 0xE0) {
							
			            		for(int i = 0; i<len; i++){
			            			intbuffer[i] = buffer[i] & 0x1F;
			            		}
			            		result = (double) (((((intbuffer[0] *32)+  intbuffer[1] )*3.3)/1024));
		            			databuffer.add(result);
	            				Platform.runLater(new Runnable() {
				                    	@Override
				                    	public void run() {
				                    		try {
				                    			double data = databuffer.poll();
			                    				MultimeterUI.updateMeter(result);
				                    		}
				                    		catch (Exception e) {
				                    			e.printStackTrace();
				                    		}
				                    	}
				                    });
							}
							else if (buffer[0] == -1 && buffer[1] == 0x00) {
								Connection.checkParam();
							}
		                   
		            	}
					}
					if(isCancelled()) {
						timer.cancel();
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

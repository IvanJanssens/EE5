package application;



import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.InputStream;
import java.util.LinkedList;
import java.util.Locale;
import java.util.Queue;
import java.util.logging.Level;

import communication.Connection;
import javafx.application.Platform;
import javafx.concurrent.Service;
import javafx.concurrent.Task;
import userinterface.GraphUI;
import userinterface.UI;

public class Oscilloscope extends Service<Object>{

	Connection connection;
	File tempFile;
	static double trigger = UI.TRIGGER;
	static int max_data = UI.MAX_DATA;
	static int attenuation = UI.ATTENUATION;
	public Oscilloscope(Connection connection, File tempFile) {
		this.connection = connection;
		this.tempFile = tempFile;
	}
	
	public static void updateTrigger(double newValue) {
		trigger = newValue;
	}
	
	public static void updateTimeDiv(int newValue) {
		max_data = newValue;
	}
	
	public static void updateAttenuation(int newValue) {
		attenuation = newValue;
	}
	
	@Override
	protected Task<Object> createTask() {
		return new Task<Object>() {
			double result, prevValue;
			int currentDataPoint;
			Queue<Double> databuffer;
			BufferedWriter temp;
			@Override
			protected Object call() {
				InputStream input = connection.getInputStream();
				
				connection.send(UI.OSCILLOSCOPE); //Send UI.OSCILLOSCOPE to start the oscilloscope datastream
				int len = -1;
				result = 0;
				byte[] buffer = new byte[64];
				int[] intbuffer = new int[64];
				databuffer = new LinkedList<Double>();
				currentDataPoint = 0;
				prevValue = 0;
				try {
					temp = new BufferedWriter(new FileWriter(tempFile,true));
					temp.write("New mesurement \n");
					while(! isCancelled()) {
						if(input.available() > 2 ) {
							len = input.read(buffer, 0, 3);
		            		for(int i = 0; i<len; i++){
		            			intbuffer[i] = buffer[i] & 0xFF;
		            		}
		            		if(len>1) {
		            			result = (double) (((intbuffer[1] << 2) |  (intbuffer[2] >> 6))*(5.11/1024)*attenuation);
		            			temp.write((String.format(Locale.US,"%.3f", result) + "\n"));
		            			
		            			databuffer.add(result);
	            				Platform.runLater(new Runnable() {
			                    	@Override
			                    	public void run() {
			                    		try {
			                    			double newDataPoint = databuffer.poll();
			                    			if(currentDataPoint >= max_data && newDataPoint > (double)trigger && prevValue <= (double)trigger) {
			                    				currentDataPoint = 0;
			                    			}
			                    			if(currentDataPoint <= max_data)
			                    				GraphUI.addData(newDataPoint,0,max_data,currentDataPoint,GraphUI.OsciUI);
			                    			prevValue = newDataPoint;
			                    			currentDataPoint++;
			                    		}
			                    		catch (Exception ex) {
			                				Main.LOGGER.log(Level.SEVERE,"Couldn't add new datapoint to graph",ex);
			                    		}
			                    	}
			                    });
		            			
		            			 
		            		}
		                   
		            	}
					}
					if(isCancelled()) {
						connection.send(UI.STOP);
						temp.flush();
						temp.close();
					}
				} catch (Exception ex) {
					Main.LOGGER.log(Level.SEVERE,"Couldn't write to temp file",ex);
				}
				return null;
			}
			
			
		};
		
	}
}

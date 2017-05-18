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
import userinterface.OscilloscopeUI;
import userinterface.UI;

public class Oscilloscope extends Service<Object>{

	File tempFile;
	static double trigger = UI.TRIGGER;
	static int max_data = UI.MAX_DATA;
	static int attenuationA = UI.ATTENUATION;
	static int attenuationB = UI.ATTENUATION;
	static String triggerSource = "A";
	public Oscilloscope(File tempFile) {
		this.tempFile = tempFile;
	}
	
	public static void updateTrigger(double newValue) {
		trigger = newValue;
	}
	
	public static void updateTimeDiv(int newValue) {
		max_data = newValue;
	}
	
	public static void updateAttenuationA(int newValue) {
		attenuationA = newValue;
	}
	
	public static void updateAttenuationB(int newValue) {
		attenuationB = newValue;
	}
	
	public static void updateTriggerSource(String newValue) {
		triggerSource = newValue;
	}
	
	@Override
	protected Task<Object> createTask() {
		return new Task<Object>() {
			double result, prevValue;
			int currentDataPoint;
			Queue<Double> databufferA;
			Queue<Double> databufferB;
			BufferedWriter temp;
			@Override
			protected Object call() {
				InputStream input = Connection.getInputStream();
				
				OscilloscopeUI.sendOsciParam(); //Send UI.OSCILLOSCOPE to start the oscilloscope datastream
				int len = -1;
				result = 0;
				byte[] buffer = new byte[2];
				int[] intbuffer = new int[2];
				databufferA = new LinkedList<Double>();
				databufferB = new LinkedList<Double>();
				currentDataPoint = 0;
				prevValue = 0;
				try {
					temp = new BufferedWriter(new FileWriter(tempFile,true));
					temp.write("New mesurement");
					while(!isCancelled()) {
						if(input.available() > 1 ) {
							len = input.read(buffer, 0, 2);
							if(((buffer[0] & 0x20) == 0x00) && (buffer[1] & 0x20) == 0x01) {
			            		for(int i = 0; i<len; i++){
			            			intbuffer[i] = buffer[i] & 0x1F;
			            		}

			            		
			            		if((buffer[1] & 0xE0) == 0x60) { //Oscilloscope A
			            			result = (double) (((intbuffer[0] << 2) |  (intbuffer[1] >> 6))*(3.3/1024)/attenuationA);
			            			temp.write("\n" + (String.format(Locale.US,"%.3f", result)));
			            			databufferA.add(result);
			            			
		            				Platform.runLater(new Runnable() {
				                    	@Override
				                    	public void run() {
				                    		try {
				                    			double newDataPoint = databufferA.poll();
				                    			if(triggerSource.equals("A")) {
					                    			if(currentDataPoint >= max_data && newDataPoint > (double)trigger && prevValue <= (double)trigger)
					                    				currentDataPoint = 0;
					                    			prevValue = newDataPoint;
					                    		}
				                    			if(currentDataPoint <= max_data)
				                    				GraphUI.addDataA(newDataPoint,0,max_data,currentDataPoint,GraphUI.OsciUI);
				                    			currentDataPoint++;
				                    		}
				                    		catch (Exception ex) {
				                				Main.LOGGER.log(Level.WARNING,"Couldn't add new datapoint to graph",ex);
				                    		}
				                    	}
				                    });
			            		}
			            		else if((buffer[1] & 0xE0) == 0xA0) { //Oscilloscope B
			            			result = (double) (((intbuffer[0] << 2) |  (intbuffer[1] >> 6))*(3.3/1024)/attenuationB);
			            			temp.write( " - " + (String.format(Locale.US, "%.3f",result)));
			            			databufferB.add(result);
			            			
			            			Platform.runLater(new Runnable() {
			            				@Override
			            				public void run() {
			            					try {
			            						double newDataPoint = databufferB.poll();
			            						if(triggerSource.equals("B")) {
				            						if(currentDataPoint >= max_data && newDataPoint > (double)trigger && prevValue <= (double)trigger)
					                    				currentDataPoint = 0;
				            						prevValue = newDataPoint;
				                    			}
				                    			if(currentDataPoint <= max_data)
				                    				GraphUI.addDataB(newDataPoint,0,max_data,currentDataPoint,GraphUI.OsciUI);
//				                    			currentDataPoint++;
				                    		}
				                    		catch (Exception ex) {
				                				Main.LOGGER.log(Level.WARNING,"Couldn't add new datapoint to graph",ex);
				                    		}
			            				}
			            			});
			            		}
		            			
		            			 
		            		}
							else if (buffer[0] == 0x00 && buffer[1] == 0xFF) {
								Connection.checkParam();
							}
							else {
								while(input.available() > 1) {
									input.read(buffer, 0, 1);
									if((buffer[0] & 0x20) == 0x01)
										break;
								}
							}
		                   
		            	}
					}
					if(isCancelled()) {
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

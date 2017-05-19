package application;




import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.LinkedList;
import java.util.Queue;
import java.util.logging.Level;


import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

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
	static int attenuationA = UI.ATTENUATION, attenuationB = UI.ATTENUATION;
	static String triggerSource = "A";
	private static XSSFWorkbook workbook;
	private static boolean AOn, BOn;
	private static Queue<Double> databufferA, databufferB;
	private static double result, prevValue;
	private static int currentDataPoint;
	
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
	
	public static void AOn(boolean on) {
		AOn = on;
		if(!on) {
			databufferA.clear();
		}
		restartValues();
	}
	
	public static void BOn(boolean on) {
		BOn = on;
		if(!on) {
			databufferB.clear();
		}
		restartValues();
	}
	
	private static void restartValues() {

		if(!BOn & !AOn) {
			currentDataPoint = 0;
		}
	}
	
	public static void save(File file) {
		FileOutputStream fileOut;
		try {
//			fileOut = new FileOutputStream(new File(System.getProperty("user.home") + "/Desktop/EE5.xlsx"),true);
			fileOut = new FileOutputStream(file);
			workbook.write(fileOut);
			fileOut.close();
		} catch (Exception ex) {
			Main.LOGGER.log(Level.SEVERE,"Couldn't save to excel file",ex);
		}
	}
	
	public static void autoSave() {
		FileOutputStream fileOut;
		try {
			File file = new File(System.getProperty("user.home") + "/Desktop/EE5.xlsx");
			// file.delete();
			// file.createNewFile();
			for (int i = 1; file.exists(); i++) {
			    file = new File(System.getProperty("user.home") + String.format("/Desktop/EE5-%d.xlsx", i));
			}
			fileOut = new FileOutputStream(file,true);
			workbook.write(fileOut);
			fileOut.close();
		} catch (Exception ex) {
			Main.LOGGER.log(Level.SEVERE,"Couldn't save to excel file",ex);
		}
	}
	@Override
	protected Task<Object> createTask() {
		return new Task<Object>() {
			
//			BufferedWriter temp;
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
				int dataReceived = 0;
				try {
					workbook = new XSSFWorkbook();
					Sheet sheet = workbook.createSheet("EE5 Measurements");
					
					Row row = sheet.createRow((short)0);
					
					row.createCell(0).setCellValue("Data Oscilloscope A");
					row.createCell(1).setCellValue("Data Oscilloscope B");

					Row dataRow = sheet.createRow(sheet.getLastRowNum() + 1);
					
//					temp = new BufferedWriter(new FileWriter(tempFile,true));
//					temp.write("New mesurement");
					while(!isCancelled()) {
						if(input.available() > 1 ) {
							
							len = input.read(buffer, 0, 2);
							dataReceived += len;
//							System.out.println("buffer0 :" + (buffer[0]& 0x20) + " buffer 1 " + (buffer[1] & 0x20));
//							System.out.println("buffer0: "+ Integer.toBinaryString(buffer[0]) +" and " + (buffer[0] & 0x20) +" and "+ Integer.toBinaryString(buffer[0] & 0x20) + " and "+ (buffer[0] == 0xFF) + " and " + 0xFF + " and "+ buffer[0]);
//							System.out.println("buffer1: "+ Integer.toBinaryString(buffer[1]) + " and " +(buffer[1] & 0x20) + " and "+ Integer.toBinaryString(buffer[1] & 0x20)+ " and "+ (buffer[1] == 0x00));
							if(((buffer[0] & 0x20) == 0x00) && (buffer[1] & 0x20) == 0x20) {
			            		for(int i = 0; i<len; i++){
			            			intbuffer[i] = buffer[i] & 0x1F;
			            		}

			            		System.out.println("data");
			            		System.out.println("Aon: " + AOn);
			            		System.out.println("bufferdata: "+((buffer[1] & 0xE0) == 0x60));
			            		if(AOn & (buffer[1] & 0xE0) == 0x60) { //Oscilloscope A
			            			result = (double) ((((((intbuffer[0] *32)+  intbuffer[1] )*3.3)/1024))*attenuationA);
			            			if(dataRow.getCell(0) != null)
			            				dataRow = sheet.createRow(sheet.getLastRowNum() + 1);
			            			dataRow.createCell(0).setCellValue(result);
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
			            		else if(BOn & (buffer[1] & 0xE0) == 0xA0) { //Oscilloscope B
			            			result = (double) ((((((intbuffer[0]*32) + intbuffer[1]) *3.3)/1024))*attenuationB);
			            			if(dataRow.getCell(0) != null)
			            				dataRow = sheet.createRow(sheet.getLastRowNum() + 1);
			            			dataRow.createCell(1).setCellValue(result);
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
				                    			if(!AOn)
				                    				currentDataPoint++;
				                    		}
				                    		catch (Exception ex) {
				                				Main.LOGGER.log(Level.WARNING,"Couldn't add new datapoint to graph",ex);
				                    		}
			            				}
			            			});
			            		}
			            		
		            			
		            			 
		            		}
							
							else if (buffer[0] == -1 && buffer[1] == 0x00) {
									Connection.checkParam();
							}
//							else
//								break;
//							else {
//								while(input.available() > 1) {
//									input.read(buffer, 0, 1);
//									if((buffer[0] & 0x20) == 0x01)
//										break;
//								}
//							}
		            	}
					}
					if(isCancelled()) {

//						temp.flush();
//						temp.close();
					}
				} catch (Exception ex) {
					Main.LOGGER.log(Level.SEVERE,"Couldn't write to Excel file",ex);
				}
				return null;
			}
			
			
		};
		
	}
}

package application;



import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import communication.Connection;
import javafx.application.Platform;
import javafx.concurrent.Service;
import javafx.concurrent.Task;
import userinterface.OscilloscopeUI;
import userinterface.UI;

public class Oscilloscope extends Service<Object>{

	Connection connection;
	File tempFile;
	double trigger = 0;
	double max_data = 10;
	public Oscilloscope(Connection connection, File tempFile) {
		this.connection = connection;
		this.tempFile = tempFile;
	}
	
	public void updateTrigger(double newValue) {
		trigger = newValue;
	}
	
	public void updateTimeDiv(double newValue) {
		max_data = newValue;
	}
	
	@Override
	protected Task<Object> createTask() {
		return new Task<Object>() {
			int result;
			float max;
			BufferedWriter temp;
			List<Double> dataPoints;
			@Override
			protected Object call() throws Exception {
				InputStream input = connection.getInputStream();
				
				System.out.println("testing2");
				connection.send(UI.OSCILLOSCOPE); //Send 9 to start the multimeter datastream
				int len = -1;
				result = 0;
				byte[] buffer = new byte[64];
				int[] intbuffer = new int[64];
				float[] fftbuffer = new float[128];
				int j = 0;
				double prevData = 0;
				dataPoints = new ArrayList<Double>();
				try {
					temp = new BufferedWriter(new FileWriter(tempFile));
					while(! isCancelled()) {
//					System.out.println(tempFile.getName());
					if(input.available() > 2 ) {
							len = input.read(buffer, 0, 3);
//		            		System.out.println(len);
		            		for(int i = 0; i<len; i++){
		            			intbuffer[i] = buffer[i] & 0xFF;
//		            			System.out.println("bufferOsci" + i + " : "+ buffer[i]);
//		            			System.out.println("intbuffer" + i + " : "+ intbuffer[i]);
		            		}
		            		if(len>1) {
		            			result = (intbuffer[1] << 2) |  (intbuffer[2] >> 6);
		            			temp.write(""+result);
		            			temp.newLine();
//		            			fftbuffer[j] = (float)result;
//		            			if(j == 127) {
//		            				float fftresult[] = fftbuffer;
//		            				float sampleI[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
////		            				System.out.println("test1");
////		            				for(int k= 0; k< fftresult.length; k++) {
////			            				System.out.println(fftresult[k]);
////			            			}
//		            				Fourrier.transform(1, 128, fftresult,sampleI);
////			            			System.out.println("test");
//		            				float mean = 0;
//		            				float fftmax = 0;
//		            				int fftindex = 0;
//		            				for(int k = 1; k<fftresult.length;k++) {
//		            					mean += fftresult[k];
//		            					if(fftresult[k] > fftmax) {
//		            						fftmax = fftresult[k];
//		            						fftindex = k;
//		            					}
//		            				}
//		            				if(fftmax < fftresult[0]) {
//		            					fftmax = fftresult[0];
//		            				}
//		            				mean = mean/fftresult.length;
//			            			float fftzeropad [] = new float[512];
//			            			
//			            			for(int k = 0; k<512; k++) {
//			            				fftzeropad[k] = 0;
//			            			}
//			            			fftzeropad[0] = fftresult[0]/fftmax;
//			            			fftzeropad[fftindex] = fftresult[fftindex]/fftmax;
//			            			fftzeropad[512-fftindex] = fftresult[128-fftindex]/fftmax;
//			            			System.out.println("fftzero");
//			            			for(int k = 0; k<fftzeropad.length; k++) {
//			            				System.out.println(fftzeropad[k]);
//			            			}
//			            			float fftim [] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//
//			            			Fourrier.transform(1, fftzeropad.length, fftzeropad, fftim);
//			            			max = 0;
//			            			System.out.println("test");
//			            			for(int k = 0; k<fftzeropad.length; k++){
//			            				System.out.println(fftzeropad[k]);
//			            				if(fftzeropad[k] > max) {
//			            					max = fftzeropad [k];
//			            				}
//			            			}
////		            				for(int k= 0; k< fftresult.length; k++) {
////			            				System.out.println(fftresult[k] + "imaginary " + sampleI[k]);
////			            			}
//		            				Platform.runLater(new Runnable() {
//		            					@Override
//		            					public void run() {
//		            						try {
//		    	                    			OscilloscopeUI.addFftData(fftzeropad, max);
//		            						} catch (Exception e){
//		            							e.printStackTrace();
//		            						}
//		            					}
//		            				});
//		            				j = -1;
//		            			}
//		            			j++;
		            			
		            			
//		            			float samples[] = {0,0.38268352f,0.70710677f,0.9238796f,1,0.9238795f,0.70710677f,0.38268343f,0,-0.38268352f,-0.70710677f,-0.9238796f,-1.0f,-0.9238795f,-0.70710677f,-0.38268343f};
//		            			float samples[] = {0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
//		            			float samplesi[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//		            			float samplesi[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//		            			for(int i= 0; i< samples.length; i++) {
//		            				System.out.println(samples[i]);
//		            			}
//		            			System.out.println("test");
//		            			Fourrier.transform(1, 32, samples, samplesi);
		            //
//		            			for(int i= 0; i< samples.length; i++) {
//		            				System.out.println(samples[i]);
//		            			}
		            			
		            			double data = ((5.11/1024)*result);
		            			System.out.println(data);
		            			System.out.println(j);
		            			System.out.println(trigger);
		            			System.out.println(prevData);
		            			System.out.println(j >= max_data);
		            			System.out.println(data > trigger);
		            			System.out.println(prevData <= trigger);
		            			if(j >= max_data && data > trigger && prevData <= trigger) {
		            				j = 0;
		            			}
		            			if(j < max_data) {
		            				dataPoints.add(data);
		            			}
		            			System.out.println(j);
		            			System.out.println(max_data-1);
		            			System.out.println(j == (max_data-1));
		            			if(j == (max_data-1)) {
		            				Platform.runLater(new Runnable() {
				                    	@Override
				                    	public void run() {
				                    		try {
//				                    			MultimeterUI.updateMeter((5.11/1024)*result);
				                    			OscilloscopeUI.addData(dataPoints);
				                    		}
				                    		catch (Exception e) {
				                    			e.printStackTrace();
				                    		}
				                    	}
				                    });
		            			}
		            			prevData = data;
		            			j++;
		            			
		            			 
		            		}
//		                    System.out.println("string" + new String(intbuffer,0,len));
//		                    System.out.println("result" + result);
//		                    System.out.println("volt" + (5.11/65536)*result);
		                   
		            	}
					}
					if(isCancelled()) {
						connection.send(UI.STOP);
						temp.flush();
						temp.close();
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
				return null;
			}
			
			
		};
		
	}
}
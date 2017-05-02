package communication;


import java.io.OutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import javafx.application.Platform;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import userinterface.UI;

public class Connection {
	
	private CommPortIdentifier commPortIdentifier;
	private CommPort port;
	private SerialPort serialPort;
	private OutputStream output;
	private InputStream input;

	//Connection object
	public Connection(CommPortIdentifier commPortIdentifier) {
		super();
		this.commPortIdentifier = commPortIdentifier;
	}
	
	//get a list of all the comm-ports available on this PC
	public static ObservableList<CommPortIdentifier> refresh() {
		ObservableList<CommPortIdentifier> choices = FXCollections.observableArrayList();
		
		    Enumeration<CommPortIdentifier> portList = CommPortIdentifier.getPortIdentifiers();

		    //add the different ports of the enumeration to the ObservableList (needed for combobox)
		    while (portList.hasMoreElements()) {
		      CommPortIdentifier cpi = (CommPortIdentifier) portList.nextElement();
		      choices.add(cpi);
		    }
	    return choices;
	}
	
	//start up a new connection
	public int load() throws Exception {
		//check if COM-port is used by another program
		if(this.commPortIdentifier.isCurrentlyOwned()) {
			System.out.println("Error: Port is currently in use");
			return 1;
		}
		else {
			//open COM-port
			port = this.commPortIdentifier.open(this.getClass().getName(),2000);
			
			if(port instanceof SerialPort) {
				this.serialPort = (SerialPort) port;
                serialPort.setSerialPortParams(57600,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);

		        this.input = serialPort.getInputStream();
		        this.output = serialPort.getOutputStream();
//                
//                reader = new Thread(new SerialReader(this.input));
//        		reader.start();
//                (new Thread(new SerialWriter(out))).start();

                return 0;
			}
			//else {
				
			//}
		}
		return 1;
	}
	
	public InputStream getInputStream() {
		return this.input;
	}
	
	//read out the input buffer of the COM port
	 public static class SerialReader implements Runnable 
	    {
	        InputStream in;
	        int result;
	        public SerialReader ( InputStream in ) {
	            this.in = in;
	            result = 0;
	        }
	        public void run () {
		            byte[] buffer = new byte[64];
		            int[] intbuffer = new int[64];
		            int len = -1;
		            try {

			        	while(!Thread.interrupted()){
			                if ( ( len = in.read(buffer)) == -1 ){
			                	break;
			                } else {
			                	if(len> 0) {
			                		System.out.println(len);
			                		for(int i = 0; i<len; i++){
			                			intbuffer[i] = buffer[i] & 0xFF;
			                			System.out.println("buffer" + i + " : "+ buffer[i]);
			                			System.out.println("intbuffer" + i + " : "+ intbuffer[i]);
			                		}
			                		if(len>1) {
			                			result = (intbuffer[0] << 8) |  intbuffer[1];
			                			 Platform.runLater(new Runnable() {
						                    	@Override
						                    	public void run() {
						                    		try {
//						                    			UI.updateMeter((5.11/65536)*result);
//						                    			UI.addData((5.11/65536)*result);
						                    		}
						                    		catch (Exception e) {
						                    			e.printStackTrace();
						                    		}
						                    	}
						                    });
			                		}
				                    System.out.println("string" + new String(intbuffer,0,len));
				                    System.out.println("result" + result);
				                    System.out.println("volt" + (5.11/65536)*result);
				                   
			                	}
			                }
			        	}
		            }
		            catch ( IOException e ) {
		                e.printStackTrace();
		            }  
	        }
	    }
	
	 //send an int to the port
	public void send(int message) {
		System.out.println(message);
		try {
			this.output.write(message);
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void clearBuffer() {
		try {
			while((input.read())!= UI.STOP) {
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void close() {
		if(this.serialPort!=null){
//			reader.interrupt();
			try {
				this.input.close();
				this.output.close();
			}
			catch (IOException e){
				e.printStackTrace();
			}
	        this.port.close(); //close serial port
	    }
	}
}

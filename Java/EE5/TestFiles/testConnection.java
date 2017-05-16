package TestFiles;


import java.io.OutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.Scanner;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

public class testConnection {
	
	private CommPortIdentifier commPortIdentifier;
	private static CommPort port;
	private static SerialPort serialPort;
	private static OutputStream output;
	private static InputStream input;

	//Connection object
	public testConnection(CommPortIdentifier commPortIdentifier) {
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
                serialPort.setSerialPortParams(9600,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);

		        this.input = serialPort.getInputStream();
		        this.output = serialPort.getOutputStream();
//                
                Thread reader = new Thread(new SerialReader(this.input));
        		reader.start();
        		Thread writer = new Thread(new Writer());
        		writer.start();

                return 0;
			}
			else {
				return 1;
			}
		}
	}
	
	public InputStream getInputStream() {
		return this.input;
	}
	
	public static class Writer implements Runnable {
		public void run() {
			while(!Thread.interrupted()) {
				Scanner input = new Scanner(System.in);
				byte message = (byte) input.nextInt();
				System.out.println("Send: " + (message & 0xFF));
				if(message == 0) {
					close();
					Runtime.getRuntime().halt(0);
				}
				send(message);
			}
		}
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
		            int len = -1;
		            int var = 0;
		            try {

			        	while(!Thread.interrupted()){
			                if ( ( len = in.read(buffer)) == -1 ){
			                	break;
			                } else {
			                	if(len> 0) {
//			                		System.out.println(len);
			                		for(int i = 0; i<len; i++){
			                			if((buffer[i] & 0x20) == 0x00) {
			                				var =  (buffer[i] & 0x1F)*32;
			                				//System.out.println("var:" + var);
			                			}
			                			else if((buffer[i] & 0xE0) == 0xE0) System.out.println("MM: " + (var + (buffer[i] & 0x1F))*3.3/(1024));
			                			else if((buffer[i] & 0xE0) == 0x60) System.out.println("A: " + (var + (buffer[i] & 0x1F))*3.3/(1024));
			                			else if((buffer[i] & 0xE0) == 0xA0) System.out.println("B: " + (var + (buffer[i] & 0x1F))*3.3/(1024));
			                		}
				                   
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
	public static void send(byte message) {
		try {
//			for(int i = 0 ; i < message.length; i++) {
//				System.out.println("send: " + (char) message[i] + " => " + message[i]);
//				output.write(message[i]);
//			}
//			output.write(message);
			//byte[] test = {(byte) 0b00101010, (byte) 0b10101010 };
			output.write(message);
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	static void close() {
		if(serialPort!=null){
			try {
				input.close();
				output.close();
			}
			catch (IOException e){
				e.printStackTrace();
			}
			serialPort.removeEventListener();
			serialPort.close();
	        port.close(); //close serial port
	        System.out.println("closed");
	    }
	}
}

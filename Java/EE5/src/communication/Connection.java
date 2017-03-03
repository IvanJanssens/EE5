package communication;


import java.io.OutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

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
//                (new Thread(new SerialReader(this.input))).start();
//                (new Thread(new SerialWriter(out))).start();

                return 0;
			}
			else {
				return 1;
			}
		}
	}
	
	//read out the input buffer of the COM port
	 public static class SerialReader implements Runnable 
	    {
	        InputStream in;
	        public SerialReader ( InputStream in ) {
	            this.in = in;
	        }
	        public void run () {
	            byte[] buffer = new byte[1024];
	            int len = -1;
	            try {
	                while ( ( len = in.read(buffer)) > -1 ){
//	                	System.out.println("available" + in.available());
//	                	System.out.println("new string" + new String(buffer,0,len));
	                	if(in.available()>0) {
	                		System.out.println(buffer.length);
		                    System.out.println(new String(buffer,0,len));
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
		try {
			this.output.write(message);
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void close() {
		if(this.serialPort!=null){
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

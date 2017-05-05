package communication;


import java.io.OutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Enumeration;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
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
		      CommPortIdentifier cpi = portList.nextElement();
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
	
	 
	 
	 //send an int to the port
	public void send(byte[] message) {
		try {
			for(int i = 0 ; i < message.length; i++) {
				System.out.println(message[i]);
				output.write(message[i]);
			}
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void clearBuffer() {
		try {
//			byte[] buffer = new byte[3];
			byte[] buffer = new byte[1];
			while(!Arrays.equals(buffer, UI.STOP)) {
//				if(input.available() > 2) {
//					input.read(buffer, 0, 3);
//				}
				input.read(buffer);
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void close() {
		if(serialPort!=null){
			try {
				input.close();
				output.close();
			}
			catch (IOException e){
				e.printStackTrace();
			}
	        port.close(); //close serial port
	    }
	}
}

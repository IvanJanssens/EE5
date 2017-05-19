package communication;


import java.io.OutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.LinkedList;
import java.util.logging.Level;

import application.Main;
//import application.decodeProtocol;
import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import userinterface.UI;

public class Connection {
	
	private CommPortIdentifier commPortIdentifier;
	private static CommPort port;
	private static SerialPort serialPort;
	private static OutputStream output;
	private static InputStream input;
	private static LinkedList<Byte> dataBuffer;

	private static Connection connection = null; // Make singleton
	//Connection object
	private Connection() {
	}
	
	public static Connection getInstance() {
		if(connection == null){ 
			connection = new Connection();
		}
		return connection;
	}
	
	public void initConnection(CommPortIdentifier commPortIdentifier) {
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
			try {
				port = this.commPortIdentifier.open(this.getClass().getName(),2000);
			} catch (Exception e) {
				Main.LOGGER.log(Level.SEVERE,"couldn't start program",e);
			}
			
			if(port instanceof SerialPort) {
				this.serialPort = (SerialPort) port;
                serialPort.setSerialPortParams(9600,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);

		        input = serialPort.getInputStream();
		        Connection.output = serialPort.getOutputStream();
		        dataBuffer = new LinkedList<Byte>();
                return 0;
			}
			else {
				return 1;
			}
		}
	}
	
	public static InputStream getInputStream() {
		return input;
	}
	
	 
	 
	 //send an int to the port
	public static void send(byte message) {
		try {
				System.out.println("--------- Send --------");
				System.out.println(Integer.toBinaryString(message));
				output.write(message);
				dataBuffer.add(message);
		}
		catch (IOException ex) {
			Main.LOGGER.log(Level.SEVERE,"Couldn't send data",ex);
		}
	}
	
	public static void checkParam() {
		try {
			byte[] buffer = new byte[1];
			input.read(buffer);
			byte prevMessage = dataBuffer.poll();
			if(buffer[0] != prevMessage)
				send(prevMessage);
			else {
				//decodeProtocol.decode(prevMessage);
			}
		}
		catch (IOException ex) {
			Main.LOGGER.log(Level.SEVERE,"Couldn't receive data",ex);
		}
	}
	
	public static void close() {
		if(serialPort!=null){
			try {
				input.close();
				output.close();
			}
			catch (IOException e){
				e.printStackTrace();
			}
			serialPort.close();
	        port.close(); //close serial port
	    }
	}
}

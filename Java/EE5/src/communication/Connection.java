package communication;


import java.io.OutputStream;
import java.io.InputStream;
import java.util.Enumeration;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

public class Connection {
	
	private CommPortIdentifier commPortIdentifier;
	private String portName;
	private SerialPort port;
	private OutputStream output;
	private InputStream input;

	public Connection(CommPortIdentifier commPortIdentifier) {
		super();
		this.commPortIdentifier = commPortIdentifier;
	}
	
	public static ObservableList<CommPortIdentifier> refresh() {
		ObservableList<CommPortIdentifier> choices = FXCollections.observableArrayList();
		
		    // get list of ports available on this particular computer,
		    // by calling static method in CommPortIdentifier.
		    Enumeration<CommPortIdentifier> portList = CommPortIdentifier.getPortIdentifiers();

		    // Process the list
		    while (portList.hasMoreElements()) {
		      CommPortIdentifier cpi = (CommPortIdentifier) portList.nextElement();
		      choices.add(cpi);
		    }
	    return choices;
	}
	
	public int load() throws Exception {
		System.out.println(this.commPortIdentifier);
		if(this.commPortIdentifier.isCurrentlyOwned()) {
			System.out.println("Error: Port is currently in use");
			return 1;
		}
		else {
			CommPort port = this.commPortIdentifier.open("PicOscilloscope",2000);
			
			if(port instanceof SerialPort) {
				SerialPort serialPort = (SerialPort) port;
                serialPort.setSerialPortParams(57600,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
                
                return 0;
//                InputStream in = serialPort.getInputStream();
//                OutputStream out = serialPort.getOutputStream();
//                
//                (new Thread(new SerialReader(in))).start();
//                (new Thread(new SerialWriter(out))).start();
			}
			else {
				return 1;
			}
		}
	}
	
	public void close() {
		
	}
}

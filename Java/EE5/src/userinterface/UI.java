package userinterface;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.scene.Scene;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.image.Image;
import javafx.stage.Stage;
import resource.ResourceLoader;

import java.io.File;
import java.io.IOException;
import java.util.logging.Level;

import application.Main;
import application.Multimeter;
import application.Oscilloscope;
import communication.Connection;
import userinterface.MultimeterUI;
import userinterface.OscilloscopeUI;
import userinterface.GeneratorUI;

public class UI {

	public static Connection connection; // global connection variable
	private static Oscilloscope oscilloscope;
	private static Multimeter multimeter;
	// messages to be send to the PIC
	public static final byte MULTIMETER = (byte) 0b11000000;
	//max data shown on the oscilloscope graph
	public static final int MAX_DATA = 500;
	public static final double TRIGGER = 1.0;
	public static final int ATTENUATION = 2;
	public static File tempFile;
	
	// start the main UI
	public static void start(Stage stage, Connection conn) {
		connection = conn;
		Stage mainStage = stage;
		try {
			tempFile = File.createTempFile("Temp_EE5_Data", ".txt");
			System.out.println(tempFile.getAbsolutePath());
			System.out.println(tempFile.getName());
			tempFile.deleteOnExit();
			Main.LOGGER.log(Level.INFO, "Temp_EE5_Data located at:" + tempFile.getAbsolutePath());
		} catch (IOException e) {
			Main.LOGGER.log(Level.SEVERE,"Couldn't create temp data file",e);
		}

		// add an icon and title to the program window
		mainStage.getIcons().add(new Image(ResourceLoader.class.getResourceAsStream("SineWave.png"),15,0,true,true));
		mainStage.setTitle("LBMS");
		
		//set standard sizes
		TabPane main = new TabPane();
		Scene scene = new Scene(main,1300,600);
		main.setPrefSize(1300.00,600.00);
		main.setTabClosingPolicy(TabPane.TabClosingPolicy.UNAVAILABLE);
		if(connection == Main.NO_CONNECTION) {
			main.getTabs().add(NoConnectionUI.noConnection());
			mainStage.setScene(scene);
			mainStage.show();
		}
		else {
			//Create the new services for background processing
			oscilloscope = new Oscilloscope(tempFile);
			multimeter = new Multimeter();
			
			//add different tab UI to the main UI
			main.getTabs().add(OscilloscopeUI.Oscilloscope(oscilloscope));
			main.getTabs().add(MultimeterUI.multimeter());
			main.getTabs().add(GeneratorUI.generator());
	
	
			//Check which tab is selected
			main.getSelectionModel().selectedItemProperty().addListener(
					new ChangeListener<Tab>() {
						@Override
						public void changed(ObservableValue<? extends Tab> ov, Tab t, Tab t1) {
//							stop the old tabs background process
							if(t.getText().equals("Oscilloscope"))
								cancelOsci();
							else if(t.getText().equals("Multimeter"))
								cancelMulti();
							else if(t.getText().equals("Function generator"))
								System.out.println("close generator");
	
							
							//start the new tabs background process
							if(t1.getText().equals("Oscilloscope"))
								oscilloscope.restart();
							else if(t1.getText().equals("Multimeter"))
								multimeter.restart();
							else if(t1.getText().equals("Function generator"))
								GeneratorUI.sendGenerator();
						}
					}
				);
			//close background processes and connection when window is closed
			mainStage.setOnCloseRequest(event -> {
					oscilloscope.cancel();
					multimeter.cancel();
					connection.close();
			});
			
			mainStage.setScene(scene);
			mainStage.show();
			//first tab is oscilloscope so start up that process
			oscilloscope.restart();
		}
	}
	
	public static void cancelOsci() {
		oscilloscope.cancel();
//		connection.clearBuffer();
	}
	public static void startOsci() {
		oscilloscope.restart();
	}
	
	public static void cancelMulti() {
		multimeter.cancel();
//		connection.clearBuffer();
	}
	public static void startMulti() {
		multimeter.restart();
	}
}

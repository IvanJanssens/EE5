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
	public static final byte MULTIMETERON = (byte) 0b11111000;
	public static final byte MULTIMETEROFF = (byte) 0b11110000;
	//max data shown on the oscilloscope graph
	public static final int MAX_DATA = 500;
	public static final double TRIGGER = 1.0;
	public static final int ATTENUATION = 1;
	public static File tempFile;
	private static TabPane mainTab;
	private static Stage mainStage;
	
	// start the main UI
	public static void start(Stage stage, Connection conn) {
		connection = conn;
		mainStage = stage;
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
		mainTab = new TabPane();
		Scene scene = new Scene(mainTab,1300,600);
		mainStage.setScene(scene);
		mainTab.setPrefSize(1300.00,600.00);
		mainTab.setTabClosingPolicy(TabPane.TabClosingPolicy.UNAVAILABLE);
		
		
		if(connection == Main.NO_CONNECTION) {
			goToFile(null);
		}
		else {
			goToPic();
		}
	}
	
	public static void goToPic() {
		mainStage.hide();
		if(connection == Main.NO_CONNECTION) {
			connection = PicUI.start(null,j->goToFile((File) j));
		}
		if(connection != Main.NO_CONNECTION) {

			mainTab.getTabs().clear();
			//Create the new services for background processing
			oscilloscope = new Oscilloscope(tempFile);
			multimeter = new Multimeter();
			
			//add different tab UI to the main UI
			mainTab.getTabs().add(OscilloscopeUI.Oscilloscope());
			mainTab.getTabs().add(MultimeterUI.multimeter());
			mainTab.getTabs().add(GeneratorUI.generator());
	
	
			//Check which tab is selected
			mainTab.getSelectionModel().selectedItemProperty().addListener(
					new ChangeListener<Tab>() {
						@Override
						public void changed(ObservableValue<? extends Tab> ov, Tab t, Tab t1) {
	//						stop the old tabs background process
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
			//first tab is oscilloscope so start up that process
			System.out.println("start");
			oscilloscope.restart();

		}
		mainStage.show();
	}
	
	public static void goToFile(File file) {
		mainStage.hide();
		mainTab.getTabs().clear();
		mainTab.getTabs().add(NoConnectionUI.noConnection());
		if(file != null) {
			NoConnectionUI.loadFile(file);
		}
		mainStage.show();
	}
	
	public static void cancelOsci() {
		oscilloscope.cancel();
	}
	public static void startOsci() {
		oscilloscope.restart();
	}
	
	public static void cancelMulti() {
		multimeter.cancel();
	}
	public static void startMulti() {
		multimeter.restart();
	}
}

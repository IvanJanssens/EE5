package userinterface;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.scene.Scene;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.image.Image;
import javafx.stage.Stage;
import resource.ResourceLoader;
import application.Multimeter;
import application.Oscilloscope;
import communication.Connection;
import userinterface.MultimeterUI;
import userinterface.OscilloscopeUI;
import userinterface.GeneratorUI;

public class UI {

	public static Connection connection; // global connection variable
	// messages to be send to the PIC
	public static final int MULTIMETER = '9';
	public static final int OSCILLOSCOPE = '8';
	public static final int STOP = '0';
	//max data shown on the oscilloscope graph
	public static final int MAX_DATA = 200;
	
	// start the main UI
	public static void start(Stage stage, Connection conn) {
		connection = conn;
		Stage mainStage = stage;

		// add an icon and title to the program window
		mainStage.getIcons().add(new Image(ResourceLoader.class.getResourceAsStream("SineWave.png"),15,0,true,true));
		mainStage.setTitle("LBMS");
		
		//set standard sizes
		TabPane main = new TabPane();
		Scene scene = new Scene(main,900,600);
		main.setPrefSize(900.00,600.00);
		main.setTabClosingPolicy(TabPane.TabClosingPolicy.UNAVAILABLE);
		
		//add different tab UI to the main UI
		main.getTabs().add(OscilloscopeUI.Oscilloscope());
		main.getTabs().add(MultimeterUI.multimeter());
		main.getTabs().add(GeneratorUI.generator());

		//Create the new services for background processing
		Oscilloscope oscilloscope = new Oscilloscope(connection);
		Multimeter multimeter = new Multimeter(connection);
		
		//Check which tab is selected
		main.getSelectionModel().selectedItemProperty().addListener(
				new ChangeListener<Tab>() {
					@Override
					public void changed(ObservableValue<? extends Tab> ov, Tab t, Tab t1) {
						//stop the old tabs background process
						if(t.getText().equals("Oscilloscope"))
							oscilloscope.cancel();
						else if(t.getText().equals("Multimeter"))
							multimeter.cancel();
						else if(t.getText().equals("Function generator"))
							System.out.println("close generator");
						
						//start the new tabs background process
						if(t1.getText().equals("Oscilloscope"))
							oscilloscope.restart();
						else if(t1.getText().equals("Multimeter"))
							multimeter.restart();
						else if(t1.getText().equals("Function generator"))
							System.out.println("open generator");
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

package application;
	


import communication.Connection;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.stage.Stage;
import userinterface.LoggerFormatter;
import userinterface.PicUI;

import java.util.logging.*;


public class Main extends Application {
	
	public static final Connection NO_CONNECTION = null;
	public final static Logger LOGGER = Logger.getLogger(Main.class.getName());
	@Override
	public void start(Stage primaryStage) {
		try {
			FileHandler loggerHandler = new FileHandler("%h/Desktop/EE5%u.log",true);
//			FileHandler loggerHandler = NEW FILEHANDLER WITH PATH TO RUNNABLE JAR !!!!!!!!!!!
			System.out.println(System.getProperty("user.home"));
			LoggerFormatter txtFormatter = new LoggerFormatter();
			loggerHandler.setFormatter(txtFormatter);
			LOGGER.addHandler(loggerHandler);
			// start a small interface to choose the pic and create a connection
			Connection connection = PicUI.start(0,j->System.exit((int) j));
			// after the Pic select interface start the main UI
			Platform.runLater(new Runnable() {
				@Override
				public void run(){
					userinterface.UI.start(primaryStage,connection);
				}
			});
			
		} catch(Exception e) {
			e.printStackTrace();
			LOGGER.log(Level.SEVERE,"couldn't start program");
		}
	}
	
	public static void main(String[] args) {
		launch(args); 
	}
}

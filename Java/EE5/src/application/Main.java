package application;
	


import communication.Connection;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.stage.Stage;
import userinterface.LoggerFormatter;
import userinterface.PicUI;
import java.util.logging.*;


public class Main extends Application {
	
	public static final Connection NO_CONNECTION = new Connection(null);
	public final static Logger LOGGER = Logger.getLogger(Main.class.getName());
	@Override
	public void start(Stage primaryStage) {
		try {
			FileHandler loggerHandler = new FileHandler("%h/Desktop/EE5%u.log",true);
			System.out.println(System.getProperty("user.home"));
			LoggerFormatter txtFormatter = new LoggerFormatter();
			loggerHandler.setFormatter(txtFormatter);
			System.out.println(loggerHandler.getFormatter());
			LOGGER.addHandler(loggerHandler);
			LOGGER.severe("Test");
			LOGGER.info("Test2");
			// start a small interface to choose the pic and create a connection
			Connection connection = PicUI.start();
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

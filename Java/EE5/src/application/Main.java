package application;
	


import communication.Connection;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.stage.Stage;
import userinterface.PicUI;


public class Main extends Application {
	@Override
	public void start(Stage primaryStage) {
		try {
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
		}
	}
	
	public static void main(String[] args) {
		launch(args);
	}
}

package application;
	


import javafx.application.Application;
import javafx.application.Platform;
import javafx.stage.Stage;
import userinterface.PicUI;


public class Main extends Application {
	@Override
	public void start(Stage primaryStage) {
		try {
			PicUI.start();
			Platform.runLater(new Runnable() {
				@Override
				public void run(){
					userinterface.UI.start(primaryStage);
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

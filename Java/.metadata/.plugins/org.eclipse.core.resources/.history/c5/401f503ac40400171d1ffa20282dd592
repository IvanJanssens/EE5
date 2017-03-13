package userinterface;


import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.Tab;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;

public class MultimeterUI extends UI{

	static Label meter;
	
	// create new tab for the multimeter
	public static Tab multimeter() {
		Tab multimeter = new Tab();
		multimeter.setText("Multimeter");
		ScrollPane multiPanel = new ScrollPane();
		multiPanel.setFitToHeight(true);
		multiPanel.setFitToWidth(true);
		multiPanel.setContent(multiBorderPane());
		multimeter.setContent(multiPanel);
		return multimeter;
	}
	
	private static BorderPane multiBorderPane() {
		BorderPane multiBorderPane = new BorderPane();
		HBox header = multiButton();
		multiBorderPane.setTop(header);
		BorderPane.setMargin(header, new Insets(10,10,10,10));
		multiBorderPane.setCenter(meter());
		return multiBorderPane;
		
	}
	
	private static HBox multiButton() {
		HBox multiButtons = new HBox(10);
		multiButtons.getChildren().addAll(led1(),led2(),led3());
		return multiButtons;
	}
	
	private static Button led1() {
		Button led1 = new Button("1 led");
		led1.setOnMouseClicked(new EventHandler<Event>() {
			@Override
			public void handle(Event e){
				connection.send((int) '1');
			}
		});
		return led1;
	}
	
	private static Button led2() {
		Button led2 = new Button("2 leds");
		led2.setOnMouseClicked(new EventHandler<Event>() {
			@Override
			public void handle(Event e){
				connection.send((int)'2');
			}
		});
		return led2;
	}
	
	private static Button led3() {
		Button led3 = new Button("3 leds");
		led3.setOnMouseClicked(new EventHandler<Event>() {
			@Override
			public void handle(Event e){
				connection.send((int)'3');
			}
		});
		return led3;
	}
	
	private static Label meter() {
		meter = new Label(" volt");
		return meter;
	}
	
	public static void updateMeter(double d) {
		meter.setText(d +" volt");
	}
}

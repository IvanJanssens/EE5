package userinterface;



import javafx.geometry.Insets;
import javafx.geometry.Orientation;
import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.Tab;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.CornerRadii;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;

public class MultimeterUI extends UI{

	static Label meter;
	static sevenSegments display;
	
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
//		HBox header = multiButton();
//		multiBorderPane.setTop(header);
//		BorderPane.setMargin(header, new Insets(10,10,10,10));
		FlowPane center = multiCenter();
		multiBorderPane.setCenter(center);
		center.setAlignment(Pos.CENTER);
		return multiBorderPane;
		
	}
	
	private static Pane digits() {
		Pane center = new Pane();
		center.setPrefSize(470, 235);
		center.setMaxSize(470, 235);
		center.setMinSize(470, 235);
		center.setBackground(new Background(new BackgroundFill(Color.BLACK,new CornerRadii(20, false), new Insets(0,0,50,0))));
		display = new sevenSegments();
		for(int i = 0; i < 4 ; i ++) {
			center.getChildren().addAll(display.getSegments()[i]);
		}
		for(int i = 0; i < 3; i ++) {
			center.getChildren().addAll(display.getDots()[i]);
		}
		return center;
	}
	
	private static FlowPane multiCenter(){
		FlowPane center = new FlowPane(Orientation.HORIZONTAL);
		
//		HBox center = new HBox(10);
		Label meter = meter();
		HBox.setMargin(meter, new Insets(120,0,0,0));
		center.getChildren().addAll(digits(),meter);
		return center;
	}
	
//	private static HBox multiButton() {
//		HBox multiButtons = new HBox(10);
//		multiButtons.getChildren().addAll(led1(),led2(),led3());
//		return multiButtons;
//	}
	
//	private static Button led1() {
//		Button led1 = new Button("1 led");
//		led1.setOnMouseClicked(new EventHandler<Event>() {
//			@Override
//			public void handle(Event e){
//				connection.send());
//			}
//		});
//		return led1;
//	}
//	
//	private static Button led2() {
//		Button led2 = new Button("2 leds");
//		led2.setOnMouseClicked(new EventHandler<Event>() {
//			@Override
//			public void handle(Event e){
//				int test = (int) '2';
//				System.out.println(test);
//				System.out.println((byte) test);
////				connection.send((int)'2');
//			}
//		});
//		return led2;
//	}
//	
//	private static Button led3() {
//		Button led3 = new Button("3 leds");
//		led3.setOnMouseClicked(new EventHandler<Event>() {
//			@Override
//			public void handle(Event e){
//				int test = (int) '3';
//				System.out.println(test);
//				System.out.println((byte) test);
////				connection.send((int)'3');
//			}
//		});
//		return led3;
//	}
	
	private static Label meter() {
		meter = new Label(" volt");
		meter.setFont(new Font("Calibri",60));
		meter.setTextFill(Color.RED);
		return meter;
	}
	
	public static void updateMeter(double d) {
//		meter.setText(String.format("%.4f", d) +" volt");
		display.writeNumber(d);
		if(d < 1)
			meter.setText("miliVolt");
		else
			meter.setText(" volt");
	}
}

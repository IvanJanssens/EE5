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
		display.start();
		return center;
	}
	
	private static FlowPane multiCenter(){
		FlowPane center = new FlowPane(Orientation.HORIZONTAL);
		Label meter = meter();
		HBox.setMargin(meter, new Insets(120,0,0,0));
		center.getChildren().addAll(digits(),meter);
		return center;
	}
	
	private static Label meter() {
		meter = new Label(" volt");
		meter.setFont(new Font("Calibri",60));
		meter.setTextFill(Color.RED);
		return meter;
	}
	
	public static void updateMeter(double d) {
		display.writeNumber(d);
		if(d < 1)
			meter.setText("milliVolt");
		else
			meter.setText(" volt");
	}
}

package userinterface;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

import javafx.embed.swing.SwingFXUtils;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.ValueAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Button;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.Tab;
import javafx.scene.control.ToggleButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.image.WritableImage;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;

public class OscilloscopeUI extends UI{


	private static BorderPane osciBody;
	private static XYChart.Series<Number, Number> data;
	private static int datapoint;
	
	// create new tab for the oscilloscope
	public static Tab Oscilloscope() {
		Tab oscilloscope = new Tab();
		oscilloscope.setText("Oscilloscope");
		ScrollPane osciPanel = new ScrollPane();
			osciPanel.setFitToHeight(true);
			osciPanel.setFitToWidth(true);
			osciPanel.setContent(osciBorderPane());
		oscilloscope.setContent(osciPanel);
		return oscilloscope;
	}
	
	private static BorderPane osciBorderPane() {
		BorderPane osciBorderPane = new BorderPane();
		HBox header = mainButtons();
		osciBorderPane.setTop(header);
		BorderPane.setMargin(header, new Insets(10,0,10,10));
		osciBody = osciBody();
		osciBorderPane.setCenter(osciBody);
		return osciBorderPane;
	}
	
	private static HBox mainButtons() {
		HBox mainButtons = new HBox(10);
		mainButtons.setAlignment(Pos.CENTER_LEFT);
		mainButtons.getChildren().addAll(Open(),Save(),printScreen(),help());
		return mainButtons;
	}
	
	//open basic filechooser
	private static Button Open(){
		Button open = new Button("Open");
		open.setPrefWidth(120);
		open.setOnMouseClicked(new EventHandler<Event>() {
			@Override
			public void handle(Event e){
				FileChooser open = new FileChooser();
				open.setTitle("Open mesurements");
				open.getExtensionFilters().add(new ExtensionFilter("text","*.txt"));
				File selectedFile = open.showOpenDialog(null);
				if(selectedFile != null){
					
				}
					
			}
		});
		return open;
	}
	
	private static Button Save() {
		Button save = new Button("Save");
		save.setPrefWidth(120);
		save.setOnMouseClicked(new EventHandler<Event>() {
			@Override
			public void handle(Event e){
				FileChooser saving = new FileChooser();
				saving.setTitle("Save mesurements");
				saving.getExtensionFilters().add(new ExtensionFilter("Text file","*.txt"));
				File selectedFile = saving.showSaveDialog(null);
				if(selectedFile != null){
					
				}
			}
		});
		return save;
	}
	
	private static Button printScreen() {
		Button printScreen = new Button("Printscreen");
		printScreen.setPrefWidth(120);
		printScreen.setOnMouseClicked(new EventHandler<Event>() {
			@Override
			public void handle(Event e){
				connection.send(STOP); // pause the inputstream of data during the snapshot
				FileChooser saving = new FileChooser();
				saving.setTitle("Printscreen");
				saving.getExtensionFilters().add(new ExtensionFilter("Png","*.png"));
				File selectedFile = saving.showSaveDialog(null);
				if(selectedFile != null){
					try {
						//get a snapshot of the oscibody, this includes the different buttons of the oscilloscope and the graph
			            WritableImage snapshot = osciBody.snapshot(null, null);
			            BufferedImage tempImg = SwingFXUtils.fromFXImage(snapshot, null);

			            ImageIO.write(tempImg, "png", selectedFile);
			            connection.send(OSCILLOSCOPE); // restart the inputstream

			        } catch (IOException ex) {
			            // TODO Auto-generated catch block
			            ex.printStackTrace();
			        }
				}
			}
		});
		return printScreen;
	}
	
	private static Button help() {
		Button help = new Button("Help");
		help.setPrefWidth(120);
		return help;
	}
	
	private static BorderPane osciBody(){
		BorderPane osciBody = new BorderPane();
		VBox osciButtons = osciButtons();
		osciBody.setRight(osciButtons);
		BorderPane.setMargin(osciButtons, new Insets(10,10,10,10));
		osciBody.setCenter(osciGraph());
		return osciBody;
	}
	
	private static VBox osciButtons() {
		VBox oscibuttons = new VBox(10);
		oscibuttons.getChildren().addAll(channel(),attenuation(),trigger(),timediv(),rms(),ptp());
		return oscibuttons;
	}
	
	private static VBox channel() {
		VBox channel = new VBox(10);
		RadioButton channel1 = new RadioButton();
		channel1.setText("Channel 1");
		RadioButton channel2 = new RadioButton();
		channel2.setText("Channel 2");
		channel.getChildren().addAll(channel1,channel2);
		return channel;
	}
	
	private static HBox attenuation() {
		HBox attenuation = new HBox();
		ToggleButton  x2 = new ToggleButton ();
		x2.setText("X2");
		x2.setPrefWidth(40);
		ToggleButton  x5 = new ToggleButton ();
		x5.setText("X5");
		x5.setPrefWidth(40);
		ToggleButton  x10 = new ToggleButton ();
		x10.setText("X10");
		x10.setPrefWidth(40);
		ToggleGroup group = new ToggleGroup();
		x2.setToggleGroup(group);
		x5.setToggleGroup(group);
		x10.setToggleGroup(group);
		group.selectToggle(x2);
		attenuation.getChildren().addAll(x2,x5,x10);
		return attenuation;
	}
	
	private static Button trigger() {
		Button trigger = new Button();
		trigger.setText("trigger");
		trigger.setPrefWidth(120);
		return trigger;
	}
	
	private static Button timediv() {
		Button timediv = new Button();
		timediv.setText("timediv");
		timediv.setPrefWidth(120);
		return timediv;
	}
	
	private static Button rms() {
		Button rms = new Button();
		rms.setPrefWidth(120);
		rms.setText("RMS");
		return rms;
	}
	
	private static Button ptp() {
		Button ptp = new Button();
		ptp.setPrefWidth(120);
		ptp.setText("PTP");
		return ptp;
	}
	
	//Linechart
	private static LineChart<Number, Number> osciGraph() {
		NumberAxis xAxis = new NumberAxis();
		NumberAxis yAxis = new NumberAxis();
		xAxis.setLabel("Time (s)");
		yAxis.setLabel("Voltage (V)");
		LineChart<Number, Number> graph = new LineChart<Number, Number>(xAxis,yAxis);
		graph.setTitle("Oscilloscope");
		data = new XYChart.Series<Number, Number>();
		data.setName("values");
		datapoint = 0;
		graph.getData().add(data);
		graph.getXAxis().setAutoRanging(false);
		graph.getYAxis().setAutoRanging(false);
		((ValueAxis<Number>) graph.getYAxis()).setUpperBound(5.5);
		return graph;
	}
	
	public static void addData(double newPoint) {
		
		//get number of datapoints
        int numOfPoint = data.getData().size();
        if(numOfPoint >= MAX_DATA) {
	        data.getData().remove(0); //remove first point
	        ((ValueAxis<Number>) data.getChart().getXAxis()).setLowerBound(datapoint-MAX_DATA); //adapt the x-axis of the chart
        }
        ((ValueAxis<Number>) data.getChart().getXAxis()).setUpperBound(datapoint);
		data.getData().add(new XYChart.Data<Number, Number>(datapoint,newPoint)); // add new datapoint
		datapoint += 1;
	}
}

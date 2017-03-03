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
import javafx.beans.value.ChangeListener;
import javafx.scene.Scene;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Button;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.Slider;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.control.TextField;
import javafx.scene.control.ToggleButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.image.WritableImage;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;
import javafx.stage.Stage;

public class UI {
	
	private static BorderPane osciBody;
	
	public static void start(Stage stage) {
		Stage mainStage = stage;
		mainStage.getIcons().add(new Image("file:SineWave.png",15,0,true,true));
		mainStage.setTitle("LBMS");
		
		TabPane main = new TabPane();
		Scene scene = new Scene(main,900,600);
		main.setPrefSize(900.00,600.00);
		main.setTabClosingPolicy(TabPane.TabClosingPolicy.UNAVAILABLE);
		main.getTabs().add(Oscilloscope());
		main.getTabs().add(multimeter());
		main.getTabs().add(generator());
		mainStage.setScene(scene);
		mainStage.show();
	}
	
	private static Tab Oscilloscope() {
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
				FileChooser saving = new FileChooser();
				saving.setTitle("Printscreen");
				saving.getExtensionFilters().add(new ExtensionFilter("Png","*.png"));
				File selectedFile = saving.showSaveDialog(null);
				if(selectedFile != null){
					try {
			            WritableImage snapshot = osciBody.snapshot(null, null);
			            BufferedImage tempImg = SwingFXUtils.fromFXImage(snapshot, null);

			            ImageIO.write(tempImg, "png", selectedFile);

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
	
	private static LineChart<Number, Number> osciGraph() {
		NumberAxis xAxis = new NumberAxis();
		NumberAxis yAxis = new NumberAxis();
		xAxis.setLabel("Time (s)");
		yAxis.setLabel("Voltage (V)");
		LineChart<Number, Number> graph = new LineChart<Number, Number>(xAxis,yAxis);
		graph.setTitle("Oscilloscope");
		XYChart.Series<Number, Number> data = new XYChart.Series<Number, Number>();
		data.setName("values");
		data.getData().add(new XYChart.Data<Number, Number>(1,5));
		data.getData().add(new XYChart.Data<Number, Number>(2,2));
		data.getData().add(new XYChart.Data<Number, Number>(3,9));
		data.getData().add(new XYChart.Data<Number, Number>(4,5));
		data.getData().add(new XYChart.Data<Number, Number>(5,0));
		data.getData().add(new XYChart.Data<Number, Number>(6,0));
		data.getData().add(new XYChart.Data<Number, Number>(7,7));
		data.getData().add(new XYChart.Data<Number, Number>(8,3));
		data.getData().add(new XYChart.Data<Number, Number>(9,4));
		data.getData().add(new XYChart.Data<Number, Number>(10,5));
		graph.getData().add(data);
		return graph;
	}
	
	private static Tab multimeter() {
		Tab multimeter = new Tab();
		multimeter.setText("Multimeter");
		ScrollPane multiPanel = new ScrollPane();
		multiPanel.setFitToHeight(true);
		multiPanel.setFitToWidth(true);
		multiPanel.setContent(multiBorderPane());
		multimeter.setContent(multiPanel);
		return multimeter;
	}
	
	private static Tab generator() {
		Tab generator = new Tab();
		generator.setText("Function generator");
		ScrollPane genePanel = new ScrollPane();
		genePanel.setFitToHeight(true);
		genePanel.setFitToWidth(true);
		genePanel.setContent(geneBorderPane());
		generator.setContent(genePanel);
		return generator;
	}
	
	private static BorderPane multiBorderPane() {
		BorderPane multiBorderPane = new BorderPane();
		return multiBorderPane;
		
	}
	
	private static BorderPane geneBorderPane() {
		BorderPane geneBorderPane = new BorderPane();
		HBox header = geneButton();
		geneBorderPane.setTop(header);
		BorderPane.setMargin(header, new Insets(10,0,10,10));
		BorderPane geneBody = geneBody();
		geneBorderPane.setCenter(geneBody);
		return geneBorderPane;
		
	}
	
	private static HBox geneButton() {
		HBox mainButtons = new HBox(10);
		mainButtons.setAlignment(Pos.CENTER_LEFT);
		mainButtons.getChildren().add(help());
		return mainButtons;
	}
	
	private static BorderPane geneBody() {
		BorderPane geneBody = new BorderPane();
		VBox generator = new VBox(10);
		generator.setAlignment(Pos.TOP_LEFT);
		
		RadioButton sine = new RadioButton();
		sine.setGraphic(new ImageView(new Image("file:SineWave.png",100,0,true,false)));
		RadioButton square = new RadioButton();
		square.setGraphic(new ImageView(new Image("file:SquareWave.png",100,0,true,false)));
		
		ToggleGroup group = new ToggleGroup();
		sine.setToggleGroup(group);
		square.setToggleGroup(group);
		group.selectToggle(sine);
		
		Slider freq = new Slider();
		freq.setMin(0);
		freq.setMax(1000);
		freq.setValue(330);
		freq.setShowTickLabels(true);
		freq.setMajorTickUnit(50);
		
		TextField freqField = new TextField("330") {
			@Override
			public void replaceText(int start, int end, String text){
				if(text.matches("[0-9]*")) {
					super.replaceText(start, end, text);
				}
			}
			
			@Override
			public void replaceSelection(String text) {
				if(text.matches("[0-9]*")) {
					super.replaceSelection(text);
				}
			}
		};
		freqField.textProperty().addListener((ChangeListener) (arg0, arg1, arg2) -> freq.setValue(Integer.parseInt(freqField.getText())));
		freq.valueProperty().addListener((ChangeListener) (arg0, arg1, arg2) -> freqField.textProperty().setValue(String.valueOf((int) freq.getValue())));
		
		generator.getChildren().addAll(sine,square,freq,freqField);
		geneBody.setCenter(generator);
		return geneBody;
	}
}

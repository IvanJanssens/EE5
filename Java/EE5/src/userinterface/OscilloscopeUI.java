package userinterface;

import java.awt.image.BufferedImage;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.nio.channels.FileChannel;
import java.nio.file.Files;
import java.util.List;

import javax.imageio.ImageIO;

import javafx.application.Platform;
import javafx.embed.swing.SwingFXUtils;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.ValueAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.Separator;
import javafx.scene.control.Spinner;
import javafx.scene.control.SpinnerValueFactory;
import javafx.scene.control.SpinnerValueFactory.IntegerSpinnerValueFactory;
import javafx.scene.control.Tab;
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
import resource.ResourceLoader;
import application.Oscilloscope;

public class OscilloscopeUI extends UI{


	private static BorderPane osciBody;
	private static XYChart.Series<Number, Number> data;
	private static XYChart.Series<Number, Number> fftData;
	private static int datapoint;
	private static int fftDatapoint;
	private static double max;
	private static double min;
	private static Label ptp;
	private static Label rms;
	private static double sum;
	private static double triggerValue;
	private static int max_data = 10;
	private static double prevValue = 0;
	private static double prevFftValue = 0;
	private static Oscilloscope oscilloscope;
	
	// create new tab for the oscilloscope
	public static Tab Oscilloscope(Oscilloscope oscillo) {
		oscilloscope = oscillo;
		Tab oscilloscopeTab = new Tab();
		oscilloscopeTab.setText("Oscilloscope");
		ScrollPane osciPanel = new ScrollPane();
			osciPanel.setFitToHeight(true);
			osciPanel.setFitToWidth(true);
			osciPanel.setContent(osciBorderPane());
			oscilloscopeTab.setContent(osciPanel);
		return oscilloscopeTab;
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
				UI.cancelOsci();
				FileChooser saving = new FileChooser();
				saving.setTitle("Save mesurements");
				saving.getExtensionFilters().add(new ExtensionFilter("Text file","*.txt"));
				File selectedFile = saving.showSaveDialog(null);
				if(selectedFile != null){
					try {
						FileChannel src = new FileInputStream(tempFile).getChannel();
						FileChannel dest = new FileOutputStream(selectedFile).getChannel();
						dest.transferFrom(src, 0, src.size());
//								BufferedWriter buffer = new BufferedWriter( new FileWriter(selectedFile));
//								for(int i = 0; i < data.getData().size(); i++) {
//									buffer.write(data.getData().get(i) + "");
//									buffer.newLine();
//								}
//								buffer.flush();
//								buffer.close();
					} catch (Exception ex) {
						ex.printStackTrace();
					}
					UI.startOsci();
					
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
				UI.cancelOsci(); // pause the inputstream of data during the snapshot
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
			            UI.startOsci(); // restart the inputstream

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
		help.setOnMouseClicked(new EventHandler<Event>() {
			@Override
			public void handle(Event e) {
				UI.cancelOsci();
				Alert helpAlert = new Alert(AlertType.INFORMATION);
				helpAlert.setTitle("Help menu");
				helpAlert.setHeaderText("Help with the oscilloscope");
				helpAlert.getDialogPane().setPrefHeight(400);
				helpAlert.setResizable(true);
				helpAlert.getDialogPane().setContent(helpContent());

				helpAlert.showAndWait();
				UI.startOsci();
			}
		});
		return help;
	}
	
	private static ScrollPane helpContent() {
		ScrollPane helpContent = new ScrollPane();
		helpContent.setFitToWidth(true);
		VBox content = new VBox(10);
		content.getChildren().addAll(
				new Label("This is the graph of the oscilloscope: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("graph.png"),300,0,true,true)),
				new Separator(),
				new Label("Open data file to display on the graph: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("open.png"),300,0,true,true)),
				new Separator(),
				new Label("Save datapoints of the graph in a file: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("save.png"),300,0,true,true)),
				new Separator(),
				new Label("Printscreen the graph and option selection to a PNG image: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("print.png"),300,0,true,true)),
				new Separator(),
				new Label("Select input source: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("source.png"),200,0,true,true)),
				new Separator(),
				new Label("Select amplification: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("amplification.png"),200,0,true,true)),
				new Separator(),
				new Label("Select trigger level: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("trigger.png"),200,0,true,true)),
				new Separator(),
				new Label("Select time of the graph (X-axis): "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("time.png"),200,0,true,true)),
				new Separator(),
				new Label("Display Root Mean Square (RMS) value of the graph: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("RMS.png"),200,0,true,true)),
				new Separator(),
				new Label("Display Peak to Peak value of the graph: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("PtP.png"),200,0,true,true)));
		helpContent.setContent(content);
		
		return helpContent;
	}
	
	private static BorderPane osciBody(){
		BorderPane osciBody = new BorderPane();
		VBox osciButtons = osciButtons();
		osciBody.setRight(osciButtons);
		BorderPane.setMargin(osciButtons, new Insets(10,10,10,10));
		osciBody.setCenter(osciGraph());
		return osciBody;
	}
	
//	private static HBox center() {
//		HBox center = new HBox(10);
//		center.getChildren().addAll(osciGraph(),fftGraph());
//		return center;
//	}
	
	private static VBox osciButtons() {
		VBox oscibuttons = new VBox(10);
		oscibuttons.getChildren().addAll(channel(),attenuation(),new Label("Trigger (v): "),trigger(),new Label("time div (msec): "),timediv(),rms(),ptp());
		return oscibuttons;
	}
	
	private static VBox channel() {
		VBox channel = new VBox(10);
		RadioButton channel1 = new RadioButton();
		channel1.setText("Channel 1");
		channel1.setSelected(true);
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
	
	private static Spinner<Double> trigger() {
		Spinner<Double> trigger = new Spinner<Double>();
		trigger.setValueFactory(new SpinnerValueFactory.DoubleSpinnerValueFactory(-5,5));
		trigger.setEditable(true);
		trigger.getValueFactory().setValue((double) 0);
		trigger.setPrefWidth(120);
		trigger.valueProperty().addListener((obs, oldValue, newValue) -> {
//			triggerValue = newValue;
			oscilloscope.updateTrigger(newValue);
		});
		return trigger;
	}
	
	private static Spinner<Integer> timediv() {
		Spinner<Integer> timediv = new Spinner<Integer>();
		timediv.setValueFactory(new IntegerSpinnerValueFactory(10,5000));
		timediv.setPrefWidth(120);
		timediv.setEditable(true);
		timediv.valueProperty().addListener((obs, oldValue, newValue) -> {
			if(newValue < max_data) {
				data.getData().remove(newValue, data.getData().size());
			}
//			max_data = newValue;
	        ((ValueAxis<Number>) data.getChart().getXAxis()).setUpperBound(newValue);
			oscilloscope.updateTimeDiv(newValue);
		});
		return timediv;
	}
	
	private static Label rms() {
		rms = new Label("Root Mean Square\n(RMS): ");
		rms.setPrefWidth(120);
		return rms;
	}
	
	private static Label ptp() {
		ptp = new Label("Peak to Peak: ");
		ptp.setPrefWidth(120);
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
		graph.setMinWidth(400);
		graph.setMaxWidth(1000);
		graph.setAnimated(false);
		((ValueAxis<Number>) graph.getYAxis()).setUpperBound(5.5);
		((ValueAxis<Number>) graph.getXAxis()).setLowerBound(0);
        ((ValueAxis<Number>) data.getChart().getXAxis()).setUpperBound(max_data);
		return graph;
	}
	
	public static void addData(List<Double>  newPoint) {
		
		//get number of datapoints
//        int numOfPoint = data.getData().size();
//		if(datapoint >= max_data && newPoint > triggerValue && prevValue <= triggerValue) {
//			datapoint = 0;
//			
//		}
//		((ValueAxis<Number>) data.getChart().getXAxis()).setLowerBound(0);
//        ((ValueAxis<Number>) data.getChart().getXAxis()).setUpperBound(max_data);
//        System.out.println("1:" + numOfPoint);
//        System.out.println("2:" + max_data);
//        System.out.println("3:" +datapoint);
//		if(numOfPoint >= max_data && datapoint < max_data)

        for(int i = 0; i < newPoint.size(); i++) {
        	if(data.getData().size() >= max_data && i < max_data) {
        		data.getData().set(i, new XYChart.Data<Number, Number>(i,newPoint.get(i))); // add new datapoint
        	}
        	else {
        		data.getData().add(new XYChart.Data<Number, Number>(i,newPoint.get(i))); // add new datapoint
        	}
        }
//        datapoint += 1;
//		if(datapoint == max_data){
//			min = (double) data.getData().get(0).getYValue();
//			max = (double) data.getData().get(0).getYValue();
//			sum = 0;
//			data.getData().forEach(value-> {
//				sum += ((double) value.getYValue()*(double) value.getYValue());
//				if((double) value.getYValue() > max)
//					max = (double)value.getYValue();
//				if((double) value.getYValue() < min)
//					min = (double)value.getYValue();
//			});
//			updateRMS(Math.sqrt(sum/max_data));
//			updatePtP(max,min);
//		}
//		prevValue = newPoint;
	}
	
	private static void updateRMS(double rmsValue) {
		rms.setText("Root Mean Square\n(RMS):\n" + (String.format("%.4f", rmsValue)));
	}
	
	private static void updatePtP(double max, double min) {
		ptp.setText("Peak to Peak: \n" + (String.format("%.4f", (max-min))));
	}
	
	private static LineChart<Number, Number> fftGraph() {
		NumberAxis xAxis = new NumberAxis();
		NumberAxis yAxis = new NumberAxis();
		xAxis.setLabel("Time (s)");
		yAxis.setLabel("Voltage (V)");
		LineChart<Number, Number> graph = new LineChart<Number, Number>(xAxis,yAxis);
		graph.setTitle("Oscilloscope");
		fftData = new XYChart.Series<Number, Number>();
		fftData.setName("FFT values");
		fftDatapoint = 0;
		graph.getData().add(fftData);
		graph.getXAxis().setAutoRanging(false);
		graph.getYAxis().setAutoRanging(false);
		graph.setMinWidth(400);
		graph.setMaxWidth(1000);
		graph.setAnimated(false);
		((ValueAxis<Number>) graph.getYAxis()).setUpperBound(5.5);
		return graph;
	}

	public static void addFftData(float[] fftzeropad, float max) {
		double newPoint;
		for(int i = 0; i< fftzeropad.length; i++) {
			newPoint = (double) (fftzeropad[i]/max*3.2);
//			System.out.println(newPoint);
			//get number of datapoints
	        int numOfPoint = fftData.getData().size();
			if(fftDatapoint >= 4*max_data && newPoint > triggerValue && prevFftValue <= triggerValue) {
				fftDatapoint = 0;
				
			}
	//        if(numOfPoint >= max_data) {
	//	        data.getData().remove(datapoint); //remove first point
	//	        ((ValueAxis<Number>) data.getChart().getXAxis()).setLowerBound(datapoint-max_data); //adapt the x-axis of the chart
	//        }
			((ValueAxis<Number>) fftData.getChart().getXAxis()).setLowerBound(0);
	        ((ValueAxis<Number>) fftData.getChart().getXAxis()).setUpperBound(4*max_data);
	//        System.out.println("1:" + numOfPoint);
	//        System.out.println("2:" + max_data);
	//        System.out.println("3:" + datapoint);
	        if(numOfPoint >= 4*max_data && fftDatapoint < 4*max_data)
	        	fftData.getData().set(fftDatapoint, new XYChart.Data<Number, Number>(fftDatapoint,newPoint)); // add new datapoint
	        else if(fftDatapoint < 4*max_data)
	        	fftData.getData().add(new XYChart.Data<Number, Number>(fftDatapoint,newPoint)); // add new datapoint
	        fftDatapoint += 1;
	        prevFftValue = newPoint;
		}
		
	}
}
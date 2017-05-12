package userinterface;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.util.logging.Level;

import javax.imageio.ImageIO;

import javafx.beans.value.ChangeListener;
import javafx.embed.swing.SwingFXUtils;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.chart.LineChart;
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
import application.Main;
import application.Oscilloscope;

public class OscilloscopeUI extends UI{


	private static BorderPane osciBody;
	private static Label ptp;
	private static Label rms;
	private static Oscilloscope oscilloscope;
	private static NumbField dataFrom;
	private static NumbField dataTill;
	private static VBox prevData;
	private static VBox oscibuttons;
	private static HBox mainButtons;
	private static File loadFile;
	
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
		mainButtons = new HBox(10);
		mainButtons.setAlignment(Pos.CENTER_LEFT);
		mainButtons.getChildren().addAll(Load(),Save(),printScreen(),help());
		return mainButtons;
	}
	
	//open basic filechooser
	private static Button Load(){
		Button load = new Button("Load");
		load.setPrefWidth(120);
		load.setOnMouseClicked(new EventHandler<Event>() {
			@Override
			public void handle(Event e){
				FileChooser open = new FileChooser();
				open.setTitle("Load mesurements");
				open.getExtensionFilters().add(new ExtensionFilter("text","*.txt"));
				loadFile = open.showOpenDialog(null);
				if(loadFile != null){
					oscibuttons.getChildren().clear();
					oscibuttons.getChildren().addAll(rms(),ptp(),prevData);
					mainButtons.getChildren().remove(0, 2);
					mainButtons.getChildren().add(0,newMes());
					mainButtons.getChildren().add(0,loadNew());
					readFile(loadFile);
				}
					
			}
		});
		return load;
	}
	private static Button newMes() {
		Button newMes = new Button("Start oscilloscope");
		newMes.setPrefWidth(120);
		newMes.setOnMouseClicked(new EventHandler<Event> () {
			@Override
			public void handle(Event e) {
				oscibuttons.getChildren().clear();
				oscibuttons.getChildren().addAll(channel(),attenuation(),new Label("Trigger (v): "),trigger(),new Label("time div (msec): "),timediv(),rms(),ptp(),fft());
				mainButtons.getChildren().remove(0,2);
				mainButtons.getChildren().add(0,Save());
				mainButtons.getChildren().add(0,Load());
			}
		});
		return newMes;
	}
	
	private static Button loadNew() {
		Button loadNew = new Button("Load new");
		loadNew.setPrefWidth(120);
		loadNew.setOnMouseClicked(new EventHandler<Event> () {
			@Override
			public void handle(Event e) {
				FileChooser open = new FileChooser();
				open.setTitle("Load mesurements");
				open.getExtensionFilters().add(new ExtensionFilter("text","*.txt"));
				loadFile = open.showOpenDialog(null);
				if(loadFile != null){
					readFile(loadFile);
				}
			}
		});
		return loadNew;
	}
	
	private static void readFile(File file) {
		GraphUI.clearGraph();
		int from = Integer.parseInt(dataFrom.getText());
		int till = Integer.parseInt(dataTill.getText());
		int currentData = from;
		int max_data = till-from + 1;
        BufferedReader in = null;
		try {
			in = new BufferedReader(new FileReader(file));
			int currentLineNo = 0;
			while(currentLineNo < from){
				if(in.readLine() == null) {
					throw new IOException("File too small");
				}
				currentLineNo++;
			}
			while(currentLineNo <= till) {
				String nextLine = in.readLine();
				if(nextLine == null)
					return;
				GraphUI.addData(Double.parseDouble(nextLine),from,(till - from), currentData,GraphUI.OsciUI);
				currentLineNo++;
				currentData++;
			}
		} catch (IOException ex) {
			Main.LOGGER.log(Level.SEVERE,"Couldn't read dataFile",ex);
		} finally {
			try {
				if(in!=null)
					in.close();
			} catch(IOException ex) {
				Main.LOGGER.log(Level.WARNING, "Datafile not closed", ex);
			}
		}
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
					} catch (Exception ex) {
						Main.LOGGER.log(Level.SEVERE,"Coudn't copy temp file to new",ex);
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
						Main.LOGGER.log(Level.SEVERE,"Coudn't make a screenshot",ex);
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
		LineChart<Number,Number> graph = GraphUI.osciGraph();
		osciBody.setCenter(graph);
		return osciBody;
	}
	
	private static VBox osciButtons() {
		oscibuttons = new VBox(10);
		oscibuttons.getChildren().addAll(channel(),attenuation(),new Label("Trigger (v): "),trigger(),new Label("time div (msec): "),timediv(),rms(),ptp(),fft());
		prevData();
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
		x2.setOnMouseClicked(new EventHandler<Event>() {
			@Override
			public void handle(Event e) {
				oscilloscope.updateAttenuation(2);
			}
		});
		x5.setOnMouseClicked(new EventHandler<Event>() {
			@Override
			public void handle(Event e) {
				oscilloscope.updateAttenuation(5);
			}
		});
		x10.setOnMouseClicked(new EventHandler<Event>() {
			@Override
			public void handle(Event e) {
				oscilloscope.updateAttenuation(10);
			}
		});
		attenuation.getChildren().addAll(x2,x5,x10);
		return attenuation;
	}
	
	private static Spinner<Double> trigger() {
		Spinner<Double> trigger = new Spinner<Double>();
		trigger.setValueFactory(new SpinnerValueFactory.DoubleSpinnerValueFactory(-5,5));
		trigger.setEditable(true);
		trigger.getValueFactory().setValue(UI.TRIGGER);
		trigger.setPrefWidth(120);
		trigger.valueProperty().addListener((obs, oldValue, newValue) -> {
			Oscilloscope.updateTrigger(newValue);
		});
		return trigger;
	}
	
	private static Spinner<Integer> timediv() {
		Spinner<Integer> timediv = new Spinner<Integer>();
		timediv.setValueFactory(new IntegerSpinnerValueFactory(10,5000));
		timediv.getValueFactory().setValue(UI.MAX_DATA);
		timediv.setPrefWidth(120);
		timediv.setEditable(true);
		timediv.valueProperty().addListener((obs, oldValue, newValue) -> {
	        Oscilloscope.updateTimeDiv(newValue);
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
	private static ToggleButton fft() {
		ToggleButton fft = new ToggleButton("fft");
		fft.setPrefWidth(120);
		return fft;
	}
	
	private static void prevData() {
		prevData = new VBox(10);
		dataFrom = new NumbField("0");
		dataFrom.textProperty().addListener((ChangeListener) (arg0, arg1, arg2) -> readFile(loadFile));
		dataTill = new NumbField("350");
		dataTill.textProperty().addListener((ChangeListener) (arg0, arg1, arg2) -> readFile(loadFile));
		prevData.getChildren().addAll(new Label("Data from: "),
				dataFrom,new Label("Till: "),dataTill);
	}
	
	
	
	public static void updateRMS(double rmsValue) {
		rms.setText("Root Mean Square\n(RMS):\n" + (String.format("%.4f", rmsValue)));
	}
	
	public static void updatePtP(double max, double min) {
		ptp.setText("Peak to Peak: \n" + (String.format("%.4f", (max-min))));
	}
	
	
}

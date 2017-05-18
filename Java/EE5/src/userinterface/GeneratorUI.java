package userinterface;

import communication.Connection;
import javafx.beans.value.ChangeListener;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.Separator;
import javafx.scene.control.Slider;
import javafx.scene.control.Tab;
import javafx.scene.control.ToggleButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import resource.ResourceLoader;

public class GeneratorUI {

	private static ToggleGroup waveType;
	private static ToggleGroup freqGroup;
	
	public static Tab generator() {
		Tab generator = new Tab();
		generator.setText("Function generator");
		ScrollPane genePanel = new ScrollPane();
		genePanel.setFitToHeight(true);
		genePanel.setFitToWidth(true);
		genePanel.setContent(geneBorderPane());
		generator.setContent(genePanel);
		return generator;
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
	
	private static Button help() {
		Button help = new Button("help");
		help.setPrefWidth(120);
		help.setOnMouseClicked(new EventHandler<Event>() {
			@Override
			public void handle(Event e) {
				Alert helpAlert = new Alert(AlertType.INFORMATION);
				helpAlert.setTitle("Help menu");
				helpAlert.setHeaderText("Help with the generator");
				helpAlert.getDialogPane().setPrefHeight(400);
				helpAlert.setResizable(true);
				helpAlert.getDialogPane().setContent(helpContent());

				helpAlert.showAndWait();
			}
		});
		return help;
	}
	
	private static ScrollPane helpContent() {
		ScrollPane helpContent = new ScrollPane();
		helpContent.setFitToWidth(true);
		VBox content = new VBox(10);
		content.getChildren().addAll(new Label("Select waveform, Sine or Square: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("wave.png"),300,0,true,true)), new Separator(), new Label("Select the frequency of the waveform: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("frequency.png"),500,0,true,true)));
		helpContent.setContent(content);
		
		return helpContent;
	}
	
	private static BorderPane geneBody() {
		BorderPane geneBody = new BorderPane();
		VBox generator = new VBox(10);
		generator.setAlignment(Pos.TOP_CENTER);
		
		generator.getChildren().addAll(wave(),freq(),stop());
		geneBody.setCenter(generator);
		return geneBody;
	}
	
	private static HBox wave() {
		HBox wave = new HBox(50);
		wave.setAlignment(Pos.TOP_CENTER);
		RadioButton sine = new RadioButton();
		sine.setGraphic(new ImageView(new Image(ResourceLoader.class.getResourceAsStream("SineWave.png"),100,0,true,false)));
		sine.setUserData("sine");
		RadioButton square = new RadioButton();
		square.setGraphic(new ImageView(new Image(ResourceLoader.class.getResourceAsStream("SquareWave.png"),100,0,true,false)));
		square.setUserData("square");
		waveType = new ToggleGroup();
		sine.setToggleGroup(waveType);
		square.setToggleGroup(waveType);
		waveType.selectToggle(sine);
		sine.setDisable(true);
		waveType.selectedToggleProperty().addListener((obs, oldToggle, newToggle) -> {
				sendWave();
				((ToggleButton) newToggle).setDisable(true);
				((ToggleButton) oldToggle).setDisable(false);
				});
		wave.getChildren().addAll(sine, square);
		return wave;
	}
	
	private static VBox freq() {
		
			Slider freqSlider = new Slider();
			freqSlider.setMin(0);
			freqSlider.setMax(1000);
			freqSlider.setValue(330);
			freqSlider.setShowTickLabels(true);
			freqSlider.setMajorTickUnit(50);
			
			NumbField freqField = new NumbField("330");
	
			ToggleButton  Hz = new ToggleButton ();
			Hz.setText("Hz");
			Hz.setUserData(1);
			Hz.setPrefWidth(120);
			ToggleButton  kHz = new ToggleButton ();
			kHz.setText("kHz");
			kHz.setUserData(1000);
			kHz.setPrefWidth(120);
			ToggleButton  MHz = new ToggleButton ();
			MHz.setText("MHz");
			MHz.setUserData(1000000);
			MHz.setPrefWidth(120);
			freqGroup  = new ToggleGroup();
			Hz.setToggleGroup(freqGroup);
			kHz.setToggleGroup(freqGroup);
			MHz.setToggleGroup(freqGroup);
			freqGroup.selectToggle(Hz);
			Hz.setDisable(true);
			
			freqField.textProperty().addListener((ChangeListener) (arg0, oldValue, newValue) -> {
																		int freqValue = Integer.parseInt((String) newValue);
																		if((int)freqValue > 1000000)
																			freqGroup.selectToggle(MHz);
																		else if ((int) freqValue > 1000)
																			freqGroup.selectToggle(kHz);
																		else
																			freqGroup.selectToggle(Hz);
																		freqSlider.setValue(freqValue / ((int) freqGroup.getSelectedToggle().getUserData()));
																		sendFreq();
			});
			freqSlider.valueProperty().addListener((ChangeListener) (arg0, oldValue, newValue) -> {
																		freqField.textProperty().setValue(String.valueOf(((int) freqSlider.getValue())*((int) freqGroup.getSelectedToggle().getUserData())));
																		sendFreq();
			});
			freqField.setMaxWidth(120);
			
			
			freqGroup.selectedToggleProperty().addListener((obs, oldToggle, newToggle) -> {
																		sendFreq();
																		((ToggleButton) newToggle).setDisable(true);
																		((ToggleButton) oldToggle).setDisable(false);
																		freqField.textProperty().setValue(String.valueOf(((int) freqSlider.getValue())*((int) freqGroup.getSelectedToggle().getUserData())));
																		});
			HBox freqMag = new HBox(10);
			freqMag.setAlignment(Pos.TOP_CENTER);
			freqMag.getChildren().addAll(Hz,kHz,MHz);

		VBox freq = new VBox(20);	
		freq.setAlignment(Pos.TOP_CENTER);
		freq.getChildren().addAll(freqSlider,freqMag, freqField);
		return freq;
	}
	
	private static Button stop() {
		Button stop = new Button("Stop Generator");
		stop.setPrefWidth(120);
		return stop;
	}
	
	public static void sendGenerator() {
		sendWave();
		sendFreq();
	}
	
	private static void sendFreq() {
		byte message = 0b00000000;
		
		Connection.send(message);
		
		
		message = 0b00001000;
		
		Connection.send(message);
		
		
		message = 0b00010000;
		
		Connection.send(message);
		
		
		message = 0b00011000;
		
		Connection.send(message);
		
		
		message = 0b00100000;
		
		Connection.send(message);
		
		message = 0b00110000;
		
		Connection.send(message); 
	}
	
	private static void sendWave() {
		byte message = 0b00111000; //start with wave type
		if(waveType.getSelectedToggle().getUserData().equals("sine")) {
			message = setBit(message,0);
		}
		Connection.send(message);
	}
	
	private static byte setBit(byte message, int pos) {
		return message |= 1 << pos;
	}
	
	private static byte clearBit(byte message, int pos) {
		return message &= ~(1<<pos);
	}
}

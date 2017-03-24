package userinterface;

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
import javafx.scene.control.TextField;
import javafx.scene.control.ToggleGroup;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import resource.ResourceLoader;

public class GeneratorUI {

	
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
		
		HBox wave = new HBox(50);
		wave.setAlignment(Pos.TOP_CENTER);
		RadioButton sine = new RadioButton();
		sine.setGraphic(new ImageView(new Image(ResourceLoader.class.getResourceAsStream("SineWave.png"),100,0,true,false)));
		RadioButton square = new RadioButton();
		square.setGraphic(new ImageView(new Image(ResourceLoader.class.getResourceAsStream("SquareWave.png"),100,0,true,false)));
		
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
		freqField.setMaxWidth(120);
		wave.getChildren().addAll(sine, square);
		generator.getChildren().addAll(wave,freq,freqField);
		geneBody.setCenter(generator);
		return geneBody;
	}
}
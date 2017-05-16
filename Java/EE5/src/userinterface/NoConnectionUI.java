package userinterface;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import javax.imageio.ImageIO;

import javafx.beans.value.ChangeListener;
import javafx.embed.swing.SwingFXUtils;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Orientation;
import javafx.geometry.Pos;
import javafx.scene.chart.LineChart;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.Separator;
import javafx.scene.control.Tab;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.image.WritableImage;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;
import resource.ResourceLoader;

public class NoConnectionUI extends UI{

	private static Label ptpA;
	private static Label rmsA;
	private static Label ptpB;
	private static Label rmsB;
	private static NumbField dataFrom;
	private static NumbField dataTill;
	private static BorderPane noConBody;
	private static File loadFile;
	private static LineChart<Number,Number> graph;
	
	public static Tab noConnection() {
		Tab noConnectionTab = new Tab();
		noConnectionTab.setText("Graph");
		ScrollPane osciPanel = new ScrollPane();
			osciPanel.setFitToHeight(true);
			osciPanel.setFitToWidth(true);
			osciPanel.setContent(noConBorderPane());
			noConnectionTab.setContent(osciPanel);
		return noConnectionTab;
	}
	
	private static BorderPane noConBorderPane() {
		BorderPane osciBorderPane = new BorderPane();
		HBox header = mainButtons();
		osciBorderPane.setTop(header);
		BorderPane.setMargin(header, new Insets(10,0,10,10));
		osciBorderPane.setCenter(noConBody());
		return osciBorderPane;
	}
	
	private static HBox mainButtons() {
		HBox mainButtons = new HBox(10);
		mainButtons.setAlignment(Pos.CENTER_LEFT);
		mainButtons.getChildren().addAll(Load(),printScreen(),help());
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
						readFile(loadFile);
					}
						
				}
			});
			return load;
		}
		
		private static void readFile(File file) {
			GraphUI.clearGraph();
			int from = Integer.parseInt(dataFrom.getText());
			int till = Integer.parseInt(dataTill.getText());
			dataFrom.setEditable(true);
			dataTill.setEditable(true);
			int currentData = from;
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
					GraphUI.addDataA(Double.parseDouble(nextLine),from,(till-from),currentData,GraphUI.NoConUI);
					currentLineNo++;
					currentData++;
				}
			} catch (IOException ex) {
				System.out.println("Problem reading file" + ex.getMessage());
			} finally {
				try {
					if(in!=null)
						in.close();
				} catch(IOException ignore) {
					
				}
			}
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
							//get a snapshot of the oscibody, this includes the different buttons of the oscilloscope and the graph
				            WritableImage snapshot = noConBody.snapshot(null, null);
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
			help.setOnMouseClicked(new EventHandler<Event>() {
				@Override
				public void handle(Event e) {
					Alert helpAlert = new Alert(AlertType.INFORMATION);
					helpAlert.setTitle("Help menu");
					helpAlert.setHeaderText("Help with the Graph");
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
			content.getChildren().addAll(
					new Label("This is the graph of the data: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("graph.png"),300,0,true,true)),
					new Separator(),
					new Label("Open data file to display on the graph: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("open.png"),300,0,true,true)),
					new Separator(),
					new Label("Printscreen the graph and option selection to a PNG image: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("print.png"),300,0,true,true)),
					new Separator(),
					new Label("Display Root Mean Square (RMS) value of the graph: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("RMS.png"),200,0,true,true)),
					new Separator(),
					new Label("Display Peak to Peak value of the graph: "),new ImageView(new Image(ResourceLoader.class.getResourceAsStream("PtP.png"),200,0,true,true)));
			helpContent.setContent(content);
			
			return helpContent;
		}
		
		private static BorderPane noConBody(){
			noConBody = new BorderPane();
			VBox noConButtons = noConButtons();
			noConBody.setRight(noConButtons);
			BorderPane.setMargin(noConButtons, new Insets(10,10,10,10));
			graph = GraphUI.osciGraph();
			noConBody.setCenter(graph);
			return noConBody;
		}
		
		private static VBox noConButtons() {
			VBox noConButtons = new VBox(10);
			noConButtons.getChildren().addAll(dataButtonsA(),new Separator(Orientation.VERTICAL),dataButtonsB());
			return noConButtons;
		}
		
		private static VBox dataButtonsA() {
			VBox dataButtons = new VBox(10);
			dataButtons.getChildren().addAll(rmsA(),ptpA(),prevData());
			return dataButtons;
		}
		
		
		private static VBox dataButtonsB() {
			VBox dataButtons = new VBox(10);
			dataButtons.getChildren().addAll(rmsB(),ptpB(),prevData());
			return dataButtons;
		}
		private static Label rmsA() {
			rmsA = new Label("Root Mean Square\n(RMS): ");
			rmsA.setPrefWidth(120);
			return rmsA;
		}
		
		private static Label ptpA() {
			ptpA = new Label("Peak to Peak: ");
			ptpA.setPrefWidth(120);
			return ptpA;
		}
		
		private static Label rmsB() {
			rmsB = new Label("Root Mean Square\n(RMS): ");
			rmsB.setPrefWidth(120);
			return rmsB;
		}
		
		private static Label ptpB() {
			ptpB = new Label("Peak to Peak: ");
			ptpB.setPrefWidth(120);
			return ptpB;
		}
		
		private static VBox prevData() {
			VBox prevData = new VBox(10);
			dataFrom = new NumbField("0");
			dataFrom.setEditable(false);
			dataFrom.textProperty().addListener((ChangeListener) (arg0, arg1, arg2) -> readFile(loadFile));
			dataTill = new NumbField("350");
			dataTill.setEditable(false);
			dataTill.textProperty().addListener((ChangeListener) (arg0, arg1, arg2) -> readFile(loadFile));
			prevData.getChildren().addAll(new Label("Data from: "),
					dataFrom,new Label("Till: "),dataTill);
			return prevData;
		}
		
		
		
		public static void updateRMSA(double rmsValue) {
			rmsA.setText("Root Mean Square\n(RMS):\n" + (String.format("%.4f", rmsValue)));
		}
		
		public static void updatePtPA(double max, double min) {
			ptpA.setText("Peak to Peak: \n" + (String.format("%.4f", (max-min))));
		}
		
		public static void updateRMSB(double rmsValue) {
			rmsB.setText("Root Mean Square\n(RMS):\n" + (String.format("%.4f", rmsValue)));
		}
		
		public static void updatePtPB(double max, double min) {
			ptpB.setText("Peak to Peak: \n" + (String.format("%.4f", (max-min))));
		}
}
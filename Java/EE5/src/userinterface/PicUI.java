package userinterface;

import java.util.Optional;


import communication.Connection;
import gnu.io.CommPortIdentifier;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.ButtonType;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Dialog;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.scene.control.ButtonBar.ButtonData;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.stage.Modality;
import javafx.util.Callback;

public class PicUI {
	private static ComboBox<CommPortIdentifier> combo;
	public static void start() {
		
		Dialog<ButtonType> portSelect = new Dialog<>();
		combo = new ComboBox<CommPortIdentifier>();
		Connection.refresh();
		Button refreshPort = new Button("Refresh ports");
		
		refreshPort.setOnMouseClicked(new EventHandler<Event>() {
		
			@Override
			public void handle(Event arg0){
				combo.setItems(Connection.refresh());
			}
		});
		
		Button selectPort = new Button("Select port");
		selectPort.setOnMouseClicked(new EventHandler<Event>() {

			@Override
			public void handle(Event arg0) {
				
				if(combo.getValue()!=null) {
					System.out.println(combo.getValue());
					try {
						if(combo.getValue() != null) {
							System.out.println("test");
							Connection connection = new Connection(combo.getValue());
							connection.load();
							portSelect.setResult(ButtonType.FINISH);;
						}
					} catch (Exception e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
				else
					System.out.println(combo.getValue());
				
			}
			
		});
		ButtonType cancel = new ButtonType("Cancel Program",ButtonData.CANCEL_CLOSE);
		
		HBox buttons = new HBox(10);
		buttons.getChildren().addAll(refreshPort,combo, selectPort);



		BorderPane main = new BorderPane();
		main.setBottom(buttons);
		BorderPane.setAlignment(buttons, Pos.CENTER);
		BorderPane.setMargin(buttons, new Insets(10,0,0,0));
		combo.setItems(Connection.refresh());
		//display custom text for the combobox
		combo.setButtonCell((ListCell<CommPortIdentifier>) cellFactory.call(null));
		combo.setCellFactory(cellFactory);
		
		
		
		portSelect.setHeaderText("Choose the port where your pic is connected");
		portSelect.setTitle("Choose port");
		portSelect.setResizable(true);
		portSelect.getDialogPane().setContent(main);
		portSelect.getDialogPane().getButtonTypes().add(cancel);
		portSelect.initModality(Modality.APPLICATION_MODAL);
		Optional<ButtonType> response = portSelect.showAndWait();
		if(cancel.equals(response.get())) {
			System.exit(0);
		}
		
	}
	
	// Change standard value of combobox to portName + portType
	static Callback<ListView<CommPortIdentifier>,ListCell<CommPortIdentifier>> cellFactory = new Callback<ListView<CommPortIdentifier>,ListCell<CommPortIdentifier>>(){
	    @Override
	    public ListCell<CommPortIdentifier> call(ListView<CommPortIdentifier> l){
	        return new ListCell<CommPortIdentifier>(){
	            @Override
	            protected void updateItem(CommPortIdentifier port, boolean empty) {
	                super.updateItem(port, empty);
	                if (port == null || empty) {
	                    setGraphic(null);
	                } else {
	                    setText(port.getName()+"    "+getPortTypeName(port.getPortType()));
	                }
	            }
	        };
	    }
	};
	
	//return string of port type
	static String getPortTypeName ( int portType ){
        switch ( portType )
        {
            case CommPortIdentifier.PORT_I2C:
                return "I2C";
            case CommPortIdentifier.PORT_PARALLEL:
                return "Parallel";
            case CommPortIdentifier.PORT_RAW:
                return "Raw";
            case CommPortIdentifier.PORT_RS485:
                return "RS485";
            case CommPortIdentifier.PORT_SERIAL:
                return "Serial";
            default:
                return "unknown type";
        }
    }
}

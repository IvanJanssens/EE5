package userinterface;

import javafx.scene.control.TextField;

public class NumbField extends TextField{
	public NumbField(String string) {
		super(string);
	}

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
	
}

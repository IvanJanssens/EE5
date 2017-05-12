package userinterface;

import javafx.scene.paint.Color;
import javafx.scene.shape.Polygon;

public class sevenSegments {

    
    private final static int OFF = 0;
    private final static int ON = 1;
    
	private static final int SEGMENT_NUMBER = 7;
	private final static int TOP = 0;
    private final static int TOP_RIGHT = 1;
    private final static int BOTTOM_RIGHT = 2;
    private final static int BOTTOM = 3;
    private final static int BOTTOM_LEFT = 4;
    private final static int TOP_LEFT = 5;
    private final static int MIDDLE = 6;
    
    private final static int zero[] = { ON, ON, ON, ON, ON, ON, OFF };
    private final static int one[] = { OFF, ON, ON, OFF, OFF, OFF, OFF };
    private final static int two[] = { ON, ON, OFF, ON, ON, OFF, ON };
    private final static int three[] = { ON, ON, ON, ON, OFF, OFF, ON };
    private final static int four[] = { OFF, ON, ON, OFF, OFF, ON, ON };
    private final static int five[] = { ON, OFF, ON, ON, OFF, ON, ON };
    private final static int six[] = { ON, OFF, ON, ON, ON, ON, ON };
    private final static int seven[] = { ON, ON, ON, OFF, OFF, OFF, OFF };
    private final static int eight[] = { ON, ON, ON, ON, ON, ON, ON };
    private final static int nine[] = { ON, ON, ON, ON, OFF, ON, ON };
    private final static int err[] = {OFF, OFF, OFF, OFF, OFF, OFF, ON};

    private final static Color off = Color.RED.darker().darker().darker().darker();
    private final static Color on = Color.RED.brighter().brighter().brighter().brighter().brighter();
    
	private Polygon[][] segments;
	private Polygon[] dots;
	private int[][] number;

	public sevenSegments() {
		segments = new Polygon[4][SEGMENT_NUMBER];
		dots = new Polygon[3];
		number = new int[4][];
		createDots(dots, 0, 0);
		for(int i= 0; i < 4; i++) {
			createSegments(segments[i],i*110+10,0);
		}
	}
	
	public Polygon[][] getSegments() {
		return segments;
	}
	public Polygon[] getDots() {
		return dots;
	}
	
	public void start() {
		dots[0].setFill(off);
		dots[1].setFill(off);
		dots[2].setFill(off);
		for(int i = 0; i < 4; i++) {
			number[i] = err;
			for (int j=0; j<SEGMENT_NUMBER; j++)
	            setSegmentState(segments[i][j], number[i][j]);
		}
	}
	
	public void writeNumber(double data) {
		String dataString = String.format("%.4f", data);
		System.out.println(dataString);
		if(dataString.startsWith("0")) {
			dataString = dataString.substring(2);
		}
		if(dataString.indexOf(",") == 1) {
			dots[0].setFill(on);
			dots[1].setFill(off);
			dots[2].setFill(off);
		}
		else if (dataString.indexOf(",") == 3){
			dots[0].setFill(off);
			dots[1].setFill(on);
			dots[2].setFill(off);
		}
		else {
			dots[0].setFill(off);
			dots[1].setFill(off);
			dots[2].setFill(on);
		}
		dataString = dataString.replace(",", "");
		for(int i = 0; i < 4; i++) {
			switch (dataString.charAt(i)) {
				 case '0':
		             number[i] = zero;
		             break;
		         case '1':
		        	 number[i] = one;
		             break;
		         case '2':
		        	 number[i] = two;
		             break;
		         case '3':
		        	 number[i] = three;
		             break;
		         case '4':
		        	 number[i] = four;
		             break;
		         case '5':
		        	 number[i] = five;
		             break;
		         case '6':
		        	 number[i] = six;
		             break;
		         case '7':
		        	 number[i] = seven;
		             break;
		         case '8':
		        	 number[i] = eight;
		             break;
		         case '9':
		        	 number[i] = nine;
		             break;
		         default: /* other number */
		        	 number[i] = zero;
		             break;
			}
			for (int j=0; j<SEGMENT_NUMBER; j++)
	            setSegmentState(segments[i][j], number[i][j]);
		}
	}
	
	private void createDots(Polygon[] dot, double x, double y) {
		dot[0] = new Polygon();
		dot[0].getPoints().addAll(new Double [] {
				x+120 ,y+155,
				x+111, y+162,
				x+120, y+169,
				x+129, y+162
		});
		x += 110;
		dot[1] = new Polygon();
		dot[1].getPoints().addAll(new Double [] {
				x+120 ,y+155,
				x+111, y+162,
				x+120, y+169,
				x+129, y+162
		});
		x += 110;
		dot[2] = new Polygon();
		dot[2].getPoints().addAll(new Double [] {
				x+120 ,y+155,
				x+111, y+162,
				x+120, y+169,
				x+129, y+162
		});
	}
	
	private void createSegments(Polygon[] segments, double x, double y) {
		
		segments[TOP] = new Polygon();
		segments[TOP].getPoints().addAll(new Double[] {
				x+20, y+8,
				x+90, y+8,
				x+98, y+15,
		        x+90, y+22,
		        x+20, y+22,
		        x+12, y+15
		});
		segments[TOP_RIGHT] = new Polygon();
		segments[TOP_RIGHT].getPoints().addAll(new Double[] {
				x+91, y+23,
				x+98, y+18,
				x+105, y+23,
		        x+105, y+81,
		        x+98, y+89,
		        x+91, y+81
		});
		segments[BOTTOM_RIGHT] = new Polygon();
		segments[BOTTOM_RIGHT].getPoints().addAll(new Double[] {
				x+91, y+97,
				x+98, y+89,
				x+105, y+97,
		        x+105, y+154,
		        x+98, y+159,
		        x+91, y+154
		});
		segments[BOTTOM] = new Polygon();
		segments[BOTTOM].getPoints().addAll(new Double[] {
				x+20, y+155,
				x+90, y+155,
				x+98, y+162,
		        x+90, y+169,
		        x+20, y+169,
		        x+12, y+162
		});
		segments[BOTTOM_LEFT] = new Polygon();
		segments[BOTTOM_LEFT].getPoints().addAll(new Double[] {
				x+5, y+97,
				x+12, y+89,
				x+19, y+97,
		        x+19, y+154,
		        x+12, y+159,
		        x+5, y+154
		});
		segments[TOP_LEFT] = new Polygon();
		segments[TOP_LEFT].getPoints().addAll(new Double[] {
				x+5, y+23,
				x+12, y+18,
				x+19, y+23,
		        x+19, y+81,
		        x+12, y+89,
		        x+5, y+81
		});
		segments[MIDDLE] = new Polygon();
		segments[MIDDLE].getPoints().addAll(new Double[] {
				x+20, y+82,
				x+90, y+82,
				x+95, y+89,
		        x+90, y+96,
		        x+20, y+96,
		        x+15, y+89
		});
	}
		
	private void setSegmentState(Polygon segment, int state) {
		if(state == OFF) segment.setFill(off);
		else segment.setFill(on);		
	}
}

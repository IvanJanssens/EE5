package userinterface;


import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.ValueAxis;
import javafx.scene.chart.XYChart;

public class GraphUI {

	private static XYChart.Series<Number, Number> dataA;
	private static XYChart.Series<Number, Number> dataB;
	private static double max;
	private static double min;
	private static double sum;
	private static int oldMaxData = 0;
	private static int tickCount = 0;
	
	public static final int NoConUI = 0;
	public static final int OsciUI = 1; 
	
	//Linechart
		public static LineChart<Number, Number> osciGraph() {
			NumberAxis xAxis = new NumberAxis();
			NumberAxis yAxis = new NumberAxis();
			xAxis.setLabel("Time (s)");
			yAxis.setLabel("Voltage (V)");
			LineChart<Number, Number> graph = new LineChart<Number, Number>(xAxis,yAxis);
			graph.setTitle("Oscilloscope");
			dataA = new XYChart.Series<Number, Number>();
			dataA.setName("Oscilloscope 1");
			dataB = new XYChart.Series<Number, Number>();
			dataB.setName("Oscilloscope 2");
			graph.getData().add(dataA);
			graph.getData().add(dataB);
			graph.getXAxis().setAutoRanging(false);
			graph.getYAxis().setAutoRanging(false);
			graph.setMinWidth(400);
			graph.setMaxWidth(1000);
			graph.setAnimated(false);
			((ValueAxis<Number>) graph.getYAxis()).setUpperBound(5.5);
			((ValueAxis<Number>) graph.getXAxis()).setLowerBound(0);
	        ((ValueAxis<Number>) graph.getXAxis()).setUpperBound(UI.MAX_DATA);
			return graph;
		}
		
		public static void addDataA(double newPoint,int startPoint, int maxData, int currentPoint, int tab) {
			if(startPoint == currentPoint)
				tickCount = 0;
			//get number of datapoints
	        int numOfPoint = dataA.getData().size();
	        

			if(maxData < oldMaxData) {
				dataA.getData().remove(maxData, dataA.getData().size());
			}

			((ValueAxis<Number>) dataA.getChart().getXAxis()).setLowerBound(startPoint);
	        ((ValueAxis<Number>) dataA.getChart().getXAxis()).setUpperBound(startPoint + maxData);
	        
			if(numOfPoint >= maxData && tickCount < maxData) {
				dataA.getData().set(tickCount, new XYChart.Data<Number, Number>(currentPoint,newPoint)); // add new datapoint
			}
			else if(numOfPoint <= maxData && tickCount <= maxData){
				dataA.getData().add(new XYChart.Data<Number, Number>(currentPoint,newPoint)); // add new datapoint
			}
			

			
			if(tickCount == maxData){
				min = (double) dataA.getData().get(0).getYValue();
				max = (double) dataA.getData().get(0).getYValue();
				sum = 0;
				dataA.getData().forEach(value-> {
					sum += ((double) value.getYValue()*(double) value.getYValue());
					if((double) value.getYValue() > max)
						max = (double)value.getYValue();
					if((double) value.getYValue() < min)
						min = (double)value.getYValue();
				});
				if(tab == NoConUI) {
					NoConnectionUI.updateRMSA(Math.sqrt(sum/maxData));
					NoConnectionUI.updatePtPA(max,min);
				}
				else if(tab == OsciUI) {
					OscilloscopeUI.updateRMSA(Math.sqrt(sum/maxData));
					OscilloscopeUI.updatePtPA(max,min);
				}
			}
			oldMaxData = maxData;
			tickCount++;
		}
		
		public static void clearGraph() {
			dataA.getData().clear();
			dataB.getData().clear();
		}
		
//		private static LineChart<Number, Number> fftGraph() {
//			NumberAxis xAxis = new NumberAxis();
//			NumberAxis yAxis = new NumberAxis();
//			xAxis.setLabel("Time (s)");
//			yAxis.setLabel("Voltage (V)");
//			LineChart<Number, Number> graph = new LineChart<Number, Number>(xAxis,yAxis);
//			graph.setTitle("Oscilloscope");
//			fftData = new XYChart.Series<Number, Number>();
//			fftData.setName("FFT values");
//			fftDatapoint = 0;
//			graph.getData().add(fftData);
//			graph.getXAxis().setAutoRanging(false);
//			graph.getYAxis().setAutoRanging(false);
//			graph.setMinWidth(400);
//			graph.setMaxWidth(1000);
//			graph.setAnimated(false);
//			((ValueAxis<Number>) graph.getYAxis()).setUpperBound(5.5);
//			return graph;
//		}
//
//		public static void addFftData(float[] fftzeropad, float max) {
//			double newPoint;
//			for(int i = 0; i< fftzeropad.length; i++) {
//				newPoint = (double) (fftzeropad[i]/max*3.2);
//				//get number of datapoints
//		        int numOfPoint = fftData.getData().size();
//				if(fftDatapoint >= 4*max_data && newPoint > triggerValue && prevFftValue <= triggerValue) {
//					fftDatapoint = 0;
//					
//				}
//				
//				((ValueAxis<Number>) fftData.getChart().getXAxis()).setLowerBound(0);
//		        ((ValueAxis<Number>) fftData.getChart().getXAxis()).setUpperBound(4*max_data);
//		        if(numOfPoint >= 4*max_data && fftDatapoint < 4*max_data)
//		        	fftData.getData().set(fftDatapoint, new XYChart.Data<Number, Number>(fftDatapoint,newPoint)); // add new datapoint
//		        else if(fftDatapoint < 4*max_data)
//		        	fftData.getData().add(new XYChart.Data<Number, Number>(fftDatapoint,newPoint)); // add new datapoint
//		        fftDatapoint += 1;
//		        prevFftValue = newPoint;
//			}
//			
//		}
}
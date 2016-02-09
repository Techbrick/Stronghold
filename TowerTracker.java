

import java.util.ArrayList;
import java.util.Iterator;

import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;
import org.opencv.videoio.VideoCapture;

import edu.wpi.first.wpilibj.networktables.NetworkTable;

/**
 * 
 * @author Elijah Kaufman
 * @version 1.0
 * @description Uses opencv and network table 3.0 to detect the vision targets
 *
 */
public class TowerTracker {
	//This is not TJ code :(
	//But it is modified by TJ :D

	/**
	 * static method to load opencv and networkTables
	 */
	static{ 
		System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
		//NetworkTable.setClientMode();
		//NetworkTable.setIPAddress("192.168.1.5");
	}
//	constants for the color RGB values
	public static final Scalar 
	RED = new Scalar(0, 0, 255),
	BLUE = new Scalar(255, 0, 0),
	GREEN = new Scalar(0, 255, 0),
	BLACK = new Scalar(0,0,0),
	YELLOW = new Scalar(0, 255, 255),
//	these are the threshold values in order 
	LOWER_BOUNDS = new Scalar(87,151,28),
	UPPER_BOUNDS = new Scalar(108,255,131);
	
//	the size for resizing the image
	public static final Size resize = new Size(320,240);
	
//	ignore these
	public static VideoCapture videoCapture;
	public static Mat matOriginal, matHSV, matThresh, clusters, matHeirarchy;
	
//	Constants for known variables
//	the height to the top of the target in first stronghold is 97 inches	
	public static final int TOP_TARGET_HEIGHT = 98;
//	the physical height of the camera lens
	public static final int TOP_CAMERA_HEIGHT = 31;
//	projectile speed in mph
	public static final double PROJECTILE_SPEED = 24;
	
//	camera details, can usually be found on the datasheets of the camera
	public static final double VERTICAL_FOV  = 33.58;
	public static final double HORIZONTAL_FOV  = 59.78;
	public static final double CAMERA_ANGLE = 26.5;
	
	public static NetworkTable table;
	static boolean networkAvailible = false;
	
	static boolean production = false;
	static int averageInterval = 5;
	
	public static boolean shouldRun = true;

	/**
	 * 
	 * @param args command line arguments
	 * just the main loop for the program and the entry points
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		matOriginal = new Mat();
		matHSV = new Mat();
		matThresh = new Mat();
		clusters = new Mat();
		matHeirarchy = new Mat();
		//NetworkTable.setClientMode();
		//NetworkTable.setIPAddress("192.168.1.5");
		try 
		{
		table = NetworkTable.getTable("datatable");
		networkAvailible = true;
		} catch(java.lang.UnsatisfiedLinkError exception)
		{
			System.out.println("WOOOPSI DERS AN ERROR: " + exception.getMessage());
		networkAvailible = false;}
//		main loop of the program
		while(shouldRun){
			try {
//				opens up the camera stream and tries to load it
				videoCapture = new VideoCapture(0);
//				replaces the ##.## with your team number
//
				videoCapture.open(0);
//				videoCapture.open("C:/Users/David/Pictures/Camera Roll/WIN_20160131_14_58_37_Pro.mp4");
//				Example
//				cap.open("http://10.30.19.11/mjpg/video.mjpg");
//				wait until it is opened
				while(!videoCapture.isOpened()){}
//				time to actually process the acquired images
				processImage();
			} catch (Exception e) {
				e.printStackTrace();
				break;
			}
		}
//		make sure the java process quits when the loop finishes
		videoCapture.release();
		System.exit(0);
	}
	/**
	 * 
	 * reads an image from a live image capture and outputs information to the SmartDashboard or a file
	 */
	public static void processImage(){
		ArrayList<MatOfPoint> contours = new ArrayList<MatOfPoint>();
		double y,targetX,distance,azimuth;
//		frame counter
		int FrameCount = 0;
		//long before = System.currentTimeMillis();
//		only run for the specified time
		//int averageCounter = 0;
		int goodValues = 0;
		double averageAzimuth = 0, averageShootyAngle = 0, averageDistance = 0;
		boolean frameStop = true;
		
		while(frameStop){
			if(FrameCount >= 100 && !production && table.getNumber("stop", 0) != 1)
			{
				frameStop = false;
			}
			
			if ( averageInterval != 0 && FrameCount % averageInterval == 0)
			{
				
				if (goodValues != 0){
				double	averageAzimuthOut = averageAzimuth / goodValues;
				double	averageShootyAngleOut = averageShootyAngle / goodValues;
				double	averageDistanceOut = averageDistance / goodValues;
				System.out.println("Average Azimuth: " + averageAzimuth);
				System.out.println("Average Shooty Angle: " + averageShootyAngleOut);
				System.out.println("Average Distance: " + averageDistanceOut);
				if(networkAvailible)
				{
					table.putNumber("averageAzimuthOut", averageAzimuthOut);
					table.putNumber("averageShootyAngleOut", averageShootyAngleOut);
					table.putNumber("averageDistanceOut", averageDistanceOut);
				}
				}
								goodValues = 0;
				// try that thing later
				averageAzimuth =0;
				averageShootyAngle = 0;
				averageDistance = 0;
			}
			contours.clear();
//			capture from the axis camera
			videoCapture.read(matOriginal);
//			captures from a static file for testing
//			matOriginal = Imgcodecs.imread("someFile.png");
			Imgproc.cvtColor(matOriginal,matHSV,Imgproc.COLOR_BGR2HSV);			
			Core.inRange(matHSV, LOWER_BOUNDS, UPPER_BOUNDS, matThresh);
			Imgproc.findContours(matThresh, contours, matHeirarchy, Imgproc.RETR_EXTERNAL, 
					Imgproc.CHAIN_APPROX_SIMPLE);
//			make sure the contours that are detected are at least 20x20 
//			pixels with an area of 400 and an aspect ration greater then 1
			for (Iterator<MatOfPoint> iterator = contours.iterator(); iterator.hasNext();) {
				MatOfPoint matOfPoint = (MatOfPoint) iterator.next();
				Rect rec = Imgproc.boundingRect(matOfPoint);
					if(rec.height < 25 || rec.width < 25){
						iterator.remove();
					continue;
					}
					float aspect = (float)rec.width/(float)rec.height;
					if(aspect < 1.0)
						iterator.remove();
				}
				for(MatOfPoint mop : contours){
					Rect rec = Imgproc.boundingRect(mop);
					Imgproc.rectangle(matOriginal, rec.br(), rec.tl(), RED);
			}
//			if there is only 1 target, then we have found the target we want
			if(contours.size() == 1){
				Rect rec = Imgproc.boundingRect(contours.get(0));
//				"fun" math brought to you by miss daisy (team 341)!
				y = rec.br().y + rec.height / 2;
				y= -((2 * (y / matOriginal.height())) - 1);
				distance = (TOP_TARGET_HEIGHT - TOP_CAMERA_HEIGHT) / 
						Math.tan((y * VERTICAL_FOV / 2.0 + CAMERA_ANGLE) * Math.PI / 180);
//				angle to target...would not rely on this
				targetX = rec.tl().x + rec.width / 2;
				targetX = (2 * (targetX / matOriginal.width())) - 1;
				azimuth = normalize360(targetX*HORIZONTAL_FOV /2.0 + 0);
//				drawing info on target
				Point center = new Point(rec.br().x-rec.width / 2 - 15,rec.br().y - rec.height / 2);
				Point centerw = new Point(rec.br().x-rec.width / 2 - 15,rec.br().y - rec.height / 2 - 20);
				Imgproc.putText(matOriginal, ""+(int)distance, center, Core.FONT_HERSHEY_PLAIN, 1, RED);
				Imgproc.putText(matOriginal, ""+(int)azimuth, centerw, Core.FONT_HERSHEY_PLAIN, 1, RED);
				double shootyAngle = TowerTracker.GetShootyAngle(distance, (double)TOP_TARGET_HEIGHT, PROJECTILE_SPEED, 0);
				//System.out.println("Angle needed to shoot:" + shootyAngle);
				//System.out.println("Azimuth:" + azimuth);;
				//System.out.println("Distance:" + distance);
				// averaging
				goodValues += 1;
				averageShootyAngle =+ shootyAngle;
				averageAzimuth += azimuth;
				averageDistance += distance;
				//if(networkAvailible)
				//{
				//	table.putNumber("0", shootyAngle);
				//	table.putNumber("1", azimuth);
				//	table.putNumber("2", distance);
				//}
				
			}
//			output an image for debugging
			Imgcodecs.imwrite("output.png", matOriginal);
			FrameCount++;
			System.out.println("Framecount:" + FrameCount);
		}
		shouldRun = false;
	}
	/**
	 * @param angle a nonnormalized angle
	 */
	public static double normalize360(double angle){
		while(angle >= 360.0)
        {
            angle -= 360.0;
        }
        while(angle < 0.0)
        {
            angle += 360.0;
        }
        return angle;
	}
	//this is TJ code ;)
	public static double GetShootyAngle(double distanceToTower, double altitude, double launchSpeed, double thinger)
	{
		//Returns -1  if the distance is no good
		if (distanceToTower <= 0)
		{
			return -1;
		}
		//convert speed to inches per second
		double inchesPerSecond = launchSpeed * 63360 / 3600;
		//convert gravity from meters to inches
		double gravityInches = 9.8 * 39.4;
		//case 1
		double case1 = Math.pow(inchesPerSecond, 4) - gravityInches * (gravityInches * distanceToTower * distanceToTower + 2 * altitude * inchesPerSecond * inchesPerSecond);
		if (case1 < 0)
		{
			return -1;
		}
		double squareroot1 = Math.sqrt(case1);
		double result1 = Math.toDegrees(Math.atan((inchesPerSecond * inchesPerSecond + squareroot1) / (gravityInches * distanceToTower)));
		double result2 = Math.toDegrees(Math.atan((inchesPerSecond * inchesPerSecond - squareroot1) / (gravityInches * distanceToTower)));
		System.out.println("ShootyAngle result1:" + result1);
		System.out.println("ShootyAngle result2:" + result2);
		return result2;
	}
}

#include "EyeCenterDetectionHeaders.h"

//Classes instantiated globally because methods called repetitively inside main loop.
FaceDetection faceDetection;
EyeDetection eyeDetection;
ImageProcessingMethods imageProcessingMethods;
EyeCenterTracker eyeCenterTracker;
Snakuscule snakuscule;
EyeCornerDetector eyeCornerDetector;
EyeGazeEstimator eyeGazeEstimator;
NoseDetector noseDetector;

Point2f TopRight(0, 0);
Point2f TopLeft(0, 0);
Point2f BottomRight(0, 0);
Point2f BottomLeft(0, 0);
float threadReturn[2] = { 0, 0 };
int calibrateStart = 0;
int frameCount = 0;

float horizontalShift = 0, verticalShift = 0;

void aggregateEyeShift()
{
	float horizontalAvg = 0, verticalAvg = 0;
	int count = 0;

	while (calibrateStart == 1)
	{
		horizontalAvg += horizontalShift;
		verticalAvg += verticalShift;
		count++;
		cout << "FrameCount: " << frameCount << " Horizontal Shift: " << horizontalShift << " Vertical Shift: " << verticalShift << endl;
	}
	*threadReturn = horizontalAvg / count;
	*(threadReturn+1) = verticalAvg / count;
	cout << threadReturn[0] << " " << threadReturn[1] << endl;
	cout << "FrameCount: " << frameCount << " Final Value - HorizontalAvg: " << horizontalAvg / count << " VerticalAvg: " << verticalAvg / count << endl;
}

void calibrateEyeGaze()
{
	//Note: Keep head fixed entire time

	//1. Display entire screen
	Mat screenImage(Size(640, 480), CV_8UC3, Scalar(255, 255, 255));
	namedWindow("Calibration Screen", CV_WINDOW_NORMAL);
	setWindowProperty("Calibration Screen", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	imshow("Calibration Screen", screenImage);
	waitKey(0);

	//2. Display circles at 4 corners.
	circle(screenImage, Point(630, 10), 10, Scalar(255, 0, 0), -1);
	imshow("Calibration Screen", screenImage);
	waitKey(1000);
	calibrateStart = 1;
	thread thread1(aggregateEyeShift);
	waitKey(1000);
	calibrateStart = 0;	
	thread1.join(); 
	TopRight.x = threadReturn[0]; TopRight.y = threadReturn[1];

	screenImage.setTo(Scalar(255, 255, 255));
	circle(screenImage, Point(10, 10), 10, Scalar(255, 0, 0), -1);
	imshow("Calibration Screen", screenImage);
	waitKey(1000);
	calibrateStart = 1;
	thread thread2(aggregateEyeShift);
	waitKey(1000);
	calibrateStart = 0;
	thread2.join(); 
	TopLeft.x = threadReturn[0]; TopLeft.y = threadReturn[1];

	screenImage.setTo(Scalar(255, 255, 255));
	circle(screenImage, Point(10, 470), 10, Scalar(255, 0, 0), -1);
	imshow("Calibration Screen", screenImage);
	waitKey(1000);
	calibrateStart = 1;
	thread thread3(aggregateEyeShift);
	waitKey(1000);
	calibrateStart = 0;
	thread3.join();
	BottomLeft.x = threadReturn[0]; BottomLeft.y = threadReturn[1];
	
	screenImage.setTo(Scalar(255, 255, 255));
	circle(screenImage, Point(630, 470), 10, Scalar(255, 0, 0), -1);
	imshow("Calibration Screen", screenImage);
	waitKey(1000);
	calibrateStart = 1;
	thread thread4(aggregateEyeShift);
	waitKey(1000);
	calibrateStart = 0;
	thread4.join();
	BottomRight.x = threadReturn[0]; BottomRight.y = threadReturn[1];
	
	cout << "Top Right: " << TopRight.x << " " << TopRight.y << endl;
	cout << "Top Left: " << TopLeft.x << " " << TopLeft.y << endl;
	cout << "Bottom left: " << BottomLeft.x << " " << BottomLeft.y << endl;
	cout << "Bottom Right: " << BottomRight.x << " " << BottomRight.y << endl;

	screenImage.setTo(Scalar(255, 255, 255));
	
	//3. Record the eye shift in all 4 locations

	//4. Linearly interpolate eye shifts to find gaze position

}

int main()
{
	//VideoCapture capture(0); //Webcam Input
	VideoCapture capture("videos//2015-10-15-15-14.avi");	//Talking Face Database
	//VideoCapture capture("D:/Face Detection Databases/BioID/BioID_%d.jpg"); //BioID Database

	if (!capture.isOpened())
	{
		cout << "Camera/File not found" << endl;
		cin.get();
	}
	
	//Calibrate begin

	
	//thread threadCalib(calibrateEyeGaze);
	//threadCalib.join();

	//Calibrate end

	clock_t  start;
	Mat frame;

	while (true)
	{	
		frameCount++;
		start = clock();

		//Capture frames
		if (!capture.read(frame)) 
		{
			cout << "Could not capture frames" << endl; cin.get();
			break;
		}
		
		//pyrUp(frame, frame, Size());

		//Limit frame size to maximum 640 pixels width.
		if (frame.cols > 640);
			frame = imageProcessingMethods.sizeReduce(frame, 640);
			
		//Call the procedure on each frame		
		detectInImage(frame);				
				
		//Display output detections
		imshow("Output", frame);
		//waitKey(0);

		//destroyAllWindows();
		if (waitKey(5) == 27)
			break;		

		//cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
	}	
	
	capture.release();
}
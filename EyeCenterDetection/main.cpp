#include "EyeCenterDetectionHeaders.h"

//Classes instantiated globally because methods called repetitively inside main loop.
FaceDetection faceDetection;
EyeDetection eyeDetection;
ImageProcessingMethods imageProcessingMethods;
EyeCenterTracker eyeCenterTracker;
Snakuscule snakuscule;
EyeCornerDetector eyeCornerDetector;
EyeGazeEstimator eyeGazeEstimator;

int main()
{
	//VideoCapture capture(0); //Webcam Input
	VideoCapture capture("videos//talking_face.avi");	//Talking Face Database
	//VideoCapture capture("D:/Face Detection Databases/BioID/BioID_%d.jpg"); //BioID Database
	if (!capture.isOpened())
	{
		cout << "Camera/File not found" << endl;
		cin.get();
	}
	
	clock_t  start;
	Mat frame;

	while (true)
	{	
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
		if (waitKey(1) == 27)
			break;		

		cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
	}	
}
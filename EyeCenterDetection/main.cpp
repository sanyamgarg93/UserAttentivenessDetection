#include "EyeCenterDetectionHeaders.h"

//Classes instantiated globally because methods called repetitively inside main loop.
FaceDetection faceDetection;
EyeDetection eyeDetection;
ImageProcessingMethods imageProcessingMethods;
EyeCenterTracker eyeCenterTracker;
Snakuscule snakuscule;

int main()
{
	//VideoCapture capture(0);
	VideoCapture capture("talking_face.avi");
	if (!capture.isOpened())
	{
		cout << "Camera/File not found" << endl;
		cin.get();
	}
	
	clock_t  start;
	Mat frame;

	while (true)
	{		
		//Capture frames
		if (!capture.read(frame)) {
			cout << "Could not capture frames" << endl;
			break;
		}

		//Limit frame size to maximum 640 pixels width.
		if (frame.cols > 640);
			frame = imageProcessingMethods.sizeReduce(frame, 640);
			
		//Call the procedure on each frame
		start = clock();
		detectInImage(frame);		
		cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;

		//Display output detections
		imshow("Output", frame);
		if (waitKey(10) == 27)
			break;		
	}	
}
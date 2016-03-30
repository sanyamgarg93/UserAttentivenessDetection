#include "EyeCenterDetectionHeaders.h"

//Classes instantiated globally because methods called repetitively inside main loop.
FaceDetection faceDetection;
EyeDetection eyeDetection;
ImageProcessingMethods imageProcessingMethods;
EyeCenterTracker eyeCenterTracker;

int main()
{
	VideoCapture capture(0);
	//VideoCapture capture("talking_face.avi");
	if (!capture.isOpened())
	{
		cout << "Camera not found [Camera Initialization Error]" << endl;
		cin.get();
	}
	
	clock_t  start;
	Mat frame;

	while (true)
	{
		if (!capture.read(frame)) 
		{
			cout << "Frames not found [Frame Capture Error]" << endl;
			break;
		}

		start = clock();
		detectInImage(frame);		
		std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

		imshow("Output", frame);

		if (waitKey(10) == 27)
			break;
	}	
}
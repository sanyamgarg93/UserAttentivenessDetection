#include "EyeCenterDetectionHeaders.h"

//Classes instantiated globally because methods called repetitively inside main loop.
FaceDetection faceDetection;
EyeDetection eyeDetection;
ImageProcessingMethods imageProcessingMethods;

int main()
{
	VideoCapture capture(0);
	if (!capture.isOpened())
	{
		cout << "Camera not found [Camera Initialization Error]" << endl;
		cin.get();
	}

	Mat frame;

	while (true)
	{
		if (!capture.read(frame)) 
		{
			cout << "Frames not found [Frame Capture Error]" << endl;
			break;
		}

		detectInImage(frame);		
		imshow("Output", frame);

		if (waitKey(10) == 27)
			break;
	}	
}
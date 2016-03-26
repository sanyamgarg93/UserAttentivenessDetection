#include "EyeCenterDetectionHeaders.h"

int main()
{
	VideoCapture capture(0);
	if (!capture.isOpened())
	{
		cout << "Camera not found [Camera Initialization Error]" << endl;
		cin.get();
	}

	FaceDetection faceDetection;
	Mat frame;

	while (1)
	{
		if (!capture.read(frame))
		{
			cout << "Frames not found [Frame Capture Error]" << endl;
			break;
		}

		vector<Rect> frontalFaces = faceDetection.storeFrontalFacePos(frame);
		faceDetection.drawFaceOnImage(frame, frontalFaces);
		
		imshow("Output", frame);

		if (waitKey(10) == 27)
			break;
	}	
}
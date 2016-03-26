#include "EyeCenterDetectionHeaders.h"

int main()
{
	VideoCapture capture(0);
	if (!capture.isOpened())
	{
		cout << "Camera not found [Camera Initialization Error]" << endl;
		cin.get();
	}

	Mat frame;

	while (1)
	{
		if (!capture.read(frame))
		{
			cout << "Frames not found [Frame Capture Error]" << endl;
			break;
		}

		imshow("Output", frame);

		if (waitKey(10) == 27)
			break;
	}	
}
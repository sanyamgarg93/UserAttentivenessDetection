#include "EyeCenterDetectionHeaders.h"

Mat ImageProcessingMethods::sizeReduce(Mat image, int detectionWidth)
{
	Mat sizeReducedImage;
	
	float scale = float(image.cols) / detectionWidth;
	if (image.cols > detectionWidth)
	{
		int scaledHeight = cvRound(image.rows / scale);
		resize(image, sizeReducedImage, Size(detectionWidth, scaledHeight));
	}
	else
	{
		image.copyTo(sizeReducedImage);
	}

	return sizeReducedImage;
}

Mat ImageProcessingMethods::RGB2GRAY(Mat image)
{
	Mat gray(Size(image.cols, image.rows), CV_8U);
	if (image.channels() == 3)
	{
		cvtColor(image, gray, CV_BGR2GRAY);		
	}
	else if (image.channels() == 4) 
	{
		cvtColor(image, gray, CV_BGRA2GRAY);
	}
	else // Access the grayscale input image directly.
	{		
		gray = image; 
	}
	return gray;
}
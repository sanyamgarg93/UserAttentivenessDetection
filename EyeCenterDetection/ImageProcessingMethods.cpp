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
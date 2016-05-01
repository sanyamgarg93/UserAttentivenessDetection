#include "EyeCenterDetectionHeaders.h"

ImageProcessingMethods::ImageProcessingMethods()
{
	histR[256] = { 0 };
	cumHistR[256] = { 0 };
}

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

Mat ImageProcessingMethods::AdaptiveHistThresh(Mat imageRGB, float thresh)
{
	//Convert to grayscale
	Mat image = imageProcessingMethods.RGB2GRAY(imageRGB);

	//Step-1: Create histogram of the eye region

	memset(histR, 0, sizeof(histR));
	//int histR[256] = { 0 };
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			histR[image.at<uchar>(i, j)]++;
		}
	}

	//Step-2: Create cumulative histograms

	memset(cumHistR, 0, sizeof(cumHistR));
	//float cumHistR[256] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		int sum = 0;
		for (int j = 0; j <= i; j++)
		{
			sum = sum + histR[j];
		}
		cumHistR[i] = sum;
	}

	//Step-3: Normalize cumulative histogram within 0.0 to 1.0

	int maxNew = cumHistR[255];

	for (int i = 0; i < 256; i++)
		cumHistR[i] = float(cumHistR[i]) / maxNew;

	//Step-4: Threshold image based on histogram

	Mat imageTrial(Size(image.cols, image.rows), CV_8U);

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			if (cumHistR[image.at<uchar>(i, j)] <= thresh)
				imageTrial.at<uchar>(i, j) = 255;
			else
				imageTrial.at<uchar>(i, j) = 0;
		}
	}

	return imageTrial;
}
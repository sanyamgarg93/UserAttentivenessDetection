#include "EyeCenterDetectionHeaders.h"
#include "constants.h"

/*
* Method to estimate the eye center position using the method described in
* "Automatic Adaptive Center of Pupil Detection Using Face Detection and CDF Analysis"
* that uses adaptive thresholding based on normalized cumulative histogram
*/
EyeCenterTracker::EyeCenterTracker()
{
	histR[256] = { 0 };
	float cumHistR[256] = { 0 };
	Point centerWrtEyeImage; Point centerWrtFrame;
}

Point EyeCenterTracker::estimateEyeCenter(Mat eyeImageRGB, string eyeCaption)
{
	//Convert to grayscale
	Mat eyeImage = imageProcessingMethods.RGB2GRAY(eyeImageRGB);
	
	//Step-1: Create histogram of the eye region
	
	memset(histR, 0, sizeof(histR));
	//int histR[256] = { 0 };
	for (int i = 0; i < eyeImage.rows; i++)
	{
		for (int j = 0; j < eyeImage.cols; j++)
		{
			histR[eyeImage.at<uchar>(i, j)]++;
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

	Mat eyeImageTrial(Size(eyeImage.cols, eyeImage.rows), CV_8U);

	for (int i = 0; i < eyeImage.rows; i++)
	{
		for (int j = 0; j < eyeImage.cols; j++)
		{
			if (cumHistR[eyeImage.at<uchar>(i, j)] <= 0.05)
				eyeImageTrial.at<uchar>(i, j) = 255;
			else
				eyeImageTrial.at<uchar>(i, j) = 0;
		}
	}

	//Step-5: Choose the darkest pixel among thresholded pixels
	
	erode(eyeImageTrial, eyeImageTrial, Mat());
	
	int min = 255, xPos = 0, yPos = 0;
	for (int i = 0; i < eyeImageTrial.rows; i++)
	{	
		for (int j = eyeImageTrial.cols * 5 / 100; j < 95 * eyeImageTrial.cols / 100; j++)
		{
			if (eyeImageTrial.at<uchar>(i, j) == 255 && eyeImage.at<uchar>(i, j) < min)
			{
				min = eyeImage.at<uchar>(i, j);
				xPos = j;
				yPos = i;
			}
		}
	}

	//Check extreme incorrect detections
	if (xPos == 0 || yPos == 0 || xPos == eyeImageTrial.cols || yPos == eyeImageTrial.rows)
	{
		xPos = eyeImage.cols / 2;
		yPos = eyeImage.rows / 2;
	}

	//Point center; 
	centerWrtEyeImage.x = xPos; centerWrtEyeImage.y = yPos;
	return centerWrtEyeImage;
}

Point EyeCenterTracker::drawLeftEyeCenter(Mat frame, Rect facePos, Rect eyePos, Point eyeCenterPos)
{		
	centerWrtFrame.x = facePos.x + EYE_SX*facePos.width + eyePos.x + eyeCenterPos.x;
	centerWrtFrame.y = facePos.y + EYE_SY*facePos.height + eyePos.y + eyeCenterPos.y;

	//line(frame, Point(centerWrtFrame.x - 0.30*facePos.width / 15, centerWrtFrame.y), Point(centerWrtFrame.x + 0.30*facePos.width / 15, centerWrtFrame.y), Scalar(255, 255, 255), 1, 8);
	//line(frame, Point(centerWrtFrame.x, centerWrtFrame.y - 0.30*facePos.height / 15), Point(centerWrtFrame.x, centerWrtFrame.y + 0.30*facePos.height / 15), Scalar(255, 255, 255), 1, 8);

	return centerWrtFrame;
}

Point EyeCenterTracker::drawRightEyeCenter(Mat frame, Rect facePos, Rect eyePos, Point eyeCenterPos)
{
	centerWrtFrame.x = facePos.x + (1 - EYE_SX - EYE_SW)*facePos.width + eyePos.x + eyeCenterPos.x;
	centerWrtFrame.y = facePos.y + EYE_SY*facePos.height + eyePos.y + eyeCenterPos.y;

	//line(frame, Point(centerWrtFrame.x - 0.30*facePos.width / 15, centerWrtFrame.y), Point(centerWrtFrame.x + 0.30*facePos.width / 15, centerWrtFrame.y), Scalar(255, 255, 255), 1, 8);
	//line(frame, Point(centerWrtFrame.x, centerWrtFrame.y - 0.30*facePos.height / 15), Point(centerWrtFrame.x, centerWrtFrame.y + 0.30*facePos.height / 15), Scalar(255, 255, 255), 1, 8);		

	return centerWrtFrame;
}

Point EyeCenterTracker::drawLeftEyeCenter(Mat frame, Point eyeCenterPos, int radius)
{
	//circle(frame, eyeCenterPos, radius, Scalar(0, 255, 0), 1, 8);

	line(frame, Point(eyeCenterPos.x - radius, eyeCenterPos.y), Point(eyeCenterPos.x + radius, eyeCenterPos.y), Scalar(0, 255, 0), 1, 8);
	line(frame, Point(eyeCenterPos.x, eyeCenterPos.y - radius), Point(eyeCenterPos.x, eyeCenterPos.y + radius), Scalar(0, 255, 0), 1, 8);

	return eyeCenterPos;
}

Point EyeCenterTracker::drawRightEyeCenter(Mat frame, Point eyeCenterPos, int radius)
{
	//circle(frame, eyeCenterPos, radius, Scalar(0, 255, 0), 1, 8);

	line(frame, Point(eyeCenterPos.x - radius, eyeCenterPos.y), Point(eyeCenterPos.x + radius, eyeCenterPos.y), Scalar(0, 255, 0), 1, 8);
	line(frame, Point(eyeCenterPos.x, eyeCenterPos.y - radius), Point(eyeCenterPos.x, eyeCenterPos.y + radius), Scalar(0, 255, 0), 1, 8);

	return eyeCenterPos;
}
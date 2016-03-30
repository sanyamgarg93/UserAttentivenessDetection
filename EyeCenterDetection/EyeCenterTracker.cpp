#include "EyeCenterDetectionHeaders.h"
#include "constants.h"

/*
* Method to estimate the eye center position using the method described in
* "Automatic Adaptive Center of Pupil Detection Using Face Detection and CDF Analysis"
* that uses adaptive thresholding based on normalized cumulative histogram
*/
Point EyeCenterTracker::estimateEyeCenter(Mat eyeImageRGB, string eyeCaption)
{
	//Convert to grayscale
	Mat eyeImage = imageProcessingMethods.RGB2GRAY(eyeImageRGB);

	//namedWindow("Grayscale " + eyeCaption, CV_WINDOW_AUTOSIZE);
	//imshow("Grayscale " + eyeCaption, eyeImage);

	//Step-1: Create histogram of the eye region
	
	int histR[256] = { 0 };
	for (int i = 0; i < eyeImage.rows; i++)
	{
		for (int j = 0; j < eyeImage.cols; j++)
		{
			histR[eyeImage.at<uchar>(i, j)]++;
		}
	}

	//Step-2: Create cumulative histograms

	float cumHistR[256] = { 0 };
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
	
	//namedWindow("Eroded eye image " + eyeCaption, CV_WINDOW_AUTOSIZE);
	//imshow("Eroded eye image " + eyeCaption, eyeImageTrial);
	erode(eyeImageTrial, eyeImageTrial, Mat());
	
	int min = 255, xPos = 0, yPos = 0;
	for (int i = 0; i < eyeImageTrial.rows; i++)
	{
		//for (int j = 0; j < eyeImageTrial.cols; j++)
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

	Point center; center.x = xPos; center.y = yPos;
	return center;
}

void EyeCenterTracker::drawLeftEyeCenter(Mat frame, Rect facePos, Rect eyePos, Point eyeCenterPos)
{
	circle(
			frame, 
			Point(facePos.x + EYE_SX*facePos.width + eyePos.x + eyeCenterPos.x, facePos.y + EYE_SY*facePos.height + eyePos.y + eyeCenterPos.y),
			3, 
			Scalar(255, 255, 255), 
			1
		  );
}

void EyeCenterTracker::drawRightEyeCenter(Mat frame, Rect facePos, Rect eyePos, Point eyeCenterPos)
{
	circle(
			frame,
			Point(facePos.x + (1 - EYE_SX - EYE_SW)*facePos.width + eyePos.x + eyeCenterPos.x, facePos.y + EYE_SY*facePos.height + eyePos.y + eyeCenterPos.y),
			3,
			Scalar(255, 255, 255),
			1
		  );
}
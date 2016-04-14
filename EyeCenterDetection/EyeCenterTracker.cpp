#include "EyeCenterDetectionHeaders.h"
#include "constants.h"

/*
* Method to estimate the eye center position using the method described in
* "Automatic Adaptive Center of Pupil Detection Using Face Detection and CDF Analysis"
* that uses adaptive thresholding based on normalized cumulative histogram
*/
EyeCenterTracker::EyeCenterTracker()
{
	Point centerWrtEyeImage, centerWrtFrame;
}

Point EyeCenterTracker::estimateEyeCenter(Mat eyeImageRGB, string eyeCaption)
{
	Mat eyeImage = imageProcessingMethods.RGB2GRAY(eyeImageRGB);
	Mat eyeImageTrial = imageProcessingMethods.AdaptiveHistThresh(eyeImage, 0.05);
		
	erode(eyeImageTrial, eyeImageTrial, Mat());
	
	int min = 255, xPos = 0, yPos = 0;
	for (int i = eyeImageTrial.rows * 5 / 100; i < 95 * eyeImageTrial.rows / 100; i++)
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
	centerWrtFrame.y = facePos.y + EYE_SY*facePos.height * (2-EW_RATIO) + eyePos.y * (2-EW_RATIO) + eyeCenterPos.y;

	//line(frame, Point(centerWrtFrame.x - 0.30*facePos.width / 15, centerWrtFrame.y), Point(centerWrtFrame.x + 0.30*facePos.width / 15, centerWrtFrame.y), Scalar(255, 255, 255), 1, 8);
	//line(frame, Point(centerWrtFrame.x, centerWrtFrame.y - 0.30*facePos.height / 15), Point(centerWrtFrame.x, centerWrtFrame.y + 0.30*facePos.height / 15), Scalar(255, 255, 255), 1, 8);

	return centerWrtFrame;
}

Point EyeCenterTracker::drawRightEyeCenter(Mat frame, Rect facePos, Rect eyePos, Point eyeCenterPos)
{
	centerWrtFrame.x = facePos.x + (1 - EYE_SX - EYE_SW)*facePos.width + eyePos.x + eyeCenterPos.x;
	centerWrtFrame.y = facePos.y + EYE_SY*facePos.height * (2-EW_RATIO) + eyePos.y * (2-EW_RATIO) + eyeCenterPos.y;

	//line(frame, Point(centerWrtFrame.x - 0.30*facePos.width / 15, centerWrtFrame.y), Point(centerWrtFrame.x + 0.30*facePos.width / 15, centerWrtFrame.y), Scalar(255, 255, 255), 1, 8);
	//line(frame, Point(centerWrtFrame.x, centerWrtFrame.y - 0.30*facePos.height / 15), Point(centerWrtFrame.x, centerWrtFrame.y + 0.30*facePos.height / 15), Scalar(255, 255, 255), 1, 8);		

	return centerWrtFrame;
}

Point EyeCenterTracker::drawLeftEyeCenter(Mat frame, Point eyeCenterPos, int radius)
{
	//circle(frame, eyeCenterPos, radius, Scalar(0, 255, 0), 1, 8);

	line(frame, Point(eyeCenterPos.x - radius/2, eyeCenterPos.y), Point(eyeCenterPos.x + radius/2, eyeCenterPos.y), Scalar(0, 255, 0), 1, 8);
	line(frame, Point(eyeCenterPos.x, eyeCenterPos.y - radius/2), Point(eyeCenterPos.x, eyeCenterPos.y + radius/2), Scalar(0, 255, 0), 1, 8);

	return eyeCenterPos;
}

Point EyeCenterTracker::drawRightEyeCenter(Mat frame, Point eyeCenterPos, int radius)
{
	//circle(frame, eyeCenterPos, radius, Scalar(0, 255, 0), 1, 8);

	line(frame, Point(eyeCenterPos.x - radius/2, eyeCenterPos.y), Point(eyeCenterPos.x + radius/2, eyeCenterPos.y), Scalar(255, 255, 255), 1, 8);
	line(frame, Point(eyeCenterPos.x, eyeCenterPos.y - radius/2), Point(eyeCenterPos.x, eyeCenterPos.y + radius/2), Scalar(255, 255, 255), 1, 8);

	return eyeCenterPos;
}
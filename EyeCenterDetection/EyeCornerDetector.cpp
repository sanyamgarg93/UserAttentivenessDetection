#include "EyeCenterDetectionHeaders.h"
#include "constants.h"

EyeCornerDetector::EyeCornerDetector()
{
	eyeCornerThresh = 0.30;

	int erosion_type = 0, erosion_elem = 0, erosion_size = 1;

	if (erosion_elem == 0) erosion_type = MORPH_RECT; 
	else if (erosion_elem == 1)	erosion_type = MORPH_CROSS; 
	else if (erosion_elem == 2)	erosion_type = MORPH_ELLIPSE; 

	erosionElement = getStructuringElement(erosion_type, Size(2 * erosion_size + 1, 2 * erosion_size + 1),	Point(erosion_size, erosion_size));	
}

Point EyeCornerDetector::returnLeftCornerPos(Mat eyeImage, string caption)
{
	Mat leftCornerImage = eyeImage(Rect(
										eyeImage.cols*CORNER_X, //Left
										eyeImage.rows*CORNER_Y, //Top
										eyeImage.cols*CORNER_W, //Width
										eyeImage.rows*(1-2*CORNER_Y) //height
										));
	
	Mat leftCornerGray = imageProcessingMethods.RGB2GRAY(leftCornerImage);
	blur(leftCornerGray, leftCornerGray, Size(3, 3));

	//namedWindow("Eye: " + caption + " Corner: L Color", CV_WINDOW_AUTOSIZE);
	//imshow("Eye: " + caption + " Corner: L Color", leftCornerGray);

	leftCornerGray = imageProcessingMethods.AdaptiveHistThresh(leftCornerGray, eyeCornerThresh);
	//threshold(leftCornerGray, leftCornerGray, 0, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);
	
	erode(leftCornerGray, leftCornerGray, Mat());
	//imshow("Eye: " + caption + " Corner: L", leftCornerGray);

	//Find left most point of the white blob
	Point leftCornerPos = Point(leftCornerGray.cols, leftCornerGray.rows);

	for (int i = 0; i < leftCornerGray.rows; i++)
	{
		for (int j = 0; j < leftCornerGray.cols; j++)
		{
			if (leftCornerGray.at<uchar>(i, j) == 255 && j <= leftCornerPos.x)
			{
				leftCornerPos.x = j;
				leftCornerPos.y = i;
			}
		}
	}
	
	return Point(leftCornerPos.x + eyeImage.cols*CORNER_X, leftCornerPos.y + eyeImage.rows*CORNER_Y);
}

Point EyeCornerDetector::returnRightCornerPos(Mat eyeImage, string caption)
{
	Mat rightCornerImage = eyeImage(Rect(
										eyeImage.cols*(1-CORNER_X-CORNER_W), //Left 
										eyeImage.rows*CORNER_Y, //Top
										eyeImage.cols*CORNER_W, //Width
										eyeImage.rows*(1-2*CORNER_Y) //Height
										));
	
	Mat rightCornerGray = imageProcessingMethods.RGB2GRAY(rightCornerImage);
	blur(rightCornerGray, rightCornerGray, Size(3, 3));

	//namedWindow("Eye: " + caption + " Corner: R Color", CV_WINDOW_AUTOSIZE);
	//imshow("Eye: " + caption + " Corner: R Color", rightCornerGray);

	rightCornerGray = imageProcessingMethods.AdaptiveHistThresh(rightCornerGray, eyeCornerThresh);
	//threshold(rightCornerGray, rightCornerGray, 0, 255, THRESH_OTSU | CV_THRESH_BINARY_INV);

	erode(rightCornerGray, rightCornerGray, Mat());
	//imshow("Eye: " + caption + " Corner: R", rightCornerGray);

	//Find right most point of the white blob	

	Point rightCornerPos = Point(0, 0);
	for (int i = 0; i < rightCornerGray.rows; i++)
	{
		for (int j = 0; j < rightCornerGray.cols; j++)
		{
			if (rightCornerGray.at<uchar>(i, j) == 255 && j >= rightCornerPos.x)
			{
				rightCornerPos.x = j;
				rightCornerPos.y = i;								
			}
		}
	}
	
	return Point(rightCornerPos.x + eyeImage.cols*(1-CORNER_X-CORNER_W), rightCornerPos.y + eyeImage.rows*CORNER_Y);
}

Point EyeCornerDetector::drawLeftEyeCorner(Mat frame, Rect facePos, Rect eyePos, Point eyeCornerPos)
{
	Point cornerWrtFrame;
	cornerWrtFrame.x = facePos.x + EYE_SX*facePos.width + eyePos.x + eyeCornerPos.x;
	cornerWrtFrame.y = facePos.y + EYE_SY*facePos.height * (2 - EW_RATIO) + eyePos.y * (2 - EW_RATIO) + eyeCornerPos.y;

	circle(frame, cornerWrtFrame, 4, Scalar(255, 255, 255));

	return cornerWrtFrame;
}

Point EyeCornerDetector::drawRightEyeCorner(Mat frame, Rect facePos, Rect eyePos, Point eyeCornerPos)
{
	Point cornerWrtFrame;
	cornerWrtFrame.x = facePos.x + (1-EYE_SX-EYE_SW)*facePos.width + eyePos.x + eyeCornerPos.x;
	cornerWrtFrame.y = facePos.y + EYE_SY*facePos.height * (2 - EW_RATIO) + eyePos.y * (2 - EW_RATIO) + eyeCornerPos.y;

	circle(frame, cornerWrtFrame, 4, Scalar(255, 255, 255));

	return cornerWrtFrame;
}
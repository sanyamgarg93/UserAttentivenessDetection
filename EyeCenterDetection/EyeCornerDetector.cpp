#include "EyeCenterDetectionHeaders.h"
#include "constants.h"

EyeCornerDetector::EyeCornerDetector()
{
	eyeCornerThresh = 0.15;
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

	Mat leftCornerBW = imageProcessingMethods.AdaptiveHistThresh(leftCornerGray, eyeCornerThresh);	
	erode(leftCornerBW, leftCornerBW, erosionElement);

#ifdef SHOW_WINDOWS
	namedWindow("Eye: " + caption + " Corner: L Color", CV_WINDOW_AUTOSIZE);
	imshow("Eye: " + caption + " Corner: L Color", leftCornerGray);	
	imshow("Eye: " + caption + " Corner: L", leftCornerBW);
#endif

	Point leftCornerPos = Point(leftCornerBW.cols/2, leftCornerBW.rows/2);
	
	//Technique-1: Find left most point of the white blob	
	for (int j = 0; j < leftCornerBW.cols; j++)
	{
		int cornerFound = 0;
		for (int i = leftCornerBW.rows - 1; i >= 0; i--)
		{
			if (leftCornerBW.at<uchar>(i, j) == 255)
			{
				leftCornerPos.x = j;
				leftCornerPos.y = i;				
				cornerFound = 1;
				break;
			}
		}
		if (cornerFound == 1)
			break;
	}

	/* Technique-2: Find darkest pixel among segmented pixels
	int min = 255; 
	for (int i = 0; i < leftCornerGray.rows; i++)
	{
		for (int j = 0; j < leftCornerGray.cols; j++)
		{
			if (leftCornerBW.at<uchar>(i, j) == 255 && leftCornerGray.at<uchar>(i, j) < min)
			{
				min = leftCornerGray.at<uchar>(i, j);
				leftCornerPos.x = j;
				leftCornerPos.y = i;
			}
		}
	}
	*/	
	
#ifdef SHOW_WINDOWS
	rectangle(eyeImage, Rect(eyeImage.cols*CORNER_X, eyeImage.rows*CORNER_Y, eyeImage.cols*CORNER_W, eyeImage.rows*(1 - 2 * CORNER_Y)), Scalar(0, 255, 255), 1);
#endif

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

	Mat rightCornerBW = imageProcessingMethods.AdaptiveHistThresh(rightCornerGray, eyeCornerThresh);
	erode(rightCornerBW, rightCornerBW, erosionElement);
	
#ifdef SHOW_WINDOWS
	namedWindow("Eye: " + caption + " Corner: R Color", CV_WINDOW_AUTOSIZE);
	imshow("Eye: " + caption + " Corner: R Color", rightCornerGray);
	imshow("Eye: " + caption + " Corner: R", rightCornerBW);
#endif

	Point rightCornerPos = Point(rightCornerBW.cols/2, rightCornerBW.rows/2);

	//Technique-1: Find right most point of the white blob	
	for (int j = rightCornerBW.cols - 1; j >= 0; j--)
	{
		int cornerFound = 0;
		for (int i = rightCornerBW.rows - 1; i >= 0; i--)
		{
			if (rightCornerBW.at<uchar>(i, j) == 255)
			{
				rightCornerPos.x = j;
				rightCornerPos.y = i;
				cornerFound = 1;
				break;
			}
		}
		if (cornerFound == 1)
			break;
	}

	/*Technique-2: Find darkest pixel among segmented pixels
	int min = 255;
	for (int i = 0; i < rightCornerGray.rows; i++)
	{
		for (int j = 0; j < rightCornerGray.cols; j++)
		{
			if (rightCornerBW.at<uchar>(i, j) == 255 && rightCornerGray.at<uchar>(i, j) < min)
			{
				min = rightCornerGray.at<uchar>(i, j);
				rightCornerPos.x = j;
				rightCornerPos.y = i;
			}
		}
	}
	*/

#ifdef SHOW_WINDOWS
	rectangle(eyeImage, Rect(eyeImage.cols*(1 - CORNER_X - CORNER_W), eyeImage.rows*CORNER_Y, eyeImage.cols*CORNER_W, eyeImage.rows*(1 - 2 * CORNER_Y)), Scalar(0, 255, 255), 1);
#endif 

	return Point(rightCornerPos.x + eyeImage.cols*(1-CORNER_X-CORNER_W), rightCornerPos.y + eyeImage.rows*CORNER_Y);
}

Point EyeCornerDetector::drawLeftEyeCorner(Mat frame, Rect facePos, Rect eyePos, Point eyeCornerPos)
{
	Point cornerWrtFrame;
	cornerWrtFrame.x = facePos.x + EYE_SX*facePos.width + eyePos.x + eyeCornerPos.x;
	cornerWrtFrame.y = facePos.y + EYE_SY*facePos.height * (2 - EW_RATIO) + eyePos.y * (2 - EW_RATIO) + eyeCornerPos.y;

	circle(frame, cornerWrtFrame, 2, Scalar(255, 255, 255));

	return cornerWrtFrame;
}

Point EyeCornerDetector::drawRightEyeCorner(Mat frame, Rect facePos, Rect eyePos, Point eyeCornerPos)
{
	Point cornerWrtFrame;
	cornerWrtFrame.x = facePos.x + (1-EYE_SX-EYE_SW)*facePos.width + eyePos.x + eyeCornerPos.x;
	cornerWrtFrame.y = facePos.y + EYE_SY*facePos.height * (2 - EW_RATIO) + eyePos.y * (2 - EW_RATIO) + eyeCornerPos.y;

	circle(frame, cornerWrtFrame, 2, Scalar(255, 255, 255));

	return cornerWrtFrame;
}
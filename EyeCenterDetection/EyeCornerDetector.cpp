#include "EyeCenterDetectionHeaders.h"

EyeCornerDetector::EyeCornerDetector()
{
	int erosion_type = 0, erosion_elem = 1, erosion_size = 2;
	if (erosion_elem == 0)
		erosion_type = MORPH_RECT; 
	else if (erosion_elem == 1)
		erosion_type = MORPH_CROSS; 
	else if (erosion_elem == 2) 
		erosion_type = MORPH_ELLIPSE; 

	erosionElement = getStructuringElement(erosion_type, Size(2 * erosion_size + 1, 2 * erosion_size + 1),	Point(erosion_size, erosion_size));

	CORNER_W = 0.25; CORNER_X = 0.05; CORNER_Y = 0.25;
}

Mat EyeCornerDetector::detectHarrisCorners(Mat rgbImage)
{
	Mat image = imageProcessingMethods.RGB2GRAY(rgbImage);

	imshow("Left Eye Image", image);

	Mat cornerStrength;
	cornerHarris(image, cornerStrength, 3, 3, 0.01);

	Mat harrisCorners;
	threshold(cornerStrength, harrisCorners, 0.0001, 255, CV_THRESH_BINARY);

	namedWindow("Eye Corners", CV_WINDOW_AUTOSIZE);
	imshow("Eye Corners", harrisCorners);
	return harrisCorners;
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
	threshold(leftCornerGray, leftCornerGray, 0, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);
	
	erode(leftCornerGray, leftCornerGray, erosionElement);
	//imshow("Left Eye Otsu " + caption, leftCornerGray);

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

	circle(leftCornerImage, leftCornerPos, 2, Scalar(255, 255, 255));
	//namedWindow("Left Eye Corner " + caption, CV_WINDOW_AUTOSIZE);
	//imshow("Left Eye Corner " + caption, leftCornerImage);

	return leftCornerPos;
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
	threshold(rightCornerGray, rightCornerGray, 0, 255, THRESH_OTSU | CV_THRESH_BINARY_INV);

	erode(rightCornerGray, rightCornerGray, erosionElement);
	//imshow("Right Eye Otsu " + caption, rightCornerGray);

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

	circle(rightCornerImage, rightCornerPos, 2, Scalar(255, 255, 255));
	//namedWindow("Right Eye Corner " + caption, CV_WINDOW_AUTOSIZE);
	//imshow("Right Eye Corner " + caption, rightCornerImage);

	return rightCornerPos;
}
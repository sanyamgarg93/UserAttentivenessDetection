#include "EyeCenterDetectionHeaders.h"

EyeGazeEstimator::EyeGazeEstimator()
{
	//cornerDistance = 0;
}

//Make private
float EyeGazeEstimator::perpDistanceFromLine(Point P, Point a, Point b)
{
	float perpDist = ((a.y - b.y)*P.x + (b.x - a.x)*P.y + (a.x*b.y - a.y*b.x)) / (float)(sqrt(pow(a.y - b.y, 2) + pow(a.x - b.x, 2)));
	return perpDist;
}

//Make private
float EyeGazeEstimator::distanceBw2Points(Point a, Point b)
{
	float distance = sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
	return distance;
}

int EyeGazeEstimator::eyeCornerDistance(Mat frame, Point leftEyeLeftCorner, Point rightEyeRightCorner)
{
	//Display line connecting the eye corners
	line(frame, leftEyeLeftCorner, rightEyeRightCorner, Scalar(0, 0, 255), 1);	

	int cornerDistance = distanceBw2Points(leftEyeLeftCorner, rightEyeRightCorner);
	return cornerDistance;
}

int EyeGazeEstimator::eyeCornerDistance(Point leftEyeLeftCorner, Point rightEyeRightCorner)
{	
	int cornerDistance = distanceBw2Points(leftEyeLeftCorner, rightEyeRightCorner);
	return cornerDistance;
}

float EyeGazeEstimator::verticalShift(Point eyeCenterLeftPos, Point eyeCenterRightPos, Point leftEyeLeftCorner, Point rightEyeRightCorner)
{
	//Find average of the shifts.
	float perpDist1 = perpDistanceFromLine(eyeCenterLeftPos, leftEyeLeftCorner, rightEyeRightCorner);
	float perpDist2 = perpDistanceFromLine(eyeCenterRightPos, leftEyeLeftCorner, rightEyeRightCorner);

	return (perpDist1 + perpDist2) / 2;

	//TODO: Return with symbol 
}

float EyeGazeEstimator::horizontalShift(Point eyeCenterLeftPos, Point eyeCenterRightPos, Point leftEyeLeftCorner, Point rightEyeRightCorner)
{
	//shiftL is distance between left eye center and left eye corner
	float shiftL = distanceBw2Points(leftEyeLeftCorner,eyeCenterLeftPos);
	float shiftR = distanceBw2Points(rightEyeRightCorner, eyeCenterRightPos);

	return (shiftL - shiftR);
}


#include "EyeCenterDetectionHeaders.h"

//CascadeClassifier frontalFaceDetector;

FaceDetection::FaceDetection()
{
	string frontalFaceCascadeFilename = "C:\\opencv\\sources\\data\\lbpcascades\\lbpcascade_frontalface.xml";
	frontalFaceDetector.load(frontalFaceCascadeFilename);
}

//NOT USED since classifier is initialized in the classifier
void FaceDetection::initializeClassifier()
{
	string frontalFaceCascadeFilename = "C:\\opencv\\sources\\data\\lbpcascades\\lbpcascade_frontalface.xml";
	frontalFaceDetector.load(frontalFaceCascadeFilename);
}

vector<Rect> FaceDetection::storeFrontalFacePos(Mat image)
{
	int flags = CV_HAAR_DO_CANNY_PRUNING;					// Search for many faces.
	Size minFeatureSize(image.cols / 5, image.cols / 5);	// Smallest face size.
	float searchScaleFactor = 1.2f;							// How many sizes to search.
	int minNeighbors = 2;									// Reliability vs many faces.

	vector<Rect> faces;
	frontalFaceDetector.detectMultiScale(	
										image, 
										faces, 
										searchScaleFactor, 
										minNeighbors, 
										flags, 
										minFeatureSize
										);
	return faces;
}

void FaceDetection::drawFaceOnImage(Mat image, vector<Rect> frontalFacePositions)
{
	for (int i = 0; i < (int)frontalFacePositions.size(); i++)
	{
		Rect frontalFaceRect(
							frontalFacePositions[i].x, 
							frontalFacePositions[i].y, 
							frontalFacePositions[i].width, 
							frontalFacePositions[i].height
							);		
		rectangle(image, frontalFaceRect, Scalar(0, 0, 255), 1);
	}
}
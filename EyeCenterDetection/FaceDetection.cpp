#include "EyeCenterDetectionHeaders.h"

//CascadeClassifier frontalFaceDetector;

FaceDetection::FaceDetection()
{
	string frontalFaceCascadeFilename = "DetectionCascades\\lbpcascade_frontalface.xml";
	frontalFaceDetector.load(frontalFaceCascadeFilename);

	searchScaleFactor = 1.2f;				// Scale density.
	minNeighbourCount = 2;					// Reliability vs many faces
	searchFlags = CV_HAAR_DO_CANNY_PRUNING; // Search for many faces. CV_HAAR_DO_CANNY_PRUNING skips areas without many lines.
	minSearchSize = Size(10, 10);			// Smallest face size.
}

vector<Rect> FaceDetection::storeFrontalFacePos(Mat image)
{	
	minSearchSize.height = image.cols / 5; 
	minSearchSize.width = image.cols / 5; 

	vector<Rect> faces;
	frontalFaceDetector.detectMultiScale(	
										image, 
										faces, 
										searchScaleFactor, 
										minNeighbourCount, 
										searchFlags, 
										minSearchSize
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
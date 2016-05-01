#include "EyeCenterDetectionHeaders.h"
#include "constants.h"

FaceDetection::FaceDetection()
{
	string frontalFaceCascadeFilename = "DetectionCascades\\lbpcascade_frontalface.xml";
	frontalFaceDetector.load(frontalFaceCascadeFilename);

	searchScaleFactor = 1.2f;				// Scale density.
	minNeighbourCount = 2;					// Reliability vs many faces
	searchFlags = CV_HAAR_DO_CANNY_PRUNING; // Search for many faces. CV_HAAR_DO_CANNY_PRUNING skips areas without many lines.
	minSearchSize = Size(50,50);				// Smallest face size.
}

vector<Rect> FaceDetection::storeFrontalFacePos(Mat image)
{	
	minSearchSize.height = image.cols / 10; 
	minSearchSize.width = image.cols / 10; 

	Mat grayFrame;
	imageProcessingMethods.RGB2GRAY(image).copyTo(grayFrame);

	//Reduce Image Size
	Mat smallerFrame = imageProcessingMethods.sizeReduce(grayFrame, SMALLER_DETECTION_WIDTH);
	float scale = float(image.cols) / SMALLER_DETECTION_WIDTH;

	equalizeHist(smallerFrame, smallerFrame);

	vector<Rect> frontalFaces;
	frontalFaceDetector.detectMultiScale(	
										smallerFrame, 
										frontalFaces, 
										searchScaleFactor, 
										minNeighbourCount, 
										searchFlags, 
										minSearchSize
										);

	//Restore the face positions
	for (int i = 0; i < (int)frontalFaces.size(); i++)
	{
		if (grayFrame.cols > SMALLER_DETECTION_WIDTH)
		{
			frontalFaces[i].x = frontalFaces[i].x*scale;
			frontalFaces[i].y = frontalFaces[i].y*scale;
			frontalFaces[i].width = frontalFaces[i].width*scale;
			frontalFaces[i].height = frontalFaces[i].height*scale;
		}
	}

	return frontalFaces;
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
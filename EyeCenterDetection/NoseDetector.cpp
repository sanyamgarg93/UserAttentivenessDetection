#include "EyeCenterDetectionHeaders.h"
#include "constants.h"

NoseDetector::NoseDetector()
{
	string noseCascadeFilename = "DetectionCascades\\haarcascade_mcs_nose.xml";
	noseDetector.load(noseCascadeFilename);
		
	searchScaleFactor = 1.1;
	minNeighbourCount = 2;
	searchFlags = CASCADE_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH; //This combination of flags terminates the search at whatever scale the first candidate is found.
	minSearchSize = Size(10, 10);
}

vector<Rect> NoseDetector::storeNosePos(Mat faceImage)
{
	Mat centerOfFace = faceImage(Rect(NOSE_SX*faceImage.cols, NOSE_SY*faceImage.rows, NOSE_SW*faceImage.cols, NOSE_SH*faceImage.rows));

	Mat centerOfFaceGray;
	imageProcessingMethods.RGB2GRAY(centerOfFace).copyTo(centerOfFaceGray);

	equalizeHist(centerOfFaceGray, centerOfFaceGray);

	imshow("Center of Face", centerOfFaceGray);

	vector<Rect> nosePos;
	noseDetector.detectMultiScale(
		centerOfFaceGray,
		nosePos,
		searchScaleFactor,
		minNeighbourCount,
		searchFlags,
		minSearchSize
		); //Run Right eye detector on left side of face

	return nosePos;
}
void NoseDetector::drawNoseOnImage(Mat image, Rect facePos, Rect nosePosition)
{
	Rect noseRect(
		facePos.x + NOSE_SX*facePos.width + nosePosition.x,
		facePos.y + NOSE_SY*facePos.height + nosePosition.y,
		nosePosition.width,
		nosePosition.height
		);

	rectangle(image, noseRect, Scalar(0, 255, 0), 1);
}

Mat NoseDetector::returnNoseImage(Mat faceImage, Rect nosePosition)
{
	return faceImage(Rect(
		NOSE_SX*faceImage.cols + nosePosition.x,
		NOSE_SY*faceImage.rows + nosePosition.y,
		nosePosition.width,
		nosePosition.height
		));
}
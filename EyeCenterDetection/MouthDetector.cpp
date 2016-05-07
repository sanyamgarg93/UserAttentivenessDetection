#include "EyeCenterDetectionHeaders.h"
#include "constants.h"

MouthDetector::MouthDetector()
{
	string mouthCascadeFilename = "DetectionCascades\\haarcascade_mcs_mouth.xml";
	mouthDetector.load(mouthCascadeFilename);

	searchScaleFactor = 1.1;
	minNeighbourCount = 2;
	searchFlags = CASCADE_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH; //This combination of flags terminates the search at whatever scale the first candidate is found.
	minSearchSize = Size(10, 10);
}

vector<Rect> MouthDetector::storeMouthPos(Mat faceImage)
{
	Mat centerOfFace = faceImage(Rect(MOUTH_SX*faceImage.cols, MOUTH_SY*faceImage.rows, MOUTH_SW*faceImage.cols, MOUTH_SH*faceImage.rows));

	Mat centerOfFaceGray;
	imageProcessingMethods.RGB2GRAY(centerOfFace).copyTo(centerOfFaceGray);

	//equalizeHist(centerOfFaceGray, centerOfFaceGray);

	//imshow("Mouth Search region", centerOfFaceGray);

	vector<Rect> mouthPos;
	mouthDetector.detectMultiScale(
		centerOfFaceGray,
		mouthPos,
		searchScaleFactor,
		minNeighbourCount,
		searchFlags,
		minSearchSize
		); //Run Right eye detector on left side of face

	return mouthPos;
}
void MouthDetector::drawMouthOnImage(Mat image, Rect facePos, Rect mouthPosition)
{
	Rect mouthRect(
		facePos.x + MOUTH_SX*facePos.width + mouthPosition.x,
		facePos.y + MOUTH_SY*facePos.height + mouthPosition.y,
		mouthPosition.width,
		mouthPosition.height
		);

	rectangle(image, mouthRect, Scalar(0, 255, 0), 1);
}

Mat MouthDetector::returnMouthImage(Mat faceImage, Rect mouthPosition)
{
	return faceImage(Rect(
		MOUTH_SX*faceImage.cols + mouthPosition.x,
		MOUTH_SY*faceImage.rows + mouthPosition.y,
		mouthPosition.width,
		mouthPosition.height
		));
}
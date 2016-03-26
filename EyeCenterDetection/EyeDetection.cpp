#include "EyeCenterDetectionHeaders.h"

EyeDetection::EyeDetection()
{
	string leftEyeCascadeFilename = "C:\\opencv\\sources\\data\\haarcascades_cuda\\haarcascade_lefteye_2splits.xml";
	leftEyeDetector.load(leftEyeCascadeFilename);

	string rightEyeCascadeFilename = "C:\\opencv\\sources\\data\\haarcascades_cuda\\haarcascade_righteye_2splits.xml";
	rightEyeDetector.load(rightEyeCascadeFilename);

	EYE_SX = 0.10, EYE_SY = 0.15, EYE_SH = 0.40, EYE_SW = 0.40;	
}

vector<Rect> EyeDetection::storeRightEyePos(Mat faceImage)
{
	//Run the eye detector on Right side of face	
	Mat topRightOfFace = faceImage(Rect((1-EYE_SX-EYE_SW)*faceImage.cols, EYE_SY*faceImage.rows, EYE_SW*faceImage.cols, EYE_SH*faceImage.rows));

	vector<Rect> rightEyePos;
	leftEyeDetector.detectMultiScale(topRightOfFace, rightEyePos, 1.1, 2, CASCADE_FIND_BIGGEST_OBJECT, Size(0, 0));	//Run Left eye detector on right side of face

	return rightEyePos;
}

vector<Rect> EyeDetection::storeLeftEyePos(Mat faceImage)
{
	//Run the eye detector on Left side of face
	Mat topLeftOfFace = faceImage(Rect(EYE_SX*faceImage.cols, EYE_SY*faceImage.rows, EYE_SW*faceImage.cols, EYE_SH*faceImage.rows));
	
	vector<Rect> leftEyePos;
	rightEyeDetector.detectMultiScale(topLeftOfFace, leftEyePos, 1.1, 2, CASCADE_FIND_BIGGEST_OBJECT, Size(0, 0)); //Run Right eye detector on left side of face

	return leftEyePos;
}

void EyeDetection::drawRightEyeOnImage(Mat image, Rect facePos, vector<Rect> eyePositions)
{
	for (int j = 0; j < eyePositions.size(); j++)
	{
		Rect eyeRect(
					facePos.x + (1-EYE_SX-EYE_SW)*facePos.width + eyePositions[j].x,
					facePos.y + EYE_SY*facePos.height + eyePositions[j].y, 
					eyePositions[j].width, 
					eyePositions[j].height
					);

		rectangle(image, eyeRect, Scalar(0, 255, 0), 1);
	}
}

void EyeDetection::drawLeftEyeOnImage(Mat image, Rect facePos, vector<Rect> eyePositions)
{
	for (int j = 0; j < eyePositions.size(); j++)
	{
		Rect eyeRect(
			facePos.x + EYE_SX*facePos.width + eyePositions[j].x,
			facePos.y + EYE_SY*facePos.height + eyePositions[j].y,
			eyePositions[j].width,
			eyePositions[j].height
			);

		rectangle(image, eyeRect, Scalar(0, 255, 0), 1);
	}
}







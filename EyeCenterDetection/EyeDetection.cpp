#include "EyeCenterDetectionHeaders.h"
#include "constants.h"

EyeDetection::EyeDetection()
{
	string leftEyeCascadeFilename = "DetectionCascades\\haarcascade_lefteye_2splits.xml";
	//string leftEyeCascadeFilename = "DetectionCascades\\haarcascade_eye_tree_eyeglasses.xml";
	leftEyeDetector.load(leftEyeCascadeFilename);

	string rightEyeCascadeFilename = "DetectionCascades\\haarcascade_righteye_2splits.xml";
	//string rightEyeCascadeFilename = "DetectionCascades\\haarcascade_eye_tree_eyeglasses.xml";
	rightEyeDetector.load(rightEyeCascadeFilename);	

	searchScaleFactor = 1.1; 
	minNeighbourCount = 2; 
	searchFlags = CASCADE_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH; //This combination of flags terminates the search at whatever scale the first candidate is found.
	minSearchSize = Size(10, 10);
}

vector<Rect> EyeDetection::storeRightEyePos(Mat faceImage)
{
	//Run the eye detector on Right side of face	
	Mat topRightOfFace = faceImage(Rect((1-EYE_SX-EYE_SW)*faceImage.cols, EYE_SY*faceImage.rows, EYE_SW*faceImage.cols, EYE_SH*faceImage.rows));
		
	Mat topRightOfFaceGray = imageProcessingMethods.RGB2GRAY(topRightOfFace);
	
	vector<Rect> rightEyePos;
	leftEyeDetector.detectMultiScale(
									topRightOfFaceGray, 
									rightEyePos, 
									searchScaleFactor, 
									minNeighbourCount, 
									searchFlags, 
									minSearchSize
									);	//Run Left eye detector on right side of face

	return rightEyePos;
}

vector<Rect> EyeDetection::storeLeftEyePos(Mat faceImage)
{
	//Run the eye detector on Left side of face
	Mat topLeftOfFace = faceImage(Rect(EYE_SX*faceImage.cols, EYE_SY*faceImage.rows, EYE_SW*faceImage.cols, EYE_SH*faceImage.rows));

	Mat topLeftOfFaceGray = imageProcessingMethods.RGB2GRAY(topLeftOfFace);

	vector<Rect> leftEyePos;
	rightEyeDetector.detectMultiScale(
									topLeftOfFaceGray, 
									leftEyePos, 
									searchScaleFactor,
									minNeighbourCount,
									searchFlags,
									minSearchSize
									); //Run Right eye detector on left side of face

	return leftEyePos;
}

void EyeDetection::drawRightEyeOnImage(Mat image, Rect facePos, Rect eyePosition)
{
	Rect eyeRect(
				facePos.x + (1-EYE_SX-EYE_SW)*facePos.width + eyePosition.x,
				facePos.y + EYE_SY*facePos.height * (2 - EW_RATIO) + eyePosition.y * (2 - EW_RATIO),
				eyePosition.width, 
				eyePosition.height * EW_RATIO
				);

	rectangle(image, eyeRect, Scalar(0, 255, 0), 1);	
}

void EyeDetection::drawLeftEyeOnImage(Mat image, Rect facePos, Rect eyePosition)
{	
	Rect eyeRect(
				facePos.x + EYE_SX*facePos.width + eyePosition.x, 
				facePos.y + EYE_SY*facePos.height * (2 - EW_RATIO) + eyePosition.y * (2 - EW_RATIO),
				eyePosition.width,
				eyePosition.height * EW_RATIO
				);

	rectangle(image, eyeRect, Scalar(0, 255, 0), 1);	
}

Mat EyeDetection::returnRightEyeImage(Mat faceImage, Rect eyePosition)
{	
	return faceImage(Rect(
						(1-EYE_SX-EYE_SW)*faceImage.cols + eyePosition.x, 
						EYE_SY*faceImage.rows * (2 - EW_RATIO) + eyePosition.y * (2 - EW_RATIO),
						eyePosition.width, 
						eyePosition.height * EW_RATIO
						));
}

Mat EyeDetection::returnLeftEyeImage(Mat faceImage, Rect eyePosition)
{
	return faceImage(Rect(
						EYE_SX*faceImage.cols + eyePosition.x, 
						EYE_SY*faceImage.rows * (2 - EW_RATIO) + eyePosition.y * (2 - EW_RATIO),
						eyePosition.width, 
						eyePosition.height * EW_RATIO
						));
}







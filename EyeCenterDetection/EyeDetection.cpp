#include "EyeCenterDetectionHeaders.h"
#include "constants.h"

EyeDetection::EyeDetection()
{
	string leftEyeCascadeFilename = "C:\\opencv\\sources\\data\\haarcascades_cuda\\haarcascade_lefteye_2splits.xml";
	leftEyeDetector.load(leftEyeCascadeFilename);

	string rightEyeCascadeFilename = "C:\\opencv\\sources\\data\\haarcascades_cuda\\haarcascade_righteye_2splits.xml";
	rightEyeDetector.load(rightEyeCascadeFilename);

	
}

Rect EyeDetection::storeRightEyePos(Mat faceImage)
{
	//Run the eye detector on Right side of face	
	Mat topRightOfFace = faceImage(Rect((1-EYE_SX-EYE_SW)*faceImage.cols, EYE_SY*faceImage.rows, EYE_SW*faceImage.cols, EYE_SH*faceImage.rows));

	vector<Rect> rightEyePos;
	leftEyeDetector.detectMultiScale(
									topRightOfFace, 
									rightEyePos, 
									1.1, 
									2, 
									CASCADE_FIND_BIGGEST_OBJECT, 
									Size(0, 0)
									);	//Run Left eye detector on right side of face

	int max = 0; Rect biggestRightEye;
	for (int i = 0; i < rightEyePos.size(); i++)
	{
		if (rightEyePos[i].width > max)
		{
			max = rightEyePos[i].width;
			biggestRightEye = rightEyePos[i];
		}
			
	}

	return biggestRightEye;
}

Rect EyeDetection::storeLeftEyePos(Mat faceImage)
{
	//Run the eye detector on Left side of face
	Mat topLeftOfFace = faceImage(Rect(EYE_SX*faceImage.cols, EYE_SY*faceImage.rows, EYE_SW*faceImage.cols, EYE_SH*faceImage.rows));
	
	vector<Rect> leftEyePos;
	rightEyeDetector.detectMultiScale(
									topLeftOfFace, 
									leftEyePos, 
									1.1, 
									2, 
									CASCADE_FIND_BIGGEST_OBJECT, 
									Size(0, 0)
									); //Run Right eye detector on left side of face

	int max = 0; Rect biggestLeftEye;
	for (int i = 0; i < leftEyePos.size(); i++)
	{
		if (leftEyePos[i].width > max)
		{
			max = leftEyePos[i].width;
			biggestLeftEye = leftEyePos[i];
		}
	}

	return biggestLeftEye;
}

void EyeDetection::drawRightEyeOnImage(Mat image, Rect facePos, Rect eyePosition)
{
	Rect eyeRect(
				facePos.x + (1-EYE_SX-EYE_SW)*facePos.width + eyePosition.x,
				facePos.y + EYE_SY*facePos.height + eyePosition.y, 
				eyePosition.width, 
				eyePosition.height
				);

	rectangle(image, eyeRect, Scalar(0, 255, 0), 1);	
}

void EyeDetection::drawLeftEyeOnImage(Mat image, Rect facePos, Rect eyePosition)
{	
	Rect eyeRect(
				facePos.x + EYE_SX*facePos.width + eyePosition.x,
				facePos.y + EYE_SY*facePos.height + eyePosition.y,
				eyePosition.width,
				eyePosition.height
				);

	rectangle(image, eyeRect, Scalar(0, 255, 0), 1);	
}

Mat EyeDetection::returnEyeImage(Mat faceImage, Rect eyePosition)
{
	return faceImage(eyePosition);	;
}







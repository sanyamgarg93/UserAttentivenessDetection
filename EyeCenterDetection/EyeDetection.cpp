#include "EyeCenterDetectionHeaders.h"
#include "constants.h"

EyeDetection::EyeDetection()
{
	string leftEyeCascadeFilename = "DetectionCascades\\haarcascade_lefteye_2splits.xml";
	leftEyeDetector.load(leftEyeCascadeFilename);

	string rightEyeCascadeFilename = "DetectionCascades\\haarcascade_righteye_2splits.xml";
	rightEyeDetector.load(rightEyeCascadeFilename);	
}

Rect EyeDetection::storeRightEyePos(Mat faceImage)
{
	//Run the eye detector on Right side of face	
	Mat topRightOfFace = faceImage(Rect((1-EYE_SX-EYE_SW)*faceImage.cols, EYE_SY*faceImage.rows, EYE_SW*faceImage.cols, EYE_SH*faceImage.rows));

	//namedWindow("Right Face Search region", CV_WINDOW_AUTOSIZE);
	//imshow("Right Face Search region", topRightOfFace);

	Mat topRightOfFaceGray = imageProcessingMethods.RGB2GRAY(topRightOfFace);
	equalizeHist(topRightOfFaceGray, topRightOfFaceGray);

	vector<Rect> rightEyePos;
	leftEyeDetector.detectMultiScale(
									topRightOfFaceGray, 
									rightEyePos, 
									1.1, 
									3, 
									CASCADE_FIND_BIGGEST_OBJECT, 
									Size(10, 10)
									);	//Run Left eye detector on right side of face

	if (rightEyePos.size() == 0) //No right eye found
	{
		//cout << "Right eye NOT FOUND" << endl;
		return Rect(0, 0, 0, 0);
	}

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
	
	//namedWindow("Left Face Search region", CV_WINDOW_AUTOSIZE);
	//imshow("Left Face Search region", topLeftOfFace);

	Mat topLeftOfFaceGray = imageProcessingMethods.RGB2GRAY(topLeftOfFace);
	equalizeHist(topLeftOfFaceGray, topLeftOfFaceGray);

	vector<Rect> leftEyePos;
	rightEyeDetector.detectMultiScale(
									topLeftOfFaceGray, 
									leftEyePos, 
									1.1, 
									3, 
									CASCADE_FIND_BIGGEST_OBJECT, 
									Size(10, 10)
									); //Run Right eye detector on left side of face

	if (leftEyePos.size() == 0) //No left eye found
	{
		//cout << "Left Eye NOT FOUND" << endl;
		return Rect(0, 0, 0, 0);
	}

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

Mat EyeDetection::returnRightEyeImage(Mat faceImage, Rect eyePosition)
{	
	return faceImage(Rect((1-EYE_SX-EYE_SW)*faceImage.cols + eyePosition.x, EYE_SY*faceImage.rows + eyePosition.y, eyePosition.width, eyePosition.height));
}

Mat EyeDetection::returnLeftEyeImage(Mat faceImage, Rect eyePosition)
{
	return faceImage(Rect(EYE_SX*faceImage.cols + eyePosition.x, EYE_SY*faceImage.rows + eyePosition.y, eyePosition.width, eyePosition.height));
}







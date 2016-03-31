#include "EyeCenterDetectionHeaders.h"
#include "constants.h"

void detectInImage(Mat frame)
{
	//Reduce Image Size
	Mat smallerFrame = imageProcessingMethods.sizeReduce(frame, SMALLER_DETECTION_WIDTH);
	float scale = float(frame.cols) / SMALLER_DETECTION_WIDTH; 
	
	//Detect faces in smaller image
	vector<Rect> frontalFaces = faceDetection.storeFrontalFacePos(smallerFrame);
	
	//Restore the face positions
	for (int i = 0; i < (int)frontalFaces.size(); i++)
	{
		if (frame.cols > SMALLER_DETECTION_WIDTH)
		{
			frontalFaces[i].x = frontalFaces[i].x*scale;
			frontalFaces[i].y = frontalFaces[i].y*scale;
			frontalFaces[i].width = frontalFaces[i].width*scale;
			frontalFaces[i].height = frontalFaces[i].height*scale;
		}
	}	

	//Detect eyes
	for (int i = 0; i < (int)frontalFaces.size(); i++) //for each face
	{
		//1. Extract the face
		Rect faceRect(frontalFaces[i].x, frontalFaces[i].y, frontalFaces[i].width, frontalFaces[i].height);
		Mat face = frame(faceRect);
		
		//2. Run eye detector on the face
		vector<Rect> leftEyePos = eyeDetection.storeLeftEyePos(face);
		vector<Rect> rightEyePos = eyeDetection.storeRightEyePos(face);

		//Left Eye Processing
		for (int j = 0; j < leftEyePos.size(); j++)
		{
			//3. Draw detected eyes
			eyeDetection.drawLeftEyeOnImage(frame, frontalFaces[i], leftEyePos[j]);			

			//4. Run eye center tracker on each eye region (if found)			
			Mat leftEyeImage = eyeDetection.returnLeftEyeImage(face, leftEyePos[j]);
			Point leftEyeCenterPosition = eyeCenterTracker.estimateEyeCenter(leftEyeImage, "Left");
			eyeCenterTracker.drawLeftEyeCenter(frame, frontalFaces[i], leftEyePos[j], leftEyeCenterPosition);						
		}

		//Right Eye Processing
		for (int j = 0; j < rightEyePos.size(); j++)
		{
			eyeDetection.drawRightEyeOnImage(frame, frontalFaces[i], rightEyePos[j]);

			Mat rightEyeImage = eyeDetection.returnRightEyeImage(face, rightEyePos[j]);
			Point rightEyeCenterPosition = eyeCenterTracker.estimateEyeCenter(rightEyeImage, "Right");
			eyeCenterTracker.drawRightEyeCenter(frame, frontalFaces[i], rightEyePos[j], rightEyeCenterPosition);			
		}
	}

	//Draw faces
	faceDetection.drawFaceOnImage(frame, frontalFaces);
}
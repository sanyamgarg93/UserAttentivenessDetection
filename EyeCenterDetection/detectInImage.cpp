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
			//3. Run eye center tracker on each eye region (if found)			
			Mat leftEyeImage = eyeDetection.returnLeftEyeImage(face, leftEyePos[j]);
					
			Point eyeCenterLeft = eyeCenterTracker.estimateEyeCenter(leftEyeImage, "Left");
			Point leftEyeCenterInitial = eyeCenterTracker.drawLeftEyeCenter(frame, frontalFaces[i], leftEyePos[j], eyeCenterLeft);

			int radius_left = (leftEyeImage.cols + leftEyeImage.rows)*R_RATIO;						
			Point leftEyeCenterFinal= snakuscule.runSnakuscule(frame, leftEyeCenterInitial, &radius_left);
						
			Point leftCorner = eyeCornerDetector.returnLeftCornerPos(leftEyeImage, "L");
			Point rightCorner = eyeCornerDetector.returnRightCornerPos(leftEyeImage, "L");
						
			//4. Draw detected eyes and eye centers
			eyeCornerDetector.drawLeftEyeCorner(frame, frontalFaces[i], leftEyePos[j], leftCorner);
			eyeCornerDetector.drawLeftEyeCorner(frame, frontalFaces[i], leftEyePos[j], rightCorner);
			eyeDetection.drawLeftEyeOnImage(frame, frontalFaces[i], leftEyePos[j]);
			eyeCenterTracker.drawRightEyeCenter(frame, leftEyeCenterFinal, radius_left);			
		}

		//Right Eye Processing
		for (int j = 0; j < rightEyePos.size(); j++)
		{
			Mat rightEyeImage = eyeDetection.returnRightEyeImage(face, rightEyePos[j]);
						
			Point eyeCenterRight = eyeCenterTracker.estimateEyeCenter(rightEyeImage, "Right");
			Point rightEyeCenterInitial = eyeCenterTracker.drawRightEyeCenter(frame, frontalFaces[i], rightEyePos[j], eyeCenterRight);		

			int radius_right = (rightEyeImage.cols + rightEyeImage.rows)*R_RATIO;
			Point rightEyeCenterFinal = snakuscule.runSnakuscule(frame, rightEyeCenterInitial, &radius_right);
			
			Point leftCorner = eyeCornerDetector.returnLeftCornerPos(rightEyeImage, "R");
			Point rightCorner = eyeCornerDetector.returnRightCornerPos(rightEyeImage, "R");

			eyeCornerDetector.drawRightEyeCorner(frame, frontalFaces[i], rightEyePos[j], leftCorner);
			eyeCornerDetector.drawRightEyeCorner(frame, frontalFaces[i], rightEyePos[j], rightCorner);
			eyeDetection.drawRightEyeOnImage(frame, frontalFaces[i], rightEyePos[j]);
			eyeCenterTracker.drawRightEyeCenter(frame, rightEyeCenterFinal, radius_right);
		}
	}

	//Draw faces
	faceDetection.drawFaceOnImage(frame, frontalFaces);
}
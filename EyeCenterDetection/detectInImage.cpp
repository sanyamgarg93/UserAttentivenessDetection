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
			Point eyeCenterLeft = eyeCenterTracker.estimateEyeCenter(leftEyeImage, "Left");
			Point leftEyeCenterPosition = eyeCenterTracker.drawLeftEyeCenter(frame, frontalFaces[i], leftEyePos[j], eyeCenterLeft);

			int radius_left = (leftEyeImage.cols + leftEyeImage.rows)*0.07;			
			int  center_initial_left_frame[2] = { leftEyeCenterPosition.x, leftEyeCenterPosition.y };

			snakuscule.runSnakuscule(frame, center_initial_left_frame, &radius_left);
			Point center_new_left; center_new_left.x = center_initial_left_frame[0]; center_new_left.y = center_initial_left_frame[1];
			
			eyeCenterTracker.drawRightEyeCenter(frame, center_new_left, radius_left);
		}

		//Right Eye Processing
		for (int j = 0; j < rightEyePos.size(); j++)
		{
			eyeDetection.drawRightEyeOnImage(frame, frontalFaces[i], rightEyePos[j]);

			Mat rightEyeImage = eyeDetection.returnRightEyeImage(face, rightEyePos[j]);
			Point eyeCenterRight = eyeCenterTracker.estimateEyeCenter(rightEyeImage, "Right");
			Point rightEyeCenterPosition = eyeCenterTracker.drawRightEyeCenter(frame, frontalFaces[i], rightEyePos[j], eyeCenterRight);		

			int radius_right = (rightEyeImage.cols + rightEyeImage.rows)*0.07;
			int center_initial_right_frame[2] = { rightEyeCenterPosition.x, rightEyeCenterPosition.y};
			
			snakuscule.runSnakuscule(frame, center_initial_right_frame, &radius_right);
			Point center_new_right; center_new_right.x = center_initial_right_frame[0]; center_new_right.y = center_initial_right_frame[1];

			eyeCenterTracker.drawRightEyeCenter(frame, center_new_right, radius_right);
		}
	}

	//Draw faces
	faceDetection.drawFaceOnImage(frame, frontalFaces);
}
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

		Point rightEyeCenterFinal = Point(0, 0);
		Point leftEyeCenterFinal = Point(0, 0);
		Point leftEyeLeftCorner = Point(0, 0);
		Point rightEyeRightCorner = Point(0, 0); 

		//Left Eye Processing
		for (int j = 0; j < leftEyePos.size(); j++)
		{
			//3. Run eye center tracker on each eye region (if found)			
			Mat leftEyeImage = eyeDetection.returnLeftEyeImage(face, leftEyePos[j]);
					
			Point eyeCenterLeft = eyeCenterTracker.estimateEyeCenter(leftEyeImage, "Left");
			Point leftEyeCenterInitial = eyeCenterTracker.drawLeftEyeCenter(frame, frontalFaces[i], leftEyePos[j], eyeCenterLeft);

			int radius_left = (leftEyeImage.cols + leftEyeImage.rows)*R_RATIO;						
			leftEyeCenterFinal= snakuscule.runSnakuscule(frame, leftEyeCenterInitial, &radius_left);
						
			Point leftCorner = eyeCornerDetector.returnLeftCornerPos(leftEyeImage, "L");
			Point rightCorner = eyeCornerDetector.returnRightCornerPos(leftEyeImage, "L");
						
			//4. Draw detected eyes and eye centers
			leftEyeLeftCorner = eyeCornerDetector.drawLeftEyeCorner(frame, frontalFaces[i], leftEyePos[j], leftCorner);
			//Point leftEyeRightCorner = eyeCornerDetector.drawLeftEyeCorner(frame, frontalFaces[i], leftEyePos[j], rightCorner);
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
			rightEyeCenterFinal = snakuscule.runSnakuscule(frame, rightEyeCenterInitial, &radius_right);
			
			Point leftCorner = eyeCornerDetector.returnLeftCornerPos(rightEyeImage, "R");
			Point rightCorner = eyeCornerDetector.returnRightCornerPos(rightEyeImage, "R");

			//Point rightEyeLeftCorner = eyeCornerDetector.drawRightEyeCorner(frame, frontalFaces[i], rightEyePos[j], leftCorner);
			rightEyeRightCorner = eyeCornerDetector.drawRightEyeCorner(frame, frontalFaces[i], rightEyePos[j], rightCorner);
			eyeDetection.drawRightEyeOnImage(frame, frontalFaces[i], rightEyePos[j]);
			eyeCenterTracker.drawRightEyeCenter(frame, rightEyeCenterFinal, radius_right);
		}
		

		//Detect eye gaze for each face only if eye corners and eye centers have been found.
		if (rightEyeRightCorner != Point(0, 0) && leftEyeLeftCorner != Point(0, 0))
		{
			//1. Find distance between the eye corners for normalization. Note it as 'C'. Draw the connecting line.
			int cornerDist = eyeGazeEstimator.eyeCornerDistance(frame, leftEyeLeftCorner, rightEyeRightCorner);

			//3. Find average perpendicular distance of the eye centers from this line. This will tell us if the user is looking up or down.
			float horizontalShift = eyeGazeEstimator.horizontalShift(leftEyeCenterFinal, rightEyeCenterFinal, leftEyeLeftCorner, rightEyeRightCorner) / cornerDist;
			cout << "Horizontal Shift: " << horizontalShift << endl;

			//4. a) Calculate distance of left eye corner from left iris. Note it as 'D1' 
			//   b) Calculate distance of right eye corner from right iris. Not it as 'D2'
			//   c) Looking left and right can be evaluated by calculating (D1-D2)/C. The sign will give the direction. Magnitude will give the angle.  
			float verticalShift = eyeGazeEstimator.verticalShift(leftEyeCenterFinal, rightEyeCenterFinal, leftEyeLeftCorner, rightEyeRightCorner) / cornerDist;
			cout << "Vertical Shift: " << verticalShift << endl;

			//5. Place the face in the center of the screen. Ask user to look in 4 directions. 
		}
	}
	//Draw faces
	faceDetection.drawFaceOnImage(frame, frontalFaces);
}
#ifndef _eyeCenterDetection
#define _eyeCenterDetection

#include<math.h>
#include<iostream>
#include<stdio.h>
#include<queue>
#include<string>
#include<iomanip>
#include<windows.h>
#include<ctime>
#include<fstream>
#include<limits.h>
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\objdetect.hpp>
#include<opencv2\imgproc.hpp>

//Namespaces
using namespace std;
using namespace cv;

//Classes
class FaceDetection
{
	CascadeClassifier frontalFaceDetector;

public:
	FaceDetection();
	void initializeClassifier();	
	vector<Rect> storeFrontalFacePos(Mat);
	void drawFaceOnImage(Mat, vector<Rect>);
};

class EyeDetection
{
	CascadeClassifier leftEyeDetector;
	CascadeClassifier rightEyeDetector;
	float EYE_SX, EYE_SY, EYE_SH, EYE_SW;

public:
	EyeDetection();
	vector<Rect> storeLeftEyePos(Mat);
	vector<Rect> storeRightEyePos(Mat);
	void drawLeftEyeOnImage(Mat, Rect, vector<Rect>);
	void drawRightEyeOnImage(Mat, Rect, vector<Rect>);
};

class ImageProcessingMethods
{
public:
	Mat sizeReduce(Mat, int);
};

//Methods
void detectInImage(Mat);

//Global Variables
extern FaceDetection faceDetection;
extern EyeDetection eyeDetection;
extern ImageProcessingMethods imageProcessingMethods;

#endif
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
	float searchScaleFactor;
	int minNeighbourCount;
	int searchFlags;
	Size minSearchSize;

public:
	FaceDetection();
	vector<Rect> storeFrontalFacePos(Mat);
	void drawFaceOnImage(Mat, vector<Rect>);
};

class EyeDetection
{
	CascadeClassifier leftEyeDetector;
	CascadeClassifier rightEyeDetector;	
	float searchScaleFactor;
	int minNeighbourCount;
	int searchFlags;
	Size minSearchSize;

public:
	EyeDetection();
	vector<Rect> storeLeftEyePos(Mat);
	vector<Rect> storeRightEyePos(Mat);
	void drawLeftEyeOnImage(Mat, Rect, Rect);
	void drawRightEyeOnImage(Mat, Rect, Rect);
	Mat returnLeftEyeImage(Mat, Rect);
	Mat returnRightEyeImage(Mat, Rect);
};

class EyeCenterTracker
{
	int histR[256]; float cumHistR[256];
	Point centerWrtEyeImage;	
	Point centerWrtFrame;

public:
	EyeCenterTracker();
	Point estimateEyeCenter(Mat, string);
	void drawLeftEyeCenter(Mat, Rect, Rect, Point);
	void drawRightEyeCenter(Mat, Rect, Rect, Point);
};

class ImageProcessingMethods
{
public:
	Mat sizeReduce(Mat, int);
	Mat RGB2GRAY(Mat);
};

//Methods
void detectInImage(Mat);

//Global Variables
extern FaceDetection faceDetection;
extern EyeDetection eyeDetection;
extern ImageProcessingMethods imageProcessingMethods;
extern EyeCenterTracker eyeCenterTracker;

#endif
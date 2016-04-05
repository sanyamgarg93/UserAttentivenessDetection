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
	Point drawLeftEyeCenter(Mat, Rect, Rect, Point);
	Point drawRightEyeCenter(Mat, Rect, Rect, Point);
	Point drawLeftEyeCenter(Mat, Point, int);
	Point drawRightEyeCenter(Mat, Point, int);
};

class ImageProcessingMethods
{
public:
	Mat sizeReduce(Mat, int);
	Mat RGB2GRAY(Mat);
};

class Snakuscule
{
	float alpha;
	int positions[50]; 

public:
	Snakuscule();
	void runSnakuscule(Mat, int *, int *);
	float snakeEnergy(Mat, int, float, int, int);
	float outerAnnulusEnergy(Mat, int, float alpha, int *);
	float innerCircleEnergy(Mat, int, int, int);
};

//Methods
void detectInImage(Mat);

//Global Variables
extern FaceDetection faceDetection;
extern EyeDetection eyeDetection;
extern ImageProcessingMethods imageProcessingMethods;
extern EyeCenterTracker eyeCenterTracker;
extern Snakuscule snakuscule;

#endif
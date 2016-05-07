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
#include<thread>
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\objdetect.hpp>
#include<opencv2\imgproc.hpp>
#include<array>

//#define SHOW_WINDOWS 1

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

class NoseDetector
{
	CascadeClassifier noseDetector;
	float searchScaleFactor;
	int minNeighbourCount;
	int searchFlags;
	Size minSearchSize;

public:
	NoseDetector();
	vector<Rect> storeNosePos(Mat);
	void drawNoseOnImage(Mat, Rect, Rect);
	Mat returnNoseImage(Mat, Rect);
};

class MouthDetector
{
	CascadeClassifier mouthDetector;
	float searchScaleFactor;
	int minNeighbourCount;
	int searchFlags;
	Size minSearchSize;

public:
	MouthDetector();
	vector<Rect> storeMouthPos(Mat);
	void drawMouthOnImage(Mat, Rect, Rect);
	Mat returnMouthImage(Mat, Rect);
};

class EyeCenterTracker
{	
	Point centerWrtEyeImage, centerWrtFrame;

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
	int histR[256]; float cumHistR[256];

public:
	ImageProcessingMethods();
	Mat sizeReduce(Mat, int);
	Mat RGB2GRAY(Mat);
	Mat AdaptiveHistThresh(Mat, float);
};

class Snakuscule
{
	float alpha;
	int positions[50]; 
	float hashTable[640][640][50]; //Since frame width does not exist 640.
	std::vector<std::array<int, 3> > hashChanges;

public:
	Snakuscule();
	Point runSnakuscule(Mat, Point, int *);
	float snakeEnergy(Mat, int, float, int, int);
	float outerAnnulusEnergy(Mat, int, float alpha, int *);
	float innerCircleEnergy(Mat, int, int, int);
	float hashEnergy(Mat, int, int, int);
};

class EyeCornerDetector
{
	Mat erosionElement;	
	float eyeCornerThresh;
	
public:
	EyeCornerDetector();	
	Point returnLeftCornerPos(Mat, string);
	Point returnRightCornerPos(Mat, string);
	Point drawLeftEyeCorner(Mat, Rect, Rect, Point);
	Point drawRightEyeCorner(Mat, Rect, Rect, Point);
};

class EyeGazeEstimator
{
private:
	//int cornerDistance;
	float distanceBw2Points(Point, Point);
	float perpDistanceFromLine(Point, Point, Point);
	
public:
	EyeGazeEstimator();
	int eyeCornerDistance(Mat, Point, Point);
	int eyeCornerDistance(Point, Point);
	float verticalShift(Point, Point, Point, Point);
	float horizontalShift(Point, Point, Point, Point);
	
};

//Methods
void detectInImage(Mat);

//Global Variables
extern FaceDetection faceDetection;
extern EyeDetection eyeDetection;
extern ImageProcessingMethods imageProcessingMethods;
extern EyeCenterTracker eyeCenterTracker;
extern Snakuscule snakuscule;
extern EyeCornerDetector eyeCornerDetector;
extern EyeGazeEstimator eyeGazeEstimator;
extern NoseDetector noseDetector;
extern MouthDetector mouthDetector;
extern float horizontalShift, verticalShift;

#endif
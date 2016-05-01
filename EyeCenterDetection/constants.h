#ifndef _constants
#define _constants

#include<Windows.h>

/*
Reducing size of image for boosting face detection performance
*/
const int SMALLER_DETECTION_WIDTH = 320;

/*
Eye search region w.r.t to face image for boosting eye detection performance
*/
const float EYE_SX = 0.10, EYE_SY = 0.15, EYE_SH = 0.40, EYE_SW = 0.40;
//const float EYE_SX = 0.0, EYE_SY = 0.0, EYE_SH = 0.50, EYE_SW = 0.50;

/*
Ratio of Snakuscule radius to (Eye_Width + Eye_Height)
*/
const float R_RATIO = 0.09;

/*
Reducing height of the detected eye
*/
const float EW_RATIO = 0.60; //Eye width ratio

/*
Eye Corner search region w.r.t. to the detected eye region
*/
const float CORNER_W = 0.20, CORNER_X = 0.07, CORNER_Y = 0.20;
//const float CORNER_W = 0.20, CORNER_X = 0.05, CORNER_Y = 0.30;

/*
Nose search region w.r.t. the face image
*/
const float NOSE_SX = 0.30, NOSE_SY = 0.40, NOSE_SW = 0.40, NOSE_SH = 0.40;

#endif
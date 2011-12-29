/*
------------------------------------------------------------------------------------------------
   ofxOpticalFlow.h | ofxKinect | 07/02/2011
 
   created by MATT JAKOB - www.mattjakob.com
   Copyright 2011. All rights reserved.
------------------------------------------------------------------------------------------------
*/
#pragma once
#include "ofxOpenCv.h"

class ofxCvFlowPoint: public ofPoint{
public:
	ofPoint oldPosition;
	ofPoint difference;
	ofxCvFlowPoint(float _x, float _y, float oldX, float oldY):ofPoint(_x, _y){
		oldPosition.set(oldX, oldY);
		difference.set(x - oldPosition.x, y - oldPosition.y);
	}
	
	float getDistance(){
		return sqrt(difference.x*difference.x + difference.y*difference.y);
	}
	
	float getDistanceSquared(){
		return difference.x*difference.x + difference.y*difference.y;
	}
};

#define MAX_CORNERS 500


class ofxOpticalFlow{
protected:
	IplImage* eigImg, *tempImg, *pyrPrev, *pyrCurr;
	int width, height;
	float invWidth, invHeight;
	CvPoint2D32f * cornersPrev, *cornersCurr;
	char featuresFound[MAX_CORNERS];
	float featuresErrors[MAX_CORNERS];
	CvSize windowSize;
	
public:
	vector<ofxCvFlowPoint> flowPoints;
	
	ofxOpticalFlow();
	~ofxOpticalFlow();
	
	void setup(int _imgWidth, int _imgHeight, int _windowWidth = 5, int _windowHeight = 5);
	void calc(ofxCvGrayscaleImage& currentFrame, ofxCvGrayscaleImage& prevFrame);
	void draw(float x = 0.0f, float y = 0.0f); 
	void clear();
	vector<ofxCvFlowPoint>& getFlowPoints();
	int getNumFlowPoints();
	void scalePointsLocalToGlobal();
	void scalePoints(float scaleX, float scaleY);
	void scalePointsToScreen();
	void normalizePoints();
	void filterPoints(float minFlow, float maxFlow);
	ofPoint getAverageFlow();
};

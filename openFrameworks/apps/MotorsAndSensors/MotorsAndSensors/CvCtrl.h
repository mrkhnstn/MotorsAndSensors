#pragma once
/*
 *  CvCtrl.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 17/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "ofxOpenCv.h"

class CvCtrl : public ofxBaseObject {
	
public:
	
	int imgW;
	int imgH;
	
	bool doDraw;
	float drawScale;
	float rayDrawScale;
	float drawX;
	float drawY;
	
	ofxCvGrayscaleImage cvImgFront;
	ofxCvContourFinder contourFinder;
	int contourMinArea;
	int contourMaxArea;
	int maxContours;
	int blobsCount;
	
	int threshold;
	int smooth;
	
	bool doDrawTest;
	int testX;
	int testY;
	int testRadius;

	float localMinSensingZone;
	float localMaxSensingZone;
	
	bool doDrawRaw;
	bool doDrawBg;
	bool doDrawAdapted;
	bool doDrawHitScore;
	
	//
	float rayGap;
	float motorGap;
	float sensorOffset;
	float sensorGap;
	float rayOffset;

	
	ofTrueTypeFont font;
	
	void setup();
	void setupGUI();
	void postGUI();
	void update();
	void draw();
	void draw3d();
	
};

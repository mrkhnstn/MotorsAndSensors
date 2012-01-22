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
	
	float motorGap; // distance between adjacent motors in cv coordinate system

	float localMinSensingZone;
	float localMaxSensingZone;
	
	ofTrueTypeFont font;
	
	void setup();
	void setupGUI();
	void postGUI();
	void update();
	void draw();
	void draw3d();
	
};

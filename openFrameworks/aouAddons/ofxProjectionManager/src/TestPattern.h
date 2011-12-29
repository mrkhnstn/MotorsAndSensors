#pragma once
/*
 *  TestPattern.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 31/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "Boundary2f.h"

class TestPattern : public ofxBaseObject {

public:
	
	int cols;
	int rows;
	
	float colSize;
	float rowSize;
	
	bool doDraw;
	bool doDrawNumbers;
	bool doDrawBlank;
	ofColor blankColor;
	ofRectangle boundary;
	ofColor color;
	int lineWidth;
	int ellipseCount;
	
	void setup();
	
	void setupGUI();
	
	void postGUI();
	
	void update();
	
	void draw();
	
};

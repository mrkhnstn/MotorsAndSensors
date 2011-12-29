/*
 ------------------------------------------------------------------------------------------------
 Spaceh.h | TestBed | 27/01/2011
 
 created by MATT JAKOB - www.mattjakob.com
 Copyright 2011. All rights reserved.
 ------------------------------------------------------------------------------------------------
 */
#pragma once
#include "ofxVec2f.h"
#include "ofMain.h"

class CoordSpace{
public:
	
	ofxVec2f corners[4];
	float ratio;
	int width, height;
	
	CoordSpace(){}
	
	void set(int x, int y, int _width, int _height)
	{
		width  = _width;
		height = _height;
		
		corners[0].x = x;
		corners[0].y = y;
		corners[1].x = x + width;
		corners[1].y = y;
		corners[2].x = x;
		corners[2].y = y+height;
		corners[3].x = x+width;
		corners[3].y = y+height;
		
		ratio = float(width/height);
	}
	
	void set(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{
		corners[0].x = x1;
		corners[0].y = y1;
		corners[1].x = x2;
		corners[1].y = y2;
		corners[2].x = x3;
		corners[2].y = y3;
		corners[3].x = x4;
		corners[3].y = y4;
		
		height = y3 + ((y4 - y3)/2.0) - y1;
		width  = x2 + ((x4 - x2)/2.0) - x1;
		ratio = float(width/height);
		
	}
	
	void draw()
	{
		ofSetColor(255,255,0);
		ofNoFill();
		
		ofBeginShape();
		for(int i=0; i<4; i++)
			ofVertex(corners[i].x,corners[i].y);
		ofEndShape(true);
		
		ofEnableAlphaBlending();
		ofSetColor(255,255,0,80);
		ofFill();
		
		ofBeginShape();
		for(int i=0; i<4; i++)
			ofVertex(corners[i].x,corners[i].y);
		ofEndShape(true);
		
		ofSetColor(255,255,255,255);
	}
	
};

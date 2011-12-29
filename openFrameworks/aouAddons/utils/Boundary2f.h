/*
 *  Boundary2f.h
 *  backdrop_mark
 *
 *  Created by Mark Hauenstein on 28/02/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "ofxVec2f.h"
#include "ofxSimpleGuiToo.h"
//#include <math.h>

class Boundary2f {
	
public:
	
	ofxVec2f min;
	ofxVec2f max;
	
	Boundary2f(float _minX=0.0f, float _maxX=0.f, float _minY=0.0f, float _maxY=0.f);
	
	void set(Boundary2f& _boundary);
	
	void set (float _minX=0.0f, float _maxX=0.f, float _minY=0.0f, float _maxY=0.f);
	
	void set(ofxVec2f _min, ofxVec2f _max);
	
	ofxVec2f& getRandomPoint(ofxVec2f& p);
	
	ofxVec2f& getNormalized(ofxVec2f& p);
	
	bool contains(const ofxVec2f& p);
	
	bool contains(const float x_, const float y_);
	
	bool overlaps(Boundary2f& b_);
	
	float getWidth();
	
	float getHeight();
	
	void draw();
	
	void addToGUI(string _name, float _minX, float _maxX, float _minY, float _maxY);
	
};
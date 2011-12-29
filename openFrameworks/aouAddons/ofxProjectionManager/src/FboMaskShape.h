#pragma once
/*
 *  FboMaskShape.h
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 26/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxVec2f.h"
#include "FboMaskSelection.h"
#include "FboUtils.h"

#define FBOMASKCLICKDISTANCE	32

class FboMaskShape{
public:
	
	vector<ofxVec2f> points;
	
	ofColor color;
	
	float clickDistance;
	
	FboMaskShape();
	
	void draw(bool editing, bool invert);
	
	void drawPoints();
	
	void drawSelection(FboMaskSelection& selection, bool active, bool mouseOver);
	
	void shiftSelection(FboMaskSelection& selection, float dx, float dy);
	
	void drawPoint(ofxVec2f& point, bool active, bool mouseover, bool midPoint);
	
	ofxVec2f getMidPoint(int i);
	
	ofxVec2f getLinearPoint(float p);
	
	ofxVec2f getCurvePoint(float p);
	
	ofxVec2f noPoint();
	
	void drawShape();
	
	void drawShapeCurved();
	
	FboMaskSelection getNearestPoint(ofxVec2f& target);
	
	void insertPoint(FboMaskSelection& selection);
	
	void replacePoint(FboMaskSelection& selection);
	
	void deletePoint(FboMaskSelection& selection);
	
	bool isValidPoint(FboMaskSelection& selection);
	
};

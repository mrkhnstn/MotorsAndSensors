#pragma once
/*
 *  FboMaskSelection.h
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 26/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxVec2f.h"


class FboMaskSelection{
public:
	FboMaskSelection();
	
	float distance(ofxVec2f& target);
	
	void setPoint(ofxVec2f& target, int pointID = -1, int shapeID = -1);
	
	int shapeID;
	int pointID;
	ofxVec2f point;
	bool valid;
	bool midPoint;
};

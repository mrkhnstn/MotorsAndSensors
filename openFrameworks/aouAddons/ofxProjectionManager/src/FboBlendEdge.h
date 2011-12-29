#pragma once
/*
 *  FboBlendEdge.h
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 24/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"

#include "FboGammaTable.h"
#include "FboUtils.h"

class FboBlendEdge{
public:
	float width;
	
	bool enabled;
	float blackLevel;
	float blendPoint;
	float shape;
	
	ofPoint position;
	ofPoint blendDirection;
	ofPoint blendTangent;
	
	FboGammaTable gammaR;
	FboGammaTable gammaG;
	FboGammaTable gammaB;
	
	float height; ///< used for debug drawing
	
	FboBlendEdge();
	
	void setAngle(float theta);
	
	void setGamma(float r, float g, float b);
	
	void setupGUI(string name="Edge");
	
	bool operator== (FboBlendEdge& b);
	
	void copyFrom(FboBlendEdge& b);
	
	float blend(ofPoint p);
	
	float blendRelative(float p);
	
	void draw();
};
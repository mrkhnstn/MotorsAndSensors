#pragma once
/*
 *  FboWarpGrid.h
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 24/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxVec2f.h"
#include "Utils.h"
#include "FboUtils.h"

class FboWarpGrid {
public:
	
	void setup(ofRectangle window, ofRectangle src, ofRectangle dst, int cols = 1, int rows = 1);
	
	void reset();
	
	bool isValidPoint(int index);
	
	bool operator== (FboWarpGrid& b);
	
	void copyFrom(FboWarpGrid& b);
	
	void saveXML(string path);
	
	void loadXML(string path);
	
	ofxVec2f forwardsRadial(ofxVec2f& before);
	
	void buildGrids(bool original, bool target, bool warp);	
	
	void buildRadialGrid();
	
	ofxVec2f originalToTarget(ofxVec2f& original);
	
	ofxVec2f targetToOriginal(ofxVec2f& target);
	
	int rows, cols;
	
	ofRectangle window;
	ofRectangle src, dst;
	
	vector<ofxVec2f> originalGrid;
	vector<ofxVec2f> targetGrid;
	vector<ofxVec2f> warpGrid;
	vector<ofxVec2f> warpedLensedGrid;
	
	ofxVec2f shift;
	ofxVec2f stretchTL, stretchTR, stretchBR, stretchBL;
	
	float radialAmount;
	float radialInverseAmount;
	float radialCenterOffsetX;
	float radialCenterOffsetY;
	float radialScale;
	float radialBias;
	//float reverseRadialAmount;
	
};

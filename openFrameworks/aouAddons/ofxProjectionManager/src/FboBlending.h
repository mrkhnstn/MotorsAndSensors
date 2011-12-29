#pragma once
/*
 *  FboBlending.h
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 16/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ImageUtil.h"

#include "FboGammaTable.h"
#include "FboBlendEdge.h"

class FboWarpOutput;

class FboBlending{
public:
	
	FboBlending();
	
	void setup(ofRectangle window, float borderH, float borderV);
	
	void setBackgroundImage(ofImage* bg);
	
	void backgroundUpdated();
	
	void redraw(ofRectangle window);
	
	void draw(FboWarpOutput* output);
	
	void update(bool forceUpdate = false);
	
	void buildImage();
	
	void buildEdge(FboBlendEdge& edge);
	
	void buildLevelsMap(); ///< separates mask into two areas that use different levels correction
	
	void setupGUI(string subname="A");
		
	string parentName;
	
	float gammaR, gammaG, gammaB;
	
	ofImage* backgroundImage;
	ofImage blendImage;
	ofImage blackImage;
	
	bool doDraw;
	
	FboBlendEdge top, bottom, left, right;
	ofRectangle rect;  
	
	int gammaTop, gammaBottom, gammaLeft, gammaRight;
	
	int _gammaTop, _gammaBottom, _gammaLeft, _gammaRight;
	
protected:
	
	FboBlendEdge _top, _bottom, _left, _right;
};


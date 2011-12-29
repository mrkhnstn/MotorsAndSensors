#pragma once
/*
 *  FboGammaTable.h
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 24/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"


class FboGammaTable{
	
public:
	
	float g;
	
	void setGamma(float gam);
	
	float gammaNormalized(float i);
	
	float igammaNormalized(float i);
	
	float gamma(float i);
	
	float igamma(float i);
	
	float apply(float i, float a, float black);
	
	ofColor apply(ofColor c, float a, float black);
	
	bool operator== (FboGammaTable& b);
	
};


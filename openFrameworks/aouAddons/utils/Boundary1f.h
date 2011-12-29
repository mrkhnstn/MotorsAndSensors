#pragma once
/*
 *  Boundary1f.h
 *  backdrop_mark
 *
 *  Created by Mark Hauenstein on 27/02/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"

class Boundary1f {

public:
	
	float min;
	float max;
	
	Boundary1f(float _min=0.0f, float _max=0.0f);	
	
	Boundary1f(const Boundary1f& mom);
	
	float set(const Boundary1f& mom);
	
	float set(float _min, float _max);
	
	float getRandomValue();
	
	float getNormalized(float f);
	
	float getMapped(float f, float min_, float max_);
	
	bool contains(float f);

	void addToGUI(string _name, float _min, float _max);
	
};
#pragma once
/*
 *  TextureForceField.h
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 13/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "ForceField2f.h"
#include "ofMain.h"
#include "ofxOpenCV.h"
#include "TextureField2f.h"

class TextureForceField2f : public ForceField2f, public TextureField2f{
public:
	
	TextureForceField2f();
	~TextureForceField2f();
	
	void setup(int width, int height);
	void clearBucket(int k);
	
	void addField(ForceField2f& field);
	//void clear();
	
	ofxVec2f getForce(ofxVec2f& pos_);
	
	//float x, y;
	float strength;
	
	//virtual void applyTo(ofVerletPoint& point);
	
	//float s;//strength
	//float mid;
	//int clamping;
	//int width;
	//int height;
	
	//float sx, sy;//scaling x/y, offset x, y
	
	bool allocated;
	//bool interpolate;
	
	float * fx;
	float * fy;

protected:
	//void clamp(int& x, int& y, bool& affect);
	//virtual void _applyTo(ofVerletPoint& point, float& strength, int& i, int& j);
	ofxVec2f _getForce(int& i, int& j);
};
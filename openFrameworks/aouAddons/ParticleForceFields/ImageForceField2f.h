#pragma once
/*
 *  ImageForceField2f.h
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 14/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ForceField2f.h"
#include "ofMain.h"
#include "TextureProcessor.h"
#include "Singleton.h"

class ImageForceField2f: public ForceField2f{
public:
	
	ImageForceField2f(){
		x = 0;
		y = 0;
		sx = 1;
		sy = 1;
		isx = 1;
		isy = 1;
		width = 0;
		height = 0;
		strength = 0;
		blur = 9;
		textureField = NULL;
		enabled = true;
	}
	
	~ImageForceField2f(){
		reset();
	}
	
	void reset(){
		if(textureField != NULL){
			delete textureField;
		}
	}
	void setup(string path, int colorChannel){
		ofImage image;
		image.loadImage(path);
		setup(image, colorChannel);
		image.clear();
	}
	
	void setup(ofImage& image, int colorChannel){
		reset();
		sx = sy = 1;//bucketSize;
		isx = 1.0/sx;
		isy = 1.0/sy;
		blur = blur%2?blur:blur+1;
		
		width = image.width;
		height = image.height;
		
		textureField = new TextureForceField2f();
		textureField->setup(width, height);
		
		TextureProcessor* processor = Singleton<TextureProcessor>::instance();
		processor->setupTextureForceField2f(*textureField, image, colorChannel, blur);
	}
	
	void update(){
		textureField->x = x;
		textureField->y = y;
		textureField->sx = sx;
		textureField->sy = sy;
		textureField->strength = strength;
	}
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		//ofxVec2f force;
		return textureField->getForce(pos_);
		//return force;
	}
	
	TextureForceField2f* textureField;
	
	int blur;
	int width;
	int widthStep;
	int height;
	float strength;
	float x, y, sx, sy, isx, isy;
	
	bool enabled;
	
	
};
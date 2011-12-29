#pragma once
/*
 *  SpreadForceField2f.h
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 13/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ForceField2f.h"
#include "TextureProcessor.h"
#include "ofxOpenCv.h"
#include "TextureForceField2f.h"

class SpreadForceField2f: public ForceField2f{
public:
	SpreadForceField2f(){
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
		interpolate = false;
		spreadImage = NULL;
		textureField = NULL;
		textureProcessor = NULL;
		enabled = true;
	}
	
	~SpreadForceField2f(){
		reset();
	}
	
	void reset(){
		if(spreadImage != NULL){
			cvReleaseImage(&spreadImage);
		}
		if(textureProcessor != NULL){
			delete textureProcessor;
		}
		if(textureField != NULL){
			delete textureField;
		}
	}
	
	void setup(float width, float height, float bucketSize, float offsetX = 0, float offsetY = 0){
		reset();
		
		x = offsetX;
		y = offsetY;
		sx = sy = bucketSize;
		
		width /= sx;
		height /= sy;
		
		this->width = width;
		this->height = height;
		
		spreadImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1);
		widthStep = spreadImage->widthStep;
		
		textureProcessor = new TextureProcessor();
		textureProcessor->setup(width, height, blur);
		
		textureField = new TextureForceField2f();
		textureField->setup(width, height);
	}
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		//ofxVec2f force;
		return textureField->getForce(pos_);
		//return force;
	}
	
	void preprocess(){
		isx = 1.0/sx;
		isy = 1.0/sy;
		blur = blur%2?blur:blur+1;
		cvSetZero(spreadImage);
	}
	void accumulate(ofxVec2f& point, float mass){
		float fx = (point.x - this->x)*isx;
		float fy = (point.y - this->y)*isy;
		int x = fx;
		int y = fy;
		if(interpolate){
			float dx = fx - x;
			float dy = fy - y;
			float idx = 1.0f - dx;
			float idy = 1.0f - dy;
			
			_accumulate(x, y, mass*idx*idy);
			_accumulate(x+1, y, mass*dx*idy);
			_accumulate(x, y+1, mass*idx*dy);
			_accumulate(x+1, y+1, mass*dx*dy);
			
		} else {
			_accumulate(x, y, mass);
		}
		
		
	}
	void process(){
		textureProcessor->process(spreadImage);
		TextureProcessor::copyCVImageToFloatArray(textureField->fx, textureProcessor->attractx);
		TextureProcessor::copyCVImageToFloatArray(textureField->fy, textureProcessor->attracty);
		textureField->strength = strength;
		textureField->x = this->x;
		textureField->y = this->y;
		textureField->sx = sx;
		textureField->sy = sy;
	}
		
	TextureProcessor* textureProcessor;
	IplImage* spreadImage;
	TextureForceField2f* textureField;
	
	int blur;
	int width;
	int widthStep;
	int height;
	bool interpolate;
	float strength;
	float x, y, sx, sy, isx, isy;
	
	bool enabled;
protected:
	void _accumulate(int x, int y, float mass){
		bool affect;
		textureField->clamp(x, y, affect);
		if(affect){
			spreadImage->imageData[x+y*widthStep] += mass;
		}
		//if(x>=0 && x < width && y >= 0 && y < height){
		//	spreadImage->imageData[x + y*widthStep] += mass;
		//}
	}
	
};

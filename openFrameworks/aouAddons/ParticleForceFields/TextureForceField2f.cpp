/*
 *  TextureForceField.cpp
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 13/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "TextureForceField2f.h"

#include "ofMain.h"

TextureForceField2f::TextureForceField2f(){
	//mid = 0;
	allocated = false;
	//s = 1;
	//x = y = 0;
	//sx = sy = 2;
	
	//clamping = TEXTURE_FIELD_FINITE;
	//interpolate = false;
	//allocated = false;
}

TextureForceField2f::~TextureForceField2f(){
	if(allocated){
		delete[] fx;
		delete[] fy;
	}
}
void TextureForceField2f::setup(int w, int h)
{
	if(!allocated || w != width || h != height){
		
		if(allocated){
			delete[] fx;
			delete[] fy;
			allocated = false;
		}
		
		fx = new float[w*h];
		fy = new float[w*h];
		
		allocated = true;
	}
	TextureField2f::setup(w, h);
}

void TextureForceField2f::addField(ForceField2f& field){
	ofxVec2f point;
	ofxVec2f force;
	int i, j, k, w, h;	
	for(j = 0; j < height; ++j){
		for(i = 0; i < width; ++i){
			//point.jump(i*sx + x, j*sy + y);
			//point.f.set(0, 0);
			//effector.applyTo(point);
			point.set(i*sx + x, j*sy + y);
			force = field.getForce(point);
			k = i+width*j;
			//			if(point.f.x > 0.1){
			//				_T("IT IS NOT ZERO");
			//			}
			fx[k] += point.x;///s;
			fy[k] += point.y;///s;
		}
	}
}
void TextureForceField2f::clearBucket(int k){
	fx[k] = 0;
	fy[k] = 0;
}


ofxVec2f TextureForceField2f::getForce(ofxVec2f& p){
	float px = (p.x - x)/sx;
	float py = (p.y - y)/sy;
	int pi = px;
	int pj = py;
	if(interpolate){
		px -= pi;
		py -= pj;
		float ipx = 1-px;
		float ipy = 1-py;
		float s;
		
		ofxVec2f force;
		for(int a = 0; a < 2; ++a){
			int i = a+pi;
			float dx = a==0?ipx:px;
			for(int b = 0; b < 2; ++b){
				int j = b+pj;
				float dy = b==0?ipy:py;
				s = dx*dy;
				force += s*_getForce(i, j);
			}
		}
		return strength*force;
	} else {
		int i = pi;
		int j = pj;
		return strength*_getForce(i, j);
	}
}
ofxVec2f TextureForceField2f::_getForce(int& i, int& j){
	ofxVec2f force;
	bool affect;
	clamp(i, j, affect);
	if(affect){
		int index = i+j*width;
		force.x = fx[index];
		force.y = fy[index];
	}
	return force;
}

#pragma once
/*
 *  TextureField2f.h
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 16/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */



#include "ForceField2f.h"
#include "ofMain.h"
#include "ofxOpenCV.h"
#include "ofxVec2f.h"

#define TEXTURE_FIELD_CLAMP			0
#define TEXTURE_FIELD_REPEAT		1
#define TEXTURE_FIELD_FINITE		2
#define TEXTURE_FIELD_REPEAT_MOD(i, m)		(i<0)?((m-(-i)%m)%m):(i%m)

#define TEXTURE_FIELD_FORCE		0
#define TEXTURE_FIELD_SHIFT		1

#define TEXTURE_FIELD_STANDARD_MODE	0
#define TEXTURE_FIELD_NORMAL_MODE	1 // uses nRadius factor of point

//template<class T, int N>
class TextureField2f{
public:
	
	TextureField2f();
	~TextureField2f();
	
	virtual void setup(int width, int height);
	virtual void clearBucket(int k);

	void clear();
	
	float x, y;
	float mid;
	int clamping;
	int width;
	int height;
	
	float sx, sy;//scaling x/y, offset x, y
	
	bool interpolate;
	
	//float * fx;
	//float * fy;
	void clamp(int& x, int& y, bool& affect);
	
	int getTextureSize();
	ofxVec2f getWorldCoordForIndex(int k);
	ofxVec2f getWorldCoordForAddress(int i, int j);
	
protected:
	//virtual void _applyTo(ofVerletPoint& point, float& strength, int& i, int& j);
	//ofxVec2f _getForce(float& strength, int& i, int& j);
};


template <class T>
class TextureField2fT: public TextureField2f{
public:
	TextureField2fT(){
		allocated = false;
		strength = 1;
	};
	~TextureField2fT(){
		if(allocated){
			buckets.clear();
		}
	};
	
	void setup(int width, int height){
		if(!allocated || this->width != width || this->height != height){
			
			if(allocated){
				buckets.clear();
				allocated = false;
			}
			
			buckets.resize(width*height);
			allocated = true;
		}
		TextureField2f::setup(width, height);
		
	};
	void clearBucket(int k){
		T t;
		buckets[k] = t;
	}
	
	T getBucket(ofxVec2f& p){
		if(interpolate){
			float px = (p.x - x)/sx;
			float py = (p.y - y)/sy;
			int pi = px;
			int pj = py;
			px -= pi;
			py -= pj;
			float ipx = 1-px;
			float ipy = 1-py;
			float s;
			
			T t;
			for(int a = 0; a < 2; ++a){
				int i = a+pi;
				float dx = a==0?ipx:px;
				for(int b = 0; b < 2; ++b){
					int j = b+pj;
					float dy = b==0?ipy:py;
					s = dx*dy;
					t += s*_getBucket(i, j);
				}
			}
			return strength*t;
		} else {
			int i = (p.x - x)/sx;
			int j = (p.y - y)/sy;
			return strength*_getBucket(i, j);
		}
	};
	
	void setBucket(ofxVec2f& p, T t){
		int i = (p.x - x)/sx;
		int j = (p.y - y)/sy;
		bool affect;
		clamp(i, j, affect);
		if(affect){
			int index = i+j*width;
			buckets[index] = t;
		}
		//return strength*_getBucket(i, j);
	}

	float strength;
	bool allocated;
	vector<T> buckets;
protected:
	T _getBucket(int& i, int& j){
		T t;
		bool affect;
		clamp(i, j, affect);
		if(affect){
			int index = i+j*width;
			t = buckets[index];
		}
		return t;
	};
};
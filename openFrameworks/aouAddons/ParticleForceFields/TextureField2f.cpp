/*
 *  TextureField2f.cpp
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 16/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "TextureField2f.h"

#include "ofMain.h"

TextureField2f::TextureField2f(){
	mid = 0;
	//s = 1;
	x = y = 0;
	sx = sy = 2;
	
	clamping = TEXTURE_FIELD_FINITE;
	interpolate = false;
}

TextureField2f::~TextureField2f(){
	//if(allocated){
	//	delete[] fx;
	//	delete[] fy;
	//}
}

void TextureField2f::setup(int w, int h)
{
	width = w;
	height = h;

}

void TextureField2f::clear(){
	int i, j, k;
	for(j = 0; j < height; ++j){
		for(i = 0; i < width; ++i){
			k = i+width*j;
			clearBucket(k);
			//fx[k] = 0;
			//fy[k] = 0;
		}
	}
}
void TextureField2f::clearBucket(int k){
};


void TextureField2f::clamp(int& i, int& j, bool& affect){
	switch(clamping){
		case TEXTURE_FIELD_CLAMP:
			if(i < 0){
				i = 0;
			} else if(i >= width){
				i = width-1;	
			}
			if(j < 0){
				j = 0;
			} else if (j >= height){
				j = height-1;
			}
			affect = true;
			break;
		case TEXTURE_FIELD_REPEAT:
			i = TEXTURE_FIELD_REPEAT_MOD(i, width);
			j = TEXTURE_FIELD_REPEAT_MOD(j, height);
			affect = true;
			break;
		case TEXTURE_FIELD_FINITE:
		default:
			if(i < 0 || i >= width || j < 0 || j >= height){
				affect = false;
				return;
			} else {
				affect = true;
			}
			break;
	}
}

int TextureField2f::getTextureSize(){
	return width*height;
}

ofxVec2f TextureField2f::getWorldCoordForIndex(int k){
	return getWorldCoordForAddress(TEXTURE_FIELD_REPEAT_MOD(k, width), k/width);
}
ofxVec2f TextureField2f::getWorldCoordForAddress(int i, int j){
	return ofxVec2f(sx*(i+x), sy*(j+y));
}
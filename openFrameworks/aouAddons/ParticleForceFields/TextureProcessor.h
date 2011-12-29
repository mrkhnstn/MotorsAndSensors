#pragma once
/*
 *  Attractor.h
 *  openFrameworks
 *
 *  Created by Robin Beitra (robin@beitra.net) on 03/02/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxOpenCV.h"
#include "ofMain.h"
#include "TextureForceField2f.h"

class TextureProcessor{
public:
	TextureProcessor(){
		width = 0;
		height = 0;
		blur = 0;
		allocated = false;
		
		// added by mark
		temp = NULL;
		temp2 = NULL;
		temp3 = NULL;
		mask = NULL;
		iplImage = NULL;
		attractx = NULL;
		attracty = NULL;
	}
	
	void setup(int _width, int _height, int _blur);
	void process(IplImage* src);
	void processSpread(IplImage* src);
	IplImage* attractx;
	IplImage* attracty;
	
	// public members added by mark
	void ofTOcvGreyscaleImage(ofImage* src, IplImage* dest, int channel);	
	void setupTextureForceField2f(TextureForceField2f& textureField, ofImage& image, int channel, int blur);
	
	static void copyCVImageToFloatArray(float* target, IplImage* source){
		//assume there is a single channel
		int cols = source->width;
		int rows = source->height;
		int rowSize = sizeof(float)*cols;
		int widthStep = source->widthStep;
		for(int i = 0; i < rows; ++i){
			memcpy((unsigned char*)(target+i*cols), (unsigned char*)(source->imageData+i*widthStep), rowSize);
		}
	}
	static void copyCVImageToCharArray(unsigned char* target, IplImage* source){
		//assume there is a single channel
		int cols = source->width;
		int rows = source->height;
		int rowSize = sizeof(unsigned char)*cols;
		int widthStep = source->widthStep;
		for(int i = 0; i < rows; ++i){
			memcpy((unsigned char*)(target+i*cols), (unsigned char*)(source->imageData+i*widthStep), rowSize);
		}
	}
	static void copyOFImageToFloatArray(float* target, ofImage* src, int channel, float scaling = 1.0f){
		
		int w = src->getWidth();
		int h = src->getHeight();
		//ofxCvGrayscaleImage* dest = ofGreyscaleImageAtSize(w, h);
		
		unsigned char* srcpixels = src->getPixels();
		float* destpixels = target;//(unsigned char*)dest->imageData;
		
		int sstep = src->bpp/8;
		int dstep = 1;//dest->nChannels;
		int max = w*h;
		int i;
		
		if(channel == 0){
			for(i = 0; i < max; ++i){
				destpixels[i*dstep] = scaling*srcpixels[i*sstep];
			}
		} else {
			for(i = 0; i < max; ++i){
				destpixels[i*dstep] = scaling*srcpixels[i*sstep + channel];
			}
		}
		//for(int i = 0; i < rows; ++i){
		//	memcpy((unsigned char*)(target+i*cols), (unsigned char*)(source->imageData+i*widthStep), rowSize);
		//}
	}
private:
	int width;
	int height;
	int blur;
	bool allocated;
	
	IplImage* temp;
	IplImage* temp2;
	IplImage* temp3;
	IplImage* mask;
	
	IplImage* iplImage;
	void setupIplImage(int width, int height);
	void clearIplImage();
};

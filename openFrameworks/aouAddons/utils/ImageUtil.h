#pragma once
/*
 *  ImageUtil.h
 *  openFrameworks
 *
 *  Created by Mark Hauenstein on 13/12/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#include "ofMain.h"

class ImageUtil {

public:
	static void getPixel(ofImage& image, int x, int y, unsigned char pixel[4]);
	static void setPixel(unsigned char * destPixels, int x, int y, int w, int h, unsigned char srcPixel[4]);
	static void setPixel(unsigned char * destPixels, int destX, int destY, int destW, int destH, unsigned char * srcPixels, int srcX, int srcY, int srcW, int srcH);
	
	/*!
	 Copies pixeld data from one ofImage to another. Note, no boundary checking is done on the parameters giving for source and destination rectangle values.
	 The Argument updateDestImage is by default true. 
	 When copying data into the same destination image multiple times, it is recommended to set updateDestImage to false and update the destination image manually after all copying has been done.
	 */
	static void copyPixel(ofImage& srcImage, int srcLeft, int srcTop, int srcWidth, int srcHeight, ofImage& destImage, int destLeft, int destTop, bool updateDestImage);
	
	/*!
	 pointX, pointY: usually position of particle or sprite
	 refX, refY, refWidth, refHeight: defines reference rectangle of bitmap on stage
	 px, pxWidth, pxHeight: defines array that represents a rgb bitmap
	 rgb: pointer to rgb array that shall be changed if particle point is within reference rectangle, otherwise rgb stays the same
	 */
	static void getNormalizedRgb(float pointX, float pointY, float refX, float refY, float refWidth, float refHeight, unsigned char * px, int pxWidth, int pxHeight, float * rgb);

	static float getNormalizedGrayscale(float pointX, float pointY, float refX, float refY, float refWidth, float refHeight, unsigned char * px, int pxWidth, int pxHeight);
	
	static void getNormalizedRgba(float pointX, float pointY, float refX, float refY, float refWidth, float refHeight, unsigned char * px, int pxWidth, int pxHeight, float * rgba);
	
	static void colorMatrix(ofImage& image,
							float rr, float rg, float rb, float ra, float r,
							float gr, float gg, float gb, float ga, float g,
							float br, float bg, float bb, float ba, float b,
							float ar, float ag, float ab, float aa, float a);
	
	static ofPoint getMatchingPoint(ofImage& image,
									unsigned char rMin, unsigned char rMax,
									unsigned char gMin, unsigned char gMax,
									unsigned char bMin, unsigned char bMax,
									unsigned char aMin, unsigned char aMax,
									int maxTries=16
									);
	
	static void getPixelFromAnyImage(ofImage& image, int x, int y, unsigned char pixel[4]);
	
	static void setPixels(unsigned char * destPixels, int imagew, int imageh, int x, int y, int w, int h, unsigned char srcPixel[4]);
	
	static void flipVertically(ofImage& image);
	
	static void grabFBO(ofImage& image, int _x, int _y, int _w, int _h);	
	
	static ofColor blendColors(const ofColor& a, const ofColor& b, ofColor& result, float blending = 0.5);
	
	static void colorSet(ofColor& color, float r, float g, float b, float a);

};
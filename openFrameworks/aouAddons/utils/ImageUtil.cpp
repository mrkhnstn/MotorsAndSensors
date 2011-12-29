/*
 *  ImageUtil.cpp
 *  openFrameworks
 *
 *  Created by Mark Hauenstein on 13/12/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageUtil.h"

void ImageUtil::getPixel(ofImage& image, int x, int y, unsigned char * pixel)
{
	unsigned char * srcPx = image.getPixels();
	for(int k=0; k<4; k++)
		pixel[k] = srcPx[ ((y * image.width * 4) + x*4) + k];
}

void ImageUtil::setPixel(unsigned char * destPixels, int x, int y, int w, int h, unsigned char srcPixel[4])
{
	for(int k=0; k<4; k++)
		destPixels[ (y	* w * 4) + (x*4) + k] = srcPixel[k];
}

void ImageUtil::setPixel(unsigned char * destPixels, int destX, int destY, int destW, int destH, unsigned char * srcPixels, int srcX, int srcY, int srcW, int srcH)
{
	for(int k=0; k<4; k++)
		destPixels[ (destY	* destW * 4) + (destX*4) + k] = srcPixels[ ((srcY * srcW * 4) + srcX*4) + k];
}

void ImageUtil::copyPixel(ofImage& srcImage, int srcLeft, int srcTop, int srcWidth, int srcHeight, ofImage& dstImage, int dstLeft, int dstTop, bool updateDestImage = true)
{
	unsigned char * srcPx = srcImage.getPixels();
	unsigned char * dstPx = dstImage.getPixels();
		
	ofColor color;
	
	int srcBPP = srcImage.bpp/8;
	int dstBPP = dstImage.bpp/8;
	
	//int incx = srcWidth>=0?1:-1;
	//int incy = srcHeight>=0?1:-1;
	
	//for(int sy = srcTop; sy < srcTop + srcHeight; sy++)
	//	for(int sx = srcLeft; sx < srcLeft + srcWidth; sx++)
	for(int dy = 0; dy < srcHeight; dy += 1){
		for(int dx = 0; dx < srcWidth; dx += 1){
			int srcx = dx+srcLeft;
			int srcy = dy+srcTop;
			int dstx = dx+dstLeft;
			int dsty = dy+dstTop;
			int srcOffset = (srcx+srcy*srcImage.width)*srcBPP;
			int dstOffset = (dstx+dsty*dstImage.width)*dstBPP;
			
			switch(srcImage.type){
				case OF_IMAGE_GRAYSCALE:
					color.r = color.g = color.b = srcPx[srcOffset];
					color.a = 255;
					break;
				case OF_IMAGE_COLOR:
					color.r = srcPx[srcOffset];
					color.g = srcPx[srcOffset+1];
					color.b = srcPx[srcOffset+2];
					color.a = 255;
					break;
				case OF_IMAGE_COLOR_ALPHA:
					color.r = srcPx[srcOffset];
					color.g = srcPx[srcOffset+1];
					color.b = srcPx[srcOffset+2];
					color.a = srcPx[srcOffset+3];
					break;
			}
			
			switch(dstImage.type){
				case OF_IMAGE_GRAYSCALE:
					dstPx[dstOffset] = color.r;
					break;
				case OF_IMAGE_COLOR:
					dstPx[dstOffset] = color.r;
					dstPx[dstOffset+1] = color.g;
					dstPx[dstOffset+2] = color.b;
					break;
				case OF_IMAGE_COLOR_ALPHA:
					dstPx[dstOffset] = color.r;
					dstPx[dstOffset+1] = color.g;
					dstPx[dstOffset+2] = color.b;
					dstPx[dstOffset+3] = color.a;
					break;
			}
			//dx = destLeft + (sx - srcLeft);
			//dy = destTop + (sy - srcTop);
			//for(int k=0; k<4; k++)
			//	destPx[ (dy	* destImage.width * 4) + (dx*4) + k] = srcPx[ ((sy * srcImage.width * 4) + sx*4) + k];
		}
	}
	//if(updateDestImage)
	dstImage.update();
}

void ImageUtil::getNormalizedRgb(float pointX, float pointY, float refX, float refY, float refWidth, float refHeight, unsigned char * px, int pxWidth, int pxHeight, float * rgb)
{
	float rx = (pointX - refX) / refWidth;
	float ry = (pointY - refY) / refHeight;
	
	if(rx > 0 && rx < 1 && ry > 0 && ry < 1){
		int x = (int)(rx * (pxWidth - 1));
		int y = (int)(ry * (pxHeight - 1));
		unsigned char * p;
		p = px + (y * pxWidth * 3) + (x * 3);
		rgb[0] = (float)(*p) / 255;
		rgb[1] = (float)(*++p) / 255;
		rgb[2] = (float)(*++p) / 255;
	}
}

void ImageUtil::getNormalizedRgba(float pointX, float pointY, float refX, float refY, float refWidth, float refHeight, unsigned char * px, int pxWidth, int pxHeight, float * rgba)
{
	float rx = (pointX - refX) / refWidth;
	float ry = (pointY - refY) / refHeight;
	
	if(rx >= 0 && rx <= 1 && ry >= 0 && ry <= 1){
		int x = (int)(rx * (pxWidth - 1));
		int y = (int)(ry * (pxHeight - 1));
		unsigned char * p;
		p = px + (y * pxWidth * 4) + (x * 4);
		rgba[0] = (float)(*p) / 255;
		rgba[1] = (float)(*++p) / 255;
		rgba[2] = (float)(*++p) / 255;
		rgba[3] = (float)(*++p) / 255;
	}
}

float ImageUtil::getNormalizedGrayscale(float pointX, float pointY, float refX, float refY, float refWidth, float refHeight, unsigned char * px, int pxWidth, int pxHeight)
{
	float rx = (pointX - refX) / refWidth;
	float ry = (pointY - refY) / refHeight;
	
	if(rx > 0 && rx < 1 && ry > 0 && ry < 1){
		int x = (int)(rx * (pxWidth - 1));
		int y = (int)(ry * (pxHeight - 1));
		unsigned char * p;
		p = px + (y * pxWidth) + x;
		return (float)(*p) / 255;
	} else {
		return 0;
	}
}

ofColor ImageUtil::blendColors(const ofColor& a, const ofColor& b, ofColor& result, float blending){
	float iblending = 1 - blending;
	result.r = iblending*a.r + blending*b.r;
	result.g = iblending*a.g + blending*b.g;
	result.b = iblending*a.b + blending*b.b;
	result.a = iblending*a.a + blending*b.a;
}

void ImageUtil::colorSet(ofColor& color, float r, float g, float b, float a){
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

void ImageUtil::colorMatrix(ofImage& image,
						float rr, float rg, float rb, float ra, float r,
						float gr, float gg, float gb, float ga, float g,
						float br, float bg, float bb, float ba, float b,
						float ar, float ag, float ab, float aa, float a){
	if(image.type == OF_IMAGE_COLOR_ALPHA){
		int w = image.getWidth();
		int h = image.getHeight();
		
		float _r, _g, _b, _a;
		unsigned char r_, g_, b_, a_;
		unsigned char* pixels = image.getPixels();
		int i;
		int max = w*h*4;
		for(i = 0; i < max; i+=4){
			r_ = pixels[i];
			g_ = pixels[i+1];
			b_ = pixels[i+2];
			a_ = pixels[i+3];
			
			_r = r + rr*r_ + rg*g_ + rb*b_ + ra*a_;
			_g = g + gr*r_ + gg*g_ + gb*b_ + ga*a_;
			_b = b + br*r_ + bg*g_ + bb*b_ + ba*a_;
			_a = a + ar*r_ + ag*g_ + ab*b_ + aa*a_;
			
			pixels[i] = _r;
			pixels[i+1] = _g;
			pixels[i+2] = _b;
			pixels[i+3] = _a;
		}
		image.update();
	}
}

ofPoint ImageUtil::getMatchingPoint(ofImage& image,
								unsigned char rMin, unsigned char rMax,
								unsigned char gMin, unsigned char gMax,
								unsigned char bMin, unsigned char bMax,
								unsigned char aMin, unsigned char aMax,
								int maxTries){
	ofPoint point;
	unsigned char pixel[4];
	float maxX = image.getWidth() - 0.001;//make sure we don't overflow?
	float maxY = image.getHeight() - 0.001;
	while(--maxTries >= 0){
		point.x = ofRandom(0, maxX);
		point.y = ofRandom(0, maxY);
		getPixelFromAnyImage(image, point.x, point.y, pixel);
		if(pixel[0] >= rMin && pixel[0] <= rMax &&
		   pixel[1] >= gMin && pixel[1] <= gMax &&
		   pixel[2] >= bMin && pixel[2] <= bMax &&
		   pixel[3] >= aMin && pixel[3] <= aMax){
			break;
		}
	}
	return point;
}

void ImageUtil::getPixelFromAnyImage(ofImage& image, int x, int y, unsigned char pixel[4]){
	unsigned char * srcPx = image.getPixels();
	int srcBpp = image.bpp/8;
	int srcOffset = (x+y*image.width)*srcBpp;
	switch(image.type){
		case OF_IMAGE_GRAYSCALE:
			pixel[0] = pixel[1] = pixel[2] = srcPx[srcOffset];
			pixel[3] = 255;
			break;
		case OF_IMAGE_COLOR:
			pixel[0] = srcPx[srcOffset];
			pixel[1] = srcPx[srcOffset+1];
			pixel[2] = srcPx[srcOffset+2];
			pixel[3] = 255;
			break;
		case OF_IMAGE_COLOR_ALPHA:
			pixel[0] = srcPx[srcOffset];
			pixel[1] = srcPx[srcOffset+1];
			pixel[2] = srcPx[srcOffset+2];
			pixel[3] = srcPx[srcOffset+3];
			break;
	}
}

void ImageUtil::setPixels(unsigned char * destPixels, int imagew, int imageh, int x, int y, int w, int h, unsigned char srcPixel[4])
{
	int maxx = x + w;
	int maxy = y + h;
	for(int j = 0; j < maxy; ++j){
		for(int i = x; i < maxx; ++i){
			int index = (i+j*imagew)*4;
			destPixels[index] = srcPixel[0];
			destPixels[index+1] = srcPixel[1];
			destPixels[index+2] = srcPixel[2];
			destPixels[index+3] = srcPixel[3];
		}
	}
}

void ImageUtil::flipVertically(ofImage& image){
	int sizeOfOneLineOfPixels = image.width * image.bpp/8;
	unsigned char* pixels = image.getPixels();
	unsigned char * tempLineOfPix = new unsigned char[sizeOfOneLineOfPixels];
	unsigned char * linea;
	unsigned char * lineb;
	int max = image.height/2;
	for (int i = 0; i < max; i++){
		linea = pixels + i * sizeOfOneLineOfPixels;
		lineb = pixels + (image.height-i-1) * sizeOfOneLineOfPixels;
		memcpy(tempLineOfPix, linea, sizeOfOneLineOfPixels);
		memcpy(linea, lineb, sizeOfOneLineOfPixels);
		memcpy(lineb, tempLineOfPix, sizeOfOneLineOfPixels);
	}
	delete [] tempLineOfPix;
	image.update();
}

void ImageUtil::grabFBO(ofImage& image, int _x, int _y, int _w, int _h){
	unsigned char* myPixels = image.getPixels();
	//if (!myPixels.bAllocated){
	//	allocate(_w, _h, OF_IMAGE_COLOR);
	//}
	image.allocate(_w, _h, image.type);
	
	int screenHeight = _h;
	_y = screenHeight - _y;
	_y -= _h; // top, bottom issues
	
	
#ifndef TARGET_OF_IPHONE
	glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT );											// be nice to anyone else who might use pixelStore
#endif
	glPixelStorei(GL_PACK_ALIGNMENT, 1);														// set read non block aligned...
	glReadPixels(_x, _y, _w, _h, image.getTextureReference().texData.glType,GL_UNSIGNED_BYTE, myPixels);		// read the memory....
#ifndef TARGET_OF_IPHONE
	glPopClientAttrib();
#endif
	
	int sizeOfOneLineOfPixels = image.width * image.bpp/8;
	unsigned char * tempLineOfPix = new unsigned char[sizeOfOneLineOfPixels];
	unsigned char * linea;
	unsigned char * lineb;
	int max = image.height/2;
	for (int i = 0; i < max; i++){
		linea = myPixels + i * sizeOfOneLineOfPixels;
		lineb = myPixels + (image.height-i-1) * sizeOfOneLineOfPixels;
		memcpy(tempLineOfPix, linea, sizeOfOneLineOfPixels);
		memcpy(linea, lineb, sizeOfOneLineOfPixels);
		memcpy(lineb, tempLineOfPix, sizeOfOneLineOfPixels);
	}
	delete [] tempLineOfPix;
	image.update();
}

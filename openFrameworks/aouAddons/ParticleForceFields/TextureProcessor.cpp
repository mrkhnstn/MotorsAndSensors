 /*
 *  Attractor.cpp
 *  openFrameworks
 *
 *  Created by Robin Beitra (robin@beitra.net) on 03/02/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "TextureProcessor.h"


void TextureProcessor::setup(int _width, int _height, int _blur){
	blur = _blur;
	if(!allocated || width != _width || height != _height){
		
		if(allocated){
			cvReleaseImage(&temp);
			cvReleaseImage(&temp2);
			cvReleaseImage(&temp3);
			cvReleaseImage(&mask);
			cvReleaseImage(&attractx);
			cvReleaseImage(&attracty);
			allocated = false;
		}
		
		width = _width;
		height = _height;
		
		int depth = IPL_DEPTH_32F;
		
		temp = cvCreateImage(cvSize(width, height), depth, 1);
		temp2 = cvCreateImage(cvSize(width, height), depth, 1);
		temp3 = cvCreateImage(cvSize(width, height), depth, 1);
		mask = cvCreateImage(cvSize(width, height), depth, 1);
		attractx = cvCreateImage(cvSize(width, height), depth, 1);
		attracty = cvCreateImage(cvSize(width, height), depth, 1);

		allocated = true;
	}
}

void TextureProcessor::process(IplImage* src){
	cvConvertScale(src, temp, 1);//get it into our depth
	cvConvertScale(src, mask, -1/255.0, 1);//create a mask
	
	cvSmooth(temp, temp2, CV_GAUSSIAN, blur);//create blurred
	
	cvSobel(temp2, temp, 1, 0, 3);//dx
	cvMul(mask, temp, attractx);//masked
	
	cvSobel(temp2, temp, 0, 1, 3);//dy
	cvMul(mask, temp, attracty);//masked
}

void TextureProcessor::processSpread(IplImage* src){
	cvConvertScale(src, temp, 1/255, 0);//get it into our depth
	//cvConvertScale(src, mask, -1/255.0, 1);//create a mask
	
	cvSmooth(temp, temp2, CV_GAUSSIAN, blur);//create blurred
	
	cvSobel(temp2, attractx, 1, 0, 3);//dx
	//cvMul(mask, temp, attractx);//masked
	
	cvSobel(temp2, attracty, 0, 1, 3);//dy
	//cvMul(mask, temp, attracty);//masked
}


void TextureProcessor::setupTextureForceField2f(TextureForceField2f& textureField, ofImage& image, int channel, int blur){
	
	setupIplImage(image.width, image.height);
	
	textureField.setup(image.width, image.height);
	
	setup(image.width, image.height, blur);
	
	ofTOcvGreyscaleImage(&image, iplImage, channel);
	
	process(iplImage);
	
	copyCVImageToFloatArray(textureField.fx, attractx);
	copyCVImageToFloatArray(textureField.fy, attracty);
	
	clearIplImage();
}

void TextureProcessor::setupIplImage(int width, int height){
	bool create = false;
	if(iplImage != NULL){
		if(iplImage->width != width || iplImage->height != height){
			clearIplImage();
			create = true;
		}
	} else {
		create = true;
	}
	if(create){
		iplImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	}
}
void TextureProcessor::clearIplImage(){
	if(iplImage != NULL){
		cvReleaseImage(&iplImage);
	}
}

void TextureProcessor::ofTOcvGreyscaleImage(ofImage* src, IplImage* dest, int channel){
	int w = src->getWidth();
	int h = src->getHeight();
	//ofxCvGrayscaleImage* dest = ofGreyscaleImageAtSize(w, h);
	
	unsigned char* srcpixels = src->getPixels();
	unsigned char* destpixels = (unsigned char*)dest->imageData;
	
	int sstep = src->bpp/8;
	int dstep = dest->nChannels;
	int max = w*h;
	int i;
	if(channel == 0){
		for(i = 0; i < max; ++i){
			destpixels[i*dstep] = srcpixels[i*sstep];
		}
	} else {
		for(i = 0; i < max; ++i){
			destpixels[i*dstep] = srcpixels[i*sstep + channel];
		}
	}
	//return cvImage;
}
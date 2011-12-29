//
// ofxCvOpticalFlowLK.h - a OpenCV cvOpticalFlowLK warpper for openFrameworks
//
// Copyright (C) 2008 Takashi Maekawa <takachin@generative.info> 
//     All rights reserved.
//     This is free software with ABSOLUTELY NO WARRANTY.
//
// You can redistribute it and/or modify it under the terms of 
// the GNU Lesser General Public License.
//

#include "ofxCvOpticalFlowLK.h"

ofxCvOpticalFlowLK::ofxCvOpticalFlowLK(void)
{
	width = DEFAULT_CAPTURE_WIDTH;
	height = DEFAULT_CAPTURE_HEIGHT;
	
	captureColsStep = DEFAULT_CAPTURE_COLS_STEP;
	captureRowsStep = DEFAULT_CAPTURE_ROWS_STEP;
	
	velThreshold	= 1;
	
	vel_x = 0;		// MEMO
	vel_y = 0;		// MEMO
}

ofxCvOpticalFlowLK::~ofxCvOpticalFlowLK(void)
{
	cvReleaseImage(&vel_x);
	cvReleaseImage(&vel_y);
}

void ofxCvOpticalFlowLK::allocate(int _w, int _h){
	width		= _w;
	height		= _h; 
	invWidth	= 1.0f/width;
	invHeight	= 1.0f/height;
	
	if(vel_x) cvReleaseImage(&vel_x);		// MEMO
	if(vel_y) cvReleaseImage(&vel_y);		// MEMO
	
	vel_x = cvCreateImage( cvSize( width, height ), IPL_DEPTH_32F, 1  );
	vel_y = cvCreateImage( cvSize( width, height ), IPL_DEPTH_32F, 1  );
	
    cvSetZero(vel_x);
    cvSetZero(vel_y);
	
	resetROI();
}

void ofxCvOpticalFlowLK::setCalcStep(int _cols, int _rows){
	captureColsStep = _cols;
	captureRowsStep = _rows;
}

void ofxCvOpticalFlowLK::calc( ofxCvGrayscaleImage & pastImage, ofxCvGrayscaleImage & currentImage, int size) {
	cvCalcOpticalFlowLK( pastImage.getCvImage(), currentImage.getCvImage(), cvSize( size, size), vel_x, vel_y );
}

void ofxCvOpticalFlowLK::draw(void){
//	glColor4f(0, 0, 0, 0.8f);
//	ofEnableAlphaBlending();
//	ofFill();
//	ofRect(0, 0, width, height);
//	ofDisableAlphaBlending();
	
	int x, y, dx, dy;
	for ( y = 0; y < getHeight(); y+=captureRowsStep ){
		for ( x = 0; x < getWidth(); x+=captureColsStep ){
			dx = (int)cvGetReal2D( vel_x, y, x );
			dy = (int)cvGetReal2D( vel_y, y, x );
			if(dx*dx+dy*dy > velThreshold) {
				glBegin(GL_LINES);
				glColor3f(1, 1, 1); glVertex2f(x, y);
				glColor3f(1, 0, 0); glVertex2f(x+dx*2,y+dy*2);
				glEnd();
			}
		}
	}
}

void ofxCvOpticalFlowLK::draw(float x, float y) {
	glPushMatrix();
	glTranslatef(x, y, 0);	
	draw();
	glPopMatrix();
}

void ofxCvOpticalFlowLK::draw(float x, float y, float w, float h) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(w/width, h/height, 1.0);
	draw();
	glPopMatrix();
}

void ofxCvOpticalFlowLK::setROI(int x, int y, int w, int h) {
	cvSetImageROI(vel_x, cvRect(x,y, w,h) );
	cvSetImageROI(vel_y, cvRect(x,y, w,h) );
	roiRect = ofRectangle(x, y, w, h);
	invWidth = 1.0f/w;
	invHeight = 1.0f/h;
}

void ofxCvOpticalFlowLK::resetROI() {
	cvResetImageROI(vel_x);
	cvResetImageROI(vel_y);
	roiRect = ofRectangle(0, 0, width, height);
	invWidth = 1.0f/width;
	invHeight = 1.0f/height;
}

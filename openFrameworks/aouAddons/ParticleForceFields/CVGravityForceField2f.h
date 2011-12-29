#pragma once
/*
 *  CVGravityForceField2f.h
 *  ofxKinect
 *
 *  Created by Mark Hauenstein on 16/12/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ForceField2f.h"
#include "ofxBaseObject.h"
#include "ofxOpenCv.h"

class CVGravityForceField2f : public ForceField2f, public ofxBaseObject {
	
public:
	
	
	float radius;
	float strength;
	int rows;
	int cols;
	
	CVGravityForceField2f(){
		radius = 1;
		strength = 0;
		rows = 24;
		cols = 32;
		colSpan = 1;
		rowSpan = 1;
		debugDraw = false;
		_isSetup = false;
	}
	
	void setup(ofxCvGrayscaleImage& cvImage){
		this->cvImage = &cvImage;
		_isSetup = true;
	}
	
	void setupGUI(string name="CVGravityForceField2f"){
		gui.addPage("CVGravityForceField2f");
		gui.addSlider("radius", radius, 1, 2048);
		gui.addSlider("strength", strength, -0.01, 0.01);
		gui.addSlider("rows", rows, 2, 256);
		gui.addSlider("cols", cols, 2, 256);
		gui.addToggle("debug draw", debugDraw);
	}
	
	void draw(){
		if(!debugDraw || !_isSetup) return;
		
		ofPushStyle();
		ofSetColor(255, 0, 0);
		//ofNoFill();
		ofSetLineWidth(0);
		
		for(int col = 0; col<cols; col++){
			for(int row = 0; row<rows; row++){
				//int col = roundf(ofMap(pos_.x, 0, ofGetWidth(), 0, cols-1, true));
				//int row = roundf(ofMap(pos_.y, 0, ofGetHeight(), 0, rows-1, true));
				
				ofxVec2f center;
				center.x = col * colSpan;
				center.y = row * rowSpan;
				
				int pixelX = ofMap(col, 0, cols, 0, cvImage->getWidth()-1, true);
				int pixelY = ofMap(row, 0, rows, 0, cvImage->getHeight()-1, true);
				
				unsigned char* pixels = cvImage->getPixels();
				int index = pixelY*cvImage->getWidth()+pixelX;
				int pixel = pixels[index];
				float cvStrength = pixel / 255.f;
				//cvStrength = 1;
				ofCircle(center.x, center.y, cvStrength*50);
			}
		}
		
		
		ofPopStyle();
		
	}
	
	void update(){
		if (!_isSetup) return;
		
		colSpan = ofGetWidth() / (cols - 1);
		rowSpan = ofGetHeight() / (rows - 1);
	}
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		
		ofxVec2f force;
		
		if(!_isSetup) return force;
		
		
		int col = roundf(ofMap(pos_.x, 0, ofGetWidth(), 0, cols-1, true));
		int row = roundf(ofMap(pos_.y, 0, ofGetHeight(), 0, rows-1, true));
		
		ofxVec2f center;
		center.x = col * colSpan;
		center.y = row * rowSpan;
		
		int pixelX = ofMap(col, 0, cols, 0, cvImage->getWidth()-1, true);
		int pixelY = ofMap(row, 0, rows, 0, cvImage->getHeight()-1, true);
		
		unsigned char* pixels = cvImage->getPixels();
		int index = pixelY*cvImage->getWidth()+pixelX;
		int pixel = pixels[index];
		float cvStrength = pixel / 255.f;
		
		if(cvStrength > 0){
			force = center - pos_;
			float d2 = force.squareLength();
			float r2 = radius * radius;
			if(d2 < r2){ // if distance larger than radius then have no effect
				float relativeStrength = (r2 - d2) / r2;			
				force *= relativeStrength * strength * cvStrength;
			}
		}
		
		
		return force;
	}
	
protected:
	
	bool		debugDraw;
	bool		_isSetup;
	float colSpan;
	float rowSpan;
	ofxCvGrayscaleImage* cvImage;
};
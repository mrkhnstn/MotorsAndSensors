#pragma once
/*
 *  TextureConstForceField2f.h
 *  ofxKinect
 *
 *  Created by Mark Hauenstein on 17/12/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "TextureForceField2f.h"
#include "ofxBaseObject.h"

class TextureConstForceField2f : public TextureForceField2f, public ofxBaseObject {
	
public:
	
	int					velocityBlur;
	float				velocityThreshold;
	int					opFlowBlockSize;
	
	ofxCvGrayscaleImage* cvImage;
	//ofxCvGrayscaleImage previousGreySmallImage;
	//ofxCvGrayscaleImage currentGreySmallImage;
	bool isSetup;
	
	//float lerpRate;
	
	ofxVec2f force;
	
	TextureConstForceField2f(){
		isSetup = false;
	}
	
	~TextureConstForceField2f(){}
	
	void setup(ofxCvGrayscaleImage& srcImage){
		this->cvImage = &srcImage;
		isSetup = true;
	}
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		
		ofxVec2f tempForce;
		
		if(!isSetup) return tempForce;
		
		int pixelX = ofMap(pos_.x, 0, ofGetWidth(), 0, cvImage->getWidth()-1, true);
		int pixelY = ofMap(pos_.y, 0, ofGetHeight(), 0, cvImage->getHeight()-1, true);
		
		unsigned char* pixels = cvImage->getPixels();
		int index = pixelY*cvImage->getWidth()+pixelX;
		int pixel = pixels[index];
		float cvStrength = pixel / 255.f;
		
		tempForce = force * cvStrength;
		
		return tempForce;
	}

	
	void setupGUI(string name=""){
		gui.addPage("TextureConstForceField2f");
		gui.addSlider("force x", force.x, -1, 1);
		gui.addSlider("force y", force.y, -1, 1);
	}
		
};
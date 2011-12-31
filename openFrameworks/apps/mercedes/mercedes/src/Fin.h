#pragma once
/*
 *  Fin.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"

class Fin : public ofxBaseObject {
	
public:
	
	Constants* constants;
	
	int		index;			// index within fin array
	float	angleN;			// normalized angle	of fin rotation
	float	posAngle;		// position angle calculated out of index and numOfFins

	int		bank;			// SD84 bank
	int		ch;				// SD84 channel
	
	void setup(){
		constants = Singleton<Constants>::instance();
		
		posAngle = index * 360 / constants->numFins;
		angleN = 0.5;
		bank = 0;
		ch = 1;
	}
	
	void setupGUI(){
		string indexS = ofToString(index);
		gui.addTitle("fin_"+indexS);
		gui.addSlider("angleN_"+indexS, angleN, 0, 1);
		gui.addSlider("bank_"+indexS, bank, 0, 3);
		gui.addSlider("channel_"+indexS, ch,1,84);
	}
	
	void draw(){
		ofPushStyle();
		ofPushMatrix();
		
		ofRotate(posAngle, 0, 1, 0);
		
		
		ofPushMatrix();
		ofTranslate(constants->cylinderRadius, 0, 0); // move from cylinder centre by cylinder radius
		
		ofRotate(ofMap(angleN, 0, 1, -90, 90, true), 0, 1, 0); // rotate fin around its own z axis
		
		ofSetColor(255, 255, 255);
		ofFill();
		ofSetLineWidth(0);
		
		// draw circle on null plane
		ofPushMatrix();
		ofRotate(90, 1, 0, 0);
		ofCircle(0, 0, 2);
		ofPopMatrix();
		
		// draw lines on null plane
		ofSetLineWidth(1);
		ofSetColor(0, 0, 255);
		ofxLine(-0.5*constants->finWidth, 0, 1, 0.5*constants->finWidth, 0, 1);
		ofSetColor(255, 0, 0);
		ofxLine(-0.5*constants->finWidth, 0, -1, 0.5*constants->finWidth, 0, -1);
		ofSetLineWidth(0);
		
		// draw boxes
		ofPushMatrix();
		ofSetColor(0, 0, 255);
		ofTranslate(0, 75, 1);
		ofScale(20, 150, 2);
		ofxBox(0, 0, 0, 1);
		ofPopMatrix();
		
		ofPushMatrix();
		ofSetColor(255, 0, 0);
		ofTranslate(0, 75, -1);
		ofScale(20, 150, 2);
		ofxBox(0, 0, 0, 1);
		ofPopMatrix();
		
		ofPopMatrix();
		
		ofSetColor(255, 255, 255);
		ofTranslate(constants->cylinderRadius-20, 0, 0);
		//ofDrawBitmapString(ofToString(index), 0, 0); //TODO: fix in 3D
		
		ofPopMatrix();
		ofPopStyle();
	}
	
	float getAngleN(){}
	
	void setAngleN(float f){
		angleN = ofClamp(f,0,1);
	}
	
};

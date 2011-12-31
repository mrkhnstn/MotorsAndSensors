#pragma once
/*
 *  Sensor.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 29/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"

class Sensor : public ofxBaseObject {

public:
	
	Constants* constants;
	
	int index;
	float posAngle;
	
	int bank;
	int ch;
	
	int* rawValues[5];
	
	void setup(){
		constants = Singleton<Constants>::instance();
		posAngle = index * 360 / constants->numSensors;
	}
	
	void draw(){
		
		ofPushStyle();
		ofPushMatrix();
		
		ofRotate(posAngle, 0, 1, 0);
		
		ofPushMatrix();
		ofTranslate(constants->cylinderRadius + 10, 0, 0); // move from cylinder centre by cylinder radius

		ofSetColor(255, 255, 0);
		ofNoFill();
		ofSetLineWidth(1);
		
		for(int i=0; i<5; i++){
			ofPushMatrix();
			ofRotate(-12.5 + i * 5., 0, 1, 0);
			int* temp = rawValues[i];
			ofLine(0, 0, *temp, 0);
			ofPopMatrix();
		}
		ofPopMatrix();
		
		ofSetColor(255, 255, 255);
		ofTranslate(constants->cylinderRadius+10, 0, 0);
		//ofDrawBitmapString(ofToString(index), 0, 0); //TODO: fix in 3D
		
		ofPopMatrix();
		ofPopStyle();
	}
	
	
	
};
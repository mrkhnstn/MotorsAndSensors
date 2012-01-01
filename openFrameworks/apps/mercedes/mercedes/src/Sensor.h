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
	
	int		index;
	float	posAngle;
	
	int		bank;
	int		ch;
	
	int*	rawValues[5];
	float*	distanceValues[5];
	
	static bool doDraw;
	static bool doDrawRays;
	static bool doDrawHitPoints;
	static bool doDrawLabels;
	
	static float userInProximityDistance; // if a sensor distance reading is smaller then userInProximityDistance then a user is considered to be in proximity
	
	void setup(){
		constants = Singleton<Constants>::instance();
		posAngle = index * 360 / constants->numSensors;
		
		_userInProximity = false;
	}
	
	void update(){
		// check for users in proximity
		_userInProximity = false;
		for(int i=0; i<5; i++)
		{
			float distance = *distanceValues[i];
			if (distance < userInProximityDistance) {
				_userInProximity = true;
				break;
			}
		}
	}
	
	void postGUI(){
		
	}
	
	void draw(){
		
		if(!doDraw) return;
		
		ofPushStyle();
		ofPushMatrix();
		
		ofRotate(posAngle, 0, 1, 0);
		
		ofPushMatrix();
		ofTranslate(constants->cylinderRadius + 10, 0, 0); // move from cylinder centre by cylinder radius
		
		ofSetColor(255, 255, 255); //TODO: make color variable

		for(int i=0; i<5; i++){
			ofPushMatrix();
			ofRotate(-12.5 + i * 5., 0, 1, 0);
			
			float* temp = distanceValues[i];
			
			if (*temp < 150) {
			
				if(doDrawRays){
					ofNoFill();
					ofSetLineWidth(1);
					ofLine(0, 0, *temp, 0);
				}
				
				if (doDrawHitPoints) {
					ofSetLineWidth(0);
					ofFill();
					ofTranslate(*temp, 0, 0);
					ofRotate(90, 1, 0, 0);
					ofCircle(0, 0, 10);
				}
			
			}
			
			ofPopMatrix();
		}

		
		ofPopMatrix();
		
		if(doDrawLabels){
			ofSetColor(255, 255, 255);
			ofTranslate(constants->cylinderRadius+10, 0, 0);
			//ofDrawBitmapString(ofToString(index), 0, 0); //TODO: fix in 3D
		}
		
		ofPopMatrix();
		ofPopStyle();
	}
	
	bool userInProximity(){
		return _userInProximity;
	}
	
protected:
	
	bool _userInProximity;
};
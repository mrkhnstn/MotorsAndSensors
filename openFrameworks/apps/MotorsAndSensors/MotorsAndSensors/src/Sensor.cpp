/*
 *  Sensor.cpp
 *  mercedes
 *
 *  Created by Mark Hauenstein on 29/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sensor.h"

bool Sensor::doDraw = true;
bool Sensor::doDrawRays = true;
bool Sensor::doDrawHitPoints = true;
bool Sensor::doDrawLabels = false;
bool Sensor::doDrawSensorThreshold = false;
float Sensor::userInProximityDistance = 230;

bool Sensor::doStaticInit = true;
ofTrueTypeFont Sensor::font;

void Sensor::setup(){
	if(doStaticInit)
	{
		font.loadFont("fonts/Hlcb____.ttf",12,true,true,true);
		doStaticInit = false;
	}
	
	constants = Singleton<Constants>::instance();
	posAngle = index * 360 / constants->numSensors;
	
	_userInProximity = false;
}

void Sensor::update(){
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

void Sensor::postGUI(){
	
}

void Sensor::draw(){
	
	if(!doDraw) return;
	
	ofPushStyle();
	ofPushMatrix();
	
	ofRotate(posAngle, 0, 1, 0);
	
	ofPushMatrix();
	ofTranslate(constants->cylinderRadius + 25, 0, 0); // move from cylinder centre by cylinder radius
	
	ofSetColor(255, 255, 255); //TODO: make color variable
	
	for(int i=0; i<5; i++){
		ofPushMatrix();
		ofRotate(-12.5 + i * 5., 0, 1, 0);
		
		float* temp = distanceValues[i];
		
		if (*temp < userInProximityDistance) {
			
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
				ofCircle(0, 0, 5);
			}
			
		}
		
		ofPopMatrix();
	}
	
	
	ofPopMatrix();
	
	if(doDrawLabels){
		ofSetColor(255, 255, 255);
		ofPushMatrix();
		ofTranslate(constants->cylinderRadius+15, 0, 0);
		
		//glDisable(GL_DEPTH_TEST);
		//ofRotate(180, 0, 0, 1);
		ofRotate(90, 0, 1, 0);
		ofRotate(90, 1, 0, 0);
		ofScale(0.5, 0.5, 0.5);
		
		ofRectangle rect = font.getStringBoundingBox(ofToString(index), 0, 0);
		font.drawString(ofToString(index), -rect.width * 0.5, rect.height * 0.5);
		
		ofPopMatrix();
		
		//glEnable(GL_DEPTH_TEST);			
		//ofDrawBitmapString(ofToString(index), 0, 0); //TODO: fix in 3D
	}
	
	ofPopMatrix();
	ofPopStyle();
}

bool Sensor::userInProximity(){
	return _userInProximity;
}
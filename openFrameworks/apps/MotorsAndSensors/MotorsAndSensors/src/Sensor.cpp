/*
 *  Sensor.cpp
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 29/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sensor.h"
#include "Coordinates.h"

bool Sensor::doDraw = true;
bool Sensor::doDrawRays = true;
bool Sensor::doDrawHitPoints = true;
bool Sensor::doDrawLabels = false;
bool Sensor::drawOnlyInSensorZone = true;
bool Sensor::doDrawSensorThreshold = false;
float Sensor::userInProximityDistance = 230;
float Sensor::angleBetweenRays = 5;


int Sensor::numSensors = 36;
float Sensor::distanceToCentre = 325;

bool Sensor::doStaticInit = true;
ofTrueTypeFont Sensor::font;

void Sensor::setup(){
	if(doStaticInit)
	{
		font.loadFont("fonts/Hlcb____.ttf",8,true,true,true);
		doStaticInit = false;
	}
	
	posAngle = -index * 360 / numSensors - 5;
	
	_userInProximity = false;
}

void Sensor::update(){
	
	
	pos.x = cosf(ofDegToRad(posAngle))*distanceToCentre;
	pos.y = sinf(ofDegToRad(posAngle))*distanceToCentre;
	
	cartPos = Coordinates::fromPolar(pos);
	
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
	
	// calculate cartesian position
	ofxVec2f cylinderOffset;
	cylinderOffset.x = distanceToCentre;
	for(int i=0; i<5; i++)
	{
		ofxVec2f temp;
		temp.y = 0;
		temp.x = *(distanceValues[i]);
		temp.rotate(2.5*angleBetweenRays - i * angleBetweenRays);
		temp += cylinderOffset;
		temp.rotate(posAngle);
		absRayPos[i] = temp;
		cartRayPos[i] = Coordinates::fromPolar(temp);
		rayCenterDist[i] = temp.length();
		inSensorZone[i] = rayCenterDist[i] > Coordinates::minSensingZone && rayCenterDist[i] < Coordinates::maxSensingZone;
	}
	
}

void Sensor::postGUI(){
	
}

void Sensor::draw(){
	
	if(!doDraw) return;
	
	
	ofPushStyle();
	ofPushMatrix();
	
	ofRotate(360-posAngle-2.5, 0, 1, 0);
	
	ofPushMatrix();
	ofTranslate(distanceToCentre, 0, 0); // move from cylinder centre by cylinder radius
	
	// draw sensor origin
	ofSetColor(255, 255, 255); //TODO: make color variable
	ofPushMatrix();
	ofRotate(90, 1, 0, 0);
	ofNoFill();
	ofCircle(0, 0, 0.5);
	ofPopMatrix();
	
	
	// draw rays using openGL transforms
	ofScale(0.25, 0.25, 0.25);
	for(int i=0; i<5; i++){
		ofPushMatrix();
		ofRotate(-2.5 * angleBetweenRays + i * angleBetweenRays, 0, 1, 0);
		
		float* temp = distanceValues[i];
		
		//if (*temp < userInProximityDistance) {
			
			if(doDrawRays){
				ofNoFill();
				ofSetLineWidth(1);
				ofLine(0, 0, *temp, 0);
			}
			
			/*
			if (doDrawHitPoints) {
				ofSetLineWidth(0);
				ofFill();
				ofTranslate(*temp, 0, 0);
				ofRotate(90, 1, 0, 0);
				ofCircle(0, 0, 5);
			}
			*/
			
		//}
		
		ofPopMatrix();
	}
	
	
	ofPopMatrix();
	
	if(doDrawLabels){
		ofSetColor(255, 255, 255);
		ofPushMatrix();
		ofTranslate(distanceToCentre, 0, 0);
		
		//glDisable(GL_DEPTH_TEST);
		//ofRotate(180, 0, 0, 1);
		ofRotate(90, 0, 1, 0);
		ofRotate(90, 1, 0, 0);
		ofScale(0.75, 0.75, 0.75);
		
		ofRectangle rect = font.getStringBoundingBox(ofToString(index), 0, 0);
		font.drawString(ofToString(index), -rect.width * 0.5, rect.height * 0.5);
		
		ofPopMatrix();
		
		//glEnable(GL_DEPTH_TEST);			
		//ofDrawBitmapString(ofToString(index), 0, 0); //TODO: fix in 3D
	}
	
	ofPopMatrix();
	 	ofPopStyle();


	/*
	if(doDrawLabels){
		ofSetColor(0, 255, 0);
		ofPushMatrix();
		ofTranslate(pos.x, 0, pos.y);
		//ofCircle(0, 0, 2);
		ofRectangle rect = font.getStringBoundingBox(ofToString(index), 0, 0);
		ofRotate(90, 1, 0, 0);
		font.drawString(ofToString(index), -rect.width * 0.5, rect.height * 0.5);
		ofPopMatrix();
	}
	
	ofxVec2f cylinderOffset;
	cylinderOffset.x = distanceToCentre;
	
	ofxVec2f temp;
	float revAngle = 0;
	float revDist = 0;
	ofxVec2f revVec;
	
	ofxVec2f xAxis;
	xAxis.x = 1;
	
	if(doDrawRays)
		for(int i=0; i<5; i++){
			if(inSensorZone[i] || !drawOnlyInSensorZone)
			ofxLine(pos.x, 0, pos.y, absRayPos[i].x, 0, absRayPos[i].y);
		}
	
	if(doDrawHitPoints)
		for(int i=0; i<5; i++){
			if(inSensorZone[i] || !drawOnlyInSensorZone){
			ofPushMatrix();
			ofTranslate(absRayPos[i].x, 0, absRayPos[i].y);
			ofRotate(90, 1, 0, 0);
			ofCircle(0, 0, 4);
			ofPopMatrix();
			}
		}
	 */
	

}

bool Sensor::userInProximity(){
	return _userInProximity;
}
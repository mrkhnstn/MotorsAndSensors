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

class Sensor;

class Fin : public ofxBaseObject {
	
public:
	
	Constants* constants;
	
	static float elapsedTime;
	static float maxMotorSpeed;
	static float maxAngleNSpeed;
	
	int		index;			// index within fin array
	float	angleN;			// normalized angle	of fin rotation
	float	tgtAngleN;
	float	posAngle;		// position angle calculated out of index and numOfFins

	int		bank;			// SD84 bank
	int		ch;				// SD84 channel
	
	static bool doDraw;		// global draw flag
	static bool doDraw3D;	// draw 3d box
	static bool doDraw2D;	// draw 2d on null plane
	static bool doDrawLabels; // draw ids of each fin
	
	static ofColor backColorLow;	// back side color of fin when NO user in proximity
	static ofColor frontColorLow;	// front side color of fin when NO user in proximity
	
	static ofColor backColorHigh;	// back side color of fin when user in proximity
	static ofColor frontColorHigh;	// front side color of fin when user in proximity
	
	static float sensorAngleRange; 
	// defines which sensors shall be included for user proximity detection
	// minRange = posAngle - 0.5 * sensorAngleRange
	// maxRange = posAngle + 0.5 * sensorAngleRange
	
	vector<Sensor*> sensors; 
	// all sensors that shall be checked for user proximity detection
	// gets populated / refreshed using updateSensors() 
	
	void setup(){
		constants = Singleton<Constants>::instance();
		
		posAngle = index * 360 / constants->numFins;
		angleN = 0.5;
		tgtAngleN = angleN;
		
		_userInProximity = false;
		
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
	
	void postGUI(){
		updateSensors(); // needs to happen at least once after every thing has been setup
	}
	
	void update(){
		// update users in proximity
		updateUserProximity();
		
		// limit fin motion to max motor speed
		float delta = tgtAngleN - angleN;
		float dir = delta < 0 ? -1 : 1;
		float clampedDelta = dir * ofClamp(abs(delta),0,maxAngleNSpeed * elapsedTime);
		angleN = angleN + clampedDelta;
	}
	
	
	
	void draw(){
		if(!doDraw) return;
		
		ofPushStyle();
		ofPushMatrix();
		
		// go to position of fin
		ofRotate(posAngle, 0, 1, 0);
		
		ofPushMatrix();
		ofTranslate(constants->cylinderRadius, 0, 0); // move from cylinder centre by cylinder radius
		
		ofRotate(ofMap(angleN, 0, 1, -90, 90, true), 0, 1, 0); // rotate fin around its own z axis
		
		if (doDraw2D) {
			
			// draw circle on null plane
			ofSetColor(255, 255, 255);
			ofFill();
			ofSetLineWidth(0);
			
			ofPushMatrix();
			ofRotate(90, 1, 0, 0);
			ofCircle(0, 0, 2);
			ofPopMatrix();
			
			// draw lines on null plane
			ofSetLineWidth(1);
			setFrontColor();
			ofxLine(-0.5*constants->finWidth, 0, 1, 0.5*constants->finWidth, 0, 1);
			setBackColor();
			ofxLine(-0.5*constants->finWidth, 0, -1, 0.5*constants->finWidth, 0, -1);
			ofSetLineWidth(0);
		
		}
		
		if (doDraw3D) {
			
			// draw boxes
			ofPushMatrix();
			setFrontColor();
			ofTranslate(0, 75, 1);
			ofScale(20, 150, 2);
			ofxBox(0, 0, 0, 1);
			ofPopMatrix();
			
			ofPushMatrix();
			setBackColor();
			ofTranslate(0, 75, -1);
			ofScale(20, 150, 2);
			ofxBox(0, 0, 0, 1);
			ofPopMatrix();
		
		}
		
		ofPopMatrix();
		
		if(doDrawLabels){
			ofSetColor(255, 255, 255);
			ofTranslate(constants->cylinderRadius-20, 0, 0);
			//ofDrawBitmapString(ofToString(index), 0, 0); //TODO: fix in 3D
		}
		
		ofPopMatrix();
		ofPopStyle();
	}
	
	// populates / refreshes the list of sensors to be used for user proximity detection
	void updateSensors();
	
	void setBackColor(){
		if(userInProximity())
		{
			ofSetColor(backColorHigh.r, backColorHigh.g, backColorHigh.b);
		} else {
			ofSetColor(backColorLow.r, backColorLow.g, backColorLow.b);
		}
	}
	
	void setFrontColor(){
		if(userInProximity())
		{
			ofSetColor(frontColorHigh.r, frontColorHigh.g, frontColorHigh.b);
		} else {
			ofSetColor(frontColorLow.r, frontColorLow.g, frontColorLow.b);
		}
	}
	
	void updateUserProximity();
	
	// simple binary way of figuring out whether a user stands in front of this fin
	bool userInProximity(){
		return _userInProximity;
	}
	
	void setUserInProximity(bool b){
		_userInProximity = b;
	}
	
	float getAngleN(){
		return angleN;
	}
	
	float getTgtAngleN(){
		return tgtAngleN;
	}
	
	void setTgtAngleN(float f){
		tgtAngleN = ofClamp(f,0,1);
	}
	
	void setAngleN(float f){
		angleN = ofClamp(f,0,1);
	}
	
protected:
	
	bool _userInProximity;
	
};

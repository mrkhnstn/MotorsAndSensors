/*
 *  Motor.cpp
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Motor.h"
#include "SensorCtrl.h"
#include "Coordinates.h"

float Motor::elapsedTime = 0;
float Motor::maxMotorSpeed = 0.3;
float Motor::maxAngleNSpeed = 1;
float Motor::distanceToCentre = 300;
int	  Motor::numMotors = 72;

bool Motor::doDraw = true;		// global draw flag
bool Motor::doDraw3D = true;		// draw 3d box
bool Motor::doDraw2D = false;		// draw 2d on null plane
bool Motor::doDrawLabels = false; // draw ids of each motor

int Motor::globalPulseMin = 600;
int Motor::globalPulseMax = 2300;
int Motor::calibrationMode = 0;

ofColor Motor::backColorLow;		// back side color of motor when NO user in proximity
ofColor Motor::frontColorLow;		// front side color of motor when NO user in proximity

ofColor Motor::backColorHigh;		// back side color of motor when user in proximity
ofColor Motor::frontColorHigh;	// front side color of motor when user in proximity


float Motor::sensorAngleRange = 10; 
float Motor::panelWidth = 20;

bool Motor::doStaticInit = true;
ofTrueTypeFont Motor::font;

int Motor::maxProximityValue = 30;
int Motor::proximityOnThreshold = 5;
float Motor::userOffDelayTime = 2;

void Motor::setup(){
	if(doStaticInit)
	{
		font.loadFont("fonts/Hlcb____.ttf",12,true,true,true);
		doStaticInit = false;
	}
	
	posAngle = -index * 360 / numMotors;
	pos.x = cosf(ofDegToRad(posAngle))*distanceToCentre;
	pos.y = sinf(ofDegToRad(posAngle))*distanceToCentre;
	
	angleN = 0.5;
	tgtAngleN = angleN;
	
	pulseMin = 600;
	pulseMax = 2300;
	pulse = pulseMin + (pulseMax - pulseMin) / 2;
	
	_userInProximity = false;
	
	bank = 0;
	ch = 1;
	
	proximityValue = 0;
	userInProximityOnTime = 0;
	userInproximityOffTime = 0;
		
	relativePos = index * MOTOR_GAP;// position in relation to rays;
	float minPos = relativePos - 2 * RAY_GAP;
	float maxPos = relativePos + 2 * RAY_GAP;
	
	SensorCtrl& sensorCtrl = *Singleton<SensorCtrl>::instance();
	rayIds.clear();
	for (int i=0; i<TOTAL_RAYS; i++) {
		if (sensorCtrl.rayPos[i] >= minPos && sensorCtrl.rayPos[i] <= maxPos) {
			rayIds.push_back(i);
		}
	}
	
	
}

void Motor::setupGUI(){
	string indexS = ofToString(index);
	gui.addTitle("motor_"+indexS);
	gui.addSlider("angleN_"+indexS, angleN, 0, 1);
	gui.addSlider("bank_"+indexS, bank, 0, 3);
	gui.addSlider("channel_"+indexS, ch,1,84);
}

void Motor::postGUI(){
	updateSensors(); // (obsolete) needs to happen at least once after every thing has been setup
	angleN = 0;
	tgtAngleN = 0;
}

void Motor::update(){
	// update users in proximity
	updateUserProximity();
	
	cartPos = Coordinates::fromPolar(pos);
	
	// limit motor motion to max motor speed
	float delta = tgtAngleN - angleN;
	float dir = delta < 0 ? -1 : 1;
	float clampedDelta = dir * ofClamp(abs(delta),0,maxAngleNSpeed * elapsedTime);
	angleN = angleN + clampedDelta;
	
	// update pulse
	switch (calibrationMode) {
		case 0:
			pulse = ofMap(angleN, 0, 1, pulseMin, pulseMax, true);
			break;
		case 1:
			pulse = pulseMin;
			break;
		case 2:
			pulse = pulseMax;
			break;
		default:
			break;
	}
	
}

void Motor::draw(){
	if(!doDraw) return;
	
	ofPushStyle();
	ofPushMatrix();
	
	// go to position of motor
	ofRotate(-posAngle, 0, 1, 0);
	
	ofPushMatrix();
	ofTranslate(distanceToCentre, 0, 0); // move from cylinder centre by cylinder radius
	
	ofRotate(ofMap(angleN, 0, 1, -90, 90, true), 0, 1, 0); // rotate motor around its own z axis
	
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
		ofxLine(-0.5*panelWidth, 0, 1, 0.5*panelWidth, 0, 1);
		setBackColor();
		ofxLine(-0.5*panelWidth, 0, -1, 0.5*panelWidth, 0, -1);
		ofSetLineWidth(0);
	}
	
	if (doDraw3D) {
		
		// draw boxes
		ofPushMatrix();
		setBackColor();
		ofTranslate(0, 75, 1);
		ofScale(20, 150, 2);
		ofxBox(0, 0, 0, 1);
		ofPopMatrix();
		
		ofPushMatrix();
		setFrontColor();
		ofTranslate(0, 75, -1);
		ofScale(20, 150, 2);
		ofxBox(0, 0, 0, 1);
		ofPopMatrix();
		
	}
	
	
	
	ofPopMatrix();
	
	
	
	if(doDrawLabels && doDraw3D){
		ofPushMatrix();
		ofSetColor(255, 255, 255);
		ofTranslate(distanceToCentre, 150 + 10, 0);
		ofRotate(180, 0, 0, 1);
		ofRotate(90, 0, 1, 0);
		ofScale(0.5, 0.5, 0.5);
		glDisable(GL_DEPTH_TEST);
		ofRectangle rect = font.getStringBoundingBox(ofToString(index), 0, 0);
		font.drawString(ofToString(index), -rect.width * 0.5, rect.height * 0.5);
		glEnable(GL_DEPTH_TEST);
		//ofDrawBitmapString(ofToString(index), 0, 0); //TODO: fix in 3D
		ofPopMatrix();
	}
	
	ofPopMatrix();
	ofPopStyle();
	
	if(doDrawLabels && doDraw2D){
		ofSetColor(255, 255, 255);
		ofPushMatrix();
		ofTranslate(pos.x, 0, pos.y);
		ofRectangle rect = font.getStringBoundingBox(ofToString(index), 0, 0);
		ofRotate(90, 1, 0, 0);
		font.drawString(ofToString(index), -rect.width * 0.5, rect.height * 0.5);
		ofPopMatrix();
	}
}

void Motor::updateSensors(){
	// obsolete code
	
	sensors.clear();
	
	float minAngle = posAngle - 0.5 * sensorAngleRange;
	if(minAngle < 0)
		minAngle += 360;
	
	float maxAngle = posAngle + 0.5 * sensorAngleRange;
	if(maxAngle >= 360)
		maxAngle -= 360;
	
	Singleton<SensorCtrl>::instance()->getSensorsBetweenAngles(sensors,minAngle,maxAngle);
}

void Motor::updateUserProximity(){
	
	// check whether any of the rays registered with this motor are hit
	bool temp = false;
	SensorCtrl& sensorCtrl = *Singleton<SensorCtrl>::instance();
	for(int i=0; i<rayIds.size(); i++)
	{
		if (sensorCtrl.hit[rayIds[i]]) {
			temp = true;
			break;
		}
	}
	
	// if user in proximity state has changed then update relevant timestamps
	if (_userInProximity != temp) {
		_userInProximity = temp;
		if (_userInProximity) {
			userInProximityOnTime = ofGetElapsedTimef();
		} else {
			userInproximityOffTime = ofGetElapsedTimef();
		}
	}
	
	
	/*
	for(int i=0; i<sensors.size(); i++)
		if (sensors[i]->userInProximity()) {
			_userInProximity = true;
			break;
		}
	*/
}

void Motor::setBackColor(){
	if(userInProximity())
	{
		ofSetColor(backColorHigh.r, backColorHigh.g, backColorHigh.b);
	} else {
		ofSetColor(backColorLow.r, backColorLow.g, backColorLow.b);
	}
}

void Motor::setFrontColor(){
	if(userInProximity())
	{
		ofSetColor(frontColorHigh.r, frontColorHigh.g, frontColorHigh.b);
	} else {
		ofSetColor(frontColorLow.r, frontColorLow.g, frontColorLow.b);
	}
}

void Motor::incrementProximityValue(int i){
	proximityValue += i;
	
	if(proximityValue >= proximityOnThreshold)
		if(!_userInProximity){
			userInProximityOn();
		}
	
	if(proximityValue > maxProximityValue)
		proximityValue = maxProximityValue;
}

void Motor::decrementProximityValue(int i){
	proximityValue -= i;
	
	if(proximityValue <= proximityOnThreshold)
		if(_userInProximity){
			userInProximityOff();
		}
	
	if(proximityValue < 0)
		proximityValue = 0;
}

// switch on user in proximity
void Motor::userInProximityOn(){
	_userInProximity = true;
	userInProximityOnTime = ofGetElapsedTimef();
	
} 

// switch off user in proximity
void Motor::userInProximityOff(){
	_userInProximity = false;
	userInproximityOffTime = ofGetElapsedTimef();
	
}

// simple binary way of figuring out whether a user stands in front of this motor
bool Motor::userInProximity(){
	if (_userInProximity){
		return true;
	} else {
		return ofGetElapsedTimef() < userInproximityOffTime + userOffDelayTime;
	}
}

float Motor::getAngleN(){
	return angleN;
}

float Motor::getTgtAngleN(){
	return tgtAngleN;
}

void Motor::setAngle(float angle){
	setTgtAngleN(ofMap(angle, 0, 180, 0, 1, true));
}

float Motor::getAngle(){
	return tgtAngleN * 180;
}

ofxVec2f Motor::getPos(){
	return pos;
}

float Motor::getPosAngle(){
	return posAngle;
}

void Motor::setTgtAngleN(float f){
	tgtAngleN = ofClamp(f,0,1);
}

void Motor::setAngleN(float f){
	angleN = ofClamp(f,0,1);
}

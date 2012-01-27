#pragma once
/*
 *  OscMotorCtrl.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "MotorCtrl.h"
#include "ofxOsc.h"

class OscMotorCtrl : public ofxBaseObject {
	
public: 
	
	ofxOscSender oscSender;
	MotorCtrl* motorCtrl;
	int* arduinoId;
	int sendFreqMs;
	int nextSendTime; // if ofGetElapsedTimeMillis() > nextSendTime then send motor values to SD84
	
	void setup();
	void setupGUI();
	void update();
	
};

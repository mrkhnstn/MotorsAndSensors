#pragma once
/*
 *  MotorCtrl.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "Motor.h"

class MotorCtrl : public ofxBaseObject {
public:
	
	vector<Motor*> motors;
	
	int servoMin;
	int servoMax;
	
	float refTime;
	
	void setup();
	void setupGUI();
	void postGUI();
	void update();
	void draw();
	
};
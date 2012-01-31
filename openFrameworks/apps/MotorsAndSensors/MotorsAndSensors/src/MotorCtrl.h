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
	float refTime;
	
	void setup();
	void setupGUI();
	void postGUI();
	void update();
	void draw();
	void draw3d();
	
	void setAllToGlobalPulseMin(ofEventArgs& e);
	void setAllToGlobalPulseMax(ofEventArgs& e);
	
	void setToPulseMin(ofEventArgs& e);
	void setToPulseMax(ofEventArgs& e);
};
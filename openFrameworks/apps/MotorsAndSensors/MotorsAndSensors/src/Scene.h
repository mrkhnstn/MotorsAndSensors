#pragma once
/*
 *  Scene.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "MotorCtrl.h"
#include "SensorCtrl.h"

class Scene : public ofxBaseObject {
	
public:
	
	MotorCtrl*	motorCtrl;	// points to MotorCtrl singleton
	SensorCtrl* sensorCtrl; // points to SensorCtrl singleton
	Constants*	constants;
	
	virtual void setup();	
	virtual void setupGUI();	
	virtual void postGUI();
	virtual void update();
	virtual void draw();
	
	//void setMotor(int motorId, float angle);
	//int getMotorCount();

};

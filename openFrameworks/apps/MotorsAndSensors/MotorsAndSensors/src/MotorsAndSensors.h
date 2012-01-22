#pragma once
/*
 *  MotorsAndSensors.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "MotorCtrl.h"
#include "OscMotorCtrl.h"
#include "SceneCtrl.h"
#include "SerialTest.h"
#include "SensorCtrl.h"
#include "ofxOscManager.h"
#include "CvCtrl.h"

class MotorsAndSensors : public ofxBaseContainer {
public:
	
	// 3d related fields
	ofxCamera camera;
	ofxVec3f cameraPos;
	ofxVec3f cameraEye;
	ofxLight globalLight;
	ofxLight directionalLight;
	float cylinderRotation;
	
	ofColor bgColor;
	
	void setup();
	void setupGUI();
	void postGUI();
	// void update(); // see ofxBaseContainer update
	void draw();
	void exit();
	
};


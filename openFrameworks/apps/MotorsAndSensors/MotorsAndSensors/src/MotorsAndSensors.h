#pragma once
/*
 *  MotorsAndSensors.h
 *  mercedes
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

class MotorsAndSensors : public ofxBaseContainer {
public:
	
	// 3d related fields
	ofxCamera camera;
	ofxVec3f cameraPos;
	ofxVec3f cameraEye;
	ofxLight globalLight;
	ofxLight directionalLight;
	float cylinderRotation;
	
	void setup();
	void setupGUI();
	void postGUI();
	// void udpate(); // see ofxBaseContainer update
	void draw();
	void exit();
	
};


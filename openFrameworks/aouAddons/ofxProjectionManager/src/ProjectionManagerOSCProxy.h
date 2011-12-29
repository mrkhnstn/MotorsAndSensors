/*
 *  ProjectionManagerOSCProxy.h
 *  TestBed_Chris
 *
 *  Created by Chris Mullany on 17/03/2011.
 *  Copyright 2011 AllofUs. All rights reserved.
 *
 */

#pragma once

#include "ofxBaseObject.h"
#include "ofxOscManager.h"
#include "ofxOscSendValueFloat.h"
#include "ofxOscReceiveValueFloat.h"
#include "Singleton.h"
#include "ofxProjectionManager.h"

#define OSC_SEND_PORT		7000
#define OSC_RECEIVE_PORT	7000
#define OSC_ADDRESS_TEST_FLOAT	"/pad"

/**
 
 Controller proxy for Projection Manager
 Receives input from an XBOX 360 game controller over OSC and routes commands through to Projection Manager
 There are 3 external dependencies for this class to function:
	1. A wired (not wireless) xbox controller connected via USB
	2. OS X XBOX 360 driver from: http://tattiebogle.net/index.php/ProjectRoot/Xbox360Controller/OsxDriver
	3. xboxInput.maxpatch, available in auoAddons/ofxProjectionManager, should be enabled and running
 
 */
class ProjectionManagerOSCProxy : public ofxBaseObject, public ofxOscHandler{
	
public:
	
	// ProjectionManager Specific fields ////////////////////////////////
	
	ofxOscManager			oscManager;
	ofxProjectionManager*	projectionManager;
	bool					enabled;
	
	// Values for frequently updating analog inputs
	float					timeElapsed;
	float					updateFrequency;
	float					leftAnalogX;
	float					leftAnalogY;
	float					rightAnalogX;
	float					rightAnalogY;
	
	// OSC Generic Methods ///////////////////////////////////////////////
	
	ProjectionManagerOSCProxy();
	~ProjectionManagerOSCProxy();
	
	void setupGUI();
	void setup();
	void update(ofEventArgs &e);
	void handleOscMessage(ofxOscMessage& message);
	
	
	// Projection Manager Proxy //////////////////////////////////////////
	
	void handleInput(int element, float value);
	void updateProjectionManager();
	float convertInput(int input);
	
	
};
/*
 *  ProjectionManagerOSCProxy.cpp
 *  TestBed_Chris
 *
 *  Created by Chris Mullany on 17/03/2011.
 *  Copyright 2011 AllofUs. All rights reserved.
 *
 */

#include "ProjectionManagerOSCProxy.h"

ProjectionManagerOSCProxy::ProjectionManagerOSCProxy() : ofxBaseObject(){
	name = "ProjectionManagerOSCProxy";
	//floatValue = 0;
	enabled = false;
	projectionManager = Singleton<ofxProjectionManager>::instance();
}

ProjectionManagerOSCProxy::~ProjectionManagerOSCProxy(){
	// TODO: oscManager cleanup?
}

void ProjectionManagerOSCProxy::setupGUI(){}

void ProjectionManagerOSCProxy::setup(){
	
	// oscManager is a class variable
	// setup ofxOscManager (only once at startup)
	oscManager.port = OSC_RECEIVE_PORT; //< set port before setup
	oscManager.setup(OSC_SEND_PORT, true);
	
	// register an osc address with the oscManager
	// see handleOscMessage method
	oscManager.registerInterest(*this, OSC_ADDRESS_TEST_FLOAT);
	
	/*
	// Use the local IP address
	oscSender.setup("127.0.0.1", OSC_SEND_PORT);
	
	oscSendValue.setup(floatValue, OSC_ADDRESS_TEST_FLOAT, oscSender);
	ofAddListener(ofEvents.update, (ofxOscSendValueBase<float>*)(&oscSendValue), &ofxOscSendValueBase<float>::_update); //< very convoluted way of adding update listener due to templated inheritance
	
	oscReceiveValue.setup(receivedFloatValue,OSC_ADDRESS_TEST_FLOAT,oscManager);
	*/
	
	// Set an update frequency for analog inputs
	timeElapsed = ofGetElapsedTimeMillis();
	updateFrequency = 100;
	ofAddListener(ofEvents.update, this, &ProjectionManagerOSCProxy::update);
}

/*
void ProjectionManagerOSCProxy::sendFloatValue(float f){
	ofxOscMessage m;
	m.setAddress(OSC_ADDRESS_TEST_FLOAT);
	m.addFloatArg(floatValue);
	oscSender.sendMessage(m);
}
*/

// override handleOscMessage
void ProjectionManagerOSCProxy::handleOscMessage(ofxOscMessage& message){
	string address = message.getAddress();
	if (address == OSC_ADDRESS_TEST_FLOAT) { //< cannot use switch heres
		int element = castOscNumberToInt(message,0); //<castOscNumberToInt is inherited from handler
		float value = castOscNumberToFloat(message,1); //<castOscNumberToFloat is inherited from handler
		//ofLog(OF_LOG_VERBOSE, "received input from element " + ofToString(element) + " with value " + ofToString(value));
		handleInput(element, value);
	}
}



// Projection Manager Proxy //////////////////////////////////////////

void ProjectionManagerOSCProxy::update(ofEventArgs &e){
	if (ofGetElapsedTimeMillis() - timeElapsed > updateFrequency) {
		timeElapsed = ofGetElapsedTimeMillis();
		updateProjectionManager();
	}
}

void ProjectionManagerOSCProxy::updateProjectionManager(){
	// Update analog inputs that are not zero
	if (leftAnalogX != 0)
		projectionManager->leftRight(convertInput(leftAnalogX));
	if (leftAnalogY != 0)
		projectionManager->leftDown(convertInput(leftAnalogY));
	if (rightAnalogX != 0)
		projectionManager->rightRight(convertInput(rightAnalogX));
	if (rightAnalogY != 0)
		projectionManager->rightDown(convertInput(rightAnalogY));
}

float ProjectionManagerOSCProxy::convertInput(int input){
	float value;
	switch (input) {
		case 1:
			value = 0.25;
			break;
		case -1:
			value = -0.25;
			break;
		case 2:
			value = 1;
			break;
		case -2:
			value = -1;
			break;
		default:
			value = input;
			break;
	}
	return value;
}

/**
 element is the device element ID and corresponds to a button or pad
 value is the value of the specified device element
 */
void ProjectionManagerOSCProxy::handleInput(int element, float value){
	
	// Check enabled toggle
	if (element == 20 && value == 1) {
		enabled = !enabled;
		projectionManager->enableEditing(enabled);
	}
	if (!enabled) return;
	
	switch (element) {
			
			
			// Misc //////////////////////////////////////////////////
		case 14:
			// Save Gride
			if (value == 1) projectionManager->output->doSaveGrid();
			break;
			
			
			// Dpad //////////////////////////////////////////////////
		case 10:
			// dpad up
			if (value == 1) projectionManager->leftUp(1);
			break;
		case 11:
			// dpad down
			if (value == 1) projectionManager->leftDown(1);
			break;
		case 12:
			// dpad left
			if (value == 1) projectionManager->leftLeft(1);
			break;
		case 13:
			// dpad right
			if (value == 1) projectionManager->leftRight(1);
			break;
			
			
			// XBYA Buttons //////////////////////////////////////////
		case 22:
			// B
			if (value == 1) projectionManager->rightRight(1);
			break;
		case 23:
			// X
			if (value == 1) projectionManager->rightLeft(1);
			break;
		case 24:
			// Y
			if (value == 1) projectionManager->rightUp(1);
			break;
		case 21:
			// A
			if (value == 1) projectionManager->rightDown(1);
			break;
			
			
			// Shoulders /////////////////////////////////////////////
		case 18:
			// left front shoulder
			if (value == 1) projectionManager->previousMode();
			break;
		case 19:
			// right front shoulder
			if (value == 1) projectionManager->nextMode();
			break;
		case 25:
			// left back shoulder
			if(value > 250) projectionManager->previousMode();
			break;
		case 26:
			// right back shoulder
			if(value > 250) projectionManager->nextMode();
			break;
			
			
			// Left Analogue /////////////////////////////////////////
		case 27:
			// left analogue horizontal
			leftAnalogX = value;
			break;
		case 28:
			// left analogue vertical
			leftAnalogY = value;
			break;
			
			
			// Right Analogue /////////////////////////////////////////
		case 29:
			// right analogue horizontal
			rightAnalogX = value;
			break;
		case 30:
			// right analogue vertical
			rightAnalogY = value;
			break;
			
		default:
			break;
	}
}
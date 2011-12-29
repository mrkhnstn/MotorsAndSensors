#pragma once
/*
 *  ofxOscObjectsTest.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 18/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "ofxOscManager.h"
#include "ofxOscSendValueFloat.h"
#include "ofxOscReceiveValueFloat.h"
#include "Singleton.h"

#define OSC_SEND_PORT		3333
#define OSC_RECEIVE_PORT	3333
#define OSC_ADDRESS_TEST_FLOAT	"/testFloat"

class ofxOscObjectsTest : public ofxBaseObject, public ofxOscHandler{

public:

	ofxOscSender oscSender;
	ofxOscSendValueFloat	oscSendValue;
	ofxOscReceiveValueFloat oscReceiveValue;
	
	float floatValue;
	float receivedFloatValue;
	float intValue;
	
	ofxOscObjectsTest() : ofxBaseObject(){
		name = "ofxOscObjectsTest";
		floatValue = 0;
	}
	
	~ofxOscObjectsTest(){
		// delete ofxOscManager on application exit (only once obviously)
		ofxOscManager* oscManager = Singleton<ofxOscManager>::instance();
		DeleteIfNotNull(oscManager);
	}
	
	void setupGUI(){
		gui.page(1).addPageShortcut(gui.addPage(name));
		gui.addSlider("sendFloat", floatValue, 0,255);
		gui.addDebug("receivedFloat", receivedFloatValue);
		
	}
	
	void setup(){
		
		// access ofxOscManager via Singleton
		ofxOscManager* oscManager = Singleton<ofxOscManager>::instance();
		
		// setup ofxOscManager (only once at startup)
		oscManager->port = OSC_RECEIVE_PORT; //< set port before setup
		oscManager->setup();
		
		// register an osc address with the oscManager
		// see handleOscMessage method
		oscManager->registerInterest(*this, OSC_ADDRESS_TEST_FLOAT);
		
		oscSender.setup("127.0.0.1", OSC_SEND_PORT);
		
		// oscManager.addFloatToSend(floatValue, "/name", oscSender);
		// oscManager.addFloatToReceive(floatValue, "/name");
		
		oscSendValue.setup(floatValue, OSC_ADDRESS_TEST_FLOAT, oscSender);
		ofAddListener(ofEvents.update, (ofxOscSendValueBase<float>*)(&oscSendValue), &ofxOscSendValueBase<float>::_update); //< very convoluted way of adding update listener due to templated inheritance
		
		oscReceiveValue.setup(receivedFloatValue,OSC_ADDRESS_TEST_FLOAT,*oscManager);
		
	}
	
	void update(){
		//oscSendValue.update();
	}
	
	void sendFloatValue(float f){
		ofxOscMessage m;
		m.setAddress(OSC_ADDRESS_TEST_FLOAT);
		m.addFloatArg(floatValue);
		oscSender.sendMessage(m);
	}
	
	// override handleOscMessage
	void handleOscMessage(ofxOscMessage& message){
		string address = message.getAddress();
		if (address == OSC_ADDRESS_TEST_FLOAT) { //< cannot use switch heres
				float receivedFloat = castOscNumberToFloat(message,0); //<castOscNumberToFloat is inherited from handler
				ofLog(OF_LOG_VERBOSE, "received float: " + ofToString(receivedFloat));
		}
	}
	
	
};
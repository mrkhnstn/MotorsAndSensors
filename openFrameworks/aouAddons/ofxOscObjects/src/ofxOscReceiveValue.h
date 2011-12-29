#pragma once
/*
 *  ofxOscReceiveValue.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 18/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxOscManager.h"

class ofxOscReceiveValue : public ofxOscHandler{
	
public:
	
	float*	val;
	string	oscAddress;
	
	ofxOscReceiveValue(){
		val = NULL;
		oscAddress = "/NULL";
	}
	
	ofxOscReceiveValue(float& f, string oscAddress, ofxOscManager& oscManager){
		setup(f, oscAddress, oscManager);
	}

	/**
	 this does not remove previous oscManager interests
	 */
	void setup(float& f, string oscAddress, ofxOscManager& oscManager){
		val = &f;
		this->oscAddress = oscAddress;
		oscManager.registerInterest(*this, oscAddress);
	}
	
	// override handleOscMessage
	void handleOscMessage(ofxOscMessage& message){
		*val = castOscNumberToFloat(message,0); 
	}
};

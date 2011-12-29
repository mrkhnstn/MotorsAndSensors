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

template<class T> class ofxOscReceiveValueBase : public ofxOscHandler{
	
public:
	
	T*	val;
	string	oscAddress;
	
	ofxOscReceiveValueBase(){
		val = NULL;
		oscAddress = "/NULL";
	}
	
	ofxOscReceiveValueBase(T& val, string oscAddress, ofxOscManager& oscManager){
		setup(val, oscAddress, oscManager);
	}
	
	/**
	 this does not remove previous oscManager interests
	 */
	void setup(T& val, string oscAddress, ofxOscManager& oscManager){
		this->val = &val;
		this->oscAddress = oscAddress;
		oscManager.registerInterest(*this, oscAddress);
	}
	
	// override handleOscMessage
	virtual void handleOscMessage(ofxOscMessage& message){
		//*val = castOscNumberToFloat(message,0); 
	}
};

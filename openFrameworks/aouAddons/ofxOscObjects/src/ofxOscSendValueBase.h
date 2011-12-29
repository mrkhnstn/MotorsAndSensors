#pragma once
/*
 *  ofxOscSendValueBase.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 18/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxOsc.h"

template<class T> class ofxOscSendValueBase {

public:
	
	T*	val;
	T	oldVal;
	string	oscAddress;
	vector<ofxOscSender*> oscSender;
	
	ofxOscSendValueBase(){
		val = NULL;
		oldVal = 0;
		oscAddress = "/NULL";
	}
	
	ofxOscSendValueBase(T& val, string oscAddress, ofxOscSender& oscSender){
		setup(val, oscAddress, oscSender);
	}
	
	void setup(T& val, string oscAddress){
		this->val = &val;
		oldVal = val;
		this->oscAddress = oscAddress;
	}
	
	
	void setup(T& val, string oscAddress, ofxOscSender& oscSender){
		this->val = &val;
		oldVal = val;
		this->oscAddress = oscAddress;
		addSender(oscSender);
	}
	
	void addSender(ofxOscSender& oscSender){
		this->oscSender.push_back(&oscSender);
	}
	
	
	void _update(ofEventArgs& e){
		update();
	}
	
	void update(){
		if(val != NULL) //< making sure it is initialized
			if (*val != oldVal) {
				send();
				oldVal = *val;
			}
	}
	
	virtual void send(){
		ofxOscMessage m;
		m.setAddress(oscAddress);
		sendMessage(m);
	}
	
	void sendMessage(ofxOscMessage& m){
		for (vector<ofxOscSender*>::iterator it = oscSender.begin(); it != oscSender.end(); ++it) {
			(**it).sendMessage(m);
		}
	}
	
};
#pragma once
/*
 *  ofxOscSendValueFloat.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 19/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxOscSendValueBase.h"

class ofxOscSendValueFloat : public ofxOscSendValueBase<float> {
	
public:
	
	void send(){
		ofxOscMessage m;
		m.setAddress(oscAddress);
		m.addFloatArg(*val);
		sendMessage(m);
	}
	
};

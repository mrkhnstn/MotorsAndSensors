#pragma once
/*
 *  ofxOscReceiveValueFloat.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 19/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxOscReceiveValueBase.h"

class ofxOscReceiveValueFloat : public ofxOscReceiveValueBase<float> {

	// override handleOscMessage
	virtual void handleOscMessage(ofxOscMessage& message){
		*val = castOscNumberToFloat(message,0); 
	}
	
};


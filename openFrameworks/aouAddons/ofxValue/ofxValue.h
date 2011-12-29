#pragma once

/*
 *  ofxValue.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 28/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"

/**
 abstract object that throws an event when it changes
 see ofxValueT for templated version
*/
class ofxValue{
	
public:
	
	ofEvent<ofEventArgs> onChanged;
	ofEventArgs e;
		
	void changed(){
		ofNotifyEvent(onChanged, e, this);
	}
	
};
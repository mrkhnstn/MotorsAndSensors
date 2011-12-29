#pragma once
/*
 *  ofxOscHandler.h
 *  remoteactors
 *
 *  Created by Robin Beitra (robin@beitra.net) on 31/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxOsc.h"
#include "VectorMap.h"
#include "Utils.h" //< included for tokenize

class ofxOscHandler{

public:
		
	/**
	classes implementing an osc handler linked to an osc manager 
	need to inherit from ofxOscHandler and override the handleOscMessage 
	 */
	virtual void handleOscMessage(ofxOscMessage& message);
	
	
	// helper functions
	
	static float castOscNumberToFloat(ofxOscMessage& message, int index);
	
	static int castOscNumberToInt(ofxOscMessage& message, int index);
	
	static bool isOscArgNumeric(ofxOscMessage& message, int index);
	
	static bool isOscArgString(ofxOscMessage& message, int index);
	
	static void splitOscAddress(ofxOscMessage& message, vector<string>& container);
	
};
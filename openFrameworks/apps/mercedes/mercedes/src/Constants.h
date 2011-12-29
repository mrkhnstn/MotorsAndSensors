#pragma once
/*
 *  Constants.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"

class Constants : public ofxBaseObject {
	
public:
	
	int numFins;
	int numSensors;
	float cylinderRadius;
	float finWidth;
	
	Constants(){
		numFins = 72;
		numSensors = 36;
		cylinderRadius = 300;
		finWidth = 20;
	}
	
};


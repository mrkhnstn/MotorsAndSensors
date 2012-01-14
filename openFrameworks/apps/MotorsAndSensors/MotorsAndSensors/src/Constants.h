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

//TODO: probably can do away with this class and integrate variables into the relevant class

class Constants : public ofxBaseObject {
	
public:
	
	int		numMotors;
	float	maxMotorSpeed; // time in seconds to rotate 60 (0 -> 1 = 0 degrees -> 180 degrees)
	float	maxAngleSpeedN; // max change in normalized angle per second
	int 	numSensors;
	float	cylinderRadius;
	float	motorWidth;
	
	Constants(){
		numMotors = 72;
		numSensors = 36;
		cylinderRadius = 300;
		motorWidth = 20;
	}
	
};


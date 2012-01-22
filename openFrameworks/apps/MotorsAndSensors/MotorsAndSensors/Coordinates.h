#pragma once
/*
 *  Coordinates.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 22/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

// use this class to convert from polar coordinate system to cartesian coordinate system and vice versa

#include "Includes.h"

class Coordinates {

public:
	
	static float cartMinX;
	static float cartMaxX;
	static float cartMinY;
	static float cartMaxY;
	
	static float polarMinRadius; // converts to cartMinX
	static float polarMaxRadius; // converts to cartMaxX
	
	static float polarMinAngle; // converts to cartMinX (usually 0)
	static float polarMaxAngle; // converts to cartMaxX (usually 360);
	
	static float minSensingZone;
	static float maxSensingZone;
	
	static ofxVec2f fromPolar(ofxVec2f vec); // convert from polar system
	static ofxVec2f toPolar(ofxVec2f vec); // convert to polar system
};

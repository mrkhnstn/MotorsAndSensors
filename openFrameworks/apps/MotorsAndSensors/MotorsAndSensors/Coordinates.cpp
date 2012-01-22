/*
 *  Coordinates.cpp
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 22/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Coordinates.h"

float Coordinates::cartMinX = 0;
float Coordinates::cartMaxX = 360;
float Coordinates::cartMinY = 0;
float Coordinates::cartMaxY = 256;

float Coordinates::polarMinRadius = 290; // converts to cartMinX
float Coordinates::polarMaxRadius = 450; // converts to cartMaxX

float Coordinates::polarMinAngle = 0; // converts to cartMinX (usually 0)
float Coordinates::polarMaxAngle = 360; // converts to cartMaxX (usually 360);

float Coordinates::minSensingZone = 300;
float Coordinates::maxSensingZone = 350;

ofxVec2f Coordinates::fromPolar(ofxVec2f vec){
	ofxVec2f xAxis;
	xAxis.x = 1;
	ofxVec2f revVec;
	
	float revAngle = xAxis.angle(vec);
	// contain revAngle within 0 and 360
	if(revAngle < 0)
		revAngle += 360;
	
	float revDist = vec.length();
	
	revVec.x = ofMap(revAngle,polarMinAngle,polarMaxAngle,cartMaxX,cartMinX);
	revVec.y = ofMap(revDist,polarMinRadius,polarMaxRadius,cartMinY,cartMaxY);
	
	return revVec;
}

ofxVec2f Coordinates::toPolar(ofxVec2f vec){
	ofxVec2f revVec;
	revVec.x = ofMap(vec.y,cartMinY,cartMaxY,polarMinRadius,polarMaxRadius);
	revVec.y = 0;
	revVec.rotate(ofMap(vec.x,cartMaxX,cartMinX,polarMinAngle,polarMaxAngle));
	return revVec;
}
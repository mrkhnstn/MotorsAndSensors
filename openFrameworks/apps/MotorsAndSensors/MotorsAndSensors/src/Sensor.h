#pragma once
/*
 *  Sensor.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 29/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"

class Sensor : public ofxBaseObject {

public:
	
	int			index;
	ofxVec2f	pos;
	ofxVec2f	cartPos;
	float		posAngle;
	
	int		bank;
	int		ch;
	
	int*	rawValues[5];
	float*	distanceValues[5];
	ofxVec2f	absRayPos[5];
	float		rayCenterDist[5];
	bool		inSensorZone[5];
	ofxVec2f	cartRayPos[5]; // cartesian ray position used for cv
	bool*		rayEnabled[5];
	
	static bool doDraw;
	static bool doDrawRays;
	static bool doDrawHitPoints;
	static bool doDrawLabels;
	static bool drawOnlyInSensorZone;
	static bool doDrawSensorThreshold;

	static int numSensors;
	static float distanceToCentre;
	
	static bool doStaticInit;
	static ofTrueTypeFont font;
	
	static float userInProximityDistance; // if a sensor distance reading is smaller then userInProximityDistance then a user is considered to be in proximity
	static float angleBetweenRays;
	
	void setup();
	void update();
	void postGUI();
	void draw();
	bool userInProximity();
	
	
	
protected:
	
	bool _userInProximity;
};